#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ACCOUNTS 100

struct Account {
    char username[50];
    char password[50];
    char email[60];
    char fullName[100];
    char accountNumber[20];
    float balance;
    char phone[15];
};

struct Transaction {
    char accountNumber[20];
    char type[20];
    float amount;
    char otherParty[20];
    char date[30];
    float resultingBalance;
    char reference[50];
};

struct Account accounts[MAX_ACCOUNTS];
int accountCount = 0;
int nextAccountNumber = 1;
int nextReferenceNumber = 100001;
struct Account *currentUser = NULL;
int isLoggedIn = 0;
int isAdmin = 0;

void clearInputBuffer();
void saveAccounts();
void loadAccounts();
void saveTransaction(struct Transaction *t);
void displayTransactionHistory(char *accountNumber);
void displayAllTransactions();
void registerAccount();
int login(int mode);
void mainMenu();
void adminMenu();
void depositFunds();
void withdrawFunds();
void transferFunds();
void changePassword();
void checkBalance();
void displayAccountInfo();
int validatePassword(char *password);
void getTimeString(char *dest);
void displayAllAccounts();

void getTimeString(char *dest) {
    time_t t;
    struct tm *tm_struct;
    t = time(NULL);
    tm_struct = localtime(&t);
    sprintf(dest, "%04d-%02d-%02d %02d:%02d:%02d",
            tm_struct->tm_year + 1900, tm_struct->tm_mon + 1, tm_struct->tm_mday,
            tm_struct->tm_hour, tm_struct->tm_min, tm_struct->tm_sec);
}

void loadAccounts() {
    FILE *fp;
    struct Account acc;
    accountCount = 0;
    fp = fopen("accounts.txt", "r");
    if (!fp) return;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%[^,\n]\n",
                  acc.fullName, acc.username, acc.password, acc.email, acc.accountNumber,
                  &acc.balance, acc.phone) == 7) {
        accounts[accountCount] = acc;
        accountCount++;
        if (accountCount >= MAX_ACCOUNTS) break;
    }
    fclose(fp);

    // Set nextAccountNumber based on highest account number assigned
    if (accountCount == 0) {
        nextAccountNumber = 1;
    } else {
        int maxAccount = 0, tempAccount;
        for (int i = 0; i < accountCount; i++) {
            tempAccount = atoi(accounts[i].accountNumber);
            if (tempAccount > maxAccount) maxAccount = tempAccount;
        }
        nextAccountNumber = maxAccount + 1;
    }
}

void saveAccounts() {
    FILE *fp;
    int i;
    fp = fopen("accounts.txt", "w");
    for (i = 0; i < accountCount; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s,%.2f,%s\n",
                accounts[i].fullName, accounts[i].username, accounts[i].password, accounts[i].email,
                accounts[i].accountNumber, accounts[i].balance, accounts[i].phone);
    }
    fclose(fp);
}

void saveTransaction(struct Transaction *t) {
    FILE *fp;
    fp = fopen("transactions.txt", "a");
    fprintf(fp, "%s,%s,%.2f,%s,%s,%.2f,%s\n",
            t->accountNumber, t->type, t->amount,
            t->otherParty, t->date, t->resultingBalance, t->reference);
    fclose(fp);
}

void displayTransactionHistory(char *accountNumber) {
    FILE *fp;
    char line[300];
    struct Transaction t;
    int found = 0;
    fp = fopen("transactions.txt", "r");
    if (!fp) {
        printf("No transaction history.\n");
        return;
    }
    printf("\n=== Transaction History ===\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%f,%[^,],%[^,],%f,%[^,\n]",
               t.accountNumber, t.type, &t.amount, t.otherParty, t.date, &t.resultingBalance, t.reference);
        if (strcmp(accountNumber, t.accountNumber) == 0) {
            printf("%s | %-14s | %.2f | Ref: %-18s | Other: %-10s | Balance: %.2f\n",
                   t.date, t.type, t.amount,
                   (strlen(t.reference) > 0) ? t.reference : "-", (strlen(t.otherParty)>0)?t.otherParty:"-", t.resultingBalance);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) printf("No transactions found!\n");
}

void displayAllTransactions() {
    FILE *fp;
    char line[300];
    struct Transaction t;
    fp = fopen("transactions.txt", "r");
    if (!fp) {
        printf("No transactions present.\n");
        return;
    }
    printf("\n=== ALL TRANSACTIONS ===\n");
    printf("Date            | Account      | Type       | Amount   | Ref.                 | Other Party  | ResultBal\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%f,%[^,],%[^,],%f,%[^,\n]",
               t.accountNumber, t.type, &t.amount, t.otherParty, t.date, &t.resultingBalance, t.reference);
        printf("%s | %-12s | %-10s | %-8.2f | %-20s | %-12s | %.2f\n",
               t.date, t.accountNumber, t.type, t.amount,
               (strlen(t.reference) > 0) ? t.reference : "-",
               (strlen(t.otherParty) > 0) ? t.otherParty : "-", t.resultingBalance);
    }
    fclose(fp);
}

