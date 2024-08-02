/*
 * Selman Yilmaz CS300 Homework 2
 * This C++ program functions similarly to a search engine.
 * Suppose you have several text files and you wish to quickly determine
 * the frequency with which a given word appears in each file.
 * Thanks to AVL Tree Implementation, addition, and search operations' time complexity is O(log n).
 *
 * This is how it works:
 * First, it asks how many text files you want to process and what their names are. Simple as that.
 * Then it goes through each file, noting how frequently each word appears in different documents.
 * The program will then find and display the occurrences of specific words in your files.
 *
 * It doesn't care whether you type words in upper or lower case; it's cool like that.
 * If you don't like a word, you can even request that it be removed from the program's memory.
 *
 * Guide:
 * When prompted, enter the number and name of your text files.
 * Enter the words you're interested in.
 * Simply state your desire to have a word removed. (REMOVE wordthatyouwantremove)
 * When you're finished, simply type "ENDOFINPUT" and you're done.
 */

#include "tree.h"  // Assuming tree.h contains the AVL tree implementation
#include <vector>
#include <fstream>
#include <sstream>

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

// Function to capitalize the first letter of a given string
string capitalize(const string& str) {
    string result;
    char c;

    // Capitalize the first letter of the input string
    for (int i = 0; i < str.length(); i++) {
        if (i == 0) {
            c = str.at(i);
            c = toupper(c);
            result += c;
        } else {
            c = str.at(i);
            c = tolower(c);
            result += c;
        }
    }

    return result;
}

// Function to capitalize the first letter of a given string
string lower(const string& str) {
    string result;
    char c;

    // Capitalize the first letter of the input string
    for (int i = 0; i < str.length(); i++) {

        c = str.at(i);
        c = tolower(c);
        result += c;
    }

    return result;
}

// Function to check if a word consists only of alphabetic characters
bool isAlphabetic(string word) {
    for (int i = 0; i < word.length(); i++) {
        char c = word[i];

        // Check if each character is alphabetic
        if (!isalpha(c)) {
            return false;
        }
    }

    return true;
}

// Function to read a text file and build a tree structure
void TextFiletoTree(const string& filename, tree<string, WordItem>& Tree) {
    ifstream file(filename);
    string line, word;
    WordItem wrdItem;

    treeNode<string, WordItem>* current;
    treeNode<string, DocumentItem> *docCurrent;

    DocumentItem docItem;

    // Read each line of the file
    while (getline(file, line)) {
        istringstream iss(line);

        // For each word in the line
        while (iss >> word) {
            if (isAlphabetic(word)) {
                string capitalizedWord = capitalize(word);
                current = Tree.find(capitalizedWord, Tree.getRoot());

                if (current == nullptr) {
                    // If the word is not found in the tree, create a new entry
                    tree<string, DocumentItem> ItemTree;
                    docItem.documentName = filename;
                    docItem.count = 1;
                    ItemTree.insert(filename, docItem);
                    wrdItem.word = word;
                    wrdItem.docItemTree = ItemTree;
                    Tree.insert(capitalize(word), wrdItem);
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
    if (t == nullptr)
        return;

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
void searchEngine(const string& queriedWord, tree<string, WordItem>& Tree, tree<string, vector<DocumentItem>>& searchResultTree) {
    vector<DocumentItem> Words;
    treeNode<string, WordItem>* current = Tree.find(capitalize(queriedWord), Tree.getRoot());

    if (current != nullptr) {
        // Build the search result tree based on the occurrences of the queried word
        buildResultTree(current->value.docItemTree.getRoot(), queriedWord, searchResultTree, Words);
    }
    else
    {
        cout << "No document contains the given query" << endl;
        if(searchResultTree.getRoot() != nullptr)
            searchResultTree.makeEmpty();
        return;
    }
}

int main() {
    vector<DocumentItem> Words;
    string queriedWord, line;
    int quantityWords= 0;
    treeNode<string, WordItem>* current;
    bool remove = false;
    vector<string> fileNames = getFileNames(), queriedWords;
    tree<string, WordItem> Tree;

    // Build the initial tree structure based on the input files
    for (int i = 0; i < fileNames.size(); i++) {
        TextFiletoTree(fileNames[i], Tree);
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do {
        quantityWords = 0;
        cout << endl << "Enter queried words in one line: ";
        getline(cin, line);
        istringstream iss(line);
        tree<string, vector<DocumentItem>> searchResultTree;

        if(capitalize(line ) == capitalize("REMOVE"))
        {
            iss>>queriedWord;
            // if the word that wanted to remove is "remove"
            searchEngine(queriedWord, Tree, searchResultTree);
        }
        while (iss >> queriedWord) {
            quantityWords++;
            if (remove) {
                // Remove the queried word from the tree
                current = Tree.find(capitalize(queriedWord), Tree.getRoot());
                if (current != nullptr) {
                    Tree.deleteKey(capitalize(queriedWord), Tree);
                    cout << queriedWord << " has been REMOVED" << endl;
                }
                break;
            }
            if (!remove && capitalize(queriedWord) == capitalize("REMOVE")) {
                // Set the remove flag to true if the input is "REMOVE"
                remove = true;
            } else if (!remove && queriedWord != "ENDOFINPUT") {
                // Perform a search for the queried word and build the search result tree
                searchEngine(queriedWord, Tree, searchResultTree);
                if(searchResultTree.getRoot() == nullptr)
                {
                    break;
                }
            }
        }

        if(searchResultTree.getRoot()!= nullptr)
        {
            // Print the search results
            printResultTree(searchResultTree.getRoot(),quantityWords);
            searchResultTree.makeEmpty();

        }

        remove = false;
    } while (capitalize(line) != capitalize("ENDOFINPUT"));

    Tree.makeEmpty();
    return 0;
}
