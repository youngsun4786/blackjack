#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<string>
using namespace std;
using std::vector;


class Card {
enum Value {ACE = 1, TWO, THREE, FOUR,FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING} ;
enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES};
public:
    //fields 
    Value value;
    Suit suit;
    //default constructor
    Card(){}; 
    // parameterized constructor
    Card(int rank, int type) { 
        // type cast so that the parameter accepts int and casts to type Value and Suit respectively
        this->value = static_cast<Value>(rank);
        this->suit = static_cast<Suit>(type);
    }
    int getValue() const; //must stay constant because we are not modifying anything
    void setValue(int rank);
    void displayCard();
};
int Card::getValue() const{
    if (value > TEN) {
        int(value); //typecast
        value = 10;
        return value;
    } 
    return value;
}

void Card::setValue(int rank){
    value = static_cast<Value>(rank);
}
void Card::displayCard(){
     if (value > TEN) {
        switch(value) {
            case JACK: cout << "J"; break;
            case QUEEN: cout << "Q"; break;
            case KING: cout << "K"; break;
            default: break;
        }
    } else {
        cout << value;
    }
    switch(suit) {
        case CLUBS: cout << "C"; break;
        case DIAMONDS: cout << "D"; break;
        case HEARTS: cout << "H"; break;
        case SPADES: cout << "S"; break;
        default: break;
     }
}

// contain a list of cards implemented as an array or vector
class Hand {
public:
    // cards --> set of cards held by a player
    vector<Card> cards;
    //constructor
    Hand(){};
    void add(Card& c);
    void clear();
    void printDeck();
    int getTotal() const; //must stay constant because we are not modifying anything

};
// adds the received card into the player set
void Hand::add(Card& c) {
    cards.push_back(c);
}  
// remove all the cards in player's hand
void Hand::clear(){
   cards.clear();
}
// prints the cards
void Hand::printDeck() {
    for (int i = 0; i < cards.size(); i++) {
        cards[i].displayCard();
        cout << " ";
    }
}
// calculates the total points earned and returns the sum
int Hand::getTotal() const{
    int sum = 0;
    int ace_count = 0;
    bool ace_found = false;
    for (int i = 0; i < cards.size(); i++) {
        // ace found so change it to true
        if (cards[i].getValue() == 1) {
            ace_found = true;
            ace_count++;
        } else {
         // else add card to the sum
            sum += cards[i].getValue();
        }
    }
    // enters the block if ace is found
    if (ace_found) {
        for (int i = 0; i < ace_count; i++) {
            if (sum + 11 <= 21) {
                sum += 11;
            } else {
                sum += 1;
            }
        }
    }
    return sum;
}


class Deck: public Hand{
public:
    //inherits the cards vector in Hand class
    // so when Deck object is created it will have its own card vector object
    // constructor
    Deck() {};
    void populate();
    void shuffle();
    void deal(Hand& hand);
    void remove(Card& c);
};

void Deck::populate() { 
    // clear the deck first
    cards.clear();
    // populate a new deck 
    for (int suits = 0; suits < 4; suits++) {
        for (int ranks = 1; ranks < 14; ranks++) {
            Card c(ranks, suits);
            cards.push_back(c);
         }
     }
 }
//randomly shuffles the deck
void Deck::shuffle() {
    // generate random seed
    std::random_device rd;
    // this generates a random number
    std::default_random_engine rng(rd());
    std::shuffle(cards.begin(), cards.end(), rng);
}
//deal a card from the deck
void Deck::deal(Hand& hand) {
    //deal from the top
    // add the last card on deck to player's set
    hand.add(cards.back());
    // might be a bit confusing so used this-> to refer to cards from deck
    this->cards.pop_back(); //remove the last card from Cards deck
}
// removes the particular card from the deck
void Deck::remove(Card& c) {
    vector<Card>::iterator iter;
    for (iter = cards.begin(); iter != cards.end(); ++iter) {
        if ((*iter).getValue() == c.getValue()) {
            cards.erase(iter); // remove this object from the deck
            break; // break out of iteration
        }
    }
}


// represents a generic abstract player that can be a human or computer.
// Abstract class -> contains pure virtual method 
class AbstractPlayer {
public:
    Hand hand; // hand instance
    int num_wins = 0; // keeps track of number
    int win_rate = 0; 
    AbstractPlayer(){};
    virtual ~AbstractPlayer(){};
    virtual bool isDrawing() const = 0;
    bool isBusted(Hand& hand);
    void setWinRate(int win_rate);
    int getWinRate();
};
//returns true if player's set sum exceeds 21
bool AbstractPlayer::isBusted(Hand& hand) {
    return hand.getTotal() > 21;
}
void AbstractPlayer::setWinRate(int win_rate) {
    this->win_rate = win_rate;
}

