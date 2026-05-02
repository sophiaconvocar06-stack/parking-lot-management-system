#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 5
#define QMAX 5
#define MAX_HISTORY 100

#define RATE 20
#define MOTOR_RATE 10

#define USER_FILE "user.txt"

// ================= STRUCTURES =================
typedef struct {
    char plate[10];
    char type[15];
    time_t timeIn;
} Car;

typedef struct {
    char plate[10];
    char type[15];
    int hours;
    int total;
} History;

// ================= GLOBAL =================
Car stack[MAX];
int top = -1;

char queue[QMAX][10];
int front = 0, rear = -1;

History history[MAX_HISTORY];
int historyCount = 0;

int totalEarnings = 0;

// ================= AUTH SYSTEM =================
void registerUser() {
    char user[20], pass[20];

    FILE *fp = fopen(USER_FILE, "w");

    printf("\n========== REGISTER ==========\n");

    printf("Username: ");
    scanf("%s", user);

    printf("Password: ");
    scanf("%s", pass);

    fprintf(fp, "%s %s", user, pass);
    fclose(fp);

    printf(">> Registration Successful!\n");
}

int loginUser() {
    char user[20], pass[20];
    char fileUser[20], filePass[20];

    FILE *fp = fopen(USER_FILE, "r");

    if (fp == NULL) {
        printf("No account found. Please register first.\n");
        return 0;
    }

    fscanf(fp, "%s %s", fileUser, filePass);
    fclose(fp);

    printf("\n========== LOGIN ==========\n");

    printf("Username: ");
    scanf("%s", user);

    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(user, fileUser) == 0 && strcmp(pass, filePass) == 0) {
        printf(">> LOGIN SUCCESSFUL\n");
        return 1;
    }

    printf(">> LOGIN FAILED\n");
    return 0;
}

// ================= UTIL =================
int getRate(char t[]) {
    return strcmp(t, "Motorcycle") == 0 ? MOTOR_RATE : RATE;
}

int isQueueEmpty() {
    return front > rear;
}