int validatePassword(char *password) {
    if (strlen(password) < 6) {
        printf("Password too short! Min 6 characters.\n");
        return 0;
    }
    return 1;
}

int login(int mode) {
    char accNum[25], password[55], email[60];
    int attempts = 0, found = -1, i;
    clearInputBuffer();
    if (mode == 2) {
        printf("\n=== ADMIN LOGIN ===\n");
        printf("Admin Username: ");
        fgets(accNum, 25, stdin); accNum[strlen(accNum)-1]='\0';
        printf("Admin Password: ");
        fgets(password, 55, stdin); password[strlen(password)-1]='\0';
        if (strcmp(accNum, "admin") == 0 && strcmp(password, "admin123") == 0) {
            isLoggedIn = 1;
            isAdmin = 1;
            printf("\nAdmin login successful!\n");
            return 2; // admin
        }
        printf("Admin login failed!\n");
        return 0;
    }
    printf("\n=== CUSTOMER LOGIN ===\n");
    while (attempts < 3) {
        printf("Account Number: ");
        fgets(accNum, 25, stdin);
        accNum[strlen(accNum)-1] = '\0';
        printf("Email: ");
        fgets(email, 60, stdin);
        email[strlen(email)-1] = '\0';
        printf("Password: ");
        fgets(password, 55, stdin);
        password[strlen(password)-1] = '\0';
        found = -1;
        for (i = 0; i < accountCount; i++) {
            if (strcmp(accNum, accounts[i].accountNumber) == 0
                && strcmp(password, accounts[i].password) == 0
                && strcmp(email, accounts[i].email) == 0) {
                found = i;
                break;
            }
        }
        if (found != -1) {
            currentUser = &accounts[found];
            isLoggedIn = 1; isAdmin = 0;
            printf("\nWelcome, %s!\n", currentUser->fullName);
            return 1;
        } else {
            attempts++;
            printf("Invalid! Attempts left: %d\n", 3 - attempts);
        }
    }
    printf("Too many failed attempts!\n");
    return 0;
}

void registerAccount() {
    char tempPassword[50], initialDepositStr[20];
    struct Account acc;
    int exists, i;
    float initialDeposit;
    struct Transaction t;
    printf("\n=== ACCOUNT REGISTRATION ===\n");
    clearInputBuffer();
    printf("Enter Full Name: ");
    fgets(acc.fullName, 100, stdin);
    acc.fullName[strlen(acc.fullName)-1] = '\0';
    // Ordered account number
    sprintf(acc.accountNumber, "%08d", nextAccountNumber++);
    printf("Enter Username: ");
    fgets(acc.username, 50, stdin);
    acc.username[strlen(acc.username)-1] = '\0';
    printf("Enter Email: ");
    fgets(acc.email, 60, stdin);
    acc.email[strlen(acc.email)-1] = '\0';
    for (i = 0; i < accountCount; i++) {
        if (strcmp(acc.username, accounts[i].username) == 0) {
            printf("Username already exists!\n");
            return;
        }
        if (strcmp(acc.email, accounts[i].email) == 0) {
            printf("Email already used!\n");
            return;
        }
    }
    do {
        printf("Enter Password (min 6 chars): ");
        fgets(tempPassword, 50, stdin);
        tempPassword[strlen(tempPassword)-1] = '\0';
    } while (!validatePassword(tempPassword));
    strcpy(acc.password, tempPassword);
    printf("Enter Phone Number: ");
    fgets(acc.phone, 15, stdin);
    acc.phone[strlen(acc.phone)-1] = '\0';
    do {
        printf("Enter initial deposit (>= 100.0): ");
        fgets(initialDepositStr, sizeof(initialDepositStr), stdin);
        initialDeposit = (float) atof(initialDepositStr);
        if (initialDeposit < 100.0) {
            printf("Invalid amount! Initial deposit should be at least 100.\n");
        }
    } while (initialDeposit < 100.0);
    acc.balance = initialDeposit;
    accounts[accountCount] = acc;
    accountCount++;
    saveAccounts();
    printf("\n=== REGISTRATION SUCCESSFUL ===\n");
    printf("Account Number: %s\nName: %s\nBalance: %.2f\nEmail: %s\n",
           acc.accountNumber, acc.fullName, acc.balance, acc.email);
    printf("Please remember your Account Number for login!\n");
    strcpy(t.accountNumber, acc.accountNumber);
    strcpy(t.type, "Initial Deposit");
    t.amount = initialDeposit;
    strcpy(t.otherParty, "");
    getTimeString(t.date);
    t.resultingBalance = acc.balance;
    // Generate reference number automatically
    sprintf(t.reference, "%d", nextReferenceNumber++);
    printf("Your Transaction Reference: %s\n", t.reference);
    saveTransaction(&t);
}

