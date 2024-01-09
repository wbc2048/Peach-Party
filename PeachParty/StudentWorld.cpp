#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), m_allActors(), m_peach(nullptr), m_yoshi(nullptr), m_moneyInBank(0), m_alreadyClean(false){}

StudentWorld::~StudentWorld() {
    cleanUp();
};
int StudentWorld::init()
{
    Board bd;
    string board_file;
    string fileName;
    if (getBoardNumber() < 10 && getBoardNumber() > 0) {
        board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
        fileName = "board0" + to_string(getBoardNumber()) + ".txt";
    }
    else {
        board_file = assetPath() + "board" + to_string(getBoardNumber()) + ".txt";
        fileName = "board" + to_string(getBoardNumber()) + ".txt";
    }

    Board::LoadResult result = bd.loadBoard(board_file);

    if (result == Board::load_fail_file_not_found) {
        //cerr << "Could not find " + fileName + " data file\n";
        return GWSTATUS_BOARD_ERROR;
    }
        
    else if (result == Board::load_fail_bad_format) {
        //cerr << "Your board was improperly formatted\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success) {
        //cerr << "Successfully loaded board\n";

        for (int i = 0; i < BOARD_WIDTH; i++) {
            for (int j = 0; j < BOARD_HEIGHT; j++) {
                int x = SPRITE_WIDTH * i;
                int y = SPRITE_HEIGHT * j;
                Board::GridEntry ge = bd.getContentsOf(i, j);
                switch (ge) {
                case Board::empty:
                    //cerr << "Location " + to_string(i) + ", " + to_string(j) + " is empty\n";                    
                    break;
                case Board::player:
                    //cerr << "Location " + to_string(i) + ", " + to_string(j) + " has Peach & Yoshi and a blue coin square\n";
                    //m_allActors.push_back(new PlayerAvatar(x, y, this));                   
                    m_allActors.push_back(new BlueCoinSquare(x, y, this)); 
                    m_peach = new Peach(x, y, this);
                    m_yoshi = new Yoshi(x, y, this);
                    break;
                case Board::blue_coin_square:
                    //cerr << "Location " + to_string(i) + ", " + to_string(j) + " has a blue coin square\n";
                    m_allActors.push_back(new BlueCoinSquare(x, y, this));             
                    break;
                case Board::red_coin_square:
                    //cerr << "Location " + to_string(i) + ", " + to_string(j) + " has a red coin square\n";
                    m_allActors.push_back(new RedCoinSquare(x, y, this));
                    break;
                case Board::star_square:
                    //cerr << "Location " + to_string(i) + ", " + to_string(j) + " has a star square\n";
                    m_allActors.push_back(new StarSquare(x, y, this));
                    break;
                case Board::right_dir_square:
                    m_allActors.push_back(new DirectionalSquare(x, y, this, 0));
                    break;
                case Board::left_dir_square:
                    m_allActors.push_back(new DirectionalSquare(x, y, this, 180));
                    break;
                case Board::up_dir_square:
                    m_allActors.push_back(new DirectionalSquare(x, y, this, 90));
                    break;
                case Board::down_dir_square:
                    m_allActors.push_back(new DirectionalSquare(x, y, this, 270));
                    break;
                case Board::bank_square:
                    m_allActors.push_back(new BankSquare(x, y, this));
                    break;
                case Board::event_square:
                    m_allActors.push_back(new EventSquare(x, y, this));
                    break;
                case Board::bowser:
                    m_allActors.push_back(new Bowser(x, y, this));
                    m_allActors.push_back(new BlueCoinSquare(x, y, this));
                    //m_allActors.push_back(new DroppingSquare(x, y, this));
                    break;
                case Board::boo:
                    m_allActors.push_back(new Boo(x, y, this));
                    m_allActors.push_back(new BlueCoinSquare(x, y, this));
                    break;
                default:
                    break;
                }
            }
        }

        startCountdownTimer(99);
    }
    return GWSTATUS_CONTINUE_GAME;
    
}



