#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>

void TextColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // We use csbi for the wAttributes word.
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        // Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

enum Status
{
    SUCCESS,
    FAIL
};

// general structure
struct CustomerInfo
{
    int acountNo;           // 6 digit
    char firstName[15];     // max 15 characters
    char lastName[15];      // max 15 characters
    int age;                // 2 digits >= 18
    char contactNumber[11]; // exactly 11 characters //contact number in char because we want to include ''0'' as well and because no calculations will be done on contact number
    int accountStatus;      // 0 - blocked, 1 - active
    int amount;             // max 20 digits
    char password[9];
};

struct AdminInfo
{
    int id;
    char firstName[15];
    char lastName[15];
    char password[9];
};

struct CurrencyInfo
{
    int cNo;
    char first[11];
    char last[11];
    char code[4];
    float rate;
};

enum Status adminID;
enum Status adminPass;
enum Status accountNoGenerated;
enum Status customerID;
enum Status customerPass;
enum Status accountFound;

struct CustomerInfo customer;
struct CustomerInfo sender;
struct CustomerInfo receiver;
struct AdminInfo admin;
struct CustomerInfo update;
struct CurrencyInfo currency;

int i, j;
char ch;

// function prototypes
void CursorPosition(int x, int y);  //goto(x,y)  imported function
void currentDateAndTime();         // imported function
int mainMenu();
void loginAsAdmin();
void createNewAccount();
int generateAccountNumber();
void loginAsCustomer();
void customerPortal();
void depositMoney();
int customerPortalMenu();
void loadingAnimation();
void withdrawAmount();
void transferAmount();
void deleteAccount();
int integerInputOnly();
void viewTransactionHistory();
void viewCurrencyRates();
void adminDeleteAccount();
void adminPortal();
int adminPortalMenu();
void viewCurrentAccInfo();
void aboutUs();
void updateCurrencyRates();
void createCustomerDataBaseBackup();
void createCustomerDataBaseBackupAnimation();
void viewMyTransactionHistory();
void searchCustomer();
void searchByAccountNumber();
void searchByName();
void bankPolicy();
void sortAsc();
void sortDes();

// File Pointers
FILE *fp;
FILE *temp;
FILE *transaction;
FILE *backup;

int main()
{
    // system("mode 120, 30");
    int mainMenuChoice;
    while (1)
    {
        mainMenuChoice = mainMenu();
        switch (mainMenuChoice)
        {
        case 1:
            system("cls");
            system("title Admin Login");
            loginAsAdmin();
            break;
        case 2:
            system("cls");
            system("title Customer Login");
            loginAsCustomer();
            break;
        case 3:
            system("cls");
            system("title Create New Account");
            createNewAccount();
            break;
        case 4:
            system("cls");
            system("title Today's Currency Rates");
            viewCurrencyRates();
            break;
        case 5:
            system("cls");
            system("title About Us");
            aboutUs();
            break;
        case 6:
            system("cls");
            CursorPosition(40, 10);
            system("title Good Bye");
            printf("Thankyou for using our service, :)");
            Sleep(2000);
            exit(0);
            break;
        default:
            system("cls");
            system("title ERROR :(");
            CursorPosition(0, 0);              //goto(x,y)
            system("color 4F");
            printf("Wrong choice entered, try again!");
            Beep(1000, 300);
            Sleep(2000);
            system("color 0F");
            break;
        }
    }
    return 0;
}

void CursorPosition(int x, int y)
{

    COORD c;
    c.X = x;
    c.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int mainMenu()
{
    int choice;
    system("color 0F");
    system("cls");
    system("title FAST NUCES BANK - MAIN MENU");
    CursorPosition(0, 0);
    TextColor(10);
    currentDateAndTime();
    CursorPosition(32, 3);
    TextColor(11);
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 FAST-NUCES BANK \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    for (i = 1; i <= 7; i++)
    {
        CursorPosition(31, 3 + i);
        printf("|");
    }
    for (i = 1; i <= 7; i++)
    {
        CursorPosition(91, 3 + i);
        printf("|");
    }
    for (i = 1; i <= 60; i++)
    {
        CursorPosition(31 + i, 11);
        printf("-");
    }
    TextColor(15);
    CursorPosition(33, 5);
    printf("1. Login as Admin");
    CursorPosition(33, 6);
    printf("2. Login as Customer");
    CursorPosition(33, 7);
    printf("3. Create new account");
    CursorPosition(33, 8);
    printf("4. Check today's currency rates");
    CursorPosition(33, 9);
    printf("5. About Us");
    CursorPosition(33, 10);
    printf("6. Exit");
    CursorPosition(32, 15);
    printf("Enter your choice: ");
    fflush(stdin);
    scanf("%d", &choice);
    return choice;
}

void currentDateAndTime()
{
    time_t t; // not a primitive datatype
    time(&t);

    printf("%s", ctime(&t));
}

void loginAsAdmin()
{
    int backSpaceCount = 0;
    adminID = FAIL;
    adminPass = FAIL;
    int id;
    char password[9];
    printf("Enter ID: ");
    TextColor(11);
    fflush(stdin);
    scanf("%d", &id);
    TextColor(15);
    system("cls");
    fp = fopen("admin.txt", "r");
    fseek(fp, 0, SEEK_SET);
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }

    while (fscanf(fp, "%6d %15s %15s %8s\n", &admin.id, admin.firstName, admin.lastName, admin.password) != EOF)
    {
        if (id == admin.id)
        {
            adminID = SUCCESS;
            printf("Enter password: ");
            TextColor(11);
            for (i = 0; i <= 7;)
            {
                ch = getch();
                if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch >= 33 && ch <= 47))
                {
                    password[i] = ch;
                    ch = '*';
                    printf("%c", ch);
                    i++;
                    backSpaceCount++;
                }
                else if (ch == 8 && backSpaceCount > 0)
                {
                    printf("\b \b");
                    backSpaceCount--;
                    i--;
                }
            }
            system("color 0F");
            password[8] = '\0';
            if ((strcmp(password, admin.password)) == 0)
            {
                adminPass = SUCCESS;
                system("cls");
                fclose(fp);
                loadingAnimation();
                adminPortal();
                break;
            }
        }
    }
    fclose(fp);
    if (adminID == FAIL)
    {
        system("cls");
        printf("This ID does not exists, try again \a");
        Beep(1000, 300);
        Sleep(1500);
        system("cls");
        loginAsAdmin();
    }
    if (adminPass == FAIL)
    {
        system("cls");
        printf("Wrong Password, try again \a");
        Beep(1000, 300);
        Sleep(1500);
        system("cls");
        loginAsAdmin();
    }
}

