/*************************************************
 * 
 * Matthew Eaton
 * CS-300
 * Project 2 - Advising Assistance Program
 * 
 * 
 * Sidenote: the supplied file had the BOM at the
 * start, which messed with the program. I overwrote
 * it as just plain utf-8 formatting without BOM
 * and now it doesn't have the BOM symbol at the
 * start of the file
 * 
 * test.txt contains the exact same contents as
 * ABCU_Advising_Program_Input.txt I was using it
 * to make sure my error checking was working
 * correctly by sometimes adding incorrect lines
 * or a preq that wasn't in it's own line etc.
 *************************************************/


#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>
#include <conio.h>
#include <filesystem>

// I am going to limit using the namespace to just string and vector
// for the whole program in a universal scope
// if I want to use other namespaces I will declare them in a smaller scope
using std::string;
using std::vector;

// global default size
const unsigned int DEFAULT_SIZE = 200;

// The definitions for the csCourse object
// csCourse object holds the information for course
struct csCourse {
	string id;
	string title;
	vector<string> preqs;
	csCourse() {
	}
};

class HashTable {

private:
	// Node definitions for a Node in the hashtable
	// each node has a key and holds a csCourse object
	// as well as a pointer to the next node incase of collisions
	struct Node {
		csCourse course;
		unsigned int key;
		Node* next;

		// default constructor
		Node() {
			key = UINT_MAX;
			next = nullptr;
		}

		// constructor initialized with a course passed to it
		Node(csCourse givenCourse) : Node() {
			course = givenCourse;
		}

		// constructor initialized with a course and key passed to it
		Node(csCourse givenCourse, unsigned int givenKey) : Node(givenCourse) {
			key = givenKey;
		}
	};

	//vector holding the id's for courses placed in the data structure
	vector<string> courseIDS;

	vector<Node> nodes;
	unsigned int tableSize = DEFAULT_SIZE;

	// declare the hash function
	unsigned int hash(string id);

public:
	HashTable();
	HashTable(unsigned int size);
	virtual ~HashTable();
	void Insert(csCourse course);
	void PrintCourses();
	void Remove(string courseId);
	csCourse Search(string courseId);
	void SetCourseIDS(string courseId);
	vector<string> GetCourseIDS();
	void SortCourseIDS();
};

// hash function for the hashtable
unsigned int HashTable::hash(string id) {
	int key = 0;

	// loop through the string creating a key
	for (unsigned int i = 0; i < id.size(); i++) {
		key = id[i] * 3 + key;
	}
	return key % tableSize;
}

// Default constructor for the hashtable
HashTable::HashTable() {
	nodes.resize(tableSize);
}

// constructor that has a size passed to it
HashTable::HashTable(unsigned int size) {
	this->tableSize = size;
	nodes.resize(tableSize);
}

// deconstructor
HashTable::~HashTable() {
	nodes.erase(nodes.begin());
}

//insert a csCourse into the hashtable
void HashTable::Insert(csCourse course) {
	// generate the key
	unsigned int key = hash(course.id);
	//check the node associated with the key
	Node* prevNode = &nodes.at(key);

	//if the node key is UNIT_MAX then the node is empty
	if (prevNode->key == UINT_MAX) {
		prevNode->key = key;
		prevNode->course = course;
		prevNode->next = nullptr;
	}
	//else there is a collision so start a linked list for the key
	else {
		while (prevNode->next != nullptr) {
			prevNode = prevNode->next;
		}
		// add the new node to the end of the linked list
		prevNode->next = new Node(course, key);
	}
}

