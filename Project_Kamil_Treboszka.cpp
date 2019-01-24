/// Created by Kamil Treboszka

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <locale.h>
#include <ctype.h>

using namespace std;

int a = 1; // poczatkowe ID ksiazki
int u;

struct date{
    string dzien;
    string miesiac;
    string rok;
};

struct book{
    int ID;
    string tytul;
    string imie_a;
    string nazwisko_a;
    string kategoria;
    struct date data;
    string imie_wyp;
    string nazwisko_wyp;
    string status;
    string inne_dane;
    struct book *poprzedni;
    struct book *nastepny;
};

struct book *glowa=NULL, *ogon=NULL;

void Dodaj()
{
    string tytul, imie_a, nazwisko_a, kategoria;
    book *nowy;
    try
    {
        nowy = new book;
    }
    catch(bad_alloc)
    {
        cout << "Brak miejsca na nowa ksiazke."<<endl;
        return;
    }

    cout << "ID ksiazki to: "<< a <<endl;
    (nowy->ID=a);
    cin.clear();
    cin.sync();
    cout << "Podaj tytul ksiazki: ";
    getline(cin, tytul);
    cout << "Podaj imie autora: ";
    getline(cin, imie_a);
    cout << "Podaj nazwisko autora: ";
    getline(cin, nazwisko_a);
    cout << "Podaj kategorie ksiazki: ";
    getline(cin, kategoria);

    nowy->tytul = tytul;
    nowy->imie_a = imie_a;
    nowy->nazwisko_a = nazwisko_a;
    nowy->kategoria = kategoria;
    nowy->status = "Dostepna";
    a++;

	nowy->nastepny = NULL;
	if (glowa == NULL) {
		nowy->poprzedni = NULL;
		glowa = nowy;
		ogon = nowy;
		return;
	}
	while (ogon->nastepny != NULL)
    {
		ogon = ogon->nastepny;
    }

	ogon->nastepny = nowy;
	nowy->poprzedni = ogon;
	ogon = nowy;
	return;
 }

 void usuwanie(struct book* usun) {
	if (glowa == NULL || usun == NULL)
		{
		    return;
		}
	if (glowa == usun)
    {
		glowa = usun->nastepny;
    }
	if (ogon == usun)
    {
		ogon = usun->poprzedni;
        a--;
    }
	if (usun->nastepny != NULL)
    {
		usun->nastepny->poprzedni = usun->poprzedni;
    }
	if (usun->poprzedni != NULL)
    {
		usun->poprzedni->nastepny = usun->nastepny;
    }
	delete(usun);
	return;
}

void usunPoIndeksie( int pozycja )
{
if (glowa == NULL || pozycja < 0) {
		cout << endl;
		cout << "Nie mozna usunac!" << endl;
		cout << endl;
		return;
	}

	struct book* biezacy = glowa;
	int count = 0;
	while (biezacy != NULL) {
		if (biezacy->ID == pozycja) {
			count++;
			break;
		}
		biezacy = biezacy->nastepny;
	}
	if (count == 0) {
		cout << endl;
		cout << "Nie ma ksiazki o podanym indeksie. Zostaniesz przeniesiony do MENU!" << endl;
		cout << endl;
		return;
	}
	cout << endl;
	cout << "Usunieto!" << endl;
	cout << endl;
	usuwanie(biezacy);
	if (a == 1)
		return;
}

void usunPoTytule(string tytul) {
	if (glowa == NULL) {
		cout << endl;
		cout << "\nBaza ksiazek jest pusta! Dodaj albo wczytaj dane ksiazek.\n" << endl;
		cout << endl;
		return;
	}
	struct book* biezacy;
	biezacy = glowa;
	while (biezacy) {
		if (!(tytul.compare(biezacy->tytul))) {
			usuwanie(biezacy);
			cout << endl;
			cout << "Usunieto!" << endl;
			cout << endl;
			return;
		}
		biezacy = biezacy->nastepny;
	}
}

