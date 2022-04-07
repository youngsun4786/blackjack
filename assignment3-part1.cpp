#include<iostream>
#include<vector>
using std::vector;
 class Card {
     enum Rank {ACE = 1, TWO, THREE, FOUR,FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING} ;
     enum Type {CLUBS, DIAMOND, HEARTS, SPADE};

     Rank value;
     Type suit;
     public:
        int getValue() {
            if (value > TEN) {
                return 10;
            }
            return value;
        }

        void displayCard() {
            
        }
            
        }

 };

// contain a list of cards implemented as an array or vector
 class Hand {

 };

 class Deck {

 };

// represents a generic abstract player that can be a human or computer.
// Abstract class -> contains pure virtual method 
 class AbstractPlayer{
     public:
        AbstractPlayer();
        virtual ~AbstractPlayer();
        virtual bool isDrawing() const = 0;

 };

 class HumanPlayer{

 };

 class ComputerPlayer{

 };

 class BlackJackGame{

 };

int main() {
    


}
