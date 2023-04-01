/*
Projekt symuluje "Gre w Zycie".
Uzytkownik jest proszony o wpisanie danych poczatkowych w formacie:
/numer wiersza nr kolumny, nr kolumny... /n
,gdzie wspó³rzêdne: [nr wiersza, nr kolumny] odpowiadaj¹ "¿ywym" komórkom
Nastepnie moze generowac kolejne generacje.
Problem rozwiazany jest przy pomocy struktury: lista list

Michal Zmyslony
03.01.2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

//Inicjalizacja stalej WIERSZE na 22, moze byc zmieniona przy kompilacji
#ifndef WIERSZE
# define WIERSZE 22
#endif

//Inicjalizacja stalej KOLUMNY na 80, moze byc zmieniona przy kompilacji
#ifndef KOLUMNY
# define KOLUMNY 80
#endif

/*Tworzy strukture Lista_new. Zawiera ona komorke, ktora posiada pare wartosci:
w - numer wiersza
k - numer kolumny
stan - 1 oznacza zywa komorke a 0 niezywa
liczba sasiadow - ilosc zywych sasiadow
nast_w,nast_k,poprz_k,poprz_w - wskazniki na najblizsze komorki w strukturze
*/
struct Lista {
    int w;
    int k;
    int stan;
    int liczba_sasiadow;
    struct Lista* nast_w;
    struct Lista* nast_k;
    struct Lista* poprz_k;
    struct Lista* poprz_w;
};
typedef struct Lista Lista_new;


//funkcja zwalnia pamiec zaalokowana na liste list l
void zwolnij_liste(Lista_new *l) {
    while(l != NULL) {
        Lista_new *pom = l->nast_w;
        while(l != NULL) {
            Lista_new *pom_2 = l->nast_k;
            free(l);
            l = NULL;
            l = pom_2;
        }
        l = pom;
    }
}

//funkcja czyta input, wpisuje go dynamicznie do tablicy A, oraz przekazuje dlugosc inputu jako n
void czytaj_input(int **A, int *n) {
    int *plansza = NULL;
    int c;
    int i;
    int rozmiar = 0;
    for(i = 0; ((c = getchar()) != EOF); i++) {
        if(i == rozmiar) {
            rozmiar = rozmiar*2 + 2;
            plansza = realloc(plansza, (size_t)rozmiar*sizeof(*plansza));
        }
        if(c >= '0' && c <= '9') {
            ungetc(c,stdin);
            scanf("%d", &c);
        }
        else if(c == '-') {
            ungetc(c,stdin);
            scanf("%d",&c);
        }
        else {
            c = (-1)*c;
        }
        plansza[i] = c;
    }
    *A = plansza;
    *n = i;
}


/*funkcja tworzy nowy element(komorke), zapelnia ja wartosciami w,k,stan.
Nastepnie nowy element jest podpiety do struktury pomiedzy poprz a nast w kolumnie
oraz pomiedzy nast_wiersz i poprz_wiersz w wierszu
wartosc komorki - liczba sasiadow jest inicjalizowana na 0
*/
void Dodaj_pomiedzy_kolumnami(Lista_new *poprz, Lista_new *nast, int w, int k, int stan, Lista_new *nast_wiersz, Lista_new *poprz_wiersz){
    Lista_new *akt = malloc(sizeof(Lista_new));
    akt->nast_k = NULL;
    akt->nast_w = NULL;
    akt->poprz_k = NULL;
    akt->poprz_w = NULL;
    akt->liczba_sasiadow = 0;
    if(poprz != NULL) {
        poprz->nast_k = akt;
    }
    akt->poprz_k = poprz;
    akt->nast_k = nast;
    akt->nast_w = nast_wiersz;
    akt->poprz_w = poprz_wiersz;
    akt->k = k;
    akt->w = w;
    akt->stan = stan;
    if(nast != NULL) {
        nast->poprz_k = akt;
    }
    if(nast_wiersz != NULL) {
        nast_wiersz->poprz_w = akt;
    }
    if(poprz_wiersz != NULL) {
        poprz_wiersz->nast_w = akt;
    }
}