void zapisz_do_pliku(string nazwa)
{
    struct book *biezacy;
    biezacy=glowa;
    ofstream plik;
    plik.open( nazwa.c_str(), ios::out);
	plik.clear();

    while (biezacy)
    {
        plik << endl;
        plik << biezacy->ID << endl;
        plik << biezacy->tytul << endl;
        plik << biezacy->imie_a << endl;
        plik << biezacy->nazwisko_a << endl;
        plik << biezacy->kategoria << endl;
        plik << biezacy->data.dzien << endl;
        plik << biezacy->data.miesiac << endl;
        plik << biezacy->data.rok << endl;
        plik << biezacy->imie_wyp << endl;
        plik << biezacy->nazwisko_wyp << endl;
        plik << biezacy->inne_dane << endl;
        plik << biezacy->status;

        biezacy = biezacy->nastepny;
    }
    plik.close();
}

void WszystkieDane(int ID, string tytul, string imie_a, string nazwisko_a, string kategoria,
                    string dzien, string miesiac, string rok,
                    string imie_wyp, string nazwisko_wyp, string status, string inne_dane)
{
    book* nowy = new book;

	nowy->ID = ID;
	nowy->tytul = tytul;
	nowy->imie_a = imie_a;
	nowy->nazwisko_a = nazwisko_a;
	nowy->kategoria = kategoria;
	nowy->status = status;
	nowy->data.dzien = dzien;
	nowy->data.miesiac = miesiac;
	nowy->data.rok = rok;
	nowy->imie_wyp = imie_wyp;
	nowy->nazwisko_wyp = nazwisko_wyp;
	nowy->inne_dane = inne_dane;
	a++;

	nowy->nastepny = NULL;
	if (glowa == NULL)
    {
		nowy->poprzedni = NULL;
		glowa = nowy;
		ogon = nowy;
		return;
	}
	while (ogon->nastepny != NULL)
    {
		ogon = ogon->nastepny;
    }
	ogon->nastepny = nowy;
	nowy->poprzedni = ogon;
	ogon = nowy;
	return;
}

void wczytaj_z_pliku(string nazwa)
{
    struct book* biezacy;
    biezacy = glowa;
    while (biezacy)
    {
        usuwanie(biezacy);
		biezacy = biezacy->nastepny;
	}
    ifstream plik;
    plik.open(nazwa.c_str());
    if (plik.fail())
    {
        cout << "Blad wczytywania danych z pliku!\n Upewnij sie czy plik istnieje!\n"<<endl;
        return;
    }
    while (!(plik.eof()))
{
    int ID;
    string puste, tytul, imie_a, nazwisko_a, kategoria, dzien, miesiac, rok, imie_wyp, nazwisko_wyp, status, inne_dane;

    plik >> ID;
    getline( plik, puste);
    getline( plik, tytul);
    getline( plik, imie_a);
    getline( plik, nazwisko_a);
    getline( plik, kategoria);
    getline( plik, dzien);
    getline( plik, miesiac);
    getline( plik, rok);
    getline( plik, imie_wyp);
    getline( plik, nazwisko_wyp);
    getline( plik, inne_dane);
    getline( plik, status);

    WszystkieDane(ID, tytul, imie_a, nazwisko_a, kategoria, dzien, miesiac, rok, imie_wyp, nazwisko_wyp, status, inne_dane);
}
    cout << "\tWczytano dane z pliku." << endl;
    plik.close();
}

void ss()
{
    book *biezacy;
    biezacy = glowa;
    if(glowa==NULL)
    {
        cout << "Brak ksiazek do pokazania."<<endl;
    }
    else
    {
        while (biezacy)
        {
        cout << biezacy->ID << "\t" << biezacy->tytul << "\t" << biezacy->imie_a << "\t" << biezacy->nazwisko_a << "\t" << biezacy->kategoria << endl;
        biezacy = biezacy->nastepny;
        }
    }
}

