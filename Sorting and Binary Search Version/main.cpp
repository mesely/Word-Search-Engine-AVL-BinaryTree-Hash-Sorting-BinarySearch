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
#include "randgen.h"

#include "hashTable.h" // hashTable.h contains the Hash Table implementation (quadratic probing)

using namespace std;

// Structure to store information about each document
struct DocumentItem {
    string documentName;
    int count;
};

// Structure to represent each word and its occurrences in documents Tree version
struct WordItem {
    string word;
    tree<string, DocumentItem> docItemTree;
};
// Structure to represent each word and its occurrences in documents Vector version
struct WordItemVector {
    string word;
    vector<DocumentItem> docVector;
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

// Function to check if a document is already in the result vector (binary version)
template <typename T>
int binaryisInVector(vector<T> vec, string value) {

    int low = 0;
    int high = vec.size()-1;
    while(low <= high)
    {
        int mid = low + ( high-low) / 2;

        if (vec[mid].documentName == value) {
            return mid;
        }
        if(vec[mid].documentName < value)
        {
            low++;
        }
        else
        {
            high--;
        }

    }
    return -1;
}

// Function to check if a document is already in the result vector
template <typename T>
int isInVector(vector<T> vec, string value) {
    for(int i = 0 ; i<vec.size(); i++)
    {
        if(vec[i].documentName == value)
        {
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
bool find( vector<WordItemVector> &vector, const WordItemVector &itemToFind) {
    for (int i = 0; i < vector.size(); ++i) {
        if (vector[i].word == itemToFind.word) {

            for (int j = 0; j< vector[i].docVector.size() ; j++) {
                if(vector[i].docVector[j].documentName == itemToFind.docVector[0].documentName)
                {
                    vector[i].docVector[j].count++;
                    return true;
                }

            }
            vector[i].docVector.push_back(itemToFind.docVector[0]);
            return true;
        }
    }

    // If not found
    return false;  // Stop searching after finding the first occurrence
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
void buildResultTree(treeNode<string, DocumentItem>* t, string queriedWord, tree<string, vector<DocumentItem> >& searchResultTree, vector<DocumentItem>& Words) {
    treeNode<string, vector<DocumentItem> >* temp;

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
void printResultTree(treeNode<string, vector<DocumentItem> >* t,int quantityWords) {
    if (t == nullptr) {
        return;
    }
    printResultTree(t->left,quantityWords);

    // Print the details of the search result for each document
    if(quantityWords == t->value.size())
    {
        cout << "in Document " << t->key << ", ";

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
void searchEngineTree(const string& queriedWord, tree<string, WordItem>& Tree, tree<string, vector<DocumentItem> >& searchResultTree) {
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
void searchEngineHash(const string& queriedWord, HashTable<string, vector<DocumentItem> >& hashTable, vector<resultItem>& hashResultVector) {
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
void TextFiletoHash(const string& filename, HashTable<string, vector<DocumentItem> >& HashTable) {
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
            cout << "in Document " << hashResultVector[i].documentName;

            // For each document in the result vector
            for (int j = 0; j < hashResultVector[i].resultVector.size(); j++) {
                cout << ", " << hashResultVector[i].resultVector[j].documentName
                     << " found " << hashResultVector[i].resultVector[j].count << " times";
            }
            cout << ".\n";
        }
    }
}
// This function inserts a WordItemVector into a max heap and adjusts the heap accordingly.
void insertHeap(vector<WordItemVector> & Heap,string docName,int & size, int &totalSize)
{


    int i = size;
    WordItemVector temp = Heap[i];

    //If word is not in the heap insert it into heap
    while(i>1 && temp.word>Heap[i/2].word)
    {
        Heap[i]=Heap[i/2];
        i=i/2;
    }
    Heap[i] = temp;
    size++;


}

// This function deletes the root (maximum element) of the max heap and adjusts the heap accordingly.
WordItemVector deleteHeap(vector<WordItemVector> &Heap, int & size) {


    WordItemVector value = Heap[1];
    // Move the last element of the heap to the root
    WordItemVector temp = Heap[size ];

    int i = 1, j = 2;

    // Adjust the heap to maintain the max heap property
    while (j <= size ) {
        // Find the child with the larger value
        if (j < size - 2 && Heap[j + 1].word > Heap[j].word) {
            j = j + 1;
        }
// If the value of the last element is less than the child, swap them
        if (temp.word < Heap[j].word) {
            Heap[i] = Heap[j];
            i = j;
            j = 2 * j;
        } else {
            break;
        }
    }
// Place the stored root value at its correct position
    Heap[i] = temp;
    Heap[size] = value;

    int l = size+1;
    int h = Heap.size()-1;
    // Perform binary search to find the correct position of the last element in the heap
    while(l<=h)
    {
        int mid = (l + (h-l)/2);
        if(Heap[mid].word > value.word)
        {
            h = mid -1;
        }
        else
        {
            l = mid +1;
        }


    }

    size--; // Reduce the size of the heap
    return value;
}



// This function performs the heap sort algorithm on a max heap represented by a vector.
void HeapSort(vector<WordItemVector> &Heap)
{
    int n = Heap.size()-1;
    // Delete the root (max element) of the heap to build a sorted array
    while(n>=1)
    {
        deleteHeap(Heap,n);
    }

}

//  This function reads a text file, processes its content, and stores word frequencies in a vector.
void TextFiletoVector( string fileName, vector<WordItemVector> &vector)
{
    ifstream file(fileName);
    string line, word;
    DocumentItem docItem;
    WordItemVector wordItem;
    string cleanLine;



    docItem.documentName = fileName;
    docItem.count = 1;

    // Read each line of the file
    while (getline(file, line)) {
        // Clean and add spaces to the line
        cleanLine = cleanAndAddSpace(line);
        istringstream iss(cleanLine);

        // For each word in the line
        while (iss >> word) {
            bool isInVector = false;
            wordItem.docVector.clear();
            wordItem.docVector.push_back(docItem);
            wordItem.word = lower(word);
// Check if the word is already in the vector
            isInVector = find(vector,wordItem);
            // If the word is not in the vector, add it
            if(!isInVector)
            {
                vector.push_back(wordItem);
            }


        }
    }

    file.close();  // Close the file

}

// This function builds a max heap from a given vector using the heapify process.
void buildHeap(vector<WordItemVector> &Heap)
{
    string fileName;
    //Build Heap
    int totalSize = Heap.size(), size = 2;
    // Iterate through the vector to build the max heap
    while(size < totalSize) {

        fileName =  Heap[size].docVector[0].documentName;
        // Insert the element at index 'size' into the heap
        insertHeap(Heap, fileName,size, totalSize);

    }

}
// This function performs the insertion sort algorithm on a vector.
void InsertionSort(vector<WordItemVector> & InsertionVector)
{

    int size = InsertionVector.size();
    int i = 1;
    // Iterate through the vector to perform insertion sort
    while( i < size)
    {

        bool flag = false;
        WordItemVector x = InsertionVector[i] ;
        int j = i-1;
        // Compare and insert the current element into the sorted portion of the vector
        while ( j> -1 && InsertionVector[j].word>= x.word && !flag)
        {
            InsertionVector[j+1] = InsertionVector[j];
            j--;
        }

        InsertionVector[j+1] = x;
        i++;
    }
}
// This function merges two subvectors in a vector using the merge process.
void Merge(vector<WordItemVector>& MergeVector, int low, int mid, int & high){
    int i = low;
    int j = mid + 1;

// Merge the two subvectors into a temporary vector
    while (i <= mid && j <= high){
        if (MergeVector[i].word < MergeVector[j].word){
            i++;
        } else if (MergeVector[i].word > MergeVector[j].word) {
            WordItemVector temp = MergeVector[j];
// Shift elements to make space for the new element
            for (int k = j; k > i; k--){
                MergeVector[k] = MergeVector[k - 1];
            }

            MergeVector[i] = temp;

            i++;
            mid++;
            j++;
        }
    }
}
// This function recursively sorts a vector A using the MergeSort algorithm.
void RecursiveMergeSort(vector<WordItemVector>& MergeVector, int low, int high){
    if (low < high){
        int mid = low + (high - low) / 2;
        // Recursively sort the two halves
        RecursiveMergeSort(MergeVector, low, mid);
        RecursiveMergeSort(MergeVector, mid + 1, high);

        // Merge the sorted halves
        Merge(MergeVector, low, mid, high);
    }
}


// This function partitions a vector using the first element as the pivot for the quicksort algorithm.
int partitionFirst(vector<WordItemVector>& A, int low, int high, WordItemVector pivot) {
    int i = low + 1;
    int j = high;
// Adjust the pivot position if it equals the last element
    if (A[j].word == pivot.word) {
        j++;
    }
    // Partition the vector based on the pivot
    while (true) {
        while (i <= j && A[i].word <= pivot.word) {
            i++;
        }
        while (A[j].word >= pivot.word && j >= i) {
            j--;
        }
        if (j < i) {
            break;
        } else {
            swap(A[i], A[j]);
        }
    }
    swap(A[low], A[j]);
    return j;
}

//  This function partitions a vector using a specified pivot element for the quicksort algorithm.
int partition(vector<WordItemVector>& A, int low, int high, WordItemVector pivot) {
    int i = low;
    int j = high;
// Partition the vector based on the pivot
    while (true) {
        while (A[i].word < pivot.word) {
            i++;
        }
        while (A[j].word > pivot.word) {
            j--;
        }
        if (i < j) {
            swap(A[i], A[j]);
        } else {
            return j;
        }
    }
}

// This function calculates the median of three elements (low, mid, high) in a vector A.
WordItemVector medianOfThree(vector<WordItemVector>& A, int low, int high) {
    int mid = low + (high - low) / 2; // Calculate mid index

    WordItemVector median;
    if ((A[low].word >= A[mid].word && A[low].word <= A[high].word) || (A[low].word >= A[high].word && A[low].word <= A[mid].word))
        median = A[low];
    else if ((A[mid].word >= A[low].word && A[mid].word <= A[high].word) || (A[mid].word >= A[high].word && A[mid].word <= A[low].word))
        median = A[mid];
    else
        median = A[high];

    return median;
}
// This function sorts a vector A using the QuickSort algorithm with the first element as the pivot.
void QuickSort(vector<WordItemVector>& A, int low, int high) {
    if (low < high) {
        WordItemVector pivot = A[low];
        int p = partitionFirst(A, low, high, pivot);
        QuickSort(A, low, p - 1);
        QuickSort(A, p + 1, high);
    }
}
// This function sorts a vector A using the QuickSort algorithm with the median-of-three pivot selection.
void QuickSortMedian(vector<WordItemVector>& A, int low, int high) {
    if (low < high) {
        WordItemVector pivot = medianOfThree(A, low, high);
        int p = partition(A, low, high, pivot);
        QuickSortMedian(A, low, p - 1);
        QuickSortMedian(A, p + 1, high);
    }
}

// This function sorts a vector A using the QuickSort algorithm with random pivot selection.
void QuickSortRandom(vector<WordItemVector>& A, int low, int high) {
    if (low < high) {
        RandGen randGen;
        int pivotIndex = randGen.RandInt(low, high);
        WordItemVector pivot = A[pivotIndex];
        swap(A[low], A[pivotIndex]);  // Move the randomly chosen pivot to the front
        int p = partition(A, low, high, pivot);
        QuickSortRandom(A, low, p - 1);
        QuickSortRandom(A, p + 1, high);
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

    // Declare vectors for various sorting algorithms
    vector<WordItemVector> unSortedVector;
    vector<WordItemVector> MergeVector;
    vector<WordItemVector> QuickFirstPivotVector;
    vector<WordItemVector> QuickMedianPivotVector;
    vector<WordItemVector> QuickRandomPivotVector;
    vector<WordItemVector> InsertionVector;
    vector<WordItemVector> Heap;
    DocumentItem docItem; WordItemVector wordItem;
    // Hash table and result vectors
    HashTable<string, vector<DocumentItem> > hashTable("", 10);
    vector<resultItem> hashResultVector;
    vector<resultItem> SortingResultVector;
    // Tree structure
    tree<string, WordItem> Tree;
    docItem.count = 0; docItem.documentName = "" ; wordItem.word = "" ; wordItem.docVector.push_back(docItem);
    Heap.push_back(wordItem);

    // Process each file
    for (int i = 0; i < fileNames.size(); i++) {

        TextFiletoVector(fileNames[i], unSortedVector);  // Assuming TextFiletoTree() is a function for building the tree
    }
    // Copy vectors for different sorting algorithms
    MergeVector = unSortedVector;
    QuickFirstPivotVector= unSortedVector;
    QuickMedianPivotVector= unSortedVector;
    QuickRandomPivotVector= unSortedVector;
    InsertionVector= unSortedVector;
    Heap= unSortedVector;
    // Build heap for heap sort
    buildHeap(Heap);

    auto start = chrono::high_resolution_clock::now();

    // QuickSort with median pivot
    QuickSortMedian(QuickMedianPivotVector,0,QuickMedianPivotVector.size()-1);


    auto QuickMedianTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
    // Record time and start sorting with random pivot
    start = chrono::high_resolution_clock::now();


    QuickSortRandom(QuickRandomPivotVector,0,QuickRandomPivotVector.size()-1);

// Perform QuickSort with random pivot on QuickRandomPivotVector
    auto QuickRandomTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
    start = chrono::high_resolution_clock::now();

// Perform QuickSort on QuickFirstPivotVector with the first element as the pivot
    QuickSort(QuickFirstPivotVector,0,QuickFirstPivotVector.size()-1);

// Measure the execution time of QuickSort with the first pivott
    auto QuickFirstTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);


    start = chrono::high_resolution_clock::now();
// Perform Recursive Merge Sort on MergeVector
    RecursiveMergeSort(MergeVector, 0, MergeVector.size() - 1);
// Measure the execution time of Merge Sort
    auto MergeTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
    start = chrono::high_resolution_clock::now();

// Perform Insertion Sort on InsertionVector
    InsertionSort(InsertionVector);

// Measure the execution time of Insertion Sort
    auto InsertionTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
    start = chrono::high_resolution_clock::now();

// Perform Heap Sort on Heap
    HeapSort(Heap);
// Measure the execution time of Heap Sort
    auto HeapTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);


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
    cout  << "After preprocessing, the unique word count is "<< hashTable.currentSize<< ". Current load ratio is " << loadFactor << endl;


    // User input for queried words

    cin.clear();
    cout << "Enter queried words in one line: ";
    getline(cin, line);
    cleanLine = cleanAndAddSpace(line);




    tree<string, vector<DocumentItem> > searchResultTree;

    // Measure time for BST search
    start = chrono::high_resolution_clock::now();
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


// Measure time for search and result building
    start = chrono::high_resolution_clock::now();
    // Perform search and result building 'k' times
    for (int i = 0; i < k; i++) {
        istringstream iss3(cleanLine);
        SortingResultVector.clear(); // Clear the result vector for each iteration
        // Process each queried word
        while (iss3 >> queriedWord) {
            // Perform a search for the queried word and build the search result tree
// Convert the queried word to lowercase
            queriedWord = lower(queriedWord);

            // Initialize variables for binary search in the heap
            int low = 0;
            int high = Heap.size()-1;
            int vecPos;
            resultItem resItem;
            resItem.resultVector.clear();
            bool Flag = false;
            // Binary search in the heap for the queried word
            while (low <= high) {
                int mid = low + (high - low) / 2;

                if (Heap[mid].word == queriedWord) {
                    Flag = true;

                    // Iterate through the documents associated with the word
                    for (int i = 0; i < Heap[mid].docVector.size(); i++) {
                        // Check if the document is already in the result vector
                        vecPos = binaryisInVector(SortingResultVector,  Heap[mid].docVector[i].documentName);

                        // If the document is not already in the result vector, add it
                        if (vecPos == -1) {

                            resItem.resultVector.clear();
                            resItem.documentName =   Heap[mid].docVector[i].documentName;
                            docItem.documentName =  Heap[mid].word;
                            docItem.count =Heap[mid].docVector[i].count;
                            resItem.resultVector.push_back(docItem);
                            SortingResultVector.push_back(resItem);
                        } else {
                            // If the document is already in the result vector, update its count
                            docItem.documentName = Heap[mid].word;
                            docItem.count =Heap[mid].docVector[i].count;
                            SortingResultVector[vecPos].resultVector.push_back(docItem);
                        }

                    }

                    break;

                } else if (Heap[mid].word < queriedWord) {
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            // If the word is not found in the heap, clear the result vector and break the loop
            if(!Flag)
            {
                SortingResultVector.clear();
                break;
            }



        }
        // If the result vector is empty, break the loop
        if (SortingResultVector.size() == 0) {
            break;
        }
    }
    auto binarySearchTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
    printResultVector(SortingResultVector);


    // Display average time for BST search
    cout << "\nBinary Search Tree Time: " << BSTTime.count() / k << "\n";
    // Display average time for hash table search
    cout << "Hash Table Time: " << HTTime.count() / k << "\n";
    // Display average time for binary search
    cout << "Binary Search Time: " << (binarySearchTime.count() / k) << "\n\n";

    // Display times for various sorting algorithms
    cout << "Quick Sort(median) Time: "<<  QuickMedianTime.count()  << endl;
    cout << "Quick Sort(random) Time: " <<  QuickRandomTime.count()  << endl;
    cout << "Quick Sort(first) Time: " <<  QuickFirstTime.count() << endl;
    cout << "Merge Sort Time: " << MergeTime.count() << endl;
    cout << "Heap Sort Time: " << HeapTime.count()  << endl;
    cout << "Insertion Sort Time: " << InsertionTime.count()  << endl << endl;
    // Display speedup factor
    cout << "Speed Up BST/HST: " << double(BSTTime.count()) / double(HTTime.count()) << "\n";
    cout <<"Speed Up Merge Sort/Quick Sort(Median): " << double(MergeTime.count()) / double(QuickMedianTime.count() ) << endl;
    cout << "Speed Up Heap Sort/Quick Sort(Median): " << double(HeapTime.count()) /double(QuickMedianTime.count() ) << endl;
    cout <<"Speed Up Insertion Sort/Quick Sort(Median): " << double( InsertionTime.count()) /double(QuickMedianTime.count())  << endl <<endl;
    cout << "Speed Up Binary Search Tree Time / Binary Search: " << double(BSTTime.count()) / double(binarySearchTime.count()) <<endl;
    cout << "Speed Up Hash Table Time / Binary Search: " << double(HTTime.count()) / double(binarySearchTime.count()) <<endl;


    // Clean up hash table and data structures
    hashTable.makeEmpty();
    hashResultVector.clear();
    searchResultTree.makeEmpty();
    Tree.makeEmpty();
    Heap.clear();
    MergeVector.clear();
    QuickRandomPivotVector.clear();
    QuickFirstPivotVector.clear();
    QuickMedianPivotVector.clear();
    InsertionVector.clear();



    return 0;
}

