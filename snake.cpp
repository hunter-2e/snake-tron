#include <ncurses.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <thread>

using namespace std;

void displayGrid(void);
void spawnFood(void);
void moveSnake(vector<vector<int>> snake, char snakeLetter);
void changeDirection(char pressed);

enum gridSize { height = 31, width = 31 };

vector<char> row(width, ' ');
vector<vector<char>> grid(height, row);

vector<int> RIGHT ={0,1};
vector<int> LEFT = {0,-1};
vector<int> DOWN = {1,0};
vector<int> UP = {-1,0};

//Starting direction to be updated
vector<int> direction = RIGHT;
vector<int> direction2 = LEFT;

vector<vector<int>> snake = {vector<int>{14,14}};
vector<vector<int>> snake2 = {vector<int>{10,10}};

char snakeLetter = 'X';
char snakeLetter2 = 'O';

int main(){
    initscr();
    start_color();
    timeout(-1);
    nodelay(stdscr, true);
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    //First snake player
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    //Background
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    //Food
    init_pair(3, COLOR_RED, COLOR_RED);
    //Border
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    //Second snake player
    init_pair(5, COLOR_CYAN, COLOR_CYAN);
    
    spawnFood();
    
    thread s2(moveSnake, snake2 ,snakeLetter2);
    
    s2.join();

    moveSnake(snake, snakeLetter);
    

    endwin();
    }


void displayGrid(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int color;

            switch(grid[i][j]){
                case 'X':
                    color = 2;
                    break;
                case '*':
                    color = 3;
                    break;
                case 'O':
                    color = 5;
                    break;
                case ' ':
                    color = 1;
                    break;
            }
            
            if(i == height-1 || j == 0 || j == width-1){
                color = 4;
            }

            attron(COLOR_PAIR(color));
            mvprintw(i, j*3, "   ");
            attroff(COLOR_PAIR(color));
        }
        mvprintw(i, (width)*3, "\n");
    }
    refresh();
}

void spawnFood(){
    srand((unsigned)time(NULL));

    int randomX = rand() % 30;
    int randomY = rand() % 30;
    while(grid[randomX][randomY] != ' '){
        randomX = rand() % 30;
        randomY = rand() % 30;
    }
        
    // '*' represents spawning of food
    grid[randomX][randomY] = '*';    
    displayGrid();
}

void moveSnake(vector<vector<int>> snake, char snakeLetter){
    for(int point = 0; point < snake.size(); point++){
        grid[snake[point][0]][snake[point][1]] = ' ';
    }

    displayGrid();

    vector<int> curDirection;

    if(snakeLetter == 'X'){
        curDirection = direction;
    }
    else{
        curDirection = direction2;
    }

    //If valid position continue game
    if(snake[snake.size()-1][0] + curDirection[0] < 30 && snake[snake.size()-1][1] + curDirection[1] < 30 && snake[snake.size()-1][1] + curDirection[1] > 0 && snake[snake.size()-1][0] + curDirection[0] > 0){       
        if(snake.size() == 1){
            snake[snake.size()-1][0] += curDirection[0];
            snake[snake.size()-1][1] += curDirection[1];
        }
        
        else{
            snake.erase(snake.begin());

            vector<int> swapFrontBack;

            swapFrontBack.push_back(snake[snake.size()-1][0] + curDirection[0]);
            swapFrontBack.push_back(snake[snake.size()-1][1] + curDirection[1]);

            snake.push_back(swapFrontBack);
        }
        
     
    if(grid[snake[snake.size()-1][0]][snake[snake.size()-1][1]] == '*'){
        snake.insert(snake.begin(), snake[snake.size()-1]);
        spawnFood();
        }
    
    }

    //If not valid end game
    else{
        exit(1);
    }


    for(int point = 0; point < snake.size(); point++){
        grid[snake[point][0]][snake[point][1]] = snakeLetter;
    }
    displayGrid();

    changeDirection(getch());
    usleep(100000);
    changeDirection(getch());

    if(snakeLetter == 'X'){
        curDirection = direction;
    }
    else{
        curDirection = direction2;
    }
    
    if(grid[snake[snake.size()-1][0] + curDirection[0]][snake[snake.size()-1][1] + curDirection[1]] == snakeLetter){
        exit(1);
    }

    moveSnake(snake, snakeLetter);
}

void changeDirection(char pressed){
    switch(pressed){
        case 'a':
            direction = LEFT;
            break;
        case 'w':
            direction = UP;
            break;
        case 's':
            direction = DOWN;
            break;
        case 'd':
            direction = RIGHT;
            break;
        case 'j':
            direction2 = LEFT;
            break;
        case 'i':
            direction2 = UP;
            break;
        case 'k':
            direction2 = DOWN;
            break;
        case 'l':
            direction2 = RIGHT;
            break;
    }
}