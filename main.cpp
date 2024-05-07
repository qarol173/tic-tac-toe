#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <iostream>
#include <algorithm>

// klasa do tworzenia obiektów-zawodników
class Player
{
    public:
        std::string id;
        char sign;
        int wins = 0;
        int draws = 0;
        int loses = 0;
        int points = 0;
        bool exist = false;

        Player(std::string id_arg, char sign_arg)
        {
            id = id_arg;
            sign = sign_arg;
        };
};

// funkcja do rodzielania stringów na pojedyncze wyrazy
std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> slowa;
    int start = 0;

        while(true)
        {
            int indeks = str.find(delimiter, start);
            if (indeks == std::string::npos) {
                break;
            }
            int dlugosc = indeks - start;
            slowa.push_back(str.substr(start, dlugosc));
            start += (dlugosc + 1);
        }

    slowa.push_back(str.substr(start));
    return slowa;
}

// import danych o przebiegu gier
std::vector<std::string> results_of_games(std::string file_name)
{
    std::vector<std::string> games;

    std::fstream file(file_name);
    if (!file.is_open())
    {
        std::cerr << "Error! Failed to open file" << std::endl;
    }

    std::string line;
    std::string gra = "";
    int i=1;
    while (std::getline(file, line))
    {
        if (i%4)
        {
            gra+=line;
        }
        else
        {
            games.push_back(gra);
            gra="";
        }
        ++i;
    }
    file.close();
    return games;
}

// import danych o zawodnikach (numer indeksu, znak)
std::vector<std::vector<std::string>> players_data_import (std::string file_name)
{
    std::vector<std::vector<std::string>> information;

    std::fstream file(file_name);
    if (!file.is_open())
    {
        std::cerr << "Error! Failed to open file" << std::endl;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<std::string> one_player_info = split(line, ' ');
        information.push_back(one_player_info);
    }
    file.close();
    return information;
}

// analiza rozgrywki
std::string rozgrywka (std::string str)
{
    char znak1 = ' ';
    char znak2 = ' ';
    char winner = ' ';
    std::string message = " ";

    for (int i=0; i<str.size(); ++i)
    {
        if (str[i]!='.' && znak1==' ') znak1=str[i];
        if (str[i]!='.' && znak1!=' ' && str[i]!=znak1) znak2=str[i];
    }
    if (znak1==' ' || znak2==' ') return message;

    if (str[0]!='.' && str[0]==str[1] && str[1]==str[2]) winner=str[0];
    if (str[3]!='.' && str[3]==str[4] && str[4]==str[5]) winner=str[3];
    if (str[6]!='.' && str[6]==str[7] && str[7]==str[8]) winner=str[6];
    if (str[0]!='.' && str[0]==str[3] && str[3]==str[6]) winner=str[0];
    if (str[1]!='.' && str[1]==str[4] && str[4]==str[7]) winner=str[1];
    if (str[2]!='.' && str[2]==str[5] && str[5]==str[8]) winner=str[2];
    if (str[0]!='.' && str[0]==str[4] && str[4]==str[8]) winner=str[0];
    if (str[2]!='.' && str[2]==str[4] && str[4]==str[6]) winner=str[2];

    std::string s1(1, znak1);
    std::string s2(1, znak2);

    if (winner!=' ')
    {
        if (winner==znak1) message=s1+s2+" ";
        if (winner==znak2) message=s2+s1+" ";
    }
    else message=" "+s1+s2;
    return message;
}

//sortowanie zawodnikow wg punktow
void sort_players(std::vector<Player*>& zawodnicy)
{
    auto compare = [](const Player* lhs, const Player* rhs) -> bool
    {
        if (lhs->points == rhs->points)
        {
            return lhs->wins > rhs->wins;
        }
        return lhs->points > rhs->points;
    };
    std::sort(zawodnicy.begin(), zawodnicy.end(), compare);
}

// wyswietlanie pliku
void view_file (std::string file_name)
{
    std::fstream file(file_name);
    if (!file.is_open())
    {
        std::cerr << "Error! Failed to open file" << std::endl;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
    }
    file.close();
}

