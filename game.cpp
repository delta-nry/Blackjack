#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <thread>
#include <algorithm>
#include <chrono>

#include "game.h"
#include "card.h"

using namespace DNRY;

using Hand = std::vector<Cards::Card>;
using Deck = std::stack<Cards::Card>;

void check_win(const Hand&, const Hand&);
bool dealer_stand_or_hit(Deck&, Hand&);
std::string hand_to_str(const Hand&);
std::string hand_to_str(const Hand&, bool show_hidden_card);
int hand_value(const Hand&);
int hand_value(const Hand&, bool add_hidden_card);
void print_hands(const Hand& player_hand, const Hand& dealer_hand);
void print_hands(const Hand& player_hand, const Hand& dealer_hand,
                                          bool show_hidden_card);
Deck make_random_deck();
bool take(Deck& d, Hand& h, unsigned n);

void Blackjack::game() {
    std::string sel{};
    while (sel != "n") {
        // "reshuffle" deck after every round
        std::cout << "\n----------\n\nShuffling deck...\n";
        Deck deck = make_random_deck();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        Hand dealer_hand{};
        Hand player_hand{};

        take(deck, dealer_hand, 2);
        take(deck, player_hand, 2);

        if (hand_value(dealer_hand) == 21 && hand_value(player_hand) != 21) {
            std::cout << '\n';
            print_hands(player_hand, dealer_hand);
            std::cout << "\nDealer has blackjack; dealer wins\n";
        }
        else if (hand_value(dealer_hand) == 21
                        && hand_value(player_hand) == 21) {
            std::cout << '\n';
            print_hands(player_hand, dealer_hand);
            std::cout << "\nDealer and player have blackjack; push\n";
        }
        else {
            std::string player_sel{};
            while (player_sel != "s") {
                if (hand_value(player_hand) >= 21) {
                    std::cout << '\n';
                    print_hands(player_hand, dealer_hand, false);
                    break;
                }
                player_sel = "";
                while (player_sel != "h" && player_sel != "s") {
                    std::cout << '\n';
                    print_hands(player_hand, dealer_hand, false);
                    std::cout << "\nStand or hit (s/h)? ";
                    std::getline(std::cin, player_sel);
                    if (player_sel == "h") {
                        if (!take(deck, player_hand, 1)) {
                            std::cout << "\nDeck is empty; no cards taken\n";
                        }
                        break;
                    }
                    else if (player_sel == "s") {
                        break;
                    }
                }
            }

            std::cout << "\n--Dealer's round--\n\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            print_hands(player_hand, dealer_hand);
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            while (dealer_stand_or_hit(deck, dealer_hand)) {
                std::cout << '\n';
                print_hands(player_hand, dealer_hand);
                std::this_thread::sleep_for(std::chrono::milliseconds(750));
            }
            std::cout << '\n';
            check_win(player_hand, dealer_hand);
        }
        sel = "";
        while (sel != "y" && sel != "n") {
            std::cout << "\nAnother round (y/n)? ";
            std::getline(std::cin, sel);
        }
    }
}

void check_win(const Hand& player_hand, const Hand& dealer_hand) {
    if (hand_value(player_hand) > 21 && hand_value(dealer_hand) <= 21) {
        std::cout << "Player busts; dealer wins" << '\n';
    }
    else if (hand_value(player_hand) <= 21 && hand_value(dealer_hand) > 21) {
        std::cout << "Dealer busts; player wins" << '\n';
    }
    else if (hand_value(player_hand) > 21 && hand_value(dealer_hand) > 21) {
        std::cout << "Dealer busts; player busts" << '\n';
    }
    else if (hand_value(player_hand) > hand_value(dealer_hand)) {
        std::cout << "Player wins" << '\n';
    }
    else if (hand_value(player_hand) < hand_value(dealer_hand)) {
        std::cout << "Dealer wins" << '\n';
    }
    else {
        std::cout << "Push" << '\n';
    }
}

// Dealer always hits on 16 or lower, always stands on 17 or higher.
// Return true on hit, return false on stand or when deck d is
// empty.
bool dealer_stand_or_hit(Deck& d, Hand& h) {
    if (hand_value(h) <= 16) {
        if (!take(d, h, 1)) {
            std::cout << "\nDeck is empty; no cards taken\n";
            return false;
        }
        return true;
    }
    return false;
}

std::string hand_to_str(const Hand& h) {
    std::string s{};
    for (auto card : h) {
        s.append(Cards::make_string(card.value()));
        s.append(Cards::make_string(card.suit()));
        s.append(" ");
    }
    // remove trailing space if hand is not empty
    if (!s.empty()) {
        s.pop_back();
    }
    return s;
}

// If show_hidden_card == false, the first card is hidden.
// Otherwise, functionality is the same as hand_to_str(const Hand&).
std::string hand_to_str(const Hand& h, bool show_hidden_card) {
    if (!show_hidden_card) {
        std::string s{};
        s.append("?? ");
        for (auto it = ++h.begin(); it != h.end(); ++it) {
            s.append(Cards::make_string(it->value()));
            s.append(Cards::make_string(it->suit()));
            s.append(" ");
        }
        // remove trailing space if hand is not empty
        if (!s.empty()) {
            s.pop_back();
        }
        return s;
    }
    else {
        return hand_to_str(h);
    }
}

