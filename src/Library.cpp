#include "../include/Library.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <iostream>

Library::Library(std::string dataFile, std::string historyFile) 
    : dataFile(dataFile), historyFile(historyFile) {
    loadFromFile();
    loadHistory();
    buildGraph();
}

Library::~Library() {
    saveToFile();
    saveHistory();
}

std::string Library::getCurrentDate() const {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::ostringstream oss;
    oss << 1900 + ltm->tm_year << "/" 
        << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "/"
        << std::setw(2) << std::setfill('0') << ltm->tm_mday;
    return oss.str();
}

void Library::updateDataStructures() {
    // بروزرسانی BST
    bookIndexBST.clear();
    for (size_t i = 0; i < booksVector.size(); i++) {
        bookIndexBST.insert(booksVector[i].getId(), i);
    }
    
    // بروزرسانی Hash Table
    titleHash.clear();
    for (size_t i = 0; i < booksVector.size(); i++) {
        std::string title = booksVector[i].getTitle();
        std::transform(title.begin(), title.end(), title.begin(), ::tolower);
        titleHash.insert(title, std::vector<int>{(int)i});
    }
}

void Library::buildGraph() {
    bookGraph = Graph();
    
    // اضافه کردن همه کتاب‌ها به گراف
    for (const auto& book : booksVector) {
        bookGraph.addVertex(book.getId());
    }
    
    // ایجاد یال بین کتاب‌های مرتبط (بر اساس ژانر یا نویسنده مشترک)
    for (size_t i = 0; i < booksVector.size(); i++) {
        for (size_t j = i + 1; j < booksVector.size(); j++) {
            double weight = 0;
            
            // ژانر مشترک
            if (booksVector[i].getGenre() == booksVector[j].getGenre()) {
                weight += 2.0;
            }
            
            // نویسنده مشترک
            if (booksVector[i].getAuthor() == booksVector[j].getAuthor()) {
                weight += 3.0;
            }
            
            // کلیدواژه مشترک
            auto kw1 = booksVector[i].getKeywords();
            auto kw2 = booksVector[j].getKeywords();
            for (const auto& k1 : kw1) {
                for (const auto& k2 : kw2) {
                    if (k1 == k2) {
                        weight += 1.0;
                    }
                }
            }
            
            if (weight > 0) {
                bookGraph.addEdge(booksVector[i].getId(), booksVector[j].getId(), weight);
            }
        }
    }
}

void Library::addBook(Book book) {
    // بررسی تکراری نبودن
    for (const auto& b : booksVector) {
        if (b.getId() == book.getId()) {
            throw std::runtime_error("کتاب با این شناسه قبلاً وجود دارد!");
        }
    }
    
    booksArray.push_back(book);
    booksVector.push_back(book);
    
    // ذخیره برای Undo
    UndoAction action;
    action.actionType = "ADD";
    action.bookId = book.getId();
    action.bookData = book;
    undoStack.push(action);
    
    updateDataStructures();
    buildGraph();
    saveToFile();
}

void Library::deleteBook(std::string bookId) {
    int index = -1;
    Book deletedBook;
    
    for (size_t i = 0; i < booksVector.size(); i++) {
        if (booksVector[i].getId() == bookId) {
            index = i;
            deletedBook = booksVector[i];
            break;
        }
    }
    
    if (index == -1) {
        throw std::runtime_error("کتاب یافت نشد!");
    }
    
    if (deletedBook.getIsBorrowed()) {
        throw std::runtime_error("نمی‌توان کتاب امانت داده شده را حذف کرد!");
    }
    
    booksVector.erase(booksVector.begin() + index);
    
    // بروزرسانی Array
    for (size_t i = 0; i < booksArray.size(); i++) {
        if (booksArray[i].getId() == bookId) {
            booksArray.erase(booksArray.begin() + i);
            break;
        }
    }
    
    // ذخیره برای Undo
    UndoAction action;
    action.actionType = "DELETE";
    action.bookId = bookId;
    action.bookData = deletedBook;
    undoStack.push(action);
    
    updateDataStructures();
    buildGraph();
    saveToFile();
}