/*
funkcja przyjmuje jako argumenty dwa wskazniki.
akt - wskakuje element zywy, aktualnie brany pod uwage
poprz - wskaznik na jakas komorke (zywa, lub nie) w wierszu ponizej lub powyzej "akt"
funkcja sprawdza czy istnieje 3 sasiadow komorki akt w wierszu ponizej lub powyzej,
jezeli nie to ich dodaje.
Zwraca wskaznik na komorke w wierszu nizej lub wyzej o wartosci w - akt->w -+ 1 oraz k - akt->k
*/
Lista_new * wiersz_sasiadow(Lista_new *akt, Lista_new *poprz) {
    while(poprz->nast_k != NULL && poprz->nast_k->k <= akt->k - 1) {
        poprz = poprz->nast_k;
    }
    if(poprz->k != akt->k - 1) {
        Dodaj_pomiedzy_kolumnami(poprz,poprz->nast_k,poprz->w,akt->k - 1,0,NULL,NULL);
        poprz = poprz->nast_k;
    }
    if(poprz->nast_k == NULL || poprz->nast_k->k > akt->k) {
        Dodaj_pomiedzy_kolumnami(poprz,poprz->nast_k,poprz->w,akt->k,0,NULL,NULL);
    }
    poprz = poprz->nast_k;
    if(poprz->nast_k == NULL || poprz->nast_k->k > akt->k + 1) {
        Dodaj_pomiedzy_kolumnami(poprz,poprz->nast_k,poprz->w,akt->k+1,0,NULL,NULL);
    }
    return poprz;
}

/*
funkcja przyjmuje wskaznik na poczatek listy list komorek - l.
Nastepnie dla kazdego elementu listy dodaje sasiadow kazdej zywej komorce(jezeli nie istnieli wczesniej).
Kazdy nowy dodany jest inicjanowany jako niezywy.
Funkcja jest wywolywana rekurencyjnie - przeglada kazdy wiersz listy osobno.
Jezeli w pierwszym wierszu listy istnieja jakies zywe komorki, to nowym poczatkiem listy bedzie l->poprz_w
*/

void Dodaj_niezywych_sasiadow_v2(Lista_new *l) {
    if(l != NULL) {
        Lista_new *akt = l;
        while(akt != NULL && akt->stan != 1) {
            akt = akt->nast_k;
        }
        if(akt != NULL) {
            Lista_new *poprz = l->poprz_w;
            Lista_new *nast = l->nast_w;
            if(poprz == NULL || poprz->w != akt->w - 1) {
                Dodaj_pomiedzy_kolumnami(NULL,NULL,akt->w - 1,akt->k - 1,0,l,poprz);
                poprz = l->poprz_w;
            }
            if(nast == NULL || nast->w != akt->w + 1) {
                Dodaj_pomiedzy_kolumnami(NULL,NULL,akt->w+1,akt->k-1,0,nast,l);
                nast = l->nast_w;
            }
            if(poprz->k > akt->k - 1) {
                if(poprz != NULL) {
                    Dodaj_pomiedzy_kolumnami(NULL,poprz,poprz->w,akt->k-1,0,l,poprz->poprz_w);
                    poprz = poprz->poprz_k;
                }
            }
            if(nast->k > akt->k-1) {
                if(nast != NULL) {
                    Dodaj_pomiedzy_kolumnami(NULL,nast,nast->w,akt->k-1,0,nast->nast_w,l);
                    nast = nast->poprz_k;
                }
            }
            while(akt != NULL) {
                if(akt->stan == 1) {
                    poprz = wiersz_sasiadow(akt,poprz);
                    if(akt->poprz_k == NULL) {
                        Dodaj_pomiedzy_kolumnami(akt->poprz_k,akt,akt->w,akt->k-1,0,NULL,NULL);
                        akt->poprz_k->nast_w = akt->nast_w;
                        akt->poprz_k->poprz_w = akt->poprz_w;
                        if(akt->poprz_w != NULL) {
                            akt->poprz_w->nast_w = akt->poprz_k;
                        }
                        if(akt->nast_w != NULL) {
                            akt->nast_w->poprz_w = akt->poprz_k;
                        }
                        akt->nast_w = NULL;
                        akt->poprz_w = NULL;
                    }
                    else if(akt->poprz_k->k < akt->k-1) {
                        Dodaj_pomiedzy_kolumnami(akt->poprz_k,akt,akt->w,akt->k-1,0,NULL,NULL);
                    }
                    if(akt->nast_k == NULL || akt->nast_k->k > akt->k+1) {
                        Dodaj_pomiedzy_kolumnami(akt,akt->nast_k,akt->w,akt->k+1,0,NULL,NULL);
                    }
                    nast = wiersz_sasiadow(akt,nast);
                }
                akt = akt->nast_k;
            }
        }
        while(l->poprz_k != NULL) {
            l = l->poprz_k;
        }
        Dodaj_niezywych_sasiadow_v2(l->nast_w);
    }
}

