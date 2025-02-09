#include "Plansza.h"
#include <iostream>

Plansza::Plansza() {
    pola.resize(SIZE, std::vector<Pionek>(SIZE));
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (i % 2 != j % 2 && (i < 3 || i > 4)) {
                pola[i][j] = Pionek((i < 3) ? Gracz::CZARNE : Gracz::BIALE);
            }
        }
    }
}

void Plansza::wyswietl() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (pola[i][j].gracz == Gracz::BIALE) {
                std::cout << (pola[i][j].damka ? 'B' : 'b') << " ";
            } else if (pola[i][j].gracz == Gracz::CZARNE) {
                std::cout << (pola[i][j].damka ? 'C' : 'c') << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

bool Plansza::wykonajRuch(int sx, int sy, int dx, int dy) {
    if (dx < 0 || dx >= SIZE || dy < 0 || dy >= SIZE || pola[sx][sy].gracz == Gracz::BRAK || pola[dx][dy].gracz != Gracz::BRAK) {
        return false;
    }

    int kierunek = (pola[sx][sy].gracz == Gracz::BIALE) ? -1 : 1;
    bool czyBicie = abs(dx - sx) == 2 && abs(dy - sy) == 2;

    if (!pola[sx][sy].damka) {
        if (!czyBicie && (dx - sx != kierunek || abs(dy - sy) != 1)) {
            return false;
        }
        if (czyBicie && (abs(dx - sx) != 2 || abs(dy - sy) != 2)) {
            return false;
        }
    } else {
        // Damka może poruszać się o dowolną liczbę pól w obu kierunkach
        if (abs(dx - sx) != abs(dy - sy)) {
            return false;
        }
        bool bicieDamka = false;
        int k = abs(dx - sx);
        int ix, iy;
        for (int i = 1; i < k; ++i) {
            ix = sx + i * (dx > sx ? 1 : -1);
            iy = sy + i * (dy > sy ? 1 : -1);
            if (pola[ix][iy].gracz != Gracz::BRAK) {
                if (pola[ix][iy].gracz != pola[sx][sy].gracz) {
                    bicieDamka = true;
                } else {
                    return false; // Cannot move over own piece
                }
            }
        }
        if (bicieDamka) {
            for (int i = 1; i < k; ++i) {
                ix = sx + i * (dx > sx ? 1 : -1);
                iy = sy + i * (dy > sy ? 1 : -1);
                if (pola[ix][iy].gracz != Gracz::BRAK && pola[ix][iy].gracz != pola[sx][sy].gracz) {
                    pola[ix][iy] = Pionek(); // Usunięcie zbitego pionka
                    break;
                }
            }
        }
    }

    if (czyBicie) {
        int mx = (sx + dx) / 2;
        int my = (sy + dy) / 2;
        if (pola[mx][my].gracz == Gracz::BRAK || pola[mx][my].gracz == pola[sx][sy].gracz) {
            return false;
        }
        pola[mx][my] = Pionek(); // Usunięcie zbitego pionka
    }

    pola[dx][dy] = pola[sx][sy];
    pola[sx][sy] = Pionek();

    // Zamiana pionka na damkę i zmiana koloru
    if ((dx == 0 && pola[dx][dy].gracz == Gracz::BIALE) || (dx == SIZE - 1 && pola[dx][dy].gracz == Gracz::CZARNE)) {
        pola[dx][dy].damka = true;
        if (pola[dx][dy].gracz == Gracz::BIALE) {
            pola[dx][dy].kolor = sf::Color::Magenta; // Różowy
        } else {
            pola[dx][dy].kolor = sf::Color::Red; // Czerwony
        }
    }

    return true;
}

std::vector<std::tuple<int, int, int, int>> Plansza::wszystkieRuchy(Gracz gracz) const {
    std::vector<std::tuple<int, int, int, int>> ruchy;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (pola[i][j].gracz == gracz) {
                int kierunek = (gracz == Gracz::BIALE) ? -1 : 1;
                if (pola[i][j].damka) {
                    // Damka może poruszać się w obu kierunkach i bić przeciwnika
                    for (int d = 1; d < SIZE; ++d) {
                        for (int dir = 0; dir < 4; ++dir) {
                            int dx = i + d * ((dir < 2) ? 1 : -1);
                            int dy = j + d * ((dir % 2 == 0) ? 1 : -1);
                            if (czyRuchJestMozliwy(i, j, dx, dy)) {
                                ruchy.emplace_back(i, j, dx, dy);
                            }
                        }
                    }
                } else {
                    // Zwykłe pionki
                    if (czyRuchJestMozliwy(i, j, i + kierunek, j - 1)) {
                        ruchy.emplace_back(i, j, i + kierunek, j - 1);
                    }
                    if (czyRuchJestMozliwy(i, j, i + kierunek, j + 1)) {
                        ruchy.emplace_back(i, j, i + kierunek, j + 1);
                    }
                    // Obsługa bicia
                    if (czyRuchJestMozliwy(i, j, i + 2 * kierunek, j - 2) &&
                        pola[i + kierunek][j - 1].gracz != gracz &&
                        pola[i + kierunek][j - 1].gracz != Gracz::BRAK) {
                        ruchy.emplace_back(i, j, i + 2 * kierunek, j - 2);
                    }
                    if (czyRuchJestMozliwy(i, j, i + 2 * kierunek, j + 2) &&
                        pola[i + kierunek][j + 1].gracz != gracz &&
                        pola[i + kierunek][j + 1].gracz != Gracz::BRAK) {
                        ruchy.emplace_back(i, j, i + 2 * kierunek, j + 2);
                    }
                    // Bicie do tyłu dla białych pionków
                    if (gracz == Gracz::BIALE) {
                        if (czyRuchJestMozliwy(i, j, i - 2 * kierunek, j - 2) &&
                            pola[i - kierunek][j - 1].gracz != gracz &&
                            pola[i - kierunek][j - 1].gracz != Gracz::BRAK) {
                            ruchy.emplace_back(i, j, i - 2 * kierunek, j - 2);
                        }
                        if (czyRuchJestMozliwy(i, j, i - 2 * kierunek, j + 2) &&
                            pola[i - kierunek][j + 1].gracz != gracz &&
                            pola[i - kierunek][j + 1].gracz != Gracz::BRAK) {
                            ruchy.emplace_back(i, j, i - 2 * kierunek, j + 2);
                        }
                    }
                    // Bicie do tyłu dla czarnych pionków
                    if (gracz == Gracz::CZARNE) {
                        if (czyRuchJestMozliwy(i, j, i - 2 * kierunek, j - 2) &&
                            pola[i - kierunek][j - 1].gracz != gracz &&
                            pola[i - kierunek][j - 1].gracz != Gracz::BRAK) {
                            ruchy.emplace_back(i, j, i - 2 * kierunek, j - 2);
                        }
                        if (czyRuchJestMozliwy(i, j, i - 2 * kierunek, j + 2) &&
                            pola[i - kierunek][j + 1].gracz != gracz &&
                            pola[i - kierunek][j + 1].gracz != Gracz::BRAK) {
                            ruchy.emplace_back(i, j, i - 2 * kierunek, j + 2);
                        }
                    }
                }
            }
        }
    }
    return ruchy;
}

bool Plansza::czyRuchJestMozliwy(int sx, int sy, int dx, int dy) const {
    if (dx < 0 || dx >= SIZE || dy < 0 || dy >= SIZE) {
        return false;
    }
    if (pola[dx][dy].gracz != Gracz::BRAK) {
        return false;
    }
    return true;
}

Pionek Plansza::getPionek(int x, int y) const {
    return pola[x][y];
}

void Plansza::setPionek(int x, int y, const Pionek& pionek) {
    pola[x][y] = pionek;
}

void Plansza::usunPionek(int x, int y) {
    pola[x][y] = Pionek();
}

bool Plansza::czyKoniecGry() const {
    bool bialeMajaRuchy = !wszystkieRuchy(Gracz::BIALE).empty();
    bool czarneMajaRuchy = !wszystkieRuchy(Gracz::CZARNE).empty();

    return !bialeMajaRuchy || !czarneMajaRuchy;
}

