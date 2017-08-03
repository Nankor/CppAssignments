/* 
 * Osman Pamuk, Assignment 3
 * Microsoft (R) C/C++ Optimizing Compiler Version 19.10.25019 for x86
 * Windows 10
 * Tested by GNU C++ compiler
 * From C++11, I have used:
 * - Brace Initialization
 */

/*******************************************************************
* FILE NAME: Assignment03.cpp
********************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <limits>
#include <cstdlib> 

#define DEFAULT_SIZE 20

using namespace std;

/********************************************************************
* FUNCTION: toLowerStr *
* PURPOSE: convert uppercase char to lower casechar
*
* PARAMETERS: strSrc, strDst
* LOCAL VARIABLES: N/A
********************************************************************/
void toLowerStr(char* strSrc, char* strDst) {
	for (auto i = 0; i < strlen(strSrc); i++) {
		strDst[i] = tolower(strSrc[i]);
	}
}

/********************************************************************
* CLASS NAME: CEmployee *
* PURPOSE: To store employee information, to mantain the employee linked list
*  and to be used as a node for the employee linked list
**********************************************************************/
class CEmployee {
public:
	CEmployee() : m_Age{0}, m_Salary{0}, m_pLink{nullptr} {};
	CEmployee(char*, unsigned int, unsigned int);
	void Open(char*);
	void ExecuteOptions();
	void AddEmp();
	void DeleteEmp();
	void SearchEmp() const;
	void ListAllEmp() const;
	void SaveToFile() const;
	void ExitProg() const;
	void Push(char [], unsigned int, unsigned int);
private:
	// m_Name will store the file name for the first object
	char m_Name[DEFAULT_SIZE];
	unsigned int m_Age;
	unsigned int m_Salary;
	// m_pLink will store the linked link header node for the first object
	CEmployee* m_pLink;

	// possible user inputs
	enum class commands { ADD = '1', DELETE, SEARCH, LIST, SAVE, EXIT };

	// functions
	static void ShowOptions();
};

/**
 * CEmployee Contructor
 */
CEmployee::CEmployee(char* strName, unsigned int nAge, unsigned int nSalary) :
	m_Age{nAge}, m_Salary{nSalary}, m_pLink(nullptr) {
	strncpy(m_Name, strName, DEFAULT_SIZE);
}

/********************************************************************
* FUNCTION: Open *
* PURPOSE: open file and parse it, create the ordered linked list to store employee objects
*
* PARAMETERS: strFileName
* LOCAL VARIABLES: infile, strLine, strTempEmpName, d, i
********************************************************************/
void CEmployee::Open(char* strFileName) {
	// Open input file
	ifstream infile;
	infile.open(strFileName);
	// Check if input file exists
	if (!infile) {
		cout << "\nFile does not exist. Use Add and Save options to create file\n";
		return;
	} 
	// copy the filename to the m_Name
	strncpy(m_Name, strFileName, DEFAULT_SIZE);
	char strLine[80]; // string to be read from the file
	auto i = 0;

	// pHeadPtr = 0;
	while (infile.getline(strLine, 80)) {
		// cout << strLine << " : " << i << endl;
		auto strName = strtok(strLine, ";");
		auto strAge = strtok(nullptr, ";");
		auto strSalary = strtok(nullptr, ";");
		Push(strName, atoi(strAge), atoi(strSalary));
		i++;
	} //while
	infile.close();
	cout << endl << i << " employee records are loaded from the data file into the link list" << endl;
}

/***********************************************************************
* FUNCTION: ExecuteOptions *
* PURPOSE: Ask user to select an action from options menu, then execute the *
* user selected option*
* PARAMETERS: N/A *
* LOCAL VARIABLES: cInput, bIfExit, bNeedSave *
***********************************************************************/
void CEmployee::ExecuteOptions() {
	char cInput; /* User defined char       */
	auto bIfExit = false;
	auto bNeedSave = false;

	ShowOptions();
	// for sallary with commas
	cout.imbue(locale(""));
	// ask user for an input
	while (cin >> cInput || !cin.eof()) {		
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (cin.good()) {
			switch (tolower(cInput)) {
				case char(commands::ADD):
					AddEmp();
					bNeedSave = true;
					break;
				case char(commands::DELETE):
					DeleteEmp();
					bNeedSave = true;
					break;
				case char(commands::SEARCH):
					SearchEmp();
					break;
				case char(commands::LIST):
					ListAllEmp();
					break;
				case char(commands::SAVE):
					SaveToFile();
					bNeedSave = false;
					break;
				case char(commands::EXIT):
					// if need save ask user if they want to save
					if (bNeedSave) {
						cout << "\nEmployee list changed. Do you want to save the changes? [Y/N]: ";
						cin >> cInput;
						// if user entered Y or y save the changes 
						if(cInput == 'Y' || cInput == 'y') {
							SaveToFile();
						}
					}
					ExitProg();
					bIfExit = true;
					break;
			}
		}
		cin.clear();
		// if user wanted to exit
		if (bIfExit) break;
		// ask user for the next input
		ShowOptions();
	} /* End of while input>-1 loop */
	cout << "\nBye bye!" << endl;
}