/*
Funkcja przyjmuje tablice input - dane poczatkowe wpisane przez uzytkownika oraz
i - poczatkowo i == 0. Po skonczeniu funkcji i bedzie wskazywac na miejsce w tablicy input,
gdzie zaczynaja sie decyzje uzytkownika o ruchach.
Funkcja tworzy liste list, ktora zawiera posortowane rosnaca(wzgledem wierszy oraz kolumn) komorki zywe, ktore
sa wartosciami startowymi podanymi jako input.
Zwraca poczatek listy.
*/

Lista_new * Wpisz_Plansze_poczatkowa_v2(int *input, int *j) {
    Lista_new atrapa;
    Lista_new *poprz_w = NULL;
    Lista_new *akt_w = &atrapa;
    Lista_new *poprz_k = NULL;
    Lista_new *akt_k = NULL;
    int pierwszy = 1;
    int w = 0;
    int i = *j;
    int k;
    while(!(input[i] == -1*'/' && input[i+1] == -1*'\n')){
        if(input[i] == -1*'/') {
            i++;
            poprz_w = akt_w;
            w = input[i];
            pierwszy = 1;
        }
        else if(input[i] != -1*' ' && input[i] != -1*'\n') {
            k = input[i];
            akt_k = malloc(sizeof(Lista_new));
            akt_k->nast_k = NULL;
            akt_k->nast_w = NULL;
            akt_k->poprz_k = NULL;
            akt_k->poprz_w = NULL;
            if(pierwszy) {
                poprz_k = NULL;
                poprz_w->nast_w = akt_k;
                akt_w = akt_k;
                akt_w->poprz_w = poprz_w;

            }
            else {
                poprz_k->nast_k = akt_k;
            }
            if(!pierwszy) {
                akt_k->nast_w = NULL;
                akt_k->poprz_w = NULL;
            }
            if(pierwszy) {
                pierwszy = 0;
            }
            akt_k->k = k;
            akt_k->w = w;
            akt_k->stan = 1;
            akt_k->nast_k = NULL;
            akt_k->poprz_k = poprz_k;
            poprz_k = akt_k;
        }
        i++;
    }
    akt_w->nast_w = NULL;
    atrapa.nast_w->poprz_w = NULL;
    *j = i + 2;
    return atrapa.nast_w;
}

/*
funkcja wypisuje liste list w startowym formacie, czyli
zawierajaca tylko zywe komorki oraz wygladajaca:
/w1 k1 ... kn
/w2
.
.
/wn k1 ... kn
*/
void Wypisz_startowy_format(Lista_new *l) {
    while(l != NULL) {
        Lista_new *pom = l;
        int pierwszy = 1;
        while(pom != NULL) {
            if(pom->stan == 1) {
                if(pierwszy) {
                    pierwszy = 0;
                    printf("/%d %d", pom->w,pom->k);
                }
                else {
                    printf(" %d", pom->k);
                }
            }
            pom = pom->nast_k;
        }
        if(!pierwszy) {
            printf("\n");
        }
        l = l->nast_w;
    }
    printf("/\n");
}

