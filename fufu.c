#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER 256

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

typedef wordList wl;      
typedef wordList *wlPtr; 

static void insert(wordList **hdPtr, wl **tlptr, const char* input, int insertionMode); //head pointer, tail pointer
static void delete(wordList **hdPtr, wl** tlptr, const char* input);
static void rm_newline_space(char *x);
static int elenxos_input(char* input);
static void printList(wl *head);
static void showfwd(wl *head, int num);
static void nuke_stdin(void);
static int wlenght(const char* word);
static int uniql(const char* word);
static void freeL(wl* head);
static void showrev(wl *tail, int num);

int main()
{
    wl *head = NULL;
    wl* tail = NULL;
    char *input = malloc(BUFFER);
    if (input == NULL) return 1;
    
    while(1){
    printf("\ncmds: insert / delete / exit / showfwd / showrev\n$>");
    if(!fgets(input, BUFFER, stdin)) break; //+elenxos
   

    switch(elenxos_input(input)){
        case 1:{
            rm_newline_space(input);
            if (strncmp(input, "insert: ", 8) == 0){ //periptwsh na grafei o user insert:\n
                    insert(&head, &tail, input + 8, 1);
            } else {
                printf("$>insert: ");
                if (fgets(input, BUFFER, stdin)){
                    rm_newline_space(input);
                    insert(&head, &tail, input, 1);
                }
            }
            printList(head);
            break;
        }
        case 2:// exit case
            printf("\nexiting\n\n");
            printList(head);
            free(input);
            freeL(head);
            return 0;
        
        case 3: {// delete case
            rm_newline_space(input);
            if (strncmp(input, "delete: ", 8) == 0)
                delete(&head, &tail, input + 8);
            else{
                printf("$>delete: ");
                if (fgets(input, BUFFER, stdin)){
                    rm_newline_space(input);
                    delete(&head, &tail, input);
                }   
            }                
            printList(head);
            break;
        }

        case 4: {
                int num;
                if (sscanf(input, "showfwd: %d", &num) == 1) {
                    showfwd(head, num);
                } else {
                    printf("wrong input\n");
                }
                break;
        }
    
        case 5:{
                int num;
                if (sscanf(input, "showrev: %d", &num) == 1) {
                    showrev(tail, num);
                } else {
                    printf("wrong input\n");
                }
                break;
        }

        default: printf("wrong input lalh\n"); printList(head);
        }
    }       
    free(input);
    freeL(head);
    return 0;
}

static int elenxos_input(char* input){     //yparxoyn 2 if gia kathe case etsi wste o user na epilegei ama thelei na dialegei to cmd kai meta na   // balei epiprostheto input, dld na mporei na grapsei "insert\n" kai meta dedomena oxi me th mia(ama thelei) 
    rm_newline_space(input);
    if(!strncmp(input,"insert", 6)) return 1;
    if(!strcmp(input,"exit")) return 2;
    if(!strncmp(input,"delete", 6)) return 3;
    if(!strncmp(input,"showfwd:", 8)) return 4;
    if(!strncmp(input,"showrev:", 8)) return 5;
    
    return 0;
}

static void rm_newline_space(char* x){ //remove newline space
    int len = strlen(x);
    while(len > 0 && (x[len-1] == '\n' || x[len-1] == ' ')){
        x[len-1] = '\0';
        len--;
    }
}

static int notletter(char c) {
    return  c == ' ' || c == '\r' || c == '\0' || c == '\n'||
            c == ',' || c == '.' || c == '!' || c == '?'||
            c == ';' || c == ':';
}

