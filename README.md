The Chitoshi Online Banking System is a C-based CLI (Command-Line Interface) application designed f>

## Key Features

1. Customer Registration
    - Users can register for a bank account by providing full name, username, password, email, and >
    - Accounts are automatically assigned ordered account numbers.

2. Secure Login
    - Customers log in using Account Number, Email and Password.
    - Admin can log in with special credentials to manage the system.

3. Admin Panel
    - Admin can view all registered customers and every transaction performed in the system.
    - Admin has access to customer details including Name, Username, Email, Phone, and Balance.

4. Account Management
    - Customers can view personal info including Full Name, Username, Email, Phone, Account Number,>

5. Deposits, Withdrawals, and Transfers
    - Customers can deposit, withdraw, and transfer money to other accounts.
    - Each transaction generates a unique reference number, displayed to the customer.
6. Transaction History
    - Customers can review their full transaction history.
    - Admin can view all transactions for all accounts.

7. Password Management
    - Customers can change their password securely after authentication.

8. Ordered Account Numbering
    - Every new account is given an incremented, easily trackable 8-digit account number.

9. Automated Reference Numbers
    - All transactions receive an automatically generated reference number for verification and sup>

10. Data Persistence
    - Accounts and transactions are stored in text files (`accounts.txt` and `transactions.txt`) to>
## Instructions

1. Compile the program:

   gcc -o banking banking.c

2. Run the program:

   ./banking


## File Structure

- `banking.c` — main source code
- `accounts.txt` — stores account details
- `transactions.txt` — stores all transactions
- `README.md` — project documentation

---

## Example Admin Credentials

- Username: `admin`
- Password: `admin123`

---

## Notes

- All data is stored in local text files; be sure to keep them secure!





