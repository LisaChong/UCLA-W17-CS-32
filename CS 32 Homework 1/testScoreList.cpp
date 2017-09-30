#include "ScoreList.h"
#include "Sequence.h"
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

int main()
{
	ScoreList LisaList;
	assert(LisaList.add(32));
	assert(LisaList.add(100));
	assert(!LisaList.add(-1));
	assert(!LisaList.add(1000));
	cerr << LisaList.maximum() << endl << LisaList.minimum();
	cerr << endl << LisaList.size();
}