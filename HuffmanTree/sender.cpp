#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left;
    Node *right;
};

class Queue {
protected:
    Node* arr[100];
    int front, rear;
public:
    Queue() {
        front = rear = -1;
    }
    bool isEmpty() {
        return front == -1;
    }
    bool isFull() {
        return rear == 99;
    }
    void push(Node *val) {
        if (isFull()) {
            cout << "Queue is full. Cannot push " << endl;
            return;
        }
        if (front == -1)
            front = rear = 0;
        else
            rear++;
        arr[rear] = val;
    }
    void pop() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot pop." << endl;
            return;
        }
        if (front == rear) {
            front = rear = -1;
        } else {
            front++;
        }
    }
    bool empty() {
        return front == -1;
    }
    Node* peek() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return NULL;
        }
        return arr[front];
    }
};

class Tree {
    Node *root;
public:
    Tree() {
        root = NULL;
    }
    void insert (int arr[255]) {
        if (root == NULL) { // Base Case
            int min1 = INT_MAX, min2 = INT_MAX, min1Index, min2Index;
            for (int i = 0; i < 255; i++) {
                if (arr[i] != 0) {
                    if (arr[i] < min1) {
                        min2 = min1; min2Index = min1Index;
                        min1 = arr[i]; min1Index = i;
                    } else if (arr[i] < min2) {
                        min2 = arr[i]; min2Index = i;
                    }
                }
            }
            int minSum = min1 + min2;
            Node *leftChild = new Node; leftChild->ch = char(min1Index); 
            leftChild->freq = arr[min1Index]; leftChild->left = leftChild->right = NULL;
            Node *rightChild = new Node; rightChild->ch = char(min2Index); 
            rightChild->freq = arr[min2Index]; rightChild->left = rightChild->right = NULL;

            // Inserting Left and right childs to Root
            Node *newRoot = new Node; newRoot->ch = char(minSum);
            newRoot->freq = -1; newRoot->right = rightChild; newRoot->left = leftChild;
            root = newRoot;
            arr[min1Index] = arr[min2Index] = 0;
        } else { // General Case Adding new Min and old tree with new ROot
            int min = INT_MAX, minIndex;
            for (int i = 0; i < 255; i++) {
                if (arr[i] != 0) {
                    if (arr[i] < min) {
                        min = arr[i]; minIndex = i;
                    }
                }
            }
            Node *leftChild = new Node; leftChild->ch = char(minIndex);
            leftChild->freq = arr[minIndex]; leftChild->left = leftChild->right = NULL;
            int minSum = arr[minIndex] + int(root->ch);

            Node *newRoot = new Node; newRoot->ch = char(minSum);
            newRoot->freq = -1; newRoot->right = root; newRoot->left = leftChild;
            root = newRoot;
            arr[minIndex] = 0;
        }
    }
    void display() {
        levelOrder(root);
    }
    void levelOrder(Node *root) { // Display to check treee implementation BFT or Level order traversal
        if (root == NULL) return;
        Queue Q;
        Q.push(root);
        while (!Q.empty()) {
            Node *current = Q.peek();
            if (current->freq == -1) {
                cout << int(current->ch) << " ";
            } else 
                cout << current->ch << " ";
            if (current->left != NULL) Q.push(current->left);
            if (current->right != NULL) Q.push(current->right);
            Q.pop();
        }
    }

    string remLast(string str) { // To remove the last character from the string
        string temp = "";
        int i = 0;
        while (i < str.length()-1) {
            temp+=str[i];
            i++;
        }
        return temp;
    }
    void coding(string fileName) { // Using the tree to generate codes for the respective characters
        string code[255] = {""}, xc;
        Node *temp = root;
        while (temp->right != NULL) {
            if (temp->freq == -1) {
                xc += '0'; 
                code[int(temp->left->ch)] = xc;
                xc = remLast(xc);
            }
            temp = temp->right;
            xc += '1';
        }
        code[int(temp->ch)] = xc;
        
        // Writing the codes in the Code.txt File
        ofstream writeFile("code.txt");
        if (writeFile.is_open()) {
            for (int i = 0; i < 255; i++) {
                if (code[i] != "") {
                    writeFile << char(i) << " :" << code[i] << endl;
                }
            }
            writeFile.close(); cout << "Codes written Successfully.\n";
        } else {
            cout << "Unable to open Code.txt file.\n";
        }
        // Calling the encryption funciton to encrypt data
        encrypt(fileName, code);
    }
    void encrypt(string fileName, string code[]) {
        string text;
        ifstream readFile(fileName);
        ofstream writeFile("output.txt");
        if (!readFile.is_open()) {
            cout << "Failed to Open seed File.\n"; return;
        }
        if (writeFile.is_open()) {
            while (getline(readFile, text)) {
                int i = 0;
                while (i < text.length()) {
                    writeFile << code[int(text[i])]; i++;
                }
                writeFile << endl;
            }
            writeFile.close(); cout << "Encryption Successfully.\n";
        } else {
            cout << "Unable to open Code.txt file.\n";
        }
    }
};

int main() {
    string fileName, text;
    int arr[255] = {0};
    cout << "Enter FileName: "; cin >> fileName; fileName += ".txt";
    ifstream readFile(fileName);
    if (!readFile.is_open()) {
        cout << "Failed to Open File.\n"; return -1;
    }
    while (getline(readFile, text)) {
        int i = 0;
        while (i < text.length()) { // Counting the frequecy of Characters
            arr[text[i]]++;
            i++;
        }
    }

    int count = 0;
    for (int i = 0; i < 255; i++) {
        if (arr[i] != 0) {
            // cout << char(i) << ": " << arr[i] << endl;
            count++;
        }
    }

    Tree t;
    for (int i = 0; i < count-1; i++) {
        t.insert(arr);
    }
    // t.display(); cout << endl;
    t.coding(fileName);
    return 0;
}



// for (int i = 0; i < 255; i++) {
//     if (code[i] != "") {
//         cout << char(i) << " :" << code[i] << endl;
//     }
// }