/*
funkcja wypisuje czesc generacji, w formacie prostokata, z poczatkiem w wierszu - w oraz kolumnie - k.
Wymiary okna to stala KOLUMNY oraz WIERSZE.
Komorka zywa jest wypisywana jako '0' a niezywa jako '.'.
*/
void wypisz_okno(Lista_new *l, int w, int k, int akt_w) {
    int max_w = w + WIERSZE - 1;
    int max_k = k + KOLUMNY - 1;
    Lista_new *pom = NULL;
    int akt_k = k;
    if(akt_w <= max_w){
        if(l != NULL) {
            if(l->w == akt_w) {
                pom = l;
                while(pom != NULL && akt_k <= max_k) {
                    if(pom->k >= k) {
                        if(pom->k == akt_k) {
                            if(pom->stan == 1) {
                                printf("0");
                            }
                            else{
                                printf(".");
                            }
                            pom = pom->nast_k;
                        }
                        else{
                            printf(".");
                        }
                        akt_k++;
                    }
                    else {
                        pom = pom->nast_k;
                    }
                }
                if(pom == NULL) {
                    while(akt_k <= max_k) {
                        printf(".");
                        akt_k++;
                    }
                }
                printf("\n");
                wypisz_okno(l->nast_w,w,k,akt_w+1);
            }
            else if(l->w > akt_w){
                for(int i = k; i <= max_k; i++) {
                    printf(".");
                }
                printf("\n");
                wypisz_okno(l,w,k,akt_w+1);
            }
            else {
                wypisz_okno(l->nast_w,w,k,akt_w);
            }

        }
        else{
            for(int i = k; i <= max_k; i++) {
            printf(".");
            }
            printf("\n");
            wypisz_okno(l,w,k,akt_w+1);
        }
    }
    else {
        for(int i = k; i <= max_k; i++) {
        printf("=");
        }
        printf("\n");
    }
}

/*
funkcja dla danej komorki akt wpisuje ilosc zywych sasiadow w wierszu poprz - wiersz nizej lub wyzej
*/
void sprawdzenie_sasiadow_wyzej_nizej(Lista_new *akt, Lista_new *poprz) {
    while(poprz != NULL && poprz->k < akt->k - 1) {
        poprz = poprz->nast_k;
    }
    if(poprz != NULL) {
                if(poprz->k >= akt->k - 1 && poprz->k <= akt->k + 1) {
                    if(poprz->stan == 1) {
                        akt->liczba_sasiadow++;
                    }
                }
                if(poprz->nast_k != NULL) {
                    if(poprz->nast_k->k >= akt->k - 1 && poprz->nast_k->k <= akt->k + 1) {
                        if(poprz->nast_k->stan == 1) {
                            akt->liczba_sasiadow++;
                        }
                    }
                    if(poprz->nast_k->nast_k != NULL) {
                        if(poprz->nast_k->nast_k->k >= akt->k - 1 && poprz->nast_k->nast_k->k <= akt->k + 1) {
                            if(poprz->nast_k->nast_k->stan == 1) {
                                akt->liczba_sasiadow++;
                            }
                        }
                    }
                }

            }
}

/*
Funckja dla kazdej komorki listy list - l, wpisuje liczbe jej zywych sasiadow.
*/
void Uzupelnij_liczbe_sasiadow(Lista_new *l) {
    if(l != NULL) {
        Lista_new *poprz = l->poprz_w;
        Lista_new *nast = l->nast_w;
        Lista_new *akt = l;
        while(akt != NULL) {
            akt->liczba_sasiadow = 0;
            sprawdzenie_sasiadow_wyzej_nizej(akt,poprz);
            if(akt->poprz_k != NULL && akt->poprz_k->k == akt->k - 1 && akt->poprz_k->stan == 1) {
                akt->liczba_sasiadow++;
            }
            if(akt->nast_k != NULL && akt->nast_k->k == akt->k + 1 && akt->nast_k->stan == 1) {
                akt->liczba_sasiadow++;
            }
            sprawdzenie_sasiadow_wyzej_nizej(akt,nast);
            akt = akt->nast_k;
        }
        Uzupelnij_liczbe_sasiadow(l->nast_w);
    }
}

/*
Funckja dla kazdej komorki listy list - l, sprawdza liczbe sasiadow oraz potencjalnie modyfikuje jej stan, wedlug zasad gry.
Jezeli komorka jest zywa i nie ma 2 ani 3 sasiadow zywych to umiera oraz jezeli komorka jest niezywa i ma 3 sasiadow to ozywa.
*/
void Zmien_stan(Lista_new *l) {
    while(l != NULL) {
        Lista_new *pom = l;
        while(pom != NULL) {
            if(pom->stan == 0) {
                if(pom->liczba_sasiadow == 3) {
                    pom->stan = 1;
                }
            }
            else if(pom->liczba_sasiadow != 2 && pom->liczba_sasiadow != 3) {
                pom->stan = 0;
            }
            pom = pom->nast_k;
        }
        l = l->nast_w;
    }
}

