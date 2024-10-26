#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
    char confirmPassword[50];
};

void putAccountToFile(FILE *ptr, const char *name, struct Record r);

// authentication functions
void login(char *a, char *pass);
void registerMenu();
//get the information of the new user (the username must be unique)

const char *getPassword(struct User u);

// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void initMenu(struct User *u);


//this function dosn't work, it must print the informations of 
//all the account that appear under the user (use the username to check) 
void checkAllAccounts(struct User u);

// this function ask the user for the account he want to change in it
// after checking he is the owner of the account
// you must ask him for what he want to change (number or country)
//create a new file with all the informations in the record file
// except the information of the that account, must be writing updated
// after that remove the old one and rename the new file to the old name (record.txt)
void updateAccount(struct User u);

// in this function you must ask the user for the account number he want 
// to check information, if he is the owner of it give him all the information about it, 
// inter = amount * interet_percentage/12.              plus
//the interet he will get every month if he get it, if he don't (current)
//tell them that the account is can't get interest
void checkAccountInfo(struct User u);

//in this functin you must print all the informations of 
//all the account that owned by the user
void checkAllAccounts(struct User u);

// in this function you must ask the user for the account number 
// after checking he is the owner of the account, ask him if he want
// to deposit or withraw. then the budget
// if he enter deposit add the budget to the old amount, else the opposit
// and recreate a file with the ammount edited
void makeTransaction(struct User u);

// in this function you must to ask the user for the accountNumber 
// he want to remove, after checking he is the owner recreate the record file
//without this account
void removeAccount(struct User u);

//in this function ask the user for the account he want to 
// transfer it, after chekcing if he is the owner, ask him 
// who the one that will have the account, after checking if the user 
// is already in our file (users.txt) recreate the record file 
//but this specific file with the new username and new user id
void transferOwner(struct User u);

int generateAccountNumber();
int IsDigit(const char* phone);
double getValidAmount(double amount);
const int GetUserId();
const int GetRecordId();
bool IsPrintableName(char *str);
bool IsPrintablePassword(char *str);