void ModyfikacjaPoID(int numer)
{
	if (glowa== NULL)
    {
		cout << "\nBaza ksiazek jest pusta! Dodaj albo wczytaj dane ksiazek.\n" << endl;
		return;
	}

	struct book* biezacy;
	biezacy = glowa;
	string valid, tytul, imie_a, nazwisko_a, kategoria, dzien, miesiac, rok, imie_wyp, nazwisko_wyp, status, inne_dane;
	cout << "Wprowadz  nowe dane, jesli chcesz zostawic stare dane wcisnij ENTER." << endl;
	cout << "Podaj nowe dane." << endl;
	cin.ignore();
	cout << "Tytul: ";
	getline(cin, tytul, '\n');
	cout << "Imie autora: ";
	getline(cin, imie_a, '\n');
	cout << "Nazwisko autora: ";
	getline(cin, nazwisko_a, '\n');
	cout << "Kategoria: ";
	getline(cin, kategoria, '\n');
	cout << "Dodatkowe dane(komentarze): ";
	getline(cin, inne_dane, '\n');
	cout << endl;
	while (biezacy) {
		if (numer == (biezacy->ID))
        {
            if ((tytul.compare(valid)))
            {
                biezacy->tytul = tytul;
            }
            if ((imie_a.compare(valid)))
            {
                biezacy->imie_a = imie_a;
            }
            if ((nazwisko_a.compare(valid)))
            {
                biezacy->nazwisko_a = nazwisko_a;
            }
            if ((kategoria.compare(valid)))
            {
                biezacy->kategoria = kategoria;
            }
            if ((inne_dane.compare(valid)))
            {
                biezacy->inne_dane = inne_dane;
            }
        }
    biezacy = biezacy->nastepny;
	}
	return;
}

void ModyfikacjaPoTytule(string nazwa)
{
	if (glowa== NULL)
    {
		cout << "\nBaza ksiazek jest pusta! Dodaj albo wczytaj dane ksiazek.\n" << endl;
		return;
	}

	struct book* biezacy;
	biezacy = glowa;
	string valid, tytul, imie_a, nazwisko_a, kategoria, dzien, miesiac, rok, imie_wyp, nazwisko_wyp, status, inne_dane;
	cout << "Wprowadz  nowe dane, jesli chcesz zostawic stare dane wcisnij ENTER." << endl;
	cout << "Podaj nowe dane." << endl;
	//cin.clear();
	//cin.ignore();
	cout << "Tytul: ";
	getline(cin, tytul, '\n');
	cout << "Imie autora: ";
	getline(cin, imie_a, '\n');
	cout << "Nazwisko autora: ";
	getline(cin, nazwisko_a, '\n');
	cout << "Kategoria: ";
	getline(cin, kategoria, '\n');
	cout << "Dodatkowe dane(komentarze): ";
	getline(cin, inne_dane, '\n');
	cout << endl;
	while (biezacy) {
		if (!(nazwa.compare(biezacy->tytul)))
        {
            if ((tytul.compare(valid)))
            {
                biezacy->tytul = tytul;
            }
            if ((imie_a.compare(valid)))
            {
                biezacy->imie_a = imie_a;
            }
            if ((nazwisko_a.compare(valid)))
            {
                biezacy->nazwisko_a = nazwisko_a;
            }
            if ((kategoria.compare(valid)))
            {
                biezacy->kategoria = kategoria;
            }
            if ((inne_dane.compare(valid)))
            {
                biezacy->inne_dane = inne_dane;
            }
        }
    biezacy = biezacy->nastepny;
	}
	return;
}

