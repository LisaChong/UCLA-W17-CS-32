#include "Sequence.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
	Sequence s;
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

#include "Sequence.h"
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
	
	
	<< "Passed all tests" << endl;
}

#include "Sequence.h"
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
}