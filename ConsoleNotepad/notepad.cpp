#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
using namespace std;

void clear_screen() {
    printf("\033[H\033[J");
}

struct Note {
    Note *prev;
    char data;
    Note *next;
    Note *up;
    Note *down;
};

class Book {
    Note *head;
    Note *tail;
    Note *curLine;
    Note *prevLineNode;
    bool chk;
    Note *pointer;
public:
    Book() {
        chk = true;
        curLine = NULL;
        head = NULL;
        tail = NULL;
        prevLineNode = NULL; pointer = NULL;
    }
    void insertAtHead(char c) {
        Note *n = new Note;
        n->data = c; n->next = NULL; n->prev = NULL; n->up = NULL; n->down = NULL;
        if (head == NULL && tail == NULL) {
            head = n;
            tail = n;
        } else {
            if (head->down != NULL) {
                Note *cur = head;
                Note *downNote = head->down;
                downNote->up = n; n->down = downNote;
                cur->prev = n; n->next = cur;
                head = n;
                while (cur != NULL) {
                    if (downNote->next != NULL) {
                        cur->down = downNote->next; downNote->next->up = cur;
                    } else {
                        cur->down = NULL;
                    }
                    cur = cur->next; downNote = downNote->next;
                }
            } else {
                head->prev = n; n->next = head; head = n;
            }
        }
        if (n->data == '|') {
            pointer = n;
        }
    }
    void insert(char c) {
        Note *n = new Note;
        n->data = c; n->next = NULL; n->prev = NULL; n->up = NULL; n->down = NULL;
        if (head == NULL && tail == NULL) {
            head = n;
            tail = n;
            curLine = n; prevLineNode = n;
        } else if (c == '\n') {
            prevLineNode = curLine;
            curLine->down = n;
            n->up = curLine; curLine = curLine->down;
            tail = n;
            chk = false;
        } else if (chk && head != NULL) {
            tail->next = n;
            n->prev = tail;
            tail = n;
        } else {
            if (prevLineNode->next != NULL) {
                prevLineNode = prevLineNode->next;
                tail->next = n;
                n->prev = tail;
                tail = n;
                prevLineNode->down = n; n->up = prevLineNode;
            } else {
                tail->next = n;
                n->prev = tail;
                tail = n;
            }
        }
        if (n->data == '|') {
            pointer = n;
        }
    }
    void delLast() {
        if (head == NULL && tail == NULL) {
            cout << "List Empty.\n";
        } else {
            if (tail == head) {
                delete tail;
                head = NULL;
                tail = NULL;
            } else {
                Note *temp = tail;
                if (tail->prev != NULL) {
                    tail = tail->prev;
                    tail->next = NULL;
                    if (prevLineNode != NULL) {
                        prevLineNode->down = NULL;
                        prevLineNode = prevLineNode->prev;
                    }
                    temp->up = NULL;
                } else {
                    if (tail->up != NULL) {
                        Note *t = tail->up;
                        while (t->next != NULL) {
                            t = t->next;
                        }
                        tail->up->down = NULL;
                        tail->up = NULL;
                        tail = t;
                    }
                }
                delete temp;
                temp = NULL;
            }
        }
    }
    void deletePointerNote() {
        if (pointer != NULL) {
            Note* prevNote = pointer->prev;
            Note* nextNote = pointer->next;
            Note* upNote = pointer->up;
            Note* downNote = pointer->down;

            if (upNote != NULL) upNote->down = downNote;
            if (downNote != NULL) downNote->up = upNote;

            // Update the left and right links of adjacent notes
            if (prevNote != NULL) prevNote->next = nextNote;
            if (nextNote != NULL) nextNote->prev = prevNote;

            // If the current note is at the head, update the head
            if (pointer == head) head = nextNote;
            // If the current note is at the tail, update the tail
            if (pointer == tail) tail = prevNote;

            // Delete the current note and set the pointer to NULL
            delete pointer;
            pointer = NULL;
        }
    }
    void delAtHead() {
        if (head == NULL && tail == NULL) {
            cout << "List Empty.\n";
        } else {
            if (tail == head) {
                delete tail;
                head = NULL;
                tail = NULL;
            } else {
                while (pointer->next != NULL) {
                    char temp = pointer->next->data;
                    pointer->next->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->next;
                }

                Note *temp = pointer;
                temp->prev->next = NULL; temp->prev = NULL; 
                if (pointer->down != NULL) {
                    pointer->down->up = NULL; pointer->down = NULL;
                }

                delete temp;
                temp = NULL;
            }
        }
    }


