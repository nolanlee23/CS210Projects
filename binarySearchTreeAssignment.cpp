#include <iostream>
#include <vector>

using namespace std;

class Data {
private:
    int intValue;
public:
    Data(int intValue){
        this->intValue = intValue;
    }
    bool compare(Data *comparison){
        if (intValue > comparison->getValue()) return true;
        else return false;
    }
    bool isEqual(Data *comparison){
        if (intValue == comparison->getValue()) return true;
        else return false;
    }
    int getValue(){
        return intValue;
    }
    void setValue(int newValue){
        intValue = newValue;
    }
    void print(){
        cout << intValue;
    }
};

template <typename T> class Node {
private:
    T *data;
    Node<T> *leftChild;
    Node<T> *rightChild;
    Node<T> *parent;
public:
    Node(T *data){
        this->data = data;
        leftChild = nullptr;
        rightChild = nullptr;
        parent = nullptr;
    }
    bool compare(T *comparison){
        if (data->compare(comparison)) return true;
        else return false;
    }
    bool isEqual(T *comparison){
        if (data->isEqual(comparison)) return true;
        else return false;
    }
    T *getData(){
        return data;
    }
    Node<T> *getLeftChild(){
        return leftChild;
    }
    Node<T> *getRightChild(){
        return rightChild;
    }
    Node<T> *getParent(){
        return parent;
    }
    void setData(T *newData){
        data = newData;
    }
    void setLeftChild(Node<T> *newLeftChild){
        leftChild = newLeftChild;
    }
    void setRightChild(Node<T> *newRightChild){
        rightChild = newRightChild;
    }
    void setParent(Node<T> *newParent){
        parent = newParent;
    }
    void print(){
        data->print();
    }
};

