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

//wordList *createNode(char* input);
void insert(wordList **wlPtr, char* input, int insertionMode);
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
    scanf("%s", input);

    switch(elenxos_input(input)){
        case 1:
            printf("$> insert ");
            while(getchar() != '\n');
                if (fgets(input, 100, stdin) != NULL){
                    input[strcspn(input, "\n")] = '\0';
                    insert(&head, input, 1);
                } else printf("error read\n");
            printList(head);
            break;
        case 2:
            printf("\nexiting\n\n");
            printList(head);
            free(input);
            return 0;
        case 3:
            printf("2");
            break;
        default:
            printf("wrong input lalh\n");
    }
}

    free(input);
    return 0;
}

int elenxos_input(char* input){
    if(strcmp(input, "insert") == 0) return 1; //to strstr prepei na gyrnaei NULL se false  
    else if (strcmp(input, "exit") == 0) return 2;
    else if (strcmp(input, "delete") == 0) return 3;    
    else return 0;
}

int notletter(char c) {
    return  c == ' ' || c == '\r' || c == '\0' ||
            c == ',' || c == '.' || c == '!' || c == '?'||
            c == ';' || c == ':';
}

void insert(wl **wlPtr, char* input, int insertionMode){
    if(input == NULL || strlen(input) == 0) return;//ftiakse elenxo swsta ama input einai enter sketo

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
                                prev = current;                                         //dld na nai megalytero to value tou input se ascii plhktrologio 
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



/*
wordList *createNode(char* input){
    wordList *new_node = (wordList*)malloc(sizeof(wordList));
    if (new_node == NULL){
        printf("failed\n");
        return NULL;
    }
    new_node->word = input; //malloc kai antigrafh
    if (new_node->word == NULL){
        free(new_node);
        printf("failed strdup\n");
        return NULL;
    }

    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}*/


