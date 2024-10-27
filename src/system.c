#include "header.h"
#include "stdbool.h"
#include "ctype.h"

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
                  r->accountType) == EOF; 
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
    FILE *fp;
    struct User newUser;
    char password;

    printf("\n\t\t\t===== Register =====\n");

    do {
        printf("\n\t\tEnteur ur username (max 49 characters): ");
        scanf("%s", newUser.name);
        if (strlen(newUser.name) > 49) {
            printf("\nUsername is too long. Please try again.\n");
            //system("clear");
            continue;
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
            } else if (!IsPrintableName(newUser.name)){
                found = 2;
            }
        }
        fclose(fp);

        if (found == 1) {
            printf("\nUsername already exists. Please choose another.\n");
        } else if (found == 2){
            printf("Username invalid cause countains inprintibale character");
        } else {
            break;
        }
    } while (1);

    do {
        printf("\n\t\tEnter your password (max 49 characters): ");
        scanf("%s", newUser.password);
        if (strlen(newUser.password) > 49) {
            system("clear");
            printf("\nPassword is too long. Please try again.\n");
            registerMenu();
        } 
        printf("\n\t\tConfirm your password: ");
        scanf("%s", newUser.confirmPassword);
        
        if (strcmp(newUser.password, newUser.confirmPassword) != 0) {
            printf("\nPasswords do not match. Please try again.\n");
            registerMenu();
            return;
        } else {
            break;
        }
    } while (1);

    newUser.id = GetUserId();  

    fp = fopen("./data/users.txt", "a");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);
    fclose(fp);
    system("clear");
    printf("\nRegistration successful!\n");
    stayOrReturn(1, mainMenu, newUser);
}

//##############################################################################################################################################################

