#ifndef STAFF_CPP
#define STAFF_CPP

#include "User.cpp"

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

#endif // STAFF_CPP
