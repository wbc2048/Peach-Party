#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <list>
#include <vector>
#include "Actor.h"


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool notEmpty(int angle, int x, int y);
	bool peachHasLandedOn(int x, int y);
	bool yoshiHasLandedOn(int x, int y);
	int getBank();
	bool isAtDS(int x, int y);
	void dropTheSquare(Bowser* b);
	void createVortex(PlayerAvatar* p);
	void addBank(int c);
	void clearBank();
	
	void teleport(Actor* a) ;
	void teleportBaddie(Actor* a);

	void swap();
	std::string displayVortex(PlayerAvatar* a);
	int winner();
	Actor* overlap(Actor* a);
	Peach* getPeach();
	Yoshi* getYoshi();




private:
	std::list<Actor*> m_allActors;
	Peach* m_peach;
	Yoshi* m_yoshi;
	int m_moneyInBank;
	bool m_alreadyClean;
};

#endif // STUDENTWORLD_H_