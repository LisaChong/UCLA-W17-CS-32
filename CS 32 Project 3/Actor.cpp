#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

#define _USE_MATH_DEFINES

Actor::Actor(int imageID, int startX, int startY, Direction direction, int depth, StudentWorld* sWorld)
		: m_sWorld(sWorld), GraphObject(imageID, startX, startY, direction, depth) {}
bool Actor::blocksAnts()
{
	return false;
}
bool Actor::isFood()
{
	return false;
}
void Actor::setAliveDead(bool value)
{
	m_alive = value;
}
bool Actor::isAlive()
{
	return m_alive;
}
void Actor::setMoved(bool value)
{
	m_moved = value;
}
bool Actor::moved()
{
	return m_moved;
}
StudentWorld* Actor::myWorld()
{
	return m_sWorld;
}

/////////////////////////////////////////////////////

Pebble::Pebble(int startX, int startY, StudentWorld* sWorld)
		: Actor(IID_ROCK, startX, startY, right, 1, sWorld) {}
bool Pebble::blocksAnts()
{
	return true;
}

/////////////////////////////////////////////////////

Water::Water(int startX, int startY, StudentWorld* sWorld)
	: Actor(IID_WATER_POOL, startX, startY, right, 2, sWorld) {}
void Water::stun()
{
	myWorld()->stunHere(getX(), getY());
}

/////////////////////////////////////////////////////

Poison::Poison(int startX, int startY, StudentWorld* sWorld)
	: Actor(IID_POISON, startX, startY, right, 2, sWorld) {}
void Poison::poison()
{
	myWorld()->poisonHere(getX(), getY());
}

/////////////////////////////////////////////////////

EnergyHolder::EnergyHolder(int imageID, int startX, int startY, int energy, Direction direction, int depth, StudentWorld* sWorld)
	: Actor(imageID, startX, startY, direction, depth, sWorld), m_energy(energy) {}
void EnergyHolder::adjustEnergy(int x) //Also sets to dead if dead
{
	m_energy += x;
	if (m_energy <= 0)
		setAliveDead(false); //Is it alive == false
	else
		setAliveDead(true);
}
int EnergyHolder::checkEnergy()
{
	return m_energy;
}

/////////////////////////////////////////////////////

Food::Food(int startX, int startY, int energy, StudentWorld* sWorld)
	: EnergyHolder(IID_FOOD, startX, startY, energy, right, 2, sWorld) {};
bool Food::isFood()
{
	return true;
}

/////////////////////////////////////////////////////

Pheromone::Pheromone(int startX, int startY, StudentWorld* sWorld, int colID)
	: EnergyHolder(colID + 11, startX, startY, 256, right, 2, sWorld), m_colonyID(colID) {};
void Pheromone::doSomething()
{
	adjustEnergy(-1);
}
int Pheromone::colony()
{
	return m_colonyID;
}

/////////////////////////////////////////////////////

AntHill::AntHill(int startX, int startY, int colony, StudentWorld* sWorld, Compiler* behavior)
	: EnergyHolder(IID_ANT_HILL, startX, startY, 8999, right, 2, sWorld), m_colID(colony), m_colonyCompiler(behavior) {};
void AntHill::doSomething()
{
	adjustEnergy(-1);
	if (checkEnergy() <= 0)
	{
		return;
	}
	int k = myWorld()->howMuchFood(getX(), getY());
	int j = 10000 - checkEnergy();
	if (k != 0 && j > 0)
	{
		if (j > k)
		{
			myWorld()->addFoodHere(getX(), getY(), -k);
			adjustEnergy(k);
		}
		else
		{
			myWorld()->addFoodHere(getX(), getY(), -j);
			adjustEnergy(j);
		}
		return;
	}
	if (checkEnergy() >= 2000)
	{
		myWorld()->addAntHere(getX(), getY(), m_colID, m_colonyCompiler, this);
		adjustEnergy(-1500);
		myWorld()->increaseAntCol(m_colID);
	}
}

/////////////////////////////////////////////////////

Insect::Insect(int imageID, int startX, int startY, int health, Direction direction, int depth, StudentWorld* sWorld)
		: EnergyHolder(imageID, startX, startY, health, direction, depth, sWorld) {}