void depositFunds() {
    float amount;
    char amountStr[30];
    struct Transaction t;
    printf("\n=== DEPOSIT ===\n");
    printf("Current Balance: %.2f\n", currentUser->balance);
    printf("Enter amount: ");
    clearInputBuffer();
    fgets(amountStr, sizeof(amountStr), stdin);
    amount = (float) atof(amountStr);
    if (amount <= 0.0) {
        printf("Invalid amount!\n");
        return;
    }
    // Generate reference number automatically
    sprintf(t.reference, "%d", nextReferenceNumber++);
    currentUser->balance += amount;
    saveAccounts();
    printf("Success! New Balance: %.2f\n", currentUser->balance);
    printf("Your Transaction Reference: %s\n", t.reference);
    strcpy(t.accountNumber, currentUser->accountNumber);
    strcpy(t.type, "Deposit");
    t.amount = amount;
    strcpy(t.otherParty, "");
    getTimeString(t.date);
    t.resultingBalance = currentUser->balance;
    saveTransaction(&t);
}

void withdrawFunds() {
    float amount;
    char amountStr[30];
    struct Transaction t;
    printf("\n=== WITHDRAW ===\n");
    printf("Current Balance: %.2f\n", currentUser->balance);
    printf("Enter amount: ");
    clearInputBuffer();
    fgets(amountStr, sizeof(amountStr), stdin);
    amount = (float) atof(amountStr);
    if (amount <= 0.0) {
        printf("Invalid amount!\n");
        return;
    }
    if (amount > currentUser->balance) {
        printf("Insufficient funds!\n");
        return;
    }
    // Generate reference number automatically
    sprintf(t.reference, "%d", nextReferenceNumber++);
    currentUser->balance -= amount;
    saveAccounts();
    printf("Success! Remaining: %.2f\n", currentUser->balance);
    printf("Your Transaction Reference: %s\n", t.reference);
    strcpy(t.accountNumber, currentUser->accountNumber);
    strcpy(t.type, "Withdraw");
    t.amount = amount;
    strcpy(t.otherParty, "");
    getTimeString(t.date);
    t.resultingBalance = currentUser->balance;
    saveTransaction(&t);
}

void transferFunds() {
    float amount;
    char recipientAcc[25], amountStr[30];
    int found, i;
    struct Transaction t1, t2;
    clearInputBuffer();
    printf("\n=== TRANSFER ===\n");
    printf("Current Balance: %.2f\n", currentUser->balance);
    printf("Enter recipient Account Number: ");
    fgets(recipientAcc, 25, stdin);
    recipientAcc[strlen(recipientAcc)-1]='\0';
    if (strcmp(recipientAcc, currentUser->accountNumber) == 0) {
        printf("Cannot transfer to yourself!\n");
        return;
    }
    found = -1;
    for (i = 0; i < accountCount; i++) {
        if (strcmp(recipientAcc, accounts[i].accountNumber) == 0) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        printf("Recipient not found!\n");
        return;
    }
    printf("Enter amount: ");
    fgets(amountStr, sizeof(amountStr), stdin);
    amount = (float) atof(amountStr);
    if (amount <= 0.0) {
        printf("Invalid amount!\n");
        return;
    }
    if (amount > currentUser->balance) {
        printf("Insufficient funds!\n");
        return;
    }
    // Generate reference number automatically
    sprintf(t1.reference, "%d", nextReferenceNumber++);
    strcpy(t2.reference, t1.reference); // Same reference for both parties

    currentUser->balance -= amount;
    accounts[found].balance += amount;
    saveAccounts();
    printf("Transfer successful!\n");
    printf("Sent: %.2f to %s\n", amount, accounts[found].fullName);
    printf("Remaining: %.2f\n", currentUser->balance);
    printf("Your Transaction Reference: %s\n", t1.reference);
    strcpy(t1.accountNumber, currentUser->accountNumber);
    strcpy(t1.type, "Transfer Out");
    t1.amount = amount;
    strcpy(t1.otherParty, accounts[found].accountNumber);
    getTimeString(t1.date);
    t1.resultingBalance = currentUser->balance;
    saveTransaction(&t1);

    strcpy(t2.accountNumber, accounts[found].accountNumber);
    strcpy(t2.type, "Transfer In");
    t2.amount = amount;
    strcpy(t2.otherParty, currentUser->accountNumber);
    getTimeString(t2.date);
    t2.resultingBalance = accounts[found].balance;
    saveTransaction(&t2);
}