int AbstractPlayer::getWinRate() {
    return this->win_rate;
}

 class HumanPlayer: public AbstractPlayer{
public:
    HumanPlayer(){};
    virtual ~HumanPlayer(){};
    bool isDrawing() const;
    void announce(int res);
};
// Asks users to draw a card if 'y' => draws another card
bool HumanPlayer::isDrawing() const {
    bool drawAgain = false;
    cout << "Do you want to draw? (y/n): ";
    char answer;
    cin >> answer;
    drawAgain = (answer == 'y' ? true : false);

    return drawAgain;
}
void HumanPlayer::announce(int res){
    if (res == 0) {
        cout << "Casino wins.\n" << endl;
    } else if (res == 1) {
        cout << "Player wins.\n" << endl;
    } else {
        cout << "Push: No one wins.\n" << endl;
    }
}


class ComputerPlayer: public AbstractPlayer{
public:
    ComputerPlayer(){};
    virtual ~ComputerPlayer(){};
    bool isDrawing() const;
 };
bool ComputerPlayer::isDrawing() const{
    bool drawAgain = false;
   if (hand.getTotal() <= 16) {
       drawAgain = true;
      }
    return drawAgain; 
}


 class BlackJackGame{
     public:
        int rounds = 0; // keep track of number of rounds
        Deck m_deck; // deck member
        ComputerPlayer m_casino; // casino member
        vector<HumanPlayer> m_player; // player member(s)
        // ask the number of players inside the constructor
        BlackJackGame(){
            cout << "Enter number of Players: ";
            int numPlayers = 0;
            cin >> numPlayers;
            // add the HumanPlayer object to the player vector
            for (int i = 0; i < numPlayers; i++) {
                HumanPlayer newPlayer;
                m_player.push_back(newPlayer);
            } 
        }
        ~BlackJackGame(){};
        void play();
};

