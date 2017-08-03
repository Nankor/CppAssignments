// Osman Pamuk, Assignment 2
// Microsoft (R) C/C++ Optimizing Compiler Version 19.10.25019 for x86
// Windows 10
// Tested by GNU C++ compiler

/*	This program creates two alleys(stacks) to park and retrieve
*	cars.*/
#include <iostream>
#include <cctype>
#include <iomanip>
#include <limits>

#define MAXSIZE 5

using namespace std;
// possible user inputs
enum class commands { PARK = 'p', RETRIVE = 'r', DISPLAY = 'd', QUIT = 'q' };

/*	CarNode : A class for one node of linked list.One node represents
*	a car and its relative position in the parking alley.*/
class CarNode {
public:
	CarNode() : m_ticketNum(0), m_pNext(nullptr) {}
	CarNode(CarNode&) : m_ticketNum(0), m_pNext(nullptr) {}

	// assign next pointer
	void SetNext(CarNode* p) { m_pNext = p; }
	// assign ticket number
	void SetTicketNum(int tN) { m_ticketNum = tN; }
	// get the next pointer
	CarNode* GetNext(void) { return (m_pNext); }
	// get the ticket number
	int GetTicketNum(void) { return (m_ticketNum); }
private:
	int m_ticketNum; // ticket number of car
	CarNode* m_pNext; // pointer to next node in stack
};


/*	Alley : A class for the stack of nodes.Two instantiations A and B
*	are used. */
class CAlley {
public:
	CAlley() : m_pTop(nullptr), mSize(0), mMaxSize(MAXSIZE) { }

	int Park(int); // park a car
	void Retrieve(CAlley*); // retrieve a car
	void Terminate(); // quit the program
	void Display() const; // display contents of alley
private:
	void SetTop(CarNode* p) { m_pTop = p; } // assign top pointer
	// check if stack is empty
	bool Empty() { return ((mSize == 0) ? true : false); }
	// check if stack is full
	bool Full() { return ((mSize == MAXSIZE) ? true : false); }
	int Push(CarNode*); // push one node onto top of stack
	static int GetTickerNo(); // get the park ticket number from user
	CarNode* Pop(); // pop one node from the top of stack
	CarNode* m_pTop; // pointer to top of Allay (stack)
	int mSize; // number of nodes in Allay (stack)
	int mMaxSize; //max number of nodes in Allay (stack)
};

////////////////////////////////////////////////////////////////
// Function: CAlley::Push
// Purpose: Add a new node to top of stack
// Parameters:
//		CarNode * pNewNode - the node to be added to top of stack
// Local Variables: N/A
// Return value:
//			- return 1 if pushed sucessfully
//			- return 0 if stack was full
////////////////////////////////////////////////////////////////
int CAlley::Push(CarNode* pNewNode) {
	// if empty return 0
	if (Full()) return 0;
	// if note empty, set new node as the new top
	pNewNode->SetNext(this->m_pTop);
	this->SetTop(pNewNode);
	// icrease the number of cars in the ally
	mSize++;
	// return true
	return 1;
}

/////////////////////////////////////////////////////////////////
// Function: CAlley::Pop
// Purpose: Remove a node to top of Allay (stack).
// Parameters:
//		CarNode *pNewNode - returns the node removed from top of Allay
//		is zero if stack is empty
// Local Variables:
//		CarNode *pTempNode - local pointer used to temporary store the node to be returned
/////////////////////////////////////////////////////////////////
CarNode* CAlley::Pop() {
	// if empty nothing to return thus just return nullptr
	if (Empty()) return nullptr;
	// if not empty, store the current top node 
	CarNode* pTempNode = this->m_pTop;
	// and set the second top as the new top
	this->SetTop(this->m_pTop->GetNext());
	// decrease the counter (number of cars in the ally)
	mSize--;
	// return the old top 
	return pTempNode;
}

///////////////////////////////////////////////////////////////
// Function: CAlley::Park ( )
// Purpose: Park a car, if lot is not full. First allocate a
// node, then addone it to the top of the stack
// Parameters:
//		userTicketNum - the ticket number for the node to be added
// Local Variables:
//		CarNode *pNewNode - local pointer to newly allocated node
// Return Value:
//		-	1 if parked sucessfully (lot not full)
//		-	0 if not parked (lot was full)
///////////////////////////////////////////////////////////////
int CAlley::Park(int userTicketNum) {
	// create the new car node in the heap
	CarNode* pNewNode = new CarNode;
	// set the ticket number of the new node
	pNewNode->SetTicketNum(userTicketNum);
	// return the push response
	return Push(pNewNode);
}