int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    std::list<Actor*>::iterator it;
    for (it = m_allActors.begin(); it != m_allActors.end(); it++) {
        if ((*it)->isActive())
            (*it)->doSomething();
    }

    if (m_peach->isActive())
        m_peach->doSomething();
    if (m_yoshi->isActive())
        m_yoshi->doSomething();
    /*
    for (it = m_allActors.begin(); it != m_allActors.end(); it++) {
        if ((*it)->isActive() == false) {
            //delete (*it);
           it = m_allActors.erase(it);
        }
            
    }
    */
    //delete inactive objects
    it = m_allActors.begin();
    while (it != m_allActors.end()) {
        if ((*it)->isActive() == false) {
            delete* it;
            it = m_allActors.erase(it); // note that erase() returns the next iterator
        }
        else {
            ++it;
        }
    }

        
    if (!(m_peach->isActive()))
        delete m_peach;
    if (!(m_yoshi->isActive()))
        delete m_yoshi;
    // Update the Game Status Line
    //update display text 
    // update the coins/stars stats text at screen top
    
    ostringstream oss;
    oss << "P1 Roll:" << " " << m_peach->getRoll() << " " << "Stars:" << " " << m_peach->getStars() << " " << "$$:" << " " << m_peach->getCoins() << " " << displayVortex(m_peach) << " | " << "Time:" << " " << timeRemaining() << " | " << "Bank: " << m_moneyInBank << " | " << "P2 Roll:" << " " << m_yoshi->getRoll() << " " << "Stars:" << " " << m_yoshi->getStars() << " " << "$$:" << " " << m_yoshi->getCoins() << " " << displayVortex(m_yoshi);
    string s = oss.str();
    setGameStatText(s);

    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        if (winner() == 1) {
            setFinalScore(m_peach->getStars(), m_peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (winner() == 2) {
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (!m_alreadyClean) {
        std::list<Actor*>::iterator it;
        for (it = m_allActors.begin(); it != m_allActors.end(); it++) {
            delete (*it);
        }
        m_allActors.clear();
        delete m_peach;
        delete m_yoshi;
        m_alreadyClean = true;
    }
    
}

bool StudentWorld::notEmpty(int angle, int x, int y) {
    if (x < 0 || x > 255 || y < 0 || y > 255)
        return false;
    std::list<Actor*>::iterator it;
    // i am checking if in the player's moving direction, is there an actor in the 0-16 pixel range, if there is, that meant we can move onto it
    if (angle == 0) {
        for (it = m_allActors.begin(); it != m_allActors.end(); it++) {
            if ((*it)->getX() > x && (*it)->getX() <= x + SPRITE_WIDTH && (*it)->getY() == y) {            
                return true;
            }
        }

    }
    else if (angle == 180) {

        for (it = m_allActors.begin(); it != m_allActors.end(); it++) {
            if ((*it)->getX() < x && (*it)->getX() >= x - SPRITE_WIDTH && (*it)->getY() == y)
                return true;
        }

    }
    else if (angle == 90) {

        for (it = m_allActors.begin(); it != m_allActors.end(); it++) {
            if ((*it)->getX() == x && (*it)->getY() <= y + SPRITE_HEIGHT && (*it)->getY() > y)
                return true;
        }

    }
    else if (angle == 270) {

        for (it = m_allActors.begin(); it != m_allActors.end(); it++) {
            if ((*it)->getX() == x && (*it)->getY() >= y - SPRITE_WIDTH && (*it)->getY() < y)
                return true;
        }
    }
    return false;

}

bool StudentWorld::peachHasLandedOn(int x, int y) {
    if (m_peach->getX() == x && m_peach->getY() == y)
        return true;
    else
        return false;
}

bool StudentWorld::yoshiHasLandedOn(int x, int y) {
    if (m_yoshi->getX() == x && m_yoshi->getY() == y)
        return true;
    else
        return false;
}

int StudentWorld::getBank() { return m_moneyInBank; };

bool StudentWorld::isAtDS(int x, int y) {
    std::list<Actor*>::iterator it;
    for (it = m_allActors.begin(); it != m_allActors.end(); it++) {
        if ((*it)->isDS() && (*it)->getX() == x && (*it)->getY() == y) {
            return true;
        }
    }
    return false;
}
void StudentWorld::dropTheSquare(Bowser* b) {
    std::list<Actor*>::iterator it;
    for (it = m_allActors.begin(); it != m_allActors.end(); it++) {

        if ((*it)->canDelete() && (*it)->getX() == b->getX() && (*it)->getY() == b->getY()) {
            (*it)->changeToNotActive();
            delete* it; //be sure to delete and erase
            m_allActors.erase(it);
            m_allActors.push_back(new DroppingSquare(b->getX(), b->getY(), this));
            //std::cerr << "delete" << std::endl;
            break;
        }
    }
    return;
}
void StudentWorld::createVortex(PlayerAvatar* p) {
    //creating a vortex infront of the walking direction of the player
    if (p->getWalkDirection() == 0) {
        m_allActors.push_back(new Vortex(p->getX() + SPRITE_WIDTH, p->getY(), this, 0));
        //std::cerr << "right vortex created" << std::endl;
    }
    else if (p->getWalkDirection() == 180) {
        m_allActors.push_back(new Vortex(p->getX() - SPRITE_WIDTH, p->getY(), this, 180));
    }
    else if (p->getWalkDirection() == 90) {
        m_allActors.push_back(new Vortex(p->getX(), p->getY() + SPRITE_HEIGHT, this, 90));
    }
    else if (p->getWalkDirection() == 270) {
        m_allActors.push_back(new Vortex(p->getX(), p->getY() - SPRITE_HEIGHT, this, 270));
    }
    return;
}
void StudentWorld::addBank(int c) {
    //std::cerr << "added to bank" << std::endl;
    m_moneyInBank += c;
}
void StudentWorld::clearBank() {
    m_moneyInBank = 0;
}

void StudentWorld::teleport(Actor* a) {
    //teleport an actor to an random actor's spot
    int i = m_allActors.size();
    int r = rand() % i;
    std::list<Actor*>::iterator it = m_allActors.begin();
    for (int j = 0; j < r; j++) {
        ++it;
    }

    a->moveTo((*it)->getX(), (*it)->getY());

};
void StudentWorld::teleportBaddie(Actor* a) {
    if (a->impactable()) { //make sure we passed in baddie
        teleport(a);
        a->setCounter();
        a->changeState(true);
        a->changeWalkDirection(0);
    }
    else
        return;
};

void StudentWorld::swap() {
   //swapping two players
    int x, y;
    x = m_peach->getX();
    y = m_peach->getY();
    m_peach->moveTo(m_yoshi->getX(), m_yoshi->getY());
    m_yoshi->moveTo(x, y);

    int w = m_peach->getWalkDirection();
    m_peach->changeWalkDirection(m_yoshi->getWalkDirection());
    m_yoshi->changeWalkDirection(w);
    int d = m_peach->getDirection();
    m_peach->setDirection(m_yoshi->getDirection());
    m_yoshi->setDirection(d);
    int t = m_peach->getTick();
    m_peach->changeTick(m_yoshi->getTick());
    m_yoshi->changeTick(t);

    bool b = m_peach->getState();
    m_peach->changeState(m_yoshi->getState());
    m_yoshi->changeState(b);

    m_peach->changeNewTo(false);
    m_yoshi->changeNewTo(false);

};
std::string StudentWorld::displayVortex(PlayerAvatar* a) {
    if (a->hasVortex())
        return "VOR";
    else
        return "";
}
int StudentWorld::winner() {
    if (m_peach->getStars() > m_yoshi->getStars())
        return 1;
    else if (m_peach->getStars() < m_yoshi->getStars())
        return 2;
    else {
        if (m_peach->getCoins() > m_yoshi->getCoins())
            return 1;
        else if (m_peach->getCoins() < m_yoshi->getCoins())
            return 2;
        else
            return rand() % 2 + 1;
    }
}
Actor* StudentWorld::overlap(Actor* a) {
    // if vortex comes in a 16 pixel radius then they are overlapping
    std::list<Actor*>::iterator it;
    int w;
    int h;
    for (it = m_allActors.begin(); it != m_allActors.end(); it++) {
        w = ((*it)->getX()) - (a->getX());
        h = ((*it)->getY()) - (a->getY());

        if ((*it)->impactable() && abs(w) < SPRITE_WIDTH && abs(h) < SPRITE_HEIGHT) {
            return (*it);
        }

    }
    return nullptr;
}
Peach* StudentWorld::getPeach() { return m_peach; };
Yoshi* StudentWorld::getYoshi() { return m_yoshi; };