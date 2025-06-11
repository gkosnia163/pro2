#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct wList 
{ 
    char *word;      // the stored word 
    int wordLength;  // length of the word in letters 
    int uniqLetters; // number of unique word letters 
    int numOfTimes;  // no of times a word appears in the text 
    int insertionMode; //0 from file, 1 from keyboard 
    //bale time kai date p paizeis
    struct wList *next; 
    struct wList *prev; 
} wordList;

typedef wordList wl;       //na nai pio syntomo
typedef wordList *wlPtr; 

void insert(wordList **wlPtr, char* input, int insertionMode);
void delete(wordList **wlPtr, char* input);
void rm_newline_space(char *x);
int elenxos_input(char* input);
void printList(wl *head);
int wlenght(char* word);
int uniql(char* word);

int main()
{
    wl *head = NULL;
    char *input = malloc(100 * sizeof(char));
    if (input == NULL) return 1;
    while(1){
    printf("\ncmds: insert / delete / exit\n$>");  
    fgets(input, sizeof(input), stdin);


    switch(elenxos_input(input)){
        case 1:
            if(strcmp(input, "insert") == 0 || strcmp(input, "insert:") == 0){ //periptwsh na grafei o user insert:\n
                printf("$>insert: ");
                if (fgets(input, 100, stdin) != NULL){
                    rm_newline_space(input);
                    insert(&head, input, 1);
                } else printf("error read\n");
            }
            else if (strncmp(input, "insert: ", 8) == 0 && strlen(input) > 8){
                char* usr_input = malloc(100 * sizeof(char)); // user input, einai olo to input ektos to "insert:"
                strcpy(usr_input, input +8);
                insert(&head, usr_input, 1);
                free(usr_input);
            }
            printList(head);
            break;
        case 2:// exit case
            printf("\nexiting\n\n");
            printList(head);
            free(input);
            return 0;
        case 3: // delete case
            if(strcmp(input, "delete") == 0 || strcmp(input, "delete:") == 0){ //periptwsh na grafei o user insert:\n
                printf("$>delete: ");
                if (fgets(input, 100, stdin) != NULL){
                    rm_newline_space(input);
                    delete(&head, input);
                }
            }
            else if (strncmp(input, "delete: ", 8) == 0 && strlen(input) > 8){
                char* usr_input = malloc(100 * sizeof(char)); // user input, einai olo to input ektos to "insert:"
                strcpy(usr_input, input +8);
                delete(&head, usr_input);
                free(usr_input);
            }
            printList(head);
            break;
        default:
            printf("wrong input lalh\n");
    }
}
    free(input);
    return 0;
}

int elenxos_input(char* input){
    rm_newline_space(input);
    if (strcmp(input, "insert") == 0 || strcmp(input, "insert:") == 0) return 1;//gia kapoio logo ama bgaleis ths strcmp "input:" de leitourgei tf
    if (strncmp(input, "insert: ", 8) == 0 && strlen(input) > 8) return 1;
    if (strcmp(input, "exit") == 0) return 2;
    if (strcmp(input, "delete") == 0 || strcmp(input, "delete:") == 0) return 3;
    if (strncmp(input, "delete: ", 8) == 0 && strlen(input) > 8) return 3;
    return 0;
}

void rm_newline_space(char* x){ //remove newline
    int len = strlen(x);
    while(len > 0 && (x[len-1] == '\n' || x[len-1] == ' ')){
        x[len-1] = '\0';
        len--;
    }
}

int notletter(char c) {
    return  c == ' ' || c == '\r' || c == '\0' ||
            c == ',' || c == '.' || c == '!' || c == '?'||
            c == ';' || c == ':';
}

