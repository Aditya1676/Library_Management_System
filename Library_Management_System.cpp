#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

// ====================== BOOK CLASS ==========================
class Book {
public:
    char bookID[10];
    char title[50];
    char author[50];

    void create() {
        cout << "\nEnter Book ID: ";
        cin >> bookID;
        cin.ignore();

        cout << "Enter Book Title: ";
        cin.getline(title, 50);

        cout << "Enter Author Name: ";
        cin.getline(author, 50);

        cout << "\nBook Record Created.\n";
    }

    void display() const {
        cout << "\nBook ID   : " << bookID;
        cout << "\nTitle     : " << title;
        cout << "\nAuthor    : " << author << "\n";
    }

    void report() const {
        cout << left << setw(10) << bookID
             << setw(30) << title
             << setw(20) << author << endl;
    }

    char* getID() { return bookID; }
};

// ==================== STUDENT CLASS =========================
class Student {
public:
    char admNo[10];
    char name[50];
    char issuedBookID[10];
    int token;

    void create() {
        cout << "\nEnter Admission No: ";
        cin >> admNo;
        cin.ignore();

        cout << "Enter Student Name: ";
        cin.getline(name, 50);

        token = 0;
        issuedBookID[0] = '\0';

        cout << "\nStudent Record Created.\n";
    }

    void display() const {
        cout << "\nAdmission No : " << admNo;
        cout << "\nName         : " << name;
        cout << "\nBook Issued  : " << token;
        if (token == 1)
            cout << "\nBook ID      : " << issuedBookID << "\n";
        else
            cout << "\n";
    }

    void report() const {
        cout << left << setw(10) << admNo
             << setw(25) << name
             << setw(10) << token << endl;
    }

    char* getAdmNo() { return admNo; }
};

// ================== GLOBAL OBJECTS ==========================

Book bk;
Student st;
fstream fp, fp1;

// ================== FUNCTIONS ===============================

// ----- Create Records -----
void writeBook() {
    fp.open("book.dat", ios::out | ios::app | ios::binary);
    bk.create();
    fp.write((char*)&bk, sizeof(Book));
    fp.close();
}

void writeStudent() {
    fp.open("student.dat", ios::out | ios::app | ios::binary);
    st.create();
    fp.write((char*)&st, sizeof(Student));
    fp.close();
}

// ----- Display All Records -----
void showAllBooks() {
    fp.open("book.dat", ios::in | ios::binary);
    if (!fp) { cout << "\nFile Error\n"; return; }

    cout << "\nALL BOOKS:\n";
    cout << left << setw(10) << "BookID"
         << setw(30) << "Title"
         << setw(20) << "Author" << endl;

    while (fp.read((char*)&bk, sizeof(Book))) {
        bk.report();
    }
    fp.close();
}

void showAllStudents() {
    fp.open("student.dat", ios::in | ios::binary);
    if (!fp) { cout << "\nFile Error\n"; return; }

    cout << "\nALL STUDENTS:\n";
    cout << left << setw(10) << "AdmNo"
         << setw(25) << "Name"
         << setw(10) << "Token" << endl;

    while (fp.read((char*)&st, sizeof(Student))) {
        st.report();
    }
    fp.close();
}

// ----- Search specific -----
void searchBook() {
    char id[10];
    cout << "\nEnter Book ID to search: ";
    cin >> id;

    fp.open("book.dat", ios::in | ios::binary);
    bool found = false;

    while (fp.read((char*)&bk, sizeof(Book))) {
        if (strcmp(bk.getID(), id) == 0) {
            bk.display();
            found = true;
        }
    }
    fp.close();

    if (!found) cout << "Book Not Found.\n";
}

void searchStudent() {
    char id[10];
    cout << "\nEnter Admission No: ";
    cin >> id;

    fp.open("student.dat", ios::in | ios::binary);
    bool found = false;

    while (fp.read((char*)&st, sizeof(Student))) {
        if (strcmp(st.getAdmNo(), id) == 0) {
            st.display();
            found = true;
        }
    }
    fp.close();

    if (!found) cout << "Student Not Found.\n";
}

// ----- Issue a Book -----
void issueBook() {
    char adm[10], bid[10];
    cout << "\nEnter Admission No: ";
    cin >> adm;

    fp.open("student.dat", ios::in | ios::out | ios::binary);
    fp1.open("book.dat", ios::in | ios::binary);

    bool foundStudent = false, foundBook = false;

    while (fp.read((char*)&st, sizeof(Student)) && !foundStudent) {
        if (strcmp(st.getAdmNo(), adm) == 0) {
            foundStudent = true;

            if (st.token == 1) {
                cout << "Student already has a book.\n";
                return;
            }

            cout << "Enter Book ID: ";
            cin >> bid;

            while (fp1.read((char*)&bk, sizeof(Book)) && !foundBook) {
                if (strcmp(bk.getID(), bid) == 0) {
                    foundBook = true;
                    st.token = 1;
                    strcpy(st.issuedBookID, bid);

                    int pos = -1 * int(sizeof(Student));
                    fp.seekp(pos, ios::cur);
                    fp.write((char*)&st, sizeof(Student));

                    cout << "\nBook Issued Successfully.\n";
                }
            }
        }
    }

    if (!foundStudent) cout << "\nStudent Not Found.\n";
    if (!foundBook) cout << "\nBook Not Found.\n";

    fp.close();
    fp1.close();
}

// ----- Deposit Book -----
void depositBook() {
    char adm[10];
    cout << "\nEnter Admission No: ";
    cin >> adm;

    fp.open("student.dat", ios::in | ios::out | ios::binary);
    bool found = false;

    while (fp.read((char*)&st, sizeof(Student)) && !found) {
        if (strcmp(st.getAdmNo(), adm) == 0) {
            found = true;

            if (st.token == 0) {
                cout << "\nNo book to deposit.\n";
                return;
            }

            st.token = 0;
            st.issuedBookID[0] = '\0';

            int pos = -1 * int(sizeof(Student));
            fp.seekp(pos, ios::cur);
            fp.write((char*)&st, sizeof(Student));

            cout << "\nBook Deposited Successfully.\n";
        }
    }

    if (!found) cout << "\nStudent Not Found.\n";

    fp.close();
}

// ================== MAIN MENU ==============================
int main() {
    int ch;

    do {
        cout << "\n===============================";
        cout << "\n   LIBRARY MANAGEMENT SYSTEM   ";
        cout << "\n===============================";
        cout << "\n1. Add Book";
        cout << "\n2. Add Student";
        cout << "\n3. Show All Books";
        cout << "\n4. Show All Students";
        cout << "\n5. Search Book";
        cout << "\n6. Search Student";
        cout << "\n7. Issue Book";
        cout << "\n8. Deposit Book";
        cout << "\n9. Exit";
        cout << "\nEnter choice: ";
        cin >> ch;

        switch (ch) {
            case 1: writeBook(); break;
            case 2: writeStudent(); break;
            case 3: showAllBooks(); break;
            case 4: showAllStudents(); break;
            case 5: searchBook(); break;
            case 6: searchStudent(); break;
            case 7: issueBook(); break;
            case 8: depositBook(); break;
            case 9: cout << "\nExiting...\n"; break;
            default: cout << "Invalid Choice\n";
        }

    } while (ch != 9);

    return 0;
}
