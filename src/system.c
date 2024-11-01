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
    fprintf(ptr, "%s %d %02d/%02d/%d %s %d %.2f %s\n",
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
        if (strlen(newUser.name) > 49 || !Onlyalphabetical(newUser.name)) {
            system("clear");
            printf("\nUsername is Invalid. Please try again.\n");
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
        clear_buffer(username, 100);
        clear_buffer(password, 100);
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
void createNewAcc(struct User *u) {
    struct Record newAccount;
    FILE *fp;

    clearScreen();
    printf("\t\t\t\tCreating new account:\n");
    memset(&newAccount, 0, sizeof(newAccount));
    
    newAccount.id = getLastAccountId();
    newAccount.userId = u->id;
    strcpy(newAccount.name, u->name);
    newAccount.accountNbr = generateAccountNumber();

    promptForCountry(&newAccount);
    promptForPhone(&newAccount);
    promptForAmount(&newAccount);
    promptForAccountType(&newAccount);
    promptForDate(&newAccount);

    fp = fopen("./data/records.txt", "a");
    if (fp == NULL) {
        printf("Error opening file. Please try again later.\n");
        stayOrReturn(1, mainMenu, *u);
        return;
    }

    fprintf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s\n",
            newAccount.id, newAccount.userId, newAccount.name, newAccount.accountNbr,
            newAccount.deposit.day, newAccount.deposit.month, newAccount.deposit.year,
            newAccount.country, newAccount.phone, newAccount.amount, newAccount.accountType);
    fclose(fp);

    printf("\nAccount created successfully!\n");
    stayOrReturn(1, mainMenu, *u);
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
    printf("\t====== All Accounts for %s ======\n\n", u.name);

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        
        if (strcmp(r.name, u.name) == 0) {
                accountsFound++;
                printf("     ╔═══════════════════════════════════════╗\n");
                printf("     ║\tAccount #%-23d     ║\n", accountsFound);
                printf("     ╠═══════════════════════════════════════╣\n");
                printf("     ║ ==> Account Number : %-17d║\n", r.accountNbr);
                printf("     ║ ==> Date Opened    : %02d/%02d/%02d       ║\n", 
                    r.deposit.day, r.deposit.month, r.deposit.year);
                printf("     ║ ==> Country        : %-17s║\n", r.country);
                printf("     ║ ==> Phone Number   : %-17d║\n", r.phone);
                printf("     ║ ==> Balance        : $%-16.2f║\n", r.amount);
                printf("     ║ ==> Account Type   : %-17s║\n", r.accountType);
                printf("     ╚═══════════════════════════════════════╝\n");
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
void updateAccount(struct User u) {
    struct Record r;
    int accountNBR, choice;
    FILE *pf = fopen(RECORDS, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    int found = 0;

    if (pf == NULL || tempFile == NULL) {
        perror("Error opening file");
        if (pf) fclose(pf);
        if (tempFile) fclose(tempFile);
        stayOrReturn(1, mainMenu, u);
        return;
    }

    printf("\t\t====== Update Account Information ======\n\n");
    printf("Enter the account number you want to update: ");
    scanf("%d", &accountNBR);

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        if (r.accountNbr == accountNBR) {
            found = 1;
            
            while (1) {
                printf("\nChoose the information you want to update:\n");
                printf("\t1. Country\n");
                printf("\t2. Phone Number\n");
                printf("\t3. Exit\n");
                printf("\nEnter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1: {
                        char newCountry[100];
                        printf("\nEnter the new country: ");
                        scanf("%s", newCountry);
                        if (!Onlyalphabetical(newCountry)) {
                            printf("\nInvalid country name. Country should be 2-99 characters and contain only letters.\n");
                            continue;
                        }
                        strncpy(r.country, newCountry, sizeof(r.country));
                        break;
                    }
                    case 2: {
                        int newPhone;
                        printf("\nEnter the new phone number: ");
                        scanf("%d", &newPhone);
                        if (!isValidPhone(newPhone)) {
                            printf("\nInvalid phone number. Please enter a 9-digit number.\n");
                            continue;
                        }
                        r.phone = newPhone;
                        break;
                    }
                    case 3:
                        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                                r.id, r.userId, r.name, r.accountNbr,
                                r.deposit.day, r.deposit.month, r.deposit.year,
                                r.country, r.phone, r.amount, r.accountType);
                        fclose(pf);
                        fclose(tempFile);
                        remove(RECORDS);
                        rename("temp.txt", RECORDS);
                        stayOrReturn(1, mainMenu, u);
                        return;
                    default:
                        printf("\nInvalid choice. Please try again.\n");
                        continue; 
                }
                break;
            }
        }
        
        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                r.id, r.userId, r.name, r.accountNbr,
                r.deposit.day, r.deposit.month, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(pf);
    fclose(tempFile);

    if (!found) {
        printf("\nAccount not found.\n");
        remove("temp.txt");
        stayOrReturn(1, mainMenu, u);
        return;
    }

    remove(RECORDS);
    rename("temp.txt", RECORDS);

    printf("\nAccount information updated successfully.\n");
}

//##############################################################################################################################################################
void checkAccountInfo(struct User u)
{
     char userName[100];
    struct Record r;
    int accountNumber;
    FILE *pf = fopen(RECORDS, "r");

    if (pf == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    system("clear");
    printf("\t\t====== Check Account Info =====\n\n");
    printf("\nEnter the account number:");
    scanf("%d", &accountNumber);

    int found = 0;
   while (getAccountFromFile(pf, r.name, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNumber) {
            found = 1;
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%02d/%02d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            break;
        }
    }
    fclose(pf);
    if (found) {
        stayOrReturn(1, checkAccountInfo, u);
    } else {
        printf("\nAccount not found!\n");
        stayOrReturn(0, checkAccountInfo, u);
        system("clear");
    }
}

//##############################################################################################################################################################

//##############################################################################################################################################################
void makeTransaction(struct User u) {
    int accountId, choice;
    double amount;
    struct Record r;
    FILE *pf = fopen(RECORDS, "r+");
    int found = 0;

    if (pf == NULL) {
        printf("Error opening file!\n");
        stayOrReturn(1, mainMenu, u);
        return;
    }

    system("clear");
    printf("\t\t====== Make Transaction =====\n\n");
    printf("\nEnter the account number: ");
    scanf("%d", &accountId);

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        if (r.accountNbr == accountId) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nAccount not found!\n");
        fclose(pf);
        stayOrReturn(1, mainMenu, u);
        return;
    }

    while (1) {
        printf("\nChoose the type of transaction:\n");
        printf("\t1. Deposit\n");
        printf("\t2. Withdraw\n");
        printf("\t3. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: {
                system("clear");
                printf("\nEnter the amount to deposit: ");
                scanf("%lf", &amount);
                
                if (amount <= 0) {
                    printf("\nInvalid amount. Please enter a positive number.\n");
                    continue;
                }
                
                r.amount += amount;
                break;
            }
            case 2: {
                system("clear");
                printf("\nEnter the amount to withdraw: ");
                scanf("%lf", &amount);
                
                if (amount <= 0 || amount > r.amount) {
                    printf("\nInvalid amount. Please enter a positive number less than or equal to your current balance.\n");
                    continue;
                }
                
                r.amount -= amount;
                break;
            }
            case 3:
                fclose(pf);
                stayOrReturn(1, mainMenu, u);
                return;
            default:
                system("clear");
                printf("\nInvalid choice. Please try again.\n");
                continue;
        }

        rewind(pf);
        FILE *tempFile = fopen("temp.txt", "w");
        if (tempFile == NULL) {
            printf("Error creating temporary file.\n");
            fclose(pf);
            stayOrReturn(1, mainMenu, u);
            return;
        }

        struct Record tempRecord;
        while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                      &tempRecord.id, &tempRecord.userId, tempRecord.name, &tempRecord.accountNbr,
                      &tempRecord.deposit.day, &tempRecord.deposit.month, &tempRecord.deposit.year,
                      tempRecord.country, &tempRecord.phone, &tempRecord.amount, tempRecord.accountType) == 11) {
            
            if (tempRecord.accountNbr == accountId) {
                fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                        r.id, r.userId, r.name, r.accountNbr,
                        r.deposit.day, r.deposit.month, r.deposit.year,
                        r.country, r.phone, r.amount, r.accountType);
            } else {
                fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                        tempRecord.id, tempRecord.userId, tempRecord.name, tempRecord.accountNbr,
                        tempRecord.deposit.day, tempRecord.deposit.month, tempRecord.deposit.year,
                        tempRecord.country, tempRecord.phone, tempRecord.amount, tempRecord.accountType);
            }
        }

        fclose(pf);
        fclose(tempFile);

        remove(RECORDS);
        rename("temp.txt", RECORDS);

        printf("\nTransaction completed successfully.\n");
        stayOrReturn(1, mainMenu, u);
        return;
    }
}

