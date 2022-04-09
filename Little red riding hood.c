/*      Programmed by:          <your name here>        <section>
        Description:            This program makes use of an intelligent agent to simulate Little Red Riding Hood walking through the woods to reach Granny. This agent is built to avoid the pits and the wolf and collect flowers and bread determined by their input coordinates. This program has two modes (auto and step-by-step) that the user can pick
        Last modified:          February 7, 2022; 6:04 AM
        [Acknowledgements:      LIBRARY: <unistd.h> FOR sleep(); SITE: https://www.poftut.com/what-is-sleep-function-and-how-to-use-it-in-c-program/
                                RESOURCE: MULTIDIMENSIONAL ARRAYS SITE: https://www.tutorialspoint.com/cprogramming/c_multi_dimensional_arrays.htm
                                RESOURCE: USING MULTIDIMENSIONAL ARRAYS AS FUNCTION PARAMETERS SITE: https://www.geeksforgeeks.org/pass-2d-array-parameter-c/
*/

#include <stdio.h>
#include <unistd.h>

void init_elements(int ngrid, char grid[ngrid][ngrid]);
void get_grid_size(int *ngrid);
void init_grid(int n, char grid[n][n]);


void elements(char *elem, int ngrid, int *nelem, char grid[ngrid][ngrid]);
void elements_coordinates(char *elem, int ngrid, int n, char grid[ngrid][ngrid]);
void elements_coordinates1(char *elem, int ngrid, char grid[ngrid][ngrid]);

void print_details(int *counter, char direction, char sense);
void printer(int n, char grid[n][n]);

void rotate(int ngrid, char grid[ngrid][ngrid], int LRRH_location[2], int counter[3], int *rotates);
char sense(int LRRH_location[2], int forward_location[2], int ngrid, char grid[ngrid][ngrid],int counter[3]);
char sense_to_char(char sensed);
void forward(char sensed, int LRRH_location[2], int forward_location[2], int ngrid, char grid[ngrid][ngrid], int reqs[3], int counter[3], char* prev_character, int *rotates);
void forward_position(int LRRH_location[2], int forward_location[2], int ngrid, char grid[ngrid][ngrid]);
void movement(int LRRH_location[2], int forward_location[2], int ngrid, char grid[ngrid][ngrid], int reqs[3], int counter[3], char* prev_character, int mode, int *rotates);
int steps_to_granny(int ngrid, char grid[ngrid][ngrid], int LRRH_position[2]);
int abs_value(int num);
void meet_granny(int reqs[3]);


/* This function runs the game by callng the other functions.
*/
int 
main()
{       
        int ngrid;
        get_grid_size(&ngrid);
        
        char grid[ngrid][ngrid];
        init_grid(ngrid, grid);
        
        int LRRH_location[] = {0,0};
        int forward_location[2];
        
        int counter[] = {0, 0, 0};
        int reqs[] = {0,0,0};
        int mode;
        int rotates = 0;

        init_elements(ngrid,grid);

        printf("Mode (0 - Auto; 1 - Step-by-step): \n");
        scanf("%d", &mode);
        while (mode != 0 && mode != 1)
        {
                printf("Please enter valid mode.\nMode (0 - Auto; 1 -Step-by-step): ");
                scanf("%d ", &mode);
        }
        
        
        printer(ngrid, grid);
        print_details(counter, ' ',' ');
        printf("\n");

        if (mode == 1){
                char c;
                scanf("%c", &c);
        }
        else {
                sleep(2);
        }

        char prev_character = '_';
        while (1)
        {               
                if (prev_character == 'W' && reqs[1] == 0){
                        printf("END! Wolf eats the Agent, Granny and Woodsman\n");
                        break;
                }
                else if (prev_character == 'P'){
                        printf("END! Agent Fell on a Pit!\n");
                        break;
                }
                else if (prev_character == 'G'){
                        meet_granny(reqs);
                        break;
                }    
                else{
                        if (prev_character == 'W')
                                reqs[1] -= 1;
                        movement(LRRH_location, forward_location, ngrid, grid, reqs, counter, &prev_character, mode, &rotates);
                }
                if (mode == 1){
                        char c;
                        scanf("%c", &c);
                }
                else {
                        sleep(2);
                }
        }   
        
        
}