// ================= RECEIPT FILE =================
void saveReceipt(Car c, int hours, int total) {
    FILE *fp = fopen("receipt.txt", "a");

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(fp, "\n========================================\n");
    fprintf(fp, "              RECEIPT TICKET           \n");
    fprintf(fp, "========================================\n");
    fprintf(fp, "Date: %02d/%02d/%d\n",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    fprintf(fp, "Time: %02d:%02d:%02d\n",
            t->tm_hour, t->tm_min, t->tm_sec);
    fprintf(fp, "Plate: %s\n", c.plate);
    fprintf(fp, "Type : %s\n", c.type);
    fprintf(fp, "Hours: %d\n", hours);
    fprintf(fp, "Total: %d PHP\n", total);
    fprintf(fp, "========================================\n");
    fprintf(fp, "THANK YOU AND DRIVE SAFELY!\n");
    fprintf(fp, "========================================\n");

    fclose(fp);
}

// ================= DISPLAY =================
void showParking() {
    printf("\n--- PARKING SLOTS ---\n");
    if (top == -1) printf("Empty\n");
    else {
        for (int i = 0; i <= top; i++)
            printf("Slot %d: %s\n", i + 1, stack[i].plate);
    }
}

void showQueue() {
    printf("\n--- WAITING QUEUE ---\n");
    if (isQueueEmpty()) printf("Empty\n");
    else {
        for (int i = front; i <= rear; i++)
            printf("%s\n", queue[i]);
    }
}

// ================= PARK =================
void park() {
    Car c;

    printf("\n[PARK]\n");
    printf("Plate: ");
    scanf("%s", c.plate);
    printf("Type: ");
    scanf("%s", c.type);

    if (top == MAX - 1) {
        if (rear - front + 1 == QMAX) {
            printf(">> Queue FULL\n");
            return;
        }

        printf(">> FULL -> Added to Queue\n");
        strcpy(queue[++rear], c.plate);
    } else {
        c.timeIn = time(NULL);
        stack[++top] = c;
        printf(">> Parked Successfully\n");
    }

    showParking();
    showQueue();
}

// ================= REMOVE =================
void removeCar() {
    printf("\n[REMOVE]\n");

    if (top == -1) {
        printf(">> Parking EMPTY\n");
        return;
    }

    Car c = stack[top];
    time_t out = time(NULL);

    int hours = (int)(difftime(out, c.timeIn) / 3600);
    if (hours < 1) hours = 1;

    int total = hours * getRate(c.type);

    // ===== RECEIPT BOX =====
    printf("\n========================================\n");
    printf("              RECEIPT TICKET           \n");
    printf("========================================\n");
    printf(" Plate : %s\n", c.plate);
    printf(" Type  : %s\n", c.type);
    printf(" Hours : %d\n", hours);
    printf(" Total : %d PHP\n", total);
    printf("========================================\n");
    printf("   THANK YOU AND DRIVE SAFELY!          \n");
    printf("========================================\n");

    saveReceipt(c, hours, total);

    // HISTORY
    if (historyCount < MAX_HISTORY) {
        strcpy(history[historyCount].plate, c.plate);
        strcpy(history[historyCount].type, c.type);
        history[historyCount].hours = hours;
        history[historyCount].total = total;
        historyCount++;
    }

    totalEarnings += total;

    printf(">> Removed: %s\n", c.plate);

    top--;

    // QUEUE SHIFT
    if (!isQueueEmpty()) {
        printf(">> Queue Car %s entered\n", queue[front]);

        for (int i = front; i < rear; i++) {
            strcpy(queue[i], queue[i + 1]);
        }
        rear--;
    }

    showParking();
    showQueue();
}

// ================= SEARCH =================
void search() {
    char p[10];

    printf("\n[SEARCH]\nEnter plate: ");
    scanf("%s", p);

    for (int i = 0; i <= top; i++) {
        if (strcmp(stack[i].plate, p) == 0) {
            printf(">> Found in Parking Slot %d\n", i + 1);
            return;
        }
    }

    for (int i = front; i <= rear; i++) {
        if (strcmp(queue[i], p) == 0) {
            printf(">> Found in Queue\n");
            return;
        }
    }

    printf(">> Not Found\n");
}

// ================= HISTORY =================
void showHistory() {
    printf("\n=== TRANSACTION HISTORY ===\n");

    for (int i = 0; i < historyCount; i++) {
        printf("%d. %s | %s | %d hrs | %d PHP\n",
               i + 1,
               history[i].plate,
               history[i].type,
               history[i].hours,
               history[i].total);
    }
}

// ================= SORT =================
void sortTotal() {
    for (int i = 0; i < historyCount - 1; i++)
        for (int j = 0; j < historyCount - i - 1; j++)
            if (history[j].total > history[j + 1].total) {
                History t = history[j];
                history[j] = history[j + 1];
                history[j + 1] = t;
            }

    printf(">> Sorted by TOTAL\n");
}

void sortHours() {
    for (int i = 0; i < historyCount - 1; i++)
        for (int j = 0; j < historyCount - i - 1; j++)
            if (history[j].hours > history[j + 1].hours) {
                History t = history[j];
                history[j] = history[j + 1];
                history[j + 1] = t;
            }

    printf(">> Sorted by HOURS\n");
}

// ================= GRAPH =================
void graph() {
    printf("\n[GRAPH]\n");
    printf("EARNINGS VISUALIZATION\n");
    printf("--------------------------------\n");

    if (totalEarnings == 0) {
        printf("| No transactions yet\n");
    } else {
        int bars = totalEarnings / 10;
        for (int i = 0; i < bars; i++) printf("|");
    }

    printf("\n--------------------------------\n");
    printf("Total Earnings: %d PHP\n", totalEarnings);
}

// ================= MENU =================
void menu() {
    printf("\n========== MENU ==========\n");
    printf("1 Park\n");
    printf("2 Remove\n");
    printf("3 Search\n");
    printf("4 History\n");
    printf("5 Sort Total\n");
    printf("6 Sort Hours\n");
    printf("7 Graph\n");
    printf("0 Exit\n");
    printf("Choice: ");
}

// ================= MAIN =================
int main() {

    int choice;
    int loggedIn = 0;

    printf("\n1. Register\n2. Login\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) registerUser();

    loggedIn = loginUser();

    if (!loggedIn) {
        printf("Access Denied. Program Terminated.\n");
        return 0;
    }

    int ch;

    while (1) {
        menu();
        scanf("%d", &ch);

        switch (ch) {
            case 1: park(); break;
            case 2: removeCar(); break;
            case 3: search(); break;
            case 4: showHistory(); break;
            case 5: sortTotal(); break;
            case 6: sortHours(); break;
            case 7: graph(); break;
            case 0: return 0;
        }
    }
}