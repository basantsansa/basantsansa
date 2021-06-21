#include <fstream>
#include "depthScanner.h"

using namespace std;

int main(int argc, char * argv[])
{
	int32_t numRows, numCols;
	char c = '0';
	string inputFileName;

	if ((argc < 2) || (argc > 2)) {
		cout << "Pass the input file name as argument 1" << endl;
		cout << "For example execute like: ./a.out in_101.txt "<< endl;
		cout << "Expected format of input file: "<< endl;
		cout << "Line 1: numRows(m)  num_columns(n)"<< endl;
		cout << "Line 2 to m+1: 'n' integers values, seprated by a space, in each line"<< endl;
		return -1;
	}
	
	inputFileName = argv[1];
	
	ifstream infile(inputFileName);
	if (!infile) {
		cout << "File '" << inputFileName << "' couldn't be opened!" << endl;
		return -1;
	}

	cout << "Reading file '" << inputFileName << "' for input data" << endl;
	infile >> numRows;
	infile >> numCols;
	cout << "Read, numRows = '" << numRows << "' and numCols = '"<< numCols << "' from the file '" << inputFileName << " Press Y to continue!" << endl;
	cin >> c;
	if ((c != 'Y') && (c != 'y')) {
		cout << "You pressed '"<< c <<"', so exiting the prog" << endl;
		return 0;
	}

	cout << "Reading "<< numRows << " X " << numCols << " 'int' values from " << inputFileName << endl;
	
	depthScanner scan(numRows, numCols);

	int depthValue;
	int32_t i, j;
	for (i=0; i < numRows; i++) {
		for (j=0; j < numCols; j++) {
			infile >> depthValue;
			scan.fillNode(i, j, depthValue);
		}
	}

	scan.calculateNeighbourNodes();

	scan.print();
	int t;
	cout << endl << "Enter the threshold, for 2D image" << endl;
	cin >> t;
	scan.get2DImage(t);
	cout << endl;

	cout << "Smoothness of the surface = " << scan.getSmoothness();
	cout << endl;
	return 0;
}
