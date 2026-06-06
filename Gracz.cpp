#include "Gracz.h"

using namespace std;

Gracz::Gracz() {
    nazwa = "...";
}

Gracz::Gracz(string imie) {
    nazwa = imie;
}

string Gracz::pobierzNazwe() {
    return nazwa;
}

bool Gracz::czyPustaReka() {
    return reka.empty();
}

void Gracz::dobierzKarte(Karta* nowaKarta) {
    reka.push_back(nowaKarta);
}

vector<Karta*>& Gracz::pobierzReke() {
    return reka;
}

bool Gracz::czyRuchLegalny(Karta* wybranaKarta, Karta* kartaNaStole, int kartyDoDobrania, int aktywnePostoje) {
    if (kartyDoDobrania > 0) {
        bool obrona = (wybranaKarta->pobierzFigure() == figuraKarty::Dwojka ||
            wybranaKarta->pobierzFigure() == figuraKarty::Trojka ||
            (wybranaKarta->pobierzFigure() == figuraKarty::Krol && (wybranaKarta->pobierzKolor() == kolorKarty::Kier || wybranaKarta->pobierzKolor() == kolorKarty::Pik)));
        if (obrona && (wybranaKarta->pobierzKolor() == kartaNaStole->pobierzKolor() || wybranaKarta->pobierzFigure() == kartaNaStole->pobierzFigure())) {
            return true;
        }
        return false;
    }
    else if (aktywnePostoje > 0) {
        if (wybranaKarta->pobierzFigure() == figuraKarty::Czworka) {
            return true;
        }
        return false;
    }
    else {
        if (wybranaKarta->pobierzKolor() == kartaNaStole->pobierzKolor() ||
            wybranaKarta->pobierzFigure() == kartaNaStole->pobierzFigure() ||
            wybranaKarta->pobierzFigure() == figuraKarty::Dama ||
            kartaNaStole->pobierzFigure() == figuraKarty::Dama) {
            return true;
        }
        return false;
    }
}

GraczCzlowiek::GraczCzlowiek() : Gracz() {
}

GraczCzlowiek::GraczCzlowiek(string imie) : Gracz(imie) {
}

GraczBot::GraczBot() : Gracz("Bot") {
}

Karta* GraczBot::zagrajKarte(Karta* kartaNaStole, int kartyDoDobrania, int aktywnePostoje) {
    if (aktywnePostoje > 0) {
        for (size_t i = 0; i < reka.size(); i++) {
            if (reka[i]->pobierzFigure() == figuraKarty::Czworka) {
                Karta* wybrana = reka[i];
                reka.erase(reka.begin() + i);
                return wybrana;
            }
        }
    }
    else if (kartyDoDobrania > 0) {
        for (size_t i = 0; i < reka.size(); i++) {
            if (czyRuchLegalny(reka[i], kartaNaStole, kartyDoDobrania, aktywnePostoje)) {
                Karta* wybrana = reka[i];
                reka.erase(reka.begin() + i);
                return wybrana;
            }
        }
    }
    else {
        for (size_t i = 0; i < reka.size(); i++) {
            if (czyRuchLegalny(reka[i], kartaNaStole, kartyDoDobrania, aktywnePostoje)) {
                Karta* wybrana = reka[i];
                reka.erase(reka.begin() + i);
                return wybrana;
            }
        }
    }
    return nullptr;
}