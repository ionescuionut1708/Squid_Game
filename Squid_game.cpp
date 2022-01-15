#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <thread>
#include <unordered_map>
#include <chrono>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

class game_participants
{
public:
    char data[108][500] = {'\0'};
    void read_data()
    {
        ifstream f("game_participants.csv");
        int i = 0; //indexarea participansilor se face de la 0
        while ((!f.eof()))  //punem toate datele din fisier intr-o matrice (vector de cuvinte)
        {
            f >> data[i ++];
        }
        
        f.close();
    }
};

class competitors : public game_participants
{
public:
    char competitors[99][500] = {'\0'};
    void random_selection()
    {
        read_data();
        for (int i = 0; i < 99; i++)
        {
            int j = rand() % 108;
            if (!strstr(data[j], "done"))
            {
                strcat(competitors[i], data[j]);
                strcpy(data[j], "done"); // marcam ce participant am ales pentru a fi concurent
            }
            else i--; // contorul i va sta pe loc
        }
    }
};

class supervisors : public competitors
{
public:
    char supervisors[9][500] = {'\0'};
    void random_selection_supervisors()
    {
        random_selection();
        for (int i = 0; i < 9; i++)
        {
            int j = rand() % 108;
            if (!strstr(data[j], "done"))
            {
                strcat(supervisors[i], data[j]);
                strcpy(data[j], "done"); // marcam ce participant am ales pentru a fi concurent
            }
            else i--; // asta inseamna ca mai sus am ales random un indice care a fost deja ales
            // contorul i va sta pe loc
        }
    }
};

class Mixing : public supervisors
{
public:
    struct data
    {
        char rectangle[500] = {'\0'}, circle[500] = {'\0'}, triangle[500] = {'\0'};
    } mix[9];
    void mixing()
    {
        random_selection_supervisors();
        for (int i = 0; i < 3; i ++) strcpy(mix[i].rectangle, supervisors[i]);
        for (int i = 3; i < 6; i ++) strcpy(mix[i].circle, supervisors[i]);
        for (int i = 6; i < 9; i ++) strcpy(mix[i].triangle, supervisors[i]);
    }
};

class Red_Light_Green_Light : public Mixing
{
public:
    void red_green()
    {
        mixing();
        for(int i = 0; i < 99; i ++)
        {
            if (i % 2 == 0) // concurentii cu numar par
            {
                for (int j = i; j < 98; j++)
                    strcpy(competitors[j], competitors[j + 1]);
            }
        }
        competitors[50][500] = {'\0'};
        cout << "\n\nThe 'Red Light Green Light' game has just ended and these are the remaining participants :\n\n";
        for (int i = 0; i < 50; i++)
            cout << competitors[i] << "\n";

        
    }
};

class Tug_of_War : public Red_Light_Green_Light
{
public:
   char new_teams[27][500] = {'\0'};
   void war()
   {
       red_green();
       int team1, team2, team3, team4;
       team1 = team2 = team3 = team4 = 0;
       for (int i = 0; i < 48; i++)
       {
           if (i <= 11) // prima echipa
           {
               int n = strlen(competitors[i]);
               int weight = 0, p = 1;
               for (int j = n - 1; j >= 0; j--)
               {
                   if (competitors[i][j] != ',')
                   {
                       weight = weight + (competitors[i][j] - '0') * p;
                       p *= 10;
                   }
                   else break; //am ajuns la virgula de la final
               }
               team1 += weight;
           }
           else if(i >= 12 && i <= 23) //a doua echipa
           {
               int n = strlen(competitors[i]);
               int weight = 0, p = 1;
               for (int j = n - 1; j >= 0; j--)
               {
                   if (competitors[i][j] != ',')
                   {
                       weight = weight + (competitors[i][j] - '0') * p;
                       p *= 10;
                   }
                   else
                       break; // am ajuns la virgula
               }
               team2 += weight;
           }
           else if (i >= 24 && i <= 35) // a treia echipa
           {
               int n = strlen(competitors[i]);
               int weight = 0, p = 1;
               for (int j = n - 1; j >= 0; j--)
               {
                   if (competitors[i][j] != ',')
                   {
                       weight = weight + (competitors[i][j] - '0') * p;
                       p *= 10;
                   }
                   else break; // am ajuns la virgula
               }
               team3 += weight;
           }
           else if (i >= 36 && i <= 47) // a treia echipa
           {
               int n = strlen(competitors[i]);
               int weight = 0, p = 1;
               for (int j = n - 1; j >= 0; j--)
               {
                   if (competitors[i][j] != ',')
                   {
                       weight = weight + (competitors[i][j] - '0') * p;
                       p *= 10;
                   }
                   else break; // am ajuns la virgula
               }
               team4 += weight;
           }
       }
       int poz1, poz2, poz = 0;
       if(team1 > team2) //incepe duelul primelor 2 echipe
       {
            poz1 = 0;
            poz2 = 11;
       }
       else
       {
           poz1 = 12;
           poz2 = 23;
       }
       for(int i = poz1; i <= poz2; i ++)
          strcpy(new_teams[poz ++], competitors[i]); 

       int poz3, poz4;
       if (team3 > team4) // incepe duelul ultimelor 2 echipe
       {
          poz3 = 24;
          poz4 = 35;
       }
       else
       {
           poz3 = 36;
           poz4 = 47;
       }
       for (int i = poz3; i <= poz4; i++)
           strcpy(new_teams[poz++], competitors[i]);

       strcpy(new_teams[25], competitors[48]);
       strcpy(new_teams[26], competitors[49]); // 2 concurenti care merg direct in etapa urmatoare
       
       cout << "\n\nThe remaining contestants after the 'Tug of War' contest are :\n\n";
       for (int i = 0; i < 27; i++)
        cout << new_teams[i] << "\n";
   }
};

class Marbles : public Tug_of_War
{
public:
   char pairs[15][500] = {'\0'};
   void pair()
   {
       war();
       int v[27] = {0}, poz, nr = 0;
       for(int i = 0; i < 27; i ++) v[i] = rand() % 300;
       
       for(int i = 0; i < 26; i ++)
       {
          if(v[i] < v[i + 1]) poz = i;
          else poz = i + 1;
          
          strcpy(pairs[nr ++], new_teams[poz]);
          i ++;
       }
       cout << "\n\nThe remaining contestants after the 'Marbles' contest are :\n\n";
       for(int i = 0; i < 13; i ++) //14 concurenti au ramas
        cout << pairs[i] << "\n";
   }
};

class Genken: public Marbles
{
public:
    char last_game[14][500] = {'\0'};
    void duel()
    {
        pair();
        int v[14] = {0};
        for (int i = 0; i < 13; i++) v[i] = rand() % 3;
        int flag = 13, nr = 0;
        while(flag != 1)
        {
            for (int i = 0; i < flag - 1; i++)
            {
                int poz = i;
                if(v[i] == 0 && v[i + 1] == 1) poz = i + 1;
                if(v[i] == 0 && v[i + 1] == 2) poz = i;
                if(v[i] == 1 && v[i + 1] == 2) poz = i + 1;
                
                strcpy(last_game[nr ++], pairs[poz]);
            }
            flag = flag / 2;
        }
        cout << "\n\n\n\nAnd the winner is ... \n" << last_game[nr - 1] << "\n !!!!!!!!!!!!!!!!!!!!!!!!!!\t\t";
    }
};

int main()
{
    Genken Squid_Game;
    Squid_Game.duel();

    return 0;
}