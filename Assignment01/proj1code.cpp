// Osman Pamuk, Assingment 1
// Microsoft (R) C/C++ Optimizing Compiler Version 19.10.25019 for x86
// Windows 10
// Tested by GNU C++ compiler

/* MAGIC SQUARE - An NxN matrix containing values from 1 to N*N that are  */
/* ordered so that the sum of the rows, columns, and the major diagonals  */
/* are all equal.  There is a particular algorithm for odd integers, and  */
/* this program constructs that matrix, up to 13 rows and columns.  This   */
/* program also adds the sums of the row, columns, and major diagonals.   */

#include <iostream>
#include <iomanip>
#include <limits>
#include <cctype>

using namespace std;
// Magic square class definition 
class CMagicSqr {
public:
	~CMagicSqr();
	static void ShowIntro();
	bool ValidateInput(int);
	void InitMatrix();
	void PrintMatrix();

private:
	int input;		/* User defined integer       */
	int otherdiag;	/* Total of one matrix diagonal*/
	int **loc;		/* Array holding all          */
};

/*****
* Function: ~CMagicSqr
* Purpose: CMagicSqr deconstructor. Safely delete loc matrix variable.
* Parameters: N/A
* Local Variables: N/A
******/
CMagicSqr::~CMagicSqr() {
	for (int i = 0; i < 14; i++)
		delete[] loc[i];
	delete[] loc;
}

/*****
* Function: ShowIntro
* Purpose: Print introduction of what this program is all about.
* Parameters: N/A
* Local Variables: N/A
******/
void CMagicSqr::ShowIntro() {
	cout << "\nMagic Squares: This program produces an NxN matrix where\n";
	cout << "N is some positive odd integer.  The matrix contains the \n";
	cout << "values 1 to N*N.  The sum of each row, each column, and \n";
	cout << "the two main diagonals are all equal.  Not only does this \n";
	cout << "program produces the matrix, it also computes the totals for\n";
	cout << "each row, column, and two main diagonals.\n";

	cout << "\nBecause of display constraints, this program can work with\n";
	cout << "values up to 13 only.\n" << endl;
}

/*****
* Function: ValidateInput
* Purpose: Validity check for input: Must be a positive odd integer < 13.
* Parameters: 
*			input:  User defined integer <Passed by value>
* Local Variables: N/A
******/
bool CMagicSqr::ValidateInput(int input) {
	if (input <= 0) {
		cout << "Sorry, but the integer has to be positive.\n";
		cout << "\nEnter a positive, odd integer (-1 to exit program):" << endl;
		return false;
	}
	if (input > 13) {
		cout << "Sorry, but the integer has to be less than 15.\n";
		cout << "\nEnter a positive, odd integer (-1 to exit program):" << endl;
		return false;
	}
	if (input % 2 == 0) {
		cout << "Sorry, but the integer has to be odd.\n";
		cout << "\nEnter a positive, odd integer (-1 to exit program):" << endl;
		return false;
	}
	this->input = input;
	return true;
}

