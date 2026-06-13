/*
 * cli.c - Blockchain CLI Wallet Application
 *
 * This program provides a command-line interface for interacting with
 * a blockchain. It allows users to create transactions, maintain a
 * transaction pool, and mine blocks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* Project headers */
#include "../blockchain/v0.2/blockchain.h"

#define BLOCKCHAIN_FILENAME "blockchain.hblk"
#define POOL_SIZE 10
#define CMD_MAX 256

/**
 * struct transaction_pool_s - Transaction pool structure
 * @transactions: Array of transaction data
 * @data_len: Array of transaction data lengths
 * @count: Number of transactions in pool
 */
typedef struct transaction_pool_s
{
	int8_t transactions[POOL_SIZE][BLOCKCHAIN_DATA_MAX];
	uint32_t data_len[POOL_SIZE];
	uint32_t count;
} transaction_pool_t;

/**
 * print_prompt - Displays the CLI prompt
 */
void print_prompt(void)
{
	printf("hblk_wallet$ ");
	fflush(stdout);
}

/**
 * print_help - Displays available commands
 */
void print_help(void)
{
	printf("\n--- Available Commands ---\n");
	printf("info              - Display blockchain information\n");
	printf("add <data>        - Add transaction to pool\n");
	printf("pool              - Display transaction pool\n");
	printf("mine              - Mine new block with pool transactions\n");
	printf("load              - Load blockchain from file\n");
	printf("save              - Save blockchain to file\n");
	printf("help              - Show this help message\n");
	printf("exit/quit         - Exit the application\n");
	printf("\n");
}

/**
 * print_blockchain_info - Prints blockchain information
 * @blockchain: Pointer to the blockchain structure
 */
void print_blockchain_info(blockchain_t const *blockchain)
{
	if (!blockchain || !blockchain->chain)
	{
		printf("Blockchain is empty or not initialized.\n");
		return;
	}

	printf("\n--- Blockchain Info ---\n");
	_blockchain_print(blockchain);
}

/**
 * print_transaction_pool - Displays all transactions in pool
 * @pool: Pointer to the transaction pool
 */
void print_transaction_pool(transaction_pool_t const *pool)
{
	uint32_t i;

	printf("\n--- Transaction Pool (%u transactions) ---\n", pool->count);
	if (pool->count == 0)
	{
		printf("Pool is empty.\n");
		return;
	}

	for (i = 0; i < pool->count; i++)
	{
		printf("[%u] Data: %.32s (len: %u)\n", i,
			   pool->transactions[i], pool->data_len[i]);
	}
	printf("\n");
}

/**
 * add_transaction - Add transaction to pool
 * @pool: Pointer to the transaction pool
 * @data: Transaction data
 *
 * Return: 0 on success, -1 if pool is full
 */
int add_transaction(transaction_pool_t *pool, const char *data)
{
	uint32_t len;

	if (pool->count >= POOL_SIZE)
	{
		printf("Transaction pool is full (%u transactions max).\n", POOL_SIZE);
		return (-1);
	}

	len = strlen(data);
	if (len > BLOCKCHAIN_DATA_MAX)
	{
		printf("Transaction data too long (max %d bytes).\n",
			   BLOCKCHAIN_DATA_MAX);
		return (-1);
	}

	memcpy(pool->transactions[pool->count], data, len);
	pool->data_len[pool->count] = len;
	pool->count++;

	printf("Transaction added to pool. (Total: %u)\n", pool->count);
	return (0);
}

/**
 * mine_block_cmd - Mine a new block using pool transactions
 * @blockchain: Pointer to the blockchain
 * @pool: Pointer to the transaction pool
 *
 * Return: 0 on success, -1 on error
 */
