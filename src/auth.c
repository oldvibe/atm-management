#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char *a, char *pass)
{
    system("clear");
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", a);
    printf("\n\n\n\t\t\tEnter your password:");
    scanf("%s", pass);
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            //should the variable be static to can use the address in the initMenu function
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}