void Library::editBook(std::string bookId, Book newBook) {
    int index = -1;
    Book oldBook;
    
    for (size_t i = 0; i < booksVector.size(); i++) {
        if (booksVector[i].getId() == bookId) {
            index = i;
            oldBook = booksVector[i];
            break;
        }
    }
    
    if (index == -1) {
        throw std::runtime_error("کتاب یافت نشد!");
    }
    
    booksVector[index] = newBook;
    
    // بروزرسانی Array
    for (size_t i = 0; i < booksArray.size(); i++) {
        if (booksArray[i].getId() == bookId) {
            booksArray[i] = newBook;
            break;
        }
    }
    
    // ذخیره برای Undo
    UndoAction action;
    action.actionType = "EDIT";
    action.bookId = bookId;
    action.bookData = oldBook;
    undoStack.push(action);
    
    updateDataStructures();
    buildGraph();
    saveToFile();
}

void Library::borrowBook(std::string bookId, std::string borrower) {
    int index = -1;
    for (size_t i = 0; i < booksVector.size(); i++) {
        if (booksVector[i].getId() == bookId) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        throw std::runtime_error("کتاب یافت نشد!");
    }
    
    Book oldBook = booksVector[index];
    booksVector[index].borrow(borrower);
    
    // بروزرسانی Array
    for (size_t i = 0; i < booksArray.size(); i++) {
        if (booksArray[i].getId() == bookId) {
            booksArray[i].borrow(borrower);
            break;
        }
    }
    
    // ثبت در تاریخچه (Linked List)
    BorrowRecord record;
    record.bookId = bookId;
    record.bookTitle = booksVector[index].getTitle();
    record.borrower = borrower;
    record.borrowDate = booksVector[index].getBorrowDate();
    record.returnDate = "";
    borrowHistory.pushFront(record);
    
    // ذخیره برای Undo
    UndoAction action;
    action.actionType = "BORROW";
    action.bookId = bookId;
    action.bookData = oldBook;
    action.extraInfo = borrower;
    undoStack.push(action);
    
    saveToFile();
    saveHistory();
}

void Library::returnBook(std::string bookId) {
    int index = -1;
    for (size_t i = 0; i < booksVector.size(); i++) {
        if (booksVector[i].getId() == bookId) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        throw std::runtime_error("کتاب یافت نشد!");
    }
    
    Book oldBook = booksVector[index];
    std::string borrower = booksVector[index].getBorrower();
    booksVector[index].returnBook();
    
    // بروزرسانی Array
    for (size_t i = 0; i < booksArray.size(); i++) {
        if (booksArray[i].getId() == bookId) {
            booksArray[i].returnBook();
            break;
        }
    }
    
    // بروزرسانی آخرین رکورد در تاریخچه
    auto historyVec = borrowHistory.toVector();
    for (auto& record : historyVec) {
        if (record.bookId == bookId && record.returnDate.empty()) {
            record.returnDate = getCurrentDate();
            break;
        }
    }
    
    // ذخیره برای Undo
    UndoAction action;
    action.actionType = "RETURN";
    action.bookId = bookId;
    action.bookData = oldBook;
    action.extraInfo = borrower;
    undoStack.push(action);
    
    // پردازش صف رزرو
    processReservations(bookId);
    
    saveToFile();
    saveHistory();
}

