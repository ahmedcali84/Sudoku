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

typedef const char * String;

namespace Sudoku {
    class Frame {
    public:
        Frame();
        ~Frame();
        int Init(String file_path);
        int UpdateFrame();

    private:
        int RenderFrame();
        void HighlightCell(int row, int col);
        TTF_Font *LoadAssets(String file_path);
        SDL_Surface *RenderTextToSurface(SDL_Color *color , String text);
        SDL_Texture *RenderTextureFromSurface(SDL_Surface *TS);
        void DrawString(String Text, SDL_Color *Color , float alpha);
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

// NOTE: Destructor
Sudoku::Frame::~Frame() {
    CleanUp();
}

// NOTE: Function that initializes Animation
int Sudoku::Frame::Init(String file_path) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "[ERROR]: Failed to Initialize Video: " << SDL_GetError() << std::endl;
        return -1;
    }

    std::cout << "[INFO]: Successfully Initialized SDL Video." << std::endl;

    if (TTF_Init() == -1) {
        std::cout << "[ERROR]: Failed to Initialize TTF: " << TTF_GetError() << std::endl;
        return -1;
    }

    std::cout << "[INFO]: Successfully Initialized TTF." << std::endl;
    
    String font_file_path = "./assets/fonts/Iosevka-Regular.ttc";
    Font = LoadAssets(font_file_path);
    if (Font == nullptr) {
        return -1;
    }

    std::cout << "[INFO]: Successfully Loaded " << font_file_path << std::endl;

    // NOTE: Create Window 
    Window = SDL_CreateWindow("Sudoku Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (Window == nullptr) {
        std::cout << "[ERROR]: Failed to Create Window: " << SDL_GetError() << std::endl;
        return -1;
    }

    std::cout << "[INFO]: Successfully Created Window" << std::endl;

    // NOTE: Create Renderer 
    Renderer = SDL_CreateRenderer(Window, -1, 0);
    if (Renderer == nullptr) {
        std::cout << "[ERROR]: Failed to Create Renderer: " << SDL_GetError() << std::endl;
        return -1;
    }

    std::cout << "[INFO]: Successfully Created Renderer." << std::endl;

    if(SDL_RenderSetLogicalSize(Renderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0) {
        std::cout << "[ERROR]: Failed to Set Render Logical Size: " << SDL_GetError() << std::endl;
        return -1;
    }

    std::cout << "[INFO]: Successfully Set Render Logical size." << std::endl;

    Grid *grid = grid_alloc();
    if(!read_file(file_path, grid)) return -1;
    InitBoard(grid, _Board);

    return 0;
}

// NOTE: Draw Frame on Window
int Sudoku::Frame::RenderFrame() {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        if (x % 3 == 0) {
            if (SDL_SetRenderDrawColor(Renderer, 200 , 200 , 200 , 255) < 0) {
                std::cout << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
                return -1;
            }
        } else {
           if (SDL_SetRenderDrawColor(Renderer, 100 , 100 , 100 , 255) < 0) {
                std::cout << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
                return -1;
            }
        }
        if (SDL_RenderDrawLine(Renderer, x*CELL_WIDTH , 0 , x*CELL_WIDTH , SCREEN_HEIGHT) < 0) {
            std::cout << "[ERROR]: Failed to Draw Line: " << SDL_GetError() << std::endl;
            return -1;
        }
    }

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        if (y % 3 == 0) {
            if (SDL_SetRenderDrawColor(Renderer, 200 , 200 , 200 , 255) < 0) {
                std::cout << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
                return -1;
            }
        } else {
           if (SDL_SetRenderDrawColor(Renderer, 100 , 100 , 100 , 255) < 0) {
                std::cout << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
                return -1;
            }
        }
        if (SDL_RenderDrawLine(Renderer, 0 , y*CELL_HEIGHT , SCREEN_WIDTH, y*CELL_HEIGHT) < 0) {
            std::cout << "[ERROR]: Failed to Draw Line: " << SDL_GetError() << std::endl;
            return -1;
        }
    }

    return 0;
}