void insert(wl **wlPtr, char* input, int insertionMode){
    if(input == NULL || strlen(input) == 0) return;

    int i = 0, start = 0, end = 0;
    int len = strlen(input);

    while(end <= len){
        if(notletter(input[end])){ 
            if(start < end){
                
                char cleaned[172] = {0}; //megalyterh leksh ths ellhnikhs pali
                int j = 0;
                
                for(i = start; i < end; i++){ //afhnei mono grammata
                    if(isalpha((unsigned char)input[i])){
                        cleaned[j++] = toupper(input[i]);
                    }
                }
                cleaned[j] = '\0'; ////////
            
                if(j>0){
                    wl *current = *wlPtr;
                    while(current != NULL){
                        if(strcmp(cleaned, current -> word) == 0){
                            current->numOfTimes++;   
                            break;
                        }
                        current = current->next;
                    }
                    if(current == NULL){
                        wl *newnode = (wl*)malloc(sizeof(wl));
                        if(newnode != NULL){ 
                            newnode->word = strdup(cleaned); 
                            newnode->next = NULL;
                            newnode->prev = NULL;
                            newnode->insertionMode = insertionMode;
                            newnode->wordLength = wlenght(newnode->word); //savearete swsta
                            newnode->uniqLetters = uniql(newnode->word); 
                            newnode->numOfTimes = 1;
                    printf("\nword lenght:%d\n", newnode->wordLength);
                    printf("%d unique letters\n", newnode->uniqLetters);
                    printf("insertion mode %d\n", newnode->insertionMode);
                            current = *wlPtr;
                            wl *prev = NULL;

                            while(current != NULL && strcmp(cleaned, current->word) > 0){ //compare 'input' me current 'word' na exei thetiko arithmo 
                                prev = current;                                         //dld na nai megalytero to value tou input se ascii value 
                                current = current->next;                                //alfabhtikh taksinomish
                            }

                            newnode->next = current;
                            newnode->prev = prev;

                            if(prev == NULL) 
                                *wlPtr = newnode;
                            else prev->next = newnode;
        
                            if(current != NULL) 
                                current->prev = newnode;

                        } else printf( "word not inserted. No memory available.\n");
                    }
                }
            }
            start = end + 1;
        }
        end++;
    }    
}

void delete(wl **wlPtr, char* input){
    if (input == NULL || strlen(input) == 0) return; // elenxos kenou

    int i,j;
    char* token = strtok(input, " "); //delete, kanei tokenize kathe katharh leksh
    while(token != NULL){
        char cleaned[172] = {0}; // megalyterh leksh ths ellhnikhs
        j = 0;
        for(i = 0; token[i] != '\0'; i++){
            if(isalpha((unsigned char)token[i])){
                cleaned[j++] = toupper(token[i]);
            }
        }
        cleaned[j] = '\0';
        if(j == 0){
            token = strtok(NULL, " ");
            continue;
        }
    
        wl* current = *wlPtr;
        while(current != NULL && strcmp(current->word, cleaned) != 0){
            current = current->next;
        }
        if(current == NULL) printf("word not found, not inputed properly\n");
        else{
            printf("word %s, removed succesfully\n", cleaned);
            if(current->prev != NULL) current->prev->next = current->next;
            else *wlPtr = current->next;
            
            if(current->next != NULL) current->next->prev = current->prev;
            free(current->word);
            free(current);
        }
        token = strtok(NULL, " ");
    }   
}
int wlenght(char *word){
    int i, temp = 0;
    for(i = 0; word[i] != '\0'; i++){
        if (isalpha(word[i]))
            temp++;
    }
    return temp;
}

int uniql(char* word){
    int i, j, count;
    char temp[172] = {"\0"};   //h megalyterh leksh sthn ellhnikh
    for(i = 0; word[i] != '\0'; i++){
        char c = word[i];
        if (c >= 'A' && c <= 'Z'){
            j = c - 'A';
            if (temp[j] == 0){
                temp[j] = 1;
                count++;
            } 
        }
    }
    return count/2; //trap teleiws kanei th doyleia toy 
}                   //baraei tis diplasies fores diwrthwse!!!! 

void printList(wl *head)
{
    while(head != NULL){
        printf("%s -> ", head->word);
        head = head->next;
    }
}
