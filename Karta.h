#pragma once
#include <string>
#include <SFML/Graphics.hpp>

enum class kolorKarty {
	Pik, Kier, Trefl, Karo
};

enum class figuraKarty {
	Dwojka, Trojka, Czworka, Piatka, Szostka, Siodemka, Osemka,
	Dziewiatka, Dziesiatka, Walet, Dama, Krol, As
};

class Karta
{
protected:
	static int licznikWyrzuconychKart;

private:
	kolorKarty kolor;
	figuraKarty figura;
	sf::Texture tekstura;
	sf::Sprite sprite;

public:
	Karta(kolorKarty kolor, figuraKarty figura);
	virtual void zagraj(int& kartyDoDobrania, int& aktywnePostoje);
	kolorKarty pobierzKolor();
	figuraKarty pobierzFigure();
	std::string wyswietlKolor();
	std::string wyswietlFigure();
	static int pobierzLicznik();

	void wczytajTeksture();
	sf::Sprite& pobierzSprite();
	void ustawPozycje(float x, float y);
};

class KartaZwykla : public Karta {
public:
	KartaZwykla(kolorKarty kolor, figuraKarty figura);
	virtual void zagraj(int& kartyDoDobrania, int& aktywnePostoje) override;
};

class KartaSpecjalna : public Karta {
public:
	KartaSpecjalna(kolorKarty kolor, figuraKarty figura);
	virtual void zagraj(int& kartyDoDobrania, int& aktywnePostoje) override;
};