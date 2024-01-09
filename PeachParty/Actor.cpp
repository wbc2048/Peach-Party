#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, int startX, int startY,
	int startDirection, int depth, double size, StudentWorld* sw) :
	GraphObject(imageID, startX, startY, startDirection, depth, size), m_studentworld(sw), m_isActive(true) {}


StudentWorld* Actor::getWorld() { return m_studentworld; }
bool Actor::isDS() { //identifier for directional square
	return false;
}
void Actor::setSprite(int dir) { // set sprite direction according to spec
	if (dir == left) 
		setDirection(left);
	else
		setDirection(right);

	return;
}

bool Actor::canDelete() { //identifier for coin square for it to be dropped
	return false;
}
bool Actor::impactable() { //idenifier for baddies to be hit by vortex
	return false;
}

void Actor::setCounter() { //counte for baddie
	return;
}
void Actor::changeState(bool tf) {
	return;
}
void Actor::changeWalkDirection(int dir) {
	return;
}


bool Actor::isActive() {
	return m_isActive;
};

void Actor::changeToActive() {
	m_isActive = true;
}
void Actor::changeToNotActive() {
	m_isActive = false;
}
bool Actor::atFork() {
	
	if (getWorld()->isAtDS(getX(), getY())) //using numbers here in order for it to work on g32
		return false;
	std::vector<int> dir;
	if (getWorld()->notEmpty(90, getX(), getY())) //up
		dir.push_back(90);
	if (getWorld()->notEmpty(270, getX(), getY())) //down
		dir.push_back(270);
	if (getWorld()->notEmpty(0, getX(), getY())) //right
		dir.push_back(0);
	if (getWorld()->notEmpty(180, getX(), getY())) //left
		dir.push_back(180);
	if (dir.size() >= 3) //if more then 3 directions are movable then it is a fork
		return true;
	else
		return false;
}
int Actor::randomDirection() {
	std::vector<int> dir;
	if (getWorld()->notEmpty(90, getX(), getY())) //up
		dir.push_back(90);
	if (getWorld()->notEmpty(270, getX(), getY()))//down
		dir.push_back(270);
	if (getWorld()->notEmpty(0, getX(), getY())) //right
		dir.push_back(0);
	if (getWorld()->notEmpty(180, getX(), getY())) //left
		dir.push_back(180);
	int r = rand() % dir.size();
	return dir[r];
}
CoinSquare::CoinSquare(int imageID, int startX, int startY, StudentWorld* sw)
	:ActivateOnPlayer(imageID, startX, startY, 0, 1, 1, sw)
{}; 

bool CoinSquare::canDelete() {
	return true;
}
// defining location helper functions for all the squares to use
bool ActivatingObject::peachLandedOn() { 
	return (getWorld()->getPeach()->getState() && getWorld()->peachHasLandedOn(getX(), getY()) && getWorld()->getPeach()->getNew());
}
bool ActivatingObject::peachMovedOn() {
	return (!getWorld()->getPeach()->getState() && getWorld()->peachHasLandedOn(getX(), getY()) && getWorld()->getPeach()->getNew());
}
bool ActivatingObject::yoshiLandedOn() {
	return (getWorld()->getYoshi()->getState() && getWorld()->yoshiHasLandedOn(getX(), getY()) && getWorld()->getYoshi()->getNew());
}
bool ActivatingObject::yoshiMovedOn() {
	return (!getWorld()->getYoshi()->getState() && getWorld()->yoshiHasLandedOn(getX(), getY()) && getWorld()->getYoshi()->getNew());
}
BlueCoinSquare::BlueCoinSquare(int startX, int startY, StudentWorld* sw)
	:CoinSquare(IID_BLUE_COIN_SQUARE, startX, startY, sw) {};

void BlueCoinSquare::doSomething() {

	if (isActive()) {
		if (peachLandedOn()) {
			getWorld()->getPeach()->addCoins(3);
			getWorld()->playSound(SOUND_GIVE_COIN);
		}
		if (yoshiLandedOn()) {
			getWorld()->getYoshi()->addCoins(3);
			getWorld()->playSound(SOUND_GIVE_COIN);
		}
		return;
	}
	else
		return;
};
RedCoinSquare::RedCoinSquare(int startX, int startY, StudentWorld* sw)
	:CoinSquare(IID_RED_COIN_SQUARE, startX, startY, sw) {};