/***********************************************************************
* FUNCTION: ShowOptions *
* PURPOSE: Shows options menu*
* PARAMETERS: N/A *
* LOCAL VARIABLES: N/A *
***********************************************************************/
void CEmployee::ShowOptions() {
	cout << "\nMenu Options :\n" <<
		"1. Add Employee\n" <<
		"2. Delete Employee\n" <<
		"3. Search Employee\n" <<
		"4. List All Employees\n" <<
		"5. Save Employee Database\n" <<
		"6. Exit Employee Database\n" <<
		"Enter Your Choice : ";
}

/***********************************************************************
* FUNCTION: DeleteEmp *
* PURPOSE: Deletes employees from link list by employee name. *
* PARAMETERS: N/A *
* LOCAL VARIABLES: pCurrObj, pPreObj, strEmpName *
***********************************************************************/
void CEmployee::DeleteEmp() {
	char strEmpName[DEFAULT_SIZE];
	// ask user the employee name
	cout << "\nEnter Employee Name you want to delete: ";
	cin.getline(strEmpName, sizeof(strEmpName));

	auto pCurrObj = m_pLink;
	CEmployee* pPreObj = nullptr;
	// loop over all nodes
	while (pCurrObj != nullptr) {
		// compare employee names
		if (strcmp(pCurrObj->m_Name, strEmpName) == 0) {
			// if found delete the node
			if (pPreObj == nullptr)
				m_pLink = pCurrObj->m_pLink;
			else
				pPreObj->m_pLink = pCurrObj->m_pLink;
			// print employee is found
			cout << "Employee: " << strEmpName << " found and deleted." << endl;
			// clean up the unused memory
			delete pCurrObj;
			// beak the while loop
			break;
		}
		// save the pre obj
		pPreObj = pCurrObj;
		// and go to the next obj
		pCurrObj = pCurrObj->m_pLink;
	}
	// if employee name not found
	if (pCurrObj == nullptr) {
		cout << "Employee: " << strEmpName << " cannot be found." << endl;
	}
}

/***********************************************************************
* FUNCTION: SaveToFile *
* PURPOSE: Saves data in link list to data file empinfo.data *
* PARAMETERS: N/A *
* LOCAL VARIABLES: pCurrObj, objOutFile *
***********************************************************************/
void CEmployee::SaveToFile() const {
	auto pCurrObj = m_pLink;
	// open file to write
	ofstream objOutFile(m_Name);	
	if (!objOutFile) {
		 cout << "\nCannot open file: " << m_Name << endl;
		 return;		
	}
	// loop over all the nodes and print employee details 
	while (pCurrObj != nullptr) {
		objOutFile << pCurrObj->m_Name << ";" << pCurrObj->m_Age << ";" << pCurrObj->m_Salary << "\n";
		// and go to the next obj
		pCurrObj = pCurrObj->m_pLink;
	}
	cout << "\nChanges succesfully saved to file : " << m_Name << endl;
	// close the file
	objOutFile.close();
}

/***********************************************************************
* FUNCTION: AddEmp *
* PURPOSE: Ask user new employee informations and send new employee infos *
*  to push method. *
* PARAMETERS: N/A *
* LOCAL VARIABLES: strEmpName,  numAge, numSalary*
***********************************************************************/
void CEmployee::AddEmp() {
	char strEmpName[DEFAULT_SIZE];
	unsigned int numAge, numSalary;
	// ask user new employee information
	cin.clear();
	cout << "\nEnter Employee Name: ";
	cin.getline(strEmpName, sizeof(strEmpName));
	cout << "Enter Employee's Age: ";
	cin >> numAge;	
	cout << "Enter Employee's Salary: ";
	cin >> numSalary;
	if (cin.good()) {
		// add new employee to the linked list
		Push(strEmpName, numAge, numSalary);
		cout << "New Employee added successfully" << endl;
	} else {
		cout << "Invalid input. New employee cannot be created!" << endl;
	}
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');	
}

/********************************************************************
* FUNCTION: ExitProg *
* PURPOSE: Exits program and deletes memory spaces that were being *
* used. *
* PARAMETERS: N/A *
* LOCAL VARIABLES: pCurr, pDeleteThisNode *
***********************************************************************/
void CEmployee::ExitProg() const {
	auto pCurr = m_pLink;
	while (pCurr != nullptr) {
		auto pDeleteThisNode = pCurr;
		pCurr = pCurr->m_pLink;
		delete pDeleteThisNode;
	}
}

