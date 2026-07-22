#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <string>
#include <iostream>

// Node برای لیست پیوندی
template<typename T>
struct Node {
    T data;
    Node* next;
    Node(T data) : data(data), next(nullptr) {}
};

// لیست پیوندی یک‌طرفه
template<typename T>
class LinkedList {
private:
    Node<T>* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() {
        clear();
    }

    // اضافه کردن به ابتدا
    void pushFront(T data) {
        Node<T>* newNode = new Node<T>(data);
        newNode->next = head;
        head = newNode;
        size++;
    }

    // اضافه کردن به انتها
    void pushBack(T data) {
        Node<T>* newNode = new Node<T>(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    // حذف از ابتدا
    T popFront() {
        if (head == nullptr) {
            throw std::runtime_error("لیست خالی است!");
        }
        Node<T>* temp = head;
        T data = head->data;
        head = head->next;
        delete temp;
        size--;
        return data;
    }

    // جستجو
    bool search(T data) const {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == data) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Linear Search
    Node<T>* linearSearch(T data) const {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == data) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // نمایش
    void display() const {
        Node<T>* current = head;
        while (current != nullptr) {
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << "nullptr" << std::endl;
    }

    // تبدیل به vector
    std::vector<T> toVector() const {
        std::vector<T> result;
        Node<T>* current = head;
        while (current != nullptr) {
            result.push_back(current->data);
            current = current->next;
        }
        return result;
    }

    int getSize() const { return size; }
    bool isEmpty() const { return head == nullptr; }

    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }
};

#endif // LINKED_LIST_H