void changePassword() {
    char currentPass[55], newPass[55], confirmPass[55];
    int passOk;
    printf("\n=== CHANGE PASSWORD ===\n");
    clearInputBuffer();
    printf("Current password: ");
    fgets(currentPass, 55, stdin);
    currentPass[strlen(currentPass)-1] = '\0';
    if (strcmp(currentPass, currentUser->password) != 0) {
        printf("Wrong current password!\n");
        return;
    }
    passOk = 0;
    while (!passOk) {
        printf("New password (min 6): ");
        fgets(newPass, 55, stdin);
        newPass[strlen(newPass)-1] = '\0';
        if (!validatePassword(newPass)) {
            continue;
        }
        printf("Confirm new password: ");
        fgets(confirmPass, 55, stdin);
        confirmPass[strlen(confirmPass)-1] = '\0';
        if (strcmp(newPass, confirmPass) != 0) {
            printf("Passwords don't match!\n");
        } else {
            passOk = 1;
        }
    }
    strcpy(currentUser->password, newPass);
    saveAccounts();
    printf("Password changed!\n");
}

void checkBalance() {
    if (!isLoggedIn || currentUser == NULL) {
        printf("\nYou must log in to see your account balance!\n");
        return;
    }
    printf("\n=== BALANCE ===\n");
    printf("Account: %s\n", currentUser->accountNumber);
    printf("Holder: %s\n", currentUser->fullName);
    printf("Email: %s\n", currentUser->email);
    printf("Balance: %.2f\n", currentUser->balance);
}

void displayAccountInfo() {
    if (!isLoggedIn || currentUser == NULL) {
        printf("\nYou must log in to see account information!\n");
        return;
    }
    printf("\n=== ACCOUNT INFO ===\n");
    printf("Account Number: %s\n", currentUser->accountNumber);
    printf("Full Name: %s\n", currentUser->fullName);
    printf("Username: %s\n", currentUser->username);
    printf("Email: %s\n", currentUser->email);
    printf("Phone: %s\n", currentUser->phone);
    printf("Current Balance: %.2f\n", currentUser->balance);
}

void displayAllAccounts() {
    int i;
    printf("\n=== ALL REGISTERED ACCOUNTS ===\n");
    printf("AccNum    | Username       | Full Name           | Email                | Phone        | Balance\n");
    for(i = 0; i < accountCount; i++) {
        printf("%-10s | %-14s | %-18s | %-20s | %-12s | %.2f\n",
               accounts[i].accountNumber, accounts[i].username, accounts[i].fullName,
               accounts[i].email, accounts[i].phone, accounts[i].balance);
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int choice, loginStatus;
    srand((unsigned int) time(NULL));
    loadAccounts();
    printf("=========================================\n");
    printf("    CHITOSHI ONLINE BANKING SYSTEM\n");
    printf("=========================================\n");
    while (1) {
        printf("\n1. Register\n2. Login\n3. Admin Login\n4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Enter a number.\n");
            clearInputBuffer();
            continue;
        }
        if (choice == 1) {
            registerAccount();
        } else if (choice == 2) {
            loginStatus = login(1);
            if (loginStatus == 1) mainMenu();
        } else if (choice == 3) {
            loginStatus = login(2);
            if (loginStatus == 2) adminMenu();
        } else if (choice == 4) {
            printf("\nThank you for banking with us!\n");
            exit(0);
        } else {
            printf("Invalid choice!\n");
        }
    }
    return 0;
}

void mainMenu() {
    int choice;
    while (isLoggedIn && !isAdmin) {
        printf("\n=== MAIN MENU ===\n");
        printf("1. Deposit\n2. Withdraw\n3. Transfer\n");
        printf("4. Check Balance\n5. Change Password\n6. Account Info\n7. Transaction History\n8. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            clearInputBuffer();
            continue;
        }
        switch (choice) {
            case 1: depositFunds(); break;
            case 2: withdrawFunds(); break;
            case 3: transferFunds(); break;
            case 4: checkBalance(); break;
            case 5: changePassword(); break;
            case 6: displayAccountInfo(); break;
            case 7: displayTransactionHistory(currentUser->accountNumber); break;
            case 8:
                isLoggedIn = 0; currentUser = NULL; isAdmin = 0;
                printf("Logged out!\n");
                break;
            default: printf("Invalid choice!\n");
        }
    }
}

void adminMenu() {
    int choice;
    while (isLoggedIn && isAdmin) {
        printf("\n=== ADMIN MENU ===\n");
        printf("1. View All Accounts\n2. View All Transactions\n3. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            clearInputBuffer();
            continue;
        }
        switch (choice) {
            case 1: displayAllAccounts(); break;
            case 2: displayAllTransactions(); break;
            case 3:
                isLoggedIn = 0; isAdmin = 0; currentUser = NULL;
                printf("Admin logged out!\n");
                break;
            default: printf("Invalid choice!\n");
        }
    }
}
