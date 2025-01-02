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
    printf("\n========== �Ȧ�t�� - ���n�J��� ==========\n");
    printf("[1] �|�����U\n");
    printf("[2] �|���n�J\n");
    printf("[0] ���}�t��\n");
    printf("-----------------------------------------\n");
    printf("�п��: ");
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
        printf("�L�Ī���ܡA�Э��s��J�C\n");
        break;
    }
}

void doLoggedInMenu() {
    int choice;
    printf("\n========== �Ȧ�t�� - �w�n�J��� ==========\n");
    printf("[1] �}��\n");
    printf("[2] ����\n");
    printf("[3] ��ܱb��l�B\n");
    printf("[4] �s��\n");
    printf("[5] ����\n");
    printf("[6] ��b\n");
    printf("[7] ��������d��\n");
    printf("[8] �n�X\n");
    printf("[0] ���}�t��\n");
    printf("-----------------------------------------\n");
    printf("�п��: ");
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
        printf("�L�Ī���ܡA�Э��s��J�C\n");
        break;
    }
}


int signUp() {
    if (userCount >= MAX_USERS) {
        printf("�ϥΪ̤w�F�W���A�L�k���U�C\n");
        return -1;
    }
    char username[20], password[20];
    printf("�п�J�Q���U���ϥΪ̦W��: ");
    scanf("%s", username);
    if (findUserIndexByUsername(username) != -1) {
        printf("���ϥΪ̦W�٤w�Q���U�C\n");
        return -1;
    }
    printf("�п�J�K�X: ");
    scanf("%s", password);
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    users[userCount].isActive = true;
    userCount++;
    saveUsers();
    printf("���U���\�I�еn�J�H�ϥΨt�Υ\��C\n");
    return 0;
}

int login() {
    char username[20], password[20];
    printf("�п�J�ϥΪ̦W��: ");
    scanf("%s", username);
    printf("�п�J�K�X: ");
    scanf("%s", password);
    int idx = findUserIndexByUsername(username);
    if (idx == -1) {
        printf("�d�L���ϥΪ�!\n");
        return -1;
    }
    if (!users[idx].isActive) {
        printf("���b���w�Q���ΩΤ��s�b�C\n");
        return -1;
    }
    if (strcmp(users[idx].password, password) == 0) {
        currentUserIndex = idx;
        printf("�n�J���\�I\n");
        return idx;
    }
    else {
        printf("�K�X���~!\n");
        return -1;
    }
}

void logout() {
    currentUserIndex = -1;
    printf("�z�w�n�X�C\n");
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
