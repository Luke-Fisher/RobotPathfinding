#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "input_error.h"

typedef struct{
    int row;
    int column;
}coord;

typedef struct queue{
    coord space;
    struct queue* next;
}queue;

typedef struct{
    int row;
    int col;
    coord start;
    coord f1;
    coord s2;
    coord f2;
    int** array;
}maze;

maze readMaze(FILE*,maze, int row, int column);
int rowLength(FILE* fp);
int columnLength(FILE* fp);
void BFS(maze room, FILE* fp, coord start, coord finish, char smarker, char fmarker);
void enqueue(coord new, queue** Qhead, queue** Qtail);
coord dequeue(queue** Qhead, queue** Qtail);
void printPath(maze room, coord**, coord start, coord finish, char smarker, char fmarker, FILE* fp);
maze mallocMaze(maze);
void freeMaze(maze);
void isFileEmpty(FILE* fp);

int main(int argc, char** argv) {
    if (argc != 3){
        exit(INCORRECT_NUMBER_OF_COMMAND_LINE_ARGUMENTS);
    }
    
    FILE* infp = fopen(argv[1], "r");
    if(infp == NULL){
        exit(INPUT_FILE_FAILED_TO_OPEN);
    }
    isFileEmpty(infp);
    
    int row;
    int column;
    
    //*******************************************************************BEGIN READING MAZE FROM FILE************************************************************************
    column = rowLength(infp);
    row = columnLength(infp);
    
    maze Maze; 
    Maze.row = row;
    Maze.col = column;
    Maze = mallocMaze(Maze);
    Maze = readMaze(infp, Maze, row, column);
    
    if(fclose(infp) == EOF){
        freeMaze(Maze);
        exit(INPUT_FILE_FAILED_TO_CLOSE);
    }
    
    FILE* outfp = fopen(argv[2], "w");
    if(outfp == NULL){
        freeMaze(Maze);
        exit(OUTPUT_FILE_FAILED_TO_OPEN);
    }
    
    //******************************************************************RUN BFS ON MAZE*********************************************************************************
    BFS(Maze, outfp, Maze.start, Maze.f1, 'S', 'E');
    BFS(Maze, outfp, Maze.s2, Maze.f2, 'F', 'L');
    freeMaze(Maze);

    if(fclose(outfp) == EOF){
        exit(OUTPUT_FILE_FAILED_TO_CLOSE);
    }
    return 0;
}

/*Deallocate space for the maze*/
void freeMaze(maze Maze){
    int i;
    
    for( i = 0; i < Maze.row; i++){
            free(Maze.array[i]);
    }
       
    free(Maze.array);
    
}

/*Allocate space for the maze*/
maze mallocMaze(maze Maze){
    int i;
    int j;
    
    Maze.array = malloc(sizeof(int*) * Maze.row);

    
    for(i = 0; i < Maze.row; i++){
        Maze.array[i] = malloc(sizeof(int) * Maze.col);

    }
    
    
    for(i = 0; i < Maze.row; i++){
        for(j = 0; j < Maze.col; j++){
            Maze.array[i][j] = 1;

        }
    }
    
    return Maze;
}
/*This BFS is the main function of our program. The graph is runs on is represented 
 * as a binary maze. As the BFS runs, an array of coordinates is maintained where 
 * each index holds the coordinates of the previous space the BFS visited for the 
 * sake of path memory.*/
