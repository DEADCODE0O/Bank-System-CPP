#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <windows.h>

using namespace std;

// تفعيل الألوان في Windows Console
void EnableVirtualTerminalProcessing()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

// رموز الألوان  
#define RESET "\033[0m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_RED "\033[1;31m"
#define BOLD_WHITE "\033[1;37m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_BLUE "\033[1;34m"

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

struct stUsers
{
    string Username;
    string Password;
    int Permissions = 0;
    bool MarkForDelete = false;
};

enum enPermission {
    All = 127,
    ShowList = 1,
    Add = 2,
    Delete = 4,
    Update = 8,
    Find = 16,
    Transactions = 32,
    ManageUsers = 64
};

bool HasPermission(stUsers User, enPermission Permission);
void ShowMainMenue(stUsers& User);
void ShowTransactionsMenue(stUsers& User);
void ShowManageUserMenue(stUsers& User);
void Login();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

enum enTransactionsMenueOptions {
    eDeposit = 1,
    eWithdraw = 2,
    eShowTotalBalance = 3,
    eShowMainMenue = 4
};

enum enMainMenueOptions {
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eShowTransactionsMenue = 6,
    eManageUser = 7,
    LogOut = 8
};

enum enManageUserMenueOptions {
    eListUsers = 1,
    eAddNewUser = 2,
    eDeleteUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eShowMain = 6
};

// ══════════════════════ UI Helper Functions ══════════════════════

void PrintBankLogo()
{
    cout << BOLD_CYAN;
    cout << "\n          ╔══════════════════════════════════════════════╗\n";
    cout << "          ║  ██████╗  █████╗ ███╗   ██╗██╗  ██╗       ║\n";
    cout << "          ║  ██╔══██╗██╔══██╗████╗  ██║██║ ██╔╝       ║\n";
    cout << "          ║  ██████╔╝███████║██╔██╗ ██║█████╔╝        ║\n";
    cout << "          ║  ██╔══██╗██╔══██║██║╚██╗██║██╔═██╗        ║\n";
    cout << "          ║  ██████╔╝██║  ██║██║ ╚████║██║  ██╗       ║\n";
    cout << "          ║  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝       ║\n";
    cout << "          ╚══════════════════════════════════════════════╝\n";
    cout << BOLD_YELLOW << "          ║  MANAGEMENT SYSTEM - Version 2.0      ║\n" << RESET;
}

void PrintMenuHeader(string title)
{
    cout << BOLD_CYAN;
    cout << "\n╔═══════════════════════════════════════════════════════════════════╗\n";
    cout << "║ " << BOLD_YELLOW << title;
    for (int i = 0; i < 66 - (int)title.length(); i++) cout << " ";
    cout << BOLD_CYAN << "║\n";
    cout << "╠═══════════════════════════════════════════════════════════════════╣\n";
    cout << RESET;
}

void PrintMenuFooter()
{
    cout << BOLD_CYAN << "╚═══════════════════════════════════════════════════════════════════╝\n" << RESET;
}

void PrintMenuItem(int num, string text, string icon = "●")
{
    string numStr = "[" + to_string(num) + "]";
    cout << BOLD_CYAN << "║  " << BOLD_YELLOW << left << setw(4) << numStr << BOLD_WHITE
        << icon << " " << setw(57) << text << BOLD_CYAN << " ║\n" << RESET;
}

void PrintSeparator()
{
    cout << BOLD_CYAN << "╠═══════════════════════════════════════════════════════════════════╣\n" << RESET;
}

void PrintSuccess(string msg)
{
    cout << BOLD_GREEN << "\n  ✓ " << msg << RESET << "\n";
}

void PrintError(string msg)
{
    cout << BOLD_RED << "\n  ✗ " << msg << RESET << "\n";
}

void PrintInfo(string msg)
{
    cout << BOLD_CYAN << "\n  ℹ " << msg << RESET << "\n";
}

// ══════════════════════ Core Functions ══════════════════════