void ZmienStatus (string nazwa, int numer)
{
	string dzien, miesiac, rok, imie_wyp, nazwisko_wyp, status;
	string wolny_status = "Dostepna";   string zajety_status = "Wypozyczona";
	int count = 0;
	char zmiana;
	struct book* biezacy;
	biezacy = glowa;
	while (biezacy)
        {
        if (!(nazwa.compare(biezacy->tytul)) || numer == (biezacy->ID))
            {
			if (!(wolny_status.compare(biezacy->status)))
			{
				count++;
				cout << "\tWprowadz nastepujace dane: \n" << endl;
				od:
				cout << "Dzien wypozyczenia: ";
				cin >> dzien;
				cout << "Miesiac wypozyczenia: ";
				cin >> miesiac;
				cout << "Rok wypozyczenia: ";
				cin >> rok;
				cout << "Imie wypozyczajacego: ";
				cin >> imie_wyp;
				cout << "Nazwisko wypozyczajacego: ";
				cin >> nazwisko_wyp;
				raz:
				cout << "Czy chcesz zmienic status na wypozyczona?\n ( t - tak / n - nie )" << endl;
				cin >> zmiana;
				switch (zmiana)
				{
                    case 't': {
                        biezacy->data.dzien = dzien;
                        biezacy->data.miesiac = miesiac;
                        biezacy->data.rok = rok;
                        biezacy->imie_wyp = imie_wyp;
                        biezacy->nazwisko_wyp = nazwisko_wyp;
                        biezacy->status = "Wypozyczona";

                        cout << "Zmieniono status na WYPOZYCZONA"<< endl;
                        return;
				}
                    case 'n':
                    {
                        cout << "Status ksiazki nie zostal zmieniony." << endl;
                        return;
                    }
                    default:
				    {
                        cout << "\nWybrales zla opcje. Sprobuj jeszcze raz! \n" << endl;
                        goto raz;
                    }
				}
			}
			else if (!(zajety_status.compare(biezacy->status)))
                {
                    char zmiana;
                    count++;
                    cout << endl;
                    cout << "-----------------------------------" << endl;
                    cout << "Ksiazka jest juz wypozyczona!" << endl << endl;
                    cout << "Tytul: " << biezacy->tytul << endl;
                    cout << "Dane autor: " << biezacy->imie_a << " " << biezacy->nazwisko_a << endl;
                    cout << "Dane wypozyczajacy: " << biezacy->imie_wyp << " " << biezacy->nazwisko_wyp << endl;
                    cout << "Data wypozyczenia: " << biezacy->data.dzien << "." << biezacy->data.miesiac << "." << biezacy->data.rok << endl;
                    cout << "-----------------------------------" << endl;
                    ponownie:
                    cout << "Czy chcesz zmienic status na DOSTEPNA?\n Wybierz t-tak lub n-nie: " << endl;
                    cin >> zmiana;
                    switch (zmiana)
                    {
                        case 't':
                        {
                            biezacy->data.dzien = "";
                            biezacy->data.miesiac = "";
                            biezacy->data.rok = "";
                            biezacy->imie_wyp = "";
                            biezacy->nazwisko_wyp = "";
                            biezacy->status = "Dostepna";
                            cout << "Status ksiazki zostal zmieniony na DOSTEPNA" << endl;
                            return;
                        }
                        case 'n':
                        {
                            cout << "Status ksiazki nie zostal zmieniony." << endl;
                            return;
                        }
                        default:
                            cout << "\nWybrales zla opcje! Sproboj jeszcze raz." << endl;
                            goto ponownie;
                    }
			}
			else
                {
                    cout << "\nKsiazka ma zly status! Zmien jej status uzywajac opcji edycji danych ksiazki.\n" << endl;
                    return;
                }
            }
            biezacy = biezacy->nastepny;
        }
        if (count == 0)
        {
            cout << "Ksiazki o podanym tytule lub nazwisku wypozyczajacego nie istnieje!" << endl;
        }
}

