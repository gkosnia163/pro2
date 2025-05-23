#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SKULL 0
#define BLACK_SHIELD 1
#define WHITE_SHIELD 2

int dice[6] = {SKULL, SKULL, SKULL, BLACK_SHIELD, WHITE_SHIELD, WHITE_SHIELD};
typedef struct{
    char id;
    int hp;
    int x,y;
    int moves;
    int attackx, attacky;
}Hero;

char** board();
void print_board(char** board);
void free_board(char** board);
void select_classes(int blyat, char hselect[], Hero heroes[]);
void select_difficulty();
void place_players(char **board,char hselect[],Hero heroes[]);
void place_cosmetic(char **board);
void place_monsters(char **board);
void game_cmnds(char **board, Hero heroes[]);
void attack(char **board, Hero *heroes);
int roll_dice();
int monster_sum, blyat = 0;
int monster_hp[6] = {0};
char hselect[5] = {'\0'};
int N = 7, M =5; 

int main() {
    Hero heroes[4];
    srand(time(NULL));
    char** map;
    select_classes(blyat, hselect, heroes);
    select_difficulty();
    map = board(N, M); //desmeyei mnhnmh board[i][j]
    if (map != NULL){
        place_players(map, hselect, heroes);
        place_cosmetic(map);
        place_monsters(map);
        print_board(map);
    }
    while(1){    
        game_cmnds(map, heroes);
       // zargon(map, heroes);
    }
    free_board(map);
    

    return 0;
}

int is_valid_hero(char c) {
    return (c == 'b' || c == 'd' || c == 'e' || c == 'm');
}

char valid_direction(char c) {
    return (c == 'U' || c == 'L' || c == 'R' || c == 'D' || c == 'A' || c == '*');
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
            printf("MVMNT ERROR\n");
            return;
    }
}

void exodos(char **board, char input[], int i){
    while(input[i] != '\0'){
        if (toupper(input[i]) == 'O') {
            exit(0);  
        }
        i++;
    }
}

int end_round(char **board, char input[], int i){
    while(input[i] != '\0'){
        if (toupper(input[i]) == 'X') {
            return 1;  
        }
        i++;
    }
}

int roll_dice(){
    int roll = rand() % 6;
    return dice[roll];
}

int check_move(char** board, int nx, int ny){
    if (nx < 0 || nx >= N || ny < 0 || ny >= M) {
        printf("OUT OF BOUNDS\n");
        return 1;
    }

    if (board[nx][ny] != '.') {
        printf("KOUTAKI OCCUPIED\n");
        return 1;
    }
    return 0;
}



void attack(char** board, Hero *heroes){
    int x, y;
    printf("%c attacking: (grammes sthles)\n", heroes->id);
    scanf("%d %d", &x, &y);
    x--; //eprepe na mpei alliws edeixne lathos location
    y--;
    char target = board[x][y];
    if (target == '.' || target == '@' || x < 0 || y < 0 || x >= N || y >= M) {
        printf("lathos coords h occupied\n"); 
        return;
    }
    
    int i, j, index;
    for (i = 0; i < monster_sum; i++){
        if (target == monster_hp[i]){
            index = i;
            break;
        }
    }
    
    printf("hero roll\n");
    int h_hits = 0, m_blocks = 0;   //heroes zari
    for (i = 0; i < 2; i++){
        int result = roll_dice();
        if (result == SKULL){
            printf("SKULL");
            h_hits++;}
        else if (result == BLACK_SHIELD) printf("BLACK SHIELD ");
        else printf("WHITE SHIELD ");
    }
    printf("\n");

    printf("monster roll\n");
    int mresult = roll_dice();      //monster zari
    if (mresult == SKULL){
            printf("SKULL");
            h_hits++;}
        else if (mresult == BLACK_SHIELD){
            printf("BLACK SHIELD ");
            m_blocks++;}
        else printf("WHITE SHIELD ");
    printf("\n");

    int true_damage = h_hits - m_blocks;
    if (true_damage > 0) {
        printf("Monster takes %d damage!\n", true_damage);
        monster_hp[index] -= true_damage;
        if (monster_hp[index] <= 0) {
            printf("Monster defeated!\n");
            board[x][y] = '.';
        } else {
            board[x][y] = monster_hp[index] + '0';
        }
    } else printf("Attack blocked!\n");
}

