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

#include "LibrarySystem.cpp"

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
