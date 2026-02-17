# Bank System C++ – Update 1.1  

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue) ![License](https://img.shields.io/badge/License-MIT-green)

A **Bank Management System** written in **C++** with **user management and permissions**.  
This console-based project allows you to manage clients, users, accounts, and perform transactions securely.

---

## 🗂 Project Structure

```
Bank-System-CPP/
│
├─ data/
│  ├─ Clients.txt   # File to store client data
│  └─ Users.txt     # File to store user credentials and permissions
│
├─ src/
│  └─ main.cpp      # Main program with menu interfaces
│
├─ .gitignore       # Ignore build files (exe, o, etc.)
└─ README.md        # Project overview
```

---

## ⚙ Features (v1.1)

### Client Management
- Add new clients with unique account numbers
- List all clients
- Delete clients safely
- Update client information
- Find client by account number
- File-based storage (`Clients.txt`) persists client data

### User Management & Permissions
- Add, list, update, delete users
- Assign permissions for each user (Show List, Add, Delete, Update, Find, Transactions, Manage Users)
- Login system with username & password
- Restrict menu options based on user permissions
- File-based storage (`Users.txt`) persists user data

### Transactions
- Deposit funds into client accounts
- Withdraw funds with balance validation
- Show total balances of all clients

---

## 🖥 Usage

1. Clone the repository:
```bash
git clone https://github.com/DEADCODE0O/Bank-System-CPP.git
```

2. Navigate to the project folder:
```bash
cd Bank-System-CPP/src
```

3. Compile the code using a C++ compiler:
```bash
g++ main.cpp -o bank.exe
```

4. Run the program:
```bash
./bank.exe   # Linux/Mac
bank.exe     # Windows
```

5. Follow the on-screen menu to manage users, clients, and transactions.

---

## 📂 Data Storage

### Clients
File: `data/Clients.txt`  
Format:
```
AccountNumber#//#PinCode#//#Name#//#Phone#//#AccountBalance
```
Example:
```
12345#//#4321#//#John Doe#//#0123456789#//#5000.50
```

### Users
File: `data/Users.txt`  
Format:
```
Username#//#Password#//#Permissions
```
Permissions are stored as integer flags (bitmask).

---

## 💡 Notes & Improvements in Update 1.1

- Added **User Management** with login and permissions  
- Login attempts limited to 3 per session  
- Menu options now **dynamic based on user permissions**  
- Transactions screen supports deposit, withdraw, and total balances  
- Improved input validation for account numbers, balances, and passwords  
- Console-based menus enhanced for better clarity  
- Designed for **learning file handling, structs, enums, and basic C++ programming**  

---

## 🛠 Dependencies
- Standard C++ library (no external libraries required)  
- Console-based, compatible with Windows, Linux, and Mac

---

## 👨‍💻 Author
**Ahmed El-Wazir**  
Educational projects in C++, file handling, and console applications.