static void insert(wl **hdPtr, wl **tlptr, const char* input, int insertionMode){
    if(!input|| strlen(input) == 0) return;

    int i = 0, start = 0, end = 0;
    int len = strlen(input);

    while(end <= len){
        if(notletter(input[end]) || end == len){ 
            if(start < end){
                
                char cleaned[BUFFER] = {0}; //megalyterh leksh ths ellhnikhs pali, de xreiazetai malloc
                int j = 0;
                for(i = start; i < end; i++){ //afhnei mono grammata
                    if(isalpha((unsigned char)input[i])){
                        cleaned[j++] = toupper((unsigned char)input[i]);
                    }
                }
                cleaned[j] = '\0'; ////////
            
                if(j>0){
                    wl *current = *hdPtr;
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
                            if(!newnode->word){ free(newnode); return;} //se peiptwsh p apotuxei strdup
                            newnode->next = NULL;
                            newnode->prev = NULL;
                            newnode->insertionMode = insertionMode;
                            newnode->wordLength = wlenght(newnode->word); //savearete swsta
                            newnode->uniqLetters = uniql(newnode->word); 
                            newnode->numOfTimes = 1;
                    printf("\nword lenght:%d\n", newnode->wordLength);
                    printf("%d unique letters\n", newnode->uniqLetters);
                    printf("insertion mode %d\n", newnode->insertionMode);
                            current = *hdPtr;
                            wl *prev = NULL;

                            while(current != NULL && strcmp(cleaned, current->word) > 0){ //compare 'input' me current 'word' na exei thetiko arithmo 
                                prev = current;                                         //dld na nai megalytero to value tou input se ascii value 
                                current = current->next;                                //alfabhtikh taksinomish
                            }

                            newnode->next = current;
                            newnode->prev = prev;

                            if(prev == NULL) 
                                *hdPtr = newnode;
                            else prev->next = newnode;
        
                            if(current != NULL) 
                                current->prev = newnode;
                            
                            if(newnode->next == NULL){
                                *tlptr = newnode;
                            }

                        } else printf( "word not inserted. No memory available.\n");
                    }
                }
            }
            start = end + 1;
        }
        end++;
    }    
}

static void delete(wl **hdPtr, wl **tlptr, const char* input){
    if (!input|| strlen(input) == 0) return; // elenxos kenou
    char buf[BUFFER];
    strncpy(buf, input, BUFFER-1); buf[BUFFER-1] = '\0';
    int i,j;
    char* token = strtok(buf, " "); //delete, kanei tokenize kathe katharh leksh
    while(token != NULL){
        char cleaned[172] = {0}; // megalyterh leksh ths ellhnikhs
        j = 0;
        for(i = 0; token[i] != '\0'; i++){
            if(isalpha((unsigned char)token[i])){
                cleaned[j++] = toupper((unsigned char)token[i]);
            }
        }
        cleaned[j] = '\0';
        if(j == 0){
            token = strtok(NULL, " ");
            continue;
        }
    
        wl* current = *hdPtr;
        while(current != NULL && strcmp(current->word, cleaned) != 0){
            current = current->next;
        }
        if(current == NULL) printf("word not found, not inputed properly\n");
        else{
            printf("word %s, removed succesfully\n", cleaned);
            if(current->prev != NULL) current->prev->next = current->next;
            else *hdPtr = current->next;
            if(current->next != NULL) current->next->prev = current->prev;
            else *tlptr = current->prev;
            free(current->word);
            free(current);
        }
        token = strtok(NULL, " ");
    }   
}

static void showfwd(wl *head, int num) {
    wl *current = head;
    int count = 0;

    while (current != NULL && count < num) {
        /* εκτύπωσε τη λέξη */
        printf("%s", current->word);
        count++;
        /* εάν δεν είμαστε στο τελευταίο στοιχείο που θα εκτυπώσουμε, πρόσθεσε το “ -> ” */
        if (current->next != NULL && count < num) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

static void showrev(wl *tail, int num) {
    wl *current = tail;
    int count = 0;

    while (current != NULL && count < num) {
        /* εκτύπωσε τη λέξη */
        printf("%s", current->word);
        count++;
        /* εάν δεν είμαστε στο τελευταίο στοιχείο που θα εκτυπώσουμε, πρόσθεσε το “ -> ” */
        if (current->prev != NULL && count < num) {
            printf(" -> ");
        }
        current = current->prev;
    }
    printf("\n");
}

static void nuke_stdin(){
    int c;
    while((c = getchar() != '\n' && c != EOF)); //apo sscanf 
}

static int wlenght(const char* word)
{
    int n = 0;
    for (int i=0; word[i]; ++i)
        if (isalpha((unsigned char)word[i])) ++n;
    return n;
}

static int uniql(const char *word)
{
    int seen[26]={0}, count=0;
    for (int i=0; word[i]; ++i) {
        char c = word[i];
        if (c>='A' && c<='Z' && !seen[c-'A']) {
            seen[c-'A']=1;
            ++count;
        }
    }
    return count;
}          
       //baraei tis diplasies fores diwrthwse!!!! 

static void printList(wl *head){
    while(head != NULL){
        printf("%s -> ", head->word);
        head = head->next;
    }
    printf("\n");
}

static void freeL(wl* head){
    int i;
    while(head){
        wl *next=head->next;
        free(head->word);
        free(head);
        head=next;
    }
}
