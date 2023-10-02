#include <iostream>
using namespace std;

struct Node {
    Node *pre;
    int data;
    Node *next;
};

class List {
    Node *head;
    Node *tail;
    public:
        List () {
            head = NULL;
            tail = NULL;
        }
        bool empty() {
            return (head == NULL && tail == NULL);
        }
        void addStart(int val) {
            Node *N = new Node;
            N->data = val;
            N->next = NULL; N->pre = NULL;
            if (empty()) {
                head = N;
                tail = N;
            } else {
                N->next = head;
                head->pre = N;
                head = N;
            }
        }
        void addEnd(int val) {
            Node *N = new Node;
            N->data = val;
            N->next = NULL; N->pre = NULL;
            if (empty()) {
                head = N;
                tail = N;
            } else {
                tail->next = N;
                N->pre = tail;
                tail = N;
            }
        }
        void addLoc(int val, int loc) {
            Node *N = new Node;
            N->data = val;
            N->next = NULL; N->pre = NULL;
            Node *temp = head, *c;
            for (int i = 1; i < loc - 1; i++) {
                temp = temp->next;
            }
            c = temp->next;
            N->pre = temp;
            temp->next = N;
            N->next = c;
            c->pre = N;
            // temp = N;
        }
        //================================================
        void delHead() {
            if (!empty()) {
                Node *temp = head;
                head = temp->next;
                head->pre = NULL;
                temp->next = NULL;
                delete temp;
                temp = NULL;
            }
        }
        void delEND() {
            if (!empty()) {
                Node *temp = tail;
                tail = tail->pre;
                tail->next = NULL;
                temp->next = NULL;
                delete temp;
                temp = NULL;
            }
        }

        void display() {
            if (!empty()) {
                Node *temp = head;
                cout << "NULL <=> ";
                while (temp != NULL) {
                    cout << temp->data << " <=> ";
                    temp = temp->next;
                }
                cout << "NULL" << endl;
            }
        }
};

int main() {
    List L;
    L.addStart(4);
    L.addStart(5);
    L.addStart(9);
    L.addEnd(1);
    L.addEnd(0);
    L.display();
    L.addLoc(12, 2);
    L.display();
    L.delHead();
    L.delEND();
    L.display();
    return 0;
}