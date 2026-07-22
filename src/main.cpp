#include "../include/Library.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

void printMenu() {
    cout << "\n╔══════════════════════════════════════════════╗" << endl;
    cout << "║   📚 Smart Library Management System 📚      ║" << endl;
    cout << "╠══════════════════════════════════════════════╣" << endl;
    cout << "║  1. افزودن کتاب                             ║" << endl;
    cout << "║  2. حذف کتاب                               ║" << endl;
    cout << "║  3. ویرایش کتاب                            ║" << endl;
    cout << "║  4. امانت کتاب                             ║" << endl;
    cout << "║  5. بازگرداندن کتاب                        ║" << endl;
    cout << "║  6. رزرو کتاب (Queue)                      ║" << endl;
    cout << "║  7. بازگشت عملیات (Undo - Stack)           ║" << endl;
    cout << "║  ─────────────────────────────────────────  ║" << endl;
    cout << "║  8. جستجوی خطی (Linear Search)             ║" << endl;
    cout << "║  9. جستجوی دودویی (Binary Search)          ║" << endl;
    cout << "║ 10. جستجوی سریع (Hash Table)               ║" << endl;
    cout << "║ 11. جستجو بر اساس نویسنده                  ║" << endl;
    cout << "║ 12. جستجو بر اساس ژانر                     ║" << endl;
    cout << "║  ─────────────────────────────────────────  ║" << endl;
    cout << "║ 13. مرتب‌سازی حبابی (Bubble Sort)          ║" << endl;
    cout << "║ 14. مرتب‌سازی ادغامی (Merge Sort)          ║" << endl;
    cout << "║ 15. مرتب‌سازی سریع (Quick Sort)            ║" << endl;
    cout << "║  ─────────────────────────────────────────  ║" << endl;
    cout << "║ 16. پیشنهاد کتاب مرتبط (Graph - BFS/DFS)   ║" << endl;
    cout << "║ 17. نمایش گراف کتاب‌ها                     ║" << endl;
    cout << "║  ─────────────────────────────────────────  ║" << endl;
    cout << "║ 18. نمایش همه کتاب‌ها                       ║" << endl;
    cout << "║ 19. نمایش تاریخچه امانت (Linked List)      ║" << endl;
    cout << "║ 20. نمایش صف رزرو (Queue)                  ║" << endl;
    cout << "║ 21. نمایش پشته Undo (Stack)                ║" << endl;
    cout << "║ 22. آمار کتابخانه                          ║" << endl;
    cout << "║  0. خروج                                   ║" << endl;
    cout << "╚══════════════════════════════════════════════╝" << endl;
    cout << "انتخاب شما: ";
}

