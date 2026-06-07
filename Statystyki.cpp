#include "Statystyki.h"
#include <fstream>

using namespace std;

void Statystyki::dodajWygrana(string zwyciezca) {
    int wygraneGracza = 0;
    int wygraneBota = 0;

    ifstream plikOdczyt("statystyki.txt");
    if (plikOdczyt.is_open()) {
        plikOdczyt >> wygraneGracza >> wygraneBota;
        plikOdczyt.close();
    }

    if (zwyciezca == "Gracz") {
        wygraneGracza++;
    }
    else {
        wygraneBota++;
    }

    ofstream plikZapis("statystyki.txt");
    if (plikZapis.is_open()) {
        plikZapis << wygraneGracza << " " << wygraneBota;
        plikZapis.close();
    }
}

string Statystyki::pobierzStatystyki() {
    int wygraneGracza = 0;
    int wygraneBota = 0;

    ifstream plik("statystyki.txt");
    if (plik.is_open()) {
        plik >> wygraneGracza >> wygraneBota;
        plik.close();
    }

    return "Wygrane - Ty: " + to_string(wygraneGracza) + " | Bot: " + to_string(wygraneBota);
}