bool allTrue(const double a[], int n)
{
	if (n <= 0)
		return true;
	bool part1 = somePredicate(a[n - 1]);
	bool part2 = allTrue(a, n - 1);
	return (part1 && part2);
}

int countFalse(const double a[], int n)
{
	if (n <= 0)
		return 0;
	int part1 = 0, part2 = 0;
	if (!somePredicate(a[n - 1]))
		part1 = 1;
	part2 = countFalse(a, n - 1);
	return part1 + part2;
}

int firstFalse(const double a[], int n)
{
	if (n <= 0)
		return -1;
	int part1 = -1, part2 = -1;
	if (!somePredicate(a[n - 1]))
		part1 = n - 1;
	part2 = firstFalse(a, n - 1);
	if (part2 == -1)
		return part1;
	else
		return part2;
}

int indexOfMin(const double a[], int n)
{
	if (n < 0)
		return -1;
	if (n == 0)
		return 0;
	int part1 = a[n - 1];
	int part2 = a[indexOfMin(a, n - 1)];
	if (part2 <= part1)
		return indexOfMin(a, n - 1);
	else
		return n - 1;
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 == 0)
		return true;
	if (n1 - n2 < 0)
		return false;
	if (a1[n1 - 1] == a2[n2 - 1])
		return includes(a1, n1 - 1, a2, n2 - 1);
	else
		return includes(a1, n1 - 1, a2, n2);
}
