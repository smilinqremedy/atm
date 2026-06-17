# Enterprise ATM Management System (C)

A robust, production-ready Automated Teller Machine (ATM) management system engineered in C. This project demonstrates modular software architecture, low-level file stream manipulation, structural data parsing, and defensive programming constraints (such as preventing updates or transactions on fixed accounts).

## 🚀 Features Implemented

- **User Authentication Platform**: Secure multi-user login and registration framework with case-insensitive unique username enforcement.
- **Account Creation Suite**: Dynamic generation of financial accounts tracking creation dates, geographic metadata, custom tracking IDs, and target account types.
- **Transactional Consistency (Deposit/Withdrawal)**: Live balance mutations using double-precision float tracking. Overdraft prevention checks for safe ledger balancing.
- **Dynamic Interest Engine**: Computes and displays structured maturity forecasting matching specific account restrictions:
  - `savings`: 7% interest rate computed and tracked monthly.
  - `fixed01` / `fixed02` / `fixed03`: Tiered interest rates (4%, 5%, 8%) triggered upon reaching multi-year maturity dates.
  - `current`: Non-interest bearing transactional asset tracking.
- **Administrative Account Management**: Granular control matrices allowing account data updates (Country, Phone) or complete deletion of account paths.
- **Asset Ownership Transfer**: Relational data-link mutations mapping an existing account profile over to an entirely separate registered system user.

---

## 🏗️ Architecture & Data Design

The system avoids a monolithic code structure by segregating logic cleanly into a layered configuration:

* **UI Layer (`main.c`)**: Manages the terminal text interface loop, processes standard user input blocks, and guides application routing.
* **Business Layer (`system.c`, `auth.c`)**: Enforces validation matrices, account transaction limits, and financial calculations.
* **Data Layer (`data/`)**: Acts as a persistent local flat-file storage cluster.

### Shadow-File Swap Pattern (ACID-like Persistence)
To protect data records from corruption during file writes, power loss, or mid-transaction crashes, mutations use a **Shadow-File-Swap** mechanism:
1. The system opens the primary file (`records.txt`) alongside an isolated temporary file (`records.tmp`).
2. Valid unmutated records stream into the temporary destination sequentially.
3. Target modifications occur safely in-memory before writing the mutation out.
4. The system executes an atomic file system override (`remove` and `rename`), keeping data secure and consistent.

---

## 📁 Repository Directory Structure

```text
.
├── Makefile            # Project compilation automation engine
├── data/               # Persistent database registry tracking text blocks
│   ├── records.txt     # Account ledger rows
│   └── users.txt       # Registered user identity mappings
└── src/                # Modular Source Code Directory
    ├── auth.c          # Registration and login verification mechanics
    ├── header.h        # Unified structure layouts and functional prototypes
    ├── main.c          # Primary loop lifecycle and entry portal
    └── system.c        # Transaction processing, mutations, and interests
