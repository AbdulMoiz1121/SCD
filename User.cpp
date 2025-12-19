#ifndef USER_CPP
#define USER_CPP

#include <iostream>
#include <string>

using namespace std;

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
    
    void changePassword(string newPass) { password = newPass; }

    virtual void displayMenu() = 0; 
};

#endif // USER_CPP
