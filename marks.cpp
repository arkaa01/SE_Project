#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

// Callback function for displaying query results
int displayResults(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << argv[i] << endl;
    }
    cout << endl;
    return 0;
}

// Function to execute SQL queries
void executeQuery(const string& query, sqlite3* db) {
    char* errorMessage = nullptr;
    int result = sqlite3_exec(db, query.c_str(), displayResults, nullptr, &errorMessage);
    if (result != SQLITE_OK) {
        cout << "SQL error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

// Function to update marks for a student
void updateMarks(sqlite3* db, const string& studentName, int marks) {
    string query = "INSERT OR REPLACE INTO student_marks (name, marks) VALUES ('" + studentName + "', " + to_string(marks) + ");";
    executeQuery(query, db);
    cout << "Marks updated successfully!" << endl;
}

// Function to correct marks for a student
void correctMarks(sqlite3* db, const string& studentName) {
    string query = "SELECT marks FROM student_marks WHERE name='" + studentName + "';";
    executeQuery(query, db);

    int marks;
    cout << "Enter corrected marks for " << studentName << ": ";
    cin >> marks;

    query = "UPDATE student_marks SET marks=" + to_string(marks) + " WHERE name='" + studentName + "';";
    executeQuery(query, db);

    cout << "Marks corrected successfully!" << endl;
}

// Function to display all student names and their marks
void displayMarks(sqlite3* db) {
    string query = "SELECT * FROM student_marks;";
    executeQuery(query, db);
}

int main() {
    sqlite3* db;
    int result = sqlite3_open("student_marks.db", &db);
    if (result != SQLITE_OK) {
        cout << "Failed to open database." << endl;
        return 0;
    }

    string createTableQuery = "CREATE TABLE IF NOT EXISTS student_marks ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "name TEXT NOT NULL,"
                              "marks INTEGER NOT NULL"
                              ");";
    executeQuery(createTableQuery, db);

    int choice;
    string studentName;

    do {
        cout << "Menu:" << endl;
        cout << "1. Update Marks" << endl;
        cout << "2. Correct Marks" << endl;
        cout << "3. Display Marks" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter student name: ";
                cin.ignore();
                getline(cin, studentName);

                int marks;
                cout << "Enter marks for " << studentName << ": ";
                cin >> marks;

                updateMarks(db, studentName, marks);
                break;

            case 2:
                cout << "Enter student name: ";
                cin.ignore();
                getline(cin, studentName);

                correctMarks(db, studentName);
                break;

            case 3:
                displayMarks(db);
                break;

            case 4:
                cout << "Exiting program..." << endl;
                break;

            default:
                cout << "Invalid choice! Please try again." << endl;
        }

        cout << endl;

    } while (choice != 4);

    sqlite3_close(db);

    return 0;
}
//trfyghujik
