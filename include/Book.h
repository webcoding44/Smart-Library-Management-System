#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Book {
private:
    std::string id;
    std::string title;
    std::string author;
    std::string genre;
    int year;
    std::string isbn;
    std::vector<std::string> keywords;
    bool isBorrowed;
    std::string borrower;
    std::string borrowDate;

public:
    Book();
    Book(std::string id, std::string title, std::string author, 
         std::string genre, int year, std::string isbn);

    // Getters
    std::string getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getGenre() const;
    int getYear() const;
    std::string getIsbn() const;
    std::vector<std::string> getKeywords() const;
    bool getIsBorrowed() const;
    std::string getBorrower() const;
    std::string getBorrowDate() const;

    // Setters
    void setTitle(std::string title);
    void setAuthor(std::string author);
    void setGenre(std::string genre);
    void setYear(int year);
    void setIsbn(std::string isbn);
    void setKeywords(std::vector<std::string> keywords);
    void addKeyword(std::string keyword);
    void setBorrower(std::string borrower);
    void setBorrowDate(std::string date);
    void setIsBorrowed(bool status);

    // Operations
    void borrow(std::string borrowerName);
    void returnBook();
    void display() const;

    // Comparison
    bool operator<(const Book& other) const;
    bool operator>(const Book& other) const;
    bool operator==(const Book& other) const;

    // File I/O
    friend std::ofstream& operator<<(std::ofstream& out, const Book& book);
    friend std::ifstream& operator>>(std::ifstream& in, Book& book);
};

#endif // BOOK_H