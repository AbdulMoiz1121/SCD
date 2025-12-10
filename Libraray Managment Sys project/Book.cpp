#ifndef BOOK_CPP
#define BOOK_CPP

#include "Utils.cpp"
#include <iostream>
#include <iomanip>

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

#endif // BOOK_CPP
