#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Expense {
    string category;
    double amount;
    string description;
    string date;
};

const int MAX_EXPENSES = 100;

void Displaymenu() {
    cout << "\n\t\t\tExpense Tracking System\n";
    cout << "-----------------------------------------------------------------\n";
    cout << "\t\t-----Expense Tracker Menu----- \n";
    cout << "1. Add an expense \n";
    cout << "2. View all expenses \n";
    cout << "3. Calculate total expenses \n";
    cout << "4. Delete expense \n";
    cout << "5. Edit expenses \n";
    cout << "6. Exit \n";
    cout << "Enter your choice : ";
}

bool isValidDate(int day, int month, int year) {
    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < 1) return false;
    return true;
}

void loadExpenses(Expense expenses[], int& count) {
    ifstream inFile("expenses.txt");
    if (!inFile) {
        cout << "No existing expenses found.\n";
        return;
    }

    count = 0; // Ensure count starts fresh when loading from the file
    Expense temp;
    string separator;
    while (getline(inFile, temp.date) &&
           getline(inFile, temp.category) &&
           inFile >> temp.amount &&
           inFile.ignore() &&
           getline(inFile, temp.description) &&
           getline(inFile, separator)) {
        expenses[count++] = temp;
    }

    inFile.close();
}

void saveExpenses(const Expense expenses[], int count) {
    ofstream outFile("expenses.txt");
    if (!outFile) {
        cout << "Error saving expenses to file.\n";
        return;
    }

    for (int i = 0; i < count; ++i) {
        outFile << "Date: " << expenses[i].date << '\n';
        outFile << "Category: " << expenses[i].category << '\n';
        outFile << "Amount: " << expenses[i].amount << '\n';
        outFile << "Description: " << expenses[i].description << '\n';
        outFile << "-----\n"; // Separator for each expense
    }

    outFile.close();
    cout << "Expenses saved successfully.\n";
}

void addExpense(Expense expenses[], int& count) {
    if (count >= MAX_EXPENSES) {
        cout << "Sorry, you've reached the maximum limit.\n";
        return;
    }

    Expense newExpense;
    int day, month, year;
    do {
        cout << "Enter date (DD MM YYYY): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) {
            cout << "Invalid date. Please try again.\n";
        }
    } while (!isValidDate(day, month, year));
    newExpense.date = to_string(day) + "/" + to_string(month) + "/" + to_string(year);

    cout << "Enter expense category: ";
    cin.ignore();
    getline(cin, newExpense.category);

    cout << "Enter expense amount: ";
    cin >> newExpense.amount;

    cout << "Enter expense description: ";
    cin.ignore();
    getline(cin, newExpense.description);

    expenses[count++] = newExpense;
    cout << "New expense added successfully.\n";
}

void viewExpense(const Expense expenses[], int count) {
    if (count == 0) {
        cout << "No expense has been added yet.\n";
        return;
    }
    cout << "List of all expenses:\n";
    cout << "-------------------------------------\n";
    for (int i = 0; i < count; ++i) {
        cout << i + 1 << ". Category: " << expenses[i].category
             << "\tAmount: $" << expenses[i].amount
             << "\tDescription: " << expenses[i].description
             << "\tDate: " << expenses[i].date << "\n";
    }
}

void editExpense(Expense expenses[], int count) {
    if (count == 0) {
        cout << "No expenses to edit.\n";
        return;
    }

    int index;
    viewExpense(expenses, count);
    cout << "Enter the number of the expense you want to edit: ";
    cin >> index;

    if (index < 1 || index > count) {
        cout << "Invalid expense number.\n";
        return;
    }

    Expense& expenseToEdit = expenses[index - 1];
    cout << "Current details:\n";
    cout << "Category: " << expenseToEdit.category << "\n";
    cout << "Amount: $" << expenseToEdit.amount << "\n";
    cout << "Description: " << expenseToEdit.description << "\n";
    cout << "Date: " << expenseToEdit.date << "\n";

    cout << "\nEnter new details (leave empty to keep the current value):\n";
    int day, month, year;
    do {
        cout << "Enter new date (DD MM YYYY): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) {
            cout << "Invalid date. Please try again.\n";
        }
    } while (!isValidDate(day, month, year));
    expenseToEdit.date = to_string(day) + "/" + to_string(month) + "/" + to_string(year);

    cin.ignore();
    string input;
    cout << "Enter new category: ";
    getline(cin, input);
    if (!input.empty()) expenseToEdit.category = input;

    cout << "Enter new amount: ";
    getline(cin, input);
    if (!input.empty()) expenseToEdit.amount = stod(input);

    cout << "Enter new description: ";
    getline(cin, input);
    if (!input.empty()) expenseToEdit.description = input;

    cout << "Expense updated successfully.\n";
}

void calculateExpense(const Expense expenses[], int count) {
    if (count == 0) {
        cout << "No expenses to calculate.\n";
        return;
    }

    double total = 0;
    for (int i = 0; i < count; ++i) {
        total += expenses[i].amount;
    }
    cout << "Total expense = $" << total << "\n";
}

void deleteExpense(Expense expenses[], int& count) {
    if (count == 0) {
        cout << "No expenses to delete.\n";
        return;
    }

    int index;
    viewExpense(expenses, count);
    cout << "Enter the number of the expense you want to delete: ";
    cin >> index;

    if (index < 1 || index > count) {
        cout << "Invalid expense number.\n";
        return;
    }

    for (int i = index - 1; i < count - 1; ++i) {
        expenses[i] = expenses[i + 1];
    }
    count--;
    cout << "Expense deleted successfully.\n";
}

int main() {
    Expense expenses[MAX_EXPENSES];
    int choice;
    int count = 0;

    loadExpenses(expenses, count); // Load existing expenses

    do {
        Displaymenu();
        cin >> choice;
        switch (choice) {
        case 1:
            addExpense(expenses, count);
            break;
        case 2:
            viewExpense(expenses, count);
            break;
        case 3:
            calculateExpense(expenses, count);
            break;
        case 4:
            deleteExpense(expenses, count);
            break;
        case 5:
            editExpense(expenses, count);
            break;
        case 6:
            saveExpenses(expenses, count); // Save expenses before exiting
            cout << "Exiting the system. GOODBYE!";
            break;
        default:
            cout << "Invalid choice. Please try again.";
            break;
        }
    } while (choice != 6);

    return 0;
}