void Library::undo() {
    if (undoStack.isEmpty()) {
        throw std::runtime_error("هیچ عملیاتی برای بازگشت وجود ندارد!");
    }
    
    UndoAction action = undoStack.pop();
    
    if (action.actionType == "ADD") {
        // حذف کتابی که اضافه شده
        for (size_t i = 0; i < booksVector.size(); i++) {
            if (booksVector[i].getId() == action.bookId) {
                booksVector.erase(booksVector.begin() + i);
                break;
            }
        }
        for (size_t i = 0; i < booksArray.size(); i++) {
            if (booksArray[i].getId() == action.bookId) {
                booksArray.erase(booksArray.begin() + i);
                break;
            }
        }
    } else if (action.actionType == "DELETE") {
        // بازگرداندن کتاب حذف شده
        booksVector.push_back(action.bookData);
        booksArray.push_back(action.bookData);
    } else if (action.actionType == "EDIT") {
        // بازگرداندن اطلاعات قبلی
        for (size_t i = 0; i < booksVector.size(); i++) {
            if (booksVector[i].getId() == action.bookId) {
                booksVector[i] = action.bookData;
                break;
            }
        }
        for (size_t i = 0; i < booksArray.size(); i++) {
            if (booksArray[i].getId() == action.bookId) {
                booksArray[i] = action.bookData;
                break;
            }
        }
    } else if (action.actionType == "BORROW") {
        // بازگرداندن وضعیت امانت
        for (size_t i = 0; i < booksVector.size(); i++) {
            if (booksVector[i].getId() == action.bookId) {
                booksVector[i] = action.bookData;
                break;
            }
        }
        for (size_t i = 0; i < booksArray.size(); i++) {
            if (booksArray[i].getId() == action.bookId) {
                booksArray[i] = action.bookData;
                break;
            }
        }
    } else if (action.actionType == "RETURN") {
        // بازگرداندن وضعیت امانت
        for (size_t i = 0; i < booksVector.size(); i++) {
            if (booksVector[i].getId() == action.bookId) {
                booksVector[i] = action.bookData;
                break;
            }
        }
        for (size_t i = 0; i < booksArray.size(); i++) {
            if (booksArray[i].getId() == action.bookId) {
                booksArray[i] = action.bookData;
                break;
            }
        }
    }
    
    updateDataStructures();
    buildGraph();
    saveToFile();
    
    std::cout << "عملیات با موفقیت بازگردانده شد!" << std::endl;
}

void Library::reserveBook(std::string bookId, std::string borrower) {
    // بررسی وجود کتاب
    bool found = false;
    for (const auto& book : booksVector) {
        if (book.getId() == bookId) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        throw std::runtime_error("کتاب یافت نشد!");
    }
    
    Reservation res;
    res.bookId = bookId;
    res.borrower = borrower;
    res.reservationDate = getCurrentDate();
    
    reservationQueue.enqueue(res);
    std::cout << "رزرو با موفقیت ثبت شد. شماره صف: " << reservationQueue.getSize() << std::endl;
}

void Library::processReservations(std::string bookId) {
    // اگر کتابی بازگردانده شد، به نفر اول صف بده
    // (این یک پیاده‌سازی ساده است - در حالت واقعی باید صف مخصوص هر کتاب داشته باشیم)
    if (!reservationQueue.isEmpty()) {
        Reservation front = reservationQueue.peek();
        if (front.bookId == bookId) {
            reservationQueue.dequeue();
            std::cout << "کتاب " << bookId << " به " << front.borrower 
                      << " (از صف رزرو) امانت داده شد." << std::endl;
            borrowBook(bookId, front.borrower);
        }
    }
}

// جستجوی خطی (Linear Search) روی Array
Book* Library::linearSearchById(std::string id) {
    for (size_t i = 0; i < booksArray.size(); i++) {
        if (booksArray[i].getId() == id) {
            return &booksArray[i];
        }
    }
    return nullptr;
}

// جستجوی دودویی (Binary Search) - نیاز به مرتب‌سازی
Book* Library::binarySearchById(std::string id) {
    // اول بر اساس ID مرتب می‌کنیم
    std::vector<Book> sorted = booksVector;
    std::sort(sorted.begin(), sorted.end(), 
        [](const Book& a, const Book& b) { return a.getId() < b.getId(); });
    
    int left = 0, right = sorted.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (sorted[mid].getId() == id) {
            // پیدا کردن در booksVector
            for (size_t i = 0; i < booksVector.size(); i++) {
                if (booksVector[i].getId() == id) {
                    return &booksVector[i];
                }
            }
        }
        if (sorted[mid].getId() < id) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return nullptr;
}

// جستجوی سریع با Hash Table
Book* Library::hashSearchByTitle(std::string title) {
    std::string lowerTitle = title;
    std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
    
    std::vector<int>* indices = titleHash.search(lowerTitle);
    if (indices && !indices->empty()) {
        int idx = (*indices)[0];
        if (idx < (int)booksVector.size()) {
            return &booksVector[idx];
        }
    }
    
    // جستجوی جزئی
    for (auto& book : booksVector) {
        std::string bookTitle = book.getTitle();
        std::transform(bookTitle.begin(), bookTitle.end(), bookTitle.begin(), ::tolower);
        if (bookTitle.find(lowerTitle) != std::string::npos) {
            return &book;
        }
    }
    
    return nullptr;
}

