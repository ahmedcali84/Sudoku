#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <cstdio>
#include <helper.h>
#include "sudoku.h"

// NOTE: Window Sizes
#define SCREEN_WIDTH  900
#define SCREEN_HEIGHT 900

// NOTE: Board Sizes
#define BOARD_WIDTH  (BOARD_ROWS)
#define BOARD_HEIGHT (BOARD_COLS)

// NOTE: Cell Sizes 
#define CELL_WIDTH  (SCREEN_WIDTH  / BOARD_WIDTH)
#define CELL_HEIGHT (SCREEN_HEIGHT / BOARD_HEIGHT)

namespace Sudoku {
    class Frame {
    public:
        Frame();
        ~Frame();
        int Init(const char *file_path);
        int RenderFrame();
        int UpdateFrame();
        void HighlightCell(int row, int col);
        TTF_Font *LoadAssets(const char *file_path);
        SDL_Surface *RenderTextToSurface(SDL_Color *color , const char *text);
        SDL_Texture *RenderTextureFromSurface(SDL_Surface *TS);
        void DrawNumber(int row, int col, int number, SDL_Color *color, float alpha);
        void CleanUp();

    private:
        SDL_Window *Window;
        SDL_Renderer *Renderer;
        TTF_Font *Font;
        CellPool _Board[BOARD_ROWS][BOARD_COLS];
    };
};

// NOTE: Constructor
Sudoku::Frame::Frame()
    : Window(nullptr), Renderer(nullptr), Font(nullptr), _Board{} // Initialize _Board to zero
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
        std::cout << "ERROR: Failed to Initialize Video: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1) {
        std::cout << "ERROR: Failed to Initialize TTF: " << TTF_GetError() << std::endl;
        return -1;
    }

    Font = LoadAssets("./assets/fonts/Iosevka-Regular.ttc");
    if (Font == nullptr) {
        return -1;
    }

    // NOTE: Create Window 
    Window = SDL_CreateWindow("Sudoku Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (Window == nullptr) {
        std::cout << "ERROR: Failed to Create Window: " << SDL_GetError() << std::endl;
        return -1;
    }

    // NOTE: Create Renderer 
    Renderer = SDL_CreateRenderer(Window, -1, 0);
    if (Renderer == nullptr) {
        std::cout << "ERROR: Failed to Create Renderer: " << SDL_GetError() << std::endl;
        return -1;
    }

    if(SDL_RenderSetLogicalSize(Renderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0) {
        std::cout << "ERROR: Failed to Set Render Logical Size: " << SDL_GetError() << std::endl;
        return -1;
    }

    Grid *grid = grid_alloc();
    if(!read_file(file_path, grid)) return -1;
    InitBoard(grid, _Board);

    return 0;
}

// NOTE: Draw Frame on Window
int Sudoku::Frame::RenderFrame() {
    if (SDL_SetRenderDrawColor(Renderer, 150 , 150 , 150 , 255) < 0) {
        std::cout << "ERROR: Failed to Set Render Color: " << SDL_GetError() << std::endl;
        return -1;
    }

    for (int x = 0; x < BOARD_WIDTH; ++x) {
        if (SDL_RenderDrawLine(Renderer, x*CELL_WIDTH , 0 , x*CELL_WIDTH , SCREEN_HEIGHT) < 0) {
            std::cout << "ERROR: Failed to Draw Line: " << SDL_GetError() << std::endl;
            return -1;
        }
    }

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        if (SDL_RenderDrawLine(Renderer, 0 , y*CELL_HEIGHT , SCREEN_WIDTH, y*CELL_HEIGHT) < 0) {
            std::cout << "ERROR: Failed to Draw Line: " << SDL_GetError() << std::endl;
            return -1;
        }
    }
    return 0;
}

