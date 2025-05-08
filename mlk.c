#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define max_monsters_spawned 6

typedef struct{
    char id;
    int hp;
    int x,y;
    int moves;
}Hero;

char** board();
void print_board(char** board);
void free_board(char** board);
void select_classes(int blyat, char select[], Hero heroes[]);
void select_difficulty();
void place_players(char **board,char select[],Hero heroes[]);
void place_cosmetic(char **board);
void place_monsters(char **board);
void move(char **board, Hero heroes[]);
int monster_sum, blyat = 0;
int monster_hp[max_monsters_spawned] = {0};
char select[5] = {0};
int N = 7, M =5; 

int main() {
    Hero heroes[4];
    srand(time(NULL));
    
    char** map;
    select_classes(blyat, select, heroes);
    select_difficulty();
    map = board(N, M); //desmeyei mnhnmh board[i][j]
    if (map != NULL){
        place_players(map, select, heroes);
        place_cosmetic(map);
        place_monsters(map);
        print_board(map);
    }
    while(1){
        int sum=0;
        sum++;     
        move(map, heroes);
        print_board(map);
    break;
    }
    free_board(map);
    

    return 0;
}

int is_valid_hero(char c) {
    return (c == 'b' || c == 'd' || c == 'e' || c == 'm');
}

char valid_direction(char c) {
    return (c == 'U' || c == 'L' || c == 'R' || c == 'D');
}

void mvmnt(int *x, int *y, char c,  int b) {
    switch (c) {
        case 'U':
            *x -= b;
            break;
        case 'D':
            *x += b;
            break;
        case 'L':
            *y -= b;
            break;
        case 'R':
            *y += b;
            break;
        default:
            printf("check\n");
            return;
    }
}

void boundandemptiness(char** board, int nx, int ny, int* stop, int t_s, int* check){
    if (nx < 0 || nx >= N || ny < 0 || ny >= M) {
        printf("Move out of bounds at step %d. Stopping.\n", t_s + 1);
        *stop = 1;
        *check = 1;
    }

    if (board[nx][ny] != '.') {
        printf("Blocked at (%d, %d) by '%c'. Stopping.\n", nx, ny, board[nx][ny]);
        *stop = 1;
        *check = 1;
    }
}


void select_classes(int blyat, char select[], Hero heroes[]) {
    select[5] = '\0'; // classes
    int h_sum = 4; // limit for the number of players on the board
    printf("Select up to 4 heroes (barbarian 'b', dwarf 'd', elf 'e', mage 'm'):\ntype - to select difficulty\n");
    for (int i = 0; i < h_sum; i++) {
        char input;

        while (1) {
            printf("Hero %d: ", i + 1);
            scanf(" %c", &input);
            if (input == '-' && i > 0) { // allow exiting after at least one hero is selected
                i = h_sum;
                break;
            }
            if (!is_valid_hero(input)) {
                printf("Invalid hero. Try again.\n");
                continue;
            }
            int already_picked = 0;
            for (int j = 0; j < i; j++) {
                if (select[j] == input) {
                    already_picked = 1;
                    break;
                }
            }
            if (already_picked) {
                printf("You already selected that hero. Pick a different one.\n");
            } else {
                select[i] = input; 
                heroes[i].id = input;
                if(input == 'b'){
                    heroes[i].hp = 8;
                    heroes[i].moves = 8;
                }
                else if(input == 'd'){
                    heroes[i].hp = 7;
                    heroes[i].moves = 6;
                }
                else if(input == 'e'){
                    heroes[i].hp = 6;
                    heroes[i].moves = 12;
                }
                else if(input == 'm') {// W magos
                    heroes[i].hp = 4;
                    heroes[i].moves = 10;
                }
                blyat++;
                break;
            }
        }
        if (select[i] == '-') {
            i = h_sum;
            break;
        }
    printf("Heroes selected successfully:\n");
    for (int i = 0; i < h_sum; i++) {
        printf("%c ", select[i]);
    }
    printf("\n");
    }
}

void select_difficulty() {
    char selection;
    int temp;
    printf("Select difficulty:\nEasy: E\nModerate: M\nHard: H\n");

    while (selection != 'E' && selection != 'M' && selection != 'H') {
        selection = getchar(); //while baraei mono scan    
        if (selection == 'E'){
     
            monster_sum = (rand() % 3) + 1;
            for(int i = 0; i < monster_sum; i++){
                temp = (rand() % 2) + 1; 
                monster_hp[i] = temp + '\0';
            }
            
        }
        else if(selection == 'M'){
        
            monster_sum = (rand() % 4) + 1;
            for (int i = 0; i < monster_sum; i++){
                temp = (rand() % 3) + 4;
                monster_hp[i] = temp + '\0';
            }
        }
        else if(selection == 'H'){
          
            monster_sum = (rand() % 6) + 1;
            for(int i = 0; i < monster_sum; i++){
                temp = (rand() % 3) + 7;  
                monster_hp[i] = temp;         
            }
        } 
    }
}

