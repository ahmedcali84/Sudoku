#include <SDL2/SDL.h>
#include <iostream>
#include <helper.h>
#include "sudoku.h"

const int SCREEN_WIDTH  = 1600;
const int SCREEN_HEIGHT = 900;

namespace Sudoku {
    class Frame {
    public:
        Frame();
        ~Frame();
        int Init(const char *file_path);
        void DrawFrame();
        void UpdateFrame();

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
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    FreeBoard(_Board);
}

// NOTE: Function that initializes Animation
int Sudoku::Frame::Init(const char *file_path) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "ERROR: Failed to Initialize Video." << SDL_GetError() << std::endl;
        return -1;
    }

    // NOTE: Create Window 
    Window = SDL_CreateWindow("Sudoku Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

        SDL_RenderPresent(Renderer);
    }

    SDL_Quit();
    return 0;
}

// NOTE: Draw Frame on Window
void Sudoku::Frame::DrawFrame() {
    // TODO: Implement
}

// NOTE: Function that Updates Animation
void Sudoku::Frame::UpdateFrame() {
    // TODO: Implement 
}

// NOTE: Main Function
int main(void) {
    Sudoku::Frame F ;
    if (F.Init("data/grid1.txt") < 0) {
        return 1;
    }
    return 0;
}