void select_classes(int blyat, char select[], Hero heroes[]) {
    select[5] = '\0'; // classes
    int i, h_sum = 4; // limit for the number of players on the board
    printf("Select up to 4 heroes (barbarian 'b', dwarf 'd', elf 'e', mage 'm'):\ntype - to select difficulty\n");
    for (i = 0; i < h_sum; i++) {
        char input;

        while (1) {
            printf("Hero %d: ", i + 1);
            scanf(" %c", &input);
            if (input == '-' && i > 0) { 
                i = h_sum;
                break;
            }
            if (!is_valid_hero(input)) {
                printf("Invalid hero. Try again.\n");
                continue;
            }
            int already_picked = 0;
            int j;
            for (j = 0; j < i; j++) {
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
                else if(input == 'm') { 
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
    int h; //pithanotata ayto htan to problhma
    for (h = 0; h < h_sum; h++) {
        printf("%c ", select[h]);
    }
    printf("\n");
    }
}

void select_difficulty() {
    char selection;
    int temp, i;
    printf("Select difficulty:\nEasy: E\nModerate: M\nHard: H\n");

    while (selection != 'E' && selection != 'M' && selection != 'H') {
        selection = getchar(); //while baraei mono scan    
        if (selection == 'E'){
     
            monster_sum = (rand() % 3) + 1;
            for(i = 0; i < monster_sum; i++){
                temp = (rand() % 2) + 1; 
                monster_hp[i] = temp + '\0';
            }
            
        }
        else if(selection == 'M'){
        
            monster_sum = (rand() % 4) + 1;
            for (i = 0; i < monster_sum; i++){
                temp = (rand() % 3) + 4;
                monster_hp[i] = temp + '\0';
            }
        }
        else if(selection == 'H'){
          
            monster_sum = (rand() % 6) + 1;
            for(i = 0; i < monster_sum; i++){
                temp = (rand() % 3) + 7;  
                monster_hp[i] = temp;         
            }
        } 
    }
}

char** board(){
    int i, j;
    char** map = (char**)malloc(sizeof(char*) * N);
    if(map == NULL){
        printf("error allocating memory!\n");
        return NULL;
    }
    for (i = 0; i < N; i++){
        map[i] = (char*)malloc(sizeof(char) * M);
        if (map[i] == NULL){
            printf("error allocating memory!\n");
            for(j = 0; j < i; j++) {
                free(map[j]);}
            free(map);
            break;
        }
        for (j = 0; j < M; j++){
            map[i][j] = '.';
        }
    }
    return map;
    
}

void print_board(char **board) {   
    int i, j;
    printf("  ");
    for (i = 0; i < M; i++){ 
        printf("%d ", i + 1);
    } //alfabhta
    printf("\n ");
    for (i = 0; i < M; i++){
        printf("--");   
    }//orizontia sthlh katw apo alfabhta   
    printf("-\n");

    for (i = 0; i < N; i++) {
        printf("%d|", i + 1);  
        for (j = 0; j < M; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
    /*for(int i = 0; i < max_monsters_spawned; i++)
        monster_hp[i] = '\0';*/            //de kserw ama einai aparaithto lgka oxi
}

void free_board(char** board){
    int i;
    for (i = 0; i < N; i++) {
        free(board[i]);
    }
    free(board);
}

void place_players(char **board, char hselect[], Hero heroes[]) {
    int i, j, placed = 0;

    while (placed < 4 && hselect[placed] != '\0') { 
        i = rand() % N;
        j = rand() % M;

        if (board[i][j] == '.') { 
            board[i][j] = hselect[placed]; 
            heroes[placed].id = hselect[placed]; 
            heroes[placed].x = i; 
            heroes[placed].y = j;
            placed++;
        }
    }
}
    
void place_cosmetic(char **board) { 
    int placed = 0, i, j;
    while (placed < 1) {
         i = rand() % N;
         j = rand() % M;

        if (board[i][j] == '.') {
            board[i][j] = '@';
            placed++;
        }
    }
}

void place_monsters(char **board) {
    int placed = 0, i, j;
    while (placed < monster_sum) {
         i = rand() % N;
         j = rand() % M;

        if (board[i][j] == '.') {    
            board[i][j] = monster_hp[placed] + '0';
            placed++;
        }
    }
}

void game_cmnds(char **board, Hero heroes[]){
    char input = '\0';        //epilogh hero
    printf("\nchoose a hero /continue game /exit\n\ncommands:\n > ");
    scanf(" %c", &input);               
    exodos(board, &input, '\0');    //exodos kai end_round barane elenxous sto input tou user 'X' / 'O'
    if (end_round(board, &input, '\0') == 1) return;

    int valid = 0;     
    int h = -1, i, j;     
                         
    for (i = 0; i < 4; i++) {//elenxos ama edwse swsto hero
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
    exodos(board, movement, h);     //to idio kai edw

    if (end_round(board, &input, '\0') == 1) return;

    if(movement[0] == 'A' && movement[1] == '*'){
        attack(board, &heroes[h]);
        return;
    }

    i = 0;
    int total_steps = 0;
    int cur_x = heroes[h].x;
    int cur_y = heroes[h].y;
    int old_x = cur_x;
    int old_y = cur_y;
    int check = 0;

    
    while(movement[i] != '\0' && total_steps < heroes[h].moves){
        char direction = movement[i];
        if (!valid_direction(movement[i])){
            printf("Invalid direction\n");
            return;
        }
        i++;

        int steps = 0;
        while(isdigit(movement[i])){
            steps = steps * 10 + (movement[i] - '0'); 
            i++;
        }
        if(steps == 0) steps = 1;
        for (j = 0; j < steps && total_steps < heroes[h].moves; j++){
            int new_x = cur_x;
            int new_y = cur_y;
            mvmnt(&new_x, &new_y, direction, 1); //thelei 1 oxi steps gt baraeu oses fores einai ta steps
            if(check_move(board, new_x, new_y)) return;
            board[cur_x][cur_y] = '.';
            cur_x = new_x;
            cur_y = new_y;
            board[cur_x][cur_y] = heroes[h].id;
            total_steps++;
        }
    }
    
    heroes[h].x = cur_x;
    heroes[h].y = cur_y;
    
    print_board(board);
}   