enPermission ConvertMainMenuToPermission(enMainMenueOptions Option)
{
    switch (Option)
    {
    case eListClients: return enPermission::ShowList;
    case eAddNewClient: return enPermission::Add;
    case eDeleteClient: return enPermission::Delete;
    case eUpdateClient: return enPermission::Update;
    case eFindClient: return enPermission::Find;
    case eShowTransactionsMenue: return enPermission::Transactions;
    case eManageUser: return enPermission::ManageUsers;
    default: return enPermission::ShowList;
    }
}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }
        MyFile.close();
    }

    return false;
}

sClient ReadNewClient()
{
    sClient Client;

    cout << BOLD_CYAN << "\n  Account Number: " << BOLD_WHITE;
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        PrintError("Account Number [" + Client.AccountNumber + "] already exists!");
        cout << BOLD_CYAN << "  Enter another Account Number: " << BOLD_WHITE;
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << BOLD_CYAN << "  Pin Code: " << BOLD_WHITE;
    getline(cin, Client.PinCode);

    cout << BOLD_CYAN << "  Name: " << BOLD_WHITE;
    getline(cin, Client.Name);

    cout << BOLD_CYAN << "  Phone: " << BOLD_WHITE;
    getline(cin, Client.Phone);

    cout << BOLD_CYAN << "  Account Balance: " << BOLD_WHITE;
    cin >> Client.AccountBalance;

    return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }

    return vClients;
}

void ShowAllClientsScreen()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    PrintMenuHeader("📋 CLIENT LIST (" + to_string(vClients.size()) + " Clients)");

    if (vClients.size() == 0)
    {
        cout << BOLD_YELLOW << "║" << setw(68) << "No Clients Available In The System!" << setw(3) << "║\n" << RESET;
    }
    else
    {
        // Table Header
        cout << BOLD_CYAN << "║ " << BOLD_YELLOW
            << left << setw(12) << "Account" << "│ "
            << setw(8) << "PIN" << "│ "
            << setw(25) << "Name" << "│ "
            << setw(12) << "Phone" << "│ "
            << setw(10) << "Balance"
            << BOLD_CYAN << " ║\n";
        PrintSeparator();

        // Table Rows
        for (sClient Client : vClients)
        {
            cout << BOLD_CYAN << "║ " << BOLD_WHITE
                << left << setw(12) << Client.AccountNumber << "│ "
                << setw(8) << Client.PinCode << "│ "
                << setw(25) << Client.Name << "│ "
                << setw(12) << Client.Phone << "│ "
                << BOLD_GREEN << setw(10) << fixed << setprecision(2) << Client.AccountBalance
                << BOLD_CYAN << " ║\n" << RESET;
        }
    }

    PrintMenuFooter();
}

void ShowTotalBalances()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    double TotalBalances = 0;

    PrintMenuHeader("💰 BALANCE LIST (" + to_string(vClients.size()) + " Clients)");

    if (vClients.size() == 0)
    {
        cout << BOLD_YELLOW << "║" << setw(68) << "No Clients Available In The System!" << setw(3) << "║\n" << RESET;
    }
    else
    {
        // Table Header
        cout << BOLD_CYAN << "║ " << BOLD_YELLOW
            << left << setw(15) << "Account" << "│ "
            << setw(30) << "Name" << "│ "
            << setw(15) << "Balance"
            << BOLD_CYAN << " ║\n";
        PrintSeparator();

        // Table Rows
        for (sClient Client : vClients)
        {
            cout << BOLD_CYAN << "║ " << BOLD_WHITE
                << left << setw(15) << Client.AccountNumber << "│ "
                << setw(30) << Client.Name << "│ "
                << BOLD_GREEN << setw(15) << fixed << setprecision(2) << Client.AccountBalance
                << BOLD_CYAN << " ║\n" << RESET;
            TotalBalances += Client.AccountBalance;
        }

        PrintSeparator();
        cout << BOLD_CYAN << "║ " << BOLD_YELLOW << "TOTAL BALANCE:" << setw(40) << " "
            << BOLD_GREEN << setw(15) << fixed << setprecision(2) << TotalBalances
            << BOLD_CYAN << " ║\n";
    }

    PrintMenuFooter();
}

