void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m->menuItems() == NULL)
	{
		cout << path << m->name() << endl;
		return;
	}
	if (m->name() != "")
		cout << path << m->name() << endl;
	for (vector<MenuItem*>::const_iterator it = m->menuItems()->begin(); it != m->menuItems()->end(); it++)
	{
		string tempPath;
		if (m->name() != "")
		{
			tempPath = path + m->name() + "/";
		}
		listAll(*it, tempPath);
	}
}