bool Insect::adjustmentsAndSleep()
{
	adjustEnergy(-1);
	if (checkEnergy() <= 0)
	{
		myWorld()->addFoodHere(this->getX(), this->getY(), 100);
		return true;
	}
	ensureSleep();
	if (stunnedAmount() == 0)
	{
		adjustStunned(-1);
	}
	else if (stunnedAmount() != -1)
	{
		adjustStunned(-1);
		return true;
	}
	return false;
}
int Insect::stunnedAmount()
{
	return m_stunned;
}
void Insect::adjustStunned(int x)
{
	m_stunned += x;
}

/////////////////////////////////////////////////////

GrassHopper::GrassHopper(int imageID, int startX, int startY, int health, StudentWorld* sWorld)
	: Insect(imageID, startX, startY, health, myWorld()->randomdirection(), 1, sWorld)
{
	m_remainingDistance = randInt(2, 10);
}
bool GrassHopper::eatFoodHere()
{
	int k = myWorld()->howMuchFood(getX(), getY());
	if (k != 0)
	{
		int j = 200 - k;
		if (j <= 0)
		{
			myWorld()->addFoodHere(getX(), getY(), -200);
			adjustEnergy(200);
		}
		else
		{
			myWorld()->addFoodHere(getX(), getY(), -k);
			adjustEnergy(k);
		}
		if (randInt(0, 1) == 0)
		{
			adjustStunned(2 - stunnedAmount());
			return true;
		}
	}
	return false;
}
void GrassHopper::moveInADirection()
{
	GraphObject::Direction curDir = getDirection();
	int moveX = 0, moveY = 0;
	if (curDir == GraphObject::Direction::left)
		moveX--;
	else if (curDir == GraphObject::Direction::right)
		moveX++;
	else if (curDir == GraphObject::Direction::up)
		moveY++;
	else if (curDir == GraphObject::Direction::down)
		moveY--;
	int newX = getX() + moveX, newY = getY() + moveY;
	if (myWorld()->isThisBlocked(newX, newY) == true)
	{
		setRemainingDistance(0);
		adjustStunned(2 - stunnedAmount());
	}
	else
	{
		setRemainingDistance(remainingDistance() -1);
		moveTo(newX, newY);
	}
}
int GrassHopper::remainingDistance()
{
	return m_remainingDistance;
}
void GrassHopper::setRemainingDistance(int x)
{
	m_remainingDistance = x;
}
void GrassHopper::ensureSleep()
{
	if (m_sleep == 3)
	{
		if (stunnedAmount() == -1)
			adjustStunned(3);
		else
			adjustStunned(2);
		m_sleep--;
	}
	else if (m_sleep == 1)
		m_sleep = 3;
	else
		m_sleep--;
}

/////////////////////////////////////////////////////

BabyGrassHopper::BabyGrassHopper(int startX, int startY, StudentWorld* sWorld)
	: GrassHopper(IID_BABY_GRASSHOPPER, startX, startY, 500, sWorld) {}
void BabyGrassHopper::doSomething()
{
	if (adjustmentsAndSleep())
		return;
	if (checkEnergy() >= 1600)
	{
		myWorld()->addAdultGrassHopperHere(getX(), getY());
		setAliveDead(false);
		myWorld()->addFoodHere(this->getX(), this->getY(), 100);
		return;
	}
	if (eatFoodHere())
		return;
	if (remainingDistance() == 0)
	{
		setDirection(randomdirection());
		setRemainingDistance(randInt(2, 10));
	}
	moveInADirection();
}

/////////////////////////////////////////////////////

AdultGrassHopper::AdultGrassHopper(int startX, int startY, StudentWorld* sWorld)
	: GrassHopper(IID_ADULT_GRASSHOPPER, startX, startY, 1600, sWorld) {}
void AdultGrassHopper::doSomething()
{
	if (adjustmentsAndSleep())
		return;
	if (randInt(1, 3) == 1)
		if (myWorld()->biteSomeoneHere(getX(), getY(), this))
		{
			adjustStunned(2 - stunnedAmount());
			return;
		}
	if (randInt(0, 9) == 0)
	{
		int moveX = 10 * cos(randInt(0, 360) * 3.14159265358979 / 180);
		int moveY = 10 * sin(randInt(0, 360) * 3.14159265358979 / 180);
		int newX = getX() + moveX, newY = getY() + moveY;
		if (newX < 0 || newX > 63 || newY < 0 || newY > 63)
			return;
		if (myWorld()->isThisBlocked(newX, newY) == false)
		{
			moveTo(newX, newY);
			adjustStunned(2 - stunnedAmount());
			return;
		}
	}
	if (eatFoodHere())
		return;
	moveInADirection();
}