void PrintClientCard(sClient Client)
{
    PrintMenuHeader("📄 CLIENT DETAILS");

    cout << BOLD_CYAN << "║  " << BOLD_YELLOW << "Account Number : " << BOLD_WHITE << setw(45) << left << Client.AccountNumber << BOLD_CYAN << " ║\n";
    cout << BOLD_CYAN << "║  " << BOLD_YELLOW << "Pin Code       : " << BOLD_WHITE << setw(45) << left << Client.PinCode << BOLD_CYAN << " ║\n";
    cout << BOLD_CYAN << "║  " << BOLD_YELLOW << "Name           : " << BOLD_WHITE << setw(45) << left << Client.Name << BOLD_CYAN << " ║\n";
    cout << BOLD_CYAN << "║  " << BOLD_YELLOW << "Phone          : " << BOLD_WHITE << setw(45) << left << Client.Phone << BOLD_CYAN << " ║\n";
    cout << BOLD_CYAN << "║  " << BOLD_YELLOW << "Balance        : " << BOLD_GREEN << setw(45) << left << fixed << setprecision(2) << Client.AccountBalance << BOLD_CYAN << " ║\n";

    PrintMenuFooter();
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;
    Client.AccountNumber = AccountNumber;

    cout << BOLD_CYAN << "\n  Pin Code: " << BOLD_WHITE;
    getline(cin >> ws, Client.PinCode);

    cout << BOLD_CYAN << "  Name: " << BOLD_WHITE;
    getline(cin, Client.Name);

    cout << BOLD_CYAN << "  Phone: " << BOLD_WHITE;
    getline(cin, Client.Phone);

    cout << BOLD_CYAN << "  Account Balance: " << BOLD_WHITE;
    cin >> Client.AccountBalance;

    return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vClients;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        PrintMenuHeader("➕ ADD NEW CLIENT");
        AddNewClient();
        PrintSuccess("Client Added Successfully!");

        cout << BOLD_CYAN << "\n  Add more clients? (Y/N): " << BOLD_WHITE;
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << BOLD_RED << "\n  Are you sure you want to delete this client? (Y/N): " << BOLD_WHITE;
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            PrintSuccess("Client Deleted Successfully!");
            return true;
        }
    }
    else
    {
        PrintError("Client with Account Number (" + AccountNumber + ") Not Found!");
        return false;
    }
    return false;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << BOLD_YELLOW << "\n  Are you sure you want to update this client? (Y/N): " << BOLD_WHITE;
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);
            PrintSuccess("Client Updated Successfully!");
            return true;
        }
    }
    else
    {
        PrintError("Client with Account Number (" + AccountNumber + ") Not Found!");
        return false;
    }
    return false;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';

    cout << BOLD_YELLOW << "\n  Confirm this transaction? (Y/N): " << BOLD_WHITE;
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                PrintSuccess("Done Successfully! New Balance: " + to_string(C.AccountBalance));
                return true;
            }
        }
        return false;
    }
    return false;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << BOLD_CYAN << "\n  Account Number: " << BOLD_WHITE;
    cin >> AccountNumber;
    return AccountNumber;
}