int hand_value(const Hand& h) {
    int i{};
    for (auto card : h) {
        switch (card.value()) {
        case Cards::Value::ace: {
            if (i > 10) {
                i += 1;
            }
            else {
                i += 11;
            }
            break;
        }
        case Cards::Value::two: {
            i += 2;
            break;
        }
        case Cards::Value::three: {
            i += 3;
            break;
        }
        case Cards::Value::four: {
            i += 4;
            break;
        }
        case Cards::Value::five: {
            i += 5;
            break;
        }
        case Cards::Value::six: {
            i += 6;
            break;
        }
        case Cards::Value::seven: {
            i += 7;
            break;
        }
        case Cards::Value::eight: {
            i += 8;
            break;
        }
        case Cards::Value::nine: {
            i += 9;
            break;
        }
        case Cards::Value::ten: {
            i += 10;
            break;
        }
        case Cards::Value::jack: {
            i += 10;
            break;
        }
        case Cards::Value::queen: {
            i += 10;
            break;
        }
        case Cards::Value::king: {
            i += 10;
            break;
        }
        }
    }
    return i;
}

// If add_hidden_card == false, the value of the first card is not
// added to the overall hand_value.
// Otherwise, functionality is the same as hand_value(const Hand&).
int hand_value(const Hand& h, bool add_hidden_card) {
    if (!add_hidden_card) {
        int i{};
        for (auto it = ++h.begin(); it != h.end(); ++it) {
            switch (it->value()) {
            case Cards::Value::ace: {
                if (i > 10) {
                    i += 1;
                }
                else {
                    i += 11;
                }
                break;
            }
            case Cards::Value::two: {
                i += 2;
                break;
            }
            case Cards::Value::three: {
                i += 3;
                break;
            }
            case Cards::Value::four: {
                i += 4;
                break;
            }
            case Cards::Value::five: {
                i += 5;
                break;
            }
            case Cards::Value::six: {
                i += 6;
                break;
            }
            case Cards::Value::seven: {
                i += 7;
                break;
            }
            case Cards::Value::eight: {
                i += 8;
                break;
            }
            case Cards::Value::nine: {
                i += 9;
                break;
            }
            case Cards::Value::ten: {
                i += 10;
                break;
            }
            case Cards::Value::jack: {
                i += 10;
                break;
            }
            case Cards::Value::queen: {
                i += 10;
                break;
            }
            case Cards::Value::king: {
                i += 10;
                break;
            }
            }
        }
        return i;
    }
    else {
        return hand_value(h);
    }
}

void print_hands(const Hand& player_hand, const Hand& dealer_hand) {
    std::cout << "Dealer: " << hand_to_str(dealer_hand) << " - "
        << hand_value(dealer_hand) << '\n';
    std::cout << "Player: " << hand_to_str(player_hand) << " - "
        << hand_value(player_hand) << '\n';
}

// If show_hidden_card == false, the first card and its value is
// hidden. Otherwise, functionality is the same as
// print_hands(const Hand& player_hand, const Hand& dealer_hand)
void print_hands(const Hand& player_hand, const Hand& dealer_hand,
            bool show_hidden_card) {
    if (show_hidden_card) {
        print_hands(player_hand, dealer_hand);
    }
    else {
        std::cout << "Dealer: " << hand_to_str(dealer_hand, false) << " - ?+"
            << hand_value(dealer_hand, false) << '\n';
        std::cout << "Player: " << hand_to_str(player_hand) << " - "
            << hand_value(player_hand) << '\n';
    }
}

Deck make_random_deck() {
    std::vector<Cards::Card> v{};

    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::ace));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::two));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::three));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::four));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::five));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::six));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::seven));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::eight));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::nine));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::ten));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::jack));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::queen));
    v.push_back(Cards::Card(Cards::Suit::spade, Cards::Value::king));

    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::ace));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::two));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::three));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::four));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::five));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::six));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::seven));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::eight));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::nine));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::ten));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::jack));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::queen));
    v.push_back(Cards::Card(Cards::Suit::heart, Cards::Value::king));

    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::ace));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::two));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::three));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::four));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::five));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::six));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::seven));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::eight));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::nine));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::ten));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::jack));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::queen));
    v.push_back(Cards::Card(Cards::Suit::diamond, Cards::Value::king));

    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::ace));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::two));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::three));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::four));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::five));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::six));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::seven));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::eight));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::nine));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::ten));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::jack));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::queen));
    v.push_back(Cards::Card(Cards::Suit::club, Cards::Value::king));

    std::random_device rd;
    std::mt19937 engine{rd()};
    std::shuffle(std::begin(v), std::end(v), engine);

    Deck d{};
    for (auto card : v) {
        d.push(card);
    }
    return d;
}

// Remove n number of cards from the top of Deck d and push each back
// into Hand h.
// Return false if there are less than n cards in d, otherwise return
// true.
bool take(Deck& d, Hand& h, unsigned n) {
    if (d.size() < n) {
        std::cout << "\nDeck has less than " << n
            << "cards; no cards taken\n";
        return false;
    }
    for (unsigned i = 0; i < n; ++i) {
        h.push_back(d.top());
        d.pop();
    }
    return true;
}

