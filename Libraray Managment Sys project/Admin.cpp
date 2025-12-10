#ifndef ADMIN_CPP
#define ADMIN_CPP

#include "User.cpp"

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

#endif // ADMIN_CPP