///////////////////////////////////////////////////////////////
// Function: CAlley:: Retrieve ( int userTicketNum, CAlley *pB)
// Purpose: Retrieve a car from alley A. Search for the car/node
// based on ticket num. by driving a car (popping off top) out of
// A and driving (pushing onto top) into B.
// If the car is found, it is not pushed onto B, but rather,
// it is deleted. Then the cars in B are moved back into A.
//
// Parameters:		
//		pB - pointer to CAlley B
//
// Local Variables:
//		userTicketNum - the ticket number for the node to be added, -1 if an invalid input
//		CarNode *pCurr - local pointer used as index//		
///////////////////////////////////////////////////////////////
void CAlley::Retrieve(CAlley* pB) {
	// if alley is empty
	if (this->Empty()) {
		cout << "Lot is Empty, No cars to Retrieve" << endl;
		return;
	}
	// Get the ticket number form user, if invalid input return
	int userTicketNum = GetTickerNo();
	if (userTicketNum == -1) return;
	// Retrieve a car from alley A
	CarNode* pCurr = this->Pop();
	// search for the car / node based on ticket num. by driving a car (popping off top)
	while (pCurr != nullptr) {
		// if found the car
		if (pCurr->GetTicketNum() == userTicketNum)
			break;
		// driving (pushing onto top) into B.
		if (!pB->Push(pCurr)) {
			cout << "Alley B is full" << endl;
			break;
		}
		// by driving a car (popping off top)
		pCurr = this->Pop();
	}
	// if car not found 
	if (pCurr == nullptr) {
		cout << "CAR NOT PARKED IN MY LOT" << endl;
	} else {
		// if found release the heap
		delete pCurr;
	}
	// Then the cars in B are moved back into A.
	while (!pB->Empty()) {
		if (!this->Push(pB->Pop())) {
			cout << "Alley A is full" << endl;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////
// Function: CAlley::Display
// Purpose: Display the parked cars in the alley 
// Parameters: N/A
// Local Variables:
//		CarNode *pCurr - local pointer used as index
////////////////////////////////////////////////////////////////
void CAlley::Display() const {
	CarNode* pCurr = this->m_pTop;
	cout << "Alley A :";
	while (pCurr != nullptr) {
		cout << setw(4) << pCurr->GetTicketNum();
		pCurr = pCurr->GetNext();
	}
	cout << endl;
}

////////////////////////////////////////////////////////////////
// Function: CAlley::Terminate
// Purpose: Terminate the instance, pops and delete all stack objects
// Parameters:
// Local Variables:
//		CarNode *pCurr - local pointer used as index
//		CarNode *PTempNode - local pointer used to temporary store the node to be deleted
////////////////////////////////////////////////////////////////
void CAlley::Terminate() {
	CarNode *pTempNode, *pCurr = this->Pop();
	while (pCurr != nullptr) {
		pTempNode = pCurr;
		pCurr = pCurr->GetNext();
		delete pTempNode;
	}
}

////////////////////////////////////////////////////////////////
// Function: CAlley::GetTickerNo
// Purpose: ask user to enter a valid integer as a parking ticket number
// Parameters: N/A
// Local Variables:
//		input - local variable to store the user input
//		result - local variable to returned, -1 if input is not valid, equal to input if it is valid
////////////////////////////////////////////////////////////////
int CAlley::GetTickerNo() {
	int input, result = -1;
	cout << "Enter the Ticket no. = ";
	cin >> input;
	if (cin.good()) {
		result = input;
	} else {
		cout << "Not a valid ticket number." << endl;
	}
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return result;
}

int main() {
	char cInput; /* User defined char       */
	CAlley* pCAlleyA = new CAlley; /* Alley A */
	CAlley* pCAlleyB = new CAlley; /* Alley B */
	int ticketNumber = 0, userInputTicketNo;

	// ask user for an input
	cout << "\nD) isplay " << setw(4) << ' ' << "P) ark" << setw(4) << ' ' << "R) etrieve" << setw(4) << ' ' << "Q) uit: ";
	while (cin >> cInput || !cin.eof()) {
		if (cin.good()) {
			/*    If input = Q or q, then exit program.			*/
			if (tolower(cInput) == char(commands::QUIT)) {
				// terminate instances 
				pCAlleyA->Terminate();
				pCAlleyB->Terminate();
				break;
			}
			/*    if input is D, d, R, r, P, or p 				*/
			switch (tolower(cInput)) {
				case char(commands::DISPLAY) :
					// display the stack in Alley A
					pCAlleyA->Display();
					break;
				case char(commands::PARK) :
					// park a car to valley A
					if (!pCAlleyA->Park(++ticketNumber)) {
						cout << "My Lot is Full" << endl;
						ticketNumber--;
					} else {
						cout << "Ticket no. = " << ticketNumber << endl;
					}
					break;
				case char(commands::RETRIVE) :
					// retrieve car with ticket number if it is parked in valley A
					pCAlleyA->Retrieve(pCAlleyB);
					break;
			}
		}
		cin.clear();
		// ask user for the next input
		cout << "\nD) isplay " << setw(4) << ' ' << "P) ark" << setw(4) << ' ' << "R) etrieve" << setw(4) << ' ' << "Q) uit: ";
	} /* End of while input>-1 loop */
	cout << "\nBye bye!" << endl;
	// clean up
	delete pCAlleyA , pCAlleyB;
}