template <typename T> class BinarySearchTree {
private:
    Node<T> *root;
    vector<Node<T> > collection;
    int numberOfElements;
    int height;
    int printCounter;
    bool exists;
public:
    BinarySearchTree(T *value){
        Node<T> *newNode = new Node<T>(value);
        root = newNode;
        numberOfElements = 1;
        height = 1;
    }
    int getNumberOfElements(){
        return numberOfElements;
    }
    int getHeight(){
        return height;
    }
    bool compare(Node<T> *node1, Node<T> *node2){
        if (node1->compare(node2->getData())) return true;
        else return false;
    }
    bool isEqual(Node<T> *node1, Node<T> *node2){
        if (node1->isEqual(node2->getData())) return true;
        else return false;
    }
    void insertElement(T *newData){
        Node<T> *newNode = new Node<T>(newData);
        if (root == nullptr){       // If no root exists, insert at root
            root = newNode;
            updateHeight(0);
            return;
        }
        Node<T> *temp = root;

        for (int i = 1; i < height + 3; ++i) {
            if (compare(newNode, temp)){           // (newNode > temp)
                if (temp->getRightChild() == nullptr){          // greater than temp and no right child
                    temp->setRightChild(newNode);  // place newNode under temp
                    newNode->setParent(temp);
                    updateHeight(i);
                    break;
                } else {                                        // right child exists, traverse down to it
                    temp = temp->getRightChild();               // repeat but with the right child
                    continue;
                }
            } else {  //(newNode < temp)                         // less than temp and no left child
                if (temp->getLeftChild() == nullptr){           // place newNode under temp
                    temp->setLeftChild(newNode);
                    newNode->setParent(temp);
                    updateHeight(i);
                    break;
                } else {                                        // left child exists, traverse down to it
                    temp = temp->getLeftChild();                // repeat but with the left child
                    continue;
                }
            }
        }
    }
    void updateHeight(int insertedHeight){  // helper method for insert, uses loop counter from tree traversal
        if (insertedHeight > height){       //    to get the new height for comparison
            height = insertedHeight;
        }
        numberOfElements++;
    }

    void findSmallest(){
        Node<T> *minimum = new Node<T>(root->getData());
        findEst(root, minimum,0);
        minimum->print();
        cout << endl;
    }
    void findBiggest(){
        Node<T> *maximum = new Node<T>(root->getData());
        findEst(root, maximum,1);
        maximum->print();
        cout << endl;
    }
    void findEst(Node<T> *temp, Node<T> *maxOrMin, bool op){
        if (temp == nullptr) return;
        if (op == 1) {
            if (compare(temp, maxOrMin)){               // Finding max, update maxOrMin when temp is greater
                maxOrMin->setData(temp->getData());
            }
        } else {
            if (compare(maxOrMin, temp)){               // Finding min, update maxOrMin when temp is less than
                maxOrMin->setData(temp->getData());
            }
        }
        findEst(temp->getLeftChild(), maxOrMin, op);            // Preorder traversal
        findEst(temp->getRightChild(), maxOrMin, op);
    }

    void findKthElement(int k){
        if (k > numberOfElements){
            cout << "Number does not exist\n";
            return;
        }
        collection.clear();
        collection.push_back(*root);
        updateList(root);
        collection.at(k-1).print();
        cout << endl;
    }
    void sortAscending(){
        collection.clear();
        printCounter = 0;
        collection.push_back(*root);
        updateList(root);
        for (auto element : collection){
            element.print();
            printCounter++;
            if (printCounter < numberOfElements){
                cout << ", ";
            }
        }
        cout << endl;
    }
    void sortDescending(){
        collection.clear();
        printCounter = 0;
        collection.push_back(*root);
        updateList(root);
        for (int i = collection.size() - 1; i >= 0; i--) {
            collection[i].print();
            printCounter++;
            if (printCounter < numberOfElements){
                cout << ", ";
            }
        }
        cout << endl;
    }
    void updateList(Node<T> *temp){
        if (temp == nullptr) return;
        updateList(temp->getLeftChild());       // Post order traversal
        updateList(temp->getRightChild());

        for (int i = 0; i <= numberOfElements; i++) {                                   // Iterate through every element in list so far
            if (compare(temp, &collection.at(collection.size()-1))){      // If temp is greater than maximum value, place at end
                collection.push_back(*temp);
                return;
            }
            if (compare(&collection.at(i), temp)){                        // If not, insert when we reach a larger number than temp
                if (isEqual(temp, root)) return;                          // Edge case to avoid placing root twice
                collection.insert((collection.begin() + i), *temp);
                return;
            }
        }
        collection.push_back(*temp);        // Fallback
    };

    void contains(Node<T> *target, Node<T> *temp){
        if (temp == nullptr) return;
        if (isEqual(target, temp)){
            exists = true;
        }
        contains(target, temp->getLeftChild());
        contains(target, temp->getRightChild());
    }
    // Finds minimum in the given subtree, used for finding successor
    Node<T> *findMin(Node<T> *temp){
        while (temp != nullptr){
            if (temp->getLeftChild() == nullptr) return temp;
            temp = temp->getLeftChild();
        }
        return temp;
    }
    // Finds maximum in the given subtree, used for finding successor
    Node<T> *findMax(Node<T> *temp){
        while (temp != nullptr){
            if (temp->getRightChild() == nullptr) return temp;
            temp = temp->getRightChild();
        }
        return temp;
    }
    // Helper method for recursion
    void deleteElement(T *data){
        Node<T> *toDelete = new Node<T>(data);
        exists = false;
        contains(toDelete, root);
        if (!exists){
            cout << "Number does not exist" << endl;
        }
        if (isEqual(root, toDelete)){       // Edge case for deleting root
            // Two children
            if (root->getLeftChild() != nullptr && root->getRightChild() != nullptr){
                // Find successor
                Node<T> *successor;
                if (root->getLeftChild() != nullptr) successor = findMax(root->getLeftChild()); // Preferred is max of left subtree
                else successor = findMin(root->getRightChild());                                // If doesn't exist, then min of right
                // Make successors parent point to null
                if (isEqual(successor->getParent()->getRightChild(), successor)) {      // Successor is a right child
                    successor->getParent()->setRightChild(nullptr);
                } else {                                                                        // Successor is a left child
                    successor->getParent()->setLeftChild(nullptr);
                }
                // Update parent
                successor->setParent(nullptr);
                // Assign temps dependencies to successor
                if (root->getRightChild() != nullptr) {
                    successor->setRightChild(root->getRightChild());
                    root->getRightChild()->setParent(successor);
                }
                if (root->getLeftChild() != nullptr) {
                    successor->setLeftChild(root->getLeftChild());
                    root->getRightChild()->setParent(successor);
                }
                // successor becomes the new root
                toDelete = root;
                root = successor;
                delete toDelete;
                numberOfElements--;
                return;
            }
            // Right child
            if (root->getLeftChild() == nullptr){
                root->getRightChild()->setParent(nullptr);
                toDelete = root;
                root = root->getRightChild();
                delete toDelete;
                numberOfElements--;
                return;
            }
            // Left child
            if (root->getRightChild() == nullptr){
                root->getLeftChild()->setParent(nullptr);
                toDelete = root;
                root = root->getLeftChild();
                delete toDelete;
                numberOfElements--;
                return;
            }
        }
        // toDelete is not root, move to recurse down the tree
        deleteRecursive(root, toDelete);
    }
    void deleteRecursive(Node<T> *temp, Node<T> *toDelete){
        if (temp == nullptr) return;
        if (isEqual(temp, toDelete)){                         // Found the matching node to delete
            Node<T> *parent = temp->getParent();
            // No children
            if (temp->getLeftChild() == nullptr && temp->getRightChild() == nullptr){
                if (parent->getRightChild() != nullptr){
                    if (isEqual(parent->getRightChild(), temp)){             // Deleted node is a right child
                        parent->setRightChild(nullptr);
                    } else {                                                                // Deleted node is a left child
                        parent->setLeftChild(nullptr);
                    }
                } else {
                    parent->setLeftChild(nullptr);      // Right node doesn't exist, must be left
                }
                delete temp;
                numberOfElements--;
                return;
            }
            // One right child
            if (temp->getLeftChild() == nullptr){
                temp->getRightChild()->setParent(temp->getParent());
                if (isEqual(parent->getRightChild(), temp)) {      // This node is a right child
                    parent->setRightChild(temp->getRightChild());
                } else {                                                          // This node is a left child
                    parent->setLeftChild(temp->getRightChild());
                }
                delete temp;
                numberOfElements--;
                return;
            }
            // One left child
            if (temp->getRightChild() == nullptr){                   // Has one left child
                temp->getLeftChild()->setParent(temp->getParent());
                if (isEqual(parent->getRightChild(), temp)) {      // This node is a right child
                    parent->setRightChild(temp->getLeftChild());
                } else {                                                          // This node is a left child
                    parent->setLeftChild(temp->getLeftChild());
                }
                delete temp;
                numberOfElements--;
                return;
            }
            // 2 Children
            // Find successor
            Node<T> *successor;
            if (temp->getLeftChild() != nullptr) successor = findMax(temp->getLeftChild());
            else successor = findMin(temp->getRightChild());
            // Make temps parent point to successor
            if (isEqual(temp->getParent()->getRightChild(), temp)) {      // Temp is a right child
                temp->getParent()->setRightChild(successor);
            } else {                                                                        // Temp is a left child
                temp->getParent()->setLeftChild(successor);
            }
            // Make successors parent point to null
            if (isEqual(successor->getParent()->getRightChild(), successor)) {      // Successor is a right child
                successor->getParent()->setRightChild(nullptr);
            } else {                                                                        // Successor is a left child
                successor->getParent()->setLeftChild(nullptr);
            }
            // Update parents
            successor->setParent(temp->getParent());
            // Assign temps dependencies to successor
            if (temp->getRightChild() != nullptr) successor->setRightChild(temp->getRightChild());
            if (temp->getLeftChild() != nullptr) successor->setLeftChild(temp->getLeftChild());
            delete temp;
            numberOfElements--;
            return;

        }
        deleteRecursive(temp->getLeftChild(), toDelete);
        deleteRecursive(temp->getRightChild(), toDelete);
    }

    void print(){
        printCounter = 0;       // Used for knowing when to avoid printing extra comma
        printPreorder(root);
    }
    void printPreorder(Node<T> *temp){
        if (temp == nullptr) return;

        temp->print();                              // Preorder, so visit root first,...
        printCounter++;
        if (printCounter < numberOfElements) cout << ", ";
        else cout << endl;

        printPreorder(temp->getLeftChild());  // then left subtree,...
        printPreorder(temp->getRightChild()); // then right subtree.
    }
};

