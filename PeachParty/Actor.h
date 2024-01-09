#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include <iostream>
#include <algorithm>


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/* 
Hierarchy

GraphObject
	Actors
		PlayerAvatar
		ActivatingObject
			ActivateOnPlayer
				BankSquare
				CoinSquare
					BlueCoinSquare
					RedCoinSquare
				DirectionalSquare
				DroppingSquare
					Baddie 
						Bowser
						Boo
				EventSquare
				StarSquare
			Vortex
*/

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, int startDirection, int depth, double size, StudentWorld* sw);
	StudentWorld* getWorld();
	virtual void doSomething() = 0;
	bool isActive();
	void changeToActive();
	void changeToNotActive();
	bool atFork();
	
	int randomDirection();
	virtual bool isDS();
	void setSprite(int dir);
	virtual bool canDelete();
	virtual bool impactable();

	virtual void setCounter();
	virtual void changeState(bool tf);
	virtual void changeWalkDirection(int dir);


private:
	StudentWorld* m_studentworld;
	bool m_isActive;
};
class ActivatingObject : public Actor
{
public:
	ActivatingObject(int imageID, int startX, int startY, int startDirection, int depth, double size, StudentWorld* sw) : Actor(imageID, startX, startY, startDirection, depth, size, sw) {};
	bool peachLandedOn();
	bool peachMovedOn();
	bool yoshiLandedOn();
	bool yoshiMovedOn();
};
class ActivateOnPlayer : public ActivatingObject
{
public:
	ActivateOnPlayer(int imageID, int startX, int startY, int startDirection, int depth, double size, StudentWorld* sw) : ActivatingObject(imageID, startX, startY, startDirection, depth, size, sw) {};
};

class Vortex : public ActivatingObject
{
public:
	Vortex(int startX, int startY, StudentWorld* sw, int dir);
	virtual void doSomething();
	int getShootDirection();
private:
	int m_direction;
};

class CoinSquare : public ActivateOnPlayer
{
public:
	CoinSquare(int imageID, int startX, int startY, StudentWorld* sw);
	virtual void doSomething() = 0;
	virtual bool canDelete();

};

class BlueCoinSquare : public CoinSquare
{
public:
	BlueCoinSquare(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();

};
class RedCoinSquare : public CoinSquare
{
public:
	RedCoinSquare(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();

};

class StarSquare : public ActivateOnPlayer
{
public:
	StarSquare(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();

};

class DirectionalSquare : public ActivateOnPlayer
{
public:
	DirectionalSquare(int startX, int startY, StudentWorld* sw, int dir);
	virtual void doSomething();
	virtual bool isDS();
private:
	int m_forcing_direction;
};

class BankSquare : public ActivateOnPlayer
{
public:
	BankSquare(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();

};

class EventSquare : public ActivateOnPlayer
{
public:
	EventSquare(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();

};
class DroppingSquare : public ActivateOnPlayer
{
public:
	DroppingSquare(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();

};


class Baddie : public ActivateOnPlayer
{
public:
	Baddie(int imageID, int startX, int startY, StudentWorld* sw);
	virtual void doSomething() = 0;
	bool getState();
	void changeState(bool tf);
	int getCounter();
	void setCounter();
	void decrementCounter();
	void changeWalkDirection(int dir);
	int getWalkDirection();
	void setTicks(int s);
	int getTicks();
	void decrementTicks();
	virtual bool impactable();
	virtual bool canDelete();

private:
	int m_travel;
	bool m_state; //true is wait and false is walking
	int m_counter;
	int m_walkingDirection;
	int m_ticks_to_move;
};

class Bowser : public Baddie
{
public:
	Bowser(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();
	
};

class Boo : public Baddie
{
public:
	Boo(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();
};


class PlayerAvatar : public Actor
{
public:
	PlayerAvatar(int imageID, int playerNum, int startX, int startY, StudentWorld* sw);
	virtual void doSomething();
	int getCoins();
	int getStars();
	int getRoll();
	bool validDirection(int d);
	int getTick() ;
	
	void changeTick(int t) ;
	void setCoins(int c);
	void setStars(int c);
	void addCoins(int c) ;
	void deductCoins(int c) ;
	void deductStars(int c) ;
	void giveStar() ;
	bool getState() ;
	void changeState(bool s) ;
	bool getNew() ;
	void changeNewTo(bool tf);
	void changeWalkDirection(int dir) ;
	int getWalkDirection() ;
	bool hasVortex() ;
	void changeVortex(bool b) ;
	
private:
	int m_ticks_to_move;
	bool m_state; //true is wait and false is walk
	int m_coins;
	int m_stars;
	int m_playerNum;
	int m_walkingDirection; //999 is invalid
	bool m_isNew;
	bool m_hasVortex;
};


class Peach: public PlayerAvatar
{
public:
	Peach(int startX, int startY, StudentWorld* sw);
	
};

class Yoshi : public PlayerAvatar
{
public:
	Yoshi(int startX, int startY, StudentWorld* sw);
};
#endif // ACTOR_H_
