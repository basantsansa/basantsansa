#ifndef DEPTH_SCANNER_H
#define DEPTH_SCANNER_H

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "node.h"

#define NDEBUG	// comment out to enable asserts for debugging
#include <cassert>

using namespace std;

const int16_t SMOOTHNESS_CONST = 10;
// all possible constant delta to neighbouring node coordinates
const struct coordinate delta[MAX_NEIGHBOURS] = 
{
{+1,  0},
{ 0, +1},
{-1,  0},
{ 0, -1},
{+1, +1},
{-1, -1},
{+1, -1},
{-1, +1}};

class depthScanner {
	private:
		int32_t numRows;
		int32_t numCols;
		node* nodes;
	public:
		depthScanner(int32_t m, int32_t n);
		~depthScanner(void);
		void fillNode(int32_t m, int32_t n, int value);
		node* getNode(int32_t m, int32_t n);
		void calculateNeighbourNodes(void);
		void print(void);
		void get2DImage(int threshold);
		int32_t getSmoothness(void);
};

depthScanner :: depthScanner(int32_t m, int32_t n)
{
	numRows = m;
	numCols = n;
	int64_t totNumNodes = m * n;
	nodes = new node [totNumNodes];
}

depthScanner :: ~depthScanner(void)
{
	delete [] nodes;
}

void depthScanner :: fillNode(int32_t m, int32_t n, int value)
{
	assert(m < numRows);
	assert(n < numCols);

	node *pixel = this->getNode(m,n);
	assert(pixel != NULL);

	pixel->setDepth(value);
}

node* depthScanner :: getNode(int32_t row, int32_t col)
{
	assert(row < numRows);
	assert(col < numCols);
	
	int32_t nodes_to_skip = ((row * numCols) + col);
	assert(nodes_to_skip < (numRows * numCols));
	
	node *pixel = &nodes[nodes_to_skip];
	assert(pixel != NULL);
	return pixel;
}

void depthScanner :: print(void)
{
	cout << endl;
	cout << "values";
	cout << endl;
	for (int32_t i=0; i < numRows; i++) {
		cout << "  ";
		for (int32_t j=0; j < numCols; j++) {
			node *pixel = this->getNode(i,j);
			pixel->print();
		}
		cout << endl;
	}
	cout << endl;
}

void depthScanner :: get2DImage(int threshold)
{
	cout << endl;
	cout << "2D Image";
	cout << endl;
	for (int32_t i=0; i < numRows; i++) {
		cout << "  ";
		for (int32_t j=0; j < numCols; j++) {
			node *pixel = this->getNode(i,j);
			if (pixel->getDepth() > threshold)
				cout << "# ";
			else
				cout << ". ";
		}
		cout << endl;
	}
	cout << endl;
}

void depthScanner :: calculateNeighbourNodes(void)
{
	int32_t nextRow, nextCol;
	struct coordinate mn;

	for (int32_t i=0; i < numRows; i++) {
		for (int32_t j=0; j < numCols; j++) {
			node *pixel = this->getNode(i,j);
			assert(pixel != NULL);
			for (int16_t k=0; k < MAX_NEIGHBOURS; k++) {
				nextRow = i + delta[k].row;
				nextCol = j + delta[k].col; 
				if (((0 <= nextRow) && (nextRow < numRows)) &&
				    ((0 <= nextCol) && (nextCol < numCols))) {
					mn.row = nextRow;
					mn.col = nextCol;
					pixel->setNeighbour(mn);
				}
			}
		}
	}
}

int32_t depthScanner :: getSmoothness(void)
{
	int32_t numSmoothLoc = 0;
	int32_t i, j, row, col, sumNeighDepth, nodeSmoothness, abs_smoothness;
	
	for (i=0; i < numRows; i++) {
		for (j=0; j < numCols; j++) {
			node *pixel = this->getNode(i,j);
			assert(pixel != NULL);
			
			int16_t numNeighbours = pixel->getNumNeighbours();
			int depth = pixel->getDepth();

			sumNeighDepth = 0;
			nodeSmoothness = 0;
			
			for (int16_t k=0; k < numNeighbours; k++) {
				row = pixel->getNeighbourRow(k);
				col = pixel->getNeighbourCol(k);

				assert(row >= 0); assert(col >= 0);
				assert(row < numRows); assert(col < numCols);

				node *neighbour = this->getNode(row,col);
				assert(neighbour != NULL);
				sumNeighDepth += neighbour->getDepth();
			}
			
			nodeSmoothness = numNeighbours * depth;
			abs_smoothness =
				abs(sumNeighDepth - nodeSmoothness);

			if (abs_smoothness < SMOOTHNESS_CONST)
				numSmoothLoc++;
		}
	}	

	return numSmoothLoc;	
}

#endif // End of #ifndef DEPTH_SCANNER_H