// NOTE: Function that Updates Animation
int Sudoku::Frame::UpdateFrame() {
    // TODO: Implement 
    int Closed = 0;
    while(!Closed) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    Closed = 1;
                } break;
            }
        }

        if (SDL_SetRenderDrawColor(Renderer, 30 , 30 , 30 , 255) < 0) {
            std::cout << "ERROR: Failed to Set Render Color: " << SDL_GetError() << std::endl;
            return -1;
        }

        if (SDL_RenderClear(Renderer) < 0) {
            std::cout << "ERROR: Failed to Clear Background: " << SDL_GetError() << std::endl;
            return -1; 
        }

        SDL_Color color = {255, 255 , 255 , 255};

        for (int i = 0; i < BOARD_WIDTH; ++i) {
            for (int j = 0; j < BOARD_HEIGHT; ++j) {
                if (CheckCellStatus(_Board, i , j)) {
                    DrawNumber(i , j , _Board[i][j].cell->value , &color , 1.0f);
                }
            }
        }
        RenderFrame();
        SDL_RenderPresent(Renderer);
    }


    return 0;
}

// NOTE: Function for highlighting a cell When being Filled
void Sudoku::Frame::HighlightCell(int row, int col) {
    SDL_Rect cellRect = { col * CELL_WIDTH, row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT };
    SDL_SetRenderDrawColor(Renderer, 255, 255, 0, 255); // Yellow color for highlight
    SDL_RenderFillRect(Renderer, &cellRect);
}

TTF_Font *Sudoku::Frame::LoadAssets(const char *file_path) {
    TTF_Font *font = TTF_OpenFont(file_path, 50);
    if (font == nullptr) {
        std::cout << "Failed To Open Font: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return font;
}

SDL_Surface *Sudoku::Frame::RenderTextToSurface(SDL_Color *color , const char *text) {
    SDL_Surface* TextSurface = TTF_RenderText_Solid(Font, text, *color);
    if (TextSurface == nullptr) {
        std::cout << "Failed To Render Text Solid: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return TextSurface;
}

SDL_Texture *Sudoku::Frame::RenderTextureFromSurface(SDL_Surface *TS) {
    SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(Renderer, TS);
    if (TextTexture == nullptr) {
        std::cout << "Failed To Render Texture From Surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return TextTexture;
}

const char *int_to_cstr(int num) {
    static char Buffer[50];
    sprintf(Buffer, "%d" , num);
    return Buffer;
}

// NOTE: Function for Drawing Color On the Frame
void Sudoku::Frame::DrawNumber(int row, int col, int number, SDL_Color *color, float alpha) {
    const char *text = int_to_cstr(number);
    // Set the color with the specified alpha
    SDL_SetRenderDrawColor(Renderer, color->r, color->g, color->b, (Uint8)(alpha * color->a));

    SDL_Surface *Surface = RenderTextToSurface(color , text);
    if (Surface == nullptr) {
        return;
    }

    SDL_Texture *Texture = RenderTextureFromSurface(Surface);
    if (Surface == nullptr) {
        return;
    }
 
   // Get the width and height of the texture
    int TextWidth, TextHeight;
    SDL_QueryTexture(Texture, nullptr, nullptr, &TextWidth, &TextHeight);

    SDL_Rect Rect;
    Rect.x = row * CELL_WIDTH  + (CELL_WIDTH - TextWidth) / 2;
    Rect.y = col * CELL_HEIGHT + (CELL_HEIGHT - TextHeight) / 2;
    Rect.w = TextWidth;
    Rect.h = TextHeight;
    SDL_RenderCopy(Renderer , Texture , nullptr , &Rect);

    // Clean up the surface and texture after use
    SDL_FreeSurface(Surface);
    SDL_DestroyTexture(Texture);
}

// NOTE: Function to Clean Up Resources On Exit
void Sudoku::Frame::CleanUp() {
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    TTF_CloseFont(Font);
    SDL_Quit();
    TTF_Quit();
    FreeBoard(_Board);
}

// NOTE: Main Function
int main(void) {
    Sudoku::Frame F ;
    if (F.Init("data/grid1.txt") < 0) return 1;
    if (F.UpdateFrame() < 0) return 1;
    return 0;
}
