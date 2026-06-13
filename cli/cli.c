/* Project headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../blockchain/v0.2/blockchain.h"
#include "../crypto/hblk_crypto.h"

/**
 * print_prompt - Displays the CLI prompt
 */
void print_prompt(void)
{
	printf("hblk_wallet$ ");
	fflush(stdout);
}

/**
 * handle_command - Processes the user input commands
 * @cmd: The command string entered by the user
 * @chain: Pointer to the blockchain structure
 *
 * Return: 1 if the loop should continue, 0 if exit
 */
int handle_command(char *cmd, void *chain)
{
	/* Strip newline character */
	cmd[strcspn(cmd, "\n")] = 0;

	if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0)
		return (0);

	if (strcmp(cmd, "info") == 0)
	{
		printf("Blockchain info:\n");
		/* Call your library function to print the chain */
		/* llist_print(chain->blocks); */
		(void)chain; /* Silence unused var warning for now */
	}
	else if (strcmp(cmd, "mine") == 0)
	{
		printf("Mining new block with transactions in pool...\n");
		/* Call your library function to mine a block */
		/* mine_block(chain); */
	}
	else if (strlen(cmd) > 0)
	{
		printf("Unknown command: %s\n", cmd);
		printf("Available commands: info, mine, exit\n");
	}

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
	void *chain = NULL;

	/* Initialize your blockchain library here */
	/* chain = blockchain_create(); */
	printf("--- Holberton/ALX Blockchain CLI Initialized ---\n");
	printf("Type 'info' to view chain, 'mine' to process pool, 'exit' to quit.\n");

	while (running)
	{
		print_prompt();
		read = getline(&line, &len, stdin);

		if (read == -1) /* Handle EOF (Ctrl+D) */
		{
			printf("\n");
			break;
		}

		running = handle_command(line, chain);
	}

	free(line);
	/* Clean up your blockchain library here to prevent memory leaks */
	/* blockchain_destroy(chain); */

	return (EXIT_SUCCESS);
}