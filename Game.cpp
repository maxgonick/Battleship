#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
  private:
    int m_Rows;
    int m_Cols;
    struct Ship {
        int id;
        int length;
        char symbol;
        std::string name;
    };
    //Storage of Ships added
    vector<Ship*> Ships;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_Rows = nRows;
    m_Cols = nCols;
}

int GameImpl::rows() const
{
    return m_Rows;
}

int GameImpl::cols() const
{
    return m_Cols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    //Dynamically Allocates new Ship and assigns member variables (already been validated by Game::addShip
    Ship* m_Ship = new Ship;
    m_Ship->symbol = symbol;
    m_Ship->length = length;
    m_Ship->name = name;
    m_Ship->id = Ships.size();
    //Adds ship to vector
    Ships.push_back(m_Ship);
    return true;
}

int GameImpl::nShips() const
{
    return Ships.size();
}

int GameImpl::shipLength(int shipId) const
{
    return Ships[shipId]->length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return Ships[shipId]->symbol;
}

string GameImpl::shipName(int shipId) const
{
    return Ships[shipId]->name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause = true)
{
    //Placing ships
    if(!p1->placeShips(b1)){
        return nullptr;
    }
    if(!p2->placeShips(b2)){
        return nullptr;
    }

    int shipsCounter1 = 0;
    int shipsCounter2 = 0;
    b1.display(false);
    b2.display(false);
while(shipsCounter1 != nShips() && shipsCounter2 != nShips()) {
    //Player 1 turn
    if (p1->isHuman()) {
        b2.display(true);
    } else {
        b2.display(false);
    }
    bool wasHit = false;
    bool wasDestroyed = false;
    int shipId = false;
    b2.attack(p1->recommendAttack(), wasHit, wasDestroyed, shipId);
    p1->recordAttackResult(p1->recommendAttack(), true, wasHit, wasDestroyed, shipId);
    if (wasDestroyed) {
        shipsCounter1++;
    }
    if (p1->isHuman()) {
        b2.display(true);
    } else {
        b2.display(false);
    }
    bool enterPressed = false;
    cout << "Press enter to continue: ";
    if(shouldPause) {
        waitForEnter();
    }
    cout << endl;
    //Player 2 turn
    if (p2->isHuman()) {
        b1.display(true);
    } else {
        b1.display(false);
    }
    bool wasHit2 = false;
    bool wasDestroyed2 = false;
    int shipId2 = false;
    b1.attack(p2->recommendAttack(), wasHit2, wasDestroyed2, shipId2);
    if (p2->isHuman()) {
        b1.display(true);
    } else {
        b1.display(false);
    }
    cout << "Press enter to continue: ";
    if(shouldPause) {
        waitForEnter();
    }
    if (wasDestroyed2) {
        shipsCounter2++;
    }
}
if(shipsCounter1 == nShips()){
    return p1;
}
else if(shipsCounter2 == nShips()){
    return p2;
}
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

