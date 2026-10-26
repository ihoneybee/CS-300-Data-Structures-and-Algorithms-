/*

Programmer: Asia Besant
Date: 10/14/2026

This program helps academic advisors find course
information and prerequisites. 
Data Structure: Binary Search Tree

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// Course information
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Tree node
struct Node {
    Course course;
    Node* left;
    Node* right;
};

Node* root = nullptr;

// Remove extra spaces
string trim(string text) {
    int start = 0;
    int end = (int)text.length() - 1;

    while (start <= end && isspace((unsigned char)text[start])) {
        start++;
    }

    while (end >= start && isspace((unsigned char)text[end])) {
        end--;
    }

    return text.substr(start, end - start + 1);
}

// Make letters uppercase
string makeUppercase(string text) {
    for (int i = 0; i < (int)text.length(); i++) {
        text[i] = toupper((unsigned char)text[i]);
    }

    return text;
}

// Add a course
Node* insertCourse(Node* node, Course course) {
    if (node == nullptr) {
        Node* newNode = new Node;

        newNode->course = course;
        newNode->left = nullptr;
        newNode->right = nullptr;

        return newNode;
    }

    if (course.courseNumber < node->course.courseNumber) {
        node->left = insertCourse(node->left, course);
    }
    else if (course.courseNumber > node->course.courseNumber) {
        node->right = insertCourse(node->right, course);
    }
    else {
        node->course = course;
    }

    return node;
}

// Find a course
Node* searchCourse(Node* node, string courseNumber) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->course.courseNumber == courseNumber) {
        return node;
    }

    if (courseNumber < node->course.courseNumber) {
        return searchCourse(node->left, courseNumber);
    }

    return searchCourse(node->right, courseNumber);
}

// Print all courses
void printCourses(Node* node) {
    if (node == nullptr) {
        return;
    }

    printCourses(node->left);

    cout << node->course.courseNumber
         << ", "
         << node->course.courseName
         << endl;

    printCourses(node->right);
}

// Delete the tree
void clearTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    clearTree(node->left);
    clearTree(node->right);

    delete node;
}

// Load the file
bool loadCourses(string fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: File could not be opened." << endl;
        return false;
    }

    clearTree(root);
    root = nullptr;

    string line;
    int count = 0;

    // Read each line
    while (getline(file, line)) {
        if (trim(line).empty()) {
            continue;
        }

        stringstream ss(line);

        string courseNumber;
        string courseName;
        string prerequisite;

        getline(ss, courseNumber, ',');
        getline(ss, courseName, ',');

        courseNumber = makeUppercase(trim(courseNumber));
        courseName = trim(courseName);

        // Check the course
        if (courseNumber.empty() || courseName.empty()) {
            cout << "Error: Invalid course information." << endl;
            continue;
        }

        Course course;

        course.courseNumber = courseNumber;
        course.courseName = courseName;

        // Add prerequisites
        while (getline(ss, prerequisite, ',')) {
            prerequisite = makeUppercase(trim(prerequisite));

            if (!prerequisite.empty()) {
                course.prerequisites.push_back(prerequisite);
            }
        }

        root = insertCourse(root, course);
        count++;
    }

    file.close();

    if (count == 0) {
        cout << "Error: No courses were loaded." << endl;
        return false;
    }

    cout << "\nCourse data loaded successfully!" << endl;
    cout << count << " courses loaded." << endl;

    return true;
}

// Check prerequisites
void checkPrerequisites(Node* node) {
    if (node == nullptr) {
        return;
    }

    checkPrerequisites(node->left);

    for (int i = 0;
         i < (int)node->course.prerequisites.size();
         i++) {

        string prerequisite = node->course.prerequisites[i];

        if (searchCourse(root, prerequisite) == nullptr) {
            cout << "Warning: Prerequisite "
                 << prerequisite
                 << " does not exist."
                 << endl;
        }
    }

    checkPrerequisites(node->right);
}

// Print one course
void printCourseInformation(string courseNumber) {
    courseNumber = makeUppercase(trim(courseNumber));

    Node* course = searchCourse(root, courseNumber);

    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << "\n"
         << course->course.courseNumber
         << ", "
         << course->course.courseName
         << endl;

    // Check prerequisites
    if (course->course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites:" << endl;

    // Print prerequisites
    for (int i = 0;
         i < (int)course->course.prerequisites.size();
         i++) {

        string prerequisite =
            course->course.prerequisites[i];

        Node* found = searchCourse(root, prerequisite);

        if (found != nullptr) {
            cout << found->course.courseNumber
                 << ", "
                 << found->course.courseName
                 << endl;
        }
        else {
            cout << prerequisite
                 << " (Course not found)"
                 << endl;
        }
    }
}

// Show the menu
void displayMenu() {
    cout << "\n";
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "\n";

    cout << "What would you like to do? ";
}

// Start the program
int main() {
    string choice;
    string fileName;
    string courseNumber;

    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    // Keep showing the menu
    while (true) {
        displayMenu();

        if (!getline(cin, choice)) {
            break;
        }

        choice = trim(choice);

        // Load courses
        if (choice == "1") {
            cout << "Enter the file name: ";
            getline(cin, fileName);

            fileName = trim(fileName);

            if (fileName.empty()) {
                cout << "Error: Please enter a file name." << endl;
            }
            else {
                dataLoaded = loadCourses(fileName);

                if (dataLoaded) {
                    checkPrerequisites(root);
                }
            }
        }

        // Show all courses
        else if (choice == "2") {
            if (dataLoaded == false) {
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << "\nHere is a sample schedule:\n" << endl;
                printCourses(root);
            }
        }

        // Find one course
        else if (choice == "3") {
            if (dataLoaded == false) {
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);

                printCourseInformation(courseNumber);
            }
        }

        // Exit the program
        else if (choice == "9") {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }

        // Invalid choice
        else {
            cout << choice
                 << " is not a valid option."
                 << endl;
        }
    }

    // Clear memory
    clearTree(root);

    return 0;
}
