#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>

template<typename T>
class Stack {
private:
    struct StackNode {
        T data;
        StackNode* next;
        StackNode(T data) : data(data), next(nullptr) {}
    };
    
    StackNode* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(T data) {
        StackNode* newNode = new StackNode(data);
        newNode->next = top;
        top = newNode;
        size++;
    }

    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack خالی است!");
        }
        StackNode* temp = top;
        T data = top->data;
        top = top->next;
        delete temp;
        size--;
        return data;
    }

    T peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack خالی است!");
        }
        return top->data;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getSize() const {
        return size;
    }

    void display() const {
        StackNode* current = top;
        std::cout << "Stack (Top -> Bottom): ";
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif // STACK_H