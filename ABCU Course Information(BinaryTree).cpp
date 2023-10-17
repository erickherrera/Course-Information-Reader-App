//============================================================================
// Name        : ABCU Course Information Application.cpp
// Author      : Erick Herrera
// Version     : 1.0
//============================================================================

#include <iostream>
#include <time.h>
#include "CSVparser.cpp"
#include <string>
#include <vector> 

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string coursetitle;
    vector<string> prerequisiteCourses;

};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize 
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void clearTree(Node* node);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    clearTree(root);
}

void BinarySearchTree::clearTree(Node* node) {
    if (node != nullptr) {
        // Recursively delete left and right subtrees
        clearTree(node->left);
        clearTree(node->right);
        // Delete the current node
        delete node;
    }
}
/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    inOrder(root);
}
/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}

/**
  Search for a course
 **/
Course BinarySearchTree::Search(string courseId) {
    Node* current = root;
    // keep looping downwards until bottom reached or matching courseId found
    while (current != nullptr) {
        // if match found, return current Course
        if (current->course.courseId == courseId) {
            return current->course;
        }
        // if courseId is smaller than current node then traverse left
        if (courseId < current->course.courseId) {
            current = current->left;
        }// else larger so traverse right
        else {
            current = current->right;
        }
    }
    Course course;
    return course;
}

/**
 * Add a Course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseId < node->course.courseId) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else { 
            addNode(node->left, course);
        }
    }
    else if (course.courseId > node->course.courseId) { 
        
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else { 
            addNode(node->right, course);
        }
    } 
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        std::cout <<endl << "Course ID: " << node->course.courseId << ", Course title: " << node->course.coursetitle << endl;
        for (int i = 0; i < node->course.prerequisiteCourses.size(); i++) {
            std::cout << "Prerequisite: " << node->course.prerequisiteCourses[i] << endl;
        }
        //InOder right
        inOrder(node->right);
    }return;
}
//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the Course information to the console (std::out)
 *
 * @param course struct containing the bid info
 */
void displayBid(Course course) {
    cout << course.courseId << ": " << course.coursetitle << " | ";
    for (int i = 0; i < course.prerequisiteCourses.size(); i++) {
        cout <<"Prerequisite: " << course.prerequisiteCourses[i] << endl;
    }
    return;
}

/**
 * Load a CSV file containing Course Information into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);
    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of courses
            Course course;
            course.courseId = file[i][0];
            course.coursetitle = file[i][1];
            std::cout << endl << "Course ID: " << course.courseId << ", Course Title: " << course.coursetitle << endl;
            // Check if there are prerequisite courses in the CSV
            if (file[i].size() > 2) {
                course.prerequisiteCourses.resize(file[i].size()-2);
                for (int j = 0; j < file[i].size()-2; j++) {
                    course.prerequisiteCourses[j] = file[i][j + 2];
                    std::cout << "Prerequisite: " << course.prerequisiteCourses[j] << std::endl;
                }
            }
            // push this course to the end
            bst->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseKey = "MATH201";
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "CourseData.csv";
        courseKey = "MATH201";
        break;
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) {
        cout <<endl << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Coursess" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            // Initialize a timer variable before loading 
            ticks = clock();
            // Complete the method call to load the
            loadCourses(csvPath, bst);
            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << endl << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();
            cout << "Enter the Course Id" << endl;
            cin >> courseKey;
            course = bst->Search(courseKey);
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!course.courseId.empty()) {
                displayBid(course);
            }
            else {
                cout << "Bid Id " << courseKey << " not found." << endl;
            }
            cout <<endl<< "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        }
        
    }
    cout << "Good bye." << endl;
    return 0;
}