/*********************************************************************
* FUNCTION: ListAllEmp *
* PURPOSE: Lists all employees in link list to the screen. *
* PARAMETER: N/A *
* LOCAL VARIABLES: pCurrObj, index, strIndex *
**********************************************************************/
void CEmployee::ListAllEmp() const {
	auto pCurrObj = m_pLink;
	auto index = 1;
	char strIndex[4];
	// print table header
	cout << "\n" << setw(4) << left << "#" << setw(20) << "Employee Name" << setw(6) << "Age" << setw(10) << "Salary" << endl;
	cout << setw(40) << setfill('=') << "" << endl;
	// loop over all the nodes and print employee details 
	while (pCurrObj != nullptr) {
		sprintf(strIndex, "%d.", index++);
		cout << setfill(' ') << setw(4) << left << strIndex << setw(20) << pCurrObj->m_Name
			<< setw(6) << pCurrObj->m_Age << setw(10) << pCurrObj->m_Salary << endl;
		// and go to the next obj
		pCurrObj = pCurrObj->m_pLink;
	}
}

/*********************************************************************
* FUNCTION: SearchEmp *
* PURPOSE: Searches link list for specified employee. *
* PARAMTERS: N/A *
* LOCAL VARIABLES: strEmpName, CurrPtr *
**********************************************************************/
void CEmployee::SearchEmp() const {
	char strEmpName[DEFAULT_SIZE];
	// ask user the employee name to be searched
	cout << "\nEnter Employee Name you want to find: ";
	cin.getline(strEmpName, sizeof(strEmpName));

	auto pCurrObj = m_pLink;
	// loop over all nodes
	while (pCurrObj != nullptr) {
		// compare employee names
		if (strcmp(pCurrObj->m_Name, strEmpName) == 0) {
			// if found print the employee information
			cout << "Employee: " << strEmpName << " found, Age: " << pCurrObj->m_Age << ", Salary: " << pCurrObj->m_Salary << endl;
			break;
		}
		// and go to the next obj
		pCurrObj = pCurrObj->m_pLink;
	}
	// if employee is not found
	if (pCurrObj == nullptr) {
		cout << "Employee: " << strEmpName << " cannot be found." << endl;
	}
}


/*********************************************************************
* FUNCTION: Push *
* PURPOSE: Create and Push a new employee to the linked list *
* PARAMTERS: strName, nAge, nSalary *
* LOCAL VARIABLES: newEmpPtr, pCurrObj, pPreObj, strTempNewName, *
* strTempCurrObjName *
**********************************************************************/
void CEmployee::Push(char* strName, unsigned int nAge, unsigned int nSalary) {
	// create a new object from heap
	auto newEmpPtr = new CEmployee{strName, nAge, nSalary};
	// if this is the first object
	if (m_pLink == nullptr) {
		m_pLink = newEmpPtr;
		return;
	}
	// if not the first one
	// search the location to put the object in a sorted way by name
	auto pCurrObj = m_pLink;
	CEmployee* pPreObj = nullptr;
	char strTempNewName[DEFAULT_SIZE], strTempCurrObjName[DEFAULT_SIZE];
	toLowerStr(strName, strTempNewName);
	// look all the nodes and compare names
	while (pCurrObj != nullptr) {
		toLowerStr(pCurrObj->m_Name, strTempCurrObjName);
		// if new obj name comes before current obj name
		if (strcmp(strTempNewName, strTempCurrObjName) < 0) {
			// insert new obj before current obj
			newEmpPtr->m_pLink = pCurrObj;
			if (pPreObj == nullptr)
				m_pLink = newEmpPtr;
			else
				pPreObj->m_pLink = newEmpPtr;
			return;
		}
		// save the pre obj
		pPreObj = pCurrObj;
		// and go to the next obj
		pCurrObj = pCurrObj->m_pLink;
	}
	// if not added yet, add to it to the end (last node)
	pPreObj->m_pLink = newEmpPtr;
}


int main(int argc, char* argv[]) {
	// look if there is an command line argument 
	if (argc < 2) {
		cout << "Usage: " << argv[0] << " <filename>" << endl;
		return 0;
	}
	// create linked list to store employee objects, ordered by name
	auto pEmpLinkedList = new CEmployee();
	pEmpLinkedList->Open(argv[1]);
	// show and execute options
	pEmpLinkedList->ExecuteOptions();

	// cleanup
	delete pEmpLinkedList;
	return 1;
}
