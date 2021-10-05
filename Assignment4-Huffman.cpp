//John Hoffman - F00555643
//CSC 545
//Assignment 4 - Huffman Coding
//10/04/2021

#include <iostream>
#include <cstdlib>
using namespace std;
 
//Setting the tree height manually
#define MAX_TREE_HT 100
 
//Structure for the Huffman node
struct MinHeapNode {
    char data;
    unsigned frequency;
    struct MinHeapNode *left, *right;
};
 
//Structure for the collection of the nodes
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};
 
//Function to create a new MinHeap node
struct MinHeapNode* newNode(char data, unsigned frequency) {
    struct MinHeapNode* temp = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
 
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->frequency = frequency;
 
    return temp;
}
 
//Function to create the MinHeap with the argument capacity
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
 
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
 
//Function to swap two MinHeap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
 
//Function to "MinHeapify" nodes
void minHeapify(struct MinHeap* minHeap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
 
    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency) {
        smallest = left;
    }
    
    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency) {
        smallest = right;
    }
 
    if (smallest != index) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[index]);
        minHeapify(minHeap, smallest);
    }
}
 
//Function to check if MinHeap size is 1
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}
 
//Function to receive the minimum value node
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];

    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);
 
    return temp;
}
 
//Function to insert a new node into MinHeap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
 
    while (i && minHeapNode->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
 
    minHeap->array[i] = minHeapNode;
}
 
//Function to build the MinHeap
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;
 
    for (i = (n - 1) / 2; i >= 0; --i) {
        minHeapify(minHeap, i);
    }
}
 
//Function to print the array
void printArray(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i) {
        cout<< arr[i];
    }
 
    cout<<"\n";
}
 
//Function to check if the node is a leaf
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}
 
//Function to create a MinHeap of "capacity" equal to "size" and inserts all of the characters from data[]
struct MinHeap* createAndBuildMinHeap(char data[], int frequency[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
 
    for (int i = 0; i < size; ++i) {
        minHeap->array[i] = newNode(data[i], frequency[i]);
    }
 
    minHeap->size = size;
    buildMinHeap(minHeap);
 
    return minHeap;
}
 
//Function to build the Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int frequency[], int size) {
    struct MinHeapNode *left, *right, *top;
 
    // Step 1: create a min heap of capacity equal to size
    struct MinHeap* minHeap = createAndBuildMinHeap(data, frequency, size);
 
    //Loop while the size of the heap isn't 1
    //Step 2: extract the two minimum frequency nodes from the MinHeap
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
 
        //Step 3: create a new internal node with a frequency equal to the sum of the two nodes frequencies
        //Make the two extracted nodes as left and right children of this new node
        //Add this node to the MinHeap
        top = newNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
 
    //Step 4: the remaining node is the root node and the tree is complete
    return extractMin(minHeap);
}
 
//Function to print Huffman codes
void printCodes(struct MinHeapNode* root, int arr[], int top) {
 
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
 
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (isLeaf(root)) {
        cout<< root->data <<": ";
        printArray(arr, top);
    }
}

//Function to build the Huffman tree and print the codes by traversal
void HuffmanCodes(char data[], int frequency[], int size) {
    struct MinHeapNode* root = buildHuffmanTree(data, frequency, size);
 
    int array[MAX_TREE_HT], top = 0;
 
    printCodes(root, array, top);
}
 
//Main driver function
int main() {
    char charArray[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int frequency[] = { 5, 9, 12, 13, 16, 45 };
 
    int size = sizeof(charArray) / sizeof(charArray[0]);
 
    HuffmanCodes(charArray, frequency, size);
 
    return 0;
}