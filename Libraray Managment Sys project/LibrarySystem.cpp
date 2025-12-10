#ifndef LIBRARYSYSTEM_CPP
#define LIBRARYSYSTEM_CPP

#include "User.cpp"
#include "Student.cpp"
#include "Staff.cpp"
#include "Admin.cpp"
#include "Book.cpp"
#include "Transaction.cpp"
#include "Utils.cpp"
#include <vector>
#include <string>

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

    void login(string requiredRole) {
        int id; string pass;
        
        while (true) { // Retry Loop
            cout << "\n=== " << requiredRole << " LOGIN ===\n";
            cout << "Enter ID (Number) [0 to Cancel]: "; 
            id = getIntInput();
            
            if (id == 0) return; 
            
            cout << "Enter Password: "; cin >> pass;

            bool found = false;
            for (size_t i = 0; i < users.size(); ++i) {
                if (users[i]->getRole() == requiredRole && users[i]->login(id, pass)) {
                    currentUser = users[i];
                    logActivity(requiredRole + " [" + intToString(users[i]->getID()) + "] logged in.");
                    cout << "Login Successful. Welcome " << users[i]->getName() << "!\n";
                    return;
                }
            }
            
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

#endif // LIBRARYSYSTEM_CPP