// print the courses in alphabetic order
void HashTable::PrintCourses() {
	csCourse printedCourse;
	std::cout << "-------------------------------------------------------------" << std::endl;
	printf("%-10s%-40s%-40s\n", "|ID|", "|Title of course|", "|Prequisites|");
	std::cout << "-------------------------------------------------------------" << std::endl;

	// loop through the list of courses
	for (unsigned int i = 0; i < courseIDS.size(); i++) {
		printedCourse = Search(courseIDS.at(i));
		printf("%-10s%-40s", printedCourse.id.c_str(), printedCourse.title.c_str());

		// loop through the preqs vector to print them out
		for (unsigned int j = 0; j < printedCourse.preqs.size(); j++) {
			std::cout << " " << printedCourse.preqs.at(j);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// remove a given course from the hashtable and the courseIDS vector
void HashTable::Remove(string courseId) {
	std::cout << "This has not been implemented yet" << std::endl;
	unsigned key = hash(courseId);
	Node* currNode = &nodes.at(key);
	Node* tempNode;

	//delete the item from the courseIDS vector
	for (unsigned int i = 0; i < courseIDS.size(); i++) {
		if (courseIDS.at(i) == courseId) {
			courseIDS.erase(courseIDS.begin() + i);
			break;
		}
	}

	// if the node is empty
	if (currNode->key == UINT_MAX) {
		return;
	}

	// if the node matches and is part of a linked list with an item after it
	if (currNode->course.id == courseId) {
		if (currNode->next != nullptr) {
			tempNode = currNode->next;
			nodes.erase(nodes.begin() + key);
			nodes.insert(nodes.begin() + key, * tempNode);
			delete tempNode;
			return;
		}
		else {
			// setting the key to UINT_MAX works if it is the only item
			currNode->key = UINT_MAX;
			return;
		}
	}
	
	tempNode = currNode;
	currNode = currNode->next;
	// while not a null pointer check the id
	while (currNode != nullptr) {
		if (currNode->course.id == courseId) {

			tempNode->next = currNode->next;
			delete currNode;
			return;
		}
		// make the previous node the current and the current the next node
		tempNode = currNode;
		currNode = currNode->next;
	}
}

// Find a specific course
csCourse HashTable::Search(string courseId) {
	csCourse course;
	unsigned key = hash(courseId);

	// check if the item is in the hashtable
	Node* node = &nodes.at(key);
	if (node->key == UINT_MAX) {
		return course;
	}
	// check if the item is in a linked list because of a collision
	while (node != nullptr) {
		if (node->course.id == courseId) {
			return node->course;
		}
		node = node->next;
	}
	// return an empty course if the course is not found
	return course;
}

void HashTable::SetCourseIDS(string courseId) {
	courseIDS.push_back(courseId);
}
vector<string> HashTable::GetCourseIDS() {
	return courseIDS;
}
// use built in sort to sort the vector storing the courseIds
// the built in sort uses quicksort
void HashTable::SortCourseIDS() {
	std::sort(courseIDS.begin(), courseIDS.end());
}

// check to make sure that all preqs are in the hashtable
void checkPreqs(HashTable* hashtable) {
	// making a local copy makes the code more legible
	vector<string> localCourseIDS = hashtable->GetCourseIDS();

	// loop through the courseID vector
	for (unsigned int i = 0; i < localCourseIDS.size(); i++) {

		// if the course is in the vector for Ids it is in the hashtable
		csCourse currentCourse = hashtable->Search(localCourseIDS.at(i));

		for (unsigned int j = 0; j < currentCourse.preqs.size(); j++) {

			// if the search returns an empty object it is not in the hashtable so display an error
			if (hashtable->Search(currentCourse.preqs.at(j)).id.empty()) {
				std::cout << "Error a prequisite was not found among the courses for course: "
					<< currentCourse.id << " there may be an error in the file" << std::endl;
				std::cout << "Please add the course: " << currentCourse.preqs.at(j)
					<< " to the file" << std::endl;
			}
		}
	}
}

// attempt to open the file and parse the contents
void openAndParseFile(string filename, HashTable* hashtable) {
	int lineNumber = 0;
	string line;
	std::ifstream file(filename);

	// vector that stores the information of a course stored on a line
	// in the file
	vector <string> courseInputVector;

	// check if the file is valid
	if (file.good()) {
		// get the line
		while (std::getline(file, line)) {
			lineNumber++;
			std::stringstream ss(line);
			csCourse course;
			// while the stringstream is good parse the line using , as a delimiter storing it in substring
			while (ss.good()) {
				std::string substring;
				getline(ss, substring, ',');

				courseInputVector.push_back(substring);
			}
			// if the vector is smaller than 2 than it is invalid
			if (courseInputVector.size() < 2) {
				std::cout << "Error improper input or formatting on line: " << lineNumber << std::endl;
				break;
			}
			// if the vector is larger than 3 than there are preqs
			else if (courseInputVector.size() >= 3) {
				for (unsigned int i = 2; i < courseInputVector.size(); i++) {
					course.preqs.push_back(courseInputVector.at(i));
				}
			}
			// set the id and title
			course.id = courseInputVector.at(0);
			course.title = courseInputVector.at(1);

			// insert the finished course into the hashtable
			hashtable->Insert(course);
			// clear the vector that stored the line items
			courseInputVector.clear();
			// call the setter to add the course id to the list of all courses
			hashtable->SetCourseIDS(course.id);
		}
		// if the vector that stores the IDs is not empty then the hashtable has items in it
		// sort and check the preqs
		if (!hashtable->GetCourseIDS().empty()) {
			hashtable->SortCourseIDS();
			checkPreqs(hashtable);
		}
		std::cout << "File loaded" << std::endl;
	}
	// file does not exist or is invalid
	else {
		std::cout << "File does not exist please enter a valid filename" << std::endl;
	}
}

// print out the menu for the user
void displayMenu() {
	std::cout << "-------------------------------------------------------------" << std::endl;
	std::cout << "Press 1 to load a specified file" << std::endl;
	std::cout << "Press 2 to print all courses in alphibetical order" << std::endl;
	std::cout << "Press 3 to print a specific course's information" << std::endl;
	std::cout << "Press 9 to quit" << std::endl;
	std::cout << "-------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
}

// parse user choice
void parseChoice(char choice, HashTable* hashtable) {
	string userFile;
	string courseToSearch;
	csCourse courseFound;

	switch (choice) {
	// open the file and parse it
	case '1':
		std::cout << "Please enter the name of the file with the extension\n"
			<< "for example test.txt" << std::endl;
		std::cin >> userFile;
		openAndParseFile(userFile, hashtable);
		std::cout << "Press any key to continue" << std::endl;
		_getch();
		break;
	// print the courses in alphabetical order
	case '2':
		hashtable->PrintCourses();
		std::cout << "Press any key to continue" << std::endl;
		_getch();
		break;
	// search and print out a specific course
	case '3':
		std::cout << "Please enter the Id of the course you want to find\n"
			<< "for example CSCI400" << std::endl;
		std::cin >> courseToSearch;
		courseFound = hashtable->Search(courseToSearch);
		// if the course id is not empty output the info
		if (!courseFound.id.empty()) {
			std::cout << "-------------------------------------------------------------" << std::endl;
			printf("%-10s%-40s%-40s\n", "|ID|", "|Title of course|", "|Prequisites|");
			std::cout << "-------------------------------------------------------------" << std::endl;
			printf("%-10s%-40s", courseFound.id.c_str(), courseFound.title.c_str());
			for (unsigned int j = 0; j < courseFound.preqs.size(); j++) {
				std::cout << " " << courseFound.preqs.at(j);
			}
		}
		// else the course is not in the hashtable
		else {
			std::cout << "\nCourse not found";
		}
		std::cout << std::endl;
		std::cout << "\nPress any key to continue" << std::endl;
		_getch();
		break;
	case '9':
		break;
	default:
		std::cout << "invalid entry" << std::endl;
		std::cout << "Press any key to continue" << std::endl;
		_getch();
	}
}

// The one and only main()
int main() {
	// create a new hashtable
	HashTable* hashtable = new HashTable;

	char choice = 0;
	while (choice != '9') {
		system("cls");
		displayMenu();
		choice = _getch();
		parseChoice(choice, hashtable);
	}
}