#pragma once
#include <vector>
#include <string>
#include "Karta.h"

class Gracz {
protected:
    std::string nazwa;
    std::vector<Karta*> reka;

public:
    Gracz();
    Gracz(std::string imie);
    std::string pobierzNazwe();
    bool czyPustaReka();
    void dobierzKarte(Karta* nowaKarta);
    std::vector<Karta*>& pobierzReke();
    bool czyRuchLegalny(Karta* wybranaKarta, Karta* kartaNaStole, int kartyDoDobrania, int aktywnePostoje);
};

class GraczCzlowiek : public Gracz {
public:
    GraczCzlowiek();
    GraczCzlowiek(std::string imie);
};

class GraczBot : public Gracz {
public:
    GraczBot();
    Karta* zagrajKarte(Karta* kartaNaStole, int kartyDoDobrania, int aktywnePostoje);
};