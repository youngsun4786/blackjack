#include<iostream>
#include<vector>
using std::vector;

 class Card {
     enum Rank {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK = 10, QUEEN = 10, KING = 10};
     enum Type {CLUBS, DIAMOND, HEARTS, SPADE};
     public:
        int getValue(Rank rank, Type type) {
            int val = 0;
            if (rank == ACE) {
                val = 1;
            } else if (rank == TWO) {
                val = 2;
            } else if (rank == THREE) {
                val = 3;
            }
            return val;
        };


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