int main()
{
    std::vector<std::string> games = results_of_games("tictactoe.txt");
    std::vector<std::vector<std::string>> players_data = players_data_import("players.txt");

    //wykorzystanie ASCII do szybszego rozwiazania zadania
    const int rozmiar = 96;
    Player* tab[rozmiar];
    for (int i=0; i<rozmiar; ++i)
    {
        tab[i]= new Player("000000", ' ');
    }

    for (int i=0; i<players_data.size(); ++i)
    {
        char sign = players_data[i][1][0];
        int indeks = int(sign)-32;
        tab[indeks]->sign = sign;
        tab[indeks]->id = players_data[i][0];
        tab[indeks]->exist = true;
    }

    for (int i=0; i<games.size(); ++i)
    {
        std::string wynik = rozgrywka(games[i]);
        if (wynik.size()!=3) std::cerr << wynik.size() << "Error! Failed to open file" << std::endl;
        else
        {
            if(wynik[0]==' ')
            {
                char player1 = wynik[1];
                int indeks1 = int(player1)-32;
                char player2 = wynik[2];
                int indeks2 = int(player2)-32;
                tab[indeks1]->draws += 1;
                tab[indeks2]->draws += 1;
                tab[indeks1]->points += 1;
                tab[indeks2]->points += 1;
            }

            if(wynik[0]!=' ')
            {
                char player1 = wynik[0];
                int indeks1 = int(player1)-32;
                char player2 = wynik[1];
                int indeks2 = int(player2)-32;
                tab[indeks1]->wins += 1;
                tab[indeks2]->loses += 1;
                tab[indeks1]->points += 3;
            }
        }
    }

// ----------- wlasciwy program -----------

    char znak;
    do
    {
        std::cout << "1 - show table \n2 - show results \n3 - show player stats \n4 - exit \nChoose option: ";
        znak = getch();
        std::cout << znak;
        if (znak!='1' && znak!='2' && znak!='3' && znak!='4') std::cout << std::endl << std::endl << "You typed an incorrect number. Try again: " << std::endl;


        if (znak == '1')
        {
            std::vector<Player*> players_pomoc;
            for (int i=0; i<rozmiar; ++i)
            {
                players_pomoc.push_back(tab[i]);
            }
            sort_players(players_pomoc);
            std::cout << std::endl << std::endl << "\tId \tSign \tWins \tDraws \tLoses \tPoints" << std::endl;
            for (int i=0; i<players_pomoc.size(); ++i)
            {
                if (players_pomoc[i]->exist) std::cout << "\t" << players_pomoc[i]->id << "\t"
                    << players_pomoc[i]->sign << "\t" << players_pomoc[i]->wins << "\t" << players_pomoc[i]->draws << "\t" << players_pomoc[i]->loses << "\t" << players_pomoc[i]->points << std::endl;
            }
            std::cout << std::endl << std::endl;
        }

        if (znak == '2')
        {
            view_file("tictactoe.txt");
            std::cout << std::endl << std::endl;
        }

        if (znak == '3')
        {
            std::string searching_player;
            std::cout << std::endl << "Enter player id: ";
            std::cin >> searching_player;
            bool znaleziono=false;
            for (int i=0; i<rozmiar; ++i)
            {
                if (tab[i]->id==searching_player)
                {
                    std::cout << "\t" << tab[i]->id << "\t" << tab[i]->sign << "\t" << tab[i]->wins
                    << "\t" << tab[i]->draws << "\t" << tab[i]->loses << "\t" << tab[i]->points << std::endl;
                    znaleziono=true;
                    break;
                }
            }
            if (znaleziono==false)
            {
                std::cout << "\tThere is no player with this id";
            }
            std::cout << std::endl << std::endl;
        }
    }
    while (znak!='4');

    if (znak == '4')
    {
        std::cout << std::endl << std::endl;
        delete[] tab;
        return 0;
    }
}
