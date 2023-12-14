#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

string remLast(string str) {
    string temp = "";
    int i = 0;
    while (i < str.length()-1) {
        temp+=str[i];
        i++;
    }
    return temp;
}
void decryption() {
    string text;
    // Reading Codes
    ifstream readCodes("code.txt");
    if (!readCodes.is_open()) {
        cout << "Failed to open code File.\n"; return;
    }
    string code[255] = {""}, COD = ""; int count = 0, max = -1; // To find the max code length
    while (getline(readCodes, text)) { // Storing codes in RAM
        for (int i = 3; i < text.length(); i++) { COD += text[i]; count++; }
        code[int(text[0])] = COD;
        if (count > max) { max = count; } count = 0; // MAx for Later Use
        COD = "";
    }

    text = ""; string subCode = "";
    // Readind recieved File
    ifstream readFile("output.txt");
    if (!readFile.is_open()) {
        cout << "Failed to open seed File.\n"; return;
    }
    ofstream writeFile("decoded.txt"); // For Writing decrypted data into decoded file
    if (writeFile.is_open()) {
        cout << "Decrypted Data: \n";
        while (getline(readFile, text)) {
            int a = 0;
            while (a < text.length()) {
                count = max;
                // SubCode of Max Characters to perform maximum matching
                for (int i = a; i < a+max; i++) { subCode += text[i]; } 
                // cout << "|" << subCode << ":" << max << "-" << a << "|";
                bool found = true;
                while (found) {
                    for (int i = 0; i < 255; i++) { // Looking for relevant Codes
                        if (code[i] != "") {
                            if (code[i] == subCode) {
                                writeFile << char(i);
                                cout << char(i); found = false; break;
                            }
                        }
                    }
                    if (found) { // If not Found remove the last character
                        subCode = remLast(subCode);
                        count--;
                    }
                }
                // cout << a << " |";
                subCode = "";
                a += count; // For while Loop traversal
            }
            writeFile << endl;
            cout << endl;
        }
        cout << "-----------------------------\n";
        writeFile.close(); cout << "Decription Successfull.\n";
    } else {
        cout << "Unable to open Code.txt file.\n";
    }
}

int main() {
    decryption();
    return 0;
}


// for (int i = 0; i < 255; i++) {
//     if (code[i] != "") {
//         cout << char(i) << " :" << code[i] << endl;
//     }
// }