void adminPortal()
{
    system("cls");
    int adminPortalChoice, id = admin.id;

    while (1)
    {
        fp = fopen("admin.txt", "r");
        if (fp == NULL)
        {
            system("cls");
            perror("Error");
            printf("\nProgram will exit\n");
            Sleep(2000);
            exit(1);
        }
        fseek(fp, 0, SEEK_SET);
        while (fscanf(fp, "%6d %15s %15s %8s\n", &admin.id, admin.firstName, admin.lastName, admin.password) != EOF)
        {
            if (id == admin.id)
            {
                break;
            }
        }
        fclose(fp);
        adminPortalChoice = adminPortalMenu();
        switch (adminPortalChoice)
        {
        case 1:
            system("title MY INFO");
            system("color 0B");
            system("cls");
            printf("Name: %s %s\n", admin.firstName, admin.lastName);
            printf("ID: %d\n", admin.id);

            printf("\nPress any key to go to your portal\n");
            getch();
            break;
        case 2:
            system("cls");
            system("title CUSTOMER ACCOUNTS INFO");
            viewCurrentAccInfo();
            break;
        case 3:
            system("cls");
            system("title TRANSACTION HISTORY");
            viewTransactionHistory();
            break;
        case 4:
            system("cls");
            system("title DELETE ACCOUNT");
            adminDeleteAccount();
            break;
        case 5:
            system("cls");
            system("title UPDATE CURRENCY RATES");
            updateCurrencyRates();
            break;
        case 6:
            system("cls");
            system("title SEARCH");
            searchCustomer();
            break;
        case 7:
            system("cls");
            system("title CREATING DATABASES BACKUP");
            createCustomerDataBaseBackupAnimation();
            createCustomerDataBaseBackup();
            break;
        case 8:
            main();
            break;
        default:
            system("cls");
            system("title ERROR");
            CursorPosition(0, 0);
            system("color 4F");
            printf("\aWrong choice entered, try again! \a");
            Sleep(1500);
            system("color 0F");
            break;
        }
    }
}

int adminPortalMenu()
{
    int choice;
    system("color 0F");
    system("cls");
    system("title ADMIN PORTAL");
    CursorPosition(0, 0);
    TextColor(10);
    currentDateAndTime();
    CursorPosition(0, 2);
    TextColor(9);
    printf("Welcome, Mr %s %s\n", admin.firstName, admin.lastName);
    CursorPosition(32, 5);
    TextColor(3);
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 FAST-NUCES BANK \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    for (i = 1; i <= 16; i++)
    {
        CursorPosition(31, 5 + i);
        printf("|");
    }
    for (i = 1; i <= 16; i++)
    {
        CursorPosition(91, 5 + i);
        printf("|");
    }
    for (i = 1; i <= 60; i++)
    {
        CursorPosition(31 + i, 22);
        printf("-");
    }
    TextColor(15);
    CursorPosition(33, 7);
    printf("1. View my information");
    CursorPosition(33, 9);
    printf("2. View current accounts information");
    CursorPosition(33, 11);
    printf("3. View Transaction History");
    CursorPosition(33, 13);
    printf("4. Delete Account");
    CursorPosition(33, 15);
    printf("5. Update Currency Rates");
    CursorPosition(33, 17);
    printf("6. Search and Sort Customers");
    CursorPosition(33, 19);
    printf("7. Create Customer Database Backup");
    CursorPosition(33, 21);
    printf("8. Logout");
    CursorPosition(32, 24);
    printf("Enter your choice: ");
    fflush(stdin);
    scanf("%d", &choice);
    return choice;
}

void createNewAccount()
{
    char ch, choice;
    int backSpaceCount = 0;
    i = 0;
    system("title CREATE NEW ACCOUNT");
    bankPolicy();
    customer.acountNo = generateAccountNumber();
    system("cls");
    do
    {
        system("cls");
        printf("Enter your first name: ");
        fflush(stdin);
        scanf("%s", customer.firstName);
        if ((strlen(customer.firstName)) >= 15)
        {
            printf("\nLength of name must be less than 15 \a");
            Sleep(1500);
        }
    } while ((strlen(customer.firstName)) >= 15);

    do
    {
        system("cls");
        printf("Enter your last name: ");
        fflush(stdin);
        scanf("%s", customer.lastName);
        if ((strlen(customer.firstName)) >= 15)
        {
            printf("\nLength of name must be less than 15 \a");
            Sleep(1500);
        }
    } while ((strlen(customer.lastName)) >= 15);

    do
    {
        system("cls");
        printf("Enter your age: ");
        fflush(stdin);
        // scanf("%d", &customer.age);
        customer.age = integerInputOnly();
        if (customer.age < 18 || customer.age > 99)
        {
            printf("\nPeople between the age 18 and 99 can only create an account\nTry Again! \a");
            Sleep(2000);
        }
    } while (customer.age < 18 || customer.age > 99);

    do
    {
        system("cls");
        printf("Enter your phone number: ");
        fflush(stdin);
        scanf("%s", customer.contactNumber);
        if ((strlen(customer.contactNumber)) != 11)
        {
            printf("\nInvalid contact number \a");
            Sleep(1000);
        }
    } while ((strlen(customer.contactNumber)) != 11);
    customer.accountStatus = 0;
    customer.amount = 0;

    system("cls");
    printf("Enter your password (must be exactly 8 characters): ");
    TextColor(11);
    for (i = 0; i <= 7;)
    {
        ch = getch();
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch >= 33 && ch <= 47))
        {
            customer.password[i] = ch;
            ch = '*';
            printf("%c", ch);
            i++;
            backSpaceCount++;
        }
        else if (ch == 8 && backSpaceCount > 0)
        {
            printf("\b \b");
            i--;
            backSpaceCount--;
        }
    }
    customer.password[8] = '\0';
    system("color 0F");
    system("cls");
    TextColor(15);
    printf("Account Number  : ");
    TextColor(9);
    printf("%d\n", customer.acountNo);
    TextColor(15);
    printf("Name            : ");
    TextColor(10);
    printf("%s %s\n", customer.firstName, customer.lastName);
    TextColor(15);
    printf("Age             : ");
    TextColor(10);
    printf("%d\n", customer.age);
    TextColor(15);
    printf("Contact Number  : ");
    TextColor(10);
    printf("%s\n", customer.contactNumber);
    TextColor(15);
    printf("Account Balance : ");
    TextColor(10);
    printf("Rs. %d\n", customer.amount);

    TextColor(1);
    printf("\nAre your sure you want to create your account: [y/n]: ");
    while (1)
    {
        ch = getch();
        if (ch == 'y' || ch == 'Y' || ch == 'n' || ch == 'N')
        {
            choice = ch;
            printf("%c", ch);
            break;
        }
    }
    system("color 0F");
    if (choice == 'y' || choice == 'Y')
    {
        fp = fopen("customer.txt", "a");
        if (fp == NULL)
        {
            system("cls");
            perror("Error");
            printf("\nProgram will exit\n");
            Sleep(2000);
            exit(1);
        }
        fprintf(fp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", customer.acountNo, customer.firstName, customer.lastName, customer.age, customer.contactNumber, customer.accountStatus, customer.amount, customer.password);
        fclose(fp);
        system("cls");
        system("color 0A");
        printf("Account created successfully\n");
        Sleep(1000);
    }
}

