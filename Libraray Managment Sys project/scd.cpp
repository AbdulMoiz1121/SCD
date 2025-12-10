/*
 * Project: Library Management System (Expert Edition)
 * Course: Software Construction & Development
 * Students: Abdul Moiz Mansab, Muhammad Aqeel
 * Features: 
 * - Login Retry Loop (New!)
 * - Auto-Increment IDs
 * - Self-Service Password Change
 * - Low Stock Alerts
 * - C++98 Compatible
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream> 
#include <cctype>

using namespace std;

// ==========================================
//              HELPER: UTILITIES
// ==========================================

int getIntInput() {
    int input;
    while (true) {
        if (cin >> input) {
            return input;
        } else {
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            cout << "Invalid input! Please enter a numeric value (Integer): ";
        }
    }
}

string intToString(int v) {
    stringstream ss;
    ss << v;
    return ss.str();
}

string toLower(string s) {
    string lowerStr = s;
    for(size_t i = 0; i < lowerStr.length(); ++i) lowerStr[i] = tolower(lowerStr[i]);
    return lowerStr;
}

struct Date {
    int day, month, year;
    string toString() const { return intToString(day) + "/" + intToString(month) + "/" + intToString(year); }
    static int daysBetween(Date d1, Date d2) {
        long t1 = d1.year * 365 + d1.month * 30 + d1.day;
        long t2 = d2.year * 365 + d2.month * 30 + d2.day;
        return (int)(t2 - t1);
    }
    static Date getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        Date d; d.day = ltm->tm_mday; d.month = 1 + ltm->tm_mon; d.year = 1900 + ltm->tm_year;
        return d;
    }
};

// ==========================================
//              ENTITY CLASSES
// ==========================================

class Book {
private:
    int bookID;
    string title;
    string author;
    string category;
    int quantity;

public:
    Book(int id, string t, string a, string c, int q) 
        : bookID(id), title(t), author(a), category(c), quantity(q) {}

    int getID() const { return bookID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getCategory() const { return category; }
    int getQuantity() const { return quantity; }
    
    void updateDetails(string t, string a, string c, int q) {
        title = t; author = a; category = c; quantity = q;
    }

    void decreaseQuantity() { if (quantity > 0) quantity--; }
    void increaseQuantity() { quantity++; }

    void display() const {
        cout << left << setw(10) << bookID << setw(25) << title 
             << setw(20) << author << setw(15) << category 
             << setw(5) << quantity << endl;
    }
};

class Transaction {
private:
    static int idCounter;
    int transactionID;
    int bookID;   
    int studentID;
    Date issueDate;
    Date dueDate;
    Date returnDate;
    bool isReturned;
    double fineAmount;

public:
    Transaction(int bID, int sID, Date due) 
        : bookID(bID), studentID(sID), dueDate(due) {
        transactionID = ++idCounter;
        issueDate = Date::getCurrentDate();
        isReturned = false;
        fineAmount = 0.0;
        returnDate.day = 0; returnDate.month = 0; returnDate.year = 0;
    }

    int getBookID() const { return bookID; }
    int getStudentID() const { return studentID; }
    bool getIsReturned() const { return isReturned; }
    double getFine() const { return fineAmount; }
    Date getDueDate() const { return dueDate; }
    Date getIssueDate() const { return issueDate; }

    void markReturned() {
        returnDate = Date::getCurrentDate();
        isReturned = true;
        int overdueDays = Date::daysBetween(dueDate, returnDate);
        if (overdueDays > 0) fineAmount = overdueDays * 10.0;
        else fineAmount = 0.0;
    }

    void display() const {
        cout << left << setw(5) << transactionID << setw(10) << bookID 
             << setw(10) << studentID << setw(12) << issueDate.toString()
             << setw(12) << dueDate.toString() 
             << setw(10) << (isReturned ? "Yes" : "No") 
             << "$" << setw(10) << fineAmount << endl;
    }
};

int Transaction::idCounter = 0;

// ==========================================
//          USER HIERARCHY
// ==========================================

class User {
protected:
    int id;
    string name;
    string password;
    string role; 
    bool isActive;

public:
    User(int i, string n, string p, string r) : id(i), name(n), password(p), role(r), isActive(true) {}
    virtual ~User() {}

    int getID() const { return id; }
    string getRole() const { return role; }
    string getName() const { return name; }
    bool getIsActive() const { return isActive; }
    
    void updateBasicDetails(string n, string p) { name = n; password = p; }
    void deactivate() { isActive = false; }
    bool login(int i, string p) { return (id == i && password == p && isActive); }
    
    // Logic Improvement 2 (Self-Service)
    void changePassword(string newPass) { password = newPass; }

    virtual void displayMenu() = 0; 
};

class Student : public User {
private:
    string className; 
public:
    Student(int i, string n, string p, string c) : User(i, n, p, "Student"), className(c) {}
    void updateStudentDetails(string n, string p, string c) { updateBasicDetails(n, p); className = c; }

    void displayMenu() {
        cout << "\n--- Student Menu (ID: " << id << ") ---\n";
        cout << "1. Search Catalog\n";
        cout << "2. View My Issued Books\n";
        cout << "3. Check My Fines\n";
        cout << "4. Notifications\n";
        cout << "5. Change Password\n"; 
        cout << "6. Logout\n";
    }
};

class Staff : public User {
public:
    Staff(int i, string n, string p) : User(i, n, p, "Staff") {}

    void displayMenu() {
        cout << "\n--- Library Staff Menu (ID: " << id << ") ---\n";
        cout << "1. Add New Book\n";
        cout << "2. Update Book Details\n";
        cout << "3. Remove Book\n";
        cout << "4. Issue Book to Student\n";
        cout << "5. Return Book from Student\n";
        cout << "6. Create Student Account\n"; 
        cout << "7. Update Student Details\n";
        cout << "8. Deactivate Student\n";
        cout << "9. Change Password\n"; 
        cout << "10. Logout\n";
    }
};

class Admin : public User {
public:
    Admin(int i, string n, string p) : User(i, n, p, "Admin") {}

    void displayMenu() {
        cout << "\n--- Admin Menu (ID: " << id << ") ---\n";
        cout << "1. Add Staff Member\n";
        cout << "2. Update Staff Details\n";
        cout << "3. Remove Staff Member\n";
        cout << "4. Monitor Transactions\n";
        cout << "5. Track Overdue & Fines\n";
        cout << "6. Oversee User Activity\n";
        cout << "7. Report: Book Usage\n";
        cout << "8. Report: Fine Collection\n";
        cout << "9. Report: User Activity\n"; 
        cout << "10. Logout\n";
    }
};

// ==========================================
//       SYSTEM CONTROLLER
// ==========================================

class LibrarySystem {
private:
    vector<User*> users;
    vector<Book> inventory;
    vector<Transaction> transactions;
    vector<string> activityLog;
    User* currentUser;

    void logActivity(string msg) {
        activityLog.push_back(Date::getCurrentDate().toString() + ": " + msg);
    }

    // Logic Improvement 1 (Auto-ID Generation)
    int getNextUserID() {
        int maxID = 0;
        for(size_t i=0; i<users.size(); ++i) {
            if(users[i]->getID() > maxID) maxID = users[i]->getID();
        }
        return maxID + 1;
    }

    int getNextBookID() {
        int maxID = 1000; 
        for(size_t i=0; i<inventory.size(); ++i) {
            if(inventory[i].getID() > maxID) maxID = inventory[i].getID();
        }
        return maxID + 1;
    }

    LibrarySystem(const LibrarySystem&);
    LibrarySystem& operator=(const LibrarySystem&);

public:
    LibrarySystem() {
        users.push_back(new Admin(1, "Abdul Moiz", "admin123"));
        inventory.push_back(Book(1001, "Software Eng", "Sommerville", "CS", 5));
        inventory.push_back(Book(1002, "Data Structures", "Horowitz", "CS", 3));
        inventory.push_back(Book(1003, "Harry Potter", "JK Rowling", "Fiction", 2));
        currentUser = 0; 
    }

    ~LibrarySystem() { 
        for (size_t i = 0; i < users.size(); ++i) delete users[i]; 
    }

    // --- LOGIN RE-TRY LOGIC ---
    void login(string requiredRole) {
        int id; string pass;
        
        while (true) { // Retry Loop
            cout << "\n=== " << requiredRole << " LOGIN ===\n";
            cout << "Enter ID (Number) [0 to Cancel]: "; 
            id = getIntInput();
            
            // Logic to break loop if user wants to cancel
            if (id == 0) return; 
            
            cout << "Enter Password: "; cin >> pass;

            bool found = false;
            for (size_t i = 0; i < users.size(); ++i) {
                if (users[i]->getRole() == requiredRole && users[i]->login(id, pass)) {
                    currentUser = users[i];
                    logActivity(requiredRole + " [" + intToString(users[i]->getID()) + "] logged in.");
                    cout << "Login Successful. Welcome " << users[i]->getName() << "!\n";
                    return; // EXIT FUNCTION ON SUCCESS
                }
            }
            
            // If we are here, login failed. Loop continues.
            cout << "Error: Invalid Credentials. Please try again.\n";
        }
    }

    void logout() {
        if(currentUser) logActivity("User [" + intToString(currentUser->getID()) + "] logged out.");
        currentUser = 0;
        cout << "Logged out successfully.\n";
    }

    bool isLoggedIn() { return currentUser != 0; }
    User* getCurrentUser() { return currentUser; }
    
    // Logic Improvement 2 (Self-Service)
    void changeMyPassword() {
        string newPass;
        cout << "\n--- Change Password ---\n";
        cout << "Enter New Password: "; cin >> newPass;
        if(currentUser) {
            currentUser->changePassword(newPass);
            cout << "Password updated successfully.\n";
            logActivity("User " + intToString(currentUser->getID()) + " changed password.");
        }
    }

    // ================= ADMIN FUNCTIONS =================
    void addStaff() {
        string name, pass;
        int id = getNextUserID(); 
        
        cout << "\n--- Add New Staff ---\n";
        cout << "Auto-Assigned ID: " << id << endl;
        
        cout << "Enter Name: "; cin.ignore(); getline(cin, name);
        cout << "Enter Password: "; cin >> pass;
        users.push_back(new Staff(id, name, pass));
        logActivity("Admin created staff account: " + intToString(id));
        cout << "Staff account created successfully with ID: " << id << "\n";
    }

    void updateStaff() {
        int id; string name, pass;
        cout << "Enter Staff ID to update (Number) [0 to Cancel]: "; 
        id = getIntInput(); if(id==0) return;
        
        for(size_t i = 0; i < users.size(); ++i) {
            Staff* s = dynamic_cast<Staff*>(users[i]);
            if(s && s->getID() == id && s->getIsActive()) {
                cout << "Enter New Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter New Password: "; cin >> pass;
                s->updateBasicDetails(name, pass);
                logActivity("Admin updated staff: " + intToString(id));
                cout << "Staff details updated.\n";
                return;
            }
        }
        cout << "Staff not found.\n";
    }

    void removeStaff() {
        int id;
        cout << "Enter Staff ID to deactivate (Number) [0 to Cancel]: "; 
        id = getIntInput(); if(id==0) return;
        
        for (size_t i = 0; i < users.size(); ++i) {
            if (users[i]->getID() == id && dynamic_cast<Staff*>(users[i])) {
                char confirm;
                cout << "Are you sure? (y/n): "; cin >> confirm;
                if(confirm != 'y' && confirm != 'Y') return;

                users[i]->deactivate(); 
                logActivity("Admin deactivated staff: " + intToString(id));
                cout << "Staff account deactivated.\n";
                return;
            }
        }
        cout << "Staff not found.\n";
    }

    void monitorTransactions() {
        cout << "\n--- All Transaction Records ---\n";
        if(transactions.empty()) { cout << "No records found.\n"; return; }
        cout << left << setw(5) << "ID" << setw(10) << "BookID" << setw(10) << "StdID" << setw(12) << "Issue" << setw(12) << "Due" << "Fine" << endl;
        for(size_t i = 0; i < transactions.size(); ++i) transactions[i].display();
    }

    void trackFines() {
        cout << "\n--- Overdue & Fines Report ---\n";
        bool found = false;
        for(size_t i = 0; i < transactions.size(); ++i) {
            int overdue = Date::daysBetween(transactions[i].getDueDate(), Date::getCurrentDate());
            if(transactions[i].getFine() > 0 || (!transactions[i].getIsReturned() && overdue > 0)) {
                transactions[i].display();
                found = true;
            }
        }
        if(!found) cout << "No overdue books.\n";
    }

    void viewActivityLog() {
        cout << "\n--- System Activity Log ---\n";
        for(size_t i = 0; i < activityLog.size(); ++i) cout << activityLog[i] << endl;
    }

    void generateReport(int type) {
        cout << "\n--- GENERATING REPORT ---\n";
        if (type == 1) { 
            cout << "[Book Usage Report]\n";
            int issuedCount = 0;
            for(size_t i = 0; i < transactions.size(); ++i) 
                if(!transactions[i].getIsReturned()) { transactions[i].display(); issuedCount++; }
            cout << "Total Books Currently Issued: " << issuedCount << endl;
        } else if (type == 2) { 
            double total = 0;
            for(size_t i = 0; i < transactions.size(); ++i) total += transactions[i].getFine();
            cout << "Total Fines Accumulated: $" << total << endl;
        } else if (type == 3) { 
            cout << "[User Activity Report]\n";
            for(size_t i = 0; i < activityLog.size(); ++i) cout << i+1 << ". " << activityLog[i] << endl;
        }
    }

    // ================= STAFF FUNCTIONS =================
    void addBook() {
        string t, a, c; int q;
        int id = getNextBookID();
        
        cout << "\n--- Add New Book ---\n";
        cout << "Auto-Assigned Book ID: " << id << endl;
        
        cout << "Title: "; cin.ignore(); getline(cin, t);
        cout << "Author: "; getline(cin, a);
        cout << "Category: "; getline(cin, c);
        cout << "Quantity: "; q = getIntInput();
        
        inventory.push_back(Book(id, t, a, c, q));
        logActivity("Staff added book: " + intToString(id));
        cout << "Book added successfully with ID: " << id << "\n";
    }
    
    void updateBook() {
        int id; 
        cout << "Enter Book ID to update (Number) [0 to Cancel]: "; 
        id = getIntInput(); if(id==0) return;
        
        for(size_t i = 0; i < inventory.size(); ++i) {
            if(inventory[i].getID() == id) {
                string t, a, c; int q;
                cout << "New Title: "; cin.ignore(); getline(cin, t);
                cout << "New Author: "; getline(cin, a);
                cout << "New Category: "; getline(cin, c);
                cout << "New Qty: "; q = getIntInput();
                inventory[i].updateDetails(t, a, c, q);
                logActivity("Staff updated book: " + intToString(id));
                cout << "Book updated.\n"; return;
            }
        }
        cout << "Book not found.\n";
    }

    void removeBook() {
        int id; 
        cout << "Enter Book ID to remove (Number) [0 to Cancel]: "; 
        id = getIntInput(); if(id==0) return;
        
        for(size_t i = 0; i < inventory.size(); ++i) {
            if(inventory[i].getID() == id) {
                char confirm;
                cout << "Are you sure? (y/n): "; cin >> confirm;
                if(confirm != 'y' && confirm != 'Y') return;

                inventory.erase(inventory.begin() + i);
                logActivity("Staff removed book: " + intToString(id));
                cout << "Book removed from inventory.\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void issueBook() { 
        int sID, bID, days;
        cout << "\n--- Issue Book ---\n";
        cout << "Enter Student ID (Number) [0 to Cancel]: "; 
        sID = getIntInput(); if(sID == 0) return;
        
        bool validStudent = false;
        for(size_t i = 0; i < users.size(); ++i) 
            if(users[i]->getID() == sID && dynamic_cast<Student*>(users[i]) && users[i]->getIsActive()) validStudent = true;
        
        if(!validStudent) { cout << "Invalid or Inactive Student ID.\n"; return; }

        cout << "Enter Book ID (Number): "; bID = getIntInput();
        
        for(size_t i = 0; i < inventory.size(); ++i) {
            if(inventory[i].getID() == bID) {
                if(inventory[i].getQuantity() > 0) {
                    cout << "Issue for (days): "; days = getIntInput();
                    Date due = Date::getCurrentDate(); due.day += days; 
                    transactions.push_back(Transaction(bID, sID, due));
                    inventory[i].decreaseQuantity();
                    
                    logActivity("Issued book " + intToString(bID) + " to " + intToString(sID));
                    cout << "Book Issued. Due: " << due.toString() << "\n";
                    
                    // Logic 3: Low Stock Alert
                    if(inventory[i].getQuantity() == 0) {
                        cout << "[WARNING] Inventory for this book is now EMPTY (0)!\n";
                    }
                    return;
                } else {
                    cout << "Book is unavailable (0 quantity).\n"; return;
                }
            }
        }
        cout << "Book ID not found.\n";
    }

    void returnBook() { 
        int bID, sID;
        cout << "\n--- Return Book ---\n";
        cout << "Enter Book ID (Number) [0 to Cancel]: "; bID = getIntInput(); if(bID==0) return;
        cout << "Enter Student ID (Number): "; sID = getIntInput();
        
        for(size_t i = 0; i < transactions.size(); ++i) {
            if(transactions[i].getBookID() == bID && transactions[i].getStudentID() == sID && !transactions[i].getIsReturned()) {
                transactions[i].markReturned();
                
                for(size_t j = 0; j < inventory.size(); ++j) {
                    if(inventory[j].getID() == bID) {
                        inventory[j].increaseQuantity();
                        cout << "System Update: Book '" << inventory[j].getTitle() << "' is now marked as Available.\n";
                    }
                }
                
                logActivity("Returned book " + intToString(bID) + " from " + intToString(sID));
                cout << "Book Returned.\n";
                if(transactions[i].getFine() > 0) cout << "*** FINE: $" << transactions[i].getFine() << " ***\n";
                else cout << "No fines applied.\n";
                return;
            }
        }
        cout << "No active issue record found.\n";
    }

    void createStudent() { 
        string name, pass, cls;
        int id = getNextUserID();
        
        cout << "\n--- Create Student Account ---\n";
        cout << "Auto-Assigned Student ID: " << id << endl;
        
        cout << "Name: "; cin.ignore(); getline(cin, name);
        cout << "Password: "; cin >> pass;
        cout << "Class: "; cin >> cls;
        users.push_back(new Student(id, name, pass, cls));
        logActivity("Staff created student: " + intToString(id));
        cout << "Student account created with ID: " << id << "\n";
    }

    void updateStudent() {
        int id; string name, pass, cls;
        cout << "\n--- Update Student Details ---\n";
        cout << "Enter Student ID (Number) [0 to Cancel]: "; 
        id = getIntInput(); if(id==0) return;
        
        for(size_t i = 0; i < users.size(); ++i) {
            Student* s = dynamic_cast<Student*>(users[i]);
            if(s && s->getID() == id && s->getIsActive()) {
                cout << "Enter New Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter New Password: "; cin >> pass;
                cout << "Enter New Class: "; cin >> cls;
                s->updateStudentDetails(name, pass, cls);
                logActivity("Staff updated student: " + intToString(id));
                cout << "Updated successfully.\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    void deactivateStudent() {
        int id;
        cout << "\n--- Deactivate Student Account ---\n";
        cout << "Enter Student ID (Number) [0 to Cancel]: "; 
        id = getIntInput(); if(id==0) return;

        for(size_t i = 0; i < users.size(); ++i) {
            Student* s = dynamic_cast<Student*>(users[i]);
            if(s && s->getID() == id) {
                char confirm;
                cout << "Are you sure? (y/n): "; cin >> confirm;
                if(confirm != 'y' && confirm != 'Y') return;

                s->deactivate();
                logActivity("Staff deactivated student: " + intToString(id));
                cout << "Deactivated successfully.\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    // ================= STUDENT FUNCTIONS =================
    void searchCatalog() {
        string query;
        cout << "Enter Title, Author, or Category: "; cin.ignore(); getline(cin, query);
        cout << "\nSearch Results:\n";
        cout << left << setw(10) << "ID" << setw(25) << "Title" << setw(20) << "Author" << endl;
        bool found = false;
        string lowerQuery = toLower(query);

        for(size_t i = 0; i < inventory.size(); ++i) {
            if(toLower(inventory[i].getTitle()).find(lowerQuery) != string::npos || 
               toLower(inventory[i].getAuthor()).find(lowerQuery) != string::npos || 
               toLower(inventory[i].getCategory()).find(lowerQuery) != string::npos) {
                inventory[i].display();
                found = true;
            }
        }
        if(!found) cout << "No matching books found.\n";
    }

    void viewStudentBooks(int sID) {
        cout << "\n--- My Issued Books ---\n";
        bool found = false;
        for(size_t i = 0; i < transactions.size(); ++i) {
            if(transactions[i].getStudentID() == sID && !transactions[i].getIsReturned()) {
                cout << "[Issued] Book ID: " << transactions[i].getBookID() 
                     << " | Due: " << transactions[i].getDueDate().toString() << endl;
                found = true;
            }
        }
        if(!found) cout << "You have no borrowed books.\n";
    }

    void viewStudentFines(int sID) {
        cout << "\n--- My Fine History ---\n";
        double total = 0;
        for(size_t i = 0; i < transactions.size(); ++i) {
            if(transactions[i].getStudentID() == sID && transactions[i].getFine() > 0) {
                cout << "Book " << transactions[i].getBookID() << ": $" << transactions[i].getFine() << endl;
                total += transactions[i].getFine();
            }
        }
        if(total == 0) cout << "No fines recorded.\n";
        else cout << "Total Payable: $" << total << endl;
    }

    void checkNotifications(int sID) {
        cout << "\n--- Notifications ---\n";
        bool note = false;
        for(size_t i = 0; i < transactions.size(); ++i) {
            if(transactions[i].getStudentID() == sID && !transactions[i].getIsReturned()) {
                int daysLeft = Date::daysBetween(Date::getCurrentDate(), transactions[i].getDueDate());
                if(daysLeft < 0) {
                    cout << "[ALERT] Book " << transactions[i].getBookID() << " is OVERDUE!\n";
                    note = true;
                } else if (daysLeft <= 2) {
                    cout << "[REMINDER] Book " << transactions[i].getBookID() << " is due soon.\n";
                    note = true;
                }
            }
        }
        if(!note) cout << "No urgent notifications.\n";
    }
};

// ==========================================
//              MAIN DASHBOARD
// ==========================================
int main() {
    LibrarySystem sys;
    int choice;

    while (true) {
        if (!sys.isLoggedIn()) {
            cout << "\n======================================\n";
            cout << "       LIBRARY DASHBOARD              \n";
            cout << "======================================\n";
            cout << "Identifty Yourself:\n";
            cout << "1. Admin\n";
            cout << "2. Library Staff\n";
            cout << "3. Student\n";
            cout << "4. Exit System\n";
            cout << "======================================\n";
            cout << "Select Option: ";
            
            choice = getIntInput();

            switch(choice) {
                case 1: sys.login("Admin"); break;
                case 2: sys.login("Staff"); break;
                case 3: sys.login("Student"); break;
                case 4: cout << "Exiting system. Goodbye!\n"; return 0;
                default: cout << "Invalid Selection.\n";
            }
        } else {
            User* u = sys.getCurrentUser();
            u->displayMenu();
            cout << "Select Option: ";
            choice = getIntInput();

            string role = u->getRole();

            if (role == "Admin") {
                switch(choice) {
                    case 1: sys.addStaff(); break;
                    case 2: sys.updateStaff(); break;
                    case 3: sys.removeStaff(); break;
                    case 4: sys.monitorTransactions(); break;
                    case 5: sys.trackFines(); break;
                    case 6: sys.viewActivityLog(); break;
                    case 7: sys.generateReport(1); break;
                    case 8: sys.generateReport(2); break;
                    case 9: sys.generateReport(3); break;
                    case 10: sys.logout(); break;
                    default: cout << "Invalid Option.\n";
                }
            } else if (role == "Staff") {
                switch(choice) {
                    case 1: sys.addBook(); break;
                    case 2: sys.updateBook(); break;
                    case 3: sys.removeBook(); break;
                    case 4: sys.issueBook(); break;
                    case 5: sys.returnBook(); break;
                    case 6: sys.createStudent(); break; 
                    case 7: sys.updateStudent(); break;
                    case 8: sys.deactivateStudent(); break;
                    case 9: sys.changeMyPassword(); break; // New
                    case 10: sys.logout(); break;
                    default: cout << "Invalid Option.\n";
                }
            } else if (role == "Student") {
                switch(choice) {
                    case 1: sys.searchCatalog(); break;
                    case 2: sys.viewStudentBooks(u->getID()); break;
                    case 3: sys.viewStudentFines(u->getID()); break;
                    case 4: sys.checkNotifications(u->getID()); break;
                    case 5: sys.changeMyPassword(); break; // New
                    case 6: sys.logout(); break;
                    default: cout << "Invalid Option.\n";
                }
            }
        }
    }
    return 0;
}