char** board(){
    char** map = (char**)malloc(sizeof(char*) * N);
    if(map == NULL){
        printf("error allocating memory!\n");
        return NULL;
    }
    for (int i = 0; i < N; i++){
        map[i] = (char*)malloc(sizeof(char) * M);
        if (map[i] == NULL){
            printf("error allocating memory!\n");
            for(int j = 0; j < i; j++) {
                free(map[j]);}
            free(map);
            break;
        }
        for (int j = 0; j < M; j++){
            map[i][j] = '.';
        }
    }
    return map;
    
}

void print_board(char **board) {   
    printf("  ");
    for (int i = 0; i < M; i++){ 
        printf("%c ", 'A' + i);
    } //alfabhta
    printf("\n ");
    for (int i = 0; i < M; i++){
        printf("--");   
    }//orizontia sthlh katw apo alfabhta   
    printf("-\n");

    for (int i = 0; i < N; i++) {
        printf("%d|", i + 1);  
        for (int j = 0; j < M; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
    /*for(int i = 0; i < max_monsters_spawned; i++)
        monster_hp[i] = '\0';*/            //de kserw ama einai aparaithto lgka oxi
}

void free_board(char** board){
    for (int i = 0; i < N; i++) {
        free(board[i]);
    }
    free(board);
}

void place_players(char **board, char select[], Hero heroes[]) {
    int placed = 0;

    while (placed < 4 && select[placed] != '\0') { 
        int i = rand() % N;
        int j = rand() % M;

        if (board[i][j] == '.') { 
            board[i][j] = select[placed]; 
            heroes[placed].id = select[placed]; 
            heroes[placed].x = i; 
            heroes[placed].y = j;
            placed++;
        }
    }
}
    
void place_cosmetic(char **board) { 
    int placed = 0;
    while (placed < 1) {
        int i = rand() % N;
        int j = rand() % M;

        if (board[i][j] == '.') {
            board[i][j] = '@';
            placed++;
        }
    }
}

void place_monsters(char **board) {
    int placed = 0;
    while (placed < monster_sum) {
        int i = rand() % N;
        int j = rand() % M;

        if (board[i][j] == '.') {    
            board[i][j] = monster_hp[placed] + '0';
            placed++;
        }
    }
}

void move(char **board, Hero heroes[]){
    char input = '\0';        //epilogh hero
    printf("Select a character to move\n");
    scanf(" %c", &input);

    int valid = 0;     
    int h = -1;           //bale na ginetai to input kefalaio                
    for (int i = 0; i < 4; i++) {//elenxos ama edwse swsto hero
        if (is_valid_hero(input) && heroes[i].id == input) { 
            valid = 1; 
            h = i;
            break;
        }
    }
    if (!valid) {
        printf("Invalid or unselected hero. Try again.\n");
        return;
    }
        
    printf("%c> ", toupper(heroes[h].id));
    char movement[32]; 
    scanf("%s", movement); 
    
    int m_l = heroes[h].moves; //na allaxtei analogos to struct moves
    int total_steps = 0;
    int cur_x = heroes[h].x;
    int cur_y = heroes[h].y;
    int old_x = cur_x;
    int old_y = cur_y;
    int i = 0;
    int stop =0;
    while(movement[i] != '\0' && total_steps < m_l && !stop){
        char direction = movement[i];
        if (!valid_direction(movement[i])){
            printf("shi\n");
            return;
        }
        i++;

        int steps = 0;
        while(isdigit(movement[i])){
            steps = steps * 10 + (movement[i] - '0');
            i++;
        }
        if(steps == 0) steps = 1;
        for (int j = 0; j < steps && total_steps < m_l; j++){
            int new_x = cur_x;
            int new_y = cur_y;
            mvmnt(&new_x, &new_y, direction, steps);
            if (new_x < 0 || new_x >= N || new_y < 0 || new_y >= M) {
                stop = 1;
                break;
                printf("skata\n");
            }
            if (board[new_x][new_y] != '.') {
                printf("Blocked at (%d, %d) by '%c'. Stopping.\n", new_x, new_y, board[new_x][new_y]);
                stop = 1;
                break;
            }

            board[cur_x][cur_y] = '.';
            cur_x = new_x;
            cur_y = new_y;
            board[cur_x][cur_y] = heroes[h].id;
            total_steps++;
        }
    }
    heroes[h].x = cur_x;
    heroes[h].y = cur_y;
}

    