void RedCoinSquare::doSomething() {

	if (isActive()) {
		if (peachLandedOn()) {
			getWorld()->getPeach()->deductCoins(3);
			getWorld()->playSound(SOUND_TAKE_COIN);
		}
		if (yoshiLandedOn()) {
			getWorld()->getYoshi()->deductCoins(3);
			getWorld()->playSound(SOUND_TAKE_COIN);
		}
		return;
	}
	else
		return;

};

StarSquare::StarSquare(int startX, int startY, StudentWorld* sw)
: ActivateOnPlayer(IID_STAR_SQUARE, startX, startY, 0, 1, 1, sw){}

void StarSquare::doSomething() {
	//if u passed it or land on it
	if (peachLandedOn() || peachMovedOn()) {
		if (getWorld()->getPeach()->getCoins() >= 20) {
			getWorld()->getPeach()->giveStar();
			getWorld()->playSound(SOUND_GIVE_STAR);
		}
		
	}
	if (yoshiLandedOn() || yoshiMovedOn()) {
		if (getWorld()->getYoshi()->getCoins() >= 20) {
			getWorld()->getYoshi()->giveStar();
			getWorld()->playSound(SOUND_GIVE_STAR);
		}
	}
	return;

}

DirectionalSquare::DirectionalSquare(int startX, int startY, StudentWorld* sw, int dir)
	:ActivateOnPlayer(IID_DIR_SQUARE, startX, startY, dir, 1, 1, sw), m_forcing_direction(dir) {};

bool DirectionalSquare::isDS() { //so that we can tell which actors are directional squares
	return true;
}
void DirectionalSquare::doSomething() {
	if (peachLandedOn() || peachMovedOn()) {
		getWorld()->getPeach()->changeWalkDirection(m_forcing_direction);
	}
	if (yoshiLandedOn() || yoshiMovedOn()) {
		getWorld()->getYoshi()->changeWalkDirection(m_forcing_direction);
	}
}



BankSquare::BankSquare(int startX, int startY, StudentWorld* sw)
	:ActivateOnPlayer(IID_BANK_SQUARE, startX, startY, 0, 1, 1, sw) {};

void BankSquare::doSomething() {
	if (peachMovedOn()) {
		getWorld()->getPeach()->deductCoins(5);
		getWorld()->addBank(5);
		getWorld()->playSound(SOUND_DEPOSIT_BANK);
	}

	else if (peachLandedOn()) {
		getWorld()->getPeach()->addCoins(getWorld()->getBank());
		getWorld()->clearBank();
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
	}
	
	if (yoshiMovedOn()) {
		getWorld()->getYoshi()->deductCoins(5);
		getWorld()->addBank(5);
		getWorld()->playSound(SOUND_DEPOSIT_BANK);
	}
	else if (yoshiLandedOn()){
		getWorld()->getYoshi()->addCoins(getWorld()->getBank());
		getWorld()->clearBank();
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
	}
	
	return;
};

EventSquare::EventSquare(int startX, int startY, StudentWorld* sw)
	:ActivateOnPlayer(IID_EVENT_SQUARE, startX, startY, 0, 1, 1, sw) {};

