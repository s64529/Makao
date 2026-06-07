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

bool GraczCzlowiek::wykonajTure(Karta*& kartaNaStole, int& kartyDoDobrania, int& aktywnePostoje, Talia& taliaGry, string& aktualnyKomunikat, sf::Vector2f pozycjaMyszy, bool kliknietoTalie, bool kliknieto) {
    if (!kliknieto) return false;

    if (kliknietoTalie) {
        if (aktywnePostoje > 0) {
            aktywnePostoje--;
            aktualnyKomunikat = "Tracisz ture (postoj). Tura bota.";
        }
        else if (kartyDoDobrania > 0) {
            for (int k = 0; k < kartyDoDobrania; k++) {
                dobierzKarte(taliaGry.dajKarte());
            }
            aktualnyKomunikat = "Pobrales karne karty: " + to_string(kartyDoDobrania) + ". Tura bota.";
            kartyDoDobrania = 0;
        }
        else {
            dobierzKarte(taliaGry.dajKarte());
            aktualnyKomunikat = "Dobrales karte. Tura bota.";
        }
        return true;
    }

    for (int i = static_cast<int>(reka.size()) - 1; i >= 0; i--) {
        if (reka[i]->pobierzSprite().getGlobalBounds().contains(pozycjaMyszy)) {
            Karta* wybranaKarta = reka[i];
            if (czyRuchLegalny(wybranaKarta, kartaNaStole, kartyDoDobrania, aktywnePostoje)) {
                kartaNaStole = wybranaKarta;
                kartaNaStole->zagraj(kartyDoDobrania, aktywnePostoje);
                reka.erase(reka.begin() + i);
                aktualnyKomunikat = "Zagrales karte. Tura bota.";
                return true;
            }
            else {
                if (kartyDoDobrania > 0) {
                    aktualnyKomunikat = "Musisz dobrac karty z talii lub uzyc karty obronnej!";
                }
                else if (aktywnePostoje > 0) {
                    aktualnyKomunikat = "Masz postoj! Rzuc 4 lub kliknij talie by pominac ture.";
                }
                else {
                    aktualnyKomunikat = "Ruch nielegalny! Nie pasuje kolor ani figura.";
                }
            }
            break;
        }
    }
    return false;
}

GraczBot::GraczBot() : Gracz("Bot") {
}

bool GraczBot::wykonajTure(Karta*& kartaNaStole, int& kartyDoDobrania, int& aktywnePostoje, Talia& taliaGry, string& aktualnyKomunikat, sf::Vector2f pozycjaMyszy, bool kliknietoTalie, bool kliknieto) {
    Karta* rzuconaKartaBota = nullptr;

    if (aktywnePostoje > 0) {
        for (size_t i = 0; i < reka.size(); i++) {
            if (reka[i]->pobierzFigure() == figuraKarty::Czworka) {
                rzuconaKartaBota = reka[i];
                reka.erase(reka.begin() + i);
                break;
            }
        }
    }
    else if (kartyDoDobrania > 0) {
        for (size_t i = 0; i < reka.size(); i++) {
            if (czyRuchLegalny(reka[i], kartaNaStole, kartyDoDobrania, aktywnePostoje)) {
                rzuconaKartaBota = reka[i];
                reka.erase(reka.begin() + i);
                break;
            }
        }
    }
    else {
        for (size_t i = 0; i < reka.size(); i++) {
            if (czyRuchLegalny(reka[i], kartaNaStole, kartyDoDobrania, aktywnePostoje)) {
                rzuconaKartaBota = reka[i];
                reka.erase(reka.begin() + i);
                break;
            }
        }
    }

    if (rzuconaKartaBota != nullptr) {
        kartaNaStole = rzuconaKartaBota;
        kartaNaStole->zagraj(kartyDoDobrania, aktywnePostoje);
        aktualnyKomunikat = "Bot zagral karte. Twoja tura.";
    }
    else {
        if (aktywnePostoje > 0) {
            aktywnePostoje--;
            aktualnyKomunikat = "Bot czeka w postoju. Twoja tura.";
        }
        else if (kartyDoDobrania > 0) {
            for (int k = 0; k < kartyDoDobrania; k++) {
                dobierzKarte(taliaGry.dajKarte());
            }
            aktualnyKomunikat = "Bot pobral karne karty: " + to_string(kartyDoDobrania) + ". Twoja tura.";
            kartyDoDobrania = 0;
        }
        else {
            dobierzKarte(taliaGry.dajKarte());
            aktualnyKomunikat = "Bot dobral karte. Twoja tura.";
        }
    }
    return true;
}