/////////////////////////////////////////////////////

Ant::Ant(int colID, int startX, int startY, StudentWorld* sWorld, Compiler* colonyCompiler, AntHill* antHill)
	: Insect(colID, startX, startY, 1500, myWorld()->randomdirection(), 1, sWorld), m_colonyID(colID), m_compiler(colonyCompiler), m_AntHill(antHill) {}
void Ant::doSomething()
{
	if (adjustmentsAndSleep())
		return;
	if (!interpreter())
	{
		setAliveDead(false);
		return;
	}
}
bool Ant::interpreter()
{
	if (m_instruction >= 10)
		m_instruction = 0;
	Compiler::Command cmd;
	for (int turns = 0; turns != 10; turns++)
	{
		if (!m_compiler->getCommand(m_instruction, cmd))
			return false;
		switch (cmd.opcode)
		{
		case Compiler::emitPheromone:
			myWorld()->addPheromoneHere(getX(), getY(), this);
			m_instruction++;
			return true;
			break;
		case Compiler::faceRandomDirection:
			setDirection(myWorld()->randomdirection());
			m_instruction++;
			return true;
			break;
		case Compiler::rotateClockwise:
			rotateDirection(1);
			m_instruction++;
			return true;
			break;
		case Compiler::rotateCounterClockwise:
			rotateDirection(-1);
			m_instruction++;
			return true;
			break;
		case Compiler::moveForward:
			moveInADirection();
			m_instruction++;
			return true;
			break;
		case Compiler::bite:
			myWorld()->biteSomeoneHere(getX(), getY(), this);
			m_instruction++;
			return true;
			break;
		case Compiler::pickupFood:
			pickUpFood();
			m_instruction++;
			return true;
			break;
		case Compiler::dropFood:
			myWorld()->addFoodHere(getX(), getY(), m_food);
			m_food = 0;
			m_instruction++;
			return true;
			break;
		case Compiler::eatFood:
			eatFood();
			m_instruction++;
			return true;
			break;
		case Compiler::generateRandomNumber:
		{
			int randNum = randInt(0, atoi(cmd.operand1.c_str()) - 1);
			if (randNum == 0 || atoi(cmd.operand1.c_str()) == 0)
				m_randomNumber = 0;
			m_instruction++; // advance to next instruction
			break;
		}
		case Compiler::goto_command:
			m_instruction = atoi(cmd.operand1.c_str());
			break;
		case Compiler::if_command:
			if (atoi(cmd.operand1.c_str()) == Compiler::Condition::i_am_carrying_food)
			{
				if (m_food > 0)
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			}
			else if (atoi(cmd.operand1.c_str()) == Compiler::Condition::i_am_hungry)
			{
				if (checkEnergy() <= 25)
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			}
			else if (atoi(cmd.operand1.c_str()) == Compiler::Condition::i_am_standing_on_food)
			{
				if (myWorld()->howMuchFood(getX(), getY()) > 0)
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			}
			else if (atoi(cmd.operand1.c_str()) == Compiler::Condition::i_am_standing_on_my_anthill)
			{
				if (getX() == m_AntHill->getX() && getY() == m_AntHill->getY())
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			}
			else if (atoi(cmd.operand1.c_str()) == Compiler::Condition::i_am_standing_with_an_enemy)
			{
				if (myWorld()->enemyHere(getX(), getY(), this) == true)
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			}
			else if (atoi(cmd.operand1.c_str()) == Compiler::Condition::i_smell_danger_in_front_of_me)
			{
				GraphObject::Direction curDir = getDirection();
				int moveX = 0, moveY = 0;
				if (curDir == GraphObject::Direction::left)
					moveX--;
				else if (curDir == GraphObject::Direction::right)
					moveX++;
				else if (curDir == GraphObject::Direction::up)
					moveY++;
				else if (curDir == GraphObject::Direction::down)
					moveY--;
				int newX = getX() + moveX, newY = getY() + moveY;
				if (myWorld()->dangerHere(newX, newY, this) == true)
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			}
			else if (atoi(cmd.operand1.c_str()) == Compiler::Condition::i_smell_pheromone_in_front_of_me)
			{
				GraphObject::Direction curDir = getDirection();
				int moveX = 0, moveY = 0;
				if (curDir == GraphObject::Direction::left)
					moveX--;
				else if (curDir == GraphObject::Direction::right)
					moveX++;
				else if (curDir == GraphObject::Direction::up)
					moveY++;
				else if (curDir == GraphObject::Direction::down)
					moveY--;
				int newX = getX() + moveX, newY = getY() + moveY;
				if (myWorld()->myPheromoneHere(newX, newY, this) == true)
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			}
			else if (atoi(cmd.operand1.c_str()) == Compiler::Condition::i_was_bit)
				if (m_bit == true)
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			else if (atoi(cmd.operand1.c_str()) == Compiler::Condition::i_was_blocked_from_moving)
				if (m_blocked == true)
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			else if (atoi(cmd.operand1.c_str()) == Compiler::Condition::last_random_number_was_zero)
			{
				if (m_randomNumber == 0)
				{
					m_instruction = atoi(cmd.operand2.c_str());
					break;
				}
			}
			m_instruction++;
			break;
		}
	}
	return true;
}
void Ant::moveInADirection()
{
	GraphObject::Direction curDir = getDirection();
	int moveX = 0, moveY = 0;
	if (curDir == GraphObject::Direction::left)
		moveX--;
	else if (curDir == GraphObject::Direction::right)
		moveX++;
	else if (curDir == GraphObject::Direction::up)
		moveY++;
	else if (curDir == GraphObject::Direction::down)
		moveY--;
	int newX = getX() + moveX, newY = getY() + moveY;
	if (myWorld()->isThisBlocked(newX, newY) == true)
		m_blocked = true;
	else
	{
		m_blocked = false;
		setBitAlreadyState(false);
		moveTo(newX, newY);
	}
}
void Ant::rotateDirection(int x)
{
	GraphObject::Direction curDir = getDirection();
	if (x > 0)
	{
		if (curDir == GraphObject::Direction::up)
			setDirection(GraphObject::Direction::right);
		else if (curDir == GraphObject::Direction::right)
			setDirection(GraphObject::Direction::down);
		else if (curDir == GraphObject::Direction::down)
			setDirection(GraphObject::Direction::left);
		else if (curDir == GraphObject::Direction::left)
			setDirection(GraphObject::Direction::up);
	}
	else
	{
		if (curDir == GraphObject::Direction::up)
			setDirection(GraphObject::Direction::left);
		else if (curDir == GraphObject::Direction::left)
			setDirection(GraphObject::Direction::down);
		else if (curDir == GraphObject::Direction::down)
			setDirection(GraphObject::Direction::right);
		else if (curDir == GraphObject::Direction::right)
			setDirection(GraphObject::Direction::up);
	}
}
void Ant::eatFood()
{
	if (m_food >= 100)
	{
		adjustEnergy(100);
		m_food -= 100;
	}
	else
	{
		adjustEnergy(m_food);
		m_food = 0;
	}
}
void Ant::pickUpFood()
{
	int k = myWorld()->howMuchFood(getX(), getY());
	int l = 1800 - m_food;
	if (k != 0 && l != 0)
	{
		if (l >= 400)
		{
			int j = 400 - k;
			if (j <= 0)
			{
				myWorld()->addFoodHere(getX(), getY(), -400);
				m_food += 400;
			}
			else
			{
				myWorld()->addFoodHere(getX(), getY(), -k);
				m_food += k;
			}
		}
		else
		{
			int m = l - k;
			if (m >= 0)
			{
				myWorld()->addFoodHere(getX(), getY(), -k);
				m_food += k;
			}
			else
			{
				myWorld()->addFoodHere(getX(), getY(), -l);
				m_food += l;
			}
		}
	}
}
bool Ant::amIBit()
{
	return m_bit;
}
void Ant::setBitAlreadyState(bool value)
{
	m_bit = value;
}
int Ant::colony()
{
	return m_colonyID;
}

/////////////////////////////////////////////////////