void EventSquare::doSomething() {
	
	if (peachLandedOn()) {
		//std::cerr << getWorld()->getPeach()->getNew() << std::endl;
		int i = rand() % 3 + 1;
		//std::cerr << i << std::endl;
		if (i == 1) {
			getWorld()->teleport(getWorld()->getPeach());
			getWorld()->getPeach()->changeWalkDirection(999);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			//std::cerr << "peach teleported!" << std::endl;
			
		}
		else if (i == 2) {
			getWorld()->swap();
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			std::cerr << "swapped!" << std::endl;
			//getWorld()->getYoshi()->changeNewTo(false);
			
		}
		else if (i == 3) {
			//std::cerr << "gave vortex to peach!" << std::endl;
			if (!getWorld()->getPeach()->hasVortex()) {
				getWorld()->getPeach()->changeVortex(true);
				getWorld()->playSound(SOUND_GIVE_VORTEX);
				
			}
		}
	}
	if (yoshiLandedOn()) {
		int i = rand() % 3 + 1;
		if (i == 1) {
			getWorld()->teleport(getWorld()->getYoshi());
			getWorld()->getYoshi()->changeWalkDirection(999);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			//std::cerr << "yoshi teleported!" << std::endl;
		}
		else if (i == 2) {
			getWorld()->swap();
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			//getWorld()->getPeach()->changeNewTo(false);
			//std::cerr << "swapped!" << std::endl;
		}
		else if (i == 3) {
			//std::cerr << "gave vortex to yoshi!" << std::endl;
			if (!getWorld()->getYoshi()->hasVortex()) {
				getWorld()->getYoshi()->changeVortex(true);
				getWorld()->playSound(SOUND_GIVE_VORTEX);
			}
		}
	}
	return;
}


DroppingSquare::DroppingSquare(int startX, int startY, StudentWorld* sw)
	:ActivateOnPlayer(IID_DROPPING_SQUARE, startX, startY, 0, 1, 1, sw) {};

void DroppingSquare::doSomething() {
	if (peachLandedOn()) {
		//std::cerr << "landed on dropping square" << std::endl;
		if (getWorld()->getPeach()->getCoins() >= 10 && getWorld()->getPeach()->getStars() >= 1) {
			int i = rand() % 2 + 1;
			if (i == 1)
				getWorld()->getPeach()->deductCoins(10);
			else
				getWorld()->getPeach()->deductStars(1);
		}
		else if (getWorld()->getPeach()->getCoins() >=  10)
			getWorld()->getPeach()->deductCoins(10);
		else if (getWorld()->getPeach()->getStars() >=  1)
			getWorld()->getPeach()->deductStars(1);
		else if (getWorld()->getPeach()->getStars() == 0 && getWorld()->getPeach()->getCoins() < 10) {
			getWorld()->getPeach()->deductCoins(10);
		}
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
	}
	if (yoshiLandedOn()) {
		//std::cerr << "landed on dropping square" << std::endl;
		if (getWorld()->getYoshi()->getCoins() >= 10 && getWorld()->getYoshi()->getStars() >= 1) {
			int i = rand() % 2 + 1;
			if (i == 1)
				getWorld()->getYoshi()->deductCoins(10);
			else
				getWorld()->getYoshi()->deductStars(1);
		}
		else if (getWorld()->getYoshi()->getCoins() >= 10)
			getWorld()->getYoshi()->deductCoins(10);
		else if (getWorld()->getYoshi()->getStars() >= 1)
			getWorld()->getYoshi()->deductStars(1);
		else if (getWorld()->getYoshi()->getStars() == 0 && getWorld()->getYoshi()->getCoins() < 10) {
			getWorld()->getYoshi()->deductCoins(10);
		}
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
	}
	return;
}

Baddie::Baddie(int imageID, int startX, int startY, StudentWorld* sw)
	:ActivateOnPlayer(imageID, startX, startY, 0, 0, 1, sw), m_travel(0), m_state(true), m_counter(180), m_walkingDirection(0), m_ticks_to_move(0){};
bool Baddie::getState() {
	return m_state;
}
void Baddie::changeState(bool tf) {
	m_state = tf;
}
int Baddie::getCounter() {
	return m_counter;
}
void Baddie::setCounter() {
	m_counter = 180;
}
void Baddie::decrementCounter() {
	m_counter--;
}
void Baddie::changeWalkDirection(int dir) {
	m_walkingDirection = dir;
}
int Baddie::getWalkDirection() {
	return m_walkingDirection;
}
void Baddie::setTicks(int s) {
	m_ticks_to_move = s;
}
int Baddie::getTicks() {
	return m_ticks_to_move;
}
void Baddie::decrementTicks() {
	m_ticks_to_move--;
}
bool Baddie::impactable() {
	return true;
}
bool Baddie::canDelete() {
	return false;
}
Bowser::Bowser(int startX, int startY, StudentWorld* sw)
	:Baddie(IID_BOWSER, startX, startY, sw) {};