int mine_block_cmd(blockchain_t *blockchain, transaction_pool_t *pool)
{
	block_t *block;
	block_t *prev_block;
	uint32_t i;
	int8_t combined_data[BLOCKCHAIN_DATA_MAX];
	uint32_t total_len = 0;

	if (pool->count == 0)
	{
		printf("Transaction pool is empty. Nothing to mine.\n");
		return (-1);
	}

	/* Combine all transactions into one data block */
	for (i = 0; i < pool->count && total_len < BLOCKCHAIN_DATA_MAX; i++)
	{
		if (total_len + pool->data_len[i] > BLOCKCHAIN_DATA_MAX)
			break;
		memcpy(&combined_data[total_len], pool->transactions[i],
			   pool->data_len[i]);
		total_len += pool->data_len[i];
	}

	/* Get previous block from blockchain */
	if (blockchain->chain && blockchain->chain->head)
		prev_block = (block_t *)blockchain->chain->head;
	else
		prev_block = NULL;

	/* Create and mine new block */
	block = block_create(prev_block, combined_data, total_len);
	if (!block)
	{
		printf("Failed to create block.\n");
		return (-1);
	}

	/* Mine the block */
	block_mine(block);

	printf("Block mined successfully!\n");
	printf("Block hash: ");
	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", block->hash[i]);
	printf("\n");

	/* Clear transaction pool */
	pool->count = 0;
	printf("Transaction pool cleared.\n");

	free(block);
	return (0);
}

/**
 * handle_command - Processes the user input commands
 * @cmd: The command string entered by the user
 * @blockchain: Pointer to the blockchain structure
 * @pool: Pointer to the transaction pool
 *
 * Return: 1 if the loop should continue, 0 if exit
 */
int handle_command(char *cmd, blockchain_t *blockchain,
				   transaction_pool_t *pool)
{
	char *token;
	char cmd_copy[CMD_MAX];

	/* Strip newline character */
	cmd[strcspn(cmd, "\n")] = 0;

	if (strlen(cmd) == 0)
		return (1);

	strncpy(cmd_copy, cmd, CMD_MAX - 1);
	cmd_copy[CMD_MAX - 1] = '\0';

	token = strtok(cmd_copy, " ");

	if (!token)
		return (1);

	if (strcmp(token, "exit") == 0 || strcmp(token, "quit") == 0)
		return (0);

	if (strcmp(token, "help") == 0)
	{
		print_help();
		return (1);
	}

	if (strcmp(token, "info") == 0)
	{
		print_blockchain_info(blockchain);
		return (1);
	}

	if (strcmp(token, "pool") == 0)
	{
		print_transaction_pool(pool);
		return (1);
	}

	if (strcmp(token, "add") == 0)
	{
		token = strtok(NULL, "");
		if (token)
			add_transaction(pool, token);
		else
			printf("Usage: add <transaction_data>\n");
		return (1);
	}

	if (strcmp(token, "mine") == 0)
	{
		mine_block_cmd(blockchain, pool);
		return (1);
	}

	if (strcmp(token, "save") == 0)
	{
		if (blockchain_serialize(blockchain, BLOCKCHAIN_FILENAME) == 0)
			printf("Blockchain saved to %s\n", BLOCKCHAIN_FILENAME);
		else
			printf("Failed to save blockchain.\n");
		return (1);
	}

	if (strcmp(token, "load") == 0)
	{
		blockchain_t *loaded;

		if (blockchain->chain)
			blockchain_destroy(blockchain);

		loaded = blockchain_deserialize(BLOCKCHAIN_FILENAME);
		if (loaded)
		{
			*blockchain = *loaded;
			printf("Blockchain loaded from %s\n", BLOCKCHAIN_FILENAME);
		}
		else
			printf("Failed to load blockchain.\n");
		return (1);
	}

	printf("Unknown command: %s\n", cmd);
	printf("Type 'help' for available commands.\n");

	return (1);
}

/**
 * main - Entry point for the Blockchain CLI Wallet
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int running = 1;
	blockchain_t *blockchain;
	transaction_pool_t pool;

	/* Initialize blockchain */
	blockchain = blockchain_create();
	if (!blockchain)
	{
		fprintf(stderr, "Failed to create blockchain.\n");
		return (EXIT_FAILURE);
	}

	/* Initialize transaction pool */
	memset(&pool, 0, sizeof(transaction_pool_t));

	printf("\n");
	printf("╔════════════════════════════════════════════════════════════╗\n");
	printf("║        Holberton/ALX Blockchain CLI Wallet v0.1           ║\n");
	printf("║                  Type 'help' for commands                 ║\n");
	printf("╚════════════════════════════════════════════════════════════╝\n");
	printf("\n");

	while (running)
	{
		print_prompt();
		read = getline(&line, &len, stdin);

		if (read == -1) /* Handle EOF (Ctrl+D) */
		{
			printf("\nExiting...\n");
			break;
		}

		running = handle_command(line, blockchain, &pool);
	}

	free(line);
	blockchain_destroy(blockchain);

	return (EXIT_SUCCESS);
}
