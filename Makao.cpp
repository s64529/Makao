#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <vector>
#include <optional>
#include <string>
#include "Talia.h"
#include "Gracz.h"
#include "Karta.h"
#include "Statystyki.h"

using namespace std;

int main()
{
    srand((time(NULL)));

    Talia taliaGry;
    taliaGry.tasuj();

    vector<Gracz*> uczestnicy;
    uczestnicy.push_back(new GraczCzlowiek("Kacper"));
    uczestnicy.push_back(new GraczBot());

    for (int i = 0; i < 5; i++) {
        uczestnicy[0]->dobierzKarte(taliaGry.dajKarte());
        uczestnicy[1]->dobierzKarte(taliaGry.dajKarte());
    }

    Karta* kartaNaStole = taliaGry.dajKarte();

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Makao");

    sf::Texture teksturaTyl;
    sf::Font czcionka;

    try {
        if (!teksturaTyl.loadFromFile("Karty/Tyl.png")) {
            throw "Krytyczny blad: Brak pliku tekstury!";
        }
        if (!czcionka.openFromFile("arial.ttf")) {
            throw "Krytyczny blad: Brak pliku czcionki!";
        }
    }
    catch (const char* wyjatek) {
        return -1;
    }

    sf::Sprite spriteTyl(teksturaTyl);
    spriteTyl.setScale({0.2f, 0.2f});

    sf::Sprite spriteTalia(teksturaTyl);
    spriteTalia.setScale({0.2f, 0.2f});
    spriteTalia.setPosition({250.0f, 260.0f});

    sf::Text tekstKomunikatu(czcionka);
    tekstKomunikatu.setCharacterSize(24);
    tekstKomunikatu.setFillColor(sf::Color::White);
    tekstKomunikatu.setPosition({20.0f, 5.0f});

    sf::Text tekstImieGracza(czcionka);
    tekstImieGracza.setCharacterSize(20);
    tekstImieGracza.setFillColor(sf::Color::White);
    tekstImieGracza.setString("Gracz: " + uczestnicy[0]->pobierzNazwe());
    tekstImieGracza.setPosition({20.0f, 680.0f});

    sf::Text tekstStatystyki(czcionka);
    tekstStatystyki.setCharacterSize(20);
    tekstStatystyki.setFillColor(sf::Color::Yellow);
    tekstStatystyki.setString(Statystyki::pobierzStatystyki());
    tekstStatystyki.setPosition({1000.0f, 5.0f});

    string aktualnyKomunikat = "Gra rozpoczeta! Twoja tura.";

    int kartyDoDobrania = 0;
    int aktywnePostoje = 0;
    bool graTrwa = true;
    int aktualnaTura = 0;

    while (window.isOpen())
    {
        bool turaZakonczona = false;

        while (const optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (graTrwa && aktualnaTura == 0)
            {
                if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouseBtn->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2f pozycjaMyszy(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));
                        bool kliknietoTalie = spriteTalia.getGlobalBounds().contains(pozycjaMyszy);

                        turaZakonczona = uczestnicy[aktualnaTura]->wykonajTure(kartaNaStole, kartyDoDobrania, aktywnePostoje, taliaGry, aktualnyKomunikat, pozycjaMyszy, kliknietoTalie, true);
                    }
                }
            }
        }

        if (graTrwa && aktualnaTura == 1)
        {
            window.clear(sf::Color(34, 139, 34));
            window.draw(spriteTalia);
            kartaNaStole->ustawPozycje(540.0f, 260.0f);
            window.draw(kartaNaStole->pobierzSprite());

            vector<Karta*>& rekaBota = uczestnicy[1]->pobierzReke();
            float przerwaBota = 160.0f;
            if (rekaBota.size() * przerwaBota > 1080.0f) {
                przerwaBota = 1080.0f / rekaBota.size();
            }
            float startXBot = (1280.0f - (rekaBota.size() * przerwaBota)) / 2.0f;
            for (size_t i = 0; i < rekaBota.size(); i++) {
                spriteTyl.setPosition({ startXBot + (i * przerwaBota), 40.0f });
                window.draw(spriteTyl);
            }

            vector<Karta*>& rekaGracza = uczestnicy[0]->pobierzReke();
            float przerwaGracza = 160.0f;
            if (rekaGracza.size() * przerwaGracza > 1080.0f) {
                przerwaGracza = 1080.0f / rekaGracza.size();
            }
            float startX = (1280.0f - (rekaGracza.size() * przerwaGracza)) / 2.0f;
            for (size_t i = 0; i < rekaGracza.size(); i++) {
                rekaGracza[i]->ustawPozycje(startX + (i * przerwaGracza), 500.0f);
                window.draw(rekaGracza[i]->pobierzSprite());
            }

            tekstKomunikatu.setString(aktualnyKomunikat);
            window.draw(tekstKomunikatu);
            window.draw(tekstImieGracza);
            window.draw(tekstStatystyki);
            window.display();

            sf::sleep(sf::seconds(1.5f));

            turaZakonczona = uczestnicy[aktualnaTura]->wykonajTure(kartaNaStole, kartyDoDobrania, aktywnePostoje, taliaGry, aktualnyKomunikat, { 0,0 }, false, true);
        }

        if (turaZakonczona) {
            if (uczestnicy[aktualnaTura]->czyPustaReka()) {
                graTrwa = false;
                string zwyciezca = uczestnicy[aktualnaTura]->pobierzNazwe();
                if (zwyciezca == "Kacper") {
                    aktualnyKomunikat = "GRATULACJE! Wygrywasz gre!";
                    Statystyki::dodajWygrana("Gracz");
                }
                else {
                    aktualnyKomunikat = "BOT WYGRYWA GRE!";
                    Statystyki::dodajWygrana("Bot");
                }
                tekstStatystyki.setString(Statystyki::pobierzStatystyki());
            }
            else {
                aktualnaTura = (aktualnaTura + 1) % 2;
            }
        }

        window.clear(sf::Color(34, 139, 34));

        window.draw(spriteTalia);

        kartaNaStole->ustawPozycje(540.0f, 260.0f);
        window.draw(kartaNaStole->pobierzSprite());

        vector<Karta*>& rekaBota = uczestnicy[1]->pobierzReke();
        float przerwaBota = 160.0f;
        if (rekaBota.size() * przerwaBota > 1080.0f) {
            przerwaBota = 1080.0f / rekaBota.size();
        }
        float startXBot = (1280.0f - (rekaBota.size() * przerwaBota)) / 2.0f;
        for (size_t i = 0; i < rekaBota.size(); i++) {
            spriteTyl.setPosition({ startXBot + (i * przerwaBota), 40.0f });
            window.draw(spriteTyl);
        }

        vector<Karta*>& rekaGracza = uczestnicy[0]->pobierzReke();
        float przerwaGracza = 160.0f;
        if (rekaGracza.size() * przerwaGracza > 1080.0f) {
            przerwaGracza = 1080.0f / rekaGracza.size();
        }
        float startX = (1280.0f - (rekaGracza.size() * przerwaGracza)) / 2.0f;
        for (size_t i = 0; i < rekaGracza.size(); i++) {
            rekaGracza[i]->ustawPozycje(startX + (i * przerwaGracza), 500.0f);
            window.draw(rekaGracza[i]->pobierzSprite());
        }

        tekstKomunikatu.setString(aktualnyKomunikat);
        window.draw(tekstKomunikatu);
        window.draw(tekstImieGracza);
        window.draw(tekstStatystyki);

        window.display();
    }

    for (Gracz* g : uczestnicy) {
        delete g;
    }
    uczestnicy.clear();

    return 0;
}