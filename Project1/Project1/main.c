<<<<<<< HEAD
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

void loadUsers() {
    FILE* fp = fopen(USERS_FILE, "r");
    if (!fp) {

        printf("??? %s???????????\n", USERS_FILE);
        userCount = 0;
        return;
    }
    userCount = 0;
    while (!feof(fp)) {
        char username[20];
        char password[20];
        int active;
        int ret = fscanf(fp, "%s %s %d", username, password, &active);
        if (ret == 3 && userCount < MAX_USERS) {
            strcpy(users[userCount].username, username);
            strcpy(users[userCount].password, password);
            users[userCount].isActive = (active == 1);
            userCount++;
        }
    }
    fclose(fp);
    printf("??? %d ??????\n", userCount);
}
users.txt
void saveUsers() {
    FILE* fp = fopen(USERS_FILE, "w");
    if (!fp) {
        printf("???? %s ????!\n", USERS_FILE);
        return;
    }
    for (int i = 0; i < userCount; i++) {
        if (users[i].isActive) {
            int activeInt = users[i].isActive ? 1 : 0;
            fprintf(fp, "%s %s %d\n",
                users[i].username,
                users[i].password,
                activeInt);
        }
    }
    fclose(fp);
}


void exitSystem() {
    saveUsers();
    printf("??????????\n");
    exit(0);
}
void openAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("??????????????\n");
        return;
    }
    int inputId;
    printf("???????????(?: 1000): ");
    scanf("%d", &inputId);


    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].isActive && accounts[i].accountId == inputId) {
            printf("???????????????\n");
            return;
        }
    }

    accounts[accountCount].accountId = inputId;
    strcpy(accounts[accountCount].ownerUsername, users[currentUserIndex].username);
    accounts[accountCount].balance = 0.0;
    accounts[accountCount].isActive = true;

    printf("????! ????: %d, ???: %s\n",
        accounts[accountCount].accountId,
        accounts[accountCount].ownerUsername);
    accountCount++;
}


void closeAccount() {
    int accountId;
    printf("???????????: ");
    scanf("%d", &accountId);

    int idx = findAccountIndexById(accountId);
    if (idx == -1) {
        printf("?????!\n");
        return;
    }

    if (strcmp(accounts[idx].ownerUsername, users[currentUserIndex].username) != 0) {
        printf("??????????!\n");
        return;
    }

    if (!accounts[idx].isActive) {
        printf("????????????\n");
        return;
    }

    accounts[idx].balance = 0;
    accounts[idx].isActive = false;
    printf("?? %d ??????\n", accountId);
}


void viewBalance() {
    int accountId;
    printf("???????????: ");
    scanf("%d", &accountId);

    int idx = findAccountIndexById(accountId);
    if (idx == -1) {
        printf("?????!\n");
        return;
    }
    if (!accounts[idx].isActive) {
        printf("?????????!\n");
        return;
    }

    if (strcmp(accounts[idx].ownerUsername, users[currentUserIndex].username) != 0) {
        printf("???????????!\n");
        return;
    }

    printf("?? %d ??: %.2f\n", accountId, accounts[idx].balance);
}
=======
// 存款
void depositMoney() {
    int accountId;
    double amount;
    printf("請輸入存款帳戶號碼: ");
    scanf("%d", &accountId);

    int idx = findAccountIndexById(accountId);
    if (idx == -1 || !accounts[idx].isActive) {
        printf("帳戶不存在或已關閉!\n");
        return;
    }
    // 只能對自己的帳戶存款
    if (strcmp(accounts[idx].ownerUsername, users[currentUserIndex].username) != 0) {
        printf("您無權對他人帳戶存款!\n");
        return;
    }

    printf("請輸入存款金額: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("存款金額需大於 0!\n");
        return;
    }

    accounts[idx].balance += amount;
    printf("帳戶 %d 存款成功，金額: %.2f，餘額: %.2f\n",
        accountId, amount, accounts[idx].balance);

    // 紀錄交易
    recordTransaction(TRANSACTION_DEPOSIT, accountId, -1, amount);
}

// 提款
void withdrawMoney() {
    int accountId;
    double amount;
    printf("請輸入提款帳戶號碼: ");
    scanf("%d", &accountId);

    int idx = findAccountIndexById(accountId);
    if (idx == -1 || !accounts[idx].isActive) {
        printf("帳戶不存在或已關閉!\n");
        return;
    }
    // 只能對自己的帳戶提款
    if (strcmp(accounts[idx].ownerUsername, users[currentUserIndex].username) != 0) {
        printf("您無權對他人帳戶提款!\n");
        return;
    }

    printf("請輸入提款金額: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("提款金額需大於 0!\n");
        return;
    }
    if (accounts[idx].balance < amount) {
        printf("餘額不足!\n");
        return;
    }

    accounts[idx].balance -= amount;
    printf("帳戶 %d 提款成功，金額: %.2f，餘額: %.2f\n",
        accountId, amount, accounts[idx].balance);

    // 紀錄交易
    recordTransaction(TRANSACTION_WITHDRAW, accountId, -1, amount);
}

