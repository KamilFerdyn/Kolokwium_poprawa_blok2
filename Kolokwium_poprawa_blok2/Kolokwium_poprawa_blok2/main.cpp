#include <iostream>
#include <cstring>
#include <cassert>
#include <exception>

using namespace std;

using Tekst = char[50];

const char tablica[5][30] = {"na maszcie", "na dachu budynku", "w pomieszczeniu", "na_scianie_budynku", "w przestrzeni otwartej"};

enum class MiejscaMontazu
{
    na_maszcie = 0,
    na_dachu_budynku = 1,
    w_pomieszczeniu = 2,
    na_scianie_budynku = 3,
    w_przestrzeni_otwartej = 4,
};

struct Antena
{
    Tekst typ;
    float dlugosc;
    float wzmocnienie;

};

struct Nadajnik {
    Antena* antena;
    MiejscaMontazu miejscaMontazu; //"na maszcie”, "na dachu budynku","na ścianie budynku","w pomieszczeniu","w przestrzeni otwartej"
    float napiecieAkumulatora;
    float mocNadajnika;
    uint16_t licznikKomunikatow;
};

Nadajnik wypelnianie(MiejscaMontazu miejscaMontazu, float napiecieAkumulatora, float mocNadajnika, uint16_t licznikKomunikatow, Antena* antena )
{
    Nadajnik nadajnik;

    nadajnik.miejscaMontazu = miejscaMontazu;
    nadajnik.napiecieAkumulatora = napiecieAkumulatora;
    nadajnik.mocNadajnika = mocNadajnika;
    nadajnik.licznikKomunikatow = licznikKomunikatow;
    nadajnik.antena = antena;

    return nadajnik;
}

void wyswietlanie(Nadajnik nadajnik)
{
    cout << tablica[(int)(nadajnik.miejscaMontazu)] << endl << nadajnik.napiecieAkumulatora << endl;
    cout << nadajnik.mocNadajnika << endl << nadajnik.licznikKomunikatow << endl;
}



// funkcja powinna przyjmować tablicę nadajników i przekazywać przez referencję dane nadajnika,
// którego liczba wysłanych komunikatów jest najbardziej zbliżonna do średniej liczby komunikatów
// wszystkich nadajników
// funkcja powinna zwrócić liczbę komunikatów

int nadajnikSredniKomunikat(Nadajnik *nadajniki, int rozmiartab, Nadajnik& nadajnik)
{
    assert(&nadajnik != nullptr);

    double sr = 0;
    for (int i = 0; i < rozmiartab; i++)
    {
        sr += nadajniki[i].licznikKomunikatow;
    }
    sr = sr / rozmiartab;

    Nadajnik nadajnik2 = nadajniki[0];
    for (int i = 0; i < rozmiartab; i++)
    {
        if (nadajniki[i].antena == nullptr) {
            throw domain_error("nadajnik nie ma przypisanej anteny");
        }

        if (sr > nadajniki[i].licznikKomunikatow)
        {
         
            if (nadajnik2.licznikKomunikatow - sr < 0) {
                if (sr - nadajniki[i].licznikKomunikatow < sr - nadajnik2.licznikKomunikatow)
                {
                    nadajnik2 = nadajniki[i];
                    nadajnik = nadajniki[i];
                }
            }
            else {
                if (sr - nadajniki[i].licznikKomunikatow <  nadajnik2.licznikKomunikatow - sr)
                {
                    nadajnik2 = nadajniki[i];
                    nadajnik = nadajniki[i];
                }
            }
        }

        else {
            if (nadajnik2.licznikKomunikatow - sr < 0) {
                if (nadajniki[i].licznikKomunikatow - sr < sr - nadajnik2.licznikKomunikatow)
                {
                    nadajnik2 = nadajniki[i];
                    nadajnik = nadajniki[i];
                }
            }
            else {
                if (nadajniki[i].licznikKomunikatow - sr < nadajnik2.licznikKomunikatow - sr)
                {
                    nadajnik2 = nadajniki[i];
                    nadajnik = nadajniki[i];
                }
            }
        }
    }

    return nadajnik2.licznikKomunikatow;
}

void testujwyjatek(Nadajnik* nadajniki, int rozmiartab, Nadajnik& nadajnik) {
    int wjtk = 0;

    try {
        nadajnikSredniKomunikat(nadajniki, rozmiartab, nadajnik);
    }
    catch (domain_error) {
        wjtk = 1;
    }

    if (wjtk == 0)
    {
        cout << "FAIL" << endl;
    }
    else {
        cout << "OK" << endl;
    }
}

void testujsrednia(Nadajnik* nadajniki, int rozmiartab, Nadajnik& nadajnik)
{
    if (nadajnikSredniKomunikat(nadajniki, rozmiartab, nadajnik) == 700)
    {
        cout << "OK" << endl;
    }
    else {
        cout << "FAIL" << endl;
    }
}

int main()
{
    Antena anteny[] = {
    {"Pretowa", 592.2, 9},
    {"Panelowa", 553, 4},
    {"Dipol półfalowy", 12.2, 5},
    };

    Nadajnik Nadajniki[] = {
        {&anteny[0],MiejscaMontazu::na_dachu_budynku,3.5,0.05,1000},
        {&anteny[1],MiejscaMontazu::na_maszcie,3.4,0.1,700},
        {&anteny[2],MiejscaMontazu::w_pomieszczeniu,3.6,0.1,800},
        {&anteny[0],MiejscaMontazu::w_przestrzeni_otwartej,3.5,0.08,450},
        {&anteny[1],MiejscaMontazu::na_dachu_budynku,3.4,0.09,600},
    };

    Nadajnik Nadajnikitestwyjatek[] = {
       {nullptr,MiejscaMontazu::na_dachu_budynku,3.5,0.05,1000},
       {&anteny[1],MiejscaMontazu::na_maszcie,3.4,0.1,700},
       {&anteny[2],MiejscaMontazu::w_pomieszczeniu,3.6,0.1,800},
       {&anteny[0],MiejscaMontazu::w_przestrzeni_otwartej,3.5,0.08,450},
       {&anteny[1],MiejscaMontazu::na_dachu_budynku,3.4,0.09,600},
    };

 


    Nadajnik nadajnik;
    cout << nadajnikSredniKomunikat(Nadajniki, 5, nadajnik) << endl;
    wyswietlanie(nadajnik);
    cout << endl;
    testujwyjatek(Nadajnikitestwyjatek, 5, nadajnik);
    testujsrednia(Nadajniki, 5, nadajnik);


    return 0;
}
