#include <iostream>

using namespace std;

// Data Class : Holds all the data that goes inside the Node
class Process {
public:
    string processName;
    int totalTime;
    Process(string processName, int totalTime) {
        this->totalTime = totalTime;
        this->processName = processName;
    }
    void updateRunTime() {
        totalTime--;
    }
    void print() {
        cout << "Process " << processName << " " ;
        cout << totalTime << "seconds" << endl;
    }
};

// Node Class : Node for the LinkedList
template <typename T> class Node {
public:
    T *data;
    Node<T> *next;
    Node<T> *prev;

    Node(T *data) {
        this->data = data;
        next = nullptr;
        prev = nullptr;
    }
    void print() { data->print(); }
};

template <typename T> class CircularDLL {
private:
    Node<T> *curr;
    int length;

public:
// Constructor
    CircularDLL(T *value) {
        Node<T> *newNode = new Node<T>(value);
        curr = newNode;
        length = 1;
    }

//Destructor
    ~CircularDLL() {
        Node<T> *temp = curr;
        while (curr) {
            curr = curr->next;
            delete temp;
            temp = curr;
        }
    }

    void printList() {
        if (length == 0) return;
        if (length == 1) {
            curr->print();
            return;
        }
        Node<T> *temp = curr;
         do {                   // Iterate through circular list, stopping at curr (head)
            temp->print();
            temp = temp->next;
        } while (temp != curr);
    }

    bool isEmpty() {
        if (length <= 0) return true;
        else return false;
    }

    Node<T> *getCurr(){
        return curr;
    }

    int getLength(){
        return length;
    }

    void insertProcess(T *value) {

        Node<T> *newNode = new Node<T>(value);

        if (length == 0) {              // No items, make newNode the new curr
            curr = newNode;
        } else if (length == 1){        // Only curr and newNode, assign pointers to match each other
            curr->prev = newNode;
            curr->next = newNode;
            newNode->prev = curr;
            newNode->next = curr;
        } else {                        // Using curr->prev to access last element in list,
            newNode->next = curr;       // ...attach newNode to first(curr) and last(curr->prev) node.
            curr->prev->next = newNode;
            newNode->prev = curr->prev;
            curr->prev = newNode;
        }
        length++;                       // Increment list length
    }

    void deleteProcess(Node<T> *temp) {

        if (length == 0) return;                // If all processes have finished, cannot delete, stop
        if (temp == curr) curr = curr->next;    // Special case if deleting the head node, ensuring no loss of curr reference

        if (length == 2) {
            temp->prev->next = nullptr;         // length = 2, make pointers of other node null before deletion
            temp->next->prev = nullptr;         // temp->next and temp->prev point to the same node in a length 2 list
        } else if (length > 2) {
            temp->prev->next = temp->next;      // Assign previous's *next to point to the next node
            temp->next->prev = temp->prev;      // Assign next's *prev to point to the previous node
        }

        delete temp;                            // Free up memory space and decrement length
        length--;
    }

};

//Main Program

int main() {

    Process *d1 = new Process("A", 3);
    Process *d2 = new Process("B", 4);
    Process *d3 = new Process("C", 4);
    Process *d4 = new Process("D", 15);
    Process *d5 = new Process("E", 15);

    int quantumTime;
    int timeElapsed = 0;
    int cycleNum = 1;
    int length;
    int newTime;
    string newName;
    char choice;
    Node<Process> *temp;
    Node<Process> *toDelete;

    CircularDLL<Process> *cdll = new CircularDLL<Process>(d1);


    cout << "Enter Quantum Time: ";
    cin >> quantumTime;
    cout << endl << "Prepopulating with processes" << endl << endl;

    // Prepopulated list inserted
    cdll->insertProcess(d2);
    cdll->insertProcess(d3);
    cdll->insertProcess(d4);
    cdll->insertProcess(d5);

    // Main system loop
    while (!cdll->isEmpty()){   // Program finished when list is empty

        cdll->printList();

        // Insert node
        cout << endl << "Add new process? (Enter Y/N) ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y'){    // User inserted node by typing "Y"
            cout << endl << "Enter New Process Name: ";
            cin >> newName;
            cout << endl << "Enter Total Process Time: ";
            cin >> newTime;
            Process *newProcess = new Process(newName, newTime);    // Create a new Process object with user parameters

            cdll->insertProcess(newProcess);    // Call insert method to join into list

            cout << endl << "Process Added." << endl << endl;
            continue;
        }


        // Run cycles and Delete nodes
        cout << endl << "Running Cycle " << cycleNum << endl << endl;
        for (int i = 1; i <= quantumTime; i++) {    // Each cycle, 1 second

            temp = cdll->getCurr();                 // Use temp pointer to iterate through list
            length = cdll->getLength();             // Use temp length to avoid problems with list shortening

            for (int j = 0; j < length; j++) {      // For each node in the current list,...

                temp->data->updateRunTime();        // Decrement node's runtime by 1 second

                if (temp->data->totalTime <= 0) {   // If process has completed(t <= 0),

                    toDelete = temp;                // Create delete proxy to avoid losing list reference
                    temp = temp->next;
                    cdll->deleteProcess(toDelete); // Delete node
                } else temp = temp->next;           // Still time remaining, advance to next node
            }
        }

        timeElapsed += quantumTime;
        cout << "After cycle " << cycleNum << " - " << timeElapsed << " second elapses - state of processes is as follows" << endl << endl;
        cycleNum++;
    }

    cout << "All processes are completed." << endl;

    return 0;
}