//##############################################################################################################################################################
void removeAccount(struct User u);

//##############################################################################################################################################################
void transferOwner(struct User u);

//##############################################################################################################################################################
//##############################################################################################################################################################
int IsNOTDigit(const char *valid) {
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
    static int accountNumber = 10;
    return ++accountNumber;
}

//##############################################################################################################################################################
void clear_buffer(char *buffer, size_t size) {
    memset(buffer, 0, size);
}

//##############################################################################################################################################################
double getValidAmount(double amount) {
    char input[100];
    while (1) {
        printf("\nEnter amount: $");
        scanf("%s", input);
        if (sscanf(input, "%le", &amount) == 1 && amount > 0) {
            return amount;
        }
        printf("Invalid input. Please enter a positive number.\n");
    }
}

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
int IsPrintableName(const char *str) {
    int i = 0;
    while (str[i]) {
        if (!((str[i] >= 32 && str[i] <= 126) || str[i] == '\n')) {
            return 0;
        }
        i++;
    }
    return 1;
}

//################################################################################################################################################
int isValidAccountType(const char* type) {
    return (strcmp(type, "saving") == 0 || strcmp(type, "current") == 0 ||
            strcmp(type, "fixed01") == 0 || strcmp(type, "fixed02") == 0 ||
            strcmp(type, "fixed03") == 0);
}

