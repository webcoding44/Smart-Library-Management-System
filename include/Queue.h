#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept>

template<typename T>
class Queue {
private:
    struct QueueNode {
        T data;
        QueueNode* next;
        QueueNode(T data) : data(data), next(nullptr) {}
    };
    
    QueueNode* front;
    QueueNode* rear;
    int size;

public:
    Queue() : front(nullptr), rear(nullptr), size(0) {}

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(T data) {
        QueueNode* newNode = new QueueNode(data);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue خالی است!");
        }
        QueueNode* temp = front;
        T data = front->data;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        size--;
        return data;
    }

    T peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue خالی است!");
        }
        return front->data;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    int getSize() const {
        return size;
    }

    void display() const {
        QueueNode* current = front;
        std::cout << "Queue (Front -> Rear): ";
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif // QUEUE_H