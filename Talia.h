#pragma once
#include <vector>
#include "Karta.h"

class Talia
{
private:
	std::vector<Karta*> stos;

public:
	Talia();
	~Talia();
	void tasuj();
	Karta* dajKarte();
};

