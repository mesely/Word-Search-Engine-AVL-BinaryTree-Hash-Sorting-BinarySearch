# Word-Search-Engine-AVL-BinaryTree-Hash-Sorting-BinarySearch

# 1. AVL Tree Version

This C++ program simulates a search engine for text files using an AVL tree for efficient word frequency analysis.

## Features

- Add and search words across multiple text files.
- Case-insensitive word handling.
- Remove words from memory.
- List occurrences of specific words in files.

## File Structure

- **`main.cpp`**: Main application logic.
- **`tree.h`**: AVL tree implementation.

## Usage

1. Run the program.
2. Enter the number of text files and their names.
3. Query words or request word removals.
4. Type "ENDOFINPUT" to exit.

## Example

```cpp
Enter the number of input files: 2
Enter the name of file 1: file1.txt
Enter the name of file 2: file2.txt
Enter queried words in one line: example
Enter queried words in one line: REMOVE example
Enter queried words in one line: ENDOFINPUT
```

# 2. Hash Table Version

This C++ program simulates a search engine for text files using a Hash Table with quadratic probing for efficient word frequency analysis.

### Features

- Add and search words across multiple text files.
- Case-insensitive word handling.
- Remove words from memory.
- List occurrences of specific words in files.
- Compare the speed of Hash Table and AVL Tree implementations.

### File Structure

- **`main.cpp`**: Main application logic.
- **`hashTable.h`**: Hash Table implementation (quadratic probing).

### Usage

1. Run the program.
2. Enter the number of text files and their names.
3. Query words or request word removals.
4. Type `ENDOFINPUT` to exit.

### Example

```cpp
Enter the number of input files: 2
Enter the name of file 1: file1.txt
Enter the name of file 2: file2.txt
Enter queried words in one line: example
Enter queried words in one line: REMOVE example
Enter queried words in one line: ENDOFINPUT
```
# 3. Sorting and Binary Search Version

This C++ program simulates a search engine for text files and performs various sorting and searching algorithms, including AVL Tree, Hash Table, and Binary Search.

### Features

- Analyze word frequency using AVL Trees, Hash Tables, and Binary Search.
- Perform sorting using Quick Sort, Merge Sort, Heap Sort, and Insertion Sort.
- Compare the performance of different sorting and searching algorithms.

### File Structure

- **`main.cpp`**: Main application logic.
- **`hashTable.h`**: Hash Table implementation (quadratic probing).
- **`tree.h`**: Binary Tree with Sorting Algorithms implementation.


### Usage

1. Run the program.
2. Enter the number of text files and their names.
3. The program will process the files and perform the sorting and searching operations.
4. Type the queried words and analyze the results.

### Example

```cpp
Enter the number of input files: 2
Enter the name of file 1: file1.txt
Enter the name of file 2: file2.txt
Enter queried words in one line: example
Enter queried words in one line: REMOVE example
Enter queried words in one line: ENDOFINPUT
```

### Performance Metrics

- **Binary Search Tree (BST) Time:** Time taken for search operations using a BST.

- **Hash Table Time:** Time taken for search operations using a Hash Table.

- **Binary Search Time:** Time taken for search operations using Binary Search in a sorted heap.

- **Sorting Algorithms:** Performance metrics for Quick Sort (median, random, first pivot), Merge Sort, Heap Sort, and Insertion Sort.

### Speedup Comparisons

- **Speed up between BST and Hash Table**

-**Speed up between Merge Sort and Quick Sort (Median)**

- **Speed up between Heap Sort and Quick Sort (Median)**

- **Speed up between Insertion Sort and Quick Sort (Median)**

- **Speed up between Binary Search Tree Time and Binary Search**

- **Speed up between Hash Table Time and Binary Search**


