#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define X 80  
#define Y 25  

char **dynamic_alloc(); 

void run(char **data1, char **data2, char *key, int *speed); 

void init_field(char **data1, FILE *file);      
void draw_field(char **data1, int speed);       
void update_field(char **data1, char **data2);  

int life(char **data2, int y, int x);  

void menu();                           
int main_menu(char **data1, int speed);  

int main() {
    initscr();  
    cbreak();   
    noecho();   
    nodelay(stdscr, TRUE);  
                            

    int speed = 100000;  
    char key = '0';  
                     

    char **data1 = dynamic_alloc();  
    char **data2 = dynamic_alloc();  

    menu();  

    if (main_menu(data1, speed)) {
        run(data1, data2, &key, &speed);
    } else {
        printw("\nError! File incorrect!\nPress q to exit...");
    }

    free(data2); 
    free(data1); 

    endwin();  

    return 0;
}

char **dynamic_alloc() {
    char **data = malloc(Y * sizeof(char *) + Y * X * sizeof(int));
    char *ptr1 = (char *)(data + Y);
    for (int i = 0; i < Y; i++) {
        data[i] = ptr1 + X * i;
    }
    return data;
}

void run(char **data1, char **data2, char *key, int *speed) {
    while (*key != 'q' && *key != 'Q') { 
        *key = getch();                  
        update_field(data1, data2);
        draw_field(data1, *speed);
        if (*key == '+' && *speed >= 20000) {  
            *speed -= 10000;
        }
        if (*key == '-' && *speed <= 150000) {  
            *speed += 10000;
        }
    }
}

void init_field(char **data1, FILE *file) {
    char temp;
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            temp = getc(file);  
            if (temp == '.') {
                data1[i][j] = ' ';
            } else if (temp == '*') {
                data1[i][j] = '*';
            } else {
                j--;
            }
        }
    }
}

void draw_field(char **data1, int speed) {
    clear();
    usleep(speed);
    printw(">");
    for (int i = 0; i < X; i++) printw("~");
    printw("<\n");
    for (int i = 0; i < Y; i++) {
        printw(">");
        for (int j = 0; j < X; j++) {
            if (j == X - 1) {
                printw("%c", data1[i][j]);
                continue;
            }
            printw("%c", data1[i][j]);
        }
        printw("<\n");
    }
    printw(">");
    for (int i = 0; i < X; i++) printw("~");
    printw("<");
}

void update_field(char **data1, char **data2) {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            data2[i][j] = data1[i][j];
        }
    }
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (life(data2, i, j))  
                data1[i][j] = '*';
            else
                data1[i][j] = ' ';
        }
    }
}

int life(char **data2, int y_point, int x_point) {
    int live_points = 0;
    if (data2[(y_point + 1 + Y) % Y][x_point] == '*') live_points++;
    if (data2[(y_point - 1 + Y) % Y][x_point] == '*') live_points++;
    if (data2[y_point][(x_point + 1 + X) % X] == '*') live_points++;
    if (data2[y_point][(x_point - 1 + X) % X] == '*') live_points++;
    if (data2[(y_point - 1 + Y) % Y][(x_point - 1 + X) % X] == '*') live_points++;
    if (data2[(y_point + 1 + Y) % Y][(x_point + 1 + X) % X] == '*') live_points++;
    if (data2[(y_point + 1 + Y) % Y][(x_point - 1 + X) % X] == '*') live_points++;
    if (data2[(y_point - 1 + Y) % Y][(x_point + 1 + X) % X] == '*') live_points++;
    if (data2[y_point][x_point] == '*' && (live_points == 2 || live_points == 3))
        return 1;
    else if (data2[y_point][x_point] == ' ' && live_points == 3)
        return 1;
    return 0;
}

void menu() {
    printw(
        "Choose shapes:\n1.symmetric "
        "oscilator\n2.rake\n3.hive\n4.diehard\n5.gun\n[+]-speed up\n[-]-speed "
        "down\nPRESS Q TO EXIT!\n");
}

int main_menu(char **data1, int speed) {
    int flag = 0;
    char key = '0';
    while (key != '1' && key != '2' && key != '3' && key != '4' && key != '5' && key != 'q' && key != 'Q') {
        key = getch();
    }
    FILE *file;
    switch (key) {
        case '1':
            file = fopen("1.txt", "r");
            flag = 1;
            init_field(data1, file);
            draw_field(data1, speed);
            break;
        case '2':
            file = fopen("2.txt", "r");
            flag = 1;
            init_field(data1, file);
            draw_field(data1, speed);
            break;
        case '3':
            file = fopen("3.txt", "r");
            flag = 1;
            init_field(data1, file);
            draw_field(data1, speed);
            break;
        case '4':
            file = fopen("4.txt", "r");
            flag = 1;
            init_field(data1, file);
            draw_field(data1, speed);
            break;
        case '5':
            file = fopen("5.txt", "r");
            flag = 1;
            init_field(data1, file);
            draw_field(data1, speed);
            break;
        default:
            flag = 0;
            break;
    }
    fclose(file);
    return flag;
}