/*  This function gets the number and the position of the "Pit", "Flower", "Bakeshop", "Wolf", "Woodsman" and "Granny"
    @param (int) ngrid is the variable for the number of rows and columns (size) of the grid
    @param (char) grid is the array that holds the grid with nxn size
    @return (Void) No return value
*/
void 
init_elements(int ngrid, char grid[ngrid][ngrid])
{
        int npits;
        char *pits = "Pit";
        elements(pits, ngrid, &npits,grid);

        int nflowers;
        char *flowers = "Flower";
        elements(flowers, ngrid, &nflowers,grid);

        int nbakeshop;
        char *bakeshop = "Bakeshop";
        elements(bakeshop, ngrid, &nbakeshop,grid);
        
        printf("\n");
        char *wolf = "Wolf";
        elements_coordinates1(wolf, ngrid, grid);

        char *woodsman = "Woodsman";
        elements_coordinates1(woodsman, ngrid, grid);

        char *granny = "Granny";
        elements_coordinates1(granny, ngrid, grid);
}

/*  This function gets the grid size.
    @param (int) ngrid is the variable for the number of rows and columns (size) of the grid
    @return (Void) No return value
*/
void 
get_grid_size(int *ngrid)
{
        printf("Enter grid size: ");
        scanf("%d", ngrid);

        while (*ngrid > 15 || *ngrid < 8){
                printf("Please enter valid grid size (8 to 15).\nEnter grid size: ");
                scanf("%d", ngrid);
        }
        return;
}

/*  This function initializes the grid by putting underscores in each element.
    @param (int) n is the variable that holds the size of the grid
    @param (char) grid[n][n] is the array that holds the grid with nxn size
    @return (void) no return value
*/
void 
init_grid(int n, char grid[n][n])
{
        for (int i = 0; i < n; i++){
                for(int j = 0; j < n; j++)
                        grid[i][j] = '_';
        }
        grid[0][0] = '^';
}

/*  This function prints the grid used in the program.
    @param (int) n is the variable that holds the size of the grid
    @param (char) grid[n][n] is the array that holds the grid with nxn size
    @return (void) no return value
*/
void 
printer(int n, char grid[n][n])
{
        for (int i = 0; i < n; i++){
                for(int j = 0; j < n; j++)
                        printf("%c ", grid[i][j]);
                printf("\n");
        }
        printf("\n");
}

/*  This function is called to get the quantity of each element as well as their coordinates.
    This function will loop the input quantity if the input is invalid. 
    @param (char) elem holds the type of element: "Pits", "Flower", and "Bakeshop"
    @param (int) ngrid holds the size of the grid
    @param (int) nelem holds the number of elements 
    @param (char) grid[n][n] is the array that holds the grid with nxn size
    @return (void) no return value
*/
void 
elements(char *elem, 
                int ngrid, 
                int *nelem, 
                char grid[ngrid][ngrid])
{
        printf("Number of %ss: ", elem);
        scanf("%d", nelem);

        while (*nelem > 3 || *nelem <= 0){
                printf("Please enter a number from 1 to 3.\nNumber of %ss: ", elem);
                scanf("%d", nelem);
        }

        for(int i = 0; i < *nelem; i++)
                elements_coordinates(elem, ngrid, i+1, grid);        
}

