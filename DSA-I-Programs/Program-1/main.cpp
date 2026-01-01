/*
Ray Ramadhar
Professor Sable
DSA 1
Fall 2025

This program creates 1 base class foobar, and 2 derived classes
foo and bar. They all have the same definition of position, but
their strength differs based on the object and position. Therefore, 
the classes  have public and private member functions, and 2 private 
members, position and name, as well as a strength override in foo and bar
The goal is to have the program read user inputted file with lines 
of object type : name, i.e foo R2D2, and use getline and stringgstream
to push the lines into a vector, foobars. The vector is read through
backwards, since first vector is back of line, and positions are updated.
An output file is then taken and the vector is looped through,
and the name and strength of each member is pushed to the output file.
The main function carries out the input, strength, and output functions.
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;


class foobar
//This is the base class that defines position and name members and member functions
{
private:
    string name;
    int position;
    
public:
    foobar(const string &n) : name(n), position(0) {}
    // This is initalizing the constructor so that name is n, and position is 0 at start
    
    void setPosition(int p) {position = p;}
    
    std::string getName() const {return name;} 

    virtual int getStrength() const {return position;}
    // This is the base strength function that will be overridden in derived classes    

    
protected:
    int getPosition() const {return position;}
};
/////////////////////////////////////////////////

class foo: public foobar
 // This is the foo derived class foo that overrides strength function
{
public:
    foo(const string &n) : foobar(n) {}
    virtual int getStrength() const override //Overriding
        {
        return getPosition() * 3;
        }
};
/////////////////////////////////////////////////

class bar: public foobar
//  This is the bar derived class that overrides strength function
{
public:
    bar(const string &n) : foobar(n) {}
    virtual int getStrength() const override 
        {
        return getPosition() + 15; 
        }
};
/////////////////////////////////////////////////

void input(std::vector<foobar*> &foobars)
// Reads input file, creates objects dynamically based on type, and stores pointers in vector.
{
    std::string filename;
    std::cout << "Enter the name of the input text file: ";
    std::cin >> filename;

    std::ifstream inputFile(filename);  
    if (!inputFile)
    {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return;
    }

    if (inputFile.is_open())
    {
        std::string line;
        while (getline(inputFile, line))
        {
            std::stringstream ss(line);
            string type, name;
            ss >> type >> name;
            if (type == "foo")
            {
                //Create the correct object type using dynamic allocation
                //so all can be stored as foobar* and used polymorphically
                foobars.push_back(new foo(name));
            }
            else if (type == "bar")
            {
                foobars.push_back(new bar(name)); 
            }
            else
            {
            foobars.push_back(new foobar(name)); 
            };
        }
        inputFile.close();
        return;
    }
        
    else
    {
        std::cerr << "Cannot open file!" << std::endl;
    }
};
/////////////////////////////////////////////////

void strength(std::vector<foobar*> &foobars)
{ // Iterating backwards, since back of the foobar line is the first line in vector
    int position = 1;
    for (int i = foobars.size() - 1; i>=0; --i)
    {
        foobars[i] -> setPosition(position);//Setting position based on backwards loop
        position ++;
    }
};
/////////////////////////////////////////////////

void output(const std::vector<foobar*> &foobars)
{// Function loops forward with updated position 
    std::string filename2;
    std::cout << "Enter the name of the text output file: ";
    std::cin >> filename2;
    //Output file taken

    ofstream outputFile(filename2);
    if (!outputFile)
    {
        std::cerr << "Error: Could not open the file " << filename2 << std::endl;
        return;
    }
    //Looping forward and storing "name strength" in output file
    for(int i = 0; i < foobars.size(); ++i)
    {
            outputFile <<foobars[i] -> getName()<< " "
            <<foobars[i] -> getStrength() << "\n"; 
            // 
    }; 
    outputFile.close();
};
/////////////////////////////////////////////////

int main()
{// The input, strength, and outputs functions are run first with foobards as the vector
    std::vector<foobar*> foobars; //Vector that gets passed in to functions
    input(foobars);
    strength(foobars);
    output(foobars);

    for (auto f : foobars) // Deallocate all dynamically created objects
        delete f;
    return 0;
};