void BlackJackGame::play(){
    int winner = -1;

    // at the beginning populate the deck
    // after, shuffle
    // distribute cards to the house and player
    m_deck.populate();
    m_deck.shuffle();

                //*** MANIPULATING WIN RATE LOGIC ***//
    // if the win rate is less than 45 for casino then iterate through the deck // 
    // if the current sum plus another card gives less than equal to 21 //
    // draw this card out of the deck and put it into casino's hand //
    // otherwise just abide by the regular rules where drawn from top //
    // to avoid large win rate discrepancy/probability, thus by the 10 rounds have//
    // been played the casino's win rate should be around 45%~55% give or take //

    // keep track of total number of Players
    int PlayerPool = m_player.size();

    //iterate through the list of players
    vector<HumanPlayer>::iterator iter;
    for (iter = m_player.begin(); iter != m_player.end(); ++iter){
        // deal twice
        m_deck.deal(iter->hand);
        m_deck.deal(iter->hand);
    }
    //deal a card to the casino
    m_deck.deal(m_casino.hand);
    // the first start you must assume that no bust will happen
    // print the house's information
    cout << "Casino: ";
    m_casino.hand.printDeck();
    std:: cout << "[" << m_casino.hand.getTotal() << "]" << endl;
    // print the player's information
    for (int i = 0; i < PlayerPool; i++) {
        std::cout << "Player " << i+1 << ": ";
        m_player[i].hand.printDeck();
        std:: cout << "[" << m_player[i].hand.getTotal() << "]" << endl;
    }
    // keep count of bust
    int bustCount = 0;
    // keep track of player number
    int playerNum = 1;
    //iterate through the players again but now to take turns
    vector<HumanPlayer>::iterator player;
    for (player = m_player.begin(); player != m_player.end(); ++player) {
        // print the which player's turn it is.
        cout << "\nPlayer #" << playerNum << "'s turn.\n";
        // while player has not busted
        while (!player->isBusted(player->hand)){ 
            // ask for another draw
            if(player->isDrawing()) {
                // deal a card each
                m_deck.deal(player->hand);
                // print the player's information
                cout << "Player #" << playerNum << " -> ";
                player->hand.printDeck();
                std:: cout << "[" << player->hand.getTotal() << "]" << endl;
                if (player->hand.getTotal() > 21) {
                    if (playerNum == PlayerPool) {
                        bustCount++; // addBust
                        std::cout << ">> Busted! Casino's turn. <<" << "\nCasino's turn" << endl;
                    } else {
                        playerNum++;
                        bustCount++; //addBust
                        cout << ">> Busted! Next player's turn. <<" << endl;
                    }
                }
            } else {
                if (playerNum == PlayerPool) {
                    cout << ">> Done. Players' turns over. <<\n" << "\nCasino's turn." << endl;
                } else {
                    cout << ">> No more hit. Next player's turn. <<" << endl;
                }
                playerNum++;
                break;
            }
        }
    }    

    while (!m_casino.isBusted(m_casino.hand)) {
        // if score is less than 16 for casino
        if(m_casino.isDrawing()) {
            // // add the hidden card drawn at the beginning to casino's hand
            // // if and only if it the sum does not exceed 21 (basically will only be added once)
            if (m_casino.getWinRate() < 65 && m_casino.hand.getTotal() > 10) {
                Card Blackjack;
                for (int i = 0; i < m_deck.cards.size(); i++) {
                    if( m_casino.hand.getTotal() + m_deck.cards[i].getValue() <= 21) {
                        Blackjack = m_deck.cards[i];
                        m_deck.remove(Blackjack);
                        m_casino.hand.add(Blackjack);
                        break;
                    }
                }
            } else { //otherwise just draw a card from top
                m_deck.deal(m_casino.hand);
            }
            // print the house's information
            cout << "Casino: ";
            m_casino.hand.printDeck();
            std:: cout << "[" << m_casino.hand.getTotal() << "]" << endl;
        } else {
            break; // break out of the loop
        }
    }
    
    // determining win-loss calculation
    // otherwise -> compare each player's result 1 by 1
    // Accumulate he win/loss ratio and then divide it by # of rounds
    // two variables necessary for calculation:
    // player pool size and number of busts
    // cases of busts -> automatic win for casino, loss for player
    // FOR PLAYER
    // => #pools - #busts => #wins
    // FOR CASINO
    // => #busts => #wins

    //casino bust
    bool casino_bust = this->m_casino.isBusted(m_casino.hand);
    //casino score
    int casino_score = this->m_casino.hand.getTotal();
    int numWins = 0;
    int numTies = 0;
    // iterate through the players vector and compare the total with casino's total
    for (iter = m_player.begin(); iter != m_player.end(); ++iter){
        int player_score = iter->hand.getTotal();
        //ignoring busts -> case when no busts exist
        if(casino_score <= 21 && player_score <= 21 && player_score > casino_score){
            numWins++;
        } else if (casino_score == player_score) {
            numTies++;
        }
    }
    // calculate the win rate
    int player_winrate = 0;
    int casino_winrate = 0;
    // if both casino and some hands bust
    if (casino_bust && bustCount > 0) {
        player_winrate = ((PlayerPool - bustCount) * 100)/PlayerPool;
        casino_winrate = (bustCount* 100)/PlayerPool;
    // casino busts but player does not at all
    } else if (casino_bust && bustCount == 0) {
        player_winrate = (PlayerPool * 100)/PlayerPool;
        casino_winrate = 0;
    // otherwise calculate normally depending # wins for player
    } else {
        player_winrate = (numWins * 100)/PlayerPool;
        // account for instances where number of ties exist
        casino_winrate = ((PlayerPool - numWins - numTies) * 100)/PlayerPool;
    }

    // *** PRINTING STATS *** ///
    //** USE this-> keyword to refer to the instance of game, players, and casino objects created at the beg. **//
    // end of round
    this->rounds++; //increment round
    // PRINT CURRENT ROUND NUMBER //
    std::cout << "\n==============================" << endl;
    std::cout << "{}{}{}{ ROUND " << this->rounds;
    std::cout << ": STATS }{}{}{}\n" << "==============================" << endl;
    // PRINT NUMBER OF PLAYER BUSTS //
    std::cout << ">> " << "Total number of busts: " << bustCount << " <<" << endl;
    std::cout << ">> " << "Total number of pushs: " << numTies << " <<" << endl;
    cout << "==============================" << endl;
    cout << "      - Round Result -" << endl;
    std::cout << "Player: " << player_winrate << "%" << endl;
    std::cout << "Casino: " << casino_winrate << "%" << endl;
    // ANNOUNCE WINNER //
    if (player_winrate > casino_winrate) {
        winner = 1;
        cout << "Player scores higher." << endl;
        iter->announce(winner);
    } else {
        winner = 0;
        cout << "Casino scores higher." << endl;
        iter->announce(winner);
    }
    // CUMULATIVE SCORE //
    // retrieve the win rate from previous games //
    // recalculate the cumulative win rate by adding the win rate from this specific round
    int p_prevRate = iter->getWinRate() * (this->rounds -1);
    int c_prevRate = m_casino.getWinRate() * (this->rounds -1);
    int cum_player = (p_prevRate + player_winrate)/this->rounds;
    int cum_casino = (c_prevRate + casino_winrate)/this->rounds;
    m_casino.setWinRate(cum_casino);
    iter->setWinRate(cum_player);

    // //print Score
    cout << "    - Cumulative Result -" << endl;
    std::cout << "Player win %: " << iter->getWinRate() << "%" << endl;
    std::cout << "Casino win %: " << m_casino.getWinRate() << "%" << endl;
    std::cout << "===========================" << endl;

    // clear house and player's decks
    for (iter = m_player.begin(); iter != m_player.end(); ++iter){
        iter->hand.clear();
    }
    m_casino.hand.clear();
}
int main() {

    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
     BlackJackGame game;
     // The main loop of the game
     bool playAgain = true;
     char answer = 'y';
     while (playAgain)
     {
         game.play();
         // Check whether the player would like to play another round
         cout << "Would you like another round? (y/n): -> ";
         cin >> answer;
         cout << endl << endl;
         playAgain = (answer == 'y' ? true : false);
}
     cout <<"\tGave over!"; 
        
    return 0;
}