void PokazPotytulach()
{
    struct book* biezacy;
	biezacy = glowa;

	if (glowa == NULL)
    {
		cout << "\nBaza ksiazek jest pusta! Dodaj albo wczytaj dane ksiazek.\n" << endl;
		return;
	}

	while (biezacy)
    {
        cout << endl;
        cout << "\tTytul ksiazki: " << biezacy->tytul << endl <<endl;
        cout << "ID ksiazki: " << biezacy->ID << "\t" ;
        cout << "Dane autor: " << biezacy->imie_a << " " << biezacy->nazwisko_a << endl;
        cout << "Kategoria: " << biezacy->kategoria <<endl;
        cout << "Data wypozyczenia: " << biezacy->data.dzien << "." << biezacy->data.miesiac << "." << biezacy->data.rok << endl;
        cout << "Dane wypozyczajacego: " << biezacy->imie_wyp << " " << biezacy->nazwisko_wyp << endl;
        cout << "Status ksiazki: " << biezacy->status << "\t" ;
        cout << "Dodatkowe dane: " << biezacy->inne_dane << endl;
        cout << "____________________________________________________________________" << endl;
        cout << endl;
        biezacy = biezacy->nastepny;
	}
}

void PokazWypozyczone()
{
    int ilosc= 0;
    string zajety_status = "Wypozyczona";
    struct book* biezacy;
	biezacy = glowa;

	if (glowa == NULL)
    {
		cout << "\nBaza ksiazek jest pusta! Dodaj albo wczytaj dane ksiazek.\n" << endl;
		return;
	}

    cout << endl << "\tKsiazki wypozyczone: " << endl;

	while (biezacy)
    {
        if(!(zajety_status.compare(biezacy->status)))
        {
        cout << endl;
        cout << "Tytul ksiazki: " << biezacy->tytul << endl <<endl;
        cout << "ID ksiazki: " << biezacy->ID << "\t" ;
        cout << "Dane autor: " << biezacy->imie_a << " " << biezacy->nazwisko_a << endl ;
        cout << "Kategoria: " << biezacy->kategoria <<endl;
        cout << "Data wypozyczenia: " << biezacy->data.dzien << "." << biezacy->data.miesiac << "." << biezacy->data.rok << endl;
        cout << "Dane wypozyczajacego: " << biezacy->imie_wyp << " " << biezacy->nazwisko_wyp << endl;
        cout << "Status ksiazki: " << biezacy->status <<endl;
        cout << "Dodatkowe dane: " << biezacy->inne_dane << endl;
        cout << "____________________________________________________________________" << endl;
        cout << endl;
        ilosc++;
        }
        biezacy = biezacy->nastepny;
    }

    if(ilosc == 0)
    {
        cout << "Brak ksiazek wypozyczonych." << endl << endl;
        return;
    }
}

void SzukajPoNazwie(string szukaj)
{
	struct book* biezacy;
	biezacy = glowa;
	int count = 0;
	while (biezacy)
    {
		if (!(szukaj.compare(biezacy->tytul)) || !(szukaj.compare(biezacy->nazwisko_wyp)))
        {
			cout << endl;
            cout << "--------------------------------" << endl;
			cout << "ID ksiazki: " << biezacy->ID << endl;
			cout << "Tytul ksiazki: " << biezacy->tytul << endl;
			cout << "Dane autor: " << biezacy->imie_a << " " << biezacy->nazwisko_a << endl;
			cout << "Kategoria: " << biezacy->kategoria << endl;
			cout << "Data wypozyczenia: " << biezacy->data.dzien << "." << biezacy->data.miesiac << "." << biezacy->data.rok << endl;
			cout << "Dane wypozyczajacego: " << biezacy->imie_wyp << " " << biezacy->nazwisko_wyp << endl;
			cout << "Status ksiazki: " << biezacy->status << endl;
			cout << "Dodatkowe dane: " << biezacy->inne_dane << endl;
            cout << "--------------------------------" << endl;
			cout << endl;
			count++;
		}
		biezacy = biezacy->nastepny;
	}

	if (count == 0)
    {
		cout << "\nBrak ksiazki o podanym tytule lub nazwisku wypozyczajacego\n" << endl;
	}

}