void BFS(maze room, FILE* fp, coord start, coord finish, char smarker, char fmarker){
    int** visited;
    coord** pred; //predecessor array
    int i, j, r, c, finflag = 0;
    coord currspace, temp; //temp is used to pass coordinates to the queue
    queue* Qhead = NULL;
    queue* Qtail = NULL;
    
    pred = malloc(sizeof(coord*) * room.row);
    for(i = 0; i < room.row; i++){
        pred[i] = malloc(sizeof(coord) * room.col);
    }
    
    visited = malloc(sizeof(int*) * room.row);
    for(i = 0; i < room.row; i++){
        visited[i] = malloc(sizeof(int) * room.col);
    }
    
    //initialize all spaces as unvisited
    for(i = 0; i < room.row; i++){
        for(j = 0; j < room.col; j++){
            visited[i][j] = 0;
        }
    }
     //initialize the queue
    enqueue(start, &Qhead, &Qtail);
    visited[start.row][start.column] = 1;
    pred[start.row][start.column].row = start.row;
    pred[start.row][start.column].column = start.column;
    
    //begin BFS
    while(Qhead != NULL){
        currspace = dequeue(&Qhead, &Qtail);
        r = currspace.row;
        c = currspace.column;
        
        if(currspace.row == finish.row && currspace.column == finish.column){
            finflag = 1;
        }
        
        /*Check every space adjacent to the current space. If a space hasn't 
         * been visited and isn't a wall, mark as visited and add enqueue it.*/
        if(visited[r+1][c] == 0 && room.array[r+1][c] == 1){
            visited[r+1][c] = 1;
            pred[r+1][c].row = r;
            pred[r+1][c].column = c;
            temp.row = r+1;
            temp.column = c;
            enqueue(temp, &Qhead, &Qtail);
        }
        if(visited[r-1][c] == 0 && room.array[r-1][c] == 1){
            visited[r-1][c] = 1;
            pred[r-1][c].row = r;
            pred[r-1][c].column = c;
            temp.row = r-1;
            temp.column = c;
            enqueue(temp, &Qhead, &Qtail);
        }
        if(visited[r][c+1] == 0 && room.array[r][c+1] == 1){
            visited[r][c+1] = 1;
            pred[r][c+1].row = r;
            pred[r][c+1].column = c;
            temp.row = r;
            temp.column = c+1;
            enqueue(temp, &Qhead, &Qtail);
        }
        if(visited[r][c-1] == 0 && room.array[r][c-1] == 1){
            visited[r][c-1] = 1;
            pred[r][c-1].row = r;
            pred[r][c-1].column = c;
            temp.row = r;
            temp.column = c-1;
            enqueue(temp, &Qhead, &Qtail);
        }
        /*Removing the next four if statements might allow for diagonal walls by
         switching from Euclidean path finding to Manhattan path finding*/
        if(visited[r+1][c+1] == 0 && room.array[r+1][c+1] == 1){
            visited[r+1][c+1] = 1;
            pred[r+1][c+1].row = r;
            pred[r+1][c+1].column = c;
            temp.row = r+1;
            temp.column = c+1;
            enqueue(temp, &Qhead, &Qtail);
        }
        if(visited[r+1][c-1] == 0 && room.array[r+1][c-1] == 1){
            visited[r+1][c-1] = 1;
            pred[r+1][c-1].row = r;
            pred[r+1][c-1].column = c;
            temp.row = r+1;
            temp.column = c-1;
            enqueue(temp, &Qhead, &Qtail);
        }
        if(visited[r-1][c+1] == 0 && room.array[r-1][c+1] == 1){
            visited[r-1][c+1] = 1;
            pred[r-1][c+1].row = r;
            pred[r-1][c+1].column = c;
            temp.row = r-1;
            temp.column = c+1;
            enqueue(temp, &Qhead, &Qtail);
        }
        if(visited[r-1][c-1] == 0 && room.array[r-1][c-1] == 1){
            visited[r-1][c-1] = 1;
            pred[r-1][c-1].row = r;
            pred[r-1][c-1].column = c;
            temp.row = r-1;
            temp.column = c-1;
            enqueue(temp, &Qhead, &Qtail);
        }
         
    }
    
    if(finflag != 1){
        for(i = 0; i < room.row; i++){
            free(pred[i]);
        }
        free(pred);

        for(i = 0; i < room.row; i++){
            free(visited[i]);
        }
        free(visited);
        
        freeMaze(room);
        fclose(fp);
        exit(GOAL_UNREACHABLE);
    }
    
    printPath(room, pred, start, finish, smarker, fmarker, fp);  
    
    for(i = 0; i < room.row; i++){
        free(pred[i]);
    }
    free(pred);

    for(i = 0; i < room.row; i++){
        free(visited[i]);
    }
    free(visited);
    
    return;
}
/*Print path starts at the end finish space of the robot and backtracks through 
 the room until it reaches the start space*/