void ShowDeleteClientScreen()
{
    PrintMenuHeader("🗑️  DELETE CLIENT");

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
    PrintMenuHeader("✏️  UPDATE CLIENT");

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowAddNewClientsScreen()
{
    AddNewClients();
}

void ShowFindClientScreen()
{
    PrintMenuHeader("🔍 FIND CLIENT");

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        PrintError("Client with Account Number [" + AccountNumber + "] Not Found!");
}

void ShowDepositScreen()
{
    PrintMenuHeader("💵 DEPOSIT");

    sClient Client;
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintError("Client [" + AccountNumber + "] does not exist!");
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << BOLD_CYAN << "\n  Deposit Amount: " << BOLD_WHITE;
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithDrawScreen()
{
    PrintMenuHeader("💸 WITHDRAW");

    sClient Client;
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintError("Client [" + AccountNumber + "] does not exist!");
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << BOLD_CYAN << "\n  Withdraw Amount: " << BOLD_WHITE;
    cin >> Amount;

    while (Amount > Client.AccountBalance)
    {
        PrintError("Amount Exceeds Balance! Maximum: " + to_string(Client.AccountBalance));
        cout << BOLD_CYAN << "  Enter another amount: " << BOLD_WHITE;
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void ShowTotalBalancesScreen()
{
    ShowTotalBalances();
}

void GoBackToMainMenue(stUsers& User)
{
    cout << BOLD_CYAN << "\n  Press any key to return to Main Menu..." << RESET;
    system("pause>0");
    ShowMainMenue(User);
}

void GoBackToTransactionsMenue(stUsers& User)
{
    cout << BOLD_CYAN << "\n  Press any key to return to Transactions Menu..." << RESET;
    system("pause>0");
    ShowTransactionsMenue(User);
}

short ReadTransactionsMenueOption()
{
    cout << BOLD_CYAN << "\n  Your choice [1-4]: " << BOLD_WHITE;
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption, stUsers& User)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue(User);
        break;

    case enTransactionsMenueOptions::eWithdraw:
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue(User);
        break;

    case enTransactionsMenueOptions::eShowTotalBalance:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue(User);
        break;

    case enTransactionsMenueOptions::eShowMainMenue:
        ShowMainMenue(User);
        break;
    }
}

void ShowTransactionsMenue(stUsers& User)
{
    system("cls");
    PrintMenuHeader("💳 TRANSACTIONS MENU");

    PrintMenuItem(1, "Deposit", "💵");
    PrintMenuItem(2, "Withdraw", "💸");
    PrintMenuItem(3, "Total Balances", "💰");
    PrintSeparator();
    PrintMenuItem(4, "Main Menu", "🏠");

    PrintMenuFooter();

    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption(), User);
}

short ReadMainMenueOption()
{
    cout << BOLD_CYAN << "\n  Your choice [1-8]: " << BOLD_WHITE;
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption, stUsers& User)
{
    if (HasPermission(User, ConvertMainMenuToPermission(MainMenueOption)) || MainMenueOption == LogOut)
    {
        switch (MainMenueOption)
        {
        case enMainMenueOptions::eListClients:
            system("cls");
            ShowAllClientsScreen();
            GoBackToMainMenue(User);
            break;

        case enMainMenueOptions::eAddNewClient:
            system("cls");
            ShowAddNewClientsScreen();
            GoBackToMainMenue(User);
            break;

        case enMainMenueOptions::eDeleteClient:
            system("cls");
            ShowDeleteClientScreen();
            GoBackToMainMenue(User);
            break;

        case enMainMenueOptions::eUpdateClient:
            system("cls");
            ShowUpdateClientScreen();
            GoBackToMainMenue(User);
            break;

        case enMainMenueOptions::eFindClient:
            system("cls");
            ShowFindClientScreen();
            GoBackToMainMenue(User);
            break;

        case enMainMenueOptions::eShowTransactionsMenue:
            system("cls");
            ShowTransactionsMenue(User);
            break;

        case enMainMenueOptions::eManageUser:
            system("cls");
            ShowManageUserMenue(User);
            break;

        case enMainMenueOptions::LogOut:
            system("cls");
            PrintInfo("Logging out...");
            Login();
            break;
        }
    }
    else
    {
        system("cls");
        PrintMenuHeader("🚫 ACCESS DENIED");
        PrintError("You don't have permission for this action!");
        PrintMenuFooter();
        GoBackToMainMenue(User);
    }
}

void ShowMainMenue(stUsers& User)
{
    system("cls");
    PrintBankLogo();

    PrintMenuHeader("🏠 MAIN MENU - Welcome " + User.Username);

    PrintMenuItem(1, "Show Client List", "📋");
    PrintMenuItem(2, "Add New Client", "➕");
    PrintMenuItem(3, "Delete Client", "🗑️");
    PrintMenuItem(4, "Update Client Info", "✏️");
    PrintMenuItem(5, "Find Client", "🔍");
    PrintMenuItem(6, "Transactions", "💳");
    PrintMenuItem(7, "Manage Users", "👥");
    PrintSeparator();
    PrintMenuItem(8, "Logout", "🚪");

    PrintMenuFooter();

    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption(), User);
}

