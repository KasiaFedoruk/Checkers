#ifndef PIONEK_H
#define PIONEK_H

#include <SFML/Graphics/Color.hpp>

enum class Gracz { BRAK, BIALE, CZARNE };

struct Pionek {
    Gracz gracz;
    bool damka;
    sf::Color kolor = sf::Color::Transparent;

    Pionek();
    Pionek(Gracz gracz);
};

#endif // PIONEK_H