/*
 * Selman Yilmaz CS300 Homework 3
 * This C++ program functions similarly to a search engine.
 * Suppose you have several text files and you wish to quickly determine
 * the frequency with which a given word appears in each file.
 * You can do it with this code to BinarySearch Tree and Hash Table Implementation,
 *
 * This is how it works:
 * First, it asks how many text files you want to process and what their names are. Simple as that.
 * Then it goes through each file, noting how frequently each word appears in different documents.
 * The program will then find and display the occurrences of specific words in your files.
 *
 * It doesn't care whether you type words in upper or lower case; it's cool like that.
 * Finally, It compares speed of BST and Hash Table. It tells you information about that which one is faster.
 *
 * Guide:
 * When prompted, enter the number and name of your text files.
 * Enter the words you're interested in.
 * Then, you can see frequency results and speed of BST and Hash Table
 */

#include "tree.h"  // tree.h contains the BinarySearch tree implementation
#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>

#include "hashTable.h" // hashTable.h contains the Hash Table implementation (quadratic probing)

using namespace std;

// Structure to store information about each document
struct DocumentItem {
    string documentName;
    int count;
};

// Structure to represent each word and its occurrences in documents
struct WordItem {
    string word;
    tree<string, DocumentItem> docItemTree;
};

// Structure to store the result of word occurrences in documents
// E.g: (documentName: a.txt ; resultVector[0].documentName = aberration ; resultVector.[0].count = 12)
struct resultItem {
    string documentName;
    vector<DocumentItem> resultVector; // Vector to store DocumentItem for each document
};

// Function to get input file names from the user
vector<string> getFileNames() {
    int fileNumber;
    string fileName;
    vector<string> fileNames;

    // Prompt the user to enter the number of input files
    cout << "Enter the number of input files: ";
    cin >> fileNumber;

    // Get the names of input files from the user
    for (int i = 1; i <= fileNumber; i++) {
        cout << "Enter the name of file " << i << ": ";
        cin >> fileName;
        fileNames.push_back(fileName);
    }

    return fileNames;
}

// Function to lower the first letter of a given string
string lower(const string& str) {
    string result;
    char c;

    // Lower the first letter of the input string
    for (int i = 0; i < str.length(); i++) {
        c = str.at(i);
        c = tolower(c);
        result += c;
    }

    return result;
}

// Function to check if a document is already in the result vector
template <typename T>
int isInVector(vector<T> vec, string value) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i].documentName == value) {
            return i;
        }
    }
    return -1;
}

// Function to clean the input line, replacing non-alphabetic characters with spaces
string cleanAndAddSpace(const string& line) {
    string cleanedLine;

    for (int i = 0; i < line.length(); ++i) {
        char c = line[i];

        if (!isalpha(c)) {
            cleanedLine += ' ';  // Replace non-alphabetic characters with space
        } else {
            cleanedLine += c;    // Keep alphabetic characters as they are
        }
    }

    return cleanedLine;
}

// Function to read a text file and build a tree structure
void TextFiletoTree(const string& filename, tree<string, WordItem>& Tree) {
    ifstream file(filename);
    string line, word;
    WordItem wrdItem;
    string cleanedLine;

    treeNode<string, WordItem>* current;
    treeNode<string, DocumentItem> *docCurrent;

    DocumentItem docItem;

    // Read each line of the file
    while (getline(file, line)) {
        cleanedLine = cleanAndAddSpace(line);
        istringstream iss(cleanedLine);

        // For each word in the line
        while (iss >> word) {

            string lowerWord = lower(word);
            current = Tree.find(lowerWord, Tree.getRoot());

            if (current == nullptr) {
                // If the word is not found in the tree, create a new entry
                tree<string, DocumentItem> ItemTree;
                docItem.documentName = filename;
                docItem.count = 1;
                ItemTree.insert(filename, docItem);
                wrdItem.word = word;
                wrdItem.docItemTree = ItemTree;
                Tree.insert(lower(word), wrdItem);
            } else {
                // If the word is found, update its occurrences in the document
                docCurrent = (current->value.docItemTree.find(filename, current->value.docItemTree.getRoot()));

                if (docCurrent == nullptr) {
                    // If the document is not found, create a new entry
                    docItem.documentName = filename;
                    docItem.count = 1;
                    current->value.docItemTree.insert(filename, docItem);
                } else {
                    // If the document is found, increment the count
                    docCurrent->value.count++;
                }
            }

        }
    }

    file.close();  // Close the file
}

