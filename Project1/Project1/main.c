// �s��
void depositMoney() {
    int accountId;
    double amount;
    printf("�п�J�s�ڱb�ḹ�X: ");
    scanf("%d", &accountId);

    int idx = findAccountIndexById(accountId);
    if (idx == -1 || !accounts[idx].isActive) {
        printf("�b�ᤣ�s�b�Τw����!\n");
        return;
    }
    // �u���ۤv���b��s��
    if (strcmp(accounts[idx].ownerUsername, users[currentUserIndex].username) != 0) {
        printf("�z�L�v��L�H�b��s��!\n");
        return;
    }

    printf("�п�J�s�ڪ��B: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("�s�ڪ��B�ݤj�� 0!\n");
        return;
    }

    accounts[idx].balance += amount;
    printf("�b�� %d �s�ڦ��\�A���B: %.2f�A�l�B: %.2f\n",
        accountId, amount, accounts[idx].balance);

    // �������
    recordTransaction(TRANSACTION_DEPOSIT, accountId, -1, amount);
}

// ����
void withdrawMoney() {
    int accountId;
    double amount;
    printf("�п�J���ڱb�ḹ�X: ");
    scanf("%d", &accountId);

    int idx = findAccountIndexById(accountId);
    if (idx == -1 || !accounts[idx].isActive) {
        printf("�b�ᤣ�s�b�Τw����!\n");
        return;
    }
    // �u���ۤv���b�ᴣ��
    if (strcmp(accounts[idx].ownerUsername, users[currentUserIndex].username) != 0) {
        printf("�z�L�v��L�H�b�ᴣ��!\n");
        return;
    }

    printf("�п�J���ڪ��B: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("���ڪ��B�ݤj�� 0!\n");
        return;
    }
    if (accounts[idx].balance < amount) {
        printf("�l�B����!\n");
        return;
    }

    accounts[idx].balance -= amount;
    printf("�b�� %d ���ڦ��\�A���B: %.2f�A�l�B: %.2f\n",
        accountId, amount, accounts[idx].balance);

    // �������
    recordTransaction(TRANSACTION_WITHDRAW, accountId, -1, amount);
}

// ��b
void transferMoney() {
    int fromId, toId;
    double amount;
    printf("�п�J��X�b�ḹ�X: ");
    scanf("%d", &fromId);

    int fromIdx = findAccountIndexById(fromId);
    if (fromIdx == -1 || !accounts[fromIdx].isActive) {
        printf("�ӷ��b�ᤣ�s�b�Τw����!\n");
        return;
    }
    // �u���ۤv���b����X
    if (strcmp(accounts[fromIdx].ownerUsername, users[currentUserIndex].username) != 0) {
        printf("�z�L�v�ާ@�L�H�b��!\n");
        return;
    }

    printf("�п�J��J�b�ḹ�X: ");
    scanf("%d", &toId);

    int toIdx = findAccountIndexById(toId);
    if (toIdx == -1 || !accounts[toIdx].isActive) {
        printf("�ت��b�ᤣ�s�b�Τw����!\n");
        return;
    }

    printf("�п�J��b���B: ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("��b���B�ݤj�� 0!\n");
        return;
    }
    if (accounts[fromIdx].balance < amount) {
        printf("�ӷ��b��l�B����!\n");
        return;
    }

    accounts[fromIdx].balance -= amount;
    accounts[toIdx].balance += amount;

    printf("��b���\!\n");
    printf("�ӷ��b�� %d �l�B: %.2f\n", fromId, accounts[fromIdx].balance);
    printf("�ت��b�� %d �l�B: %.2f\n", toId, accounts[toIdx].balance);

    // �������
    recordTransaction(TRANSACTION_TRANSFER, fromId, toId, amount);
}

// ��������d��
void viewTransactions() {
    if (transactionCount == 0) {
        printf("�ثe�L�����������C\n");
        return;
    }

    printf("\n========== �z��������� ==========\n");
    for (int i = 0; i < transactionCount; i++) {
        TransactionRecord* t = &transactions[i];

        // �u��ܡu�P�ۤv�b�ᦳ���v�����
        bool related = false;
        int fromIdx = findAccountIndexById(t->fromAccountId);
        int toIdx = findAccountIndexById(t->toAccountId);

        // �ˬd�ӷ��b��O�_�ݩ�ۤv
        if (fromIdx != -1 &&
            strcmp(accounts[fromIdx].ownerUsername, users[currentUserIndex].username) == 0) {
            related = true;
        }
        // �ˬd�ت��b��O�_�ݩ�ۤv
        if (toIdx != -1 &&
            strcmp(accounts[toIdx].ownerUsername, users[currentUserIndex].username) == 0) {
            related = true;
        }

        if (!related) {
            // �o������P�ڵL���A�����
            continue;
        }

        // �����r��
        char typeStr[10];
        switch (t->type) {
        case TRANSACTION_DEPOSIT:   strcpy(typeStr, "�s��");   break;
        case TRANSACTION_WITHDRAW:  strcpy(typeStr, "����");   break;
        case TRANSACTION_TRANSFER:  strcpy(typeStr, "��b");   break;
        default:                    strcpy(typeStr, "����");   break;
        }

        printf("ID:%d | ����:%s | �ӷ��b��:%d | �ت��b��:%d | ���B:%.2f | �ɶ�:%s\n",
            t->transactionId, typeStr, t->fromAccountId, t->toAccountId,
            t->amount, t->timestamp);
    }
}

/* ===============================
   �u��禡
   =============================== */

   // �ھ� username ��ϥΪ̯���
int findUserIndexByUsername(const char* username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && users[i].isActive) {
            return i;
        }
    }
    return -1;
}

// �ھ� accountId ��b�����
int findAccountIndexById(int accountId) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountId == accountId && accounts[i].isActive) {
            return i;
        }
    }
    return -1;
}

// �������
void recordTransaction(TransactionType type, int fromId, int toId, double amount) {
    if (transactionCount >= MAX_TRANSACTIONS) {
        printf("��������w�F�W���A�L�k�O�������!\n");
        return;
    }
    TransactionRecord* tr = &transactions[transactionCount];
    tr->transactionId = transactionCount + 1;
    tr->type = type;
    tr->fromAccountId = fromId;
    tr->toAccountId = toId;
    tr->amount = amount;

    // ���o�t�ήɶ��r��
    getCurrentTimeString(tr->timestamp, sizeof(tr->timestamp));

    transactionCount++;
}

// ���o YYYY-MM-DD HH:MM:SS �榡���ɶ��r��
void getCurrentTimeString(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(buffer, size, "%04d-%02d-%02d %02d:%02d:%02d",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec);
}