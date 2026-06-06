#include "Talia.h"
#include <ctime>

Talia::Talia() {
	for (int k = 0; k < 4; k++) {
		for (int f = 0; f < 13; f++) {
			kolorKarty aktualnyKolor = static_cast<kolorKarty>(k);
			figuraKarty aktualnaFigura = static_cast<figuraKarty>(f);

			if (aktualnaFigura == figuraKarty::Piatka || aktualnaFigura == figuraKarty::Szostka || aktualnaFigura == figuraKarty::Siodemka ||
				aktualnaFigura == figuraKarty::Osemka || aktualnaFigura == figuraKarty::Dziewiatka || aktualnaFigura == figuraKarty::Dziesiatka) {
				stos.push_back(new KartaZwykla(aktualnyKolor, aktualnaFigura));
			}
			else {
				stos.push_back(new KartaSpecjalna(aktualnyKolor, aktualnaFigura));
			}
		}
	}
}

Talia::~Talia() {
	for (int i = 0; i < stos.size(); i++) {
		delete stos[i];
	}
	stos.clear();
}

void Talia::tasuj() {
	srand(time(NULL));

	for (int i = 0; i < stos.size(); i++) {
		int los = rand() % stos.size();
		std::swap(stos[i], stos[los]);
	}
}

Karta* Talia::dajKarte() {
	if (stos.empty()) {
		return nullptr;
	}

	Karta* pobranaKarta = stos.back();
	stos.pop_back();
	return pobranaKarta;
}