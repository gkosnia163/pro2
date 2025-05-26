#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct wList 
{ 
    char *word;      // the stored word 
    //int wordLength;  // length of the word in letters 
    //int uniqLetters; // number of unique word letters 
    //int numOfTimes;  // no of times a word appears in the text 
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
            scanf("%s", input);
            insert(&head, input, 1);
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

void insert(wl **wlPtr, char* input, int insertionMode){
    
    wl *newnode = (wl*)malloc(sizeof(wl));

    if(newnode != NULL) {
        newnode->word = strdup(input); //duplicate
        newnode->next = NULL;
        newnode->prev = NULL;
        newnode->insertionMode = 1;

        if(wlPtr == NULL){
            *wlPtr = newnode;
            return;
        }

        wl *current = *wlPtr;
        wl *prev = NULL;

        while(current != NULL && strcmp(input, current->word) > 0){
            prev = current;
            current = current->next;
        }

        newnode->next = current;
        newnode->prev = prev;

        if(prev == NULL) *wlPtr = newnode;
        else prev->next = newnode;
        if(current != NULL)
            current->prev = newnode;
    }else{
        printf( "%c not inserted. No memory available.\n", input );
        return;
    }
}

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