// Helper function to build the search result tree
void buildResultTree(treeNode<string, DocumentItem>* t, string queriedWord, tree<string, vector<DocumentItem>>& searchResultTree, vector<DocumentItem>& Words) {
    treeNode<string, vector<DocumentItem>>* temp;

    if (t == nullptr)
        return;

    buildResultTree(t->left, queriedWord, searchResultTree, Words);

    DocumentItem docItem;
    docItem.documentName = queriedWord;
    docItem.count = t->value.count;
    Words.push_back(docItem);
    temp = searchResultTree.find(t->key, searchResultTree.getRoot());

    if (temp == nullptr)
        // If the word is not found in the search result tree, create a new entry
        searchResultTree.insert(t->key, Words);
    else
        // If the word is found, update its occurrences in the search result tree
        temp->value.push_back(docItem);

    while (Words.size() != 0)
        Words.pop_back();

    buildResultTree(t->right, queriedWord, searchResultTree, Words);
}

// Function to print the results of the search
void printResultTree(treeNode<string, vector<DocumentItem>>* t,int quantityWords) {
    if (t == nullptr) {
        return;
    }
    printResultTree(t->left,quantityWords);

    // Print the details of the search result for each document
    if(quantityWords == t->value.size())
    {
        cout << "In Document " << t->key << ", ";

        for (int i = 0; i < t->value.size(); ++i) {
            cout << lower(t->value[i].documentName) << " found " << t->value[i].count << " times";
            if (i == t->value.size() - 1) {
                cout << "." << endl;
            } else
                cout << ", ";
        }
    }

    printResultTree(t->right,quantityWords);
}

// Function to perform a search for a queried word
void searchEngineTree(const string& queriedWord, tree<string, WordItem>& Tree, tree<string, vector<DocumentItem>>& searchResultTree) {
    vector<DocumentItem> Words;
    treeNode<string, WordItem>* current = Tree.find(lower(queriedWord), Tree.getRoot());

    if (current != nullptr) {
        // Build the search result tree based on the occurrences of the queried word
        buildResultTree(current->value.docItemTree.getRoot(), queriedWord, searchResultTree, Words);
    }
    else
    {
        if(searchResultTree.getRoot() != nullptr)
            searchResultTree.makeEmpty();
        return;
    }
}

// Function to search for a queried word in the Hash Table and update the result vector
void searchEngineHash(const string& queriedWord, HashTable<string, vector<DocumentItem>>& hashTable, vector<resultItem>& hashResultVector) {
    int pos;
    string curr, lowerWord = lower(queriedWord);
    DocumentItem docItem;

    // Find the lowercased queried word in the hash table
    curr = hashTable.find(lowerWord);

    // If the word is not found, clear the result vector
    if (curr == "") {
        hashResultVector.clear();
    } else {
        // Word found, retrieve its position in the hash table
        pos = hashTable.findPos(lowerWord);
        int vecPos;

        // Iterate through the documents associated with the word
        for (int i = 0; i < hashTable.array[pos].value.size(); i++) {
            vecPos = isInVector(hashResultVector, hashTable.array[pos].value[i].documentName);

            // If the document is not already in the result vector, add it
            if (vecPos == -1) {
                resultItem resItem;
                resItem.documentName = hashTable.array[pos].value[i].documentName;
                docItem.documentName = hashTable.array[pos].key;
                docItem.count = hashTable.array[pos].value[i].count;
                resItem.resultVector.push_back(docItem);
                hashResultVector.push_back(resItem);
            } else {
                // If the document is already in the result vector, update its count
                docItem.documentName = hashTable.array[pos].key;
                docItem.count = hashTable.array[pos].value[i].count;
                hashResultVector[vecPos].resultVector.push_back(docItem);
            }
        }
    }
}

