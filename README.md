# Blockchain CLI Wallet

A command-line interface for interacting with a blockchain. This project demonstrates blockchain fundamentals including block creation, mining, transaction management, and persistence.

## Project Overview

This is an ALX/Holberton School project that implements a blockchain CLI wallet built on top of:
- **Blockchain library** (blockchain/v0.3): Handles block creation, mining, and chain validation
- **Crypto library** (crypto): Provides cryptographic operations including SHA256 hashing and EC signing

## Features

- **Blockchain Creation & Management**: Create and manage a blockchain with genesis block
- **Transaction Pool**: Maintain a pool of pending transactions (max 10 transactions)
- **Mining**: Mine new blocks with transactions from the pool
- **Persistence**: Save and load blockchain to/from disk files
- **Chain Validation**: Automatic validation of blocks and chain integrity
- **Proof of Work**: Difficulty-based mining system

## Building the Project

### Requirements

- GCC 4.8.4 or later
- OpenSSL development libraries (`libssl-dev` on Ubuntu)
- C libraries and standard development tools

### Compilation

```bash
cd cli
make
```

### Cleaning Build Artifacts

```bash
cd cli
make clean      # Remove object files
make fclean     # Remove all generated files including executable
make re         # Clean and rebuild
```

## Usage

### Running the Wallet

```bash
./cli
```

### Available Commands

| Command | Description |
|---------|-------------|
| `help` | Display all available commands |
| `info` | Display blockchain information and current state |
| `add <data>` | Add a transaction to the transaction pool |
| `pool` | Display all pending transactions in the pool |
| `mine` | Mine a new block using transactions from the pool |
| `save` | Save the blockchain to `blockchain.hblk` file |
| `load` | Load a blockchain from `blockchain.hblk` file |
| `exit` or `quit` | Exit the application |

### Example Session

```bash
$ ./cli

╔════════════════════════════════════════════════════════════╗
║        Holberton/ALX Blockchain CLI Wallet v0.1           ║
║                  Type 'help' for commands                 ║
╚════════════════════════════════════════════════════════════╝

hblk_wallet$ add Alice sent 5 coins to Bob
Transaction added to pool. (Total: 1)

hblk_wallet$ add Bob sent 3 coins to Charlie
Transaction added to pool. (Total: 2)

hblk_wallet$ pool

--- Transaction Pool (2 transactions) ---
[0] Data: Alice sent 5 coins to Bob (len: 27)
[1] Data: Bob sent 3 coins to Charlie (len: 28)

hblk_wallet$ mine
Block mined successfully!
Block hash: a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6q7r8s9t0u1v2w3x4y5z6a7b8c9d0e1f2
Transaction pool cleared.

hblk_wallet$ info

--- Blockchain Info ---
[...]

hblk_wallet$ save
Blockchain saved to blockchain.hblk

hblk_wallet$ exit
Exiting...
```

## Project Structure

```
.
├── blockchain/
│   ├── v0.2/
│   │   └── blockchain.h         # Blockchain API header
│   └── v0.3/
│       ├── libhblk_blockchain.a # Compiled blockchain library
│       ├── block_*.c            # Block implementation files
│       ├── blockchain_*.c       # Blockchain implementation files
│       └── provided/            # Helper functions
├── crypto/
│   ├── libhblk_crypto.a         # Compiled crypto library
│   ├── sha256.c                 # SHA256 hashing
│   ├── ec_*.c                   # Elliptic curve operations
│   └── hblk_crypto.h            # Crypto API header
├── cli/
│   ├── Makefile                 # Build configuration
│   └── cli.c                    # CLI wallet implementation
└── README.md                    # This file
```

## Implementation Details

### Transaction Pool Structure

```c
typedef struct transaction_pool_s
{
    int8_t transactions[POOL_SIZE][BLOCKCHAIN_DATA_MAX];  // Max 10 transactions
    uint32_t data_len[POOL_SIZE];                         // Length of each transaction
    uint32_t count;                                       // Number of transactions
} transaction_pool_t;
```

### Mining Process

1. Collect all transactions from the pool
2. Combine them into a single data block
3. Create a new block with the combined data
4. Apply proof-of-work mining algorithm
5. Add block to blockchain (automatically linked to previous block)
6. Clear transaction pool

### Persistence

- **Save**: `blockchain_serialize()` writes entire blockchain to a binary file
- **Load**: `blockchain_deserialize()` reads blockchain from file back into memory

## Code Quality

- **Betty Compliant**: All code follows the Betty style guide
- **Memory Safe**: Proper allocation and deallocation of resources
- **Error Handling**: Comprehensive error checking and validation
- **Documentation**: Full function documentation with Betty-style comments

## Compilation Flags

```
-Wall -Werror -Wextra -pedantic    # Strict warning levels
-lssl -lcrypto                     # OpenSSL libraries for cryptography
```

## Limitations & Notes

- Transaction pool limited to 10 transactions max
- Each transaction data limited to 1024 bytes max
- Blockchain file format: Binary file with HBLK magic bytes header
- Mining uses difficulty adjustment based on block history

## Author

- **Project**: ALX/Holberton School
- **Blockchain Technology**: Educational implementation of core blockchain concepts

## License

This is an educational project created as part of the ALX/Holberton School curriculum.
