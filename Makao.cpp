#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <vector>
#include <optional>
#include <string>
#include "Talia.h"
#include "Gracz.h"
#include "Karta.h"

using namespace std;

int main()
{
    srand((time(NULL)));

    Talia taliaGry;
    taliaGry.tasuj();

    GraczCzlowiek gracz("Kacper");
    GraczBot bot;

    for (int i = 0; i < 5; i++) {
        gracz.dobierzKarte(taliaGry.dajKarte());
        bot.dobierzKarte(taliaGry.dajKarte());
    }

    Karta* kartaNaStole = taliaGry.dajKarte();

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Makao");

    sf::Texture teksturaTyl;
    if (!teksturaTyl.loadFromFile("Karty/Tyl.png")) {
        return -1;
    }
    sf::Sprite spriteTyl(teksturaTyl);
    spriteTyl.setScale({0.2f, 0.2f});

    sf::Sprite spriteTalia(teksturaTyl);
    spriteTalia.setScale({0.2f, 0.2f});
    spriteTalia.setPosition({250.0f, 260.0f});

    sf::Font czcionka;
    if (!czcionka.openFromFile("arial.ttf")) {
        return -1;
    }

    sf::Text tekstKomunikatu(czcionka);
    tekstKomunikatu.setCharacterSize(24);
    tekstKomunikatu.setFillColor(sf::Color::White);
    tekstKomunikatu.setPosition({20.0f, 5.0f});

    sf::Text tekstImieGracza(czcionka);
    tekstImieGracza.setCharacterSize(20);
    tekstImieGracza.setFillColor(sf::Color::White);
    tekstImieGracza.setString("Gracz: " + gracz.pobierzNazwe());
    tekstImieGracza.setPosition({20.0f, 680.0f});

    string aktualnyKomunikat = "Gra rozpoczeta! Twoja tura.";

    bool turaGracza = true;
    int kartyDoDobrania = 0;
    int aktywnePostoje = 0;
    bool graTrwa = true;

    while (window.isOpen())
    {
        while (const optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (graTrwa && turaGracza)
            {
                if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouseBtn->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2f pozycjaMyszy(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));

                        if (spriteTalia.getGlobalBounds().contains(pozycjaMyszy))
                        {
                            if (aktywnePostoje > 0) {
                                aktywnePostoje--;
                                aktualnyKomunikat = "Tracisz ture (postoj). Tura bota.";
                            }
                            else if (kartyDoDobrania > 0) {
                                for (int k = 0; k < kartyDoDobrania; k++) {
                                    gracz.dobierzKarte(taliaGry.dajKarte());
                                }
                                aktualnyKomunikat = "Pobrales karne karty: " + to_string(kartyDoDobrania) + ". Tura bota.";
                                kartyDoDobrania = 0;
                            }
                            else {
                                gracz.dobierzKarte(taliaGry.dajKarte());
                                aktualnyKomunikat = "Dobrales karte. Tura bota.";
                            }
                            turaGracza = false;
                        }

                        vector<Karta*>& reka = gracz.pobierzReke();
                        for (int i = static_cast<int>(reka.size()) - 1; i >= 0; i--)
                        {
                            if (reka[i]->pobierzSprite().getGlobalBounds().contains(pozycjaMyszy))
                            {
                                Karta* wybranaKarta = reka[i];

                                if (gracz.czyRuchLegalny(wybranaKarta, kartaNaStole, kartyDoDobrania, aktywnePostoje)) {
                                    kartaNaStole = wybranaKarta;
                                    kartaNaStole->zagraj(kartyDoDobrania, aktywnePostoje);
                                    reka.erase(reka.begin() + i);

                                    if (reka.empty()) {
                                        aktualnyKomunikat = "GRATULACJE! Wygrywasz gre!";
                                        graTrwa = false;
                                    }
                                    else {
                                        aktualnyKomunikat = "Zagrales karte. Tura bota.";
                                        turaGracza = false;
                                    }
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
                    }
                }
            }
        }

        if (graTrwa && !turaGracza)
        {
            window.clear(sf::Color(34, 139, 34));
            window.draw(spriteTalia);
            kartaNaStole->ustawPozycje(540.0f, 260.0f);
            window.draw(kartaNaStole->pobierzSprite());

            vector<Karta*>& rekaBota = bot.pobierzReke();
            float przerwaBota = 160.0f;
            if (rekaBota.size() * przerwaBota > 1080.0f) {
                przerwaBota = 1080.0f / rekaBota.size();
            }
            float startXBot = (1280.0f - (rekaBota.size() * przerwaBota)) / 2.0f;
            for (size_t i = 0; i < rekaBota.size(); i++) {
                spriteTyl.setPosition({startXBot + (i * przerwaBota), 40.0f});
                window.draw(spriteTyl);
            }

            vector<Karta*>& rekaGracza = gracz.pobierzReke();
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

            window.display();

            sf::sleep(sf::seconds(1.5f));

            Karta* rzuconaKartaBota = bot.zagrajKarte(kartaNaStole, kartyDoDobrania, aktywnePostoje);

            if (rzuconaKartaBota != nullptr) {
                kartaNaStole = rzuconaKartaBota;
                kartaNaStole->zagraj(kartyDoDobrania, aktywnePostoje);
                aktualnyKomunikat = "Bot zagral karte. Twoja tura.";

                if (bot.pobierzReke().empty()) {
                    aktualnyKomunikat = "BOT WYGRYWA GRE!";
                    graTrwa = false;
                }
            }
            else {
                if (aktywnePostoje > 0) {
                    aktywnePostoje--;
                    aktualnyKomunikat = "Bot czeka w postoju. Twoja tura.";
                }
                else if (kartyDoDobrania > 0) {
                    for (int k = 0; k < kartyDoDobrania; k++) {
                        bot.dobierzKarte(taliaGry.dajKarte());
                    }
                    aktualnyKomunikat = "Bot pobral karne karty: " + to_string(kartyDoDobrania) + ". Twoja tura.";
                    kartyDoDobrania = 0;
                }
                else {
                    bot.dobierzKarte(taliaGry.dajKarte());
                    aktualnyKomunikat = "Bot dobral karte. Twoja tura.";
                }
            }

            turaGracza = true;
        }

        window.clear(sf::Color(34, 139, 34));

        window.draw(spriteTalia);

        kartaNaStole->ustawPozycje(540.0f, 260.0f);
        window.draw(kartaNaStole->pobierzSprite());

        vector<Karta*>& rekaBota = bot.pobierzReke();
        float przerwaBota = 160.0f;
        if (rekaBota.size() * przerwaBota > 1080.0f) {
            przerwaBota = 1080.0f / rekaBota.size();
        }
        float startXBot = (1280.0f - (rekaBota.size() * przerwaBota)) / 2.0f;
        for (size_t i = 0; i < rekaBota.size(); i++) {
            spriteTyl.setPosition({startXBot + (i * przerwaBota), 40.0f});
            window.draw(spriteTyl);
        }

        vector<Karta*>& rekaGracza = gracz.pobierzReke();
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

        window.display();
    }

    return 0;
}