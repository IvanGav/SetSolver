#include <iostream>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <bitset>
#include <set>

// #include "sets.h"

using namespace std;

typedef uint8_t card;

bool DEBUG = false;

//deck of all cards as a bitset
bitset<171> all_cards("111011101110000011101110111000001110111011100000000000000000000011101110111000001110111011100000111011101110000000000000000000001110111011100000111011101110000011101110111");
//deck of no cards as a bitset (bitset of 0)
bitset<171> no_cards;

//card numbers (0 - 80) by card (0b00000000 - 0b10101010)
unordered_map<card, int> card_nums;
//actual cards (0b00000000 - 0b10101010) by their number (0 - 80)
vector<card> cards = {0,1,2,4,5,6,8,9,10,16,17,18,20,21,22,24,25,26,32,33,34,36,37,38,40,41,42,64,65,66,68,69,70,72,73,74,80,81,82,84,85,86,88,89,90,96,97,98,100,101,102,104,105,106,128,129,130,132,133,134,136,137,138,144,145,146,148,149,150,152,153,154,160,161,162,164,165,166,168,169,170};

struct card_set {
    card f;
    card s;
    card t;

    card_set(card c1, card c2, card c3) {
        //insert them so that `f` is smallest and `t` is largest
        if(c3 < c2) swap(c2, c3);
        if(c2 < c1) swap(c1, c2);
        if(c3 < c2) swap(c2, c3);
        f = c1; s = c2; t = c3;
    }

    friend bool operator<(const card_set& lhs, const card_set& rhs) {
        if(lhs.f == rhs.f) {
            if(lhs.s == rhs.s) {
                return lhs.t < rhs.t;
            }
            return lhs.s < rhs.s;
        }
        return lhs.f < rhs.f;
    }

    friend bool operator==(const card_set& lhs, const card_set& rhs) {
        return lhs.f == rhs.f && lhs.s == rhs.s && lhs.t == rhs.t;
    }

    friend ostream& operator<<(ostream& os, const card_set& self) {
        return os << (int) self.f << ',' << (int) self.s << ',' << (int) self.t;
    }

    //put all own cards into 'cards' and return true
    //return false **and don't put any cards** if any of the cards are already inserted
    bool set_or(bitset<171>& cards) const {
        if(cards[f] | cards[s] | cards[t]) return false;
        cards.set(f);
        cards.set(s);
        cards.set(t);
        return true;
    }

    //remove all own cards from 'cards'
    void unset(bitset<171>& cards) const {
        cards.reset(f);
        cards.reset(s);
        cards.reset(t);
    }

    bool contains(card& c) {
        return f == c | s == c | t == c;
    }
};

// 0 <= trait_num < 4; 0 <= val < 3
card trait(int trait_num, int val) {
    return val << (trait_num*2);
}

card third_trait(card t1, card t2) {
    if(t1 == t2)
        return t1;
    if(t1 == 0b00 || t2 == 0b00) {
        //from t1 and t2 at least one is 0b00 -> 0b00+0b00 OR 0b00+0b01 OR 0b00+0b10
        // but them being equal is taken care of by the first condition
        if(t1 == 0b01 || t2 == 0b01) {
            return 0b10;
        } else {
            return 0b01;
        }
    } else {
        //t1 and t2 are not 0b00 -> they're 0b01+0b10 OR 0b01+0b01 OR 0b10+0b10
        // but them being equal is taken care of by the first condition
        return 0b00;
    }
}

card from_num(int num) {
    card c = 0;
    c |= trait(0, num%3);
    c |= trait(1, num/3%3);
    c |= trait(2, num/9%3);
    c |= trait(3, num/27%3);
    return c;
}

int into_num(card c) {
    return card_nums[c];
}

card finish_set(card f, card s) {
    card t = 0;
    t |= third_trait((f >> 0) & 0b11, (s >> 0) & 0b11) << 0;
    t |= third_trait((f >> 2) & 0b11, (s >> 2) & 0b11) << 2;
    t |= third_trait((f >> 4) & 0b11, (s >> 4) & 0b11) << 4;
    t |= third_trait((f >> 6) & 0b11, (s >> 6) & 0b11) << 6;
    return t;
}

// int count = 0;
// int count_before_exit = 1000;

bool find_solution_basic(const vector<card_set>& all_sets, bitset<171>& solution, int at, vector<card_set>& solution_seq, int layer) {
    if(all_sets[at].set_or(solution)) {
        if(solution == all_cards) {
            solution_seq.push_back(all_sets[at]);
            return true;
        }
        //could insert a set `all_sets[i]`
        int new_layer = layer+1;
        for(int i = at+1; i < all_sets.size(); i++) {
            if(find_solution_basic(all_sets, solution, i, solution_seq, new_layer)) {
                solution_seq.push_back(all_sets[at]);
                return true;
            }
            // count++;
            // if(count == count_before_exit) exit(1);
        }
        all_sets[at].unset(solution);
        // if(layer <= 11) cout << layer << 'd';
    }
    //couldn't insert a set `all_sets[i]`
    return false;
}

bool find_solution_basic_entry(vector<card_set>& all_sets, bitset<171>& solution, int at, vector<card_set>& solution_seq) {
    at = 118; //temp
    if(all_sets[at].set_or(solution)) {
        if(solution == all_cards) {
            solution_seq.push_back(all_sets[at]);
            return true;
        }
        //could insert a set `all_sets[i]`
        for(int i = at+1; i < all_sets.size(); i++) {
            if(find_solution_basic(all_sets, solution, i, solution_seq, 1)) {
                solution_seq.push_back(all_sets[at]);
                return true;
            }
            cout << i << endl << endl << endl << endl;
        }
        all_sets[at].unset(solution);
    }
    //couldn't insert a set `all_sets[i]`
    return false;
}

