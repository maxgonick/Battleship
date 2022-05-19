#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <queue>

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
            return true;
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
            Point p(0,0);
            return p;
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
        crossRecord.resize(0);
        counter = 2;
//        Sets record matrix to all be false;
        for (int i = 0; i < game().rows(); ++i) {
            for (int j = 0; j < game().cols(); ++j) {
                masterRecord[i][j] = false;
            }
        }
        //Constructs CheckBoard Pattern
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
    bool hasBeenAttacked(Point p){
        if(masterRecord[p.r][p.c]){
            return true;
        }
        else{
            return false;
        }
    }

    Point randomCheckerPoint(){
        int index = randInt(randomRecord.size());
        Point p = randomRecord[index];
        while(masterRecord[p.r][p.c]){
//            randomRecord.erase(randomRecord.begin() + index - 1);
            int index2 = randInt(randomRecord.size());
            p = randomRecord[index2];
        }
        //Update masterRecord
        masterRecord[p.r][p.c] = true;
        return p;
    }

    int randomIndex(vector<Point> vectorTarget){
        int index = randInt(vectorTarget.size());
        return index;
    };

    Point pointFromVector(vector<Point>& vectorTarget){
        int index = randInt(vectorTarget.size());
        Point p = vectorTarget[index];
        vectorTarget.erase(vectorTarget.begin() + index);
        return p;
    }

    void updateMaster(Point p){
        masterRecord[p.r][p.c] = true;
        return;
    }
    Point recommendAttack() override{
        //Randomly attack checkerboard
        if(state == 1){

            if(randomRecord.empty()){
                Point p(randInt(game().rows()), randInt(game().cols()));
                while(masterRecord[p.r][p.c] == true){
                    Point temp(randInt(game().rows()),randInt(game().cols()));
                    p = temp;
                }
                masterRecord[p.r][p.c] = true;
                return p;
            }

            int index = randInt(randomRecord.size());
            Point p = randomRecord[index];
            while(masterRecord[p.r][p.c]){
                int index2 = randInt(randomRecord.size());
                p = randomRecord[index2];
            }
            //Update masterRecord
            masterRecord[p.r][p.c] = true;
            return p;
        }
        //First Time entering Cross Attack
        if(state == 2 && crossRecord.size() == 0){
            //Check Right Adjacent
            if((starting.c + 1) < game().cols()){
                Point pRight(starting.r,starting.c+1);
                //Checking if that spot has been attacked
                if(!hasBeenAttacked(pRight)){
                    crossRecord.push_back(pRight);
                }
            }
            //Check left Adjacent
            if((starting.c - 1) >= 0){
                Point pLeft(starting.r,starting.c-1);
                //Checking if spot has been attacked
                if(!hasBeenAttacked(pLeft)){
                    crossRecord.push_back(pLeft);
                }
            }
            //Check up Adjacent
            if((starting.r - 1) >= 0){
                Point pUp(starting.r - 1, starting.c);
                //Check if spot has been attacked
                if(!hasBeenAttacked(pUp)){
                    crossRecord.push_back(pUp);
                }
            }
            //Check down Adjacent
            if((starting.r + 1) < game().rows()){
                Point pDown(starting.r+1, starting.c);
                //Check if spot has been attacked
                if(!hasBeenAttacked(pDown)){
                    crossRecord.push_back(pDown);
                }
            }
            if(!crossRecord.empty()){
             int index = randomIndex(crossRecord);
             Point p = crossRecord[index];
             masterRecord[p.r][p.c] = true;
             crossRecord.erase(crossRecord.begin() + index);
             return p;
            }
            //All adjacent Points are invalid
            else {
                state = 1;
                return randomCheckerPoint();
            }
        }
        //Attack adjacent
        if(state == 2 && !crossRecord.empty()){

            Point p = pointFromVector(crossRecord);
            updateMaster(p);
            return p;
        }
        //State 3
        if(state == 3 && !Directions.empty()){
            if(Directions.front() == "LEFT"){
                if(starting.c - counter >= 0){
                    Point p(starting.r, starting.c - counter);
                    updateMaster(p);

                    return p;
                }
            }
            if(Directions.front() == "RIGHT"){
                if(starting.c + counter < game().cols()){
                    Point p(starting.r, starting.c + counter);
                    updateMaster(p);

                    return p;
                }
            }
            if(Directions.front() == "UP"){
                if(starting.r - counter >= 0){
                    Point p(starting.r - counter, starting.c);
                    updateMaster(p);
                    return p;
                }
            }
            if(Directions.front() == "DOWN"){
                if(starting.r + counter < game().rows()){
                    Point p(starting.r + counter, starting.c);
                    updateMaster(p);
                    return p;
                }
            }
            Directions.pop();
        }

return randomCheckerPoint();
    }

    void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) override{
        //Checks that shot should be recorded
        if(validShot){
            //Stay in Neutral
            if(state == 1 && !shotHit){
                return;
            }
            //Ship was hit randomly in neutral
            if(state == 1 && shotHit){
                state = 2;
                starting = p;
                return;
            }
            if(state == 2 && !shotHit){
                return;
            }
            //Ship was hit in cross
            if(state == 2 && shotHit){
                state = 3;
                //If shot hit was right to starting
                if(starting.c + 1 < game().cols()){
                    if(starting.c + 1 == p.c){
                        Directions.push("RIGHT");
                        Directions.push("LEFT");
                        Directions.push("UP");
                        Directions.push("DOWN");
                        }
                    }
                //If shot hit was left to starting
                if(starting.c - 1 >= 0 ){
                    if(starting.c - 1 == p.c){
                        Directions.push("LEFT");
                        Directions.push("RIGHT");
                        Directions.push("UP");
                        Directions.push("DOWN");

                    }
                }
                //If shot hit was above starting
                if(starting.r - 1 >= 0){
                    if(starting.r - 1 == p.r){
                        Directions.push("UP");
                        Directions.push("DOWN");
                        Directions.push("LEFT");
                        Directions.push("RIGHT");
                    }
                }
                //If shot hit was below starting
                if(starting.r + 1 < game().rows()){
                    if(starting.r + 1 == p.r){
                        Directions.push("DOWN");
                        Directions.push("UP");
                        Directions.push("LEFT");
                        Directions.push("RIGHT");
                    }
                }
                return;
            }
            if(state == 3 && !shotHit && !shipDestroyed){
                if(!Directions.empty()){
                    Directions.pop();
                    counter = 1;
                }
                else{
                    state = 1;
                }
                return;
            }
            if(state == 3 && shotHit && !shipDestroyed){
                counter++;
                return;
            }
            if(state == 3 && shipDestroyed){
                state = 1;
                counter = 2;
                crossRecord.clear();
                while(!Directions.empty()){
                    Directions.pop();
                }
                return;
            }
        }
        else return;
    }
    void recordAttackByOpponent(Point p) override{
        ;
    }
private:
    int state;
    int counter;
    bool masterRecord[MAXROWS][MAXCOLS];
    vector<Point> randomRecord;
    vector<Point> crossRecord;
    Point starting;
    queue<string> Directions;


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
