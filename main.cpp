#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include "Gra.h"

const int TILE_SIZE = 80;

void drawBoard(sf::RenderWindow& window, const Plansza& plansza) {
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    sf::CircleShape piece(TILE_SIZE / 2 - 5);

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            tile.setFillColor((i + j) % 2 == 0 ? sf::Color::White : sf::Color(192, 192, 192));
            window.draw(tile);

            Pionek pionek = plansza.getPionek(i, j);
            if (pionek.gracz != Gracz::BRAK) {
                piece.setPosition(j * TILE_SIZE + 5, i * TILE_SIZE + 5);
                if (pionek.damka) {
                    piece.setFillColor(pionek.kolor); // Użycie koloru damki
                } else {
                    piece.setFillColor(pionek.gracz == Gracz::BIALE ? sf::Color::White : sf::Color::Black);
                }

                if (pionek.damka) {
                    piece.setOutlineThickness(5);
                    piece.setOutlineColor(sf::Color::Yellow); // Kolor obramowania damki
                } else {
                    piece.setOutlineThickness(0);
                }

                window.draw(piece);
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SIZE * TILE_SIZE, SIZE * TILE_SIZE), "Warcaby Rosyjskie");
    Gra gra;
    Plansza plansza = gra.getPlansza();

    sf::Vector2i selectedStart(-1, -1);  // Pozycja startowa
    sf::Vector2i selectedEnd(-1, -1);    // Pozycja końcowa
    bool isPieceSelected = false;        // Flaga wskazująca, czy pionek jest wybrany

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (gra.getAktualnyGracz() == Gracz::BIALE) {
                        int x = event.mouseButton.x / TILE_SIZE;
                        int y = event.mouseButton.y / TILE_SIZE;

                        if (!isPieceSelected) {
                            Pionek pionek = plansza.getPionek(y, x);
                            if (pionek.gracz == Gracz::BIALE) {
                                selectedStart = sf::Vector2i(x, y);
                                isPieceSelected = true;
                            }
                        } else {
                            selectedEnd = sf::Vector2i(x, y);
                            if (gra.wykonajRuch(selectedStart.y, selectedStart.x, selectedEnd.y, selectedEnd.x)) {
                                isPieceSelected = false;
                                gra.ustawKolejnyGracz(Gracz::CZARNE);
                                plansza = gra.getPlansza();
                                window.clear();
                                drawBoard(window, plansza);
                                window.display();
                            } else {
                                isPieceSelected = false;
                            }
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                window.close();
            }
        }

        if (gra.getAktualnyGracz() == Gracz::CZARNE) {
            gra.ruchKomputera();
            //std::this_thread::sleep_for(std::chrono::seconds(1));
            gra.ustawKolejnyGracz(Gracz::BIALE);
        }

        plansza = gra.getPlansza(); // Aktualizacja planszy po ruchu użytkownika lub komputera

        window.clear();
        drawBoard(window, plansza);
        window.display();
    }

    return 0;
}
