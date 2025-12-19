#ifndef TRANSACTION_CPP
#define TRANSACTION_CPP

#include "Utils.cpp"
#include <iostream>
#include <iomanip>

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

#endif // TRANSACTION_CPP
