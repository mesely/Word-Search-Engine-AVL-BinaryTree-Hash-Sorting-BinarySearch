/*
 * Selman Yilmaz CS300 Homework 3
* This C++ code defines a generic HashTable class that implements a basic hash table using open
* addressing and quadratic probing for collision resolution.
*
* The HashTable class supports key-value pairs, where the key and value can be of any data type. It provides functionalities for inserting, finding, and removing elements.
* Additionally, the hash table automatically adjusts its size (rehashes) when the load factor exceeds a certain threshold.
*
* The HashTable uses a struct called HashEntry to represent each element in the table. The EntryType enumeration defines different states an entry can have (ACTIVE, EMPTY, or DELETED).
*
* The class includes functions for finding the next prime number, calculating hash values for strings, rehashing the table, and determining the position of a key using quadratic probing.
*
*/
#include <vector>
#include <iostream>
#include <cmath>

using namespace  std;

// Template class for a Hash Table
template <class KeyType, class ValueType>
class HashTable {
    enum EntryType { ACTIVE, EMPTY, DELETED };  // Enumeration for different entry states

public:
    // Struct to represent each entry in the hash table
    struct HashEntry {
        KeyType key;
        ValueType value;
        EntryType info;

        // Constructor for HashEntry
        HashEntry(const KeyType &k = KeyType(), const ValueType &v = ValueType(), EntryType i = EMPTY)
                : key(k), value(v), info(i) {}
    };

    vector<HashEntry> array;  // The actual array to store entries
    int currentSize;               // Current number of elements in the hash table
    const KeyType ITEM_NOT_FOUND;  // Constant representing an item not found in the hash table

    // Function to find the next prime number
    int nextPrime(int n) {
        if (n <= 2)
            return 2;

        int next = n % 2 == 0 ? n + 1 : n + 2;  // Ensure next is odd
        bool isPrime = false;

        // Loop to find the next prime number
        while (!isPrime) {
            isPrime = true;
            for (int i = 3; i * i <= next; i += 2) {
                if (next % i == 0) {
                    isPrime = false;
                    break;
                }
            }

            if (!isPrime)
                next += 2;
        }

        return next;
    }

    // Copy constructor
    HashTable(const HashTable &rhs)
            : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
              array(rhs.array), currentSize(rhs.currentSize) {}

    // Constructor with parameters
    HashTable(const KeyType &notFound, int size) : ITEM_NOT_FOUND(notFound), array(nextPrime(size)) {
        makeEmpty();
    }

    // Function to find a key in the hash table
    const KeyType &find(const KeyType &x) {
        int currentPos = findPos(x);
        if (isActive(currentPos))
            return array[currentPos].key;

        return ITEM_NOT_FOUND;
    }

    // Function to make the hash table empty
    void makeEmpty() {
        currentSize = 0;
        for (int i = 0; i < array.size(); ++i) {
            array[i].info = EMPTY;
        }
    }

    // Function to insert a key-value pair into the hash table
    void insert(const KeyType &x, const ValueType &value) {
        // Insert x as active
        int currentPos = findPos(x);
        if (isActive(currentPos))
            return;
        array[currentPos] = HashEntry(x, value, ACTIVE);
        currentSize++;

        // Calculate load factor
        double loadFactor = static_cast<double>(currentSize) / array.size();

        // Enlarge the hash table if necessary
        if (loadFactor >= 0.7) {
            rehash();
        }
    }

    // Function to remove a key from the hash table
    void remove(const KeyType &x) {
        int currentPos = findPos(x);
        if (isActive(currentPos))
            array[currentPos].info = DELETED;
    }

    // Hash function for strings
    int hash(const string &str, int m) {
        int p = 31;
        int hashValue = 0;
        int p_pow = 1;
        const int len = str.length();

        // Calculate the hash value
        for (int i = 0; i < len; ++i) {
            hashValue = (hashValue + (str[i] - 'a' + 1) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }

        // Include the string length in the hash
        hashValue = (hashValue * 31 + len) % m;

        return hashValue;
    }

    // Function to rehash the hash table when load factor exceeds 0.7
    void rehash() {
        vector<HashEntry> oldArray = array;

        // Create new double-sized, empty table
        array.resize(nextPrime(2 * oldArray.size()));
        for (int j = 0; j < array.size(); j++)
            array[j].info = EMPTY;

        // Copy table over
        currentSize = 0;
        for (int i = 0; i < oldArray.size(); i++)
            if (oldArray[i].info == ACTIVE)
                insert(oldArray[i].key, oldArray[i].value);  // Use insert to update currentSize

        double loadFactor = static_cast<double>(currentSize) / array.size();
        cout << "rehashed..." << endl
          << "previous table size: " << oldArray.size() << " new table size: " << array.size() << ", current unique word count " << currentSize << endl
          << "current load factor: " << loadFactor << endl;
    }

    // Function to find the position of a key in the hash table
    int findPos(const KeyType &x) {
        int collisionNum = 0;
        int currentPos = hash(x, array.size());

        // Quadratic probing to handle collisions
        while (array[currentPos].info != EMPTY && array[currentPos].key != x) {
            currentPos += pow(++collisionNum, 2);  // Add the difference
            if (currentPos >= array.size())         // Perform the mod
                currentPos -= array.size();         // If necessary
        }
        return currentPos;
    }

private:
    // Function to check if a position is active
    bool isActive(int currentPos) const {
        return array[currentPos].info == ACTIVE;
    }
};
