#ifndef CARD_H
#define CARD_H

#include <ostream>
#include <string>

namespace DNRY {
    namespace Cards {
        enum class Suit {
            club, diamond, heart, spade
        };

        enum class Value {
            ace, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king
        };

        class Card {
        public:
            Card(const Suit& suit, const Value& value);
            Suit suit() const;
            Value value() const;
        private:
            Suit s;
            Value v;
        };
        std::string make_string(Suit);
        std::string make_string(Value);
        std::ostream& operator<<(std::ostream&, const Card&);
    }
}

#endif
