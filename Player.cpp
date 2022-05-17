#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    //Attacks one by one starting from the end going down each row
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
    public:
        HumanPlayer(string nm, const Game& g);
        //string name() const is already implemented through inheritance
        //const Game& game() const is already implemented through inheritance
        void input(char dir){
            if(dir == 'v'){
                cout << "Enter row and column of topmost cell (e.g. 3 5): ";
                getLineWithTwoIntegers(rowInput, colInput);
                cout << endl;
            }
        }
        bool isHuman() const override{
            return true;
        }
        bool placeShips(Board& b) override{
            //Iterating placement of each ship in game
            for (int i = 0; i < game().nShips(); ++i) {
                //Deals with h or v prompt
                b.display(false);
                cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i ) << "): ";
                char horOrVer;
                cin >> horOrVer;
                cout << endl;
                while(horOrVer != 'h' && horOrVer != 'v'){
                    cout << "Direction must be h or v." << endl;
                    cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i ) << "): ";
                    cin >> horOrVer;
                    cout << endl;
                }
                if(horOrVer == 'h'){
                    input('h');
                    while(!isdigit(rowInput) || !isdigit(colInput) || rowInput < 0 || colInput < 0 || rowInput >= game().rows() || colInput >= game().rows()){
                        if(!isdigit(rowInput) || !isdigit(colInput)){
                            cout << endl << "You must enter two integers." << endl;
                            input('h');
                        }
                        else{
                            cout << "The ship cannot be placed there." << endl;
                            input('h');
                        }
                    }
                }
                else{
                    input('v');
                    while(!isdigit(rowInput) || !isdigit(colInput) || rowInput < 0 || colInput < 0 || rowInput >= game().rows() || colInput >= game().cols()){
                        if(!isdigit(rowInput) || !isdigit(colInput)){
                            cout << endl << "You must enter two integers." << endl;
                            input('v');
                        }
                        else{
                            cout << "The ship cannot be placed there." << endl;
                            input('v');
                        }
                    }
                }
            }
            return true;
        }
        Point recommendAttack() override{
            cout << "Enter the row and column to attack (e.g., 3 5): ";
            int row;
            int col;
            getLineWithTwoIntegers(row, col);
            cout << endl;
            while(!isdigit(row) || !isdigit(col) || row >= game().rows() || col >= game().cols() || row < 0 || col < 0){
                if(!isdigit(row) || !isdigit(col)){
                    cout << "You must enter two integer" << endl;
                }
                else if(row >= game().rows() || col >= game().cols() || row < 0 || col < 0){
                    cout << name() << " the Human wasted a shot at (" << row << "," << col << ")." << endl;
                    Point p(row, col);
                    return p;
                }
            }
            Point p1(row, col);
            return p1;
        }

    void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) override {
            ;
        }

    void recordAttackByOpponent(Point p) override{
            ;
        }
    private:
        int rowInput;
        int colInput;



};
    HumanPlayer::HumanPlayer(string nm, const Game &g) : Player(nm, g){
        ;
    }
//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class MediocrePlayer : public Player{
    public:
        MediocrePlayer(string nm, const Game& g) : Player(nm, g) {
        ;
        }

        bool placeShips(Board& b) override{
            b.block();
            do{
                placeShipsHelper(b, 0);
            }
            while (!placeShipsHelper(b, 0));
        }

        bool placeShipsHelper(Board& b, int shipId){
            if(shipId + 1 == game().nShips()){
                return true;
            }
            //Vertical Place
            bool wasPlaced = false;
            for (int i = 0; i < game().cols(); ++i){
                for (int j = 0; j < game().rows(); ++j){
                    Point p(i,j);
                    if(b.placeShip(p, shipId, VERTICAL)){
                        wasPlaced = true;
                        placeShipsHelper(b, shipId+1);
                    }
                }
            }
            if(!wasPlaced){
                for (int i = 0; i < game().rows(); ++i) {
                    for (int j = 0; j < game().cols(); ++j) {
                        Point p(i, j);
                        if(b.placeShip(p, shipId, HORIZONTAL)){
                            wasPlaced = true;
                            placeShipsHelper(b, shipId+1);
                        }
                    }
                }
            }
        }

        void recordAttackByOpponent(Point p) override{
            ;
        }
        Point recommendAttack() override{
            ;
        }
        void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) override{
            ;
        }


    private:
};

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
typedef AwfulPlayer GoodPlayer;

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
