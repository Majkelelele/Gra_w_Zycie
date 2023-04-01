

# Gra_w_Zycie - https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
Program symuluje grę w życie:

Symulacja jest prowadzona na planszy, składającej się z nieskończenie wielu wierszy i nieskończenie wielu kolumn.
Wiersze i kolumny planszy są numerowane liczbami całkowitymi ze znakiem.
Na każdym polu planszy jest komórka w jednym z dwóch stanów: żywa lub martwa.
Przyjmujemy, że komórka w wierszu w i kolumnie k sąsiaduje na planszy z ośmioma innymi komórkami, które mają numer wiersza od w - 1 do w + 1 a numer kolumny od k - 1 do k + 1.
Symulacja zaczyna się od pewnej generacji początkowej, na podstawie której liczymy następne.
W następnej generacji komórka będzie żywa wtedy i tylko wtedy, gdy:
-w bieżącej generacji jest żywa i ma dokładnie dwóch lub trzech żywych sąsiadów, albo
-w bieżącej generacji jest martwa i ma dokładnie trzech żywych sąsiadów.
