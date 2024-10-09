#include <iostream>

using namespace std;

int const VERTEXCOUNT = 5;

class Data {
private:
    int sourceVertex;
    int destinationVertex;
    int edgeCost;
public:
    Data(int source, int destination, int cost){
        sourceVertex = source;
        destinationVertex = destination;
        edgeCost = cost;
    }
    int getCost(){
        return edgeCost;
    }
    int getSource(){
        return sourceVertex;
    }
    int getDestination(){
        return destinationVertex;
    }
    void setCost(int newValue){
        edgeCost = newValue;
    }
    void setSource(int source){
        sourceVertex = source;
    }
    void setDestination(int destination){
        destinationVertex = destination;
    }
    void print(){
        cout << sourceVertex<<" - "<<destinationVertex<<" -> "<<edgeCost;
    }
    bool compare(Data *comparison){
        if (edgeCost > comparison->getCost()) return true;
        else return false;
    }
};

template <typename T> class Node{
private:
    T* data;
    Node<T>* leftChild;
    Node<T>* rightChild;
    Node<T>* parent;
public:
    Node(T* data){
        this->data = data;
        leftChild = nullptr;
        rightChild = nullptr;
        parent = nullptr;
    }
    T* getData(){
        return data;
    }
    Node<T>* getLeftChild(){
        return leftChild;
    }
    bool leftExist(){
        if (leftChild == nullptr) return false;
        else return true;
    }
    Node<T>* getRightChild(){
        return rightChild;
    }
    bool rightExist(){
        if (rightChild == nullptr) return false;
        else return true;
    }
    Node<T>* getParent(){
        return parent;
    }
    bool parentExist(){
        if (parent == nullptr) return false;
        else return true;
    }
    void setData(T* newData){
        data = newData;
    }
    void setLeftChild(Node<T>* newLeftChild){
        leftChild = newLeftChild;
    }
    void setRightChild(Node<T>* newRightChild){
        rightChild = newRightChild;
    }
    void setParent(Node<T>* newParent){
        parent = newParent;
    }
    void print(){
        data->print();
    }
    bool compare(T *comparison){
        if (data->compare(comparison)) return true;
        else return false;
    }
};

