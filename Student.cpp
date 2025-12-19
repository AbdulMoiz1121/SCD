#ifndef STUDENT_CPP
#define STUDENT_CPP

#include "User.cpp"

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

#endif // STUDENT_CPP