int generateAccountNumber()
{
    int acc;
    fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
check:
    fseek(fp, 0, SEEK_SET);
    srand(time(0));
    do
    {
        acc = (rand() % 1000) + 100000;
    } while (acc < 100000 || acc > 999999);

    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &customer.acountNo, customer.firstName, customer.lastName, &customer.age, customer.contactNumber, &customer.accountStatus, &customer.amount, customer.password) != EOF)
    {
        if (acc == customer.acountNo)
        {
            goto check;
        }
        else
        {
            goto ganend;
        }
    }
ganend:
    fclose(fp);
    return acc;
}

void loginAsCustomer()
{
    int backSpaceCount = 0;
    // customer.acountNo = -1;
    // for(i=0;i<=7;i++){
    //     customer.password[i] = 'x';
    // }
    // customer.password[8] = '\0';
    customerID = FAIL;
    customerPass = FAIL;
    int accNo = 0;
    char password[9];
    printf("Enter Account Number: ");
    TextColor(11);
    fflush(stdin);
    scanf("%d", &accNo);
    system("cls");
    TextColor(15);
    fp = fopen("customer.txt", "r");
    fseek(fp, 0, SEEK_SET);
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &customer.acountNo, customer.firstName, customer.lastName, &customer.age, customer.contactNumber, &customer.accountStatus, &customer.amount, customer.password) != EOF)
    {
        if (accNo == customer.acountNo)
        {
            customerID = SUCCESS;
            system("color 0F");
            printf("Enter password: ");
            TextColor(11);
            for (i = 0; i < 8;)
            {
                ch = getch();
                if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch >= 33 && ch <= 47))
                {
                    password[i] = ch;
                    ch = '*';
                    printf("%c", ch);
                    i++;
                    backSpaceCount++;
                }
                else if (ch == 8 && backSpaceCount > 0)
                {
                    printf("\b \b");
                    backSpaceCount--;
                    i--;
                }
            }
            password[8] = '\0';
            if ((strcmp(password, customer.password)) == 0)
            {
                customerPass = SUCCESS;
                system("cls");
                fclose(fp);
                loadingAnimation();
                customerPortal();
                break;
            }
        }
    }
    fclose(fp);
    if (customerID == FAIL)
    {
        system("cls");
        printf("This ID does not exists, try again");
        Beep(1000, 300);
        Sleep(1500);
        system("cls");
        loginAsCustomer();
    }
    if (customerPass == FAIL)
    {
        system("cls");
        printf("Error: Wrong Password, retry");
        Beep(1000, 300);
        Sleep(1500);
        system("cls");
        loginAsCustomer();
    }
}

void customerPortal()
{
    system("cls");
    int customerPortalChoice, acc_No = customer.acountNo;

    while (1)
    {
        fp = fopen("customer.txt", "r");
        if (fp == NULL)
        {
            system("cls");
            perror("Error");
            printf("\nProgram will exit\n");
            Sleep(2000);
            exit(1);
        }
        fseek(fp, 0, SEEK_SET);
        while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &customer.acountNo, customer.firstName, customer.lastName, &customer.age, customer.contactNumber, &customer.accountStatus, &customer.amount, customer.password) != EOF)
        {
            if (acc_No == customer.acountNo)
            {
                break;
            }
        }
        fclose(fp);
        customerPortalChoice = customerPortalMenu();
        switch (customerPortalChoice)
        {
        case 1:

            system("color 0B");
            system("cls");
            system("title MY INFO");
            printf("Name: ");
            printf("%s %s\n", customer.firstName, customer.lastName);
            printf("Account Number  : %d\n", customer.acountNo);
            printf("Age             : %d\n", customer.age);
            printf("Contact Number  : %s\n", customer.contactNumber);
            printf("Account Status  : %s\n", (customer.accountStatus == 0 ? "Active" : "Blocked"));
            printf("Account Balance : %d\n", customer.amount);

            printf("\nPress any key to go to your portal\n");
            getch();
            system("color 0F");
            // customerPortal();
            break;
        case 2:
            system("cls");
            system("title DEPOSIT AMOUNT");
            depositMoney();
            break;
        case 3:
            system("cls");
            system("title WITHDRAW AMOUNT");
            withdrawAmount();
            break;
        case 4:
            system("cls");
            system("title TRANSFER AMOUNT");
            transferAmount();
            break;
        case 5:
            system("cls");
            system("title VIEW TRANSACTION HISTORY");
            viewMyTransactionHistory();
            break;
        case 6:
            system("cls");
            system("title DELETE ACCOUNT");
            deleteAccount();
            break;
        case 7:
            main();
            break;
        default:
            system("cls");
            system("title ERROR");
            CursorPosition(0, 0);
            system("color 4F");
            printf("\aWrong choice entered, try again! \a");
            Sleep(1500);
            system("color 0F");
            break;
        }
    }
}

