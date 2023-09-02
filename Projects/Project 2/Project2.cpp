//============================================================================
// Name : Project2.cpp
// Author : Adam Sissoko
// Version : 1.0
//============================================================================
#include <iostream>
#include <fstream>
#include "CSVparser.hpp"
using namespace std;

//GLOBAL DEFINITIONS
//Every course comes with a vector, an ID, and a name to store the prequisites. 

struct Course
{
  string courseNum;
  string courseName;
    vector < string > preReqs;
    Course ()
  {
  }
};

//Every node stores the Course Information, as well as the left/right children. 
//I used more than one constructor, allowing me to create nodes without defined courses.

struct Node
{
  Course course;
  Node *left;
  Node *right;
    Node ()
  {
    left = nullptr;
    right = nullptr;
  }
  Node (Course aCourse):Node ()
  {
    this->course = aCourse;
  }
};

// CLASS DEFINITIONS

class CourseBST
{
private:
  Node * root;
  void addNode (Node * node, Course course);
  void printSampleSchedule (Node * node);
  void printCourseInformation (Node * node, string courseNum);
public:
    CourseBST ();
    virtual ~ CourseBST ();
  void DeleteRecursive (Node * node);
  void Insert (Course course);
  int NumPrerequisiteCourses (Course course);
  void PrintSampleSchedule ();
  void PrintCourseInformation (string courseNum);
};

//Creates BST(binary search tree), initializes root.

CourseBST::CourseBST ()
{
  root = nullptr;
}

//Deconstructs BST by passing root to function that deletes each node recursively
CourseBST::~CourseBST ()
{
  DeleteRecursive (root);
}

//Deletes BST nodes recursively
void
CourseBST::DeleteRecursive (Node * node)
{
  if (node)
    {
      DeleteRecursive (node->left);
      DeleteRecursive (node->right);
      delete node;
    }
}

//Add node to BST
void
CourseBST::Insert (Course course)
{
//If root isn't initialized
  if (root == nullptr)
//initialize it with the course Information
    root = new Node (course);

//otherwise send the node to the addNode method
  else

    this->addNode (root, course);
}

//Prints # of prequisites for passed course 

int
CourseBST::NumPrerequisiteCourses (Course course)
{
  int count = 0;
  for (unsigned int i = 0; i < course.preReqs.size (); i++)
    {
      if (course.preReqs.at (i).length () > 0)
	count++;
    }
  return count;
}

//Transfers  BST root to private method
void
CourseBST::PrintSampleSchedule ()
{
  this->printSampleSchedule (root);
}

//Transfers BST Root and course information, allowing us to search for them 

void
CourseBST::PrintCourseInformation (string courseNum)
{
  this->printCourseInformation (root, courseNum);
}


void
CourseBST::addNode (Node * node, Course course)
{

// Current courseNum <  node's courseNum
  if (node->course.courseNum.compare (course.courseNum) > 0)
    {
      if (node->left == nullptr)
	node->left = new Node (course);
      else
	this->addNode (node->left, course);
    }
// Current courseNum >=  node courseNum
  else
    {
      if (node->right == nullptr)
	node->right = new Node (course);
      else
	this->addNode (node->right, course);
    }
}

//prints loaded courses in order, recursively
void
CourseBST::printSampleSchedule (Node * node)
{
  if (node != nullptr)
    {
      printSampleSchedule (node->left);
      cout << node->course.courseNum << ", " << node->course.
	courseName << endl;
      printSampleSchedule (node->right);
    }
  return;
}

// Display information for a single course
void
CourseBST::printCourseInformation (Node * curr, string courseNum)
{

  // Traverse BST until reaching bottom or finding matching coursenum
  while (curr != nullptr)
    {

// Passed courseNum matches the current courseNum
      if (curr->course.courseNum.compare (courseNum) == 0)
	{

// Display course and get num prereqs
	  cout << endl << curr->course.courseNum << ", " << curr->course.
	    courseName << endl;
	  unsigned int size = NumPrerequisiteCourses (curr->course);
	  cout << "Prerequisite(s): ";
// If there are prereqiuisites, display them
	  unsigned int i = 0;
	  for (i = 0; i < size; i++)
	    {
	      cout << curr->course.preReqs.at (i);
	      if (i != size - 1)
		cout << ", ";
	    }
// If no prequisites, tell user.
	  if (i == 0)
	    cout << "This course does not require any prequisites.";
	  cout << endl;
	  return;
	}
// Passed courseNum <  current courseNum. traverse left
      else if (courseNum.compare (curr->course.courseNum) < 0)
	curr = curr->left;
// Passed courseNum > current courseNum. traverse right
      else
	curr = curr->right;
    }
// Course cannot be found has
  cout << "Course " << courseNum << " not found." << endl;
}

