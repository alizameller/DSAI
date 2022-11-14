//
// Created by Aliza Meller on 11/3/21.
//
#include <iostream>
#include <new>
#include <fstream>
#include <list>
#include <cstring>
using namespace std;

template <typename T> // template for different types (int, double, string)
class SimpleList { // abstract base class for stacks and queues
private:
    class Node { // private nested class for Nodes containing data and a pointer to next node
    private:
        T data;
        Node *next;

    public:
        //data and next setters
        void setData(T val) {
            data = val;
        }

        void setNext(Node* ptr) {
            next = ptr;
        }

        //data and next getters
        T getData() const {
            return data;
        }

        Node* getNext() const {
            return next;
        }

        Node(T x, Node* y) { // Node constructor to initialize values for data and next
            data = x;
            next = y;
        }
    };

    string name;
    Node* head;
    Node* tail;
    int size;

protected:
    virtual void insertStartNode(T val) { // function to push to a stack
        Node* startNode = new Node(val, head);
        head = startNode;
        if (size == 0) {  // edge case where node is the first node
            tail = head;
        }
        size = size + 1;
        return;
    }

    virtual void insertEndNode(T val) { // function to push to a queue
        Node* endNode = new Node(val, NULL);
        if (!size++) { //more concise version of first node case
             head = tail = endNode;
             return;
         }
        tail->setNext(endNode);
        tail = endNode;
        return;
    };

    virtual Node removeStartNode() { // function to pop from either stack or queue
        Node removedNode = *head;
        head = removedNode.getNext();
        size = size - 1;
        return removedNode;
    }

public:
    string getName() const { //member function to get name of a SimpleList because member is private
        return name;
    };

    int getSize() const { //member function to get size of a SimpleList because member is private
        return size;
    };
    // public pure virtual member functions for push and pop because stacks and queues differ
    virtual void push(T val) = 0;
    virtual T pop() = 0;

    SimpleList(string listName) { //SimpleList constructor
        name = listName;
        head = NULL;
        tail = NULL;
        size = 0;
    }
}; //end of class declaration for SimpleList

template <typename T>
class Stack:public SimpleList<T> { // Stack derived class
public:
    Stack(string listName): SimpleList<T>(listName){

    }; // Stack constructor
    void push(T val) { //calling push
        this->insertStartNode(val); // this = refer current class instance variable
    };
    T pop(){ //calling pop
        return this->removeStartNode().getData();
    };
};

template <typename T>
class Queue:public SimpleList<T> {// Queue derived class
public:
    Queue(string listName): SimpleList<T>(listName){ // Queue constructor

    };
    Queue queue(string listName); //calling push
    void push(T val) {
        this->insertEndNode(val);
    };
    T pop(){ //calling pop
        return this->removeStartNode().getData();
    };
};

template <typename T>
SimpleList<T>* findList(char* listName, list<SimpleList<T> *> masterList) { // function to check if list exists by searching the masterList
    for(auto const& i : masterList) {
        if (i->getName() == listName){
            return i;
        }
    }
    return NULL;
};

template <typename T>
void createList(char* listName, list<SimpleList<T> *>* pmasterList, char* thirdWord, ofstream* outFile) { //function to create a list
    SimpleList<T>* pList;
    pList = findList(listName, *pmasterList); // Calling findList and retruning a pointer to the existing list
    if (pList) { //check if list called listName exists
        *outFile << "ERROR: This name already exists!" << endl;
        return;
    }
    SimpleList<T>* newList; // if list does not exist, create a Stack or Queue object of the respective type
    if (!strcmp(thirdWord, "stack")){
        newList = new Stack<T> (listName);
    } else if (!strcmp(thirdWord, "queue")){
        newList = new Queue<T> (listName);
    }
    pmasterList->push_back(newList); //add a new element (newList) to masterList

    return;
}

template <typename T>
void pushList(char* listName, list<SimpleList<T> *>* pmasterList, T thirdWord, ofstream* outFile) { //function to push to a list
    SimpleList<T>* pList;
    pList = findList(listName, *pmasterList);
    if (!pList) { // check if list exists
        *outFile << "ERROR: This name does not exist!" << endl;
        return;
    }
    pList->push(thirdWord); //if list exists, push to list

    return;
}

template <typename T>
void popList(char* listName, list<SimpleList<T> *>* pmasterList, ofstream* outFile) { // function to pop from list
    SimpleList<T>* pList;
    pList = findList(listName, *pmasterList);
    if (!pList) { //check if list exists
        *outFile << "ERROR: This name does not exist!" << endl;
        return ;
    }

    if (!(pList->getSize())) { //check if list is empty
        *outFile << "ERROR: This list is empty!" << endl;
        return;
    }
    T val;
    val = pList->pop(); // if list exists and not empty, pop from list
    *outFile << "Value popped: " << val << endl;
    return;
}

int main () {
    ifstream inFile; //read inFile
    ofstream outFile; //write outFile
    string inputFileName;
    string outputFileName;

    cout << "Enter name of input file: ";
    cin >> inputFileName;
    cout << "Enter name of output file: ";
    cin >> outputFileName;
    //open both files
    inFile.open(inputFileName);
    outFile.open(outputFileName);

    string line;
    char *commandWord;
    char *listName;
    char *thirdWord;

    list<SimpleList<int> *> listSLi; //list of pointers to all int Stacks and Queues
    list<SimpleList<double> *> listSLd; //list of pointers to all double Stacks and Queues
    list<SimpleList<string> *> listSLs; //list of pointers to all string Stacks and Queues

    int iv; //int variable
    double dv; //double variable
    string sv; //string variable

    while (getline(inFile, line)) { // use getLine function to parse through the inFile line by line
        outFile << "PROCESSING COMMAND: " << line << endl;
        commandWord = strtok(strdup(line.c_str()), " "); //convert string to array of chars and set first word = commandWord
        listName = strtok(NULL, " "); //get second word as listName
        thirdWord = strtok(NULL, " "); //get third word as thirdWord (could be NULL, could be value to push)

        if (!strcmp(commandWord, "create")) { //compare commandWord to "create"
            if (listName[0] == 'i') { //get first char from listName char array to determine the type
                createList(listName, &listSLi, thirdWord, &outFile); //call create list and pass in the list of all int Stacks and Queues
            } else if (listName[0] == 'd') {
                createList(listName, &listSLd, thirdWord, &outFile); //call create list and pass in the list of all double Stacks and Queues
            } else if (listName[0] == 's') {
                createList(listName, &listSLs, thirdWord, &outFile); //call create list and pass in the list of all string Stacks and Queues
            }

        } else if (!strcmp(commandWord, "push")) { //compare commandWord to "push"
            if (listName[0] == 'i') {
                iv = atoi(thirdWord); //string to int
                pushList(listName, &listSLi, iv, &outFile);
            } else if (listName[0] == 'd') {
                dv = atof(thirdWord); //string to float (double)
                pushList(listName, &listSLd, dv, &outFile);
            } else if (listName[0] == 's') {
                sv = string(thirdWord); //keep as string
                pushList(listName, &listSLs, sv, &outFile);
            }

        } else if (!strcmp(commandWord, "pop")) { //compare commandWord to "pop"
            if (listName[0] == 'i') {
                popList(listName, &listSLi, &outFile);
            } else if (listName[0] == 'd') {
                popList(listName, &listSLd, &outFile);
            } else if (listName[0] == 's') {
                popList(listName, &listSLs, &outFile);
            }
        }
    }
    outFile.close(); //close the outFile
    return 0;
};