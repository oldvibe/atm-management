#include "header.h"

void mainMenu(struct User u)
{
    int option;
    system("clear");
      printf("              ╔═════════════════════════════════════════════════════╗\n");
    printf("              |                                                     |\n");
    printf("              |                ======= ATM =======                  |\n");
    printf("              |                                                     |\n");
    printf("              |                                                     |\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              | ->> Feel free to choose one of the options below <<-|\n");
    printf("              |                                                     |\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              | [1] - Create a new account                          |\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              | [2] - Update account information                    |\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              | [3] - Check accounts                                |\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              | [4] - Check list of owned accounts                  |\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              | [5] - Make Transaction                              |\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              | [6] - Remove existing account                       |\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              | [7] - Transfer ownership                            |\n");
    printf("              |═════════════════════════════════════════════════════|\n");
    printf("              | [8] - Exit                                          |\n");
    printf("              |                                                     |\n");
    printf("              ╚═════════════════════════════════════════════════════╝\n\n\n");
    printf("              ========> ");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
       system("clear");
        createNewAcc(u);
        break;
    case 2:
        // student TODO : add your **Update account information** function
        system("clear");
       updateAccount(u);
        // here
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        checkAccountInfo(u);
        // here
        break;
    case 4:
    system("clear");
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
        //removeAccount(u);
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        //transferOwner(u);
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        exit(1);
        break;
    default:
            printf("\nInvalid choice.\n");
            mainMenu(u);
    }
};

void initMenu(struct User *u)
{
    int r = 0;
    int option;
    system("clear");
   printf("              ╔═════════════════════════════════════════════╗\n");
    printf("              |             ======= ATM =======             |\n");
    printf("              |                                             |\n");
    printf("              |                                             |\n");
    printf("              |       -->> Feel free to login / register :  |\n");
    printf("              |                                             |\n");
    printf("              |                [1] - Login                  |\n");
    printf("              |                                             |\n");
    printf("              |                [2] - Register               |\n");
    printf("              |                                             |\n");
    printf("              |                [3] - Exit                   |\n");
    printf("              |                                             |\n");
    printf("              ╚═════════════════════════════════════════════╝\n\n\n");
    printf("              ========> ");
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            login(u->name, u->password);
            r = 1;
            break;
        case 2:
            system("clear");
            registerMenu(u->name, u->password);
            r = 1;
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
            exit(0);
        }
    }
};

int main()
{
    struct User u;
    
    initMenu(&u);
    mainMenu(u);

    return 0;
}