int main() {
    Library library("smart_library.txt", "borrow_history.txt");
    int choice;

    do {
        printMenu();
        cin >> choice;

        try {
            switch (choice) {
                case 1: {
                    string id, title, author, genre, isbn;
                    int year;
                    
                    cout << "شناسه کتاب: ";
                    cin >> id;
                    cin.ignore();
                    
                    cout << "عنوان: ";
                    getline(cin, title);
                    
                    cout << "نویسنده: ";
                    getline(cin, author);
                    
                    cout << "ژانر: ";
                    getline(cin, genre);
                    
                    cout << "سال انتشار: ";
                    cin >> year;
                    cin.ignore();
                    
                    cout << "ISBN: ";
                    getline(cin, isbn);
                    
                    Book book(id, title, author, genre, year, isbn);
                    
                    string kw;
                    cout << "کلیدواژه‌ها (با , جدا کنید، برای پایان Enter): ";
                    getline(cin, kw);
                    if (!kw.empty()) {
                        stringstream ss(kw);
                        string keyword;
                        while (getline(ss, keyword, ',')) {
                            if (!keyword.empty()) {
                                book.addKeyword(keyword);
                            }
                        }
                    }
                    
                    library.addBook(book);
                    cout << "✓ کتاب با موفقیت اضافه شد!" << endl;
                    break;
                }
                case 2: {
                    string id;
                    cout << "شناسه کتاب: ";
                    cin >> id;
                    library.deleteBook(id);
                    cout << "✓ کتاب با موفقیت حذف شد!" << endl;
                    break;
                }
                case 3: {
                    string id, title, author, genre, isbn;
                    int year;
                    
                    cout << "شناسه کتاب: ";
                    cin >> id;
                    cin.ignore();
                    
                    cout << "عنوان جدید: ";
                    getline(cin, title);
                    
                    cout << "نویسنده جدید: ";
                    getline(cin, author);
                    
                    cout << "ژانر جدید: ";
                    getline(cin, genre);
                    
                    cout << "سال انتشار جدید: ";
                    cin >> year;
                    cin.ignore();
                    
                    cout << "ISBN جدید: ";
                    getline(cin, isbn);
                    
                    Book newBook(id, title, author, genre, year, isbn);
                    library.editBook(id, newBook);
                    cout << "✓ کتاب با موفقیت ویرایش شد!" << endl;
                    break;
                }
                case 4: {
                    string id, borrower;
                    cout << "شناسه کتاب: ";
                    cin >> id;
                    cin.ignore();
                    cout << "نام امانت گیرنده: ";
                    getline(cin, borrower);
                    library.borrowBook(id, borrower);
                    cout << "✓ کتاب با موفقیت امانت داده شد!" << endl;
                    break;
                }
                case 5: {
                    string id;
                    cout << "شناسه کتاب: ";
                    cin >> id;
                    library.returnBook(id);
                    cout << "✓ کتاب با موفقیت بازگردانده شد!" << endl;
                    break;
                }
                case 6: {
                    string id, borrower;
                    cout << "شناسه کتاب: ";
                    cin >> id;
                    cin.ignore();
                    cout << "نام رزرو کننده: ";
                    getline(cin, borrower);
                    library.reserveBook(id, borrower);
                    break;
                }
                case 7: {
                    library.undo();
                    break;
                }
                case 8: {
                    string id;
                    cout << "شناسه کتاب: ";
                    cin >> id;
                    Book* book = library.linearSearchById(id);
                    if (book) {
                        cout << "✓ کتاب یافت شد (Linear Search):" << endl;
                        book->display();
                    } else {
                        cout << "✗ کتاب یافت نشد!" << endl;
                    }
                    break;
                }
                case 9: {
                    string id;
                    cout << "شناسه کتاب: ";
                    cin >> id;
                    Book* book = library.binarySearchById(id);
                    if (book) {
                        cout << "✓ کتاب یافت شد (Binary Search):" << endl;
                        book->display();
                    } else {
                        cout << "✗ کتاب یافت نشد!" << endl;
                    }
                    break;
                }
                case 10: {
                    string title;
                    cin.ignore();
                    cout << "عنوان کتاب: ";
                    getline(cin, title);
                    Book* book = library.hashSearchByTitle(title);
                    if (book) {
                        cout << "✓ کتاب یافت شد (Hash Search):" << endl;
                        book->display();
                    } else {
                        cout << "✗ کتاب یافت نشد!" << endl;
                    }
                    break;
                }
                case 11: {
                    string author;
                    cin.ignore();
                    cout << "نام نویسنده: ";
                    getline(cin, author);
                    auto results = library.searchByAuthor(author);
                    if (results.empty()) {
                        cout << "✗ کتابی یافت نشد!" << endl;
                    } else {
                        cout << "✓ " << results.size() << " کتاب یافت شد:" << endl;
                        for (const auto& book : results) {
                            book.display();
                        }
                    }
                    break;
                }
                case 12: {
                    string genre;
                    cin.ignore();
                    cout << "ژانر: ";
                    getline(cin, genre);
                    auto results = library.searchByGenre(genre);
                    if (results.empty()) {
                        cout << "✗ کتابی یافت نشد!" << endl;
                    } else {
                        cout << "✓ " << results.size() << " کتاب یافت شد:" << endl;
                        for (const auto& book : results) {
                            book.display();
                        }
                    }
                    break;
                }
                case 13:
                    library.bubbleSortByTitle();
                    cout << "✓ کتاب‌ها بر اساس عنوان مرتب شدند (Bubble Sort)!" << endl;
                    break;
                case 14:
                    library.mergeSortByYear();
                    cout << "✓ کتاب‌ها بر اساس سال مرتب شدند (Merge Sort)!" << endl;
                    break;
                case 15:
                    library.quickSortByAuthor();
                    cout << "✓ کتاب‌ها بر اساس نویسنده مرتب شدند (Quick Sort)!" << endl;
                    break;
                case 16: {
                    string id;
                    cout << "شناسه کتاب: ";
                    cin >> id;
                    auto suggestions = library.suggestRelatedBooks(id);
                    if (suggestions.empty()) {
                        cout << "✗ کتاب مرتبطی یافت نشد!" << endl;
                    } else {
                        cout << "✓ کتاب‌های پیشنهادی:" << endl;
                        for (const auto& book : suggestions) {
                            book.display();
                        }
                    }
                    break;
                }
                case 17:
                    library.displayBookGraph();
                    break;
                case 18:
                    library.displayAllBooks();
                    break;
                case 19:
                    library.displayBorrowHistory();
                    break;
                case 20:
                    library.displayReservationQueue();
                    break;
                case 21:
                    library.displayUndoStack();
                    break;
                case 22:
                    library.displayStatistics();
                    break;
                case 0:
                    cout << "👋 خروح از برنامه..." << endl;
                    break;
                default:
                    cout << "✗ انتخاب نامعتبر!" << endl;
            }
        } catch (const exception& e) {
            cout << "⚠ خطا: " << e.what() << endl;
        }

    } while (choice != 0);

    return 0;
}