/*****
* Function: InitMatrix
* Purpose: Initialize matrix
* Parameters: N/A
* Local Variables: 
*			row: Determines row of matrix
*			col: Determines col of matrix
*			value: Integer between 1 and
******/
void CMagicSqr::InitMatrix() {	
	/*  input*input values.       */
	int row;	/* Determines row of matrix   */
	int col;	/* Determines col of matrix   */
	int value;	/* Integer between 1 and      */
	loc = new int*[14];
	for (int i = 0; i < 14; i++)
		loc[i] = new int[14];
	/*                                                                        */
	/*    Initialize matrix, row, col, and otherdiag                          */
	/*                                                                        */
	for (row = 0; row <= input; row++)		/* Initialize matrix with     */
		for (col = 0; col <= input; col++)	/*  all zeroes.               */
			loc[row][col] = 0;				/* Values will reside within  */
	/*  rows 1 to input*input and */
	/*  columns 1 to input*input. */
	/* Row totals will reside in  */
	/*  loc[row][0], where row is */
	/*  the row number, while the */
	/*  column totals will reside */
	/*  in loc[0][col], where col */
	/*  is the column number.     */
	row = 1;				/* First value gets to sit on */
	col = input / 2 + 1;	/*  1st row, middle of matrix.*/
	otherdiag = 0;

	/*                                                                        */
	/*    Loop for every value up to input*input, and position value in matrix*/
	/*                                                                        */
	for (value = 1; value <= input * input; value++) { /* Loop for all values.       */
		if (loc[row][col] > 0) /* If some value already      */ { /*  present, then             */
			row += 2;			/*  move down 1 row of prev.  */
			if (row > input)	/*  If exceeds side, then     */
				row -= input;	/*   go to other side.        */

			col--;				/*  move left 1 column.       */
			if (col < 1)		/*  If exceeds side, then     */
				col = input;	/*   go to other side.        */
		}

		loc[row][col] = value; /* Assign value to location.  */

		/*                                                                        */
		/*       Add to totals                                                    */
		/*                                                                        */
		loc[0][col] += value;	/* Add to its column total.   */
		loc[row][0] += value;	/* Add to its row total.      */
		if (row == col)			/* Add to diagonal total if   */
			loc[0][0] += value; /*  it falls on the diagonal. */

		if (row + col == input + 1) /* Add to other diagonal if   */
			otherdiag += value;		/*  it falls on the line.     */

		/*                                                                        */
		/*       Determine where new row and col are                              */
		/*                                                                        */
		row--;
		if (row < 1)		/* If row exceeds side then   */
			row = input;	/*  goto other side.          */
		col++;
		if (col > input)	/* If col exceeds side then   */
			col = 1;		/*  goto other side.          */
	} /* End of getting all values. */
}

/*****
* Function: PrintMatrix
* Purpose: Print out the matrix with its totals
* Parameters: N/A
* Local Variables:
*			row: Determines row of matrix
*			col: Determines col of matrix
******/
void CMagicSqr::PrintMatrix() {
	int row; /* Determines row of matrix   */
	int col; /* Determines col of matrix   */
	/*                                                                        */
	/*    Print out the matrix with its totals                                */
	/*                                                                        */
	cout << "\nThe number you selected was " << input;
	cout << ", and the matrix is:\n" << endl;
	for (row = 1; row <= input; row++)			/* Loop: print a row at a time*/ {
		cout << setw(5) << "";					/* Create column for diag.total*/
		for (col = 1; col <= input; col++)
			cout << setw(5) << loc[row][col];	/* Print values found in a row*/
		cout << " = " << setw(5) << loc[row][0] << endl; /* Print total of row.        */
	}

	/*                                                                        */
	/*    Print out the totals for each column, starting with diagonal total. */
	/*                                                                        */
	for (col = 0; col <= input; col++)			/* Print line separating the  */
		cout << "-----";						/*  value matrix and col totals*/
	cout << "\n" << setw(5) << otherdiag;		/* Print out the diagonal total*/
	for (col = 1; col <= input; col++)
		cout << setw(5) << loc[0][col];			/* Print out the column totals*/
	cout << setw(8) << loc[0][0] << endl;		/* Print out the other diagonal*/
												/*  total                     */
}

int main() {
	int input;	/* User defined integer       */	
	CMagicSqr* pMagicSqr = new CMagicSqr;	// magic square object
	//  Print an introduction of what this program is all about.
	pMagicSqr->ShowIntro();
	// ask user for an input
	cout << "Enter a positive, odd integer (-1 to exit program):" << endl;
	while (cin >> input || !cin.eof()) {
		if (cin.good()) {
			/*                                                                        */
			/*    If input = -1, then exit program.                                   */
			/*                                                                        */
			if (input == -1)
				break;
			/*                                                                        */
			/*    Validity check for input: Must be a positive odd integer < 13.      */
			/*                                                                        */
			if (!pMagicSqr->ValidateInput(input))
				continue;
			// initialize matrix as a magic square
			pMagicSqr->InitMatrix();
			// print the initialized magic square matrix
			pMagicSqr->PrintMatrix();
		} else if (!isdigit(input)) 
			cout << "Not a number\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		// ask user for the next input
		cout << "\nEnter a positive, odd integer (-1 to exit program):" << endl;
	} /* End of while input>-1 loop */
	cout << "\nBye bye!" << endl;
	// clean up
	delete pMagicSqr;
}
