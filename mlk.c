#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char** board(int N, int M);
void print_board(char** board, int N, int M);
void free_board(char** board, int N);
void select_classes(int kar);
void select_difficulty(int monster_sum, int monster_hp, int count);
void place_players(char **board,char select[], int kar, int N, int M);
void place_cosmetics(char **board, int count, int N, int M);
void place_monsters(char **board, int monster_sum, int N, int M);
int monster_sum ,monster_hp, kar = 0;
char select[5];
int count= 0; //count gia cosmetics

int main() {
    srand(time(NULL));
    int N = 7, M =5; 
    char** map;
    select_classes(kar);
    select_difficulty(monster_sum,monster_hp,count);
    map = board(N, M); //desmeyei mnhnmh board[i][j]
    if (map != NULL){
        place_players(map, select, kar, N, M);
        place_cosmetics(map, count, N, M);
        place_monsters(map, monster_sum, N, M);
        print_board(map, N, M);
        free_board(map,N);
    }
    return 0;
}

int is_valid_hero(char c) {
    return (c == 'b' || c == 'd' || c == 'e' || c == 'm');
}

void select_classes(int kar) {
    select[5] = '\0'; //classes
    int h_sum = 4; //limit gia to posoi paiktes yparxoyn sto tamplo 
    printf("Select up to 4 heroes (barbarian 'b', dwarf 'd', elf 'e', mage 'm'):\ntype - to select difficulty\n");
    for (int i = 0; i < h_sum; i++) {
        char input;
        
        while (1) {
            printf("Hero %d: ", i + 1);
            scanf(" %c", &input);
            if (input == '-' && select[i] != select[0]) { //gamaw
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
                kar++;
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

void select_difficulty(int monster_sum, int monster_hp, int count) {
    char select;
    printf("Select difficulty:\nEasy: E\nModerate: M\nHard: H\n");
    while (select != 'E' && select != 'M' && select != 'H') {
        select = getchar(); //while baraei mono scan    
        if (select == 'E'){
            monster_hp = (rand() % 3) + 1;
            monster_sum = (rand() % 2) + 1;
            count=2;
        }
        else if(select == 'M'){
            monster_hp = (rand() % 4) + 1;
            monster_sum = (rand() % 3) + 6;
            count=4;
        }
        else if(select == 'H'){
            monster_hp = (rand() % 6) + 1;
            monster_sum = (rand() % 3) + 9;
            count=6;
        } 
    }
}

char** board(int N, int M){
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

void print_board(char **board, int N, int M) {   
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
}

void free_board(char** board, int N){
    for (int i = 0; i < N; i++) {
        free(board[i]);
    }
    free(board);
}

void place_players(char **board,char select[],int kar, int N, int M){
    int placed = 0,i = 0;
    char* term;

    while (i < select[placed]){
        int i = rand() % N;
        int j = rand() % M;

        if (board[i][j] == '.'){
            term = &select[placed];
            board[i][j] = *term;
            placed++;
        }
    }  
}
    
void place_cosmetics(char **board, int count, int N, int M) {
    int placed = 0;
    while (placed < count) {
        int i = rand() % N;
        int j = rand() % M;

        if (board[i][j] == '.') {
            board[i][j] = '@';
            placed++;
        }
    }
}

void place_monsters(char **board, int monster_sum, int N, int M) {
    int placed = 0;
    while (placed < monster_sum) {
        int i = rand() % N;
        int j = rand() % M;

        if (board[i][j] == '.') {
            board[i][j] = '1' + (rand() % 9); // Αριθμοί 1-9
            placed++;
        }
    }
}

/*fanculo policia frattelo
gang way or no way*/