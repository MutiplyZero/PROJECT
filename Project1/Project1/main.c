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