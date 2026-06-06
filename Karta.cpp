#include "Karta.h"
#include <string>

using namespace std;

int Karta::licznikWyrzuconychKart = 0;

Karta::Karta(kolorKarty k, figuraKarty f) : sprite(tekstura) {
    kolor = k;
    figura = f;
    wczytajTeksture();
}

kolorKarty Karta::pobierzKolor() {
    return kolor;
}

figuraKarty Karta::pobierzFigure() {
    return figura;
}

string Karta::wyswietlKolor() {
    switch (pobierzKolor()) {
    case kolorKarty::Pik: return "Pik";
    case kolorKarty::Kier: return "Kier";
    case kolorKarty::Trefl: return "Trefl";
    case kolorKarty::Karo: return "Karo";
    default: return "Nie ma takiego typu";
    }
}

string Karta::wyswietlFigure() {
    switch (pobierzFigure()) {
    case figuraKarty::Dwojka: return "Dwojka";
    case figuraKarty::Trojka: return "Trojka";
    case figuraKarty::Czworka: return "Czworka";
    case figuraKarty::Piatka: return "Piatka";
    case figuraKarty::Szostka: return "Szostka";
    case figuraKarty::Siodemka: return "Siodemka";
    case figuraKarty::Osemka: return "Osemka";
    case figuraKarty::Dziewiatka: return "Dziewiatka";
    case figuraKarty::Dziesiatka: return "Dziesiatka";
    case figuraKarty::Walet: return "Walet";
    case figuraKarty::Dama: return "Dama";
    case figuraKarty::Krol: return "Krol";
    case figuraKarty::As: return "As";
    default: return "Nieznana figura";
    }
}

void Karta::zagraj(int& kartyDoDobrania, int& aktywnePostoje) {
}

int Karta::pobierzLicznik() {
    return licznikWyrzuconychKart;
}

void Karta::wczytajTeksture() {
    string nazwaPliku = "Karty/" + wyswietlFigure() + "_" + wyswietlKolor() + ".png";
    if (tekstura.loadFromFile(nazwaPliku)) {
        sprite.setTexture(tekstura, true);
        sprite.setScale({ 0.2f, 0.2f });
    }
}

sf::Sprite& Karta::pobierzSprite() {
    return sprite;
}

void Karta::ustawPozycje(float x, float y) {
    sprite.setPosition({x, y});
}

KartaZwykla::KartaZwykla(kolorKarty k, figuraKarty f) : Karta(k, f) {
}

void KartaZwykla::zagraj(int& kartyDoDobrania, int& aktywnePostoje) {
    licznikWyrzuconychKart++;
}

KartaSpecjalna::KartaSpecjalna(kolorKarty k, figuraKarty f) : Karta(k, f) {
}

void KartaSpecjalna::zagraj(int& kartyDoDobrania, int& aktywnePostoje) {
    licznikWyrzuconychKart++;
    switch (pobierzFigure()) {
    case figuraKarty::Dwojka:
        kartyDoDobrania = kartyDoDobrania + 2;
        break;
    case figuraKarty::Trojka:
        kartyDoDobrania = kartyDoDobrania + 3;
        break;
    case figuraKarty::Czworka:
        aktywnePostoje = aktywnePostoje + 1;
        break;
    case figuraKarty::Krol:
        if (pobierzKolor() == kolorKarty::Kier || pobierzKolor() == kolorKarty::Pik) {
            kartyDoDobrania = kartyDoDobrania + 5;
        }
        break;
    default:
        break;
    }
}