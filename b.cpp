#include <iostream>
using namespace std;

class Stack {
    int *arr; 
    int size;
    int top;
    public:
        Stack(int s) {
            arr = new int[s];
            size = s;
            top = -1;
        }
        bool isempty() {
            return (top == -1);
        }
        bool isfull() {
            return (top == (size-1));
        }
        void push(int val) {
            if (isfull()) {
                cout << "Stack is full.\n";
            } else {
                arr[++top] = val;
            }
        }
        int pop() {
            int v;
            if (isempty()) {
                cout << "Underflow error.\n";
            } else {
                v = arr[top--];
            }
            return v;
        }
        int peek() {
            if (isempty()) {
                cout << "Stack is empty.\n"; 
            } else {
                return arr[top];
            }
        }
        void display() {
            for (int i = top; i >= 0; i--) {
                cout << arr[i] << " "; 
            }
            cout << endl;
        }
        void reverse() {
            
        }
};

int main() {
    Stack s(6);
    s.push(8);
    s.push(10);
    s.push(12);
    s.push(2);
    s.display();
    s.pop();
    s.display();
    return 0;
}