// NOTE: Function that Updates Animation
int Sudoku::Frame::UpdateFrame() {
    // NOTE: Implemented
    bool Closed = false;
    while(!Closed) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    Closed = true;
                    std::cout << "[INFO]: Successfully Closed the Application." << std::endl;
                } break;
            }
        }

        if (SDL_SetRenderDrawColor(Renderer, 0, 0 , 0 , 255) < 0) {
            std::cout << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
            return -1;
        }

        if (SDL_RenderClear(Renderer) < 0) {
            std::cout << "[ERROR]: Failed to Clear Background: " << SDL_GetError() << std::endl;
            return -1; 
        }

        String InitializationInformation = "INITIAL STATE OF THE BOARD";
        SDL_Color StringColor = {255, 255, 255 , 255};
        DrawString(InitializationInformation, &StringColor, 1.0f);

        if (RenderFrame() < 0) return -1;

        SDL_Color NumberColor = {255, 255 , 255 , 255};

        for (int i = 0; i < BOARD_WIDTH; ++i) {
            for (int j = 0; j < BOARD_HEIGHT; ++j) {
                if (CheckCellStatus(_Board, i , j)) {
                    HighlightCell(i , j);
                    int CurrentInt = _Board[i][j].cell->value;
                    DrawNumber(i , j , CurrentInt, &NumberColor , 1.0f);
                    SDL_RenderPresent(Renderer);
                    SDL_Delay(500);
                }
            }
        }
        SDL_Delay(1000);
    }
    return 0;
}

// NOTE: Function for highlighting a cell When being Filled
void Sudoku::Frame::HighlightCell(int row, int col) {
    SDL_Rect cellRect = { row * CELL_WIDTH, col * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT };
    SDL_SetRenderDrawColor(Renderer, 90, 90, 90, 50);
    SDL_RenderFillRect(Renderer, &cellRect); 
}

TTF_Font *Sudoku::Frame::LoadAssets(String file_path) {
    TTF_Font *font = TTF_OpenFont(file_path, 50);
    if (font == nullptr) {
        std::cout << "[ERROR]: Failed To Open Font: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return font;
}

SDL_Surface *Sudoku::Frame::RenderTextToSurface(SDL_Color *color , String text) {
    SDL_Surface* TextSurface = TTF_RenderText_Solid(Font, text, *color);
    if (TextSurface == nullptr) {
        std::cout << "[ERROR]: Failed To Render Text Solid: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return TextSurface;
}

SDL_Texture *Sudoku::Frame::RenderTextureFromSurface(SDL_Surface *TS) {
    SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(Renderer, TS);
    if (TextTexture == nullptr) {
        std::cout << "[ERROR]: Failed To Render Texture From Surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return TextTexture;
}

String int_to_cstr(int num) {
    static char Buffer[50];
    sprintf(Buffer, "%d" , num);
    return Buffer;
}

void Sudoku::Frame::DrawString(String Text, SDL_Color *Color , float alpha) {
    // Set the color with the specified alpha
    SDL_SetRenderDrawColor(Renderer, Color->r, Color->g, Color->b, (Uint8)(alpha * Color->a));

    SDL_Surface *Surface = RenderTextToSurface(Color , Text);
    if (Surface == nullptr) {
        return;
    }

    SDL_Texture *Texture = RenderTextureFromSurface(Surface);
    if (Texture == nullptr) {
        return;
    }

    int TextWidth, TextHeight;
    SDL_SetTextureBlendMode(Texture, SDL_BLENDMODE_BLEND); // Set blend mode 
    SDL_SetTextureScaleMode(Texture, SDL_ScaleModeLinear); // Set linear scaling
 
   // Get the width and height of the texture
    SDL_QueryTexture(Texture, nullptr, nullptr, &TextWidth, &TextHeight);

    SDL_Rect Rect;
    Rect.x = 0;
    Rect.y = 0;
    Rect.w = TextWidth;
    Rect.h = TextHeight;
    SDL_RenderCopy(Renderer , Texture , nullptr , &Rect);

    // Clean up the surface and texture after use
    SDL_FreeSurface(Surface);
    SDL_DestroyTexture(Texture);
}

// NOTE: Function for Drawing Color On the Frame
void Sudoku::Frame::DrawNumber(int row, int col, int number, SDL_Color *Color, float alpha) {
    // Convert the Number to be rendered to string 
    String Text = int_to_cstr(number);

    // Set the color with the specified alpha
    SDL_SetRenderDrawColor(Renderer, Color->r, Color->g, Color->b, (Uint8)(alpha * Color->a));

    SDL_Surface *Surface = RenderTextToSurface(Color , Text);
    if (Surface == nullptr) {
        return;
    }

    SDL_Texture *Texture = RenderTextureFromSurface(Surface);
    if (Texture == nullptr) {
        return;
    }

    int TextWidth, TextHeight;
    SDL_SetTextureBlendMode(Texture, SDL_BLENDMODE_BLEND); // Set blend mode 
    SDL_SetTextureScaleMode(Texture, SDL_ScaleModeLinear); // Set linear scaling
 
   // Get the width and height of the texture
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