//STATIC METHODS 

//Load Courses from cin choice for csv file
bool
loadCourses (string csvPath, CourseBST * coursesBST)
{

// Open course file, separate lines and insert them into binary search tree.
  try
  {
    ifstream courseFile (csvPath);
    if (courseFile.is_open ())
      {
	while (!courseFile.eof ())
	  {

// Each csv the course contains will be held by a vector.
	    vector < string > courseInfo;
	    string courseData;
	    getline (courseFile, courseData);
	    while (courseData.length () > 0)
	      {

// Get substring for each course, add them to the vectorm and delete them.
		unsigned int comma = courseData.find (',');
		if (comma < 100)
		  {		// Datafields can have 99 strings or less.
		    courseInfo.push_back (courseData.substr (0, comma));
		    courseData.erase (0, comma + 1);
		  }
// Add  last course after last comma
		else
		  {
		    courseInfo.push_back (courseData.substr (0,
							     courseData.
							     length ()));
		    courseData = "";
		  }
	      }
// Insert the values into a course, then insert into BST and close file.
	    Course course;
	    course.courseNum = courseInfo[0];
	    course.courseName = courseInfo[1];
	    for (unsigned int i = 2; i < courseInfo.size (); i++)
	      {
		course.preReqs.push_back (courseInfo[i]);
	      }
	    coursesBST->Insert (course);
	  }
	courseFile.close ();
	return true;
      }
  }
  catch (csv::Error & e)
  {
    cerr << e.what () << endl;
  }
  return false;
}

//MAIN

int
main (int argc, char *argv[])
{

  string csvPath, courseId;
  switch (argc)
    {
    case 2:
      csvPath = argv[1];
      break;
    case 3:
      csvPath = argv[1];
      courseId = argv[2];
      break;
    default:
      csvPath = "";
      break;
    }
// Defines BST and prints welcome message for user.
  CourseBST *coursesBST = nullptr;
  cout << "\nWelcome to your online course planner. \n" << endl;
// Forces user to choose a string and converts 1st character to int. 
  string choice = "0";
  int userChoice = choice[0] - '0';
//if not 9 or exit
  while (userChoice != 9)
    {
      cout << " 1. Load Data" << endl;
      cout << " 2. Print list of courses." << endl;
      cout << " 3. Print individual course" << endl;
      cout << " 9. Exit Program " << endl;
      cout << "\n Please choose an option. ";
      cin >> choice;

// Checks whether user selection is a double-digit number.
      if (choice.length () == 1)
	userChoice = choice[0] - '0';
      else
	userChoice = 0;
      bool success = 1;

      switch (userChoice)
	{

	case 1:
	  if (coursesBST == nullptr)
	    coursesBST = new CourseBST ();
	  if (csvPath.length () == 0)
	    {
	      cout << "Course Data Needed: Enter File Name: ";
	      cin >> csvPath;
	    }

//Checks if file was opened and loaded succesfully.
	  success = loadCourses (csvPath, coursesBST);
	  if (success)
	    cout << "Courses loaded \n" << endl;
	  else
	    cout << "File cannot be located\n" << endl;
	  csvPath = "";
	  break;
// Check whether BST exists, prints sample schedule. 
	case 2:
	  if (coursesBST != nullptr && success)
	    {
	      cout << "Sample Schedule:\n" << endl;
	      coursesBST->PrintSampleSchedule ();
	      cout << endl;
	    }
	  else
	    cout << "Please select option 1 first, and load your courses.\n" << endl;
	  break;
// Check whether BST exists, display course information.
	case 3:
	  if (coursesBST != nullptr && success)
	    {
	      if (courseId.length () == 0)
		{
		  cout <<
		    "Which course would you like to receive information about?";
		  cin >> courseId;
		for (auto & userChoice:courseId)
		    userChoice = toupper (userChoice);
		}
	      coursesBST->PrintCourseInformation (courseId);
	      cout << endl;
	    }
	  else
	    cout << "Please select option 1 first, and load your courses.\n" << endl;
	  courseId = "";
	  break;

// Error/ Exit Messages.
	default:
	  if (userChoice != 9)
	    cout << choice << " is an invalid option.\n" << endl;
	  break;
	}
    }
  cout << "\nProgram Exited. Thanks for using!" << endl;
  return 0;
}
