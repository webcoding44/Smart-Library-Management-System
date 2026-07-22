#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "BST.h"
#include "HashTable.h"
#include "Graph.h"
#include <vector>
#include <string>

// ساختار برای Undo
struct UndoAction {
    std::string actionType; // ADD, DELETE, EDIT, BORROW, RETURN
    std::string bookId;
    Book bookData;
    std::string extraInfo;
};

// ساختار برای تاریخچه امانت
struct BorrowRecord {
    std::string bookId;
    std::string bookTitle;
    std::string borrower;
    std::string borrowDate;
    std::string returnDate;
    
    bool operator==(const BorrowRecord& other) const {
        return bookId == other.bookId && borrower == other.borrower;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const BorrowRecord& record) {
        os << "[" << record.borrowDate << "] " << record.bookTitle 
           << " -> " << record.borrower;
        if (!record.returnDate.empty()) {
            os << " (بازگشت: " << record.returnDate << ")";
        }
        return os;
    }
};

// ساختار برای رزرو
struct Reservation {
    std::string bookId;
    std::string borrower;
    std::string reservationDate;
    
    friend std::ostream& operator<<(std::ostream& os, const Reservation& res) {
        os << "کتاب " << res.bookId << " - " << res.borrower 
           << " (" << res.reservationDate << ")";
        return os;
    }
};

class Library {
private:
    // ساختمان داده‌های اصلی
    std::vector<Book> booksArray;              // Array
    std::vector<Book> booksVector;             // Vector
    LinkedList<BorrowRecord> borrowHistory;    // Linked List
    Stack<UndoAction> undoStack;               // Stack
    Queue<Reservation> reservationQueue;       // Queue
    BST<std::string, int> bookIndexBST;        // BST
    HashTable<std::string, std::vector<int>> titleHash; // Hash Table
    Graph bookGraph;                           // Graph
    
    std::string dataFile;
    std::string historyFile;

    void updateDataStructures();
    void buildGraph();
    std::string getCurrentDate() const;

public:
    Library(std::string dataFile = "smart_library.txt", 
            std::string historyFile = "borrow_history.txt");
    ~Library();

    // عملیات اصلی
    void addBook(Book book);
    void deleteBook(std::string bookId);
    void editBook(std::string bookId, Book newBook);
    void borrowBook(std::string bookId, std::string borrower);
    void returnBook(std::string bookId);

    // Undo
    void undo();

    // رزرو
    void reserveBook(std::string bookId, std::string borrower);
    void processReservations(std::string bookId);

    // جستجو
    Book* linearSearchById(std::string id);
    Book* binarySearchById(std::string id);
    Book* hashSearchByTitle(std::string title);
    std::vector<Book> searchByAuthor(std::string author);
    std::vector<Book> searchByGenre(std::string genre);

    // مرتب‌سازی
    void bubbleSortByTitle();
    void mergeSortByYear();
    void quickSortByAuthor();

    // نمایش
    void displayAllBooks() const;
    void displayBorrowHistory() const;
    void displayReservationQueue() const;
    void displayUndoStack() const;

    // پیشنهاد
    std::vector<Book> suggestRelatedBooks(std::string bookId);
    void displayBookGraph() const;

    // آمار
    void displayStatistics() const;

    // فایل
    void saveToFile() const;
    void loadFromFile();
    void saveHistory() const;
    void loadHistory();
};

#endif // LIBRARY_H