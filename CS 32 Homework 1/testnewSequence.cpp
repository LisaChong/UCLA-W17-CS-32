/*#include "newSequence.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
	Sequence a(1000);   // a can hold at most 1000 items
	Sequence b(5);      // b can hold at most 5 items
	Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
	ItemType v = 45;

	// No failures inserting 5 items into b
	for (int k = 0; k < 5; k++)
		assert(b.insert(k, v));

	//get rid off of the 3rd item and add back a 4th
	assert(b.erase(3));
	//cerr << b.size();
	assert(b.insert(4) == 0);
	
	//Find where certain values are
	assert(b.find(v) == 1);
	assert(b.find(1) == -1);

	//Get item at 3 aka v=45
	ItemType value = 0;
	assert(b.get(3, value));
	//cerr << value;

	// Failure if we try to insert a sixth item into b
	assert(!b.insert(2, v));

	//Failure bc it is not empty
	assert(!b.empty());

	// When two Sequences' contents are swapped, their capacities are
	// swapped as well:
	a.swap(b);
	assert(!a.insert(5, v));
	for (int k = 0; k < 1000; k++)
		assert(b.insert(k, v));

	//Get rid of all v, there are 4
	//assert(a.remove(v) == 4);

	//Replace 2nd block with 20
	assert(a.set(1, 20));

	a.dump();

	b = a;
	
	Sequence d = a;

	cerr << endl;
	b.dump();
	cerr << endl;
	d.dump();

	/*Sequence s;
	s.insert(0, 35);
	s.insert(1, 66);
	s.insert(2, 5);
	s.insert(3, 45);
	s.insert(4, 12);

	Sequence a;
	a.insert(17);
	a.insert(26);
	a.insert(2);

	a.swap(s);

	s.dump();
	cerr << endl;
	a.dump();
}

#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Sequence s;
	assert(s.insert(0, 10));
	assert(s.insert(0, 20));
	assert(s.size() == 2);
	ItemType x = 999;
	assert(s.get(0, x) && x == 20);
	assert(s.get(1, x) && x == 10);
}

int main()
{
	test();
	cerr << "Passed all tests" << endl;
}*/

#include "newSequence.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Sequence s;
	assert(s.insert(0, "lavash"));
	assert(s.insert(0, "tortilla"));
	assert(s.size() == 2);
	ItemType x = "injera";
	assert(s.get(0, x) && x == "tortilla");
	assert(s.get(1, x) && x == "lavash");
}

int main()
{
	test();
	cerr << "Passed all tests" << endl;
}