// Function to read a text file and build a Hash Table structure
void TextFiletoHash(const string& filename, HashTable<string, vector<DocumentItem>>& HashTable) {
    ifstream file(filename);
    string line, word, current;
    DocumentItem docItem;
    int pos = -1;
    int vecPos;
    string cleanLine;

    // Read each line of the file
    while (getline(file, line)) {
        cleanLine = cleanAndAddSpace(line);
        istringstream iss(cleanLine);

        // For each word in the line
        while (iss >> word) {

            string lowerWord = lower(word);
            current = HashTable.find(lowerWord);

            // If the word is not found in the hash table, insert a new entry
            if (current == "") {
                docItem.documentName = filename;
                docItem.count = 1;
                vector<DocumentItem> docVec;
                docVec.push_back(docItem);
                HashTable.insert(lowerWord, docVec);
            } else {
                // If the word is found, update the corresponding document count
                pos = HashTable.findPos(lowerWord);
                vecPos = isInVector(HashTable.array[pos].value, filename);

                if (vecPos == -1) {
                    // If the document is not already in the list, add a new entry
                    docItem.documentName = filename;
                    docItem.count = 1;
                    HashTable.array[pos].value.push_back(docItem);
                } else {
                    // If the document is already in the list, increment the count
                    HashTable.array[pos].value[vecPos].count++;
                }
            }
        }
    }

    file.close();  // Close the file
}

// Function to print the result vector
void printResultVector(const vector<resultItem>& hashResultVector) {
    if (hashResultVector.size() == 0) {
        cout << "No document contains the given query" << endl;
    } else {
        for (int i = 0; i < hashResultVector.size(); ++i) {
            cout << "In Document " << hashResultVector[i].documentName;

            // For each document in the result vector
            for (int j = 0; j < hashResultVector[i].resultVector.size(); j++) {
                cout << ", " << hashResultVector[i].resultVector[j].documentName
                     << " found " << hashResultVector[i].resultVector[j].count << " times";
            }
            cout << ".\n";
        }
    }
}

int main() {

    // Declare and initialize variables
    int k = 100; //For speed comprasion
    vector<DocumentItem> Words;
    string queriedWord, line, cleanLine, curr;
    int quantityWords = 0, pos;

    // Get file names and create necessary data structures
    vector<string> fileNames = getFileNames();
    tree<string, WordItem> Tree;
    HashTable<string, vector<DocumentItem>> hashTable("", 50);
    vector<resultItem> hashResultVector;

    // Build the initial tree structure based on the input files
    for (int i = 0; i < fileNames.size(); i++) {
        TextFiletoTree(fileNames[i], Tree);  // Assuming TextFiletoTree() is a function for building the tree
    }


    // Build the hash table based on the input files
    for (int i = 0; i < fileNames.size(); i++) {
        TextFiletoHash(fileNames[i], hashTable);  // Assuming TextFiletoHash() is a function for building the hash table
    }
    cin.ignore();

    // Calculate and display load factor
    double loadFactor = double(hashTable.currentSize) / double(hashTable.array.size());
    cout << endl << "After preprocessing, the unique word count is "<< hashTable.currentSize<< ". Current load ratio is " << loadFactor << endl;

    // User input for queried words

    cout << "Enter queried words in one line: ";
    getline(cin, line);
    cleanLine = cleanAndAddSpace(line);
    tree<string, vector<DocumentItem>> searchResultTree;

    // Measure time for BST search
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        quantityWords = 0;
        istringstream iss(cleanLine);
        searchResultTree.makeEmpty();
        while (iss >> queriedWord) {
            // Perform a search for the queried word and build the search result tree
            quantityWords++;
            searchEngineTree(queriedWord, Tree, searchResultTree);
            if (searchResultTree.getRoot() == nullptr) {
                break;
            }
        }

    }
    auto BSTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);

    // Print BST search results if the tree is not empty
    if(searchResultTree.getRoot()== nullptr)
    {
        cout << "No document contains the given query" << endl;
    }
    printResultTree(searchResultTree.getRoot(), quantityWords);


    // Measure time for hash table search
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        istringstream  iss2(cleanLine);
        hashResultVector.clear();
        while (iss2 >> queriedWord) {
            searchEngineHash(queriedWord, hashTable, hashResultVector);
            if(hashResultVector.empty())
            {
                break;
            }

        }

    }
    auto HTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);

    // Print hash table search results
    printResultVector(hashResultVector);

    // Display average time for BST search
    cout << "\nTime: " << BSTTime.count() / k << "\n";
    // Display average time for hash table search
    cout << "\nTime: " << HTTime.count() / k << "\n";
    // Display speedup factor
    cout << "\nSpeed Up: " << double(BSTTime.count()) / double(HTTime.count()) << "\n";

    // Clean up hash table and data structures
    hashTable.makeEmpty();
    hashResultVector.clear();
    searchResultTree.makeEmpty();
    Tree.makeEmpty();

    return 0;
}