// ══════════════════════ USER MANAGEMENT ══════════════════════

stUsers ConvertLinetoRecordUser(string Line, string Seperator = "#//#")
{
    stUsers User;
    vector<string> vUserData;

    vUserData = SplitString(Line, Seperator);

    User.Username = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}

bool HasPermission(stUsers User, enPermission Permission)
{
    if (User.Permissions == enPermission::All)
        return true;
    return (User.Permissions & Permission) != 0;
}

bool AskPermission(string Message)
{
    char Answer;
    cout << BOLD_CYAN << "  " << Message << " (Y/N): " << BOLD_WHITE;
    cin >> Answer;
    return (tolower(Answer) == 'y');
}

int ReadPermissions()
{
    int Permissions = 0;

    PrintInfo("Set Permissions:");

    if (AskPermission("Give full access?"))
        return enPermission::All;

    if (AskPermission("Allow Show List?"))
        Permissions |= enPermission::ShowList;

    if (AskPermission("Allow Add?"))
        Permissions |= enPermission::Add;

    if (AskPermission("Allow Delete?"))
        Permissions |= enPermission::Delete;

    if (AskPermission("Allow Update?"))
        Permissions |= enPermission::Update;

    if (AskPermission("Allow Find?"))
        Permissions |= enPermission::Find;

    if (AskPermission("Allow Transactions?"))
        Permissions |= enPermission::Transactions;

    if (AskPermission("Allow Manage Users?"))
        Permissions |= enPermission::ManageUsers;

    return Permissions;
}

string ConvertRecordToLineUser(stUsers User, string Seperator = "#//#")
{
    string stUserRecord = "";
    stUserRecord += User.Username + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions);
    return stUserRecord;
}

vector <stUsers> LoadUsersDataFromFile(string FileName)
{
    vector <stUsers> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile, Line))
        {
            User = ConvertLinetoRecordUser(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }

    return vUsers;
}

bool FindUserByUsername(string Username, vector <stUsers> vUsers, stUsers& User)
{
    for (stUsers C : vUsers)
    {
        if (C.Username == Username)
        {
            User = C;
            return true;
        }
    }
    return false;
}

string ReadUsername()
{
    string Username = "";
    cout << BOLD_CYAN << "  Username: " << BOLD_WHITE;
    cin >> Username;
    return Username;
}

string ReadPassword()
{
    string Password = "";
    cout << BOLD_CYAN << "  Password: " << BOLD_WHITE;
    cin >> Password;
    return Password;
}

void PrintUserCard(stUsers User)
{
    PrintMenuHeader("👤 USER DETAILS");

    cout << BOLD_CYAN << "║  " << BOLD_YELLOW << "Username    : " << BOLD_WHITE << setw(48) << left << User.Username << BOLD_CYAN << " ║\n";
    cout << BOLD_CYAN << "║  " << BOLD_YELLOW << "Password    : " << BOLD_WHITE << setw(48) << left << User.Password << BOLD_CYAN << " ║\n";
    cout << BOLD_CYAN << "║  " << BOLD_YELLOW << "Permissions : " << BOLD_WHITE << setw(48) << left << User.Permissions << BOLD_CYAN << " ║\n";

    PrintMenuFooter();
}

bool UserExistsByUsarname(string Username, string FileName)
{
    vector <stUsers> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile, Line))
        {
            User = ConvertLinetoRecordUser(Line);
            if (User.Username == Username)
            {
                MyFile.close();
                return true;
            }
            vUsers.push_back(User);
        }
        MyFile.close();
    }

    return false;
}