void Bowser::doSomething() {
	if (getState() == true) {
		if (peachLandedOn()) {
			int i = rand() % 2 + 1;
			if (i == 1) {
			//if(true){
				getWorld()->getPeach()->deductCoins(getWorld()->getPeach()->getCoins());
				getWorld()->getPeach()->deductStars(getWorld()->getPeach()->getStars());
				getWorld()->playSound(SOUND_BOWSER_ACTIVATE);

			}
		}
		if (yoshiLandedOn()) {
			int i = rand() % 2 + 1;
			if (i == 1) {
				getWorld()->getYoshi()->deductCoins(getWorld()->getYoshi()->getCoins());
				getWorld()->getYoshi()->deductStars(getWorld()->getYoshi()->getStars());
				getWorld()->playSound(SOUND_BOWSER_ACTIVATE);

			}
		}
		if (getCounter() > 0)
			decrementCounter();
		else { //counter == 0
			int squares_to_move = rand() % 10 + 1;
			setTicks(squares_to_move * 8);
			changeWalkDirection(randomDirection());
			
			changeState(false);
		}
	}
	else {
		setSprite(getWalkDirection());
		
		if (atFork()) {
			//std::cerr << "at fork" << std::endl;
			int r = randomDirection();
			changeWalkDirection(r);
			setSprite(r);
		}
		else {
			//std::cerr << "not at fork" << std::endl;
			if (!getWorld()->notEmpty(getWalkDirection(), getX(), getY())) {
				if (getWalkDirection() == right || getWalkDirection() == left) {
					if (getWorld()->notEmpty(up, getX(), getY())) {
						setDirection(right);
						changeWalkDirection(up);
					}
					else {
						setDirection(right);
						changeWalkDirection(down);
					}
				}
				else {
					if (getWorld()->notEmpty(right, getX(), getY())) {
						setDirection(right);
						changeWalkDirection(right);
					}
					else {
						setDirection(left);
						changeWalkDirection(left);
					}

				}
			}
			
		}

		if (getTicks() == 0) {
			changeState(true);
			setCounter();
			int i = rand() % 4 + 1;
			if (i == 1) {
				//std::cerr << "drop" << std::endl;
				getWorld()->dropTheSquare(this);
				getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
			}

		}
		else {
			moveAtAngle(getWalkDirection(), 2);
			decrementTicks();
		}

	}
	
	return;
}

Boo::Boo(int startX, int startY, StudentWorld* sw)
	:Baddie(IID_BOO, startX, startY, sw) {};

