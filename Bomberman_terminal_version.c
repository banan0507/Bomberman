#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAP_SIZE 11   //trebuie sa fie matrice bidimensionala si cu coef impar

char map[MAP_SIZE][MAP_SIZE];
char errorMessage[200] = ""; // Buffer pentru mesajul de eroare

void clear_screen()
{
    printf("\033[H\033[J");     
}

int X = 1, Y = 1;    //coordonate player
int bombX = -1, bombY = -1; //-1 = nicio bomba
int bomba = 0;  // 0 = fara bomba, 1 = bomba plasata

void generate_map()
{
    for(int i = 0; i<MAP_SIZE; i++)
        for(int j = 0; j<MAP_SIZE; j++)
            if(i == 0 || j == 0 || i == MAP_SIZE - 1 || j == MAP_SIZE -1)
                map[i][j] = '#';   // marginea hartii este perete solid
            else if(i % 2 == 0 && j % 2 == 0)    //punem peretii din mijlocul hartii
                map[i][j] = '#';
            else
                map[i][j] = (rand() % 4 == 0) ? '@' : ' ';   // @ reprezinta peretii distructibili cu o sansa de 25% de spawn
}

void afisare()
{
    for(int i=0; i<MAP_SIZE; i++)
    {
        for(int j=0; j<MAP_SIZE; j++)
            if(i == Y && j == X)
                printf("P");
            else if(bomba == 1 && i == bombY && j == bombX)
                printf("B");
            else
                printf("%c", map[i][j]);
        printf("\n");
    }
    if(strlen(errorMessage) > 0)
        printf("\n\033[1;31m%s\033[0m\n", errorMessage);   //mesaj rosu sa fie la vederea utilizatorului
}

void mutare_player(char input)
{
    int newX = X, newY = Y;
    if(input == 'w' || input == 'W') newY--;
    if(input == 's' || input == 'S') newY++;
    if(input == 'a' || input == 'A') newX--;
    if(input == 'd' || input == 'D') newX++;

    if(map[newY][newX] == ' ')
    {
        X = newX;
        Y = newY;
    }
}

void punere_bomba()
{
    if(!bomba)
    {
        bombX = X;
        bombY = Y;
        bomba = 1;
    }
}

void explode_bomba()
{
    if(bomba == 1)
        sleep(2); // am folosit biblioteca unistd.h pt a face un delay la bomba
    int explozie_x[] = {0, 0, -1, 1};
    int explozie_y[] = {-1, 1, 0, 0};    //pt explozii sus-jos-stanga-dreapta
    int x,y;

    for(int i=0; i<4; i++)
    {
        x = bombX + explozie_x[i];
        y = bombY + explozie_y[i];
        if(map[y][x] == '@')
            map[y][x] = ' ';
    }
    bomba = 0;
}

int main()
{
    srand(time(NULL));
    generate_map();
    
    char input;
    while(1)
    {
        clear_screen();
        afisare();
        printf("\nMisca-te cu WASD, pune bomba cu B: ");
        scanf(" %c", &input);

        if(input == 'b' || input == 'B')
        {
            punere_bomba();
            explode_bomba();
            strcpy(errorMessage, "");
        }
        else if(input == 'w' || input == 'a' || input == 's' || input == 'd')
        {
            mutare_player(input);\
            strcpy(errorMessage, "");
        }
        else
        {
            strcpy(errorMessage, "Comanda invalida! Foloseste una dintre urmatoarele litere:\n W - Muta sus, S - Muta jos, A - Muta stanga, D - Muta dreapta, B - Pune bomba\n");
            afisare();
            sleep(5);
        }
    }
    printf("\n");
    
    return 0;
}