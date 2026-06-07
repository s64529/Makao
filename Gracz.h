#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Karta.h"
#include "Talia.h"

class Gracz {
protected:
    std::string nazwa;
    std::vector<Karta*> reka;

public:
    Gracz();
    Gracz(std::string imie);
    virtual ~Gracz() = default;
    std::string pobierzNazwe();
    bool czyPustaReka();
    void dobierzKarte(Karta* nowaKarta);
    std::vector<Karta*>& pobierzReke();
    bool czyRuchLegalny(Karta* wybranaKarta, Karta* kartaNaStole, int kartyDoDobrania, int aktywnePostoje);

    virtual bool wykonajTure(Karta*& kartaNaStole, int& kartyDoDobrania, int& aktywnePostoje, Talia& taliaGry, std::string& aktualnyKomunikat, sf::Vector2f pozycjaMyszy, bool kliknietoTalie, bool kliknieto) = 0;
};

class GraczCzlowiek : public Gracz {
public:
    GraczCzlowiek();
    GraczCzlowiek(std::string imie);
    bool wykonajTure(Karta*& kartaNaStole, int& kartyDoDobrania, int& aktywnePostoje, Talia& taliaGry, std::string& aktualnyKomunikat, sf::Vector2f pozycjaMyszy, bool kliknietoTalie, bool kliknieto) override;
};

class GraczBot : public Gracz {
public:
    GraczBot();
    bool wykonajTure(Karta*& kartaNaStole, int& kartyDoDobrania, int& aktywnePostoje, Talia& taliaGry, std::string& aktualnyKomunikat, sf::Vector2f pozycjaMyszy, bool kliknietoTalie, bool kliknieto) override;
};