//################################################################################################################################################
int isValidDate(int day, int month, int year) {
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if(year < 2000 || year > 2050) return false;
    
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
//****************************************************************************************************************************************************************

int Onlyalphabetical(char *str) {
    int i = 0;
    while (str[i]) {
        if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))) {
            return 0;
        }
        i++;
    }

    return 1;
}


void promptForCountry(struct Record *newAccount) {
    while (true) {
        printf("\nEnter the country: ");
        scanf("%s", newAccount->country);
        
        if (!Onlyalphabetical(newAccount->country) || isValidCountry(newAccount->country)) {
            clearScreen();
            printf("\nInvalid country name. Country should be 2-99 characters and contain only letters.\n");
        } else {
            break;
        }
    }
}

void promptForPhone(struct Record *newAccount) {
    while (true) {
        printf("\nEnter the phone number: ");
        scanf("%d", &newAccount->phone);
        
        if (newAccount->phone < 100000000 || newAccount->phone > 999999999) {
            clearScreen();
            printf("\nInvalid phone number. Please enter a 9-digit number.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
}

void promptForAmount(struct Record *newAccount) {
    while (true) {
        printf("\nEnter the amount to deposit: $");
        if (scanf("%lf", &newAccount->amount) != 1 || newAccount->amount < 0) {
            clearScreen();
            printf("\nInvalid amount. Please enter a positive number.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
}

void promptForAccountType(struct Record *newAccount) {
    while (true) {
        printf("\nEnter the type of account : \n\t\t\t=====> . saving\n\t\t\t=====> . current\n\t\t\t=====> . fixed01\n\t\t\t=====> . fixed02\n\t\t\t=====> . fixed03\n=================>\t  ");
        scanf("%9s", newAccount->accountType);
        
        if (!isValidAccountType(newAccount->accountType)) {
            clearScreen(); 
            printf("\nInvalid account type. Please choose from the given options.\n");
        } else {
            break;
        }
    }
}

void promptForDate(struct Record *newAccount) {
    while (true) {
        printf("\nEnter today's date (dd/mm/yyyy): ");
        scanf("%d/%d/%d", &newAccount->deposit.day, &newAccount->deposit.month, &newAccount->deposit.year);
        
        if (!isValidDate(newAccount->deposit.day, newAccount->deposit.month, newAccount->deposit.year)) {
            clearScreen();
            printf("\nInvalid date format or date. Please use dd/mm/yyyy format.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
}

bool isValidPhone(int phone) {
    char phoneStr[11];
    snprintf(phoneStr, sizeof(phoneStr), "%d", phone);
    return phone >= 100000000 && phone <= 999999999 && IsNOTDigit(phoneStr);
}

bool isValidCountry(const char *country) {
    return strlen(country) > 1 && strlen(country) < 99 && 
           IsNOTDigit(country) && IsPrintableName(country);
}

void clearScreen() {
    system("clear");
}