#include "History.h"
#include "globals.h"

#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	m_rows = nRows - 1;
	m_cols = nCols - 1;
	for (int r = 0; r < nRows; r++)
		for (int c = 0; c < nCols; c++)
			m_grid[r][c] = '.';
}
bool History::record(int r, int c)
{
	if (r < 1 || c < 1 || r - 1 > m_rows || c - 1 > m_cols)
		return false;
	if (m_grid[r - 1][c - 1] == '.')
		m_grid[r - 1][c - 1] = 'A';
	else if (m_grid[r - 1][c - 1] == 'Z')
		return true;
	else
		m_grid[r - 1][c - 1]++;
	return true;

}
void History::display() const
{
	clearScreen();
	for (int r = 0; r <= m_rows; r++)
	{
		for (int c = 0; c <= m_cols; c++)
			cout << m_grid[r][c];
		cout << endl;
	}
	cout << endl;
	cout << "Press enter to continue.";
	cin.ignore(10000, '\n');
}