int main() {
    int a[] = {10,45,23,67,89,34,12,99};
    Data* newData = new Data(a[0]);
    BinarySearchTree<Data>* newBST = new BinarySearchTree<Data>(newData);
    for(int i=1;i< (sizeof(a)/sizeof(int));i++)
    {
        newData = new Data(a[i]);
        newBST->insertElement(newData);
    }
    // Assignment requirements checklist
    newBST->print();
    newBST->findSmallest();
    newBST->findBiggest();
    newData = new Data(10);
    newBST->deleteElement(newData); // delete root
    newBST->print();
    newData = new Data(45);
    newBST->deleteElement(newData); //delete with two children
    newBST->print();
    newData = new Data(12);
    newBST->deleteElement(newData); //delete with one child
    newBST->print();
    newData = new Data(10);
    newBST->deleteElement(newData); // delete a number that doesnt exist. What will you print?
    newBST->print();
    newBST->findKthElement(1); //first element
    newBST->findKthElement(newBST->getNumberOfElements()); //last element
    newBST->findKthElement(3); // some element in between
    newBST->findKthElement(7); // Edge case where item does not exist. What will you print?
    newBST->findSmallest();
    newBST->findBiggest();
    newBST->sortAscending();
    newBST->sortDescending();
    return 0;
}