/*  This function gets the input coordinates for "Pit", "Bakeshop", and "Flower". It will loop if the input is invalid or if the input coordinates overlap with existing elements. 
        @param (char) elem holds the type of element: "Pits", "Flower", and "Bakeshop"
        @param (int) ngrid holds the x axis location of the element               
        @param (int) n holds the y axis location of the element 
        @param (char) grid[n][n] is the array that holds the grid with nxn size
        @return (void) no return value
*/
void 
elements_coordinates(char *elem, 
                int ngrid, 
                int n, 
                char grid[ngrid][ngrid])
{
        int x, y;
        printf("Location of %s #%d (row column): ", elem, n);
        scanf("%d", &x);
        scanf("%d", &y);

        while(x > ngrid || y > ngrid || x < 0 || y < 0 || grid[x-1][y-1] != '_'){
                if(x > ngrid || y > ngrid || x < 0 || y < 0)
                        printf("Please enter valid coordinates (1 to %d only)\n", ngrid);
                else
                        printf("Already occupied. Enter another location\n");
                printf("Location of %s #%d (row column): ", elem, n);
                scanf("%d", &x);
                scanf("%d", &y);
        }
        
        if (elem == "Pit"){
            grid[x-1][y-1] = 'P';
        }
        else if (elem == "Flower"){
            grid[x-1][y-1] = 'F'; 
        }
        else if (elem == "Bakeshop"){
            grid[x-1][y-1] ='B';

        }

}

/*  This function gets the input coordinates for "Wolf", "Woodsman", and "Granny". It will loop if the input is invalid or if the input coordinates overlap with existing elements.
        @param (char) elem holds the location of the element
        @param (int) ngrid holds the size of the grid               
        @param (char) grid[n][n] is the array that holds the grid with nxn size
        @return (void) no return value
*/
void 
elements_coordinates1(char *elem, 
                int ngrid, 
                char grid[ngrid][ngrid])
{
        int x, y;
        printf("Location of %s (row column): ", elem);
        scanf("%d", &x);
        scanf("%d", &y);

        while(x > ngrid || y > ngrid || x < 0 || y < 0 || grid[x-1][y-1] != '_'){
                if(x > ngrid || y > ngrid || x < 0 || y < 0)
                        printf("Please enter valid coordinates (1 to %d only)\n", ngrid);
                else
                        printf("Already occupied. Enter another location\n");
                printf("Location of %s (row column): ", elem);
                scanf("%d", &x);
                scanf("%d", &y);
        }

        if (elem == "Wolf"){
            grid[x-1][y-1] = 'W';
        }
        else if (elem == "Woodsman"){
            grid[x-1][y-1] = 'M'; 
        }
        else if (elem == "Granny"){
            grid[x-1][y-1] = 'G';

        }

}

