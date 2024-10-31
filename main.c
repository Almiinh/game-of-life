/* Author : Minh-Hoang HUYNH
 * Date : 6/11/2022
 * Created on : 2/11/2022
 *
 * --------------------------------------------
 * Conway's Game of Life Program using ncurses
 * --------------------------------------------
 *
 * 
 * In this program we create a 2D array called "field" and each coordinates indicates the presence
 * of a "cell".
 * 
 * We could use a user defined structure "cell",(with a status and coordinates) but here it is not
 * absolutely necessary as long as we pass i,j in each function.
 * 
 * We choose only to represent the field with Boolean arrays because it takes less memory than int
 * arrays. (bool is 1 octet instead of 4 for int)
 * 
 * Each cell is going to be designated by only its coordinates (i,j) in a matrix-like notation and
 * the field will return its state:
 *      field[i][j] will return either {Alive : true} or  {Dead : false}
 * --------------------------------------------------------------------------------------------------------------------------------
 */

#include <stdio.h>          
#include <stdlib.h>         /* for malloc */
#include <stdbool.h>        /* Introduce boolean type: 'bool' based on C99 _Bool, 
                            values { true = 1 or anything else, false = 0}, 
                            size of bool is 1 whereas size of int is 4*/
#include <ncurses.h>        /* ncurses library (need compiling option -lncurses)*/
#include <unistd.h>         /* sleep function */
#include <string.h>         /* strlen function*/

int getAliveNeighborNumber();
bool updateCell();
bool** createArray();
void initArray();
int main();



int getAliveNeighborNumber(bool** array, int i, int j){ 
    // returns the number of alive neighbors by scanning the 8 neighbors
    // taking into account the extreme borders and returns the counter.    
    int count = 0;

    if (i > 0) {                    //if not on top border
        if (j > 0)                  //if not on left border
            if(array[i-1][j-1])
                count++;
        if (j < COLS-1)             //if not on right border
            if(array[i-1][j+1])
                count++;
        if(array[i-1][j])
            count++;
    }
    if (i < LINES-1) {              //if not on bottom border
        if (j > 0)                  //if not on left border
            if(array[i+1][j-1])
                count++;
        if(array[i+1][j])
            count++;
        if (j < COLS-1)             //if not on right border
            if(array[i+1][j+1])
                count++;
    }
    if (j > 0)                      //if not on left border
        if(array[i][j-1])
            count++;
    if (j < COLS-1)                 //if not on right border
        if(array[i][j+1])
            count++;
    return count;
}


bool updateCell(bool** field, int i, int j){    
    int neighborAlive = getAliveNeighborNumber(field, i, j);
    bool cellAlive = field[i][j];
	if(cellAlive && (neighborAlive == 2 || neighborAlive == 3))
        return true;
    else if(!cellAlive && neighborAlive == 3)
        return true;
    return false;
}


//  Create a 2D array [i, j] (matrix notation) from 0..LINES-1 and 0..COLS-1 filled with 'false'
bool** createArray(int y_size, int x_size){
    bool ** array = (bool **) malloc(y_size * sizeof(bool*));       // bool** = array of bool*
    for (int i = 0; i < y_size; i++) {
        array[i] = (bool *) malloc(x_size * sizeof(bool));          // bool* = array of bool
        for (int j = 0; j < x_size; j++)
            array[i][j] = false;
    }
    return array;
}

/* Initiate the field by placing the start pattern in the middle
 * The start pattern are represented graphically by lists of 1 and 0 then transformed.
 */
void initArray(bool** array){

    // R-pentomino 
    int l1[5] = {0,0,0,0,0};
    int l2[5] = {0,0,1,1,0};
    int l3[5] = {0,1,1,0,0};
    int l4[5] = {0,0,1,0,0};
    int l5[5] = {0,0,0,0,0};

    // Diehard 
    // int l1[11] = {0,0,0,0,0,0,0,0,0,0,0};
    // int l2[11] = {0,0,0,0,0,0,0,0,1,0,0};
    // int l3[11] = {0,1,1,0,0,0,0,0,0,0,0};
    // int l4[11] = {0,0,1,0,0,0,0,1,1,1,0};
    // int l5[11] = {0,0,0,0,0,0,0,0,0,0,0};

    // Acorn
    // int l1[9] = {0,0,0,0,0,0,0,0,0};
    // int l2[9] = {0,0,1,0,0,0,0,0,0};
    // int l3[9] = {0,0,0,0,1,0,0,0,0};
    // int l4[9] = {0,1,1,0,0,1,1,1,0};
    // int l5[9] = {0,0,0,0,0,0,0,0,0};

    /* 
     * This part transforms the lists into points
     * in order to display it at the center of the field
     */
    int* grid[5] = {l1,l2,l3,l4,l5};
    int k = sizeof(grid)/sizeof(grid[0]); 
    int l = sizeof(l1)/sizeof(l1[0]);
    int dx = COLS/2 - (l-1)/2;          /* Middle point less x offset to place the lists*/
    int dy = LINES/2 - (k-1)/2;         /* Middle point less y offset to place the lists*/

    for(int i=0; i < k; i++){
        for(int j=0; j < l; j++){
            array[dy+i][dx+j] = (bool) grid[i][j];
        }
    }
}

int main(){   
    /* Create display*/    
    initscr();              // initiate the window stdscr
    bool displayOn = true;  // state of the display
    curs_set(false);        // hide the default screen cursor.
    nodelay(stdscr, true);  // no wait when using input entry getch()

    bool ** newField;
    bool ** field = createArray(LINES,COLS);
    initArray(field);
    int k=0;

    /* Main loop */
    while(displayOn){
        clear();    /* Clear window at each step */
        newField = createArray(LINES, COLS);
        
        for (int i = 1; i < LINES; i++){                    /* Update part */
    		for (int j=0; j < COLS; j++){
    			newField[i][j] = updateCell(field,i, j);    /*The new field will be created based on the former field*/
    		}
    	}

    	//swap
    	field = newField;
    	
        /* 
         * Display based on the field. Scan field and print on the window
         * according to the state of each cell.
         */
        for(int i=0; i < LINES; i++){
            for(int j=0; j < COLS; j++){
                if (!field[i][j])
                    mvprintw(i,j," ");
                else if(field[i][j])
                    mvprintw(i,j,"#");
            }
        }

        /* Displaying some messages */
        mvprintw(LINES-1,0,"Minh-Hoang HUYNH");
        mvprintw(0,0,"Conway's Game of Life");
        mvprintw(1,0,"Step %d",k);  /* Show the steps*/
        char* msg = "Press any key to stop";
        mvprintw(0,COLS-strlen(msg),"%s",msg);

        sleep(1);                   /* Set the delay of 1 second between displaying two steps*/
        k++;                        /* Keep the counter up*/
        refresh();                  /* Refresh the default window (stdscr) for displaying*/ 
    
        if(getch()!=ERR) {          /* ERR: No pressed key -> If a key is pressed*/
            endwin();               /* End the window */
            displayOn = false;      /* Break the loop and end the program */
        }
    }
    return 0;
}