int customerPortalMenu()
{
    int choice;
    system("color 0F");
    system("cls");
    system("title MY PORTAL");
    CursorPosition(0, 0);
    TextColor(10);
    currentDateAndTime();
    CursorPosition(0, 2);
    TextColor(9);
    printf("Welcome, Mr %s %s\n", customer.firstName, customer.lastName);
    printf("Current Balance: Rs. %d", customer.amount);
    CursorPosition(32, 5);
    TextColor(11);
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 FAST-NUCES BANK \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    for (i = 1; i <= 14; i++)
    {
        CursorPosition(31, 5 + i);
        printf("|");
    }
    for (i = 1; i <= 14; i++)
    {
        CursorPosition(91, 5 + i);
        printf("|");
    }
    for (i = 1; i <= 60; i++)
    {
        CursorPosition(31 + i, 20);
        printf("-");
    }
    TextColor(15);
    CursorPosition(33, 7);
    printf("1. View my information");
    CursorPosition(33, 9);
    printf("2. Deposit Money");
    CursorPosition(33, 11);
    printf("3. Withdraw Money");
    CursorPosition(33, 13);
    printf("4. Transfer Money");
    CursorPosition(33, 15);
    printf("5. View My Transaction History");
    CursorPosition(33, 17);
    printf("6. Delete Account");
    CursorPosition(33, 19);
    printf("7. Logout");
    CursorPosition(32, 22);
    TextColor(5);
    printf("Enter your choice: ");
    fflush(stdin);
    scanf("%d", &choice);
    system("color 0F");
    return choice;
}

