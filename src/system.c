#include "header.h"
#include "stdbool.h"
#include "ctype.h"
//#include <sqlite3.h>
#define MAX_LINE 256

const char *RECORDS = "./data/records.txt";
const char *USER = "./data/user.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.day,
                  &r->deposit.month,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) == 11; 
}

//##############################################################################################################################################################

void putAccountToFile(FILE *ptr, const char *name, struct Record r) {
    fprintf(ptr, "%d %d %s %d %02d/%02d/%d %s %d %.2lf %s\n",
            r.id,
            r.userId,
            name,
            r.accountNbr,
            r.deposit.day,
            r.deposit.month,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

//##############################################################################################################################################################

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%s %d %02d/%02d/%d %s %d %.2lf %s\n",
            u.name,
            r.accountNbr,
            r.deposit.day,
            r.deposit.month,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

//##############################################################################################################################################################

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

//##############################################################################################################################################################
void registerMenu() {
    system("clear");
    FILE *fp;
    struct User newUser;
    char password[50];

    printf("\n\t\t\t===== Register =====\n");

    do {
        printf("\n\t\tEnter your username (max 49 characters): ");
        scanf("%51s", &newUser.name);
        if (strlen(newUser.name) > 50) {
            printf("\nUsername is too long. Please try again.\n");
            registerMenu();
            return;
        }
        fp = fopen("./data/users.txt", "r");
        if (fp == NULL) {
            printf("Error opening file\n");
            exit(1);
        }

        int found = 0;
        struct User tempUser;
        while (fscanf(fp, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) != EOF) {
            if (strcmp(newUser.name, tempUser.name) == 0) {
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found) {
            printf("\nUsername already exists. Please choose another.\n");
        } else {
            break;
        }
    } while (1);

    printf("\n\t\tEnter your password (max 49 characters): ");
    scanf("%s", newUser.password);

    printf("\n\t\tConfirm your password: ");
    scanf("%s", newUser.confirmPassword);
    
    if (strcmp(newUser.password, newUser.confirmPassword) != 0) {
        printf("\nPasswords do not match. Please try again.\n");
        registerMenu();
        return;
    }

    newUser.id = getLastUserId();  

    fp = fopen("./data/users.txt", "a");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);
    fclose(fp);

    printf("\nRegistration successful!\n");
    stayOrReturn(1, mainMenu, newUser);
}

//##############################################################################################################################################################

int login(char *username, char *password) {
    FILE *file = fopen("data/users.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    char line[MAX_LINE];
    struct User user;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d %s %s", &user.id, user.name, user.password);
        if (strcmp(username, user.name) == 0 && strcmp(password, user.password) == 0) {
            fclose(file);
            return user.id;
        }
    }

    fclose(file);
    return -1;  
}

//##############################################################################################################################################################
int validatePhoneNumber(const char* phone) {
    int i;
    for (i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i])) {
            return 0;
        }
    }
    return 1;
}

//##############################################################################################################################################################
int generateAccountNumber() {
    static int accountNumber = 1000000001;
    return ++accountNumber;
}


//##############################################################################################################################################################
// double getValidAmount(double amount) {
//     char input[100];
//     while (1) {
//         printf("\nEnter amount: $");
//         scanf("%s", input);
//         if (sscanf(input, "%le", &amount) == 1 && amount > 0) {
//             return amount;
//         }
//         printf("Invalid input. Please enter a positive number.\n");
//     }
// }

//##############################################################################################################################################################
int getLastAccountId() {
    FILE *fp = fopen("./data/records.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int lastId = 0;
    struct Record tempRecord;

    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &tempRecord.id, &tempRecord.userId, tempRecord.name, &tempRecord.accountNbr,
                  &tempRecord.deposit.day, &tempRecord.deposit.month, &tempRecord.deposit.year,
                  tempRecord.country, &tempRecord.phone, &tempRecord.amount, tempRecord.accountType) != EOF) {
        if (tempRecord.id > lastId) {
            lastId = tempRecord.id;
        }
    }

    fclose(fp);
    return lastId + 1;
}
//##############################################################################################################################################################
int getLastUserId() {
    FILE *fp = fopen("./data/users.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int lastId = 0;
    struct User tempUser;

    while (fscanf(fp, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) != EOF) {
        if (tempUser.id > lastId) {
            lastId = tempUser.id;
        }
    }

    fclose(fp);
    return lastId + 1;
}

//##############################################################################################################################################################
void createNewAcc(struct User u);

//##############################################################################################################################################################
void mainMenu(struct User u);

//##############################################################################################################################################################
void checkAllAccounts(struct User u);

//##############################################################################################################################################################
void updateAccount(struct User u);

//##############################################################################################################################################################
void checkAccountInfo(struct User u);

//##############################################################################################################################################################
void checkAllAccounts(struct User u);

//##############################################################################################################################################################
void makeTransaction(struct User u);

//##############################################################################################################################################################
void removeAccount(struct User u);

//##############################################################################################################################################################
void transferOwner(struct User u);

//##############################################################################################################################################################
