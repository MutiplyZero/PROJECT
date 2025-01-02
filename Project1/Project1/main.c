#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_USERS 10
#define MAX_ACCOUNTS 50
#define MAX_TRANSACTIONS 200
#define USERS_FILE "users.txt"

typedef struct {
    char username[20];
    char password[20];
    bool isActive;
} User;

typedef struct {
    int accountId;
    char ownerUsername[20];
    double balance;
    bool isActive;
} Account;

typedef enum {
    TRANSACTION_DEPOSIT,
    TRANSACTION_WITHDRAW,
    TRANSACTION_TRANSFER
} TransactionType;

typedef struct {
    int transactionId;
    TransactionType type;
    int fromAccountId;
    int toAccountId;
    double amount;
    char timestamp[30];
} TransactionRecord;

User users[MAX_USERS];
Account accounts[MAX_ACCOUNTS];
TransactionRecord transactions[MAX_TRANSACTIONS];

int userCount = 0;
int accountCount = 0;
int transactionCount = 0;

int currentUserIndex = -1;

void loadUsers();
void saveUsers();
void exitSystem();
void showMenu();
void doNotLoggedInMenu();
void doLoggedInMenu();
int signUp();
int login();
void logout();
bool isLoggedIn();
void openAccount();
void closeAccount();
void viewBalance();
void depositMoney();
void withdrawMoney();
void transferMoney();
void viewTransactions();
int findUserIndexByUsername(const char* username);
int findAccountIndexById(int accountId);
void recordTransaction(TransactionType type, int fromId, int toId, double amount);
void getCurrentTimeString(char* buffer, size_t size);

int main() {
    memset(accounts, 0, sizeof(accounts));
    memset(transactions, 0, sizeof(transactions));
    loadUsers();
    while (1) {
        showMenu();
    }
    return 0;
}

void showMenu() {
    if (!isLoggedIn()) {
        doNotLoggedInMenu();
    }
    else {
        doLoggedInMenu();
    }
}

void doNotLoggedInMenu() {
    int choice;
    printf("\n========== 銀行系統 - 未登入選單 ==========\n");
    printf("[1] 會員註冊\n");
    printf("[2] 會員登入\n");
    printf("[0] 離開系統\n");
    printf("-----------------------------------------\n");
    printf("請選擇: ");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
    case 1:
        signUp();
        break;
    case 2:
        login();
        break;
    case 0:
        exitSystem();
        break;
    default:
        printf("無效的選擇，請重新輸入。\n");
        break;
    }
}

void doLoggedInMenu() {
    int choice;
    printf("\n========== 銀行系統 - 已登入選單 ==========\n");
    printf("[1] 開戶\n");
    printf("[2] 關戶\n");
    printf("[3] 顯示帳戶餘額\n");
    printf("[4] 存款\n");
    printf("[5] 提款\n");
    printf("[6] 轉帳\n");
    printf("[7] 交易紀錄查詢\n");
    printf("[8] 登出\n");
    printf("[0] 離開系統\n");
    printf("-----------------------------------------\n");
    printf("請選擇: ");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
    case 1:
        openAccount();
        break;
    case 2:
        closeAccount();
        break;
    case 3:
        viewBalance();
        break;
    case 4:
        depositMoney();
        break;
    case 5:
        withdrawMoney();
        break;
    case 6:
        transferMoney();
        break;
    case 7:
        viewTransactions();
        break;
    case 8:
        logout();
        break;
    case 0:
        exitSystem();
        break;
    default:
        printf("無效的選擇，請重新輸入。\n");
        break;
    }
}


int signUp() {
    if (userCount >= MAX_USERS) {
        printf("使用者已達上限，無法註冊。\n");
        return -1;
    }
    char username[20], password[20];
    printf("請輸入想註冊的使用者名稱: ");
    scanf("%s", username);
    if (findUserIndexByUsername(username) != -1) {
        printf("此使用者名稱已被註冊。\n");
        return -1;
    }
    printf("請輸入密碼: ");
    scanf("%s", password);
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    users[userCount].isActive = true;
    userCount++;
    saveUsers();
    printf("註冊成功！請登入以使用系統功能。\n");
    return 0;
}

int login() {
    char username[20], password[20];
    printf("請輸入使用者名稱: ");
    scanf("%s", username);
    printf("請輸入密碼: ");
    scanf("%s", password);
    int idx = findUserIndexByUsername(username);
    if (idx == -1) {
        printf("查無此使用者!\n");
        return -1;
    }
    if (!users[idx].isActive) {
        printf("此帳號已被停用或不存在。\n");
        return -1;
    }
    if (strcmp(users[idx].password, password) == 0) {
        currentUserIndex = idx;
        printf("登入成功！\n");
        return idx;
    }
    else {
        printf("密碼錯誤!\n");
        return -1;
    }
}

void logout() {
    currentUserIndex = -1;
    printf("您已登出。\n");
}

bool isLoggedIn() {
    return (currentUserIndex != -1);
}


