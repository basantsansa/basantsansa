#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <cstdint>

using namespace std;

struct coordinate {
	int16_t row;
	int16_t col;
};

const int16_t MAX_NEIGHBOURS = 8;

class node {
	private: 
		int depth;
		int16_t numNeighbours;
		struct coordinate neighbour[MAX_NEIGHBOURS];
	public:
		node(void);
		~node();
		void setDepth(int val);
		int getDepth(void);
		void print(void);
		void setNeighbour(struct coordinate mn);
		int32_t getNeighbourRow(uint16_t index);
		int32_t getNeighbourCol(uint16_t index);
		int getNumNeighbours(void);
};

node :: node(void)
{
	depth = 0;
	numNeighbours = 0;
}

node :: ~node()
{

}

void node :: setDepth(int val)
{
	depth = val;
}

int node :: getDepth(void)
{
	return depth;
}

void node :: setNeighbour(struct coordinate mn)
{
	neighbour[numNeighbours].row = mn.row;
	neighbour[numNeighbours].col = mn.col;
	numNeighbours++;
}

int32_t node :: getNeighbourRow(uint16_t index)
{
	return neighbour[index].row;
}

int32_t node :: getNeighbourCol(uint16_t index)
{
	return neighbour[index].col;
}

int node :: getNumNeighbours(void)
{
	return numNeighbours;
}

void node :: print(void)
{
	cout << depth << " ";
}

#endif // End of #ifndef NODE_H