void SzukajPoID(int ID)
{
	struct book* biezacy;
	biezacy = glowa;
	int count = 0;
	while (biezacy)
    {
        if (ID == (biezacy->ID))
		{
			cout << endl;
			cout << "--------------------------------" << endl;
			cout << "ID ksiazki: " << biezacy->ID << endl;
			cout << "Tytul ksiazki: " << biezacy->tytul << endl;
			cout << "Dane autor: " << biezacy->imie_a << " " << biezacy->nazwisko_a << endl;
			cout << "Kategoria: " << biezacy->kategoria << endl;
			cout << "Data wypozyczenia: " << biezacy->data.dzien << "." << biezacy->data.miesiac << "." << biezacy->data.rok << endl;
			cout << "Dane wypozyczajacego: " << biezacy->imie_wyp << " " << biezacy->nazwisko_wyp << endl;
			cout << "Status ksiazki: " << biezacy->status << endl;
			cout << "Dodatkowe dane: " << biezacy->inne_dane << endl;
            cout << "--------------------------------" << endl;
			cout << endl;
			count++;
		}
		biezacy = biezacy->nastepny;
	}

	if (count == 0)
    {
		cout << "\nBrak ksiazki o podanym tytule lub nazwisku wypozyczajacego\n" << endl;
	}

}

