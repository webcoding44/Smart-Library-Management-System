# 📚 Smart Library Management System

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey?style=for-the-badge)
![Status](https://img.shields.io/badge/status-stable-success?style=for-the-badge)
![Level](https://img.shields.io/badge/level-Advanced-red?style=for-the-badge)
![Data Structures](https://img.shields.io/badge/Data%20Structures-8-orange?style=for-the-badge)
![Algorithms](https://img.shields.io/badge/Algorithms-7-purple?style=for-the-badge)

**A comprehensive Library Management System implementing 8 Data Structures & 7 Algorithms from scratch in C++**

[Features](#-features) • [Data Structures](#-data-structures) • [Algorithms](#-algorithms) • [Installation](#-installation) • [Usage](#-usage) • [Architecture](#-architecture)

</div>

---

## 📖 Overview

The **Smart Library Management System** is an advanced C++ project that demonstrates mastery of fundamental data structures and algorithms by implementing them **from scratch** (without using STL containers for the core structures). This project manages a library with features like book borrowing, reservations, undo operations, and intelligent book recommendations using graph algorithms.

>  **توجه:** این پروژه یک تمرین آموزشی پیشرفته برای یادگیری و پیاده‌سازی **ساختمان داده‌ها و الگوریتم‌ها** از صفر در زبان ++C است.

### 🎯 Project Goals

- ✅ Implement 8 different data structures from scratch
- ✅ Implement 7 different sorting and searching algorithms
- ✅ Apply data structures in real-world scenarios
- ✅ Demonstrate advanced OOP concepts
- ✅ Handle complex operations like Undo and Recommendations

---

## ✨ Features

### 📚 Core Library Operations
- ➕ **Add** new books with metadata (title, author, genre, year, ISBN, keywords)
- ✏️ **Edit** existing book information
- 🗑️ **Delete** books (with validation)
- 📤 **Borrow** books with borrower tracking
- 📥 **Return** borrowed books
- 🔙 **Undo** last operation (Stack-based)
- 🎫 **Reserve** books when unavailable (Queue-based)

### 🔍 Advanced Search Operations
- 🔎 **Linear Search** - Sequential search on Array
- 🔍 **Binary Search** - O(log n) search on sorted data
- ⚡ **Hash Search** - O(1) average search using Hash Table
- 👤 **Author Search** - Partial matching with case-insensitive search
- 📖 **Genre Search** - Filter books by category

### 📊 Sorting Operations
- 🫧 **Bubble Sort** - Sort by title
- 🔀 **Merge Sort** - Sort by publication year
- ⚡ **Quick Sort** - Sort by author name

### 🎯 Smart Features
- 🤖 **Book Recommendations** - Graph-based related book suggestions
- 📜 **Borrow History** - Complete tracking using Linked List
- 🔄 **Undo System** - Revert any operation using Stack
- 🎫 **Reservation Queue** - Fair queue system for popular books
- 📈 **Statistics** - Comprehensive library analytics

---

## 🏗️ Data Structures

This project implements **8 different data structures** from scratch:

| # | Data Structure | Implementation | Use Case | Time Complexity |
|---|----------------|----------------|----------|-----------------|
| 1 | **Array** | Built-in C++ Array | Basic book storage | O(n) access |
| 2 | **Vector** | `std::vector` | Main book collection | O(1) access, O(n) insert |
| 3 | **Linked List** | Custom Template | Borrow history tracking | O(n) access, O(1) insert |
| 4 | **Stack** | Custom Template (LIFO) | Undo operations | O(1) push/pop |
| 5 | **Queue** | Custom Template (FIFO) | Book reservation system | O(1) enqueue/dequeue |
| 6 | **Binary Search Tree** | Custom Template | Book indexing by ID | O(log n) search |
| 7 | **Hash Table** | Custom with Chaining | Fast title search | O(1) average search |
| 8 | **Graph** | Adjacency List | Book relationships | O(V+E) traversal |

### 📊 Data Structure Details

#### 1️⃣ Array
```cpp
std::vector<Book> booksArray;  // Used for Linear Search demonstration