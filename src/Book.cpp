#include "../include/Book.h"
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>

Book::Book() : year(0), isBorrowed(false) {}

Book::Book(std::string id, std::string title, std::string author, 
           std::string genre, int year, std::string isbn)
    : id(id), title(title), author(author), genre(genre), 
      year(year), isbn(isbn), isBorrowed(false) {}

std::string Book::getId() const { return id; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getGenre() const { return genre; }
int Book::getYear() const { return year; }
std::string Book::getIsbn() const { return isbn; }
std::vector<std::string> Book::getKeywords() const { return keywords; }
bool Book::getIsBorrowed() const { return isBorrowed; }
std::string Book::getBorrower() const { return borrower; }
std::string Book::getBorrowDate() const { return borrowDate; }

void Book::setTitle(std::string title) { this->title = title; }
void Book::setAuthor(std::string author) { this->author = author; }
void Book::setGenre(std::string genre) { this->genre = genre; }
void Book::setYear(int year) { this->year = year; }
void Book::setIsbn(std::string isbn) { this->isbn = isbn; }
void Book::setKeywords(std::vector<std::string> keywords) { this->keywords = keywords; }

void Book::addKeyword(std::string keyword) {
    keywords.push_back(keyword);
}

void Book::setBorrower(std::string borrower) { this->borrower = borrower; }
void Book::setBorrowDate(std::string date) { this->borrowDate = date; }
void Book::setIsBorrowed(bool status) { this->isBorrowed = status; }

void Book::borrow(std::string borrowerName) {
    if (isBorrowed) {
        throw std::runtime_error("این کتاب قبلاً امانت داده شده است!");
    }
    isBorrowed = true;
    borrower = borrowerName;
    
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::ostringstream oss;
    oss << 1900 + ltm->tm_year << "/" 
        << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "/"
        << std::setw(2) << std::setfill('0') << ltm->tm_mday;
    borrowDate = oss.str();
}

void Book::returnBook() {
    if (!isBorrowed) {
        throw std::runtime_error("این کتاب امانت داده نشده است!");
    }
    isBorrowed = false;
    borrower = "";
    borrowDate = "";
}

void Book::display() const {
    std::cout << "┌─────────────────────────────────────┐" << std::endl;
    std::cout << "│ شناسه: " << std::left << std::setw(32) << id << "│" << std::endl;
    std::cout << "│ عنوان: " << std::left << std::setw(32) << title << "│" << std::endl;
    std::cout << "│ نویسنده: " << std::left << std::setw(30) << author << "│" << std::endl;
    std::cout << "│ ژانر: " << std::left << std::setw(32) << genre << "│" << std::endl;
    std::cout << "│ سال: " << std::left << std::setw(33) << year << "│" << std::endl;
    std::cout << "│ ISBN: " << std::left << std::setw(32) << isbn << "│" << std::endl;
    std::cout << "│ وضعیت: " << std::left << std::setw(30) 
              << (isBorrowed ? "امانت داده شده" : "موجود") << "│" << std::endl;
    if (isBorrowed) {
        std::cout << "│ امانت گیرنده: " << std::left << std::setw(24) << borrower << "│" << std::endl;
        std::cout << "│ تاریخ امانت: " << std::left << std::setw(25) << borrowDate << "│" << std::endl;
    }
    std::cout << "└─────────────────────────────────────┘" << std::endl;
}

bool Book::operator<(const Book& other) const {
    return title < other.title;
}

bool Book::operator>(const Book& other) const {
    return title > other.title;
}

bool Book::operator==(const Book& other) const {
    return id == other.id;
}

std::ofstream& operator<<(std::ofstream& out, const Book& book) {
    out << book.id << "|" << book.title << "|" << book.author << "|"
        << book.genre << "|" << book.year << "|" << book.isbn << "|"
        << book.isBorrowed << "|" << book.borrower << "|" << book.borrowDate << "|";
    
    for (size_t i = 0; i < book.keywords.size(); i++) {
        out << book.keywords[i];
        if (i < book.keywords.size() - 1) out << ",";
    }
    out << "\n";
    return out;
}

std::ifstream& operator>>(std::ifstream& in, Book& book) {
    std::string line;
    std::getline(in, line);
    
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
    
    return in;
}