// 轉帳
void transferMoney() {
    int fromId, toId;
    double amount;
    printf("請輸入轉出帳戶號碼: ");
    scanf("%d", &fromId);

    int fromIdx = findAccountIndexById(fromId);
    if (fromIdx == -1 || !accounts[fromIdx].isActive) {
        printf("來源帳戶不存在或已關閉!\n");
        return;
    }
    // 只能對自己的帳戶轉出
    if (strcmp(accounts[fromIdx].ownerUsername, users[currentUserIndex].username) != 0) {
        printf("您無權操作他人帳戶!\n");
        return;
    }

    printf("請輸入轉入帳戶號碼: ");
    scanf("%d", &toId);

    int toIdx = findAccountIndexById(toId);
    if (toIdx == -1 || !accounts[toIdx].isActive) {
        printf("目的帳戶不存在或已關閉!\n");
        return;
    }

    printf("請輸入轉帳金額: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("轉帳金額需大於 0!\n");
        return;
    }
    if (accounts[fromIdx].balance < amount) {
        printf("來源帳戶餘額不足!\n");
        return;
    }

    accounts[fromIdx].balance -= amount;
    accounts[toIdx].balance += amount;

    printf("轉帳成功!\n");
    printf("來源帳戶 %d 餘額: %.2f\n", fromId, accounts[fromIdx].balance);
    printf("目的帳戶 %d 餘額: %.2f\n", toId, accounts[toIdx].balance);

    // 紀錄交易
    recordTransaction(TRANSACTION_TRANSFER, fromId, toId, amount);
}

// 交易紀錄查詢
void viewTransactions() {
    if (transactionCount == 0) {
        printf("目前無任何交易紀錄。\n");
        return;
    }

    printf("\n========== 您的交易紀錄 ==========\n");
    for (int i = 0; i < transactionCount; i++) {
        TransactionRecord* t = &transactions[i];

        // 只顯示「與自己帳戶有關」的交易
        bool related = false;
        int fromIdx = findAccountIndexById(t->fromAccountId);
        int toIdx = findAccountIndexById(t->toAccountId);

        // 檢查來源帳戶是否屬於自己
        if (fromIdx != -1 &&
            strcmp(accounts[fromIdx].ownerUsername, users[currentUserIndex].username) == 0) {
            related = true;
        }
        // 檢查目的帳戶是否屬於自己
        if (toIdx != -1 &&
            strcmp(accounts[toIdx].ownerUsername, users[currentUserIndex].username) == 0) {
            related = true;
        }

        if (!related) {
            // 這筆交易與我無關，不顯示
            continue;
        }

        // 類型字串
        char typeStr[10];
        switch (t->type) {
        case TRANSACTION_DEPOSIT:   strcpy(typeStr, "存款");   break;
        case TRANSACTION_WITHDRAW:  strcpy(typeStr, "提款");   break;
        case TRANSACTION_TRANSFER:  strcpy(typeStr, "轉帳");   break;
        default:                    strcpy(typeStr, "未知");   break;
        }

        printf("ID:%d | 類型:%s | 來源帳戶:%d | 目的帳戶:%d | 金額:%.2f | 時間:%s\n",
            t->transactionId, typeStr, t->fromAccountId, t->toAccountId,
            t->amount, t->timestamp);
    }
}

/* ===============================
   工具函式
   =============================== */

   // 根據 username 找使用者索引
int findUserIndexByUsername(const char* username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && users[i].isActive) {
            return i;
        }
    }
    return -1;
}

// 根據 accountId 找帳戶索引
int findAccountIndexById(int accountId) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountId == accountId && accounts[i].isActive) {
            return i;
        }
    }
    return -1;
}

// 紀錄交易
void recordTransaction(TransactionType type, int fromId, int toId, double amount) {
    if (transactionCount >= MAX_TRANSACTIONS) {
        printf("交易紀錄已達上限，無法記錄此交易!\n");
        return;
    }
    TransactionRecord* tr = &transactions[transactionCount];
    tr->transactionId = transactionCount + 1;
    tr->type = type;
    tr->fromAccountId = fromId;
    tr->toAccountId = toId;
    tr->amount = amount;

    // 取得系統時間字串
    getCurrentTimeString(tr->timestamp, sizeof(tr->timestamp));

    transactionCount++;
}

// 取得 YYYY-MM-DD HH:MM:SS 格式的時間字串
void getCurrentTimeString(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(buffer, size, "%04d-%02d-%02d %02d:%02d:%02d",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec);
}
>>>>>>> project_3
