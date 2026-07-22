#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include <functional>
#include <iostream>

template<typename K, typename V>
class HashTable {
private:
    struct HashNode {
        K key;
        V value;
        HashNode* next;
        
        HashNode(K key, V value) : key(key), value(value), next(nullptr) {}
    };
    
    std::vector<HashNode*> table;
    int capacity;
    int size;

    // Hash function
    size_t hash(K key) const {
        std::hash<K> hasher;
        return hasher(key) % capacity;
    }

public:
    HashTable(int capacity = 101) : capacity(capacity), size(0) {
        table.resize(capacity, nullptr);
    }

    ~HashTable() {
        clear();
    }

    void insert(K key, V value) {
        size_t index = hash(key);
        
        // Check if key exists
        HashNode* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        // Insert new node
        HashNode* newNode = new HashNode(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
    }

    V* search(K key) const {
        size_t index = hash(key);
        HashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        
        return nullptr;
    }

    bool contains(K key) const {
        return search(key) != nullptr;
    }

    bool remove(K key) {
        size_t index = hash(key);
        HashNode* current = table[index];
        HashNode* prev = nullptr;
        
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                size--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        
        return false;
    }

    std::vector<std::pair<K, V>> getAll() const {
        std::vector<std::pair<K, V>> result;
        for (int i = 0; i < capacity; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                result.push_back({current->key, current->value});
                current = current->next;
            }
        }
        return result;
    }

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    bool isEmpty() const { return size == 0; }

    double getLoadFactor() const {
        return (double)size / capacity;
    }

    void clear() {
        for (int i = 0; i < capacity; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        size = 0;
    }

    void display() const {
        for (int i = 0; i < capacity; i++) {
            if (table[i] != nullptr) {
                std::cout << "Bucket " << i << ": ";
                HashNode* current = table[i];
                while (current != nullptr) {
                    std::cout << "[" << current->key << "] ";
                    current = current->next;
                }
                std::cout << std::endl;
            }
        }
    }
};

#endif // HASH_TABLE_H