void Boo::doSomething() {
	if (getState() == true) {
		if (peachLandedOn()) {
			int i = rand() % 2 + 1;
			if (i == 1) {
				//if(true){
				int c = getWorld()->getPeach()->getCoins();
				getWorld()->getPeach()->setCoins(getWorld()->getYoshi()->getCoins());
				getWorld()->getYoshi()->setCoins(c);
				getWorld()->playSound(SOUND_BOO_ACTIVATE);

			}
			if (i == 2) {
				int c = getWorld()->getPeach()->getStars();
				getWorld()->getPeach()->setStars(getWorld()->getYoshi()->getStars());
				getWorld()->getYoshi()->setStars(c);
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
			}
		}
		if (yoshiLandedOn()) {
			int i = rand() % 2 + 1;
			if (i == 1) {
				//if(true){
				int c = getWorld()->getPeach()->getCoins();
				getWorld()->getPeach()->setCoins(getWorld()->getYoshi()->getCoins());
				getWorld()->getYoshi()->setCoins(c);
				getWorld()->playSound(SOUND_BOO_ACTIVATE);

			}
			if (i == 2) {
				int c = getWorld()->getPeach()->getStars();
				getWorld()->getPeach()->setStars(getWorld()->getYoshi()->getStars());
				getWorld()->getYoshi()->setStars(c);
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
			}
		}
		if (getCounter() > 0)
			decrementCounter();
		else { //counter == 0
			int squares_to_move = rand() % 3 + 1;
			setTicks(squares_to_move * 8);
			changeWalkDirection(randomDirection());

			changeState(false);
		}
	}
	else {
		setSprite(getWalkDirection());

		if (atFork()) {
			//std::cerr << "at fork" << std::endl;
			int r = randomDirection();
			changeWalkDirection(r);
			setSprite(r);
		}
		else {
			//std::cerr << "not at fork" << std::endl;
			if (!getWorld()->notEmpty(getWalkDirection(), getX(), getY())) {

				if (getWalkDirection() == right || getWalkDirection() == left) {
					if (getWorld()->notEmpty(up, getX(), getY())) {
						setDirection(right);
						changeWalkDirection(up);
					}
					else {
						setDirection(right);
						changeWalkDirection(down);
					}

				}
				else {
					if (getWorld()->notEmpty(right, getX(), getY())) {
						setDirection(right);
						changeWalkDirection(right);
					}
					else {
						setDirection(left);
						changeWalkDirection(left);
					}

				}
			}

		}

		if (getTicks() == 0) {
			changeState(true);
			setCounter();
		
		}
		else {
			moveAtAngle(getWalkDirection(), 2);
			decrementTicks();
		}

	}

	return;
}


Vortex::Vortex(int startX, int startY, StudentWorld* sw, int dir) 
	:ActivatingObject(IID_VORTEX, startX, startY, 0, 0, 1, sw), m_direction(dir){};

int Vortex::getShootDirection() {
	return m_direction;
}
void Vortex::doSomething() {
	if (isActive()) {
		//std::cerr << "moved" << std::endl;
		moveAtAngle(m_direction, 2);
		std::cerr << getX() << std::endl;
		if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT) {
			//std::cerr << "not active now" << std::endl;
			changeToNotActive();
		}

		if (getWorld()->overlap(this) != nullptr) {
			//std::cerr << "overlap" << std::endl;
			getWorld()->teleportBaddie(getWorld()->overlap(this));
			changeToNotActive();
			getWorld()->playSound(SOUND_HIT_BY_VORTEX);
		}
		
		return;
	}
	else
		return;
}

PlayerAvatar::PlayerAvatar(int imageID, int playerNum, int startX, int startY, StudentWorld* sw)
	: Actor(imageID, startX, startY, 0, 0, 1, sw), m_ticks_to_move(0), m_state(true), m_coins(0), m_stars(0), m_playerNum(playerNum), m_walkingDirection(right), m_isNew(true), m_hasVortex(false) {};