/*
    (Re)generating functions
*/

//generate a bitset of all cards interpreted as numbers
bitset<171> gen_deck_bitset() {
    bitset<171> deck;
    //3,240 pairs (81 choose 2)
    for(int i = 0; i < 81; i++) {
        card c = from_num(i);
        deck[c] = true;
    }
    if(DEBUG) cout << "Deck: " << deck << endl;
    return deck;
}

//generate a vector of cards (int the correct order) `num_to_card` and a hashmap of card to their number `card_to_num`
void gen_all_cards(vector<card>& num_to_card, unordered_map<card, int>& card_to_num) {
    num_to_card.clear();
    card_to_num.clear();
    for(int i = 0; i < 81; i++) {
        card c = from_num(i);
        num_to_card.push_back(c);
        card_to_num[c] = i;
    }
    if(DEBUG) {
        cout << "All cards (0th to 80th): {";
        for(int i = 0; i < num_to_card.size(); i++) {
            cout << (int) num_to_card[i] << ',';
        }
        cout << '}' << endl;
        cout << "All card pairs (card, int): {";
        for(auto i : card_to_num) {
            cout << '(' << (int) i.first << ',' << i.second << "),";
        }
        cout << '}' << endl;
    }
}

//generate a vector of all sets
void get_all_sets(vector<card_set>& all_sets, const vector<card>& all_cards) {
    set<card_set> all_sets_set;
    for(int i = 0; i < all_cards.size()-1; i++) {
        for(int j = i+1; j < all_cards.size(); j++) {
            // card_set s = {all_cards[i],all_cards[j],finish_set(all_cards[i], all_cards[j])};
            // all_sets_set.insert(s);
            all_sets_set.emplace(all_cards[i],all_cards[j],finish_set(all_cards[i], all_cards[j]));
            // cout << s << '(' << (int) all_cards[i] << ',' << (int) all_cards[j] << ',' << (int) finish_set(all_cards[i], all_cards[j]) << ')' << ';';
        }
    }
    all_sets.clear();
    all_sets = vector<card_set>(all_sets_set.begin(), all_sets_set.end());

    if(DEBUG) {
        cout << '{';
        for(int i = 0; i < all_sets.size(); i++) {
            cout << '{' << all_sets[i] << "},";
        }
        cout << '}' << endl;
        cout << "size=" << all_sets.size() << endl;
    }
}

/*
    Main
*/

int main() {
    // bitset<171> deck;
    // //3,240 pairs (81 choose 2)
    // for(int i = 0; i < 81; i++) {
    //     card c = from_num(i);
    //     cout << "i=" << i << ", (int)card=" << (int) c << ", card=" << bitset<8>(c) << endl;
    //     cards.push_back(c);
    //     deck[c] = true;
    // }
    // cout << deck << endl;
    // int c1, c2;
    // cin >> c1 >> c2;
    // cout << bitset<8>(cards[c1]) << ", " << bitset<8>(cards[c2]) << "->" << bitset<8>(finish_set(cards[c1], cards[c2])) << endl;

    // ^^^ Code that was used to calculate things, that are now hardcoded or can be calculated with gen_deck_bitset or gen_all_cards

    vector<card_set> all_sets; //it's a bit too long to hard code, easier to generate every time + not that hard to do
    // for(int i = 0; i < cards.size()-1; i++) {
    //     for(int j = i+1; j < cards.size(); j++) {
    //         card_set s = {cards[i],cards[j],finish_set(cards[i], cards[j])};
    //         all_sets.push_back(s);
    //     }
    // }

    // ^^^ will give you triple the actual sets - (0,1,2), (0,2,1), (1,2,0)

    get_all_sets(all_sets, cards);

    // cout << '{';
    // for(int i = 0; i < all_sets.size(); i++) {
    //     cout << '{' << all_sets[i] << "},";
    // }
    // cout << '}' << endl << endl;
    // cout << "size=" << all_sets.size() << endl;

    // ^^^ print all sets

    //try to find 1 solution
    vector<card_set> solution_seq;
    bitset<171> solution;
    int i = 0;
    cout << "--enter" << endl;
    find_solution_basic_entry(all_sets, solution, i, solution_seq);
    cout << "--exit" << endl;

    cout << '[';
    for(int i = 0; i < solution_seq.size(); i++) {
        cout << '(' << solution_seq[i] << "),";
    }
    cout << ']' << endl;

    cout << ((solution == all_cards) ? "the final board contains all cards" : 
                                       "BAD THE FINAL BOARD IS MISSING CARDS OR HAS TOO MANY") << endl;
    return 0;
}

/*
    Winning boards:
    [(168,169,170),(164,165,166),(160,161,162),(152,153,154),(148,149,150),(144,145,146),(136,137,138),(132,133,134),(128,129,130),(104,105,106),(100,101,102),(96,97,98),(88,89,90),(84,85,86),(80,81,82),(72,73,74),(68,69,70),(64,65,66),(40,41,42),(36,37,38),(32,33,34),(24,25,26),(20,21,22),(16,17,18),(8,9,10),(4,5,6),(0,1,2),]
*/