template <typename T> class BinaryHeap{
private:
    Node<T>* root;
    Node<T>* tail;  // yes, it's a tail
    int numberOfElements;
    int height;
public:
    BinaryHeap(T* data){
        Node<T>* newNode = new Node<T>(data);
        root = newNode;
        tail = newNode;
        numberOfElements = 1;
        height = 1;
    }
    BinaryHeap(){
        root = nullptr;
        tail = nullptr;
        numberOfElements = 0;
        height = 1;
    }
    Node<T>* getRoot(){
        return root;
    }
    Node<T>* getTail(){
        return tail;
    }
    int getNumberOfElements(){
        return numberOfElements;
    }
    int getHeight(){
        return height;
    }
    T* getMin(){
        return getRoot()->getData();
    }
    void setRoot(Node<T>* newRoot){
        root = newRoot;
    }
    void setTail(Node<T>* newTail){
        tail = newTail;
    }
    void setNumberOfElements(int num){
        numberOfElements = num;
    }
    void incrementNumberOfElements(){
        numberOfElements = numberOfElements + 1;
    }
    void decrementNumberOfElements(){
        numberOfElements = numberOfElements - 1;
    }

    void setHeight(int num){
        height = num;
    }
    bool compare(Node<T> *node1, Node<T> *node2){       // (1 > 2) -> TRUE
        if (node1->compare(node2->getData())) return true;
        else return false;
    }

    void insertElement(T* data) {
        // Inserted node
        Node<T>* newNode = new Node<T>(data);

        // Edge case for absence of root
        if (getNumberOfElements() == 0){
            setRoot(newNode);
            setTail(newNode);
            incrementNumberOfElements();
            return;
        }

        Node<T>* temp = getTail();
        T* swapHelper;
        bool leftChildReached;
        bool insertRight = false;

        // Insert near root
        if (getNumberOfElements() == 1){                // Edge case for first insert
            temp->setLeftChild(newNode);
            newNode->setParent(temp);
            incrementNumberOfElements();
            setTail(newNode);

        } else if (getNumberOfElements() == 2){         // Edge case for second insert
            getRoot()->setRightChild(newNode);
            newNode->setParent(getRoot());
            incrementNumberOfElements();
            setTail(newNode);

        } else {

            // Insert at tail position + 1

            // Go up until temp is a left child or the root
            for (int i = 0; i <= VERTEXCOUNT; ++i) {
                if (temp == getRoot()) {                                      // temp ran into root ; pivot left down
                    leftChildReached = false;
                    break;
                }
                if (temp->getParent()->leftExist()) {
                    if (temp->getParent()->getLeftChild() == temp) {     // temp is a left child ; pivot right down
                        leftChildReached = true;
                        temp = temp->getParent();
                        break;
                    }
                }
                temp = temp->getParent();                               // None found ; continue traverse up
            }
            // If left child reached, go to right child
            if (leftChildReached) {
                // If no right child exists, insert directly there
                if (!temp->rightExist()) insertRight = true;
                else {
                    // If not, move to the right child
                    temp = temp->getRightChild();
                    // Go down left until leaf is reached
                    while (temp->leftExist())  temp = temp->getLeftChild();
                }
            } else {
                // Go down left until leaf is reached (without traversing right first)
                while (temp->leftExist())  temp = temp->getLeftChild();
            }
            // Empty slot reached ; insert!
            if (insertRight) temp->setRightChild(newNode);
            else temp->setLeftChild(newNode);

            newNode->setParent(temp);

            incrementNumberOfElements();
            setTail(newNode);

        }

        // Upheap

        Node<T>* parent = newNode->getParent();
        // While newNode's parent > newNode,...
        while (compare(parent, newNode)){

            // Swap the data of parent and child
            swapHelper = newNode->getData();
            newNode->setData(parent->getData());
            parent->setData(swapHelper);

            // If newNode has no parent (is the root), avoid 139
            if (!parent->parentExist()) return;

            // Move up pointers to prepare for next loop
            newNode = newNode->getParent();
            parent = parent->getParent();
        }
    }

    T* deleteMin(){

        // Root's data is saved, gets returned later
        T* deletedData = getRoot()->getData();
        //cout << "Deleting: ";
        //deletedData->print();
        // Tail reference is saved to delete later
        Node<T>* nodeToDelete = getTail();

        // If root is only remaining node, delete it
        if (getNumberOfElements() == 1){
            delete getRoot();
            return deletedData;
        }
        // Last inserted node
        Node<T>* temp = getTail();

        // Data from last node transfers to root
        getRoot()->setData(temp->getData());

        // Downheap
        Node<T>* base = getRoot();
        T* swapHelper;
        bool leftLess;
        bool rightLess;
        // While newNode's parent > newNode,...
        while (true){
            leftLess = false;
            rightLess = false;

            if (base->leftExist()){    // Base has a lesser left child?
                if (compare(base, base->getLeftChild())) leftLess = true;
            }
            if (base->rightExist()){   // Base has a lesser right child?
                if (compare(base, base->getRightChild())) rightLess = true;
            }
            if (leftLess && rightLess){ // Base has both, must compare to find swap
                if (compare(base->getRightChild(), base->getLeftChild())){  // R > L
                    rightLess = false;
                } else {    // If one is greater than the other, their LESSthan flag turns off
                    leftLess = false;
                }
            }

            // Only one LESSthan flag should be activated
            // Swap with left
            if (leftLess){
                swapHelper = base->getData();
                base->setData(base->getLeftChild()->getData());
                base->getLeftChild()->setData(swapHelper);
                // Prepare for next loop
                base = base->getLeftChild();
            }
            // Swap with right
            if (rightLess){
                swapHelper = base->getData();
                base->setData(base->getRightChild()->getData());
                base->getRightChild()->setData(swapHelper);
                // Prepare for next loop
                base = base->getRightChild();
            }
            // Base is less than both its children : heap property satisfied
            if (!leftLess && !rightLess) break;
        }


        // Update Tail for next insertion/deletion
        for (int i = 0; i <= VERTEXCOUNT; ++i) {
            if (temp == getRoot()) {                                      // temp ran into root ; pivot right down
                break;
            }
            if (temp->getParent()->rightExist()) {
                if (temp->getParent()->getRightChild() == temp) {     // temp is a left child ; pivot right down
                    // If right child reached, move to its sibling
                    temp = temp->getParent();
                    temp = temp->getLeftChild();
                    break;
                }
            }
            temp = temp->getParent();                               // None found ; continue traverse up
        }

        // Go down left until leaf is reached (without traversing right first)
        while (temp->rightExist())  temp = temp->getRightChild();

        // Empty slot reached ; update tail
        decrementNumberOfElements();
        setTail(temp);

        // Delete previous tail node
        temp = nodeToDelete->getParent();
        if (temp->getLeftChild() == nodeToDelete){
            // toDelete is a left child
            temp->setLeftChild(nullptr);
        } else {
            // toDelete is a right child
            temp->setRightChild(nullptr);
        }
        delete nodeToDelete;

        return deletedData;
    }

    void print(){
        printPreorder(root);
        cout << endl;
    }
    void printPreorder(Node<T> *temp){
        if (temp == nullptr) return;

        temp->print();                              // Preorder, so visit root first
        /*
        if (temp->leftExist()) {
            cout << " --- LC:";
            temp->getLeftChild()->print();
        }
        if (temp->rightExist()){
            cout << " ---- RC: ";
            temp->getRightChild()->print();
        }
         */
        cout << endl;

        printPreorder(temp->getLeftChild());  // then left subtree
        printPreorder(temp->getRightChild()); // then right subtree
    }
};

void runPrims(int G [VERTEXCOUNT][VERTEXCOUNT], BinaryHeap<Data>* heap){
    bool visited[VERTEXCOUNT] = {false, false, false, false, false};

    // Visit minimum vertex first
    Data* curr = heap->getMin();
    visited[curr->getSource()] = true;

    while (heap->getNumberOfElements() > 1){
        curr = heap->deleteMin();
        // v1 is visited AND v2 is not visited
        if (visited[curr->getSource()] && !visited[curr->getDestination()]){
            // visit v2 and print edge
            visited[curr->getDestination()] = true;
            curr->print();
            cout << endl;
        }
    }

}

int main() {

    int G[VERTEXCOUNT][VERTEXCOUNT] = {{0,  3,  65, 0,  0},
                   {3,  0,  85, 20, 45},
                   {65, 85, 0,  41, 77},
                   {0,  20, 41, 0,  51},
                   {0,  45, 77, 51, 0}};
    BinaryHeap<Data>* heap = new BinaryHeap<Data>();
    Data* newData;

    // Load every non-zero value from array into the min heap
    for (int i = 0; i < VERTEXCOUNT - 1; ++i) {
        for (int j = 0; j < VERTEXCOUNT; ++j) {
            if (G[i][j] != 0){
                newData = new Data(i,j,G[i][j]);
                heap->insertElement(newData);
            }
        }
    }

    runPrims(G, heap);

    return 0;
}
