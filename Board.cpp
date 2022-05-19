#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <algorithm>

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
            boardMatrix[i][j] = '.';
        }
    }
}

void BoardImpl::clear()
{
    for(int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            boardMatrix[i][j] = '.';
        }
    }
}

void BoardImpl::block()
{
    int counter = 0;
    while(counter != ((m_Rows*m_Cols)/2)){
        int x = randInt(m_Rows);
        int y = randInt(m_Cols);
        if(boardMatrix[x][y] == '.'){
            boardMatrix[x][y] = '#';
            counter++;
        }
    }
}

void BoardImpl::unblock()
{
    for(int i = 0; i < m_Rows; ++i){
        for (int j = 0; j < m_Cols; ++j) {
            if(boardMatrix[i][j] == '#'){
                boardMatrix[i][j] = '.';
            }
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    // Check if shipId is valid
    if(shipId > m_game.nShips()){
        return false;
    }
    // Check if ship fits within boundaries
    if(dir == HORIZONTAL){
        //Check if starting point is outside
        if(topOrLeft.c > m_Cols || topOrLeft.r > m_Rows){
            return false;
        }
        if((m_game.shipLength(shipId) + topOrLeft.c - 1) >= m_Cols){
            return false;
        }
    }
    if(dir == VERTICAL){
        //Check if starting point is outside
        if(topOrLeft.c > m_Cols || topOrLeft.r > m_Rows){
            return false;
        }
        if((m_game.shipLength(shipId) + topOrLeft.r - 1) >= m_Rows){
            return false;
        }
    }
    // TODO Check if ship overlaps on another
    if(dir == HORIZONTAL){
        for (int i = 0; i < m_game.shipLength(shipId); ++i) {
            if(boardMatrix[topOrLeft.r][topOrLeft.c + i] != '.'){
                return false;
            }
        }
    }
    if(dir == VERTICAL){
        for (int i = 0; i < m_game.shipLength(shipId); ++i) {
            if(boardMatrix[topOrLeft.r + i][topOrLeft.c] != '.'){
                return false;
            }
        }
    }

    // Check if ship overlaps on X spot in boardMatrix
    if(dir == HORIZONTAL){
        for (int i = topOrLeft.c; i < m_game.shipLength(shipId); ++i) {
            if(boardMatrix[topOrLeft.r][i] == 'X'){
                return false;
            }
        }
    }
    if(dir == VERTICAL){
        for (int i = topOrLeft.r; i < m_game.shipLength(shipId); ++i) {
            if(boardMatrix[i][topOrLeft.c] == 'X'){
                return false;
            }
        }
    }
    // Check if ID has already been placed
    for(int i = 0; i < m_Rows; ++i) {
        for (int j = 0; j < m_Cols; ++j) {
            if(boardMatrix[i][j] == m_game.shipSymbol(shipId)){
                return false;
            }
        }
    }
    //Placing Ship
    if(dir == HORIZONTAL){
        for (int i = topOrLeft.c; i < (m_game.shipLength(shipId) + topOrLeft.c); ++i) {
            boardMatrix[topOrLeft.r][i] = m_game.shipSymbol(shipId);
        }
        return true;
    }
    else if(dir == VERTICAL){
        for (int i = topOrLeft.r; i < (m_game.shipLength(shipId) + topOrLeft.r); ++i) {
            boardMatrix[i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
        return true;
    }
    else return false;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    //Checks if shipId is valid
    if(shipId > m_game.nShips() - 1){
        return false;
    }
    //Check if board contains entire ship
    if(dir == HORIZONTAL){
        for (int i = 0; i < topOrLeft.c; ++i) {
            if(boardMatrix[topOrLeft.r][i] != m_game.shipSymbol(shipId)){
                return false;
            }
        }
    }
    if(dir == VERTICAL){
        for (int i = 0; i < topOrLeft.r; ++i) {
            if(boardMatrix[i][topOrLeft.c] != m_game.shipSymbol(shipId)){
                return false;
            }
        }
    }
    //Unplacing Ship
    if(dir == HORIZONTAL){
        for(int i = topOrLeft.c; i <= m_game.shipLength(shipId); i++){
            boardMatrix[topOrLeft.r][i] = '.';
        }
        return true;
    }
    if(dir == VERTICAL){
        for(int i = topOrLeft.r; i <= m_game.shipLength(shipId); i++){
            boardMatrix[i][topOrLeft.c] = '.';
        }
        return true;
    }
    return false;
}

void BoardImpl::display(bool shotsOnly) const
{
    //shotsOnly is true
    if(shotsOnly){
    //First Line
        cout << "  ";
        for (int i = 0; i < m_Cols; ++i) {
            cout << i;
        }
        cout << endl;
        //Display remaining lines
        for (int i = 0; i < m_Rows; ++i) {
            cout << i << " ";
            for (int j = 0; j < m_Cols; ++j) {
                if(boardMatrix[i][j] == 'o' || boardMatrix[i][j] == 'X'){
                cout << boardMatrix[i][j];
                    }
                else{
                    cout << '.';
                }
            }
            cout << endl;
        }
    }
    //shotsOnly is false
    if(!shotsOnly){
        //First Line
        cout << "  ";
        for (int i = 0; i < m_Cols; ++i) {
            cout << i;
        }
        cout << endl;
        //Display remaining lines
        for (int i = 0; i < m_Rows; ++i) {
            cout << i << " ";
            for (int j = 0; j < m_Cols; ++j) {
                cout << boardMatrix[i][j];
            }
            cout << endl;
        }
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    //checking if attack is in bounds
    if(p.r <= m_Rows-1 || p.r >= 0 || p.c <= m_Cols-1 || p.c >= 0){
        //
        for (int i = 0; i < m_game.nShips(); ++i) {
            //Case if attack hits ship
            if(boardMatrix[p.r][p.c] == m_game.shipSymbol(i)){
                boardMatrix[p.r][p.c] = 'X';
                shipId = i;
                shotHit = true;
            }
        }
        if(boardMatrix[p.r][p.c] != 'X'){
        boardMatrix[p.r][p.c] = 'o';
        shotHit = false;
            }
        //Checking if entire ship is destroyed
        if(shotHit) {
            for (int i = 0; i < m_Rows; ++i) {
                for (int j = 0; j < m_Cols; ++j) {
                    if (boardMatrix[i][j] == m_game.shipSymbol(shipId)) {
                        shipDestroyed = false;
                        return true;
                    }
                }

            }
            shipDestroyed = true;
        }
        return true;
    }
    else return false;
}

bool BoardImpl::allShipsDestroyed() const
{
    for (int i = 0; i < m_Rows; ++i) {
        for (int j = 0; j < m_Cols; ++j) {
            if(boardMatrix[i][j] != '.' || boardMatrix[i][j] != 'X' || boardMatrix[i][j] != 'o'){
                return false;
            }
        }
    }
    return true;

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

