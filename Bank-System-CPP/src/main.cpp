#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
const string filename = "Clients.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

void ShowTransactionsMenu();
void ShowMainMenu();

enum enMainMenuOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6,
    eExit = 7
};

enum enTransactionsMenuOptions {
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalances = 3,
    eMainMenu = 4
};

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

vector<string> SplitString(string S1, string Delim) {
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != Delim) {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (!S1.empty()) {
        vString.push_back(S1);
    }

    return vString;
}

string JoinString(vector<string> TS, string Delime) {
    string S1 = "";

    for (string& X : TS) {
        S1 = S1 + X + Delime;
    }

    S1 = S1.substr(0, S1.length() - 1);

    return S1;
}

sClient ConvertLineToRecord(string Line, string Delim = "#//#") {
    sClient Client;
    vector<string> vClientData = SplitString(Line, Delim);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}

string ConvertRecordToLine(sClient Client, string Delim = "#//#") {
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Delim;
    stClientRecord += Client.PinCode + Delim;
    stClientRecord += Client.Name + Delim;
    stClientRecord += Client.Phone + Delim;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

vector<sClient> LoadCleintsDataFromFile(string filename) {
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(filename, ios::in);
    if (MyFile.is_open()) {
        string line;
        while (getline(MyFile, line)) {
            sClient Client = ConvertLineToRecord(line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

vector <sClient> SaveClientsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }

    return vClients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(35) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsScreen() {
    vector<sClient> vClients = LoadCleintsDataFromFile(filename);
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << setw(15) << left << "AccountNumber";
    cout << "| " << setw(10) << left << "PinCode";
    cout << "| " << setw(35) << left << "Name";
    cout << "| " << setw(12) << left << "Phone";
    cout << "| " << setw(12) << left << "AccountBalance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
    {
        cout << "No Clients Available In The System" << endl;
    }
    else
    {
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

bool ClientExistsByAccountNumber(string filename, string AccountNumber) {

    fstream MyFile;

    MyFile.open(filename, ios::in);
    if (MyFile.is_open()) {
        string line;
        sClient Client;
        while (getline(MyFile, line)) {
            Client = ConvertLineToRecord(line);
            if (Client.AccountNumber == AccountNumber) {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

sClient ReadNewClientData() {

    sClient Client;

    cout << "Enter Account Number: ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber("Clients.txt", Client.AccountNumber)) {
        cout << "Account Number already exists. Please enter a different Account Number: ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter Pin Code: ";
    getline(cin, Client.PinCode);

    cout << "Enter Name: ";
    getline(cin, Client.Name);

    cout << "Enter Phone: ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

void AddDataLineToFile(string FileName, string stDataLine) {

    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open()) {

        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void AddClient() {

    sClient Client;
    Client = ReadNewClientData();
    AddDataLineToFile(filename, ConvertRecordToLine(Client));
}

void AddClients() {

    char AddMore = 'N';
    do {
        AddClient();
        cout << "\n\nClient Added Successfully!\n" << endl;
        cout << "Do you want to add another client? (Y/N): ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientScreen() {

    cout << "\n_______________________________________________________";
    cout << "\n\t\tAdd New Client Screen";
    cout << "\n_______________________________________________________\n";

    AddClients();
}

void PrintClientCard(sClient Client) {
    cout << "\n__________________________Client Card__________________________\n" << endl;
    cout << "Account Number  : " << Client.AccountNumber << endl;
    cout << "Pin Code        : " << Client.PinCode << endl;
    cout << "Name            : " << Client.Name << endl;
    cout << "Phone           : " << Client.Phone << endl;
    cout << "Account Balance : " << Client.AccountBalance << endl;
    cout << "\n______________________________________________________________\n" << endl;
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

bool MarkClientForDeleteByAccountNumber(vector<sClient>& Client, string AccountNumber) {

    for (sClient& C : Client) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

void DeleteMarkedClients(string AccountNumber, vector<sClient>& vClients) {

    sClient Client;
    char Answer;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        PrintClientCard(Client);

        cout << "Are you sure you want to delete this client? (Y/N): ";
        cin >> Answer;

        if (toupper(Answer) == 'Y') {
            MarkClientForDeleteByAccountNumber(vClients, AccountNumber);
            SaveClientsDataToFile(filename, vClients);
            cout << "\nClient deleted successfully.\n" << endl;
            vClients = LoadCleintsDataFromFile(filename);
        }
        else {
            cout << "\nClient deletion canceled.\n" << endl;
        }
    }
    else {
        cout << "\nClient with Account Number " << AccountNumber << " not found.\n" << endl;
    }
}

void ShowDeleteClientScreen() {

    string AccountNumber;
    cout << "\n_______________________________________________________";
    cout << "\n\t\tDelete Client Screen";
    cout << "\n_______________________________________________________\n";
    cout << "Enter Account Number of the client to delete: ";
    cin >> AccountNumber;
    vector<sClient> vClients = LoadCleintsDataFromFile(filename);
    DeleteMarkedClients(AccountNumber, vClients);
}

sClient ChangeClientRecord(string AccountNumber) {
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter new Pin Code ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter new Name ";
    getline(cin, Client.Name);

    cout << "Enter new Phone ";
    getline(cin, Client.Phone);

    cout << "Enter new Account Balance ";
    cin >> Client.AccountBalance;

    return Client;
}

void UpdateMarkedClients(string AccountNumber, vector<sClient>& vClients) {

    sClient Client;
    char Answer;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        PrintClientCard(Client);

        cout << "Are you sure you want to update this client? (Y/N): ";
        cin >> Answer;

        if (toupper(Answer) == 'Y') {

            for (sClient& C : vClients) {
                if (C.AccountNumber == AccountNumber) {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientsDataToFile(filename, vClients);

            cout << "\nClient updated successfully.\n" << endl;
        }
        else {
            cout << "\nClient update canceled.\n" << endl;
        }
    }
    else {
        cout << "\nClient with Account Number " << AccountNumber << " not found.\n" << endl;
    }
}

void ShowUpdateClientScreen() {

    string AccountNumber;
    cout << "\n_______________________________________________________";
    cout << "\n\t\tUpdate Client Screen";
    cout << "\n_______________________________________________________\n";
    cout << "Enter Account Number of the client to update: ";
    cin >> AccountNumber;
    vector<sClient> vClients = LoadCleintsDataFromFile(filename);
    UpdateMarkedClients(AccountNumber, vClients);
}

void ShowFindClientScreen() {
    string AccountNumber;
    sClient Client;
    cout << "\n_______________________________________________________";
    cout << "\n\t\tFind Client Screen";
    cout << "\n_______________________________________________________\n";
    cout << "Enter Account Number of the client to find: ";
    cin >> AccountNumber;
    vector<sClient> vClients = LoadCleintsDataFromFile(filename);
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintClientCard(Client);
    }
    else {
        cout << "\nClient with Account Number " << AccountNumber << " not found.\n" << endl;
    }
}

void ShowExitScreen() {
    cout << "\n_______________________________________________________";
    cout << "\n\t\tExit Screen";
    cout << "\n_______________________________________________________\n";
    cout << "Thank you for using the Client Management System. Goodbye!\n" << endl;
}

short ReadMainMenuOption(string txt) {
    short choice;
    cout << txt;
    cin >> choice;
    return choice;
}

void GoBackToMainMenu() {

    cout << "\nPress any key to go back to the Main Menu...";
    system("pause>nul");
    system("cls");
    ShowMainMenu();
}

void GoBackToTransactionsMenu() {

    cout << "\nPress any key to go back to the Transactions Menu...";
    system("pause>nul");
    system("cls");
    ShowTransactionsMenu();
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients) {

    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(filename, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }
}

void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowTotalBalances() {
    vector <sClient> vClients = LoadCleintsDataFromFile(filename);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;
}

void ShowDepositScreen() {

    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(filename);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithdrawScreen() {

    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;
    vector <sClient> vClients = LoadCleintsDataFromFile(filename);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}


void ShowTotalBalancesScreen() {

    ShowTotalBalances();
}

void PerformTransactionsOption(enTransactionsMenuOptions Transactions) {

    switch (Transactions) {
    case eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case eWithdraw:
    {
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case eTotalBalances:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case eMainMenu:
    {
        ShowMainMenu();
        break;
    }
    }
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption) {

    switch (MainMenuOption)
    {
    case eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    }
    case eAddNewClient:
    {
        system("cls");
        ShowAddNewClientScreen();
        GoBackToMainMenu();
        break;
    }
    case eDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    }
    case eUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    }
    case eFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    }
    case eTransactions:
    {
        system("cls");
        ShowTransactionsMenu();
        break;
    }
    case eExit:
    {
        system("cls");
        ShowExitScreen();
        break;
    }
    }
}

void ShowMainMenu() {
    system("cls");
    cout << "\n_______________________________________________________";
    cout << "\n\t\tMain Menu";
    cout << "\n_______________________________________________________\n";
    cout << " \t[1] List Clients\n";
    cout << " \t[2] Add New Client\n";
    cout << " \t[3] Delete Client\n";
    cout << " \t[4] Update Client\n";
    cout << " \t[5] Find Client\n";
    cout << " \t[6] Transactions\n";
    cout << " \t[7] Exit\n";
    cout << "_______________________________________________________\n";

    PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption("Please enter your choice (1-7): "));
}

void ShowTransactionsMenu() {
    cout << "\n_______________________________________________________";
    cout << "\n\t\t Transactions Menu";
    cout << "\n_______________________________________________________\n";
    cout << " \t[1] Deposit\n";
    cout << " \t[2] Withdraw\n";
    cout << " \t[3] Total Balances\n";
    cout << " \t[4] Main Menu\n";
    cout << "_______________________________________________________\n";
    PerformTransactionsOption((enTransactionsMenuOptions)ReadMainMenuOption("Please enter your choice (1-4): "));
}

int main() {

    ShowMainMenu();
    system("pause");
    return 0;
}