void printPath(maze room, coord** pred, coord start, coord finish, char smarker, char fmarker, FILE* fp){
    char** path; //graphical representation of the room
    int i, j, temprow;
    coord current = finish;
    
    path = malloc(sizeof(char*) * room.row);
    for(i = 0; i < room.row; i++){
        path[i] = malloc(sizeof(char) * room.col);
    }
    
    //initialize path as a blank room
    for(i = 0; i < room.row; i++){
        for(j = 0; j < room.col; j++){
            if(room.array[i][j] == 1){
                path[i][j] = ' ';
            }
            else{
                path[i][j] = '#';
            }
        }
    }
    
    //backtrack from finish to start, marking each space visited
    while(current.row != start.row || current.column != start.column){  
        path[current.row][current.column] = smarker;
        temprow = current.row;
        current.row = pred[temprow][current.column].row;
        current.column = pred[temprow][current.column].column;   
    }
    path[start.row][start.column] = smarker;
    path[finish.row][finish.column] = fmarker;
    
    for(i = 0; i < room.row; i++){
        for(j = 0; j < room.col; j++){
            fprintf(fp, "%c", path[i][j]);
        }
        fprintf(fp, "\n");
    }
    
    for(i = 0; i < room.row; i++){
        free(path[i]);
    }
    free(path);
    
    return;
}

/*Standard enqueue function*/
void enqueue(coord new, queue** Qhead, queue** Qtail){
    queue* newnode = malloc(sizeof(queue));
    
    newnode->space.row = new.row;
    newnode->space.column = new.column;
    newnode->next = NULL;
    
    if(*Qhead == NULL){
        *Qhead = newnode;
    }
    else{
        (*Qtail)->next = newnode;
    }
    *Qtail = newnode;
    
    return;
}

/*Standard dequeue function*/
coord dequeue(queue** Qhead, queue** Qtail){
    coord value = (*Qhead)->space;
    queue* temp = *Qhead;
    *Qhead = (*Qhead)->next;
    
    if(Qhead == NULL){
        *Qtail = NULL;
    }
    
    free(temp);
    return value;
}

/*Read the maze in from the file as a binary maze, remembering the coordinates of
 the robots and their end goals*/
maze readMaze(FILE* file, maze Maze, int row, int column){
    char c;
    int i = 0, j = 0, fflag = 0, sflag = 0, eflag = 0, lflag = 0;
    
    for(i=0;i<row;i++){
        char line[75] = "";
        fscanf(file, "%[^\n]%*c", line);
        
        for(j=0;j<column;j++){
            c = line[j];
            if (c == 'S'){
                Maze.start.row = i;
                Maze.start.column = j;
                Maze.array[i][j] = 1;
                sflag = 1;
            }

            if (c == 'F'){
                Maze.s2.row = i;
                Maze.s2.column = j;
                Maze.array[i][j] = 1;
                fflag = 1;
            }

            if (c == 'E'){
                Maze.f1.row = i;
                Maze.f1.column = j;
                Maze.array[i][j] = 1; 
                eflag = 1;
            }

            if (c == 'L'){
                Maze.f2.row = i;
                Maze.f2.column = j;
                Maze.array[i][j] = 1;
                lflag = 1;
            }

            if(c == ' '){
                Maze.array[i][j] = 1;
            }

            if(c == '#'){
                Maze.array[i][j] = 0;
            }
            
            if(c != 'S' && c != 'F' && c != 'E' && c != 'L' && c != ' ' && c != '#' && c != 0){
                freeMaze(Maze);
                fclose(file);
                exit(PARSING_ERROR_INVALID_CHARACTER_ENCOUNTERED);
            }
        }
    }
    
    //check if maze has both goals
    if(sflag != 1 || fflag != 1 || eflag != 1 || lflag != 1){
        freeMaze(Maze);
        fclose(file);
        exit(PARSING_ERROR_MISSING_GOALS);
    }
    return Maze;
}

/*Finds the total number of lines in the file to use as the value for the room 
 * array's rows*/
int rowLength(FILE* fp){
    size_t len = 0;
    char* line = NULL;
    int check;
    int check2 = 1;
    
    while(!feof(fp)){
        getline(&line, &len, fp);
        check = strlen(line);
        if(check > check2){
            check2 = check;
        }
    }
    check2 = check2 + 1;
    rewind(fp);
    free(line);
    return check2;
}

/*Finds the maximum number of characters per line to use as the value for the room 
 * array's columns*/
int columnLength(FILE* fp){   
    size_t len = 0;
    char* line = NULL;
    int check2 = 0;
    
    while(!feof(fp)){
        getline(&line, &len, fp);
        
        check2 = check2 + 1;
        
    }
    
    check2 = check2 + 1;
    rewind(fp);
    free(line);
    return check2; 
}

/*Checks if a file is empty*/
void isFileEmpty(FILE* fp){
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    
    if(size == 0){
        fclose(fp);
        exit(PARSING_ERROR_EMPTY_INPUT_FILE);
    }
    
    rewind(fp);
    return;
}