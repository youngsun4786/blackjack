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
        HumanPlayer m_player;   // player member
        BlackJackGame(){}; 
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

    // deal two cards to the player
    for (int i = 0; i < 2; i++) {
        m_deck.deal(m_player.hand);
    }
    //deal a card to the casino
    m_deck.deal(m_casino.hand);
    // the first start you must assume that no bust will happen
    // print the house's information
    cout << "Casino: ";
    m_casino.hand.printDeck();
    std:: cout << "[" << m_casino.hand.getTotal() << "]" << endl;
    // print the player's information
    cout << "Player: ";
    m_player.hand.printDeck();
    std:: cout << "[" << m_player.hand.getTotal() << "]" << endl;
    
    // while player has not busted
    while (!m_player.isBusted(m_player.hand)) { 
        // ask for another draw
        if(m_player.isDrawing()) {
            // deal a card each
            m_deck.deal(m_player.hand);
            // print the player's information
            cout << "Player: ";
            m_player.hand.printDeck();
            std:: cout << "[" << m_player.hand.getTotal() << "]" << endl;
        } else {
            break;
        }
    }

    // first check if the player hasn't busted or not
    if(!m_player.isBusted(m_player.hand)) {
        while (!m_casino.isBusted(m_casino.hand)) {
            // if score is less than 16 for casino
            if(m_casino.isDrawing()) {
                // // add the hidden card drawn at the beginning to casino's hand
                // // if and only if it the sum does not exceed 21 (basically will only be added once)
                if (m_casino.getWinRate() < 55 && m_casino.hand.getTotal() > 10) {
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
    }
    // determining win condition (prioritize busts)
    // 1. compare the results (get higher than the opponent or reach 21)
    // 2. the opponent busts
    bool player_bust = m_player.isBusted(m_player.hand);
    bool casino_bust = m_casino.isBusted(m_casino.hand);
    int casino_score = m_casino.hand.getTotal();
    int player_score = m_player.hand.getTotal();
    // player wins
    if (casino_bust) {
        winner = 1; // 1 --> player wins
        this->m_player.num_wins++; // increment win

        cout << "Casino busts." << endl;
        m_player.announce(winner);
        //increment score for player
    
    // casino wins
    } else if (player_bust) {
        winner = 0;
        this->m_casino.num_wins++; // increment win

        cout << "Player busts." << endl;
        m_player.announce(winner);

    // no busts -> player decides to stop drawing so compare scores
    } else {
        if (player_score > casino_score) {
            winner = 1; // 1 --> player wins
            this->m_player.num_wins++; // increment win

            cout << "Player scores higher." << endl;
            m_player.announce(winner);

        // casino wins
        } else if (casino_score > player_score) {
            winner = 0;
            this->m_casino.num_wins++; // increment win
            cout << "Casino scores higher." << endl;
            m_player.announce(winner);

        // tie (push)
        } else {
            m_player.announce(winner);
        }
    }
    
    //** USE this-> keyword to refer to the instance of game, players, and casino objects created at the beg. **//
    // end of round
    this->rounds++; //increment round
    std::cout << "===========================" << endl;
    std::cout << "{}{}{ ROUND " << this->rounds;
    // calculate scores and update win_rate attribute
    m_player.setWinRate((this->m_player.num_wins * 100)/this->rounds);
    m_casino.setWinRate((this->m_casino.num_wins * 100)/this->rounds);
    //print Score
    std::cout << ": STATS }{}{}\n" << "===========================" << endl;
    std::cout << "   Player win %: " << m_player.getWinRate() << "%" << endl;
    std::cout << "   Casino win %: " << m_casino.getWinRate() << "%" << endl;
    std::cout << "===========================" << endl;

    // clear house and player's decks
    m_casino.hand.clear();
    m_player.hand.clear();
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
 