/*  This function prints the details (Dashboard) needed after the grid.
        @param (int) counter holds the number of time the instance rotated, sensed and forwarded.
        @param (char) direction holds the character containing the current direction and the number of rotations
        @param (char) sense holds the character containing the current element in front and the number of times sense was used
        @return (void) no return value
*/
void 
print_details(int *counter, 
                char direction, 
                char sense)
{
        char *dir, *sen;

        if (direction == '>'){
                printf("Rotate: %d (right) Sense: %d Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (direction == 'v'){
                printf("Rotate: %d (down) Sense: %d Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (direction == '<'){
                printf("Rotate: %d (left) Sense: %d Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (direction == '^'){
                printf("Rotate: %d (up) Sense: %d Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}

        if (sense == 'h'){
                printf("Rotate: %d Sense: %d (heat) Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (sense == 'c'){
                printf("Rotate: %d Sense: %d (scent) Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (sense == 's'){
                printf("Rotate: %d Sense: %d (sound) Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (sense == 'b'){
                printf("Rotate: %d Sense: %d (breeze) Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (sense == 'o'){
                printf("Rotate: %d Sense: %d (howl) Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (sense == 'l'){
                printf("Rotate: %d Sense: %d (light) Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (sense == 'n'){
                printf("Rotate: %d Sense: %d (none) Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else if (sense == 'i'){
                printf("Rotate: %d Sense: %d (out of bounds) Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
        else{
                printf("Rotate: %d Sense: %d Forward: %d\n", counter[0], counter[1], counter[2]);
                printf("Legend: ^ < > v- Agent\n");
                printf("P-pit, F-flower, W-wolf, \nB-Bakeshop, M-Woodsman, G-granny\n");
                return;}
} 

/*  This function implements the rotate action of LRRH
        @param (int) LRRH_location[2] holds the current location of LRRH
        @param (int) ngrid holds the size of the ngrid
        @param (int) counter[3] holds the number of actions of LRRH
        @param (char) grid[n][n] is the array that holds the grid with nxn size
        @param (char) grid holds the grid used in this program
         @param  (int *) rotates keeps count on how many times LLRH has rotated in-place. Relevant for when LLRH is trapped between two elements that can cause a losing condition
        @return (void) no return value
*/
void rotate(int ngrid, char grid[ngrid][ngrid], int LRRH_location[2], int counter[3], int *rotates){
        char *LRRH_orientation = &grid[LRRH_location[0]][LRRH_location[1]];
        if (*LRRH_orientation == '>')
                {
                        *LRRH_orientation = 'v';
                }
        else if (*LRRH_orientation == 'v')
                {
                        *LRRH_orientation = '<';
                }
        else if (*LRRH_orientation == '<')
                {
                        *LRRH_orientation = '^';
                }
        else if (*LRRH_orientation == '^')
                {
                        *LRRH_orientation = '>';
                }
        counter[0]++;
        *rotates = *rotates + 1;

}

/*  This function gets the position of the element in front of LRRH
        @param (int) LRRH_location[2] holds the current location of LRRH
        @param (int) forward_location[2] holds the location forward of LRRH
        @param (int) ngrid holds the size of the ngrid
        @param (char) grid[n][n] is the array that holds the grid with nxn size
        @return (void) no return value
*/
void forward_position(int LRRH_location[2], int forward_location[2], int ngrid, char grid[ngrid][ngrid]){
        int i = LRRH_location[0];
        int j = LRRH_location[1];

        if (grid[i][j] == '^'){
                forward_location[0] = LRRH_location[0]-1;
                forward_location[1] = LRRH_location[1];
        }
        
        else if (grid[i][j] == '>'){
                forward_location[0] = LRRH_location[0];
                forward_location[1] = LRRH_location[1]+1;
        }
        
        else if (grid[i][j] == 'v'){
                forward_location[0] = LRRH_location[0]+1;
                forward_location[1] = LRRH_location[1];
        }
        
        else if (grid[i][j] == '<'){
                forward_location[0] = LRRH_location[0];
                forward_location[1] = LRRH_location[1]-1;
        }
       
}

/*  This function implements the sense action of LRRH
        @param (int) LRRH_location[2] holds the current location of LRRH
        @param (int) forward_location[2] holds the location forward of LRRH
        @param (int) ngrid holds the size of the ngrid
        @param (char) grid[n][n] is the array that holds the grid with nxn size
        @param (int) counter[3] holds the number of actions made by LRRH
        @return (void) no return value
*/
char sense(int LRRH_location[2], int forward_location[2], int ngrid, char grid[ngrid][ngrid],int counter[3]){

        forward_position(LRRH_location,forward_location,ngrid,grid);
        
        counter[1]++;

        if(forward_location[0] == -1 || forward_location[1] == -1 || forward_location[0] == ngrid || forward_location[1] == ngrid)
                return 'i';
        if (grid[forward_location[0]][forward_location[1]] == 'B')
               return 'h';
        if (grid[forward_location[0]][forward_location[1]] == 'F')
                return 'c';
        if (grid[forward_location[0]][forward_location[1]] == 'M')
                return 's';
        if (grid[forward_location[0]][forward_location[1]] == 'P')
                return 'b';
        if (grid[forward_location[0]][forward_location[1]] == 'W')
                return 'o';
        if (grid[forward_location[0]][forward_location[1]] == 'G')
                return 'l';
        
        return 'n';
}

/*  This function will map the value of sensed to its equivalent element 
        @param (char) sensed holds the value of the sensed variable
        @return (char) holds the equivalent element of the sensed value
*/
char sense_to_char(char sensed){
        if (sensed == 'h'){
                return 'B';
        }
        else if (sensed == 'c')
                return 'F';
        else if (sensed == 's')
                return 'M';
        else if (sensed == 'b')
                return 'P';
        else if (sensed == 'o')
                return 'W';
        else if (sensed == 'l')
                return 'G';
        else
                return '_';
}

/*  This function will move LRRH forward. 
        @param (char) sensed holds the sensed value in front of LRRH
        @param (int) LRRH_location[2] holds the location of LRRH              
        @param (int) forward_location[2] holds the forward location of LRRH 
        @param (int) ngrid holds the size of the grid
        @param (int) req[3] holds the condition for winning
        @param (int) counter[3] Counts the number of actions taken by LRRH
        @param (char) prev_character Contains the symbol of the squre currently visiting
        @param  (int *) rotates keeps count on how many times LLRH has rotated in-place. Relevant for when LLRH is trapped between two elements that can cause a losing condition and two invalid spaces
        @return (void) no return value
*/
void forward(char sensed, int LRRH_location[2], int forward_location[2], int ngrid, char grid[ngrid][ngrid], int reqs[3], int counter[3], char* prev_character, int *rotates){
        if (sensed == 'h' || sensed == 'c' || sensed == 's'){
                if (sensed == 'h')
                        reqs[1] = 1;
                else if (sensed == 'c')
                        reqs[0] = 1;
                        
                else if (sensed == 's')
                        reqs[2] = 1;
        }    

        grid[forward_location[0]][forward_location[1]] = grid[LRRH_location[0]][LRRH_location[1]]; 
        grid[LRRH_location[0]][LRRH_location[1]] = *prev_character;
        
        LRRH_location[0] = forward_location[0];
        LRRH_location[1] = forward_location[1]; 
        counter[2]++;

        printer(ngrid, grid);
        print_details(counter,' ',' ');
        *prev_character = sense_to_char(sensed);
        if (*prev_character == 'M'){
                reqs[2] = 1;
                int steps = steps_to_granny(ngrid, grid, LRRH_location);
                
                printf("%d Steps left to Granny \n\n", steps);
        }
        *rotates = 0;
}

/*  This function implements the movement of LRRH (including moving forward and rotating)
        @param (int) LRRH_location[2] holds the current location of LRRH
        @param (int) forward_location[2] holds the location of the element in front of LRRH 
        @param (int) ngrid holds the size of the grid
        @param (char) grid[n][n] is the array that holds the grid with nxn size
        @param (int) reqs[3] hold the flags for the winning condition
        @param (int) counter[3] Counts the number of actions taken by LRRH
        @param (char) prev_character Contains the symbol of the spaces already visited by LLRH
        @param  (int *) rotates keeps count on how many times LLRH has rotated in-place. Relevant for when LLRH is trapped between two elements that can cause a losing condition and two invalid spaces
        @return (void) no return value
*/
void movement(int LRRH_location[2], int forward_location[2], int ngrid, char grid[ngrid][ngrid], int reqs[3], int counter[3], char* prev_character, int mode, int *rotates){       
        
        char sensed = sense(LRRH_location,forward_location,ngrid,grid,counter);
        printer(ngrid, grid);
        print_details(counter, ' ',sensed); 

        if (mode == 1){
                char c;
                scanf("%c", &c);
        }
        else {
                sleep(2);
        }

        int n = counter[0] + counter[2];
        
        if (n % 7 == 0){
                rotate(ngrid, grid, LRRH_location, counter, rotates);
                printer(ngrid, grid);
                print_details(counter, grid[LRRH_location[0]][LRRH_location[1]],' ');
                return;
        }

        if (n % (ngrid-1) == 0){
                rotate(ngrid, grid, LRRH_location, counter, rotates);
                printer(ngrid, grid);
                print_details(counter, grid[LRRH_location[0]][LRRH_location[1]],' ');
                
                if (mode == 1){
                        char c;
                        scanf("%c", &c);
                }
                else {
                        sleep(2);
                }
                rotate(ngrid, grid, LRRH_location, counter, rotates);
                printer(ngrid, grid);
                print_details(counter, grid[LRRH_location[0]][LRRH_location[1]],' ');

                if (mode == 1){
                        char c;
                        scanf("%c", &c);
                }
                else {
                        sleep(2);
                }
                rotate(ngrid, grid, LRRH_location, counter, rotates);
                printer(ngrid, grid);
                print_details(counter, grid[LRRH_location[0]][LRRH_location[1]],' ');
                return;
        }

        if (sensed == 'h' || sensed == 'c' || sensed == 's' || sensed == 'l' || sensed == 'n'){
                forward(sensed, LRRH_location, forward_location, ngrid, grid, reqs,counter, prev_character, rotates);
                if (sensed == 'l')
                        return;
                return;
        }
        else if (sensed == 'b' || sensed == 'o'){
                if (*rotates % 5 == 0 && *rotates != 0){
                        forward(sensed, LRRH_location, forward_location, ngrid, grid, reqs,counter, prev_character, rotates);
                        return;
                }

                rotate(ngrid, grid, LRRH_location, counter, rotates);
                printer(ngrid, grid);
                print_details(counter, grid[LRRH_location[0]][LRRH_location[1]],' ');

                 if (mode == 1){
                        char c;
                        scanf("%c", &c);
                }
                else {
                        sleep(2);
                }

                rotate(ngrid, grid, LRRH_location, counter, rotates);
                printer(ngrid, grid);
                print_details(counter, grid[LRRH_location[0]][LRRH_location[1]],' ');

                
                return;
        }
        else if (sensed == 'i'){
                rotate(ngrid, grid, LRRH_location, counter, rotates);
                printer(ngrid, grid);
                print_details(counter, grid[LRRH_location[0]][LRRH_location[1]],' ');
                return;
        }
        else {
                return;
        }
        
}

/*  This function will return the absolute value of granny's location relative to the Woodsman in either the x and y axis 
        @param (int) num is the number to be made as input to the absolute value function        
        @return (int) is the absolute value of the input number
*/
int abs_value(int num){
        if (num > 0){
                return num;
        }
        else if (num == 0){
                return 0;
        }
        else {
                return 0 - num;
        }
}



/*  This function will determine whether LRRH wins or loses when she meets granny
        @param (int) reqs[3] holds the flags for the winning condition
        @return (void) no return value
*/
void meet_granny(int reqs[3]){
        if (reqs[0] == 1 && reqs[1] == 1 && reqs[2] == 1){
                printf("END! Agent meets Granny with a flower, bread, and Woodsman\n");
                return;
        }
        printf("END! ");
        if (reqs[0] == 0 && reqs[1] == 1 && reqs[2] == 1)
                printf("Agent meets Granny with only a bread and Woodsman. ");
        else if (reqs[0] == 1 && reqs[1] == 0 && reqs[2] == 1)
                printf("Agent meets Granny with only a flower and Woodsman. ");
        else if (reqs[0] == 1 && reqs[1] == 1 && reqs[2] == 0)
                printf("Agent meets Granny with only a flower and bread. ");
        else if (reqs[0] == 0 && reqs[1] == 0 && reqs[2] == 1)
                printf("Agent meets Granny with only Woodsman. ");
        else if (reqs[0] == 1 && reqs[1] == 0 && reqs[2] == 0)
                printf("Agent meets Granny with only a flower. ");
        else if (reqs[0] == 0 && reqs[1] == 1 && reqs[2] == 0)
                printf("Agent meets Granny with only a bread. ");
        else if (reqs[0] == 0 && reqs[1] == 0 && reqs[2] == 0)
                printf("Agent meets Granny empty-handed. ");

        printf("Wolf eats the Agent, Granny and Woodsman\n");
        return;
}

/*  This function will determine how much steps LRRH needs to take to meet Granny
        @param (int) ngrid holds the size of the grid
        @param (char) grid[ngrid][ngrid] holds the grid itself
        @param (int) LRRH_position[2] holds the position of the woodcutter
        @return (void) grannyrow + grannycolumn returns the total number of steps to Granny
*/
int steps_to_granny(int ngrid, char grid[ngrid][ngrid], int LRRH_position[2]){
        
        int i, j, x = 0;
        int y = 0;

        for (i = 0; i < ngrid; i++){
                for(j = 0; j < ngrid; j++){
                        if (grid[i][j] == 'G'){
                                x = i;
                                y = j;
                        }
                } 
        }     
        int grannyrow = abs_value(x - LRRH_position[0]);
        int grannycolumn = abs_value(y - LRRH_position[1]);

        return grannyrow + grannycolumn;
        
}