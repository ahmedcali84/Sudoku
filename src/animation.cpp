#include <SDL2/SDL.h>
#include <iostream>
#include <helper.h>
#include "sudoku.h"

// NOTE: Window Sizes
#define SCREEN_WIDTH  900
#define SCREEN_HEIGHT 900

// NOTE: Board Sizes
#define BOARD_WIDTH  (BOARD_ROWS)
#define BOARD_HEIGHT (BOARD_COLS)

// NOTE: Cell Sizes 
#define CELL_WIDTH  ((float) SCREEN_WIDTH  / (float) BOARD_WIDTH)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / (float) BOARD_HEIGHT)

namespace Sudoku {
    class Frame {
    public:
        Frame();
        ~Frame();
        int Init(const char *file_path);
        int RenderFrame();
        int UpdateFrame();
        void CleanUp();

    private:
        SDL_Window *Window;
        SDL_Renderer *Renderer;
        CellPool _Board[BOARD_ROWS][BOARD_COLS];
    };
};

// NOTE: Constructor
Sudoku::Frame::Frame()
    : Window(nullptr), Renderer(nullptr), _Board{} // Initialize _Board to zero
{
    // No need for additional initialization here
}

// NOTE: Deconstructor
Sudoku::Frame::~Frame() {
    CleanUp();
}

// NOTE: Function that initializes Animation
int Sudoku::Frame::Init(const char *file_path) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "ERROR: Failed to Initialize Video." << SDL_GetError() << std::endl;
        return -1;
    }

    // NOTE: Create Window 
    Window = SDL_CreateWindow("Sudoku Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (Window == nullptr) {
        std::cout << "ERROR: Failed to Create Window. " << SDL_GetError() << std::endl;
        return -1;
    }

    // NOTE: Create Renderer 
    Renderer = SDL_CreateRenderer(Window, -1, 0);
    if (Renderer == nullptr) {
        std::cout << "ERROR: Failed to Create Renderer. " << SDL_GetError() << std::endl;
        return -1;
    }

    if(SDL_RenderSetLogicalSize(Renderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0) {
        std::cout << "ERROR: Failed to Set Render Logical Size. " << SDL_GetError() << std::endl;
        return -1;
    }

    return 0;
}

// NOTE: Draw Frame on Window
int Sudoku::Frame::RenderFrame() {
    if (SDL_SetRenderDrawColor(Renderer, 150 , 150 , 150 , 255) < 0) {
        std::cout << "ERROR: Failed to Set Render Color." << SDL_GetError() << std::endl;
        return -1; 
    }

    for (int x = 0; x < BOARD_WIDTH; ++x) {
        if (SDL_RenderDrawLine(Renderer, x*CELL_WIDTH , 0 , x*CELL_WIDTH , SCREEN_HEIGHT) < 0) {
            std::cout << "ERROR: Failed to Draw Line." << SDL_GetError() << std::endl;
            return -1;  
        }
    }

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        if (SDL_RenderDrawLine(Renderer, 0 , y*CELL_HEIGHT , SCREEN_WIDTH, y*CELL_HEIGHT) < 0) {
            std::cout << "ERROR: Failed to Draw Line." << SDL_GetError() << std::endl;
            return -1;  
        }
    }
    return 0;
}

// NOTE: Function that Updates Animation
int Sudoku::Frame::UpdateFrame() {
    // TODO: Implement 
    int quit = 0;
    while(!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = 1;
                } break;
            }
        }

        if (SDL_SetRenderDrawColor(Renderer, 30 , 30 , 30 , 255) < 0) {
            std::cout << "ERROR: Failed to Set Render Color." << SDL_GetError() << std::endl;
            return -1; 
        }

        if (SDL_RenderClear(Renderer) < 0) {
            std::cout << "ERROR: Failed to Clear Background." << SDL_GetError() << std::endl;
            return -1; 
        }

        RenderFrame();
        SDL_RenderPresent(Renderer);
    }

    SDL_Quit();
    return 0;
}

void Sudoku::Frame::CleanUp() {
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    // FreeBoard(_Board);
}

// NOTE: Main Function
int main(void) {
    Sudoku::Frame F ;
    if (F.Init("data/grid1.txt") < 0) return 1;
    if (F.UpdateFrame() < 0) return 1;
    return 0;
}

