#ifndef PLANSZA_H
#define PLANSZA_H

#include <vector>
#include "Pionek.h"

const int SIZE = 8;

class Plansza {
private:
    std::vector<std::vector<Pionek>> pola;

public:
    Plansza();
    void wyswietl();
    bool wykonajRuch(int sx, int sy, int dx, int dy);
    std::vector<std::pair<int, int>> mozliweRuchy(Gracz gracz);
    std::vector<std::tuple<int, int, int, int>> wszystkieRuchy(Gracz gracz) const;
    bool czyRuchJestMozliwy(int sx, int sy, int dx, int dy) const;
    Pionek getPionek(int x, int y) const;
    void setPionek(int x, int y, const Pionek& pionek);
    void usunPionek(int x, int y);
    bool czyKoniecGry() const;
};

#endif // PLANSZA_H