std::vector<Book> Library::searchByAuthor(std::string author) {
    std::vector<Book> results;
    std::string lowerAuthor = author;
    std::transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);
    
    for (const auto& book : booksVector) {
        std::string bookAuthor = book.getAuthor();
        std::transform(bookAuthor.begin(), bookAuthor.end(), bookAuthor.begin(), ::tolower);
        if (bookAuthor.find(lowerAuthor) != std::string::npos) {
            results.push_back(book);
        }
    }
    return results;
}

std::vector<Book> Library::searchByGenre(std::string genre) {
    std::vector<Book> results;
    std::string lowerGenre = genre;
    std::transform(lowerGenre.begin(), lowerGenre.end(), lowerGenre.begin(), ::tolower);
    
    for (const auto& book : booksVector) {
        std::string bookGenre = book.getGenre();
        std::transform(bookGenre.begin(), bookGenre.end(), bookGenre.begin(), ::tolower);
        if (bookGenre.find(lowerGenre) != std::string::npos) {
            results.push_back(book);
        }
    }
    return results;
}

// Bubble Sort بر اساس عنوان
void Library::bubbleSortByTitle() {
    int n = booksVector.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (booksVector[j].getTitle() > booksVector[j + 1].getTitle()) {
                std::swap(booksVector[j], booksVector[j + 1]);
            }
        }
    }
    updateDataStructures();
    saveToFile();
}

// Merge Sort بر اساس سال
void Library::mergeSortByYear() {
    std::function<void(std::vector<Book>&, int, int)> mergeSort;
    std::function<void(std::vector<Book>&, int, int, int)> merge;
    
    merge = [&merge](std::vector<Book>& arr, int left, int mid, int right) {
        std::vector<Book> temp;
        int i = left, j = mid + 1;
        
        while (i <= mid && j <= right) {
            if (arr[i].getYear() <= arr[j].getYear()) {
                temp.push_back(arr[i++]);
            } else {
                temp.push_back(arr[j++]);
            }
        }
        
        while (i <= mid) temp.push_back(arr[i++]);
        while (j <= right) temp.push_back(arr[j++]);
        
        for (int k = 0; k < (int)temp.size(); k++) {
            arr[left + k] = temp[k];
        }
    };
    
    mergeSort = [&mergeSort, &merge](std::vector<Book>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    };
    
    if (!booksVector.empty()) {
        mergeSort(booksVector, 0, booksVector.size() - 1);
        updateDataStructures();
        saveToFile();
    }
}

