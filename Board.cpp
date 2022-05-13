#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
      int m_Cols;
      int m_Rows;
    const Game& m_game;
    char boardMatrix[MAXROWS][MAXCOLS];
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    m_Cols = m_game.cols();
    m_Rows = m_game.rows();
    //Construct Empty Matrix
    for (int i = 0; i < m_Cols; ++i) {
        for (int j = 0; j < m_Rows; ++j) {
            boardMatrix[i][j] == '.';
        }
    }
}

void BoardImpl::clear()
{
    for(int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            boardMatrix[i][j] == '.';
        }
    }
}

void BoardImpl::block()
{
    for (int i = 0; i < ((m_Cols)*(m_Rows))/2; ++i) {
        int x = randInt(MAXCOLS);
        int y = randInt(MAXROWS);
        if(boardMatrix[x][y] == '.'){
            boardMatrix[x][y] = 'X';
        }
        else{
            i--;
        }
    }
}

void BoardImpl::unblock()
{
    // TODO:  Replace this with code to unblock all blocked cells
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return false; // This compiles, but may not be correct
}

void BoardImpl::display(bool shotsOnly) const
{
    // This compiles, but may not be correct
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return false; // This compiles, but may not be correct
}

bool BoardImpl::allShipsDestroyed() const
{
    return false; // This compiles, but may not be correct
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
