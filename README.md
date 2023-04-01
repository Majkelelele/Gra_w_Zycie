

# Gra_w_Zycie -

https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
Program symuluje grę w życie:

Symulacja jest prowadzona na planszy, składającej się z nieskończenie wielu wierszy i nieskończenie wielu kolumn.
Wiersze i kolumny planszy są numerowane liczbami całkowitymi ze znakiem.
Na każdym polu planszy jest komórka w jednym z dwóch stanów: żywa lub martwa.
Przyjmujemy, że komórka w wierszu w i kolumnie k sąsiaduje na planszy z ośmioma innymi komórkami, które mają numer wiersza od w - 1 do w + 1 a numer kolumny od k - 1 do k + 1.
Symulacja zaczyna się od pewnej generacji początkowej, na podstawie której liczymy następne.
W następnej generacji komórka będzie żywa wtedy i tylko wtedy, gdy:

-w bieżącej generacji jest żywa i ma dokładnie dwóch lub trzech żywych sąsiadów, albo

-w bieżącej generacji jest martwa i ma dokładnie trzech żywych sąsiadów.

# Postać Danych:
Postać danych
Na wejściu programu jest opis generacji początkowej i ciąg poleceń.
Opis generacji wskazuje komórki, które są żywe. Ma postać ciągu wierszy zaczynających się od znaku '/' (slash).
W ostatnim wierszu opisu generacji jest tylko znak '/'. We wszystkich pozostałych wierszach po znaku '/' jest liczba całkowita, będąca numerem wiersza planszy. Po niej jest uporządkowany rosnąco niepusty ciąg liczb całkowitych, będących numerami kolumn. Każda z tych liczb jest poprzedzona jedną spacją.
Wiersz postaci:

/w k1 .. kn

informuje, że w wierszu planszy o numerze w żyją komórki w kolumnach k1, .., kn.
W opisie generacji ciąg numerów wierszy, czyli liczb poprzedzonych znakami '/', jest uporządkowany rosnąco.
Po opisie generacji początkowej, na wejściu programu jest ciąg jednowierszowych poleceń.

# Program rozpoznaje polecenia:

-zakończenia pracy,
  Ma postać wiersza ze znakiem '.' (kropka).  
  Przerywa pętlę czytającą polecenia i kończy pracę programu.

-obliczenia N-tej kolejnej generacji,
  Ma postać wiersza z dodatnią liczbą całkowitą N.
  Liczy N-tą kolejną generację, zaczynając od aktualnej. W szczególności, polecenie 1 powoduje obliczenie następnej generacji.

-obliczenia następnej generacji,
  Ma postać wiersza pustego. Jest równoważne poleceniu 1.

-zrzutu stanu aktualnej generacji,
  Ma postać wiersza z liczbą 0 (zero).
  Generuje kompletny opis aktualnej generacji w takim samym formacie, w jakim był wczytany przez program opis generacji początkowej.

-przesunięcia okna.
  Ma postać wiersza z dwiema liczbami całkowitymi, w oraz k, rozdzielonymi spacją.
  Zmienia współrzędne lewego górnego rogu okna, umieszczając go w wierszu w i kolumnie k.