// Quick Sort بر اساس نویسنده
void Library::quickSortByAuthor() {
    std::function<void(std::vector<Book>&, int, int)> quickSort;
    std::function<int(std::vector<Book>&, int, int)> partition;
    
    partition = [&partition, &quickSort](std::vector<Book>& arr, int low, int high) {
        std::string pivot = arr[high].getAuthor();
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (arr[j].getAuthor() <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    };
    
    quickSort = [&quickSort, &partition](std::vector<Book>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    };
    
    if (!booksVector.empty()) {
        quickSort(booksVector, 0, booksVector.size() - 1);
        updateDataStructures();
        saveToFile();
    }
}

void Library::displayAllBooks() const {
    if (booksVector.empty()) {
        std::cout << "هیچ کتابی در کتابخانه وجود ندارد." << std::endl;
        return;
    }
    
    std::cout << "\n=== لیست همه کتاب‌ها (" << booksVector.size() << " کتاب) ===" << std::endl;
    for (const auto& book : booksVector) {
        book.display();
    }
}

void Library::displayBorrowHistory() const {
    std::cout << "\n=== تاریخچه امانت (Linked List) ===" << std::endl;
    if (borrowHistory.isEmpty()) {
        std::cout << "هیچ رکوردی وجود ندارد." << std::endl;
        return;
    }
    borrowHistory.display();
}

void Library::displayReservationQueue() const {
    std::cout << "\n=== صف رزرو کتاب (Queue) ===" << std::endl;
    if (reservationQueue.isEmpty()) {
        std::cout << "صف خالی است." << std::endl;
        return;
    }
    reservationQueue.display();
}

void Library::displayUndoStack() const {
    std::cout << "\n=== پشته Undo (Stack) ===" << std::endl;
    if (undoStack.isEmpty()) {
        std::cout << "پشته خالی است." << std::endl;
        return;
    }
    undoStack.display();
}

std::vector<Book> Library::suggestRelatedBooks(std::string bookId) {
    std::vector<Book> suggestions;
    std::vector<std::string> relatedIds = bookGraph.getRelatedBooks(bookId, 5);
    
    for (const auto& id : relatedIds) {
        for (const auto& book : booksVector) {
            if (book.getId() == id) {
                suggestions.push_back(book);
                break;
            }
        }
    }
    
    return suggestions;
}

void Library::displayBookGraph() const {
    std::cout << "\n=== گراف کتاب‌ها (Graph) ===" << std::endl;
    std::cout << "تعداد راس‌ها: " << bookGraph.getVertexCount() << std::endl;
    std::cout << "تعداد یال‌ها: " << bookGraph.getEdgeCount() << std::endl;
    std::cout << "\nلیست همسایگی:" << std::endl;
    bookGraph.display();
}

void Library::displayStatistics() const {
    std::cout << "\n=== آمار کتابخانه ===" << std::endl;
    std::cout << "تعداد کل کتاب‌ها: " << booksVector.size() << std::endl;
    
    int borrowed = 0;
    for (const auto& book : booksVector) {
        if (book.getIsBorrowed()) borrowed++;
    }
    
    std::cout << "کتاب‌های امانت داده شده: " << borrowed << std::endl;
    std::cout << "کتاب‌های موجود: " << (booksVector.size() - borrowed) << std::endl;
    std::cout << "تعداد رزروها: " << reservationQueue.getSize() << std::endl;
    std::cout << "تعداد عملیات Undo: " << undoStack.getSize() << std::endl;
    std::cout << "تعداد رکوردهای تاریخچه: " << borrowHistory.getSize() << std::endl;
    std::cout << "ظرفیت Hash Table: " << titleHash.getCapacity() << std::endl;
    std::cout << "Load Factor: " << titleHash.getLoadFactor() << std::endl;
}

void Library::saveToFile() const {
    std::ofstream file(dataFile);
    if (!file.is_open()) {
        throw std::runtime_error("خطا در باز کردن فایل!");
    }
    
    for (const auto& book : booksVector) {
        file << book;
    }
    
    file.close();
}

void Library::loadFromFile() {
    std::ifstream file(dataFile);
    if (!file.is_open()) {
        return;
    }
    
    booksVector.clear();
    booksArray.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Book book;
            std::istringstream iss(line);
            std::string token;
            
            std::getline(iss, book.id, '|');
            std::getline(iss, book.title, '|');
            std::getline(iss, book.author, '|');
            std::getline(iss, book.genre, '|');
            
            std::getline(iss, token, '|');
            book.year = std::stoi(token);
            
            std::getline(iss, book.isbn, '|');
            
            std::getline(iss, token, '|');
            book.isBorrowed = (token == "1");
            
            std::getline(iss, book.borrower, '|');
            std::getline(iss, book.borrowDate, '|');
            
            std::string keywordsStr;
            std::getline(iss, keywordsStr);
            
            book.keywords.clear();
            std::istringstream kwStream(keywordsStr);
            std::string keyword;
            while (std::getline(kwStream, keyword, ',')) {
                if (!keyword.empty()) {
                    book.keywords.push_back(keyword);
                }
            }
            
            booksVector.push_back(book);
            booksArray.push_back(book);
        }
    }
    
    file.close();
    updateDataStructures();
}

void Library::saveHistory() const {
    std::ofstream file(historyFile);
    if (!file.is_open()) return;
    
    auto records = borrowHistory.toVector();
    for (const auto& record : records) {
        file << record.bookId << "|" << record.bookTitle << "|" 
             << record.borrower << "|" << record.borrowDate << "|" 
             << record.returnDate << "\n";
    }
    
    file.close();
}

void Library::loadHistory() {
    std::ifstream file(historyFile);
    if (!file.is_open()) return;
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            BorrowRecord record;
            std::istringstream iss(line);
            std::getline(iss, record.bookId, '|');
            std::getline(iss, record.bookTitle, '|');
            std::getline(iss, record.borrower, '|');
            std::getline(iss, record.borrowDate, '|');
            std::getline(iss, record.returnDate);
            borrowHistory.pushBack(record);
        }
    }
    
    file.close();
}