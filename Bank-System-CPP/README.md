# Bank System C++  

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue) ![License](https://img.shields.io/badge/License-MIT-green)

A simple **Bank Management System** written in **C++**.  
This project allows you to manage clients, their accounts, and perform transactions in a console-based environment.

---

## 🗂 Project Structure

Bank-System-CPP/
│
├─ data/
│ └─ Clients.txt # File to store client data
│
├─ src/
│ ├─ main.cpp # Main program and menu interface
│
├─ .gitignore # Ignore build files (exe, o, etc.)
└─ README.md # Project overview


---

## ⚙ Features

- Add new clients with unique account numbers
- List all clients
- Delete clients safely
- Update client information
- Find client by account number
- Perform transactions:
  - Deposit
  - Withdraw
  - Show total balances of all clients
- File-based storage (`Clients.txt`) to persist client data

---

## 🖥 Usage

1. Clone the repository:
```bash
git clone https://github.com/DEADCODE0O/Bank-System-CPP.git
Navigate to the project folder:

cd Bank-System-CPP/src
Compile the code using a C++ compiler (e.g., g++):

g++ main.cpp -o bank.exe
Run the program:

./bank.exe   # or bank.exe on Windows
Follow the on-screen menu to manage clients and perform transactions.

🛠️ Dependencies
Standard C++ library (no external dependencies)

Compatible with Windows, Linux, and Mac (console-based)

📂 Data Storage
All client data is stored in data/Clients.txt

Format: AccountNumber#//#PinCode#//#Name#//#Phone#//#AccountBalance

Example:

12345#//#4321#//#John Doe#//#0123456789#//#5000.50
💡 Notes
Input validation is implemented to prevent duplicate account numbers and invalid transactions.

The system uses console-based menus for easy navigation.

Designed for learning and practice in file handling, structs, and basic C++ programming.
