#include <iostream>
using namespace std;

struct Node {
    int data;
    Node *next;
};

class List {
    Node *tail;
    public:
        List () {
            tail = NULL;
        }
        bool isempty() {
            return tail == NULL;
        }

        void addHead(int val) {
            Node *N = new Node;
            N->data = val;
            N->next = NULL;
            if (tail == NULL) {
                tail = N;
                tail->next = N;
            } else {
                N->next = tail->next;
                tail->next = N;
            }
        }
        void addTail(int val) {
            Node *N = new Node;
            N->data = val;
            N->next = NULL;
            if (tail == NULL) {
                tail = N;
                N->next = N;
            } else {
                N->next = tail->next;
                tail->next = N;
                tail = N;
            }
        }
        void delStart() {
            if (tail == NULL) {
                cout << "List is empty.\n";
            }
            Node *temp = tail->next;
            tail->next = temp->next;
            delete temp;
            temp = NULL;
        }
        void delEnd() {
            if (tail == NULL) {
                cout << "List is empty.\n";
            }
            Node *p = tail;
            Node *temp = tail;
            while (temp->next != tail) {
                temp = temp->next;
            }
            tail = temp;
            temp->next = p->next;
        }
        //-------------------DISPLAY-----------------------
        void display() {
            if (isempty()) {
                cout << "List is empty.\n";
            } else {
                Node *temp = tail->next;
                do {
                    cout <<temp->data << "->";
                    temp = temp->next;
                } while (temp != tail->next);
                cout << "Repeat" << endl;
                temp = NULL;
                delete temp;
            }
        }
};

int main() {
    List L;
    L.addHead(4);
    L.addTail(1);
    L.addTail(0);
    L.addTail(9);
    L.addTail(8);
    L.display();
    L.delStart();
    L.display();
    L.delEnd();
    L.display();
    return 0;
}