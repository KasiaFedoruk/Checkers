#ifndef GRA_H
#define GRA_H

#include "Plansza.h"
#include <utility>
#include <chrono>

class Gra {
private:
    Plansza plansza;
    Gracz aktualnyGracz;

    int minimax(Plansza& plansza, int glebokosc, bool maxGracz, int alfa, int beta);
    std::tuple<int, int, int, int> znajdzNajlepszyRuch();
    int ocenaHeurystyczna(const Plansza& plansza) const;

public:
    Gra();
    void uruchom();
    void ruchKomputera();
    void ruchUzytkownika();
    bool wykonajRuch(int sx, int sy, int dx, int dy);
    void ustawKolejnyGracz(Gracz gracz);
    Gracz getAktualnyGracz() const;
    Plansza getPlansza() const;
};

#endif // GRA_H