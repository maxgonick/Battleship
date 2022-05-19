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
        bool input(char dir, Board& b, int id){
            if(dir == 'v'){
                cout << "Enter row and column of topmost cell (e.g. 3 5): ";
                if(getLineWithTwoIntegers(rowInput, colInput)){
                    if(rowInput >= game().rows() || colInput >= game().cols()){
                        cout << "The ship could not be placed there." << endl;
                        return false;
                    }
                    Point p1(rowInput, colInput);
                    if(!b.placeShip(p1, id, VERTICAL)){
                        cout << "The ship could not be placed there." << endl;
                        return false;
                    }
                    return true;
                }
                else{
                    cout << "You must enter two integers." << endl;
                    return false;
                }
            }
            else if(dir == 'h'){
                cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
                if(getLineWithTwoIntegers(rowInput,colInput)){
                    if(rowInput >= game().rows() || colInput >= game().cols()){
                        cout << "The ship could not be placed there.";
                        cout << endl;
                        return false;
                    }
                    Point p(rowInput, colInput);
                    if(!b.placeShip(p, id, HORIZONTAL)){
                        cout << "The ship could not be placed there." << endl;
                        return false;
                    }
                    return true;
                }
                else{
                    cout << "You must enter two integers." << endl;
                    return false;
                }
            }
        }
        bool isHuman() const override{
            return true;
        }
        bool placeShips(Board& b) override {
            //Iterating placement of each ship in game
            for (int i = 0; i < game().nShips(); ++i) {
                //Deals with h or v prompt
                b.display(false);
                cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i)
                     << "): ";
                string throwAway;
                getline(cin,throwAway);
                char horOrVer =  throwAway[0];
                while (horOrVer != 'h' && horOrVer != 'v') {
                    cout << "Direction must be h or v." << endl;
                    cout << "Enter h or v for direction of " << game().shipName(i) << " (length "
                         << game().shipLength(i) << "): ";
                    string throwAway1;
                    getline(cin,throwAway1);
                    horOrVer = throwAway1[0];
                }
                if (horOrVer == 'h') {
                    while (!input('h', b, i)) {
                    }

                } else if (horOrVer == 'v') {
                    while (!input('v', b, i)) {
                    }
                }
            }
            b.display(false);
            return true;
        }
        Point recommendAttack() override{
            cout << "Enter the row and column to attack (e.g., 3 5): ";
            int row;
            int col;
            while(!getLineWithTwoIntegers(row, col)){
                cout << "You must enter two integers." << endl;
                cout << "Enter the row and column to attack (e.g., 3 5): ";
            }
            Point p(row, col);
            return p;
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

class MediocrePlayer : public Player{
    public:
        MediocrePlayer(string nm, const Game& g) : Player(nm, g) {
        state = 1;
            for (int i = 0; i < game().rows(); ++i) {
                for (int j = 0; j < game().cols(); ++j) {
                    Point p(i,j);
                    record.push_back(p);
                    recordRandom[i][j] = false;
                }
            }

            recordCross.resize(0);
        }

        bool placeShips(Board& b) override{
                    for (int i = 0; i < 50; ++i) {
            b.block();
            if(placeShipsHelper(b, 0)){
                b.unblock();
                return true;
            }
            b.clear();
        }
        return false;
        }

        bool placeShipsHelper(Board& b, int shipId){
            //All ships are placed
            if(shipId == game().nShips()){
                return true;
            }

            for (int i = 0; i < game().rows(); ++i) {
                for (int j = 0; j < game().cols(); ++j) {
                    Point p(i, j);
                    if(b.placeShip(p, shipId, VERTICAL)){
                        bool allWerePlacedSuccessfully = placeShipsHelper(b, shipId+1);
                        //Placed all ships
                        if(allWerePlacedSuccessfully == true){
                            return true;
                        }
                        b.unplaceShip(p, shipId, VERTICAL);
                    }
                    if(b.placeShip(p, shipId, HORIZONTAL)){
                        bool allWerePlacedSuccessfully2 = placeShipsHelper(b, shipId+1);
                        //Placed all ships
                        if(allWerePlacedSuccessfully2 == true){
                            return true;
                        }
                        b.unplaceShip(p,shipId, HORIZONTAL);
                    }
                }
            }
            return false;
        }

        void recordAttackByOpponent(Point p) override{
            ;
        }
        Point recommendAttack() override{
            if(state == 1){
               int index = randInt(record.size());
               Point p = record[index];
               while(recordRandom[p.r][p.c] == true){
                   index = randInt(record.size());
                   p = record[index];
               }
               record.erase(record.begin() + index);
               recordRandom[p.r][p.c] = true;
               return p;
            }
            //First Time Entering State 2
            else if(state == 2 && recordCross.size() == 0){
                //Populates recordCross with possible positions
                for (int i = 1; i <= 4; ++i) {
                   if(starting.r+i < game().rows() && starting.r+i >= 0){
                       Point p1(starting.r+i,starting.c);
                       recordCross.push_back(p1);
                   }
                   else break;
                }
                for (int i = 1; i <= 4; ++i) {
                    if(starting.r-i < game().rows() && starting.r-i >= 0){
                        Point p1(starting.r-i,starting.c);
                        recordCross.push_back(p1);
                    }
                    else break;
                }
                for (int i = 1; i <= 4; ++i) {
                    if(starting.c+i < game().cols() && starting.c+i >= 0){
                        Point p1(starting.r, starting.c+i);
                        recordCross.push_back(p1);
                    }
                    else break;
                }
                for (int i = 1; i <= 4; ++i) {
                    if(starting.c-i < game().cols() && starting.c-i >= 0){
                        Point p1(starting.r, starting.c-i);
                        recordCross.push_back(p1);
                    }
                    else break;
                }
                for (int i = 0; i < game().rows(); ++i) {
                    for (int j = 0; j < game().cols(); ++j) {
                        if(recordRandom[i][j] == true){
                            Point tester(i,j);
                            for (int k = 0; k < recordCross.size(); ++k) {
                                Point tester2(recordCross[k].r, recordCross[k].c);
                                if(tester2.r == tester.r && tester2.c == tester.c){
                                    recordCross.erase(recordCross.begin()+k);
                                }
                            }
                        }
                    }
                }
                int index = randInt(recordCross.size());
                Point returnPoint = recordCross[index];
                recordCross.erase(recordCross.begin() + index);
                return returnPoint;
            }
            //Takes last point out and clears
            else if(state == 2 && recordCross.size() == 1){
                Point returnPoint = recordCross[0];
                recordCross.clear();
                return returnPoint;
            }
            //Entering cross pattern that is already initialized
            else if(state == 2 && recordCross.size() != 0){
                int index1 = randInt(recordCross.size());
                Point returnPoint = recordCross[index1];
                recordCross.erase(recordCross.begin() + index1);
                recordRandom[returnPoint.r][returnPoint.c] = true;
                return returnPoint;
            }
        }


        void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) override{
            if(validShot && state == 1){
                if(!shotHit){
                    return;
                }
                if(shotHit == true && shipDestroyed == false){
                    //Record starting point for cross reference
                    Point throwaway(p.r,p.c);
                    starting = throwaway;
                    state = 2;
                    return;
                }
                if(shotHit == true && shipDestroyed == true){
                    return;
                }
            }
            if(validShot && state == 2){
                if(!shipDestroyed){
                    return;
                }
                else if(shipDestroyed){
                    recordCross.clear();
                    state = 1;
                    return;
                }
            }
        }


    private:
        int state;
        Point starting;
        vector<Point> record;
        vector<Point> recordCross;
        bool recordRandom[MAXROWS][MAXCOLS];
};

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player
{
public:

    GoodPlayer(string nm, const Game& g) : Player(nm, g){
        state = 1;
        secondHit = false;
        direction = "UNKNOWN";
        changed = false;
        changedOrientation = false;
        counter = 0;
        crossRecord.resize(0);
        //Sets record matrix to all be false;
        for (int i = 0; i < game().rows(); ++i) {
            for (int j = 0; j < game().cols(); ++j) {
                masterRecord[i][j] = false;
            }
        }

        for (int i = 0; i < game().rows(); ++i) {
            for (int j = 0; j < game().cols(); ++j) {
                //even row
                if(i % 2 == 0){
                    //odd column
                    if(j % 2 == 0){
                        Point tempPush(i,j);
                        randomRecord.push_back(tempPush);
                    }
                }
                //odd row
                else if(i % 2 == 1){
                    //odd column
                    if(j % 2 == 1){
                        Point tempPush1(i,j);
                        randomRecord.push_back(tempPush1);
                    }
                }
            }
        }
    }

    bool placeShips(Board &b) override{
        for (int i = 0; i < 50; ++i) {
            b.block();
            if(placeShipsHelper(b, 0)){
                b.unblock();
                return true;
            }
            b.clear();
        }
        return false;
    }

    bool placeShipsHelper(Board& b, int shipId){
        //All ships are placed
        if(shipId == game().nShips()){
            return true;
        }

        for (int i = 0; i < game().rows(); ++i) {
            for (int j = 0; j < game().cols(); ++j) {
                Point p(i, j);
                if(b.placeShip(p, shipId, VERTICAL)){
                    bool allWerePlacedSuccessfully = placeShipsHelper(b, shipId+1);
                    //Placed all ships
                    if(allWerePlacedSuccessfully == true){
                        return true;
                    }
                    b.unplaceShip(p, shipId, VERTICAL);
                }
                if(b.placeShip(p, shipId, HORIZONTAL)){
                    bool allWerePlacedSuccessfully2 = placeShipsHelper(b, shipId+1);
                    //Placed all ships
                    if(allWerePlacedSuccessfully2 == true){
                        return true;
                    }
                    b.unplaceShip(p,shipId, HORIZONTAL);
                }
            }
        }
        return false;
    }

    Point recommendAttack() override{
        if(state == 1){
            //Picks a point from checkerboard points
            int index = randInt(randomRecord.size());
            Point p = randomRecord[index];
            //Removes it after it has been chosen
            randomRecord.erase(randomRecord.begin() + index);
            masterRecord[p.r][p.c] = true;
            return p;
        }
        //Attacks adjacent
        else if(state == 2 && !secondHit && crossRecord.size() == 0) {
            //Initialize CrossRecord
            for (int i = 1; i <= 4; ++i) {
                if(starting.r + i <= game().rows()){
                    if(!masterRecord[starting.r + i][starting.c]){
                        Point p(starting.r + i, starting.c);
                        crossRecord.push_back(p);
                    }
                }
            }
            for (int i = 1; i <= 4; ++i) {
                if(starting.r - i <= game().rows() && starting.r - i >= 0){
                    if(!masterRecord[starting.r - i][starting.c]){
                        Point p(starting.r - i, starting.c);
                        crossRecord.push_back(p);
                    }
                }
            }
            for (int i = 1; i <= 4; ++i) {
                if(starting.c + i <= game().cols()){
                    if(!masterRecord[starting.r][starting.c + i]){
                        Point p(starting.r, starting.c + i);
                        crossRecord.push_back(p);
                    }
                }
            }
            for (int i = 1; i <= 4; ++i) {
                if(starting.c - i <= game().cols() && starting.c - i >= 0){
                    if(!masterRecord[starting.r][starting.c - i]){
                        Point p(starting.r, starting.c - i);
                        crossRecord.push_back(p);
                    }
                }
            }
            int index = randInt(crossRecord.size());
            Point returnPoint = crossRecord[index];
            crossRecord.erase(crossRecord.begin() + index);
            return returnPoint;
        }
        else if (state == 2 && !secondHit && crossRecord.size() > 0){
            //Cross attack pattern and crossRecord is already initialized
            int index = randInt(crossRecord.size());
            Point returnPoint = crossRecord[index];
            crossRecord.erase(crossRecord.begin() + index);
            return returnPoint;
        }
        //Repeated Attacking
        else if(state == 2 && secondHit){
            if(direction == "LEFT"){
                if(starting.c - counter >= 0){
                    if(masterRecord[starting.r][starting.c - counter] == false){
                    Point temp(starting.r, starting.c - counter);
                    masterRecord[starting.r][starting.c - counter] = true;
                    return temp;
                        }
                    else{
                        if(starting.c - counter - 1 >= 0){
                            Point temp(starting.r, starting.c - counter - 1);
                            return temp;
                        }
                        else{
                            direction = "RIGHT";
                            Point temp(starting.r , starting.c + 1);
                            counter = 2;
                            return temp;
                        }
                    }
                }
            }
            if(direction == "RIGHT"){
                if(starting.c + counter < game().cols()){
                    if(masterRecord[starting.r][starting.c + counter] == false){
                    Point temp(starting.r, starting.c + counter);
                    masterRecord[starting.r][starting.c + counter] = true;
                    return temp;
                        }
                    else{
                        if(starting.c + counter + 1 > game().cols()){
                            Point temp(starting.r, starting.c + counter + 1);
                            return temp;
                        }
                        else{
                            direction = "LEFT";
                            Point temp(starting.r, starting.c -1);
                            counter = 2;
                            return temp;
                        }
                    }
                }
            }
            if(direction == "UP"){
                if(starting.r - counter >= 0){
                    if(masterRecord[starting.r - counter][starting.c] == false){
                    Point temp(starting.r - counter, starting.c);
                    masterRecord[starting.r - counter][starting.c];
                    return temp;
                        }
                    else{
                        if(starting.r - counter - 1 >= 0){
                            Point temp(starting.r-counter-1,starting.c);
                            return temp;
                        }
                        else{
                            direction = "DOWN";
                            Point temp(starting.r + 1,starting.c);
                            counter = 2;
                            return temp;
                        }
                    }
                }
            }
            if(direction == "DOWN"){
                if(starting.r + counter < game().rows()){
                    if(masterRecord[starting.r + counter][starting.c] == false){
                    Point temp(starting.r + counter, starting.c);
                    masterRecord[starting.r + counter][starting.c] = true;
                    return temp;
                        }
                    else{
                        if(starting.r + counter + 1 < game().rows()){
                            Point temp(starting.r+counter+1, starting.c);
                            return temp;
                        }
                        else{
                            direction = "UP";
                            Point temp(starting.r - 1, starting.c);
                            counter = 2;
                            return temp;
                        }
                    }
                }
            }
        }
    }

    void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) override{
        if(validShot && state == 1){
            if(!shotHit){
                return;
            }
            if(shotHit == true && shipDestroyed == false){
                //Record starting point for cross reference
                Point throwaway(p.r,p.c);
                starting = throwaway;
                state = 2;
                return;
            }
            if(shotHit == true && shipDestroyed == true){
                return;
            }
        }
        //Attacking cross pattern and shot was not hit
         if(validShot && state == 2 && crossRecord.size() > 0 && !shotHit){
             return;
         }
        //Have been attacking cross pattern, and hit a ship segment (must start attacking in that direction)
        if(validShot && state == 2 && crossRecord.size() > 0 && shotHit){
            if(direction == "UNKNOWN"){
                //Setting direction
            //Attack is to the left of starting
            if(p.r == starting.r && p.c < starting.c){
                direction = "LEFT";
                shotHit = true;
                counter = 1;
            }
            //Attack is to the left of starting
            if(p.r == starting.r && p.c > starting.c){
                direction = "RIGHT";
                shotHit = true;
                counter = 1;
            }
            //Attack is above starting
            if(p.c == starting.c && p.r < starting.r){
                direction = "UP";
                shotHit = true;
                counter = 1;
            }
            //Attack is below starting
            if(p.c == starting.c && p.r > starting.r){
                direction = "DOWN";
                shotHit = true;
                counter = 1;
            }
                }
            crossRecord.clear();
            secondHit = true;
            return;
        }
        //Attacking in repeated direction has missed so we must switch directions
        if(!shotHit && state == 2 && direction != "UNKNOWN" && !changed){
            if(direction == "LEFT"){
                direction = "RIGHT";
                counter = 1;
                changed = true;
            }
            else if(direction == "RIGHT"){
                direction = "LEFT";
                counter = 1;
                changed = true;
            }
            else if(direction == "UP"){
                direction = "DOWN";
                counter = 1;
                changed = true;
            }
            else if(direction == "DOWN"){
                direction = "UP";
                counter = 1;
                changed = true;
            }
            return;
        }
        //Must change orientation
        if(!shotHit && state == 2 && direction != "UNKNOWN" && changed && !changedOrientation){
            if(direction == "LEFT" || direction == "RIGHT"){
                direction = "UP";
                changed = false;
                changedOrientation = true;
            }
            else if(direction == "UP" || direction == "DOWN"){
                direction = "LEFT";
                changed = false;
                changedOrientation = true;
            }
        }
        //Attacking in one direction hit without sinking ship (we should keep going that way
        if(shotHit && state == 2 && direction != "UNKNOWN" && !shipDestroyed){
            counter++;
            return;
        }
        //Attacking in repeated direction sunk ship
        if(state == 2 && direction != "UNKNOWN" && shipDestroyed){
            state = 1;
            direction = "UNKNOWN";
            counter = 1;
            secondHit = false;
            changed = false;
        }
        //Edge case
        if(!shotHit && state == 2 && direction != "UNKNOWN" && changed){
            state = 1;
            return;
        }

        if(state == 2 && changed && direction != "UNKNOWN" && counter > 6){
            state = 1;
        }
    }

    void recordAttackByOpponent(Point p) override{
        ;
    }
private:
    int state;
    bool secondHit;
    string direction;
    int counter;
    bool changed;
    bool changedOrientation;
    bool masterRecord[MAXROWS][MAXCOLS];
    vector<Point> randomRecord;
    vector<Point> crossRecord;
    Point starting;
    Point repeatedAttack;

};

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