vector <stUsers> SaveUsersDataToFile(string FileName, vector <stUsers> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;

    if (MyFile.is_open())
    {
        for (stUsers C : vUsers)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLineUser(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vUsers;
}

stUsers ReadNewUser()
{
    stUsers User;

    cout << BOLD_CYAN << "\n  Username: " << BOLD_WHITE;
    getline(cin >> ws, User.Username);

    while (UserExistsByUsarname(User.Username, UsersFileName))
    {
        PrintError("User [" + User.Username + "] already exists!");
        cout << BOLD_CYAN << "  Enter another Username: " << BOLD_WHITE;
        getline(cin >> ws, User.Username);
    }

    cout << BOLD_CYAN << "  Password: " << BOLD_WHITE;
    getline(cin, User.Password);

    User.Permissions = ReadPermissions();

    return User;
}

void AddNewUser()
{
    stUsers User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLineUser(User));
}

void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        PrintMenuHeader("➕ ADD NEW USER");
        AddNewUser();
        PrintSuccess("User Added Successfully!");

        cout << BOLD_CYAN << "\n  Add more users? (Y/N): " << BOLD_WHITE;
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

bool MarkUserForDeleteByUsername(string Username, vector <stUsers>& vUsers)
{
    for (stUsers& C : vUsers)
    {
        if (C.Username == Username)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteUserByUsername(string Username, vector <stUsers>& vUsers)
{
    stUsers User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);

        cout << BOLD_RED << "\n  Are you sure you want to delete this user? (Y/N): " << BOLD_WHITE;
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUsername(Username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            vUsers = LoadUsersDataFromFile(UsersFileName);

            PrintSuccess("User Deleted Successfully!");
            return true;
        }
    }
    else
    {
        PrintError("User with Username (" + Username + ") Not Found!");
        return false;
    }
    return false;
}

stUsers ChangeUserRecord(string Username)
{
    stUsers User;
    User.Username = Username;

    cout << BOLD_CYAN << "\n  Password: " << BOLD_WHITE;
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissions();

    return User;
}

bool UpdateUserByUsername(string Username, vector <stUsers>& vUsers)
{
    stUsers User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);

        cout << BOLD_YELLOW << "\n  Are you sure you want to update this user? (Y/N): " << BOLD_WHITE;
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (stUsers& C : vUsers)
            {
                if (C.Username == Username)
                {
                    C = ChangeUserRecord(Username);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);
            PrintSuccess("User Updated Successfully!");
            return true;
        }
    }
    else
    {
        PrintError("User with Username (" + Username + ") Not Found!");
        return false;
    }
    return false;
}

