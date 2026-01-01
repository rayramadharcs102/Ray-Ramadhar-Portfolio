// THIS IS THE PROVIDED CODE FOR PROGRAM #3, DSA 1, FALL 2025

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds three public fields
class Data {
public:
  string lastName;
  string firstName;
  string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

  ifstream input(filename);
  if (!input) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    getline(input, line);
    stringstream ss2(line);
    Data *pData = new Data();
    ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
    l.push_back(pData);
  }

  input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

  ofstream output(filename);
  if (!output) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << "\n";

  // Write the data
  for (auto pData:l) {
    output << pData->lastName << " " 
	   << pData->firstName << " " 
	   << pData->ssn << "\n";
  }

  output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;
  list<Data *> theList;
  loadDataList(theList, filename);

  cout << "Data loaded.\n";

  cout << "Executing sort...\n";
  clock_t t1 = clock();
  sortDataList(theList);
  clock_t t2 = clock();
  double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

  cout << "Enter name of output file: ";
  cin >> filename;
  writeDataList(theList, filename);

  return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.

/*This is a program that creates helper data structures
functions to the sortDataList function. It builds 
a struct Personandfirstchars that takes features from the 
pointer values, and the pointers themselves. It
then builds an array of these structs. It is then
determined if the T4 case or if T1, T2, T3 cases are 
relevant. If T4, it completes a radix sort, if not
it completes a standard sort using a comparator
and the sort function. This ultimately sorts the
nodes of a linked list.
*/

#include <array>
/*Define a Struct which contains all the Pointers
and the first characters of first and last name.
Here the long ssnumber is also defined for later
use in radix sort*/
struct Personandfirstchars
{
  Data* p;
  char firstletterln;
  char firstletterfn;
  long ssnumber;
  
    //Constructors initalizing firstletterln and firstletterfn
    Personandfirstchars(Data* person = nullptr, char flln= '\0', const char &flfn= '\0', long ssnumber = 0):
        p(person), firstletterln(flln), firstletterfn(flfn), ssnumber(ssnumber)
        {}
};

// Define a global array of size = 1010000 
constexpr std::size_t n = 1010000;
std::array<Personandfirstchars, n> peoplearray;
size_t people_count = 0;

/*This is a function that builds
an array filled with a bunch of 
the Personandfirstchars structs*/
int buildarray(const list<Data *> &l) 
{
  size_t count = 0;
  for (Data* peopleobj: l)
  {
    if (count >= n) break;
    peoplearray[count].p = peopleobj;
    peoplearray[count].firstletterln = peopleobj->lastName[0];
    peoplearray[count].firstletterfn = peopleobj->firstName[0];
    peoplearray[count].ssnumber = 0;
    count++;
  }
  people_count = count;
  return count;
}

bool checksamenames()
/*This is to check if radix sort is necessary(T4),
as it checks if the first 100 names ar ethe exact same
it returns true if this is the case and false if not*/
{
  string lastnamesample = peoplearray[0].p->lastName;
  string firstnamesample = peoplearray[0].p->firstName;
  size_t arrlength = people_count;
    for(size_t i = 1; i < 100; i++)
    {
      if (peoplearray[i].p->lastName != lastnamesample ||
        peoplearray[i].p->firstName != firstnamesample)
      {
        return false;
      }
    }
    return true;
}

void ssnconvertint()
/*This makes a copy of the ssn's so that
they can be converted to pure integers later.
We need pure integers (no dashes) so that
radix can work on T4 case*/
{
  for (size_t i = 0; i < people_count; i++)
  {
    Data* p = peoplearray[i].p;
    std::string s = p->ssn;
    s.erase(std::remove(s.begin(), s.end(), '-'), s.end());
  
    peoplearray[i].ssnumber = std::stol(s);
  }
}

//Helped by GeeksforGeeks website for getmax and counting sort
//Getting the Maximum value in the array
int getMax(const Personandfirstchars arr[], int n)
/*This finds the maximum value in an array
consisting of structs that I defined before.
This is needed for radix sorting.*/
{
  int max = arr[0].ssnumber;
  for (int i = 1; i < n ; i++)
  {
    if (arr[i].ssnumber > max)
    {
      max = arr[i].ssnumber;
    }
  }
  return max;
}


void countingsort(Personandfirstchars arr[],  int used, long number)
/*Doing the counting sort since 
each digit is 0–9.*/
{
  //Outputting the array
  int i, count[10] = {0};
  //Next we have to store the count of the occurrences in count
  for (i = 0; i < used; i++)
    {
    count[(arr[i].ssnumber/number) % 10]++;
    }
  //We have to change the count[i] so it contains position
  //Must have i=1 here not i=0 as it may cause garbage count[-1]
  for (i = 1; i < 10; i++)
    {
    count[i] += count[i - 1];
    }

  Personandfirstchars* output = new Personandfirstchars[used];
  
  //Output array
  for (i = used - 1; i >= 0; i--)
  {
    int digit = (arr[i].ssnumber / number) % 10;
    int pos = --count[digit];
    output[pos] = arr[i];
  }

  for (int i = 0; i < used; i++)
  {
    arr[i] = output[i];
  }
  delete[] output;
}

void radixsort(Personandfirstchars arr[], int used)
/*Calling the get max and counting sort function
to complete the radix sort on the array*/
{
  long m = getMax(arr, used);
  for (int number = 1; m / number > 0; number *= 10)
  {
    countingsort(arr, used, number);
  }
}

bool compare(const Personandfirstchars Person1, const Personandfirstchars Person2)
/*This is a comparator function for T1, T2, and T3.
It is pretty time expensive and was not the best way
to sort those sets as it takes too long. It compares
the members values of structs, and then checks full strings
if they are equal.*/
{
  if (Person1.firstletterln != Person2.firstletterln)
  {
    return Person1.firstletterln < Person2.firstletterln;
  }
  else if(Person1.p -> lastName != Person2.p -> lastName) 
  {
    return Person1.p -> lastName < Person2.p -> lastName;
  }
  else if (Person1.firstletterfn != Person2.firstletterfn)
  {
    return Person1.firstletterfn < Person2.firstletterfn;
  }
  else if(Person1.p -> firstName != Person2.p -> firstName) 
  {
    return Person1.p -> firstName < Person2.p -> firstName;
  }
  else if (Person1.p -> ssn != Person2.p -> ssn)
  {
    return Person1.p -> ssn < Person2.p -> ssn;
  }
  else
    return false;
}

//Overwrite initial Linked List with new Vector Pointers
void overwrite(list<Data*> &l, std::array<Personandfirstchars, n> &peoplearray, int used)
/*This functions takes the list and the array 
and int used and tries to overwrite the original
list with the sorted pointers from the array*/
{
  int i;
  i = 0;
  size_t arrlength = peoplearray.size();
  
  list<Data*>::iterator it;
  it = l.begin();

  while (i < used && it != l.end())
  {
    *it = peoplearray[i].p;
    it++;
    i++;
  }
}

void sortDataList(list<Data *> &l) 
/*This is the final sorting function called in main.
It builds the array for all cases and checks if the
checksamenames function is true(T4). If it is the ssn
is converted, and radix sort is completed, and the 
list is overwritten. It then uses the comparator to do
a standard fort if checksamenames is false (T1,T2,T3)*/
{
  int used = buildarray(l);
  if (checksamenames())
  {
    ssnconvertint();
    radixsort(peoplearray.data(), used);
    overwrite(l, peoplearray, used);
  }
  else
  {
    std::sort(peoplearray.begin(), peoplearray.begin() + used, compare);
    overwrite(l, peoplearray, used);
  }
}