void depositMoney()
{
    int newAmount, tempAcc;
    long long int amountToDeposit;
    amountToDeposit = 0;
    tempAcc = customer.acountNo;
    system("cls");
    do
    {
        system("cls");
        printf("Enter amount to deposit(-1 to go back): Rs. ");
        fflush(stdin);
        scanf("%lld", &amountToDeposit);
        if ((customer.amount + amountToDeposit) > 999999999)
        {
            system("cls");
            // add red color here
            printf("Error: Account Limit reached. Enter a smaller amount");
            Sleep(1000);
        }
        else if (amountToDeposit < 0 && amountToDeposit != -1)
        {
            system("cls");
            printf("Error: Amount can not be negative");
            Sleep(1000);
        }
        else if (amountToDeposit == -1)
        {
            system("cls");
            goto depositMoneyEnd;
        }
    } while ((customer.amount + amountToDeposit > 999999999) || amountToDeposit < 0);
    fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    temp = fopen("customerTemp.txt", "w");
    if (temp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    transaction = fopen("transaction_history.txt", "a");
    if (transaction == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }

    newAmount = customer.amount + amountToDeposit;
    fseek(fp, 0, SEEK_SET);
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
    {
        if (tempAcc == update.acountNo)
        {
            fprintf(temp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", update.acountNo, update.firstName, update.lastName, update.age, update.contactNumber, update.accountStatus, newAmount, update.password);
            fprintf(transaction, "%-6d %-15s %-15s %-9d Deposit\n", update.acountNo, update.firstName, update.lastName, amountToDeposit);
        }
        else
        {
            fprintf(temp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", update.acountNo, update.firstName, update.lastName, update.age, update.contactNumber, update.accountStatus, update.amount, update.password);
        }
    }
    system("cls");
    printf("Rs. %d deposited successfully in your account\n", amountToDeposit);
    Sleep(2000);
    fclose(temp);
    fclose(fp);
    fclose(transaction);
    remove("customer.txt");
    // if (remove("customer.txt") == 0)
    //     printf("Success\n");
    // else
    //     perror("Unable to delete the file");
    rename("customerTemp.txt", "customer.txt");
depositMoneyEnd:
    system("cls");
}

void loadingAnimation()
{
    int i;
    system("cls");
    system("color 0A");
    CursorPosition(56, 10);
    printf("Loading");
    CursorPosition(40, 12);
    printf("[");
    CursorPosition(78, 12);
    printf("]");
    CursorPosition(41, 12);
    for (i = 1; i <= 37; i++)
    {
        printf("%c", 177);
    }
    CursorPosition(41, 12);
    for (i = 1; i <= 37; i++)
    {
        Sleep(30);
        printf("%c", 219);
    }
    CursorPosition(0, 0);
    system("cls");
    system("color 0F");
}

void withdrawAmount()
{
    int newAmount, tempAcc;
    long long int amountToWithdraw;
    amountToWithdraw = 0;
    tempAcc = customer.acountNo;
    system("cls");
    do
    {
        system("cls");
        printf("Enter amount to withdraw(-1 to go back): Rs. ");
        fflush(stdin);
        scanf("%lld", &amountToWithdraw);
        if (amountToWithdraw > customer.amount)
        {
            system("cls");
            // add red color here
            printf("Error: Not enough account balance");
            Sleep(1000);
        }
        else if (amountToWithdraw < 0 && amountToWithdraw != -1)
        {
            system("cls");
            printf("Error: Amount can not be negative");
            Sleep(1000);
        }
        else if (amountToWithdraw == -1)
        {
            goto withdrawAmountEnd;
        }
    } while (amountToWithdraw > customer.amount || amountToWithdraw < 0);
    fp = fopen("customer.txt", "r");
    temp = fopen("customerTemp.txt", "w");
    transaction = fopen("transaction_history.txt", "a");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    if (temp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    if (transaction == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    newAmount = customer.amount - amountToWithdraw;
    fseek(fp, 0, SEEK_SET);
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
    {
        if (tempAcc == update.acountNo)
        {
            fprintf(temp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", update.acountNo, update.firstName, update.lastName, update.age, update.contactNumber, update.accountStatus, newAmount, update.password);
            fprintf(transaction, "%-6d %-15s %-15s %-9d Withdraw\n", update.acountNo, update.firstName, update.lastName, amountToWithdraw);
        }
        else
        {
            fprintf(temp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", update.acountNo, update.firstName, update.lastName, update.age, update.contactNumber, update.accountStatus, update.amount, update.password);
        }
    }
    system("cls");
    printf("Rs. %d withdrawn successfully from your account\n", amountToWithdraw);
    Sleep(2000);
    fclose(temp);
    fclose(fp);
    fclose(transaction);
    remove("customer.txt");
    // if (remove("customer.txt") == 0)
    //     printf("Success\n");
    // else
    //     perror("Unable to delete the file");
    rename("customerTemp.txt", "customer.txt");
withdrawAmountEnd:
    system("cls");
}

void transferAmount()
{
    int accNoReceiver;
    long long int amountToTransfer;
    amountToTransfer = 0;
    accountFound = FAIL;
    sender.acountNo = customer.acountNo;
    strcpy(sender.firstName, customer.firstName);
    strcpy(sender.lastName, customer.lastName);
    sender.age = customer.age;
    strcpy(sender.contactNumber, customer.contactNumber);
    sender.accountStatus = customer.accountStatus;
    sender.amount = customer.amount;
    strcpy(sender.password, customer.password);
    system("cls");
    while (1)
    {
        printf("Enter receiver's account number (-1 to go back): ");
        fflush(stdin);
        scanf("%d", &accNoReceiver);
        if (accNoReceiver == -1)
        {
            goto transferAmountEnd;
        }
        else if (accNoReceiver != sender.acountNo)
        {
            break;
        }
        else
        {
            system("cls");
            system("color 04");
            printf("\aError! You can not transfer to yourself, please enter a valid account no\n");
            Sleep(1000);
            system("cls");
            system("color 0F");
        }
    }
    fp = fopen("customer.txt", "a+");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &receiver.acountNo, receiver.firstName, receiver.lastName, &receiver.age, receiver.contactNumber, &receiver.accountStatus, &receiver.amount, receiver.password) != EOF)
    {
        if (accNoReceiver == receiver.acountNo)
        {
            accountFound = SUCCESS;
            break;
        }
        else
        {
            accountFound = FAIL;
        }
    }

    fseek(fp, 0, SEEK_SET);
    if (accountFound == SUCCESS)
    {
        printf("Enter amount to transfer: ");
        fflush(stdin);
        scanf("%lld", &amountToTransfer);
        if(amountToTransfer <= 0){
            printf("Error, amount can not be negative\n");
            Sleep(2000);
            fclose(fp);
            goto transferAmountEnd;
        }
        if (sender.amount >= amountToTransfer)
        {
            if (receiver.amount + amountToTransfer <= 999999999)
            {
                temp = fopen("customerTemp.txt", "w");
                transaction = fopen("transaction_history.txt", "a");
                if (temp == NULL)
                {
                    system("cls");
                    perror("Error");
                    printf("\nProgram will exit\n");
                    Sleep(2000);
                    exit(1);
                }
                if (transaction == NULL)
                {
                    system("cls");
                    perror("Error");
                    printf("\nProgram will exit\n");
                    Sleep(2000);
                    exit(1);
                }
                fprintf(transaction, "%-6d %-15s %-15s %-9d Transfer\n", sender.acountNo, sender.firstName, sender.lastName, amountToTransfer);
                while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
                {
                    if (update.acountNo == sender.acountNo)
                    {
                        fprintf(temp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", sender.acountNo, sender.firstName, sender.lastName, sender.age, sender.contactNumber, sender.accountStatus, sender.amount - amountToTransfer, sender.password);
                    }
                    else if (update.acountNo == receiver.acountNo)
                    {
                        fprintf(temp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", receiver.acountNo, receiver.firstName, receiver.lastName, receiver.age, receiver.contactNumber, receiver.accountStatus, receiver.amount + amountToTransfer, receiver.password);
                    }
                    else
                    {
                        fprintf(temp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", update.acountNo, update.firstName, update.lastName, update.age, update.contactNumber, update.accountStatus, update.amount, update.password);
                    }
                }
                system("cls");
                TextColor(15);
                printf("%d ", amountToTransfer);
                TextColor(10);
                printf("transferred successfully to ");
                TextColor(11);
                printf("%s %s\n", receiver.firstName, receiver.lastName);
                Sleep(1500);
                system("cls");
                system("color 0F");
                fclose(fp);
                fclose(temp);
                fclose(transaction);
                remove("customer.txt");
                rename("customerTemp.txt", "customer.txt");
            }
            else
            {
                system("cls");
                printf("Account limit of %s %s reached maximum", receiver.firstName, receiver.lastName);
                Sleep(500);
            }
        }
        else
        {
            system("cls");
            printf("You don't have enough balance");
            Sleep(500);
        }
    }
    else
    {
        system("cls");
        printf("Account does not exists");
        Sleep(500);
    }
    fclose(fp);
transferAmountEnd:
    system("cls");
}

void deleteAccount()
{
    char ch;
    int accountToDelete = customer.acountNo;
    system("cls");
    if (customer.amount >= 0)
    {
        printf("Are you sure you want to delete your account? [y,n]: ");
        fflush(stdin);
        ch = getche();
        if (ch == 'y' || ch == 'Y')
        {
            fp = fopen("customer.txt", "r");
            temp = fopen("temp.txt", "w");
            if (fp == NULL)
    		{
    		    system("cls");
        		perror("Error");
        		printf("\nProgram will exit\n");
        		Sleep(2000);
        		exit(1);
				}
 			if (temp == NULL)
 			{	
				system("cls");
 				perror("Error");
        		printf("\nProgram will exit\n");
        		Sleep(2000);
        		exit(1);
    			}
            if (fp == NULL || temp == NULL)
            {
                perror("Error");
                Sleep(1000);
            }
            fseek(fp, 0, SEEK_SET);
            while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
            {
                if (update.acountNo != accountToDelete)
                {
                    fprintf(temp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", update.acountNo, update.firstName, update.lastName, update.age, update.contactNumber, update.accountStatus, update.amount, update.password);
                }
            }
            fclose(fp);
            fclose(temp);
            remove("customer.txt");
            rename("temp.txt", "customer.txt");
            system("cls");
            printf("Account Deleted Successfully, Redirecting to Main Menu");
            Sleep(2000);
            loadingAnimation();
            fflush(stdin);
            main();
        }
        else if (ch == 'n' || ch == 'N')
        {
            system("cls");
            customerPortal();
        }
        else
        {
            printf("Wrong choice entered, please enter a valid choice");
            system("cls");
            Sleep(1000);
            deleteAccount();
        }
    }
    else
    {
        printf("You can not delete you account as you have negative balance\nPay the balance first to delete your account\n");
    }
}

int integerInputOnly()
{
    int x = 0, backSpaceCount = 0;
    char ch;
    int count = 0;

    for (;;)
    {
        ch = getch();
        fflush(stdin);
        if (ch >= '0' && ch <= '9')
        {
            printf("%c", ch);
            x = (x * 10) + (ch - 48);
            backSpaceCount++;
        }
        else if (ch == 13)
        {
            break;
        }
        else if (ch == 8 && backSpaceCount > 0)
        {
            printf("\b \b");
            backSpaceCount--;
        }
    }
    return x;
}

void viewCurrentAccInfo()
{
    fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    system("cls");
    system("color f0");
    TextColor(4);
    printf("Account Number  First Name      Last Name      Age  Status   Amount     \n\n");
    TextColor(0);
    while (fscanf(fp, "%6d  %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
    {
        printf("%-14d  %-15s %-15s %-3d %6s   %-9d\n", update.acountNo, update.firstName, update.lastName, update.age, update.accountStatus == 0 ? "Active" : "Blocked", update.amount);
    }
    printf("\n\nPress any key to continue ");
    getch();
    fclose(fp);
    system("color 0F");
}

void viewTransactionHistory()
{
    char transactionType[10];
    transaction = fopen("transaction_history.txt", "r");
    if (transaction == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    system("cls");
    TextColor(11);
    printf("Account Number      First Name       Last Name       Amount    Type\n\n");
    TextColor(15);
    while (fscanf(transaction, "%6d %15s %15s %9d %s\n", &update.acountNo, update.firstName, update.lastName, &update.amount, transactionType) != EOF)
    {
        printf("%-14d      %-15s  %-15s %-9d %-s\n", update.acountNo, update.firstName, update.lastName, update.amount, transactionType);
    }
    printf("\n\nPress any key to continue...");
    CursorPosition(0, 0);
    getch();
    fclose(transaction);
}

void adminDeleteAccount()
{
    int accountNoToDelete, accountFound = 0;
    system("cls");
    printf("Enter account number to delete: ");
    fflush(stdin);
    scanf("%d", &accountNoToDelete);

    fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
    {
        if (update.acountNo == accountNoToDelete)
        {
            accountFound = 1;
            break;
        }
    }
    fclose(fp);
    if (accountFound == 0)
    {
        system("cls");
        TextColor(4);
        printf("\aAccount does not exists");
        Sleep(1000);
        system("color 0F");
        goto doNothing;
    }
    if (update.amount >= 0)
    {
        printf("Are you sure you want to delete this account? [y,n]: ");
        fflush(stdin);
        ch = getche();
        if (ch == 'y' || ch == 'Y')
        {
            fp = fopen("customer.txt", "r");
            temp = fopen("temp.txt", "w");
            if (fp == NULL || temp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
            fseek(fp, 0, SEEK_SET);
            while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
            {
                if (update.acountNo != accountNoToDelete)
                {
                    fprintf(temp, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", update.acountNo, update.firstName, update.lastName, update.age, update.contactNumber, update.accountStatus, update.amount, update.password);
                }
            }
            fclose(fp);
            fclose(temp);
            remove("customer.txt");
            rename("temp.txt", "customer.txt");
            system("cls");
            TextColor(10);
            printf("Account Deleted Successfully\n");
            Sleep(2000);
            TextColor(15);
            adminPortal();
        }
        else if (ch == 'n' || ch == 'N')
        {
            system("cls");
            adminPortal();
        }
        else
        {
            printf("Wrong choice entered, please enter a valid choice");
            system("cls");
            Sleep(1000);
            adminDeleteAccount();
        }
    }
    else
    {
        printf("You can not delete you account as you have negative balance\nPay the balance first to delete your account\n");
    }
doNothing:
    printf("Loading");
    system("cls");
}

void viewCurrencyRates()
{
    fp = fopen("currency_rates.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    system("cls");
    system("color F0");
    TextColor(4);
    printf("No Currency              Symbol  Rate\n\n");
    TextColor(0);
    while (fscanf(fp, "%2d %10s %10s %3s %f\n", &currency.cNo, currency.first, currency.last, currency.code, &currency.rate) != EOF)
    {
        printf("%2d %-10s %-10s %-6s  %-.2f\n", currency.cNo, currency.first, currency.last, currency.code, currency.rate);
    }
    fclose(fp);
    printf("\nPRESS ANY KEY TO CONTINUE\n");
    getch();
    system("color 0F");
}

void aboutUs()
{
    system("cls");
    for (j = 21; j >= 0; j--)
    {
        if (j == 21)
        {
            system("color 01");
        }
        if (j == 19)
        {
            system("color 02");
        }
        if (j == 18)
        {
            system("color 03");
        }
        if (j == 17)
        {
            system("color 04");
        }
        if (j == 16)
        {
            system("color 05");
        }
        if (j == 15)
        {
            system("color 06");
        }
        if (j == 14)
        {
            system("color 07");
        }
        if (j == 13)
        {
            system("color 08");
        }
        if (j == 12)
        {
            system("color 09");
        }
        if (j == 11)
        {
            system("color 0A");
        }
        if (j == 10)
        {
            system("color 0B");
        }
        if (j == 9)
        {
            system("color 0C");
        }
        if (j == 8)
        {
            system("color 0D");
        }
        if (j == 7)
        {
            system("color 0E");
        }
        if (j == 6)
        {
            system("color 0F");
        }
        if (j == 5)
        {
            system("color 01");
        }
        if (j == 4)
        {
            system("color 02");
        }
        if (j == 3)
        {
            system("color 03");
        }
        if (j == 2)
        {
            system("color 0C");
        }
        if (j == 1)
        {
            system("color 0E");
        }
        if (j == 0)
        {
            system("color 0F");
        }
        system("cls");
        CursorPosition(46, j - 1);
        printf("THIS SYSTEM IS DESIGNED BY");
        CursorPosition(46, j + 1);
        printf("Sufiyaan Usmani  (21K-3195)");
        CursorPosition(46, j + 2);
        printf("Ahsan Ashraf     (21K-3186)");
        CursorPosition(46, j + 3);
        printf("Qasim Hasan      (21K-3210)");
        CursorPosition(46, j + 4);
        printf("Talha Shaikh     (21K-4546)");
        CursorPosition(0, 0);
        Sleep(600);
    }

    CursorPosition(0, 15);
    printf("Press any key to continue\n");
    getch();
}

void updateCurrencyRates()
{
    int cNoToUpdate, isFound, backSpaceCount = 0;
    float newRate;
    char ch;
    cNoToUpdate = 0;
    isFound = 0;
    viewCurrencyRates();

    system("cls");
    printf("Enter currency number to update: ");
    fflush(stdin);
    while (1)
    {
        ch = getch();
        if (ch >= '0' && ch <= '9')
        {
            printf("%c", ch);
            cNoToUpdate = (cNoToUpdate * 10) + (ch - 48);
            backSpaceCount++;
        }
        else if (ch == 13)
        {
            break;
        }
        else if (ch == 8 && backSpaceCount > 0)
        {
            printf("\b \b");
            backSpaceCount--;
        }
    }
    fp = fopen("currency_rates.txt", "a+");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    while (fscanf(fp, "%2d %10s %10s %3s %f\n", &currency.cNo, currency.first, currency.last, currency.code, &currency.rate) != EOF)
    {
        if (cNoToUpdate == currency.cNo)
        {
            isFound = 1;
            break;
        }
    }
    fclose(fp);
    system("cls");
    if (isFound == 1)
    {
        while (1)
        {
            printf("Enter new rates: ");
            fflush(stdin);
            scanf("%f", &newRate);
            if (newRate > 0)
            {
                break;
            }
            else
            {
                system("cls");
                printf("Error: Rates can not be negative");
                Sleep(1000);
                system("cls");
            }
        }
        fp = fopen("currency_rates.txt", "r");
        temp = fopen("currencyTemp.txt", "w");
        if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    if (temp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
        while (fscanf(fp, "%2d %10s %10s %3s %f\n", &currency.cNo, currency.first, currency.last, currency.code, &currency.rate) != EOF)
        {
            if (currency.cNo == cNoToUpdate)
            {
                fprintf(temp, "%-2d %-10s %-10s %-3s %-.2f\n", currency.cNo, currency.first, currency.last, currency.code, newRate);
            }
            else
            {
                fprintf(temp, "%-2d %-10s %-10s %-3s %-.2f\n", currency.cNo, currency.first, currency.last, currency.code, currency.rate);
            }
        }
        fclose(fp);
        fclose(temp);
        remove("currency_rates.txt");
        rename("currencyTemp.txt", "currency_rates.txt");
    }
    else
    {
        printf("Error: Currency not found\n\nRedirecting");
        Sleep(1500);
    }
}

void createCustomerDataBaseBackup()
{
    char fname[16];
    char date[50];
    char fileName[29] = "./backup/";

    time_t t; // not a primitive datatype
    time(&t);

    strcpy(date, ctime(&t));
    fname[0] = date[0];
    fname[1] = date[1];
    fname[2] = date[2];
    fname[3] = ' ';
    fname[4] = date[4];
    fname[5] = date[5];
    fname[6] = date[6];
    fname[7] = ' ';
    fname[8] = date[8];
    fname[9] = date[9];
    fname[10] = ' ';
    fname[11] = date[20];
    fname[12] = date[21];
    fname[13] = date[22];
    fname[14] = date[23];
    fname[15] = '\0';

    strcat(fileName, fname);
    strcat(fileName, ".txt");
    backup = fopen(fileName, "w");
    fp = fopen("customer.txt", "r");
    if (fp == NULL || backup == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
    {
        fprintf(backup, "%-6d %-15s %-15s %-2d %-11s %-d %-9d %-8s\n", update.acountNo, update.firstName, update.lastName, update.age, update.contactNumber, update.accountStatus, update.amount, update.password);
    }
    fclose(fp);
    fclose(backup);
}

void createCustomerDataBaseBackupAnimation()
{
    int i;
    system("cls");
    system("color 09");
    CursorPosition(50, 10);
    printf("Creating Backup");
    CursorPosition(40, 12);
    printf("[");
    CursorPosition(78, 12);
    printf("]");
    CursorPosition(41, 12);
    for (i = 1; i <= 37; i++)
    {
        printf("%c", 177);
    }
    CursorPosition(41, 12);
    for (i = 1; i <= 37; i++)
    {
        Sleep(35);
        printf("%c", 219);
    }
    CursorPosition(0, 0);
    system("cls");
    system("color 0A");
    printf("Backup Created Successfully");
    Sleep(2000);
    system("cls");
}

void viewMyTransactionHistory()
{
    int flag = 0;
    char transactionType[10];
    transaction = fopen("transaction_history.txt", "r");
    if (transaction == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    while (fscanf(transaction, "%6d %15s %15s %9d %s\n", &update.acountNo, update.firstName, update.lastName, &update.amount, transactionType) != EOF)
    {
        if (customer.acountNo == update.acountNo)
        {
            flag = 1;
            break;
        }
    }
    fclose(transaction);
    if (flag == 1)
    {
        printf("Amount        Type    \n\n");
        transaction = fopen("transaction_history.txt", "r");
        if (transaction == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
        while (fscanf(transaction, "%6d %15s %15s %9d %s\n", &update.acountNo, update.firstName, update.lastName, &update.amount, transactionType) != EOF)
        {
            if (customer.acountNo == update.acountNo)
            {
                printf("%-9d     %-s\n", update.amount, transactionType);
            }
        }
        fclose(transaction);
    }
    else
    {
        system("cls");
        printf("You currently have no transactions\n");
        Sleep(2000);
    }
    printf("\nPress any key to continue");
    CursorPosition(0, 0);
    getch();
}

void searchCustomer()
{
    int searchChoice;
    printf("1. Search by Account Number\n");
    printf("2. Search by Name\n");
    printf("3. Sort by amount (ASCENDING)\n");
    printf("4. Sort by amount (DESCENDING)\n");
    printf("5. Go back\n\n");
    TextColor(10);
    printf("Enter your choice: ");
    fflush(stdin);
    TextColor(15);
    scanf("%d", &searchChoice);

    switch (searchChoice)
    {
    case 1:
        searchByAccountNumber();
        break;
    case 2:
        searchByName();
        break;
    case 3:
        sortAsc();
        break;
    case 4:
        sortDes();
        break;
    case 5:
        goto searchCustomerEnd;
        break;
    default:
        printf("Error, wrong choice entered");
        Sleep(1000);
        break;
    }
searchCustomerEnd:
    system("cls");
}

void searchByAccountNumber()
{
    int accNo, flag;
    flag = 0;
    system("cls");
    while (1)
    {
        printf("Enter account number (-1 to go back) : ");
        fflush(stdin);
        scanf("%d", &accNo);
        if (accNo != -1)
        {
            if (accNo >= 100000 && accNo <= 999999)
            {
                fp = fopen("customer.txt", "r");
                if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
                while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
                {
                    if (accNo == update.acountNo)
                    {
                        flag = 1;
                        system("color 5F");
                        system("cls");
                        printf("Name            : ");
                        printf("%s %s\n", update.firstName, update.lastName);
                        printf("Account Number  : %d\n", update.acountNo);
                        printf("Age             : %d\n", update.age);
                        printf("Contact Number  : %s\n", update.contactNumber);
                        printf("Account Status  : %s\n", (update.accountStatus == 0 ? "Active" : "Blocked"));
                        printf("Account Balance : %d\n", update.amount);
                        printf("\n\nPress any key to go to your portal\n");
                        getch();
                        system("color 0F");
                        goto searchEnd;
                    }
                }
                if (flag == 0)
                {
                    system("cls");
                    printf("This account does not exists\n");
                    Sleep(2000);
                    system("cls");
                }
            }
            else
            {
                system("cls");
                printf("Error, account number must be of 6 digits");
                Sleep(2000);
                system("cls");
            }
        }
        else
        {
            goto searchEnd;
        }
    }
searchEnd:
    fclose(fp);

    system("cls");
}

void searchByName()
{
    int flag = 0, count = 0;
    char name[30], fullName[30];
    system("cls");
    printf("Enter full name: ");
    fflush(stdin);
    gets(name);
    strupr(name);
    system("cls");
    fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
    {
        strcpy(fullName, update.firstName);
        strcat(fullName, " ");
        strcat(fullName, update.lastName);
        strupr(fullName);
        if (strcmp(fullName, name) == 0)
        {
            flag = 1;
            system("color 5F");
            printf("Name            : ");
            printf("%s %s\n", update.firstName, update.lastName);
            printf("Account Number  : %d\n", update.acountNo);
            printf("Age             : %d\n", update.age);
            printf("Contact Number  : %s\n", update.contactNumber);
            printf("Account Status  : %s\n", (update.accountStatus == 0 ? "Active" : "Blocked"));
            printf("Account Balance : %d\n\n\n", update.amount);
            count++;
        }
    }
    if (flag == 1)
    {
        printf("There are currently %d account(s) with this name\n", count);
        printf("Press any key to continue");
        getch();
    }
    else
    {
        printf("This name does not exists");
        Sleep(2000);
    }
    fclose(fp);
    system("cls");
}

void bankPolicy()
{
    system("cls");
    system("title Bank Policy");
    system("color F0");
    TextColor(0);
    printf("By signing, you, the individual(s) named in this form are:\n\n");
    TextColor(1);
    printf("1. Confirming you are applying to the Bank for banking services;\n");
    printf("2. Confirming that any details you have supplied to us are true and complete;\n");
    printf("3. Authorizing us to:\n");
    printf("        i.  Make credit reference and other inquiries in connection with this application in accordance with our\n");
    printf("            normal procedures; and\n");
    printf("        ii. Disclose information to licensed credit reference agencies and otherwise in accordance with\n");
    printf("            conditions 94-105 of General Terms and Conditions.\n");
    printf("4. Confirming that you understand that credit reference agencies record information and the information they\n");
    printf("   record may be used by other organizations to help make decisions about credit and credit-related services and\n");
    printf("   to manage accounts for customers and for fraud prevention.\n");
    printf("5. Agreeing to be bound by the terms of the application and the Bank's General Terms and Conditions which contain\n");
    printf("   your obligations to us and our obligations to you which may be altered from time to time;\n");
    printf("6. Understanding that the Bank may decline your account without stating a reason.\n");
    printf("7. Contact the admin directly in case of any issues\n");
    printf("\nPress any key to continue...");
    getch();
    system("cls");
    system("color F4");
    TextColor(4);
    printf("Before proceeding further:\n");
    TextColor(0);
    printf("1.	Make sure to record your account number\n");
    printf("2.	In case you forgot your account number, contact the admin, and provide your full information for authentication\n");
    printf("3.	Make sure to log out of your account after use.\n");
    printf("4.	Make sure to read the user guide before using Banking System\n\n");

    TextColor(4);
    printf("The Bank will not be responsible if:\n");
    TextColor(0);
    printf("1.	User forgets to log out of his/her account\n");
    printf("2.	User edits, modifies or deletes the database files\n\n");
    TextColor(2);
    printf("Press any key to continue...");
    getch();
    system("cls");
    system("color 0F");
}

void sortAsc()
{
    int size = 0, i = 0, round;
    fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
    {
        size++;
    }
    fclose(fp);
    system("color F1");
    struct CustomerInfo sort[size], temp1;

    fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    i = 0;
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &temp1.acountNo, temp1.firstName, temp1.lastName, &temp1.age, temp1.contactNumber, &temp1.accountStatus, &temp1.amount, temp1.password) != EOF)
    {
        sort[i] = temp1;
        i++;
    }

    for (round = 1; round < size; round++)
    {
        for (i = 0; i < size - round; i++)
        {
            if (sort[i].amount > sort[i + 1].amount)
            {
                temp1 = sort[i];
                sort[i] = sort[i + 1];
                sort[i + 1] = temp1;
            }
        }
    }
    fclose(fp);
    system("cls");
    TextColor(1);
    printf("Account Number  First Name      Last Name      Age  Status   Amount     \n\n");
    TextColor(0);
    i = 0;
    for (i = 0; i < size; i++)
    {
        printf("%-14d  %-15s %-15s %-3d %6s   %-9d\n", sort[i].acountNo, sort[i].firstName, sort[i].lastName, sort[i].age, sort[i].accountStatus == 0 ? "Active" : "Blocked", sort[i].amount);
    }
    printf("\nPress any key to continue...");
    getch();
    system("cls");
}

void sortDes()
{
    int size = 0, i = 0, round;
    fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &update.acountNo, update.firstName, update.lastName, &update.age, update.contactNumber, &update.accountStatus, &update.amount, update.password) != EOF)
    {
        size++;
    }
    fclose(fp);
    system("color F1");
    struct CustomerInfo sort[size], temp1;

    fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        system("cls");
        perror("Error");
        printf("\nProgram will exit\n");
        Sleep(2000);
        exit(1);
    }
    i = 0;
    while (fscanf(fp, "%6d %15s %15s %2d %11s %d %9d %8s\n", &temp1.acountNo, temp1.firstName, temp1.lastName, &temp1.age, temp1.contactNumber, &temp1.accountStatus, &temp1.amount, temp1.password) != EOF)
    {
        sort[i] = temp1;
        i++;
    }

    for (round = 1; round < size; round++)
    {
        for (i = 0; i < size - round; i++)
        {
            if (sort[i].amount < sort[i + 1].amount)
            {
                temp1 = sort[i];
                sort[i] = sort[i + 1];
                sort[i + 1] = temp1;
            }
        }
    }
    fclose(fp);
    system("cls");
    TextColor(1);
    printf("Account Number  First Name      Last Name      Age  Status   Amount     \n\n");
    TextColor(0);
    i = 0;
    for (i = 0; i < size; i++)
    {
        printf("%-14d  %-15s %-15s %-3d %6s   %-9d\n", sort[i].acountNo, sort[i].firstName, sort[i].lastName, sort[i].age, sort[i].accountStatus == 0 ? "Active" : "Blocked", sort[i].amount);
    }
    printf("\nPress any key to continue...");
    getch();
    system("cls");
}
