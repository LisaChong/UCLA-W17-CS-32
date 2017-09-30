#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction direction, int depth, StudentWorld* sWorld);
	virtual void doSomething() {};
	virtual bool blocksAnts();
	virtual bool isFood();
	virtual bool isPheromone() { return false; };
	virtual bool isDangerous() { return false; };
	void setAliveDead(bool value);
	bool isAlive();
	void setMoved(bool value);
	bool moved();
	virtual void adjustEnergy(int x) {};
	virtual int checkEnergy() { return -1; };
	virtual bool ableToBeStunned() { return false; };
	virtual int stunnedAmount() { return -1; };
	virtual void adjustStunned(int x) {};
	virtual bool ableToBePoisoned() { return false; };
	virtual bool ableToBeBit() { return false; };
	virtual bool willBiteBack() { return false; };
	virtual bool amIBit() { return false;  };
	virtual void setBitAlreadyState(bool value) {};
	virtual int colony() { return -1; };
	StudentWorld* myWorld();
private:
	bool m_alive = true;
	StudentWorld* m_sWorld;
	bool m_moved = false;
};

class Pebble : public Actor
{
public:
	Pebble(int startX, int startY, StudentWorld* sWorld);
	virtual bool blocksAnts();
};

class Water : public Actor
{
public:
	Water(int startX, int startY, StudentWorld* sWorld);
	virtual bool isDangerous() { return true; }; 
	void stun();
};

class Poison : public Actor
{
public:
	Poison(int startX, int startY, StudentWorld* sWorld);
	virtual bool isDangerous() { return true; };
	void poison();
};

class EnergyHolder : public Actor
{
public:
	EnergyHolder(int imageID, int startX, int startY, int energy, Direction direction, int depth, StudentWorld* sWorld);
	void adjustEnergy(int x);
	int checkEnergy();
private:
	int m_energy = 0;
};

class Food : public EnergyHolder
{
public:
	Food(int startX, int startY, int energy, StudentWorld* sWorld);
	virtual bool isFood();
private:
};

class Pheromone : public EnergyHolder
{
public:
	Pheromone(int startX, int startY, StudentWorld* sWorld, int colID);
	virtual void doSomething();
	virtual bool isPheromone() { return true; };
	virtual int colony();
private:
	int m_colonyID;
};

class AntHill : public EnergyHolder
{
public:
	AntHill(int startX, int startY, int colony, StudentWorld* sWorld, Compiler* object);
	virtual void doSomething();
private:
	Compiler* m_colonyCompiler;
	int m_colID;
};

class Insect : public EnergyHolder
{
public:
	Insect(int imageID, int startX, int startY, int health, Direction direction, int depth, StudentWorld* sWorld);
	bool adjustmentsAndSleep();
	virtual int stunnedAmount();
	virtual void adjustStunned(int x);
	bool ableToBeBit() { return true; };
	virtual bool isDangerous() { return true; };
private:
	virtual void ensureSleep() {};
	int m_stunned = -1;
};

class GrassHopper : public Insect
{
public:
	GrassHopper(int imageID, int startX, int startY, int health, StudentWorld* sWorld);
	bool eatFoodHere();
	void moveInADirection();
	int remainingDistance();
	void setRemainingDistance(int x);
private:
	virtual void ensureSleep();
	int m_sleep = 1;
	int m_remainingDistance = 0;
};

class BabyGrassHopper : public GrassHopper
{
public:
	BabyGrassHopper(int startX, int startY, StudentWorld* sWorld);
	virtual void doSomething();
	virtual bool ableToBeStunned() { return true; };
	virtual bool ableToBePoisoned() { return true; };
private:
};

class AdultGrassHopper : public GrassHopper
{
public:
	AdultGrassHopper(int startX, int startY, StudentWorld* sWorld);
	virtual void doSomething();
	virtual bool willBiteBack() { return true; };
private:
};

class Ant : public Insect
{
public:
	Ant(int colID, int startX, int startY, StudentWorld* sWorld, Compiler* colonyCompiler, AntHill* antHill);
	virtual void doSomething();
	virtual bool amIBit();
	virtual void setBitAlreadyState(bool value);
	virtual int colony();
private:
	bool interpreter();
	virtual void moveInADirection();
	void rotateDirection(int x);
	void eatFood();
	void pickUpFood();
	bool m_bit = false;
	bool m_blocked = false;
	int m_colonyID;
	int m_food = 0;
	int m_randomNumber;
	int m_instruction = 0;
	AntHill* m_AntHill;
	Compiler* m_compiler;
};

#endif // ACTOR_H_