short ReadManageUserMenueOption()
{
    cout << BOLD_CYAN << "\n  Your choice [1-6]: " << BOLD_WHITE;
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void ShowListUsers()
{
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

    PrintMenuHeader("👥 USERS LIST (" + to_string(vUsers.size()) + " Users)");

    if (vUsers.size() == 0)
    {
        cout << BOLD_YELLOW << "║" << setw(68) << "No Users Available In The System!" << setw(3) << "║\n" << RESET;
    }
    else
    {
        // Table Header
        cout << BOLD_CYAN << "║ " << BOLD_YELLOW
            << left << setw(20) << "Username" << "│ "
            << setw(20) << "Password" << "│ "
            << setw(15) << "Permissions"
            << BOLD_CYAN << " ║\n";
        PrintSeparator();

        // Table Rows
        for (stUsers User : vUsers)
        {
            cout << BOLD_CYAN << "║ " << BOLD_WHITE
                << left << setw(20) << User.Username << "│ "
                << setw(20) << User.Password << "│ "
                << setw(15) << User.Permissions
                << BOLD_CYAN << " ║\n" << RESET;
        }
    }

    PrintMenuFooter();
}

void ShowAddNewUsers()
{
    AddNewUsers();
}

void ShowDeleteUserScreen()
{
    PrintMenuHeader("🗑️  DELETE USER");

    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    string Username = ReadUsername();
    DeleteUserByUsername(Username, vUsers);
}

void ShowUpdateUserScreen()
{
    PrintMenuHeader("✏️  UPDATE USER");

    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    string Username = ReadUsername();
    UpdateUserByUsername(Username, vUsers);
}

void ShowFindUsertScreen()
{
    PrintMenuHeader("🔍 FIND USER");

    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    stUsers User;
    string Username = ReadUsername();

    if (FindUserByUsername(Username, vUsers, User))
        PrintUserCard(User);
    else
        PrintError("User with Username [" + Username + "] Not Found!");
}

void GoBackToManageUserMenue(stUsers& User)
{
    cout << BOLD_CYAN << "\n  Press any key to return to Manage Users Menu..." << RESET;
    system("pause>0");
    ShowManageUserMenue(User);
}

void PerfromManageUserMenueOption(enManageUserMenueOptions ManageUserMenueOption, stUsers& User)
{
    switch (ManageUserMenueOption)
    {
    case enManageUserMenueOptions::eListUsers:
        system("cls");
        ShowListUsers();
        GoBackToManageUserMenue(User);
        break;

    case enManageUserMenueOptions::eAddNewUser:
        system("cls");
        ShowAddNewUsers();
        GoBackToManageUserMenue(User);
        break;

    case enManageUserMenueOptions::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUserMenue(User);
        break;

    case enManageUserMenueOptions::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUserMenue(User);
        break;

    case enManageUserMenueOptions::eFindUser:
        system("cls");
        ShowFindUsertScreen();
        GoBackToManageUserMenue(User);
        break;

    case enManageUserMenueOptions::eShowMain:
        system("cls");
        ShowMainMenue(User);
        break;
    }
}

void ShowManageUserMenue(stUsers& User)
{
    system("cls");
    PrintMenuHeader("👥 MANAGE USERS MENU");

    PrintMenuItem(1, "Show User List", "📋");
    PrintMenuItem(2, "Add New User", "➕");
    PrintMenuItem(3, "Delete User", "🗑️");
    PrintMenuItem(4, "Update User Info", "✏️");
    PrintMenuItem(5, "Find User", "🔍");
    PrintSeparator();
    PrintMenuItem(6, "Main Menu", "🏠");

    PrintMenuFooter();

    PerfromManageUserMenueOption((enManageUserMenueOptions)ReadManageUserMenueOption(), User);
}

// ══════════════════════ LOGIN ══════════════════════

bool CheckUsername(string Username, stUsers& User)
{
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    return FindUserByUsername(Username, vUsers, User);
}

bool CheckPassword(string Password, stUsers User)
{
    return (Password == User.Password);
}

bool CheckLogin(stUsers& User)
{
    string Username = ReadUsername();
    string Password = ReadPassword();

    if (!CheckUsername(Username, User))
    {
        PrintError("Username not found!");
        return false;
    }

    int attempts = 3;
    while (!CheckPassword(Password, User) && attempts > 0)
    {
        attempts--;
        PrintError("Invalid Password! Attempts remaining: " + to_string(attempts));

        if (attempts > 0)
        {
            Password = ReadPassword();
        }
    }

    if (CheckPassword(Password, User))
    {
        PrintSuccess("Login Successful! Welcome " + User.Username);
        system("pause>0");
        return true;
    }
    else
    {
        PrintError("Too many failed attempts! Access denied.");
        return false;
    }
}

void Login()
{
    system("cls");
    PrintBankLogo();

    PrintMenuHeader("🔐 LOGIN TO BANK SYSTEM");
    PrintMenuFooter();

    stUsers User;
    if (CheckLogin(User))
    {
        ShowMainMenue(User);
    }
}

// ══════════════════════ MAIN ══════════════════════

int main()
{
    EnableVirtualTerminalProcessing(); // تفعيل الألوان
    Login();
    return 0;
}