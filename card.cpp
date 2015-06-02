#include <ostream>
#include <string>
#include <cassert>

#include "card.h"

using namespace DNRY;

Cards::Card::Card(const Suit& suit, const Value& value) : s{suit}, v{value} {}

Cards::Suit Cards::Card::suit() const {
    return s;
}

Cards::Value Cards::Card::value() const {
    return v;
}

std::string Cards::make_string(Suit s) {
#ifdef _WIN32
    // workaround for VS2013's Windows-1252 code page format and lack
    // of u8 string literals
    const auto club = "C";
    const auto diamond = "D";
    const auto heart = "H";
    const auto spade = "S";
#else
    const auto club = u8"♣";
    const auto diamond = u8"♦";
    const auto heart = u8"♥";
    const auto spade = u8"♠";
#endif
    switch (s) {
    case Cards::Suit::club: {
        return std::string(club);
    }
    case Cards::Suit::diamond: {
        return std::string(diamond);
    }
    case Cards::Suit::heart: {
        return std::string(heart);
    }
    case Cards::Suit::spade: {
        return std::string(spade);
    }
    default: {
        assert(false);
        return "";
    }
    }
}

std::string Cards::make_string(Value v) {
    switch (v) {
    case Cards::Value::ace:
        return "A";
    case Cards::Value::two:
        return "2";
    case Cards::Value::three:
        return "3";
    case Cards::Value::four:
        return "4";
    case Cards::Value::five:
        return "5";
    case Cards::Value::six:
        return "6";
    case Cards::Value::seven:
        return "7";
    case Cards::Value::eight:
        return "8";
    case Cards::Value::nine:
        return "9";
    case Cards::Value::ten:
        return "10";
    case Cards::Value::jack:
        return "J";
    case Cards::Value::queen:
        return "Q";
    case Cards::Value::king:
        return "K";
    default: {
        assert(false);
        return "";
    }
    }
}

std::ostream& Cards::operator<<(std::ostream& os, const Cards::Card& c) {
    os << Cards::make_string(c.value()) << Cards::make_string(c.suit());
    return os;
}
