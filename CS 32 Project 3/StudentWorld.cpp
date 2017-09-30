#include "StudentWorld.h"
#include <vector>
#include <iostream>
#include <sstream>  
#include <iomanip> 

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir) {};

int StudentWorld::init()
{
	Field f;
	string fieldFile = getFieldFilename();
	string error;
	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + error);
		return GWSTATUS_LEVEL_ERROR;
	}
	bool a[4] = { false, false, false, false };
	fileNames = getFilenamesOfAntPrograms();
	for (int m = 0; m != fileNames.size(); m++)
	{
		compilerForEntrant[m] = new Compiler;
		if (!compilerForEntrant[m]->compile(fileNames[0], error))
		{
			setError(fileNames[0] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
		a[m] = true;
	}
	for (int k = 0; k < 64; k++) //k is the row (y)
		for (int j = 0; j < 64; j++) //j is the column (x)
		{
			Field::FieldItem item = f.getContentsOf(j, k);
			if (item == Field::FieldItem::rock)
				m_field[k][j].push_back(new Pebble(j, k, this));
			else if (item == Field::FieldItem::grasshopper)
				m_field[k][j].push_back(new BabyGrassHopper(j, k, this));
			else if (item == Field::FieldItem::food)
				m_field[k][j].push_back(new Food(j, k, 6000, this));
			else if (item == Field::FieldItem::poison)
				m_field[k][j].push_back(new Poison(j, k, this));
			else if (item == Field::FieldItem::water)
				m_field[k][j].push_back(new Water(j, k, this));
			else if (item == Field::FieldItem::anthill0 && a[0] == true)
			{
				m_field[k][j].push_back(new AntHill(j, k, 0, this, compilerForEntrant[0]));
				m_numOfCol++;
			}
			else if (item == Field::FieldItem::anthill1 && a[1] == true)
			{
				m_field[k][j].push_back(new AntHill(j, k, 1, this, compilerForEntrant[1]));
				m_numOfCol++;
			}
			else if (item == Field::FieldItem::anthill2 && a[2] == true)
			{
				m_field[k][j].push_back(new AntHill(j, k, 2, this, compilerForEntrant[2]));
				m_numOfCol++;
			}
			else if (item == Field::FieldItem::anthill3 && a[3] == true)
			{
				m_field[k][j].push_back(new AntHill(j, k, 3, this, compilerForEntrant[3]));
				m_numOfCol++;
			}
		}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	for (int k = 0; k < 64; k++) //k is the row (y)
		for (int j = 0; j < 64; j++) //j is the column (x)
			for (list<Actor*>::iterator li = m_field[k][j].begin(); li != m_field[k][j].end(); li++)
				(*li)->setMoved(false);
	m_ticks++;
	for (int k = 0; k < 64; k++) //k is the row (y)
		for (int j = 0; j < 64; j++) //j is the column (x)
		{
			for (list<Actor*>::iterator li = m_field[k][j].begin(); li != m_field[k][j].end();)
			{
				if ((*li)->moved() == false)
				{
					(*li)->doSomething();
					(*li)->setMoved(true);
				}
				if ((*li)->getY() != k || (*li)->getX() != j)
				{
					m_field[(*li)->getY()][(*li)->getX()].push_back(*li);
					li = m_field[k][j].erase(li);
				}
				else
					li++;
			}
		}
	for (int k = 0; k < 64; k++) //k is the row (y)
		for (int j = 0; j < 64; j++) //j is the column (x)
		{
			list<Actor*>::iterator li = m_field[k][j].begin();
			for (li; li != m_field[k][j].end();)
				if ((*li)->isAlive() == false)
				{
					delete (*li);
					li = m_field[k][j].erase(li);
				}
				else
					li++;
		}
	setDisplayText();
	if (m_ticks < 2000)
		return GWSTATUS_CONTINUE_GAME;
	if (m_ticks == 2000)
		if (getWinningAntNumber() == -1)
			return GWSTATUS_NO_WINNER;
		else
		{
			vector<string> namesTemp = getFilenamesOfAntPrograms();
			setWinner(namesTemp[getWinningAntNumber()]);
			return GWSTATUS_PLAYER_WON;
		}
	else
		return GWSTATUS_NO_WINNER;
}

void StudentWorld::cleanUp()
{
	for (int k = 0; k < 64; k++) //k is the row (y)
		for (int j = 0; j < 64; j++) //j is the column (x)
		{
			for (list<Actor*>::iterator li = m_field[k][j].begin(); li != m_field[k][j].end(); li++)
			{
				delete *li;
			}
		}
	for (int m = 0; m != fileNames.size(); m++)
	{
		delete compilerForEntrant[m];
	}
}

bool StudentWorld::isThisBlocked(int x, int y)
{
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (; li != m_field[y][x].end(); li++)
		if ((*li)->blocksAnts() == true)
			return true;
	return false;
}

void StudentWorld::addFoodHere(int x, int y, int amount)
{
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (li; li != m_field[y][x].end(); li++)
	{
		if ((*li)->isFood() == true)
		{
			(*li)->adjustEnergy(amount);
			return;
		}
	}
	m_field[y][x].push_back(new Food(x, y, amount, this));
}

int StudentWorld::howMuchFood(int x, int y)
{
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (li; li != m_field[y][x].end(); li++)
	{
		if ((*li)->isFood() == true)
		{
			return (*li)->checkEnergy();
		}
	}
	return 0;
}

void StudentWorld::addPheromoneHere(int x, int y, Actor* me)
{
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (li; li != m_field[y][x].end(); li++)
	{
		if ((*li)->isPheromone() == true && (*li)->colony() == me->colony())
		{
			if (768 - (*li)->checkEnergy() > 0)
			{
				if (768 - (*li)->checkEnergy() <= 256)
				{
					(*li)->adjustEnergy(768 - (*li)->checkEnergy());
					return;
				}
				else
				{
					(*li)->adjustEnergy(256);
					return;
				}
			}
			
		}
	}
	m_field[y][x].push_back(new Pheromone(x, y, this, me->colony()));
}

bool StudentWorld::myPheromoneHere(int x, int y, Actor* me)
{
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (li; li != m_field[y][x].end(); li++)
		if ((*li)->isPheromone() == true && (*li)->colony() == me->colony())
			return true;
	return false;
}

void StudentWorld::addAntHere(int x, int y, int colID, Compiler* compilerCode, AntHill* anthill)
{
	m_field[y][x].push_back(new Ant(colID, x, y, this, compilerCode, anthill));
}

void StudentWorld::addAdultGrassHopperHere(int x, int y)
{
	m_field[y][x].push_back(new AdultGrassHopper(x, y, this));
}

bool StudentWorld::biteSomeoneHere(int x, int y, Actor* me)
{
	vector<int> pointerValue;
	int pointerRounds = 0;
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (li; li != m_field[y][x].end(); li++)
		if ((*li)->ableToBeBit() == true && (*li) != me && !((*li)->amIBit()) && ((me)->colony() == -1 || (*li)->colony() != me->colony()))
		{
			pointerValue.push_back(pointerRounds);
			pointerRounds++;
		}
	if (pointerValue.size() != 0)
	{
		int k = pointerValue[randInt(1, pointerValue.size())];
		li = m_field[y][x].begin();
		for (int m = 0; m != k; m++)
			li++;
		if ((me)->colony() == -1)
			(*li)->adjustEnergy(-50);
		else
			(*li)->adjustEnergy(-15);
		(*li)->setBitAlreadyState(true);
		if ((*li)->willBiteBack())
			biteSomeoneHere(x, y, *li);
		return true;
	}
	return false;
}

void StudentWorld::stunHere(int x, int y)
{
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (li; li != m_field[y][x].end(); li++)
	{
		if ((*li)->ableToBeStunned() && (*li)->stunnedAmount() == -1)
			(*li)->adjustStunned(3);
	};
}

void StudentWorld::poisonHere(int x, int y)
{
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (li; li != m_field[y][x].end(); li++)
	{
		if ((*li)->ableToBePoisoned())
			(*li)->adjustEnergy(-150);
	};
}

bool StudentWorld::dangerHere(int x, int y, Actor* me)
{
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (li; li != m_field[y][x].end(); li++)
		if ((*li)->isDangerous() == true && (*li)->colony() != me->colony())
			return true;
	return false;
}

bool StudentWorld::enemyHere(int x, int y, Actor* me)
{
	list<Actor*>::iterator li = m_field[y][x].begin();
	for (li; li != m_field[y][x].end(); li++)
		if ((*li)->ableToBeBit() == true && (*li)->isDangerous() == true && (*li)->colony() != me->colony())
			return true;
	return false;
}

void StudentWorld::increaseAntCol(int col)
{
	m_antNum[col]++;
}

int StudentWorld::getWinningAntNumber()
{
	vector<int> contest;
	for (int k = 0; k != m_numOfCol; k++)
		if (m_antNum[k] > 5)
			contest.push_back(k);
	if (contest.size() == 0)
		return -1;
	else
	{
		int winner = 0;
		int k = 0;
		for (; k != contest.size(); k++)
			if (m_antNum[k] > m_antNum[winner])
				winner = k;
		return k;
	}
}

int StudentWorld::leadingAnt()
{
	int lead = 0;
	for (int k = 0; k != m_numOfCol; k++)
		if (m_antNum[k] > m_antNum[lead])
			lead = k;
	return lead;
}

void StudentWorld::setDisplayText()
{
	int winningAntNumber = getWinningAntNumber();
	int leader = leadingAnt();
	string a[4] = {"", "", "", ""};
	a[leader] = "*";
	ostringstream text;
	int ticks = 2000 - m_ticks;
	text << "Ticks:" << setw(5) << ticks << " - " << compilerForEntrant[0]->getColonyName() << a[0] << ": ";
	text << text.fill('0') << setw(2) << m_antNum[0];
	if (fileNames.size() > 1)
		text << "  " << compilerForEntrant[1]->getColonyName() << a[1] << ": " << text.fill('0') << setw(2) << m_antNum[1];
	if (fileNames.size() > 2)
		text << "  " << compilerForEntrant[2]->getColonyName() << a[2] << ": " << text.fill('0') << setw(2) << m_antNum[2];
	if (fileNames.size() > 3)
		text << "  " << compilerForEntrant[3]->getColonyName() << a[3] << ": " << text.fill('0') << setw(2) <<  m_antNum[3];
	string s = text.str();
	setGameStatText(s);
}

GraphObject::Direction StudentWorld::randomdirection()
{
	int k = randInt(1, 4);
	if (k == 1)
		return GraphObject::Direction::up;
	else if (k == 2)
		return GraphObject::Direction::right;
	else if (k == 3)
		return GraphObject::Direction::down;
	else
		return GraphObject::Direction::left;
}