/*
funkcja usuwa niepotrzebne komorki z listy list - l;
Komorka zostanie usunieta tylko wtedy, jezeli jest niezywa i ma 0 zywych sasiadow.
Nastepnie zwracany jest poczatek nowej listy
*/
Lista_new * Usun_niepotrzebnych_sasiadow(Lista_new *l) {
    Lista_new *zap = NULL;
    while(l != NULL) {
        Lista_new *pom = l;
        Lista_new *nastepny = l->nast_w;
        while(pom != NULL) {
            if(pom->stan == 0 && pom->liczba_sasiadow == 0) {
                Lista_new *poprz = pom->poprz_k;
                Lista_new *nast = pom->nast_k;
                if(poprz == NULL) {
                        if(nast != NULL) {
                            nast->poprz_k = NULL;
                            nast->nast_w = pom->nast_w;
                            nast->poprz_w = pom->poprz_w;
                            if(pom->poprz_w != NULL) {
                                pom->poprz_w->nast_w = nast;
                            }
                            if(pom->nast_w != NULL) {
                                pom->nast_w->poprz_w = nast;
                            }
                            free(pom);
                            pom = NULL;
                        }
                        else {
                            Lista_new *pop_w = pom->poprz_w;
                            Lista_new *nast_w = pom->nast_w;
                            if(pop_w != NULL) {
                                pop_w->nast_w = nast_w;
                            }
                            if(nast_w != NULL) {
                                nast_w->poprz_w = pop_w;
                            }
                        free(pom);
                        }
                        pom = nast;
                }
                else {
                    poprz->nast_k = nast;
                    if(nast != NULL) {
                        nast->poprz_k = poprz;
                    }
                    free(pom);
                    pom = NULL;
                    pom = nast;
                }
            }
            else {
                if(zap == NULL) {
                    zap = pom;
                }
                pom = pom->nast_k;
            }
        }
            l = nastepny;
    }
    while(zap != NULL && zap->poprz_k != NULL) {
        zap = zap->poprz_k;
    }
    while(zap != NULL && zap->poprz_w != NULL) {
        zap = zap->poprz_w;
    }
    return zap;
}


/*
funkcja produkuje stan gry po 'n' generacjach, zaczynajac od stanu zawartego w 'Plansza',
nastepnie wypisuje ja w domyslnym formacie okna.
Zwraca nowy poczatek listy list zawierajacej komorki
*/
Lista_new * Generacja(int n, Lista_new *Plansza,int w, int k) {
    while(n > 0) {
        Dodaj_niezywych_sasiadow_v2(Plansza);
        while(Plansza != NULL && Plansza->poprz_k != NULL) {
            Plansza = Plansza->poprz_k;
        }
        while(Plansza != NULL && Plansza->poprz_w != NULL) {
            Plansza = Plansza->poprz_w;
        }
        Uzupelnij_liczbe_sasiadow(Plansza);
        Zmien_stan(Plansza);
        Plansza = Usun_niepotrzebnych_sasiadow(Plansza);
        n--;
    }
        wypisz_okno(Plansza,w,k,w);
        return Plansza;
}

/*
funkcja symuluje cala gre "Gra w Zycie".
Czyta input, wpisuje plansze poczatkowa oraz nastepnie prowadzi gre wedle polecen gracza
nastepnie zwalnia cala zaaolokowana pamiec.
*/
void symulacja() {
    int *input;
    int n;
    int i = 0;
    int okno_w = 1;
    int okno_k = 1;
    czytaj_input(&input,&n);
    Lista_new *Plansza = Wpisz_Plansze_poczatkowa_v2(input,&i);
    wypisz_okno(Plansza,1,1,1);

    while(input[i] != -1*'.') {
        if(input[i+1] == -1*' ') {
            okno_w = input[i];
            okno_k = input[i+2];
            i += 4;
            wypisz_okno(Plansza,okno_w,okno_k,okno_w);
        }
        else if(input[i] == 0) {
            Wypisz_startowy_format(Plansza);
            i += 2;
            wypisz_okno(Plansza,okno_w,okno_k,okno_w);
        }
        else if(input[i] == -1*'\n') {
            Plansza = Generacja(1,Plansza,okno_w,okno_k);
            i++;
        }
        else {
            Plansza = Generacja(input[i],Plansza,okno_w,okno_k);
            i += 2;
        }
    }
    zwolnij_liste(Plansza);
    free(input);
}


/*
rozpoczyna gre
*/
int main()
{

    symulacja();

    return 0;
}
