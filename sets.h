#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <bitset>

using namespace std;

typedef uint8_t card;

struct card_set {
    card f;
    card s;
    card t;
    
    friend ostream& operator<<(ostream& os, const card_set& self) {
        return os << (int) self.f << ',' << (int) self.s << ',' << (int) self.t;
    }

    //put all own cards into 'cards' and return true
    //return false **and don't put any cards** if any of the cards are already inserted
    bool set_or(bitset<170>& cards) {
        if(cards[f] | cards[s] | cards[t]) return false;
        cards.set(f);
        cards.set(s);
        cards.set(t);
    }

    //remove all own cards from 'cards'
    void unset(bitset<170>& cards) {
        cards.reset(f);
        cards.reset(s);
        cards.reset(t);
    }

    bool contains(card& c) {
        return f == c | s == c | t == c;
    }
};

vector<card_set> all_sets = {{1,2,3},{2,3,5}};

extern bitset<170> all_cards;
bitset<170> no_cards;

unordered_map<card, int> card_nums;
vector<card> cards;