void PlayerAvatar::doSomething() {
	//m_isNew = true;
	if (m_state == true) {
		m_isNew = false;
		if (m_walkingDirection == 999) { //this means invalid position
			int r = randomDirection();
			changeWalkDirection(r);
			setSprite(r);
		}
		
		int i = getWorld()->getAction(m_playerNum);
		int die_roll;
		switch (i) 
		{
		case ACTION_ROLL:
			std::cerr << "pressed" << std::endl;
			die_roll = rand() % 10 + 1;
			//die_roll = 1;
			m_ticks_to_move = die_roll * 8;
			m_state = false;
			break;
		case ACTION_FIRE:
			if (m_hasVortex == true) {
				
				getWorld()->createVortex(this);
				
				m_hasVortex = false;
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				std::cerr << "fire" << std::endl;
			}
			break;
		default:
			return;
		}
	}
	if (m_state == false) {
		m_isNew = false;
		if (atFork()) {
			//std::cerr << "at fork" << std::endl;
			int a = getWorld()->getAction(m_playerNum);
			switch (a) {
			case ACTION_RIGHT:
				if (validDirection(right)) {
					//std::cerr << "walk right" << std::endl;
					setDirection(right);
					m_walkingDirection = right;
					moveAtAngle(m_walkingDirection, 2);
					m_ticks_to_move--;
					return;
				}
				else
					return;
			case ACTION_LEFT:
				if (validDirection(left)) {
					//std::cerr << "walk left" << std::endl;
					setDirection(left);
					m_walkingDirection = left;
					moveAtAngle(m_walkingDirection, 2);
					m_ticks_to_move--;
					return;
				}
				else
					return;
			case ACTION_UP:
				//std::cerr << "pressed up" << std::endl;
				if (validDirection(up)) {
					
					setDirection(right);
					m_walkingDirection = up;
					std::cerr << "walk up" << std::endl;
					moveAtAngle(m_walkingDirection, 2);
					m_ticks_to_move--;
					return;
				}
				else
					return;
			case ACTION_DOWN:
				if (validDirection(down)) {
					std::cerr << "walk down" << std::endl;
					setDirection(right);
					m_walkingDirection = down;
					moveAtAngle(m_walkingDirection, 2);
					m_ticks_to_move--;
					return;
				}
				else
					return;
			default:
				return;
			}
		}
		
		else if (!getWorld()->notEmpty(m_walkingDirection, getX(), getY())){
			if (m_walkingDirection == right || m_walkingDirection == left) {
				if (getWorld()->notEmpty(up, getX(), getY())) {
					setDirection(right);
					m_walkingDirection = up;
				}
				else {
					setDirection(right);
					m_walkingDirection = down;
				}

			}
			else {
				if (getWorld()->notEmpty(right, getX(), getY())) {
					setDirection(right);
					m_walkingDirection = right;
				}
				else {
					setDirection(left);
					m_walkingDirection = left;
				}

			}
		}
		moveAtAngle(m_walkingDirection, 2);
		m_ticks_to_move--;
		//std::cerr << "here" << std::endl;
		
		if (m_ticks_to_move == 0)
			m_state = true;
		m_isNew = true;
	}
	
}

int PlayerAvatar::getCoins() {
	return m_coins;
}
int PlayerAvatar::getStars() {
	return m_stars;
}

int PlayerAvatar::getRoll() {
	return m_ticks_to_move / 8;
}

Peach::Peach(int startX, int startY, StudentWorld* sw)
	: PlayerAvatar(IID_PEACH, 1, startX, startY, sw) {};
Yoshi::Yoshi(int startX, int startY, StudentWorld* sw)
	: PlayerAvatar(IID_YOSHI, 2, startX, startY, sw) {};

bool PlayerAvatar::validDirection(int d) {
	if (abs(d - m_walkingDirection) == 180) //the player cannot go backwards in any case
		return false;
	else {
		
		if (getWorld()->notEmpty(d, getX(), getY()))
			return true;
		else
			return false;
			
	}
}
int PlayerAvatar::getTick() { return m_ticks_to_move; };

void PlayerAvatar::changeTick(int t) {
	m_ticks_to_move = t;
};
void PlayerAvatar::setCoins(int c) {
	m_coins = c;
}
void PlayerAvatar::setStars(int c) {
	m_stars = c;
}
void PlayerAvatar::addCoins(int c) { m_coins += c; };
void PlayerAvatar::deductCoins(int c) {
	if (m_coins >= c)
		m_coins -= c;
	else
		m_coins = 0;
};
void PlayerAvatar::deductStars(int c) {
	if (m_stars >= c)
		m_stars -= c;
	else
		m_stars = 0;
};
void PlayerAvatar::giveStar() {
	if (m_coins >= 20) {
		m_coins -= 20;
		m_stars++;
	}
};
bool PlayerAvatar::getState() { return m_state; };
void PlayerAvatar::changeState(bool s) {
	m_state = s;
};
bool PlayerAvatar::getNew() { return m_isNew; };
void PlayerAvatar::changeNewTo(bool tf) {
	m_isNew = tf;
}
void PlayerAvatar::changeWalkDirection(int dir) {
	m_walkingDirection = dir;
};
int PlayerAvatar::getWalkDirection() {
	return m_walkingDirection;
};
bool PlayerAvatar::hasVortex() {
	return m_hasVortex;
};
void PlayerAvatar::changeVortex(bool b) {
	m_hasVortex = b;
};