int main()
{
    char wybor;

    while(1)
    {

    cout << "---------------------------------" << endl;
    cout << "\tMENU:" << endl;
    cout << "---------------------------------" << endl;
    cout << " 1. Dodaj nowa ksiazke." << endl;
    cout << " 2. Usun ksiazke." << endl;
    cout << " 3. Edytuj dane ksiazki." << endl;
    cout << " 4. Zmien status ksiazki." << endl;
    cout << " 5. Wyszukaj ksiazke." << endl;
    cout << " 6. Wyswietl baze po tytulach." << endl;
    cout << " 7. Wyswietl ksiazki wypozyczone." << endl;
    cout << " 8. Zapisz do pliku." << endl;
    cout << " 9. Odczytaj z pliku." << endl;
    cout << " X. Koniec pracy." << endl;
    cout << "---------------------------------" << endl << endl;

    cout << "Wybrales: ";
    cin >> wybor;
    wybor = ( toupper(wybor) );

        switch(wybor)
        {
        case '1':
            {
            dodac:
                Dodaj();
                cout << "Dodano ksiazke!"<< endl << endl;
            popraw:
                char w;
                cout << "Wybierz odpowiednia opcje: " << endl;
                cout << "1) Dodaj kolejna ksiazke." << endl;
                cout << "2) Wroc do MENU." << endl;
                cin >> w;
                if (w == '1')
                {
                    goto dodac;
                }
                else if (w == '2')
                {
                    break;
                }
                else
                {
                    cout << "Wybrales niepoprawna opcje! Sprobuj ponownie." << endl << endl;
                    goto popraw;
                }
                break;
            }
        case '2':
            {
            char d;
			cout << "Jak chcesz usunac ksiazke? Po ID czy tytule?\n  Wybierz (i - ID / t - tytul): ";
			cin >> d;
			if (d == 'i') {
				int id;
				cout << "Wprowadz ID ksiazki ktora chcesz usunac: ";
				cin >> id;
				cout << endl;
				usunPoIndeksie(id);
			}
			else if (d == 't') {
				string tytul;
				cout << "Wprowadz tytul ksiazki ktora chcesz usunac: ";
				cin.ignore();
				getline( cin, tytul );
				cout << endl;
				usunPoTytule(tytul);
			}
			break;
            }
        case '3':
            {
                char wybor;
                string nazwa; int m;
                cout << "Czy chcesz modyfikowac po ID czy po tytule?\nWybierz odpowiednia opcje (i - ID / t - Tytul): " << endl;
                cin >> wybor;
                if(wybor == 'i')
                {
                    cout << "\tModyfikujesz przez wybor ID ksiazki." << endl << endl;
                    cout << "Podaj ID ksiazk, ktora chcesz modyfikowac: " << endl;
                    cin >> m;
                    if (m <= a && m > 0)
                    {
                    ModyfikacjaPoID(m);
                    }
                    else if (m == 0 || m < 0)
                    {
                        cout << "Ksiazka o podanym ID nie istnieje. " << endl;
                    }
                }
                else if (wybor == 't')
                {
                    cout << "\tModyfikujesz przez wybor tytulu ksiazki" << endl << endl;
                    cout << "Podaj tytul ksiazki, ktora chcesz modyfikowac: " << endl;
                    cin.clear();
                    cin.ignore();
                    getline(cin, nazwa);
                    ModyfikacjaPoTytule(nazwa);
                }
                break;
            }
        case '4':
            {
                string zs; int num;
                char w;
                cout << "Wybiesz czy chcesz zmienic status ksiazki po ID czy jej tytule.\n ( i-ID / t-Tytul ): " << endl;
                cin >> w;
                if (w == 'i')
                {
                    cout << "Podaj ID ksiazki, w ktorej chcesz zmienic status: ";
                    cin >> num;
                    ZmienStatus(zs, num);
                }
                else if (w == 't')
                {
                    cout << "Podaj tytul ksiazki, w ktorej chcesz zmienic status: ";
                    cin.clear();
                    cin.ignore();
                    getline(cin, zs);
                    ZmienStatus(zs, num);
                }
                else
                {
                    cout << "Wybrales nie poprawna opcje. Zostaniesz przeniesiony do MENU!\n" << endl;
                }
                break;
            }
        case '5':
            {
                char s;
                int nume;
                string szuk;
                cout << "Wybierz jak chcesz wyszukac ksiazke.\n 1) Po ID\n 2) Po tytule ksiazki\n 3) Po nazwisku wypozyczajacego\n Wybor: ";
                cin >> s;
                if (s == '1')
                {
                    cout << "Wprowadz ID ksiazki ktora chcesz wyszukac: ";
                    cin >> nume;
                    SzukajPoID(nume);
                    cout << "\tNacisnij dowolny klawisz aby kontynuowac" << endl;
                    system("PAUSE>NULL");
                }
                else if (s == '2')
                {
                    cout << "Wprowadz tytul ksiazki, ktora chcesz wyszukac: ";
                    cin.clear();
                    cin.ignore();
                    getline(cin, szuk);
                    SzukajPoNazwie(szuk);
                    cout << "\tNacisnij dowolny klawisz aby kontynuowac" << endl;
                    system("PAUSE>NULL");
                }
                else if (s == '3')
                {
                    cout << "Wprowadz nazwisko wypozyczajacego: ";
                    cin.clear();
                    cin.ignore();
                    getline(cin, szuk);
                    SzukajPoNazwie(szuk);
                    cout << "\tNacisnij dowolny klawisz aby kontynuowas" << endl;
                    system("PAUSE>NULL");
                }
                else
                {
                    cout << "Wybrales nie poprawna opcje! Zostaniesz przeniesiony do MENU!";
                }

                break;
            }
        case '6':
            {
                PokazPotytulach();
                cout << "\tNacisnij dowolny klawisz aby kontynuowac" << endl;
                    system("PAUSE>NULL");
                cout << endl;
                break;
            }
        case '7':
            {
                PokazWypozyczone();
                cout << "\tNacisnij dowolny klawisz aby kontynuowac" << endl;
                    system("PAUSE>NULL");
                cout << endl;
                break;
            }
        case '8':
            {
                string nazwa;
                cout << "Podaj nazwe pliku (nazwa.txt): " << endl;
                cin >> nazwa;
                cout << "\nZapisuje dane..." << endl << endl;
                zapisz_do_pliku(nazwa);
                cout << "\tZapisano dane do pliku." << endl << endl;
                break;
            }
        case '9':
            {
                string nazwa1;
                cout << "Podaj nazwe pliku z ktorego chcesz wczytac dane (nazwa.txt): " << endl;
                cin >> nazwa1;
                cout << "Wczytuje dane..." << endl <<endl;
                wczytaj_z_pliku(nazwa1);
                cout << endl;
                break;
            }
        case 'S':
            {
                ss();
                break;
            }
        case 'X':
            {
                return 0;
            }
        default:
            {
                cout << "Wybrales niepoprawna opcje! Sproboj jeszcze raz!" << endl << endl;
            }
        }
    }
}
