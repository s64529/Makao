#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <optional>
#include "Talia.h"
#include "Gracz.h"
#include "Karta.h"

class Gra {
private:
    sf::RenderWindow window;
    sf::Texture teksturaTyl;
    sf::Font czcionka;
    sf::Sprite spriteTyl;
    sf::Sprite spriteTalia;
    sf::Text tekstKomunikatu;
    sf::Text tekstImieGracza;
    sf::Text tekstStatystyki;

    Talia taliaGry;
    std::vector<Gracz*> uczestnicy;
    Karta* kartaNaStole;

    std::string aktualnyKomunikat;
    int kartyDoDobrania;
    int aktywnePostoje;
    bool graTrwa;
    int aktualnaTura;
    bool turaZakonczona;

    void inicjalizujZasoby();
    void rozdajKarty();
    void obsluzZdarzenia();
    void obsluzTureBota();
    void sprawdzStanGry();
    void rysujScene();

public:
    Gra();
    ~Gra();
    void uruchom();
};