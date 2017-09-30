#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include <map>
#include <list>
#include <string>

using namespace std;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool isThisBlocked(int x, int y);
	void addFoodHere(int x, int y, int amount);
	int howMuchFood(int x, int y);
	void addPheromoneHere(int x, int y, Actor* me);
	bool myPheromoneHere(int x, int y, Actor* me);
	void addAntHere(int x, int y, int colID, Compiler* compilerCode, AntHill* anthill);
	void addAdultGrassHopperHere(int x, int y);
	bool biteSomeoneHere(int x, int y, Actor* me);
	void stunHere(int x, int y);
	void poisonHere(int x, int y);
	bool dangerHere(int x, int y, Actor* me);
	bool enemyHere(int x, int y, Actor* me);
	void increaseAntCol(int col);
	GraphObject::Direction randomdirection();
private:
	void setDisplayText();
	int getWinningAntNumber();
	int leadingAnt();
	int m_ticks = 0;
	int m_antNum[4] = { 0 };
	int m_numOfCol = 0;
	Compiler* compilerForEntrant[4];
	vector<string> fileNames;
	map<int, map<int, list<Actor*>>> m_field;
};

#endif // STUDENTWORLD_H_