    void display() {
        if (head == NULL && tail == NULL) {
            cout << "List Empty.\n";
        } else {
            Note* lineStart = head;
            while (lineStart != NULL) {
                Note* temp = lineStart;
                while (temp != NULL ) {
                    // if (temp->data == '\n')
                    //     cout << endl;
                    cout << temp->data;
                    temp = temp->next;
                }
                // cout << endl;
                lineStart = lineStart->down;
            }
        }
    }
    void save() {
        ofstream file("save.txt");
        if (file.is_open()) {
            if (head == NULL && tail == NULL) {
                cout << "List Empty.\n";
            } else {
                Note* lineStart = head;
                while (lineStart != NULL) {
                    Note* temp = lineStart;
                    while (temp != NULL ) {
                        if (temp->data != '\n' && temp->data != '\0' && temp->data != '|')
                            file << temp->data;
                        temp = temp->next;
                    }
                    file << endl;
                    lineStart = lineStart->down;
                }
            }
            file.close();
            cout << "\nFile saved successfully.....";
        } else {
            cout << "Error: Unable to open the file for writing....." << endl;
        }
    }
    void load(string fileName) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Failed to open File.\n";
        }
        string line;
        while (getline(file, line)) {
            for (char c : line) {
                cout << c; insert(c);
            }
            cout << endl;
            insert('\n');
        }
        file.close();
    }
    void destroy(int n) {
        while (n > 0) {
            this->delLast();
            n--;
        }
        if (n <= 0) {
            cout << "\nMemory Released Successfully.";
        }
    }

    bool isInDictionary(const string& word, const vector<string>& dictionary) {
        for (int i = 0; i < dictionary.size(); i++) {
            const string& WORD = dictionary[i];
            if (WORD.length() == word.length()) {
                bool match = true;
                for (int j = 0; j < word.length(); j++) {
                    if (WORD[j] != word[j]) {
                        match = false;
                        break;
                    }
                }
                if (match)
                    return true;
            }
        }
        return false;
    }
    string substitution(const string& word, const vector<string>& dictionary) {
        string subWord = word;
        for (int i = 0; i < word.length(); i++) {
            char origChar = word[i];
            for (char subChar = 'a'; subChar <= 'z'; subChar++) {
                if (subChar != origChar) {
                    string tempWord = word;
                    tempWord[i] = subChar;
                    if (isInDictionary(tempWord, dictionary)) {
                        return tempWord;
                    }
                }
            }
        }
        return word;
    }
    string omission(const string& word, const vector<string>& dictionary) {
        string omitWord;
        for (int i = 0; i < word.length(); i++) {
            string tempWord = word;
            tempWord.erase(i, 1);
            if (isInDictionary(tempWord, dictionary)) {
                return tempWord;
            }
        }
        return word;
    }
    string insertion(const string& word, const vector<string>& dictionary) {
        int len = word.length();
        for (char adChar = 'a'; adChar <= 'z'; adChar++) {
            for (int i = 0; i <= len; i++) {
                string tempWord = word;
                tempWord.insert(i, 1, adChar);
                if (isInDictionary(tempWord, dictionary)) {
                    return tempWord;
                }
            }
        }
        return word;
    }
    string reversal(const string &word, const vector<string>& dictionary) {
        string revWord = word;
        for (int i = 0; i < word.length()-1; i++) {
            if (revWord[i] != revWord[i+1]) {
                swap(revWord[i], revWord[i+1]);
                if (isInDictionary(revWord, dictionary)) {
                    return revWord;
                }
                swap(revWord[i], revWord[i+1]);
            }
        }
        return word;
    }
    void suggestions(const string& word, const vector<string>& dictionary) {
        string substitution_word, omission_word, insertion_word, reversal_word;
        int counter = 0;
        // WORD SUBSTITUTIONS----------------------------------------
        substitution_word = substitution(word, dictionary);
        if (word == substitution_word)
            substitution_word = "";
        else {
            cout << substitution_word << "/";
            if (substitution_word != "")
                counter++;
        }
        // WORD Omissions--------------------------------------------
        omission_word = omission(word, dictionary);
        if (word == omission_word)
            omission_word = "";
        else {
            cout << omission_word << "/";
            if (omission_word != "")
                counter++;
        }
        // WORD INSERTIONS------------------------------------------
        insertion_word = insertion(word, dictionary);
        if (word == insertion_word)
            insertion_word = "";
        else {
            cout << insertion_word << "/";
            if (insertion_word != "")
                counter++;
        }
        // WORD REVERSALS--------------------------------------------
        reversal_word = reversal(word, dictionary);
        if (word == reversal_word)
            reversal_word = "";
        else {
            cout << reversal_word << "/";
            if (reversal_word != "")
                counter++;
        }

        if (counter == 0) {
            cout << "You are a hopeless speller!";
        }
    }
    void spellChecker(const vector<string>& dictionary) {
        Note* temp = tail;
        if (temp == nullptr) {
            return; 
        } 
        if (temp->prev == NULL || temp->prev->data == '\n') {
            // cout << temp->data << endl;
            string s = ""; s += tolower(temp->data);
            if (!isInDictionary(s, dictionary)) {
                suggestions(s, dictionary); cout << "\n";
            }
            return;
        }
        while (temp->prev != NULL) {
            temp = temp->prev;
        }
        string word = "";
        bool inWord = false;

        while (temp != nullptr) {
            if (temp->data == ' ' || temp->data == '\n' || temp->next == nullptr) { 
                if (temp->next == NULL) 
                    word += temp->data;
                if (inWord) {
                    string temp = word; // ToLower word normalization for dictionary matching
                    for (int i = 0; i < word.length(); i++) {
                        if (isupper(temp[i])) {
                            temp[i] = tolower(temp[i]);
                        } if (temp[i] == '|') {
                            temp.erase(i, 1);
                        }
                    }
                    if (!isInDictionary(temp, dictionary)) {
                        suggestions(temp, dictionary); cout << " ";
                    }
                    word = "";
                    inWord = false;
                }
            } else {
                word += temp->data;
                inWord = true;
            }

            temp = temp->next;
        } cout << endl;
    }

    void cursorLeft(bool firstTime) {
        if (firstTime) {
            this->insert('|');
        } else {
            if (pointer->prev == NULL && pointer->up == NULL) { // Start to end
                this->delAtHead();
                this->insert('|');
            } if (pointer->prev->data != '\n') { // In line Movement
                char temp = pointer->prev->data;
                pointer->prev->data = pointer->data;
                pointer->data = temp;
                pointer = pointer->prev;
            } 
            else if (pointer->prev->data == '\n') { // Moving Lines
                if (pointer->up == NULL && pointer->down == NULL) {
                    this->deletePointerNote();
                    this->insert('|');
                    // If start of line move to above line or end of text.
                } else if (pointer->up != NULL && pointer->down == NULL) {
                    //=================================================================================================
                    Note *n = new Note; n->data = '|'; n->next = NULL; n->prev = NULL; n->up = NULL; n->down = NULL;
                    Note *cur = pointer->next;
                    Note *noteUp = pointer->up;
                    while (pointer->next != NULL) {
                        char temp = pointer->next->data;
                        pointer->next->data = pointer->data;
                        pointer->data = temp;
                        pointer = pointer->next;
                    }
                    this->delLast();
                    while (noteUp->next != NULL) {
                        noteUp = noteUp->next;
                        if (cur->next != NULL)
                            cur = cur->next;
                    }
                    if (cur->next != NULL) {
                        noteUp->next = n; n->prev = noteUp; cur->up = n; n->down = cur;
                    } else {
                        noteUp->next = n; n->prev = noteUp; n->down = NULL;
                    }
                    pointer = n;
                } 
                else if (pointer->up != NULL && pointer->down != NULL) { // ELSE IF UP != NULL AND DOWN != NULL
                    Note *Current = pointer->prev->up;
                    Note *upper = pointer->prev->up;
                    Note *cur = pointer;
                    Note *lower = pointer->down;
                    // cout << upper->data << cur->data << lower->data << endl;
                    while (pointer->next != NULL) {
                        char temp = pointer->next->data;
                        pointer->next->data = pointer->data;
                        pointer->data = temp;
                        pointer = pointer->next;
                    }
                    Note *temp = pointer;
                    if (pointer->up != NULL && pointer->down != NULL) {
                        pointer->prev->next = NULL; temp->prev = NULL;
                        pointer->up->down = NULL; pointer->up = NULL;
                        pointer->down->up = NULL; pointer->down = NULL;
                    } else if (pointer->up == NULL && pointer->down != NULL) {
                        pointer->prev->next = NULL; temp->prev = NULL;
                        pointer->up = NULL;
                        pointer->down->up = NULL; pointer->down = NULL;
                    } else if (pointer->up != NULL && pointer->down == NULL) {
                        pointer->prev->next = NULL; temp->prev = NULL;
                        pointer->down = NULL;
                        pointer->up->down = NULL; pointer->up = NULL;
                    } else if (pointer->up == NULL && pointer->down == NULL) {
                        pointer->prev->next = NULL; temp->prev = NULL;
                        pointer->down = NULL; pointer->up = NULL;
                    }

                    delete temp;
                    temp = NULL;

                    if (Current->up == NULL) {
                        lower = Current->down;
                        Note *n = new Note; n->data = '|'; n->next = NULL; n->prev = NULL; n->up = NULL; n->down = NULL;
                        while (Current->next != NULL) {
                            Current = Current->next;
                            if (lower->next != NULL)
                                lower = lower->next;
                        }
                        if (lower->next != NULL) {
                            Current->next = n; n->prev = Current; lower->up = n; n->down = lower;
                        } else {
                            Current->next = n; n->prev = Current; n->down = NULL;
                        }
                        pointer = n;
                    } else {
                        upper = Current->up;
                        lower = Current->down;
                        Note *n = new Note; n->data = '|'; n->next = NULL; n->prev = NULL; n->up = NULL; n->down = NULL;
                        while (Current->next != NULL) {
                            Current = Current->next;
                            if (lower->next != NULL)
                                lower = lower->next;
                            if (upper->next != NULL)
                                upper = upper->next;
                        }
                        if (lower->next != NULL) {
                            Current->next = n; n->prev = Current; lower->up = n; n->down = lower;
                        } else {
                            Current->next = n; n->prev = Current; n->down = NULL;
                        }
                        if (lower->next != NULL && upper->next != NULL) {
                            Current->next = n; n->prev = Current; 
                            lower->up = n; n->down = lower;
                            upper->down = n; n->up = upper;
                        } else if (lower->next == NULL && upper->next != NULL) {
                            Current->next = n; n->prev = Current;
                            n->down = NULL;
                            upper->down = n; n->up = upper;
                        } else if (lower->next != NULL && upper->next == NULL) {
                            Current->next = n; n->prev = Current;
                            lower->up = n; n->down = lower;
                            n->up = NULL;
                        } else if (lower->next == NULL && upper->next == NULL) {
                            Current->next = n; n->prev = Current;
                            n->down = NULL; n->up = NULL;
                        }
                        pointer = n;
                        
                    }
                } 
            }
        }
    }
    void cursorRight(bool firstTime) {
        if (firstTime) {
            this->insert('|');
        } else {
            if (pointer->next != NULL) {
                char temp = pointer->next->data;
                pointer->next->data = pointer->data;
                pointer->data = temp;
                pointer = pointer->next;
            } else if (pointer->next == NULL) {
                if (pointer->down == NULL) {
                    this->delLast();
                    this->insertAtHead('|');
                }
            }
        }
    }
    void cursorUp(bool firstTime) {
        if (firstTime) {
            this->insert('|');
        } else {
            if (head == pointer) {
                return;
            } else if (pointer->up == NULL && pointer != head) {
                // this->deletePointerNote();
                // this->insertAtHead('|');
                while (pointer->prev != NULL) {
                    char temp = pointer->prev->data;
                    pointer->prev->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->prev;
                }
            } else if (pointer->up != NULL && pointer->down == NULL) {
                Note *n = new Note; n->data = '|'; n->next = NULL; n->prev = NULL; n->up = NULL; n->down = NULL;
                Note *cur = pointer->next;
                Note *noteUp = pointer->up;
                int counter = 0;
                // cout << pointer->up->data << endl;
                // while (true) {cur = cur;}
                while (pointer->prev != NULL) {
                    char temp = pointer->prev->data;
                    pointer->prev->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->prev;
                    counter++;
                }
                while (pointer->next != NULL) {
                    char temp = pointer->next->data;
                    pointer->next->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->next;
                }
                this->delLast();
                while (noteUp->next != NULL) {
                    noteUp = noteUp->next;
                    if (cur->next != NULL)
                        cur = cur->next;
                }
                if (cur->next != NULL) {
                    noteUp->next = n; n->prev = noteUp; cur->up = n; n->down = cur;
                } else {
                    noteUp->next = n; n->prev = noteUp; n->down = NULL;
                }
                pointer = n;
                counter--;
                while (pointer->prev != NULL) {
                    char temp = pointer->prev->data;
                    pointer->prev->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->prev;
                    // counter++;
                }
                // cout << counter << endl;
                // while (true) {cur = cur;}
                
                while (pointer->next != NULL && counter > 0) {
                    char temp = pointer->next->data;
                    pointer->next->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->next;
                    counter--;
                }
                // cout << pointer->next->data << endl;
                // while (true) {cur = cur;}
            } else if (pointer->up != NULL && pointer->down != NULL) { //----------------------
                Note *Current = pointer->prev->up;
                Note *upper = pointer->prev->up;
                Note *cur = pointer;
                Note *lower = pointer->down;
                int counter = 0;
                while (pointer->prev != NULL) {
                    char temp = pointer->prev->data;
                    pointer->prev->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->prev;
                    counter++;
                }
                while (pointer->next != NULL) {
                    char temp = pointer->next->data;
                    pointer->next->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->next;
                }
                Note *temp = pointer;
                if (pointer->up != NULL && pointer->down != NULL) {
                    pointer->prev->next = NULL; temp->prev = NULL;
                    pointer->up->down = NULL; pointer->up = NULL;
                    pointer->down->up = NULL; pointer->down = NULL;
                } else if (pointer->up == NULL && pointer->down != NULL) {
                    pointer->prev->next = NULL; temp->prev = NULL;
                    pointer->up = NULL;
                    pointer->down->up = NULL; pointer->down = NULL;
                } else if (pointer->up != NULL && pointer->down == NULL) {
                    pointer->prev->next = NULL; temp->prev = NULL;
                    pointer->down = NULL;
                    pointer->up->down = NULL; pointer->up = NULL;
                } else if (pointer->up == NULL && pointer->down == NULL) {
                    pointer->prev->next = NULL; temp->prev = NULL;
                    pointer->down = NULL; pointer->up = NULL;
                }

                delete temp;
                temp = NULL;

                if (Current->up == NULL) {
                    lower = Current->down;
                    Note *n = new Note; n->data = '|'; n->next = NULL; n->prev = NULL; n->up = NULL; n->down = NULL;
                    while (Current->next != NULL) {
                        Current = Current->next;
                        if (lower->next != NULL)
                            lower = lower->next;
                    }
                    if (lower->next != NULL) {
                        Current->next = n; n->prev = Current; lower->up = n; n->down = lower;
                    } else {
                        Current->next = n; n->prev = Current; n->down = NULL;
                    }
                    pointer = n;
                    // counter-=2;
                    while (pointer->prev != NULL) {
                        char temp = pointer->prev->data;
                        pointer->prev->data = pointer->data;
                        pointer->data = temp;
                        pointer = pointer->prev;
                    }
                    while (pointer->next != NULL && counter > 0) {
                        char temp = pointer->next->data;
                        pointer->next->data = pointer->data;
                        pointer->data = temp;
                        pointer = pointer->next;
                        counter--;
                    }
                } else {
                    upper = Current->up;
                    lower = Current->down;
                    Note *n = new Note; n->data = '|'; n->next = NULL; n->prev = NULL; n->up = NULL; n->down = NULL;
                    while (Current->next != NULL) {
                        Current = Current->next;
                        if (lower->next != NULL)
                            lower = lower->next;
                        if (upper->next != NULL)
                            upper = upper->next;
                    }
                    if (lower->next != NULL) {
                        Current->next = n; n->prev = Current; lower->up = n; n->down = lower;
                    } else {
                        Current->next = n; n->prev = Current; n->down = NULL;
                    }
                    if (lower->next != NULL && upper->next != NULL) {
                        Current->next = n; n->prev = Current; 
                        lower->up = n; n->down = lower;
                        upper->down = n; n->up = upper;
                    } else if (lower->next == NULL && upper->next != NULL) {
                        Current->next = n; n->prev = Current;
                        n->down = NULL;
                        upper->down = n; n->up = upper;
                    } else if (lower->next != NULL && upper->next == NULL) {
                        Current->next = n; n->prev = Current;
                        lower->up = n; n->down = lower;
                        n->up = NULL;
                    } else if (lower->next == NULL && upper->next == NULL) {
                        Current->next = n; n->prev = Current;
                        n->down = NULL; n->up = NULL;
                    }
                    pointer = n;
                    counter--;
                    while (pointer->prev != NULL) {
                        char temp = pointer->prev->data;
                        pointer->prev->data = pointer->data;
                        pointer->data = temp;
                        pointer = pointer->prev;
                    }                    
                    while (pointer->next != NULL && counter > 0) {
                        char temp = pointer->next->data;
                        pointer->next->data = pointer->data;
                        pointer->data = temp;
                        pointer = pointer->next;
                        counter--;
                    }
                }
            }
        }
    }
    void cursorDown(bool firstTime) {
        if (firstTime) {
            this->insert('|');
        } else {
            if (tail == pointer) {
                return;
            } else if (pointer->down == NULL && pointer != tail) {
                while (pointer->next != NULL) {
                    char temp = pointer->next->data;
                    pointer->next->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->next;
                }
            } else if (pointer->down != NULL && pointer->up == NULL) {
                Note *n = new Note; n->data = '|'; n->next = NULL; n->prev = NULL; n->up = NULL; n->down = NULL;
                int counter = 0;
                
                while (pointer->prev != NULL && pointer->prev->data != '\n' && pointer->prev->data != '\0') {
                    char temp = pointer->prev->data;
                    pointer->prev->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->prev;
                    counter++;
                }
                Note *cur = pointer;
                Note *noteDown = pointer->next->down;
                while (pointer->next != NULL) {
                    char temp = pointer->next->data;
                    pointer->next->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->next;
                }
                // this->delLast();
                Note *temp = pointer;
                temp->prev->next = NULL; temp->prev = NULL; 
                if (pointer->down != NULL) {
                    pointer->down->up = NULL; pointer->down = NULL;
                }
                delete temp;
                temp = NULL;

                // cout << "DAS: " << cur->data << noteDown->data << endl;
                // while (true) {cur = cur;}
                while (noteDown->next != NULL) {
                    noteDown = noteDown->next;
                    if (cur->next != NULL)
                        cur = cur->next;
                }
                if (cur->next != NULL) {
                    noteDown->next = n; n->prev = noteDown; cur->down = n; n->up = cur;
                } else {
                    noteDown->next = n; n->prev = noteDown; n->up = NULL;
                }
                pointer = n;

                // counter--;
                while (pointer->prev != NULL) {
                    char temp = pointer->prev->data;
                    pointer->prev->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->prev;
                    // counter++;
                }
                // cout << counter << endl;
                // while (true) {cur = cur;}
                
                while (pointer->next != NULL && counter > 0) {
                    char temp = pointer->next->data;
                    pointer->next->data = pointer->data;
                    pointer->data = temp;
                    pointer = pointer->next;
                    counter--;
                }
                // cout << pointer->next->data << endl;
                // while (true) {cur = cur;}
            } 
        }
    }
    void bufferDisplay(int Len, int left, int right, const vector<string>& dictionary) {
        clear_screen();
        cout << "TEXT EDITOR";
        for (int j = 11; j < left; j++) cout << " ";
        cout << "| SUGESSTIONS\n";
        // Redraw the entire content
        this->display(); cout << endl;
        // cout << Len << " length";
        for (int i = Len; i < left; i++) cout << " ";
        cout << "| "; this->spellChecker(dictionary);
    }

};

int GetConsoleWindowWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
}

int main() {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD NumEvents = 0;
    INPUT_RECORD InRec;

    // READING WORDS FROM DICTIONARY ...............
    vector<string> dictionary;
    ifstream dictionaryFile("dictionary.txt");
    if (dictionaryFile.is_open()) {
        string LINE;
        while (getline(dictionaryFile, LINE)) {
            string WORD; 
            for (char c : LINE) {
                if (c != ' ') {
                    if (isupper(c)) 
                        c = tolower(c);
                    WORD += c;
                }
            }
            dictionary.push_back(WORD);
        }
        dictionaryFile.close();
    } else {
        cout << "Failed to Open Dictionary.\n";
    } //---------------------------------------------

    Book b; // List for Text
    int i = 0, width = 0;
    bool firstLine = true;
    bool ctrlPressed = false; // Flag to track Ctrl key press
    bool pointerFirst = true;

    int screenWidth = GetConsoleWindowWidth();
    int textEditorWidth = static_cast<int>(screenWidth * 0.75);
    int grammarWidth = static_cast<int>(screenWidth * 0.25);
    cout << "TEXT EDITOR";
    for (int j = 11; j < textEditorWidth; j++) cout << " ";
    cout << "| SUGESSTIONS\n";

    while (true) {
        ReadConsoleInput(hInput, &InRec, 1, &NumEvents);
        if (InRec.EventType == KEY_EVENT && InRec.Event.KeyEvent.bKeyDown) {
            if (InRec.Event.KeyEvent.uChar.AsciiChar == 27) { // Check for 'esc' to quit
                b.destroy(i);
                break;
            } else if (InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) { // Enter Key
                for (int j = width; j < textEditorWidth; j++)
                    cout << " ";
                cout << "| "; b.spellChecker(dictionary);
                width = 0;
                b.insert('\n'); i++;
            } else if (InRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK) {
                if (i > 0) {
                    cout << "\b \b";
                    b.delLast();
                    i--;
                }
            } else if (InRec.Event.KeyEvent.wVirtualKeyCode == VK_LEFT) { // Left Cursor Movement
                b.cursorLeft(pointerFirst);
                b.bufferDisplay(i+1, textEditorWidth, grammarWidth, dictionary);
                // b.display(); cout << endl;
                pointerFirst = false;
            } else if (InRec.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) {
                b.cursorRight(pointerFirst);
                b.bufferDisplay(i+1, textEditorWidth, grammarWidth, dictionary);
                pointerFirst = false;
            } else if (InRec.Event.KeyEvent.wVirtualKeyCode == VK_UP) {
                b.cursorUp(pointerFirst);
                b.bufferDisplay(i+1, textEditorWidth, grammarWidth, dictionary);
                pointerFirst = false;
            } else if (InRec.Event.KeyEvent.wVirtualKeyCode == VK_DOWN) {
                b.cursorDown(pointerFirst);
                b.bufferDisplay(i+1, textEditorWidth, grammarWidth, dictionary);
                pointerFirst = false;
            } else if (InRec.Event.KeyEvent.uChar.AsciiChar == 3) {
                // Check for Ctrl + C to copy or any other Ctrl + combination
                // Handle Ctrl + C or other Ctrl + actions
                // ...
            } else if (InRec.Event.KeyEvent.uChar.AsciiChar == 19) {
                // Check for Ctrl + S
                if (ctrlPressed) {
                    if (i > 0) {
                        b.save();
                    }
                }
            } else if (InRec.Event.KeyEvent.uChar.AsciiChar == 12) {
                if (ctrlPressed) {
                    b.load("save.txt");
                }
            } else if (InRec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED) {
                // Ctrl key is pressed
                ctrlPressed = true;
            } else if (InRec.Event.KeyEvent.uChar.AsciiChar != '\0') {
                if (width < textEditorWidth) {
                    cout << InRec.Event.KeyEvent.uChar.AsciiChar; // Print the pressed key
                    width++;
                }
                b.insert(InRec.Event.KeyEvent.uChar.AsciiChar);
                i++;
                // cout << i;
            }
        } else if (InRec.EventType == KEY_EVENT && !InRec.Event.KeyEvent.bKeyDown) {
            if (InRec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED) {
                // Ctrl key is released
                ctrlPressed = false;
            }
        }
    }

    cout << "\nHere are the keys you pressed: ";
    b.display();

    return 0;
}