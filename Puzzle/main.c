#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

typedef struct
{
    char *word;
    char *clue;
    int x;
    int y;
    char direction;
    int f;
}Word_t;

Word_t* loadTextFile(FILE* myFile, int nrWords);
void displayBoard(int rows, int cols, char** myBoard);
int isBoardFilled(int rows, int cols, char** myBoard);
char** createArray(int rows, int cols);
int findIndex(int arr[], int size, int val);
void playGame(char** myBoard, int nrWords, Word_t *words, int x, int y, int countToFinisih);
char** updateBoard(char** myBoard, Word_t *words, int solve);

int main()
{
    char** myBoard = NULL;
    FILE* myFile = NULL;
    int rows;
    int cols;
    int nrWords;
    Word_t* words = NULL;
    int countToFinish = 0;

    char fileName[256];
    scanf("%s",fileName);

    myFile = fopen(fileName,"r");

    if(myFile == NULL){
        printf("File open error \n");
        return 0;
    }
    int singleLine[256];

    fgets(singleLine,sizeof(singleLine),myFile);
    char* piece = strtok(singleLine," ");
    rows = atoi(piece);
    piece = strtok(NULL," ");
    cols = atoi(piece);
    piece = strtok(NULL,"\0");
    nrWords = atoi(piece);
    printf("Game is %d rows x %d cols with %d words\n", rows, cols, nrWords);

    words = (Word_t*)malloc(sizeof(Word_t)*nrWords);
    words = loadTextFile(myFile, nrWords);
    fclose(myFile);
    myBoard = createArray(rows, cols);
    playGame(myBoard, nrWords, words, rows, cols, countToFinish);

    free(words);
    free(myBoard);
    return 0;
}

char** createArray(int rows, int cols) {
    char **myArray = NULL;

    myArray = malloc(sizeof(int*)*rows);

    if (myArray == NULL) {
        printf("FATAL ERROR: out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0;i < rows;i++) {
        myArray[i] = malloc(sizeof(int)*cols);
        if (myArray[i] == NULL) {
            printf("FATAL ERROR: out of memory: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0;i < rows;i++){
        for(int j = 0;j < cols;j++){
            myArray[i][j] = '#';
        }
    }
    return myArray;
}

void displayBoard(int rows, int cols, char** myBoard){
    printf("Current Puzzle: \n");
    printf("\n\t  ");
    for(int i = 0;i < cols;i++){
        printf("%d  ", i+1);
    }
    printf("\n\t  ");
    for(int i = 0;i < cols;i++){
        printf("-- ");
    }
    printf("\n");
    for(int i = 0;i < rows;i++){
        printf("%d\t|",i+1);
        for(int j = 0;j < cols;j++){
            printf(" %c ", myBoard[i][j]);
        }
        printf("\n");
    }
    printf("\t");
    for(int i = 0;i < cols;i++){
        printf("***");
    }
}

char** updateBoard(char** myBoard, Word_t * words, int solve){
    int x;
    int y;
    y = words[solve].x-1;
    x = words[solve].y-1;
    for(int i = 0;i < strlen(words[solve].word);i++){
        if(words[solve].f == 1){
            myBoard[y][x] = words[solve].word[i];
        }
        else if(words[solve].f == 0){
            myBoard[y][x] = '_';
        }
        if(words[solve].direction == 'V'){
            y++;
        }
        else if(words[solve].direction == 'H'){
            x++;
        }
    }
    return myBoard;
}

Word_t* loadTextFile(FILE* myFile, int nrWords){
    printf("Words and clues are read!\n");
    char buffer[1024];
    Word_t* myArray = NULL;
    myArray = (Word_t *)malloc(sizeof(Word_t)*nrWords);
    int i;

    for(i = 0;i < nrWords;i++){
        myArray[i].word = (char *)malloc(sizeof(char *) * 256);
        myArray[i].clue = (char *)malloc(sizeof(char *) * 256);
        fscanf(myFile, "%c %d %d ", &myArray[i].direction, &myArray[i].x, &myArray[i].y);
        int j = 0;
        do
            fscanf(myFile, "%c", &buffer[j]);
        while(buffer[j++] != '\n');
        buffer[j-1] = 0;
        j--;
        int k;
        for(k = 0; k < j && buffer[k] != ' ';k++){
            myArray[i].word[k] = buffer[k];
        }
        myArray[i].word[k] = 0;
        k++;
        int l = 0;
        for(;k < j;k++){
            myArray[i].clue[l++] = buffer[k];
        }
        myArray[i].clue[l] = 0;
        myArray[i].f = 0;
    }

    return myArray;
}

int isBoardFilled(int rows, int cols, char** myBoard){
    for(int i = 0;i < rows;i++){
        for(int j = 0;j < cols;j++){
            if(myBoard[i][j] == '_') return 0;
        }
    }
    return 1;
}

int findIndex(int arr[], int size, int val){
    if(val == 0) return 0;
    for(int i = 0;i < size;i++){
        if(arr[i] == val) return 1;
    }
    return 0;
}

void playGame(char** myBoard, int nrWords, Word_t *words, int rows, int cols, int countToFinish){
    char direction[256];
    int check[nrWords];
    int solved[nrWords];
    int solve;
    char solveChar[256];
    char answer[256];

    for(int i = 0;i < nrWords;i++){
        check[i] = i+1;
        solved[i] = 0;
    }

    for(int i = 0;i < nrWords;i++){
        myBoard = updateBoard(myBoard, words, i);
    }
    displayBoard(rows, cols, myBoard);

    do{
        printf("\n\nAsk for hint:");
        printf("\n#  Direction\t row  col");
        printf("\n----------------------------");

        for(int i = 0;i < nrWords;i++){
            if(words[i].direction == 'H'){
                strcpy(direction, "Horizontal");
            }
            else if(words[i].direction == 'V'){
                strcpy(direction, "Vertical");
            }
            if(findIndex(check, nrWords, i+1) == 1){
                printf("\n%d: %s\t  %d    %d", i+1, direction, words[i].x, words[i].y);
            }
        }
        printf("\n");
        do{
            printf("\nEnter -1 to exit");
            printf("\nWhich word to solve next?: ");
            scanf("%s",&solveChar);
            solve = atoi(solveChar);
            if(solve == 0) continue;
            if(solve == -1) return;
            if(solve < 1){
                solve = 0;
            }
            if(findIndex(solved, nrWords, solve) == 1){
                printf("That word has been solved before");
                solve = 0;
            }
            else if(findIndex(check, nrWords, solve) == 0){
                printf("Invalid input");
                solve = 0;
            }
        }while(solve < 1 || solve > nrWords);

        printf("Current hint: %s",words[solve-1].clue);
        printf("\nEnter your solution: ");
        scanf("%s",answer);

        for(int i = 0;answer[i];i++){
            answer[i] = toupper(answer[i]);
        }

        if(strcmp(answer,words[solve-1].word) != 0){
            printf("WRONG ANSWER\n");
            displayBoard(rows, cols, myBoard);
        }else{
            printf("\nCorrect!\n\n");
            solved[solve-1] = solve;
            check[solve-1] = 0;
            words[solve-1].f = 1;
            myBoard = updateBoard(myBoard, words, solve-1);
            displayBoard(rows, cols, myBoard);
            countToFinish++;
        }
        if(isBoardFilled(rows, cols, myBoard) == 1) break;
    }while(countToFinish != nrWords);
    printf("\nCongratulations! You beat the puzzle!");

}
