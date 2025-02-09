#include "Gra.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <algorithm>
#include <chrono>

Gra::Gra() : aktualnyGracz(Gracz::BIALE) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void Gra::uruchom() {
    while (true) {
        plansza.wyswietl();
        if (aktualnyGracz == Gracz::BIALE) {
            ruchUzytkownika();
            aktualnyGracz = Gracz::CZARNE;
            plansza.wyswietl(); // Wyświetl planszę po ruchu użytkownika
        } else {
            ruchKomputera();
            aktualnyGracz = Gracz::BIALE;
            plansza.wyswietl(); // Wyświetl planszę po ruchu komputera
        }
    }
}

void Gra::ruchKomputera() {
    auto najlepszyRuch = znajdzNajlepszyRuch();
    int sx = std::get<0>(najlepszyRuch);
    int sy = std::get<1>(najlepszyRuch);
    int dx = std::get<2>(najlepszyRuch);
    int dy = std::get<3>(najlepszyRuch);
    plansza.wykonajRuch(sx, sy, dx, dy);
}

void Gra::ruchUzytkownika() {
    int sx, sy, dx, dy;
    std::cout << "Podaj ruch (sx sy dx dy): ";
    std::cin >> sx >> sy >> dx >> dy;
    while (!plansza.wykonajRuch(sx, sy, dx, dy)) {
        std::cout << "Nieprawidłowy ruch. Spróbuj ponownie: ";
        std::cin >> sx >> sy >> dx >> dy;
    }
}

int Gra::minimax(Plansza& plansza, int glebokosc, bool maxGracz, int alfa, int beta) {
    if (glebokosc == 0 || plansza.czyKoniecGry()) {
        return ocenaHeurystyczna(plansza);
    }

    auto ruchy = plansza.wszystkieRuchy(maxGracz ? Gracz::CZARNE : Gracz::BIALE);
    if (ruchy.empty()) {
        return maxGracz ? -1000 : 1000;  // Przegrana lub wygrana
    }

    int najlepszaWartosc = maxGracz ? -10000 : 10000;
    for (auto& ruch : ruchy) {
        Plansza nowaPlansza = plansza;
        if (nowaPlansza.wykonajRuch(std::get<0>(ruch), std::get<1>(ruch), std::get<2>(ruch), std::get<3>(ruch))) {
            int wartosc = minimax(nowaPlansza, glebokosc - 1, !maxGracz, alfa, beta);
            if (maxGracz) {
                najlepszaWartosc = std::max(najlepszaWartosc, wartosc);
                alfa = std::max(alfa, wartosc);
            } else {
                najlepszaWartosc = std::min(najlepszaWartosc, wartosc);
                beta = std::min(beta, wartosc);
            }
            if (beta <= alfa) {
                break;
            }
        }
    }
    return najlepszaWartosc;
}

std::tuple<int, int, int, int> Gra::znajdzNajlepszyRuch() {
    std::vector<std::tuple<int, int, int, int>> ruchy = plansza.wszystkieRuchy(Gracz::CZARNE);
    std::tuple<int, int, int, int> najlepszyRuch;
    int najlepszaWartosc = std::numeric_limits<int>::max(); // Minimalizujący gracz, więc szukamy minimum

    for (const auto& ruch : ruchy) {
        Plansza nowaPlansza = plansza;
        if (nowaPlansza.wykonajRuch(std::get<0>(ruch), std::get<1>(ruch), std::get<2>(ruch), std::get<3>(ruch))) {
            int wartosc = minimax(nowaPlansza, 4, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()); // głębokość jest ustalona wcześniej

            if (wartosc < najlepszaWartosc) {
                najlepszaWartosc = wartosc;
                najlepszyRuch = ruch;
            }
        }
    }

    return najlepszyRuch;
}

int Gra::ocenaHeurystyczna(const Plansza& plansza) const {
    int ocena = 0;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            Pionek pionek = plansza.getPionek(i, j);
            if (pionek.gracz == Gracz::BIALE) {
                ocena += pionek.damka ? 7 : 3;  // Damka jest warta więcej niż zwykły pionek
            } else if (pionek.gracz == Gracz::CZARNE) {
                ocena -= pionek.damka ? 7 : 3;  //
            }
        }
    }

    return ocena;
}

bool Gra::wykonajRuch(int sx, int sy, int dx, int dy) {
    return plansza.wykonajRuch(sx, sy, dx, dy);
}

void Gra::ustawKolejnyGracz(Gracz gracz) {
    aktualnyGracz = gracz;
}

Gracz Gra::getAktualnyGracz() const {
    return aktualnyGracz;
}

Plansza Gra::getPlansza() const {
    return plansza;
}