void login(char *username, char *password) {
     FILE *fp;
     int found = 0;
    do {
        system("clear");
        printf("\n\n\t\t Bank Management System\n");
        printf("\n\t\t Username: ");
        scanf("%s", username);
        printf("\t\t Password: ");
        scanf("%s", password);

        fp = fopen("./data/users.txt", "r");
        if (fp == NULL) {
            printf("\n\t\t Cannot open users file!\n");
            exit(1);
        }
        struct User tempUser;

        while (fscanf(fp, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) != EOF) {
            if (strcmp(tempUser.name, username) == 0 && strcmp(tempUser.password, password) == 0) {
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found) {
            printf("\n\t\t Login successful!\n");
            mainMenu(tempUser);
            return;
        } else {
            printf("\n\t\t Wrong username or password!\n");
            printf("\n\t\t Press Enter to try again...");
            getchar();
            getchar();
        }
         
    } while (!found);
}

//##############################################################################################################################################################
int IsDigit(const char *valid) {
    int i;
    for (i = 0; valid[i] != '\0'; i++) {
        if (!isdigit(valid[i])) {
            return 0;
        }
    }
    return 1;
}

//##############################################################################################################################################################
int generateAccountNumber() {
    static int accountNumber = 101;
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
int GetUserId() {
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

//################################################################################################################################################
bool IsPrintableName(char *str) {
    int i = 0;
    while (str[i]) {
        if (!((str[i] >= 32 && str[i] <= 126) || str[i] == '\n')) {
            return false;
        }
        i++;
    }
    return true;
}

//################################################################################################################################################
bool isValidAccountType(const char* type) {
    return (strcmp(type, "saving") == 0 || strcmp(type, "current") == 0 ||
            strcmp(type, "fixed01") == 0 || strcmp(type, "fixed02") == 0 ||
            strcmp(type, "fixed03") == 0);
}

//################################################################################################################################################
bool isValidDate(int day, int month, int year) {
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        daysInMonth[1] = 29;
    }
    
    return day <= daysInMonth[month - 1];
}

//################################################################################################################################################
// bool IsPrintablePassword(char *str) {
//     int i = 0;
//     while (str[i]) {
//         if (!(str[i] >= 32 && str[i] <= 126)) {
//             return false;
//         }
//         i++;
//     }
//     return true;
// }

//##############################################################################################################################################################
void createNewAcc(struct User u) {
    struct Record newAccount;
    FILE *fp;
    char phoneStr[11];
    bool isValid;

    do {
        system("clear");
        printf("\t\t\t\tCreating new account:\n");
        
        newAccount.id = getLastAccountId();
        newAccount.userId = u.id;
        strcpy(newAccount.name, u.name);
        newAccount.accountNbr = generateAccountNumber();
        isValid = true;
        invalidcountry:
        printf("\nEnter the country: ");
        scanf("%s", newAccount.country);
        if (strlen(newAccount.country) > 99 || strlen(newAccount.country) < 2 || 
            IsDigit(newAccount.country) || !IsPrintableName(newAccount.country)) {
            isValid = false;
            printf("\nInvalid country name. Country should be 2-99 characters and contain only letters.\n");
            goto invalidcountry;
        }

        invalidPhoneNumber:
        printf("\nEnter the phone number: ");
        scanf("%d", &newAccount.phone);
        snprintf(phoneStr, sizeof(phoneStr), "%d", newAccount.phone);
        if (newAccount.phone < 100000000 || newAccount.phone > 999999999 || !IsDigit(phoneStr)) {
            printf("\nInvalid phone number. Please enter a 9-digit number.\n");
            isValid = false;
            goto invalidPhoneNumber;
        }
        invalidAmount:
        printf("\nEnter the amount to deposit: $");
        if (scanf("%lf", &newAccount.amount) != 1 || newAccount.amount < 0) {
            printf("\nInvalid amount. Please enter a positive number.\n");
            while (getchar() != '\n');
            isValid = false;
            goto invalidAmount;
        }
        invalidtypeaccount:
        printf("\nEnter the type of account : \n\t\t\t=====> . saving\n\t\t\t=====> . current\n\t\t\t=====> . fixed01\n\t\t\t=====> . fixed02\n\t\t\t=====> . fixed03\n=================>\t  ");
        scanf("%9s", newAccount.accountType);
        if (!isValidAccountType(newAccount.accountType)) {
            printf("\nInvalid account type. Please choose from the given options.\n");
            isValid = false;
            goto invalidtypeaccount;
        }

        invalidDate:
        printf("\nEnter today's date (dd/mm/yyyy): ");
        if (scanf("%d/%d/%d", &newAccount.deposit.day, &newAccount.deposit.month, &newAccount.deposit.year) != 3 ||
            !isValidDate(newAccount.deposit.day, newAccount.deposit.month, newAccount.deposit.year)) {
            printf("\nInvalid date format or date. Please use dd/mm/yyyy format.\n");
            while (getchar() != '\n');
            isValid = false;
            goto invalidDate;
        }

        if (isValid) break;
    } while (1);

    fp = fopen("./data/records.txt", "a");
    if (fp == NULL) {
        printf("Error opening file. Please try again later.\n");
        stayOrReturn(1, mainMenu, u);
        return;
    }

    fprintf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s\n",
            newAccount.id, newAccount.userId, newAccount.name, newAccount.accountNbr,
            newAccount.deposit.day, newAccount.deposit.month, newAccount.deposit.year,
            newAccount.country, newAccount.phone, newAccount.amount, newAccount.accountType);
    fclose(fp);

    printf("\nAccount created successfully!\n");
    stayOrReturn(1, mainMenu, u);
}
//##############################################################################################################################################################

//##############################################################################################################################################################
void checkAllAccounts(struct User u) {
    struct Record r;
    int accountsFound = 0;
    FILE *pf = fopen(RECORDS, "r");
    
    if (pf == NULL) {
        printf("\nError: Unable to open records file. Please try again later.\n");
        stayOrReturn(1, mainMenu, u);
        return;
    }

    system("clear");
    printf("\t\t====== All Accounts for %s ======\n\n", u.name);
    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == EOF) {
        
        if (strcmp(r.name, u.name) == 0) {
            accountsFound++;
            printf("\n═══════════════════════════════════════\n");
            printf("Account #%d\n", accountsFound);
            printf("═══════════════════════════════════════\n");
            printf("Account Number: %d\n", r.accountNbr);
            printf("Date Opened: %02d/%02d/%04d\n", 
                   r.deposit.day, r.deposit.month, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone Number: %d\n", r.phone);
            printf("Balance: $%.2f\n", r.amount);
            printf("Account Type: %s\n", r.accountType);
            printf("═══════════════════════════════════════\n");
        }
    }

    if (accountsFound == 0) {
        printf("\nNo accounts found for user %s.\n", u.name);
    } else {
        printf("\nTotal accounts found: %d\n", accountsFound);
    }

    if (ferror(pf)) {
        printf("\nError occurred while reading the file.\n");
    }

    fclose(pf);
    
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); 
    getchar(); 
    
    stayOrReturn(1, mainMenu, u);
}
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