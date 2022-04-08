#include<iostream>
#include<vector>
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
        case CLUBS: cout << "C\n"; break;
        case DIAMONDS: cout << "D\n"; break;
        case HEARTS: cout << "H\n"; break;
        case SPADES: cout << "S\n"; break;
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
    }
}
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
    void deal(Hand& h);
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
    std::random_shuffle(cards.begin(), cards.end());
 }
//deal a card from the deck
 void Deck::deal(Hand& h) {
    //deal from the top
    // add the last card on deck to player's set
    h.add(cards.back());
    cards.pop_back(); //remove the last card from Cards deck
 }


// represents a generic abstract player that can be a human or computer.
// Abstract class -> contains pure virtual method 
 class AbstractPlayer {
     public:
        Hand player;
        AbstractPlayer(){};
        virtual ~AbstractPlayer(){};
        virtual bool isDrawing() const = 0;
        bool isBusted(Hand& h); 
 };
//returns true if player's set sum exceeds 21
bool AbstractPlayer::isBusted(Hand& h) {
    return h.getTotal() > 21;
}


 class HumanPlayer: public AbstractPlayer{
     public:
        // ask this tomorrow during tutorial
        HumanPlayer(){};
        virtual ~HumanPlayer(){};
        bool isDrawing() const;
        void announce(int res);
 };
// Asks users to draw a card if 'y' => draws another card
bool HumanPlayer::isDrawing() const {
    bool drawAgain = false;
    cout << "Do you want to draw? (y/n):";
    char answer;
    cin >> answer;
    drawAgain = (answer == 'y' ? true : false);
    return drawAgain;
}
void HumanPlayer::announce(int res){
    if (res == 0) {
        cout << "Casino wins.";
    } else if (res == 1) {
        cout << "Player wins.";
    } else {
        cout << "Push: No one wins.";
    }
}

class ComputerPlayer: public AbstractPlayer{
     ComputerPlayer(){};
     virtual ~ComputerPlayer(){};
     bool isDrawing() const;
 };
bool ComputerPlayer::isDrawing() const{
    bool drawAgain = false;
   if (player.getTotal() <= 16) {
       drawAgain = true;
      }
      return drawAgain; 
}
 class BlackJackGame{
     public:   
        Deck m_deck;
        ComputerPlayer m_casino;
        void play(){
            cout << "What's good" << endl;
        }
 };

int main() {
    Deck d;
    d.populate();
    cout << d.getTotal() << endl;
    d.shuffle();
    HumanPlayer p1;
    d.deal(p1.player);
    p1.player.printDeck();
    cout << d.cards.size() << endl;
    d.deal(p1.player);
    p1.player.printDeck();
    cout << d.cards.size() << endl;




//     cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
//      BlackJackGame game;
//      // The main loop of the game
//      bool playAgain = true;
//      char answer = 'y';
//      while (playAgain)
//      {
//          game.play();
//          // Check whether the player would like to play another round
//          cout << "Would you like another round? (y/n): ";
//          cin >> answer;
//          cout << endl << endl;
//          playAgain = (answer == 'y' ? true : false);
// }
//      cout <<"Gave over!";     
    return 0;
}
 
