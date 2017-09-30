#include <stack>
#include <iostream>
#include <string>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);


int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}

/*int main()
{
	// declare a stack of Coords

	Coord a(5, 6);
	coordStack.push(a);          // push the coordinate (5,6)
	coordStack.push(Coord(3, 4)); // push the coordinate (3,4)

	Coord b = coordStack.top();  // look at top item in the stack
	coordStack.pop();            // remove the top item from stack
	if (coordStack.empty())      // Is the stack empty?
		cout << "empty!" << endl;
	cout << coordStack.size() << endl;  // number of elements
}*/

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
	stack<Coord> coordStack;
	coordStack.push(Coord(sr, sc));
	maze[sr][sc] = '-';
	while (coordStack.empty() == false)
	{
		Coord a = coordStack.top();
		coordStack.pop();
		if (a.r() == er && a.c() == ec) //Is the point we're at now the end point?
		{
			return true;
		}
		else
		{
			if (a.r() - 1 > 0 && maze[a.r() - 1][a.c()] == '.') //Tests if North position is valid, pushes if so
			{
				coordStack.push(Coord(a.r() - 1, a.c()));
				maze[a.r() - 1][a.c()] = '-';
			}
			if (a.c() + 1 < nCols - 1 && maze[a.r()][a.c() + 1] == '.') //Tests if East position is valid, pushes if so
			{
				coordStack.push(Coord(a.r(), a.c() + 1));
				maze[a.r()][a.c() + 1] = '-';
			}
			if (a.r() + 1 < nRows - 1 && maze[a.r() + 1][a.c()] == '.') //Tests if South position is valid, pushes if so
			{
				coordStack.push(Coord(a.r() + 1, a.c()));
				maze[a.r() + 1][a.c()] = '-';
			}
			if (a.c() - 1 > 0 && maze[a.r()][a.c() - 1] == '.') //Tests if West position is valid, pushes if so
			{
				coordStack.push(Coord(a.r(), a.c() - 1));
				maze[a.r()][a.c() - 1] = '-';
			}
		}
	}
	return false;
}