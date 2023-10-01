#include <iostream>
#include <ctime>
#include<string>
#include <conio.h>
using namespace std;

struct Post {
    static int uniqueIDs;
    int ID;
    string text;
    string timeStamp;
    Post *Next;
    Post() {
        ID = uniqueIDs++;
    }
    Post(string txt) {
        this->ID = uniqueIDs++;
        this->text = txt;
    }
};
int Post::uniqueIDs = 1000;

class Feed {
    Post *head;
    public:
        Feed() {
            head = NULL;
        }
        string TimeStampFun() {
            time_t curr_time;
            tm* curr_tm;
            char timeString[100];
            time(&curr_time);
            curr_tm = localtime(&curr_time);
            // Date in specified format
            strftime(timeString, 50, "%B %d, %Y", curr_tm);
            string timeStr = timeString;
            // Concatenate Time
            timeStr += "; ";
            timeStr += to_string(curr_tm->tm_hour);
            timeStr += ":";
            timeStr += to_string(curr_tm->tm_min);
            timeStr += ":";
            timeStr += to_string(curr_tm->tm_sec);
            return timeStr;
        }

        void addPost() {
            Post *temp = new Post;
            cout << "Enter Text: ";getline(cin, temp->text);
            temp->Next = NULL;
            temp->timeStamp = TimeStampFun();
            
            if (head == NULL) {
                head = temp;
            } else {
                temp->Next = head;
                head = temp;
            }
        }
        void editPost(int id, string txt) {
            if (head == NULL) {
                cout << "The Feed is Empty.\n";
            } else {
                Post *ptr = head;
                bool found = false;
                while(ptr != NULL) {
                    if (ptr->ID == id) {
                        // Update
                        ptr->text = txt;
                        ptr->timeStamp = TimeStampFun();
                        // Move the post to begining
                        if (ptr != head) {
                            Post *prev = head;
                            while (prev->Next != ptr) {
                                prev = prev->Next;
                            }
                            prev->Next = ptr->Next;
                            ptr->Next = head;
                            head = ptr;
                        }
                        cout << "Post has been edited.\n";
                        found = true;
                    }
                    ptr = ptr->Next;
                }
                if (!found)
                    cout << "Post with ID " << id << " not found.\n";
            }
        }
        //---               ------------                    -----
        void deletePost(int id) {
            if (head == NULL) {
                cout << "The Feed is Empty.\n";
            } else {
                // For First Node
                if (head->ID == id) {
                    Post *temp = head;
                    head = temp->Next;
                    delete temp;
                    return;
                }
                Post *prev = head, *current = head->Next;
                while (current != NULL) {
                    if (current->ID == id) {
                        prev->Next = current->Next;
                        delete current;
                        return;
                    }
                    prev = current;
                    current = current->Next;
                }
                cout << "Post with ID " << id << " not found." << endl;
            }
        }

        void searchPost(string searchText) {
            bool found = false;
            Post* ptr;
            for (ptr = head; ptr != NULL; ptr = ptr->Next) {
                if (ptr->text.find(searchText) != string::npos) {
                    cout << "<<===================================>\n"
                        << "||\t " << ptr->ID << "\n"
                        << "||\t " << ptr->text << "\n"
                        << "||\t " << ptr->timeStamp << "\n"
                        << "<<---------------------------------->\n|\n";
                    found = true;
                }
            }
            cout << endl << "\t\tXXXXXXXXXXXXXXXXXXXXXXXXX\n" << endl;
            if (!found) {
                cout << "No posts containing '" << searchText << "' were found." << endl;
            }
            // return 
        }
        void displayAllPostsLIFO() {
            if (head == NULL)
                cout << "The Feed is Emtpy :(" << endl;
            else {
                Post *ptr = head;
                while (ptr != NULL) {
                    cout << "<<===================================>\n"
                        << "||\t " << ptr->ID << "\n"
                        << "||\t " << ptr->text << "\n"
                        << "||\t " << ptr->timeStamp << "\n"
                        << "<<---------------------------------->\n|\n";
                    ptr = ptr->Next;
                }
                cout << endl << "\t\tXXXXXXXXXXXXXXXXXXXXXXXXX\n" << endl;
            }
        } // Latest First

        void displayAllPostsFILO(Post* current) {
            if (current == NULL)
                return;
            displayAllPostsFILO(current->Next);
            cout << "<<===================================>\n"
                << "||\t " << current->ID << "\n"
                << "||\t " << current->text << "\n"
                << "||\t " << current->timeStamp << "\n"
                << "<<---------------------------------->\n|\n";
        }

        // Wrapper function to start the display
        void displayAllPostsFILO() {
            if (head == NULL)
                cout << "The Feed is Empty :(" << endl;
            else {
                displayAllPostsFILO(head);
                cout << endl << "\t\tXXXXXXXXXXXXXXXXXXXXXXXXX\n" << endl;
            }
        } // Oldest First
};

int main() {
    Feed F;
    cout << "||>>-----Welcome to Otto Feed-----<<||\n";
    cout << "Press 0 to create new Post.\nPress 1 to view Feed (Latest First).\nPress 2 to view Feed (Oldest First).\nPress 3 to Delete post.\n";
    cout << "Press 4 to Edit Post.\nPress 5 to search Post by text.\nPress 6 to exit.\n-------Select Below--------\n";
    int x; 
    do {
        cout << "Enter Choice: \n"; x = _getch() - '0';
        switch(x) {
            case 0: {
                F.addPost();
            } break;
            case 1: {
                F.displayAllPostsLIFO();
            } break;
            case 2: {
                F.displayAllPostsFILO();
            } break;
            case 3: {
                int ids;
                cout << "Enter ID (****): "; cin >> ids;
                F.deletePost(ids);
            } break;
            case 4: {
                string Stxt; int ids;
                cout << "Enter ID (****): "; cin >> ids; cin.ignore(); cout << "Enter Text: "; getline(cin, Stxt);
                F.editPost(ids, Stxt);
            } break;
            case 5: {
                string searchTxt;
                cout << "Enter txt: "; getline(cin, searchTxt);
                F.searchPost(searchTxt);
            } break;
            case 6: {
                exit(0);
            } break;
            default: cout << "Invalid Number.\n";
        }
    } while(x != 6);

    return 0;
}

// {
    // F.addPost();
    // F.addPost();
    // F.addPost();
    // F.addPost();
    // F.addPost();
    // // F.addPost();
    // // F.addPost();
    // // F.addPost();
    // F.addPost();
    // F.addPost();
    // cout << "CONTAINING WORLD:\n";
    // F.searchPost("World");
    // // F.displayAllPostsLIFO();
    // cout << "CONTAINING guys:\n";
    // F.searchPost("guys");
    // string s; cin >> s;
    // F.editPost(1000, s);
    // F.displayAllPostsLIFO();
    // F.deletePost(1000);
    // F.displayAllPostsLIFO();
// }