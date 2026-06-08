#include "Gra.h"
#include "Statystyki.h"
#include <ctime>

using namespace std;
using namespace sf;

Gra::Gra() :
    window(VideoMode({ 1280, 720 }), "Makao"),
    spriteTyl(teksturaTyl),
    spriteTalia(teksturaTyl),
    tekstKomunikatu(czcionka),
    tekstImieGracza(czcionka),
    tekstStatystyki(czcionka),
    kartyDoDobrania(0),
    aktywnePostoje(0),
    graTrwa(true),
    aktualnaTura(0),
    turaZakonczona(false)
{
    srand((time(NULL)));

    uczestnicy.push_back(new GraczCzlowiek("Kacper"));
    uczestnicy.push_back(new GraczBot());

    inicjalizujZasoby();
    rozdajKarty();

    aktualnyKomunikat = "Gra rozpoczeta! Twoja tura.";
}

Gra::~Gra()
{
    for (Gracz* g : uczestnicy) {
        delete g;
    }
    uczestnicy.clear();
}

void Gra::inicjalizujZasoby()
{
    if (!teksturaTyl.loadFromFile("Karty/Tyl.png")) {
        throw "Krytyczny blad: Brak pliku tekstury!";
    }
    if (!czcionka.openFromFile("arial.ttf")) {
        throw "Krytyczny blad: Brak pliku czcionki!";
    }

    spriteTyl = Sprite(teksturaTyl);
    spriteTyl.setScale({ 0.2f, 0.2f });

    spriteTalia = Sprite(teksturaTyl);
    spriteTalia.setScale({ 0.2f, 0.2f });
    spriteTalia.setPosition({ 250.0f, 260.0f });

    tekstKomunikatu = Text(czcionka);
    tekstKomunikatu.setCharacterSize(24);
    tekstKomunikatu.setFillColor(Color::White);
    tekstKomunikatu.setPosition({ 20.0f, 5.0f });

    tekstImieGracza = Text(czcionka);
    tekstImieGracza.setCharacterSize(20);
    tekstImieGracza.setFillColor(Color::White);
    tekstImieGracza.setString("Gracz: " + uczestnicy[0]->pobierzNazwe());
    tekstImieGracza.setPosition({ 20.0f, 680.0f });

    tekstStatystyki = Text(czcionka);
    tekstStatystyki.setCharacterSize(20);
    tekstStatystyki.setFillColor(Color::Yellow);
    tekstStatystyki.setString(Statystyki::pobierzStatystyki());
    tekstStatystyki.setPosition({ 1000.0f, 5.0f });
}

void Gra::rozdajKarty()
{
    taliaGry.tasuj();
    for (int i = 0; i < 5; i++) {
        uczestnicy[0]->dobierzKarte(taliaGry.dajKarte());
        uczestnicy[1]->dobierzKarte(taliaGry.dajKarte());
    }
    kartaNaStole = taliaGry.dajKarte();
}

void Gra::uruchom()
{
    while (window.isOpen()) {
        turaZakonczona = false;

        obsluzZdarzenia();
        obsluzTureBota();
        sprawdzStanGry();
        rysujScene();
    }
}

void Gra::obsluzZdarzenia()
{
    while (const optional<Event> event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }

        if (graTrwa && aktualnaTura == 0) {
            if (const auto* mouseBtn = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseBtn->button == Mouse::Button::Left) {
                    Vector2f pozycjaMyszy(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));
                    bool kliknietoTalie = spriteTalia.getGlobalBounds().contains(pozycjaMyszy);

                    turaZakonczona = uczestnicy[aktualnaTura]->wykonajTure(kartaNaStole, kartyDoDobrania, aktywnePostoje, taliaGry, aktualnyKomunikat, pozycjaMyszy, kliknietoTalie, true);
                }
            }
        }
    }
}

void Gra::obsluzTureBota()
{
    if (graTrwa && aktualnaTura == 1) {
        rysujScene();
        sleep(seconds(1.5f));
        turaZakonczona = uczestnicy[aktualnaTura]->wykonajTure(kartaNaStole, kartyDoDobrania, aktywnePostoje, taliaGry, aktualnyKomunikat, { 0,0 }, false, true);
    }
}

void Gra::sprawdzStanGry()
{
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
}

void Gra::rysujScene(){
    window.clear(Color(34, 139, 34));

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