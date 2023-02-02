#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define WIDTH 500
#define HEIGHT 500

#define ROWS 50
#define COLS 50
#define SIZE 10

/* The board has 50x50 cells */

int board[ROWS][COLS];

/* SDL Global Variables */

SDL_Window* window;
SDL_Renderer* renderer;

void makeCells(){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            SDL_Rect cell = {j * SIZE, i * SIZE, SIZE, SIZE};

            if (board[i][j]) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            }

            SDL_RenderFillRect(renderer, &cell);
        }
    }
    SDL_RenderPresent(renderer);
}

void board_init(int _board_[ROWS][COLS]){
    /* Initialize the board */

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            _board_[i][j] = 0;
        }
    }
}

void init(){
    
    /* Initialize SDL variables and parameters */

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Initializing unsuccessful with SDL 2.0\n");
        return ;
    }

    window = SDL_CreateWindow("GAME OF LIFE", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    /* Initialize the board using the helper function */

    board_init(board);
}

int getval(int i, int j) {
	if (i >= ROWS) return 0;
	if (i < 0) return 0;

	if (j >= COLS) return 0;
	if (j < 0) return 0;

	return board[i][j];
}

int get_mmbr_c(int i, int j){
	int count = 0;
	/* Max count will be 8 */
	int n, s, e, w, ne, nw, se, sw;
	n = getval(i - 1, j);
	s = getval(i + 1, j);

	e = getval(i, j + 1);
	w = getval(i, j - 1);

	ne = getval(i - 1, j + 1);
	nw = getval(i - 1, j - 1);

	se = getval(i + 1, j + 1);
	sw = getval(i + 1, j - 1);

	int sum = n + s + e + w + ne + nw + se + sw;
	return sum;

}

void Game_Of_Life(){

    /* Make a new board */

    int new[ROWS][COLS];
    board_init(new);

    /* Apply the rules of the game */
    
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            
            int cell = board[i][j];
            int cnt = get_mmbr_c(i, j);

            if (cell == 1 && (cnt == 3 || cnt == 2)) {new[i][j] = 1;}
			else if (cell == 0 && cnt == 3) {new[i][j] = 1;}
			else {new[i][j] = 0;}
        }
    }

    /* Apply the new board to the old one */
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            board[i][j] = new[i][j];
        }
    }
}

void RenderScreen(){
    SDL_RenderClear(renderer);
    Game_Of_Life();
    makeCells();
}

int main(int argc, char* argv[]){
    /* Initialize the SDL parameters */
    init();

    /* Render the cells */
    makeCells();

    /* Handle SDL Events */

    SDL_Event event;

    while (1) {
        SDL_WaitEvent(&event);

        /* Quit when user wants to */
        if (event.type == SDL_QUIT) break;

        /* Play the game when space is pressed */
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) RenderScreen();
        
        /* Allow users to make the cells under the cursor to be alive */
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int x, y;
			SDL_GetMouseState(&x, &y);

			x /= SIZE;
			y /= SIZE;

            board[y][x] = 1;
			makeCells();
        }
    }

    /* Unallocate and remove SDL parameters when user quits */

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}