/*
Ray Ramadhar
ECE 264
Programming assignment 2

This program recieves a text file of commands and creates new lists, push, and pop data from the lists.
It uses a SImpleList abstract base class and stack queue derived class, and input and output function
to accept input file and write to output file, a search and parse function, and the main function.

*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <list>
using namespace std;

///////////////////////////////////////////////Base Class//////////////////////////////////////////////////

template <typename T>
class SimpleList
// Abstract base class called SimpleList that provides some
// aspects of singly linked list functionality
// Base class for stacks and queues
{
private:
// Nested node class for the singly linked list
    class Node
    {
    public:
        T data;
        Node *next;
        // Constructor to initialize node with data and next pointer
        Node(T value, Node *nextptr = nullptr):
            data(value), next(nextptr)
            {}
    };

    string listname; 
    Node *head;
    Node *tail;

public:
    //Constructors initalizing listname and *head
    SimpleList(const string &ln, Node *headptr = nullptr, Node *tailptr = nullptr):
        listname(ln), head(headptr), tail(tailptr)
        {}

    //member function to retrieve private name
    std::string getName() const 
    {
        return listname;
    };

    //Pure virtual pop and push
    virtual T  pop() = 0; 

    virtual void  push(T data) = 0; 

protected:
    //Ability to insert a node at the start of the list
    void insertAtFront(T newdata)
    {
        //Prepare new node
        Node *newnode = new Node(newdata);
        //Check for empty list then for non-empty list
        if (head == nullptr)
        {
            head = newnode;
            tail = newnode;
            return;
        }
        //When list is not empty
            newnode -> data = newdata;
            //Put it in front of current head
            newnode -> next = head;
            //Move head to point to new node
            head = newnode;
    };

    void insertAtEnd(T newdata)
    //Insert a node at the end of the list
    {
        //Prepare new node
        Node *newnode = new Node(newdata);
        newnode -> data= newdata;
        newnode -> next = NULL;
        //If Linked list empty, new node is head node else tail
        if (head == nullptr)
        {
            head = newnode;
            tail = newnode;
            return;
        }
        //When list is not empty
            tail -> next = newnode;
            tail = newnode;
    };

    //Provide the ability to remove a node at the start of the list
    T removeAtFront()
    {
        if (head == nullptr)
        {
            throw std::runtime_error("empty"); // signal that the list is empty
        }
        Node *temp = head;
        T valuepop = temp -> data;
        head = head -> next;
        delete temp;

        if (head == nullptr)
        tail = nullptr;

        return valuepop;
    };
};

///////////////////////////////////////////////Derived Classes//////////////////////////////////////////////////
template <typename T>
class Stack: public SimpleList<T>
//Stack push/pop both use the front of the list
{
    public:
        T pop() override
            {
            return this ->removeAtFront();
            }
        virtual void push(T data) override
            {
            this -> insertAtFront(data);
            }
        //Constructors which should accept the name of the stack
        Stack(const string &stackname):
        SimpleList<T>(stackname)
        {}
    
}; 

template <typename T>
class Queue: public SimpleList<T>
//Queue implementation push inserts at end and pop removes from front
{
    public:
        T pop() override
            {
            return this ->removeAtFront();
            }
        virtual void push(T data) override
            {
            this -> insertAtEnd(data);
            }
    Queue(const string &queuename):
        SimpleList<T>(queuename)
        {}
};

///////////////////////////////////////////////Functions//////////////////////////////////////////////////

//Open the Input File
void input(ifstream &inputFile)
{
    std::string commandfile;
    std::cout << "Enter the name of the input text file: ";
    std::cin >> commandfile;

    inputFile.open(commandfile);
    if (!inputFile)
        cerr << "Error: Could not open the file " << commandfile << endl;
};

//Open the Output File
void output(ofstream &outputFile)
{
    std::string filename;
    std::cout << "Enter the name of the Output text file: ";
    std::cin >> filename;

    outputFile.open(filename);
    if (!outputFile)
        cerr << "Error: Could not open the file " << filename << endl;
};

//Template function to search for a SimpleList with a specified name
template <typename T>
SimpleList<T>* search(list<SimpleList<T>*> &container, const string &name)
{
    for (SimpleList<T>* item: container)
    {
        if (item -> getName() == name)
        {
            return item;
        }
    }
    return nullptr;
};

//Large function to parse the input file and process all the commands
void parse(list<SimpleList<int>*> &listSLi,
    list<SimpleList<double>*> &listSLd, 
    list<SimpleList<string>*> &listSLs)
{
    ifstream inputFile;
    ofstream outputFile;
    input(inputFile);
    output(outputFile);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cerr << "Error: Could not open one or both files "  << std::endl;
        return;
    }

    if (inputFile.is_open())
    {
        std::string line;
        while(getline(inputFile, line))
        {
            outputFile << "PROCESSING COMMAND: "<< line << std::endl;
            
            std::stringstream ss(line);
            string command, name, typeORvalue;
            ss >> command >> name;
            if (command != "pop")
                ss >> typeORvalue;

// 'create' allocates a new stack or queue of the right type
            if (command == "create")
            {
                if (name[0] == 'i')
                {
                    if (search(listSLi, name))
                    {
                        outputFile << "ERROR: This name already exists!" << std::endl;
                    }
                    else
                    {
                        SimpleList<int>* listPtr1;
                        if (typeORvalue == "stack")
                        {
                            listPtr1 = new Stack<int>(name);
                        }
                        else
                        {
                            listPtr1 = new Queue<int>(name);
                        }
                        listSLi.push_front(listPtr1);
                    }
                }
                else if (name[0] == 'd')
                {
                    if (search(listSLd, name))
                    {
                        outputFile << "ERROR: This name already exists!" << std::endl;
                    }
                    else
                    {
                        SimpleList<double>* listPtr2;
                        if (typeORvalue == "stack")
                        {
                            listPtr2 = new Stack<double>(name);
                        }
                        else
                        {
                            listPtr2 = new Queue<double>(name);
                        }
                        listSLd.push_front(listPtr2);
                    }
                }
                else if (name[0] == 's')
                {
                    if (search(listSLs, name))
                    {
                        outputFile << "ERROR: This name already exists!" << std::endl;
                    }
                    else
                    {
                        SimpleList<string>* listPtr3;
                        if (typeORvalue == "stack")
                        {
                            listPtr3 = new Stack<string>(name);
                        }
                        else
                        {
                            listPtr3 = new Queue<string>(name);
                        }
                        listSLs.push_front(listPtr3);
                    }
                }
            }
    
// 'push' convert value to correct type and insert into whichever was the chosen list        
            else if (command == "push") 
            {
                if (name[0] == 'i')
                {
                    SimpleList<int>* listPtr1 = search(listSLi, name);
                    if (!listPtr1)
                    {
                        outputFile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else
                    {
                        listPtr1 -> push(stoi(typeORvalue));
                    }
                }
                else if (name[0] == 'd')
                {
                    SimpleList<double>* listPtr2 = search(listSLd, name);
                    if (!listPtr2)
                    {
                        outputFile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else
                    {
                        listPtr2 -> push(stod(typeORvalue));
                    }
                }
                else if (name[0] == 's')
                {
                    SimpleList<string>* listPtr3 = search(listSLs, name);
                    if (!listPtr3)
                    {
                        outputFile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else
                    {
                        listPtr3 -> push(typeORvalue);
                    }
                }
                
            }

//'pop' removes value from list and print it or print empty-list error
            else if (command == "pop")
            {
                if (name[0] == 'i')
                {
                    SimpleList<int>* listPtr1 = search(listSLi, name);
                    if (!listPtr1)
                    {
                        outputFile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else
                    {
                        try
                        {
                        int popi = listPtr1 -> pop();
                        outputFile << "Value popped: " << popi << std::endl;
                        }
                        catch (const std::runtime_error&)
                        {
                            outputFile << "ERROR: This list is empty!" << std::endl;
                        }
                    }
                }
            

                else if (name[0] == 'd')
                {
                    SimpleList<double>* listPtr2 = search(listSLd, name);
                    if (!listPtr2)
                    {
                        outputFile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else
                    {
                        try
                        {
                        double popd = listPtr2 -> pop();
                        outputFile << "Value popped: " << popd << std::endl;
                        }
                        catch (const std::runtime_error&)
                        {
                            outputFile << "ERROR: This list is empty!" << std::endl;
                        }
                    }
                }
    
                else if (name[0] == 's')
                {
                    SimpleList<string>* listPtr3 = search(listSLs, name);
                    if (!listPtr3)
                    {
                        outputFile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else
                    {
                        try
                        {
                        string pops = listPtr3 -> pop();
                        outputFile << "Value popped: " << pops << std::endl;
                        }
                        catch (const std::runtime_error&)
                        {
                            outputFile << "ERROR: This list is empty!" << std::endl;
                        }
                    }
                }
            }
        }
    }
};



int main()
{
    list<SimpleList<int> *> listSLi; // all integer stacks and queues 
    list<SimpleList<double> *> listSLd; // all double stacks and queues 
    list<SimpleList<string> *> listSLs; // all string stacks and queues 
    parse(listSLi, listSLd, listSLs);

    //Clean up all the dynamically allocated lists that were created during parsing
    for (auto p : listSLi) delete p;
    for (auto p : listSLd) delete p;
    for (auto p : listSLs) delete p;
};
