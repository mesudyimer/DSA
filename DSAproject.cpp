#include <iostream>
using namespace std;
struct Student {
    int id;
    string name;
    int presentCount;
    int absentCount;
    Student* next;
};
class StudentList {
private:
    Student* head;

public:
    StudentList() {
        head = NULL;
    }

    Student* getHead() {
        return head;
    }

   
    Student* findById(int id) {
        Student* temp = head;

        while (temp != NULL) {
            if (temp->id == id)
                return temp;

            temp = temp->next;
        }

        return NULL;
    }

    
    void addStudent(int id, string name) {
        if (findById(id)) {
            cout << "ID already exists!\n";
            return;
        }

        Student* newNode = new Student();

        newNode->id = id;
        newNode->name = name;
        newNode->presentCount = 0;
        newNode->absentCount = 0;
        newNode->next = NULL;
        if (head == NULL) {
            head = newNode;
        }
        else {
            Student* temp = head;

            while (temp->next != NULL) {
                temp = temp->next;
            }

            temp->next = newNode;
        }

        cout << "Student added successfully!\n";
    }
    void display() {

        if (head == NULL) {
            cout << "No students found.\n";
            return;
        }

        Student* temp = head;

        while (temp != NULL) {

            cout << "ID: " << temp->id
                 << " | Name: " << temp->name
                 << " | Present: " << temp->presentCount
                 << " | Absent: " << temp->absentCount
                 << endl;

            temp = temp->next;
        }
    }
    void markAttendance(Student* s, bool present) {

        if (present) {

            s->presentCount++;

            cout << s->name << " -> Present\n";
        }
        else {

            s->absentCount++;

            cout << s->name << " -> Absent\n";

            if (s->absentCount >= 3) {
                cout << "!! Teacher sign required for "
                     << s->name << endl;
            }
        }
    }
    void undoAttendance(int id, bool wasPresent) {

        Student* s = findById(id);

        if (!s)
            return;

        if (wasPresent) {

            if (s->presentCount > 0)
                s->presentCount--;
        }
        else {

            if (s->absentCount > 0)
                s->absentCount--;
        }

        cout << "Undo successful for "
             << s->name << endl;
    }
};
struct QueueNode {
    Student* data;
    QueueNode* next;
};

class AttendanceQueue {

private:
    QueueNode* front;
    QueueNode* rear;

public:

    AttendanceQueue() {
        front = rear = NULL;
    }
    void enqueue(Student* s) {

        QueueNode* newNode = new QueueNode();

        newNode->data = s;
        newNode->next = NULL;

        if (rear == NULL) {

            front = rear = newNode;
        }
        else {

            rear->next = newNode;
            rear = newNode;
        }
    }
    Student* dequeue() {

        if (front == NULL)
            return NULL;

        QueueNode* temp = front;

        Student* s = temp->data;

        front = front->next;

        if (front == NULL)
            rear = NULL;

        delete temp;

        return s;
    }

    bool isEmpty() {
        return front == NULL;
    }
};
struct StackNode {
    int id;
    bool wasPresent;
    StackNode* next;
};

class Stack {

private:
    StackNode* top;

public:

    Stack() {
        top = NULL;
    }
    void push(int id, bool state) {

        StackNode* newNode = new StackNode();

        newNode->id = id;
        newNode->wasPresent = state;

        newNode->next = top;

        top = newNode;
    }
    StackNode* pop() {

        if (top == NULL)
            return NULL;

        StackNode* temp = top;

        top = top->next;

        return temp;
    }

    bool isEmpty() {
        return top == NULL;
    }
};
struct TreeNode {
    Student* data;
    TreeNode* left;
    TreeNode* right;
};

class BST {

public:

    TreeNode* root;

    BST() {
        root = NULL;
    }

    TreeNode* insert(TreeNode* node, Student* s) {

        if (node == NULL) {

            TreeNode* newNode = new TreeNode();

            newNode->data = s;
            newNode->left = NULL;
            newNode->right = NULL;

            return newNode;
        }

        if (s->id < node->data->id) {

            node->left = insert(node->left, s);
        }
        else {

            node->right = insert(node->right, s);
        }

        return node;
    }
    TreeNode* search(TreeNode* node, int id) {

        if (node == NULL)
            return NULL;

        if (node->data->id == id)
            return node;

        if (id < node->data->id) {

            return search(node->left, id);
        }
        else {

            return search(node->right, id);
        }
    }
};
int main() {

    StudentList list;
    AttendanceQueue q;
    Stack history;
    BST tree;

    int choice;

    while (true) {

        cout << "\n========== ATTENDANCE SYSTEM ==========\n";

        cout << "1. Add Student\n";
        cout << "2. Load Students For Attendance\n";
        cout << "3. Mark Attendance\n";
        cout << "4. Search Student By ID\n";
        cout << "5. Undo Last Attendance\n";
        cout << "6. Display All Students\n";
        cout << "7. Exit\n";

        cout << "Enter Choice: ";
        cin >> choice;
        if (choice == 1) {

            int id;
            string name;

            cout << "Enter Student ID: ";
            cin >> id;

            cin.ignore();

            cout << "Enter Student Name: ";
            getline(cin, name);
            list.addStudent(id, name);
            Student* s = list.findById(id);
            if (s != NULL) {
                tree.root = tree.insert(tree.root, s);
            }
        }
        else if (choice == 2) {

            if (!q.isEmpty()) {

                cout << "Attendance queue already loaded!\n";
                continue;
            }

            Student* temp = list.getHead();

            if (temp == NULL) {

                cout << "No students available.\n";
                continue;
            }

            while (temp != NULL) {

                q.enqueue(temp);

                temp = temp->next;
            }

            cout << "Students loaded successfully into queue!\n";
        }

        else if (choice == 3) {

            if (q.isEmpty()) {

                cout << "Queue is empty. Load students first.\n";
                continue;
            }

            while (!q.isEmpty()) {

                Student* s = q.dequeue();

                int status;

                cout << "\n"
                  << s->name
                     << " (1 = Present, 0 = Absent): ";

                cin >> status;
                history.push(s->id, status == 1);
                list.markAttendance(s, status == 1);
            }

            cout << "\nAttendance completed successfully!\n";
        }

        
        else if (choice == 4) {

            int id;

            cout << "Enter Student ID: ";
            cin >> id;

            TreeNode* result = tree.search(tree.root, id);

            if (result != NULL) {

                cout << "\nStudent Found!\n";

                cout << "ID: " << result->data->id<< endl;

                cout << "Name: "<< result->data->name<< endl;

                cout << "Present: "<< result->data->presentCount<< endl;

                cout << "Absent: "<< result->data->absentCount<< endl;
            }
            else {

                cout << "Student not found.\n";
            }
        }

        else if (choice == 5) {

            if (history.isEmpty()) {

                cout << "Nothing to undo.\n";
                continue;
            }

            StackNode* last = history.pop();

            list.undoAttendance(last->id, last->wasPresent);
            delete last;
        }

            else if (choice == 6) {

            list.display();
        }
        else if (choice == 7) {

            cout << "Program terminated.\n";
            break;
        }

        else {

            cout << "Invalid choice.\n";
        }
    }

    return 0;
}