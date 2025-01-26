#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdio>
#include <helper.h>
#include <unordered_map>
#include "sudoku.h"

#define FACTOR 1.0f

// NOTE: Window Sizes
#define SCREEN_WIDTH  900 * FACTOR
#define SCREEN_HEIGHT 900 * FACTOR

// NOTE: Board Sizes
#define BOARD_WIDTH  (BOARD_ROWS)
#define BOARD_HEIGHT (BOARD_COLS)

// NOTE: Cell Sizes 
#define CELL_WIDTH  (int) ((float) (SCREEN_WIDTH  / BOARD_WIDTH) * FACTOR)
#define CELL_HEIGHT (int) ((float) (SCREEN_HEIGHT / BOARD_HEIGHT) * FACTOR)

typedef const char * String;

namespace Sudoku {
    class sWindow {
    public:
        // Constructor
        sWindow() {
            // NOTE: Initialize SDL
            if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                std::cerr << "[ERROR]: Failed to Initialize Video: " << SDL_GetError() << std::endl;
                throw std::runtime_error("SDL Initialization Failed");
            }

            std::cout << "[INFO]: Successfully Initialized SDL Video." << std::endl;

            // NOTE: Create Window 
            Window = SDL_CreateWindow("Sudoku Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
            if (Window == nullptr) {
                std::cerr << "[ERROR]: Failed to Create Window: " << SDL_GetError() << std::endl;
                throw std::runtime_error("Window Creation Failed");
            }

            std::cout << "[INFO]: Successfully Created Window" << std::endl;    
        }
        
        // Destructor
        ~sWindow() {
            // NOTE: Destroy the Window
            SDL_DestroyWindow(Window);
            SDL_Quit();
        }
        
        // Getter foe the Window
        SDL_Window *GetWindow() const {
            return Window;
        }

    private:
        SDL_Window *Window;
    };

    class sRenderer {
    public:
        // Constructor
        sRenderer(const sWindow &Window) {
            // NOTE: Create Renderer 
            Renderer = SDL_CreateRenderer(Window.GetWindow(), -1, 0);
            if (Renderer == nullptr) {
                std::cerr << "[ERROR]: Failed to Create Renderer: " << SDL_GetError() << std::endl;
                throw std::runtime_error("Renderer Creation Failed");
            }

            std::cout << "[INFO]: Successfully Created Renderer." << std::endl;

            if(SDL_RenderSetLogicalSize(Renderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0) {
                std::cerr << "[ERROR]: Failed to Set Render Logical Size: " << SDL_GetError() << std::endl;
                SDL_DestroyRenderer(Renderer);
                throw std::runtime_error("Failed to Set Render Logical Size");
            }

            std::cout << "[INFO]: Successfully Set Render Logical size." << std::endl;
        }
        
        SDL_Renderer *GetRenderer() const {
            return Renderer;
        }
        
        // Destructor
        ~sRenderer() {
            // NOTE: Destroy the Renderer
            SDL_DestroyRenderer(Renderer);
        }

    private:
        SDL_Renderer *Renderer;
    };

    class sFont {
    public:
        // Constructor
        sFont(String FontFilePath, const int FontSize) {
            // NOTE: Initialize SDL Font
            if (TTF_Init() == -1) {
                std::cerr << "[ERROR]: Failed to Initialize TTF: " << TTF_GetError() << std::endl;
                throw std::runtime_error("TTF Initialization Failed");
            }

            std::cout << "[INFO]: Successfully Initialized TTF." << std::endl;

            // NOTE: Create Font
            Font = TTF_OpenFont(FontFilePath, FontSize);
            if (Font == nullptr) {
                std::cerr << "[ERROR]: Failed To Open Font: " << TTF_GetError() << std::endl;
                throw std::runtime_error("Failed To Open Font");
            }

            std::cout << "[INFO]: Successfully Loaded " << FontFilePath << std::endl;
        }
        
        // Destructor
        ~sFont() {
            TTF_CloseFont(Font);
            TTF_Quit();
        }
        
        // Getter For Font
        TTF_Font *GetFont() const {
            return Font;
        }

    private:
        TTF_Font *Font;
    };

    class sSurface {
    public:
        // Constructor: Create Surface From Text
        sSurface(sFont& Font, String Text , SDL_Color Color) {
            Surface = TTF_RenderText_Solid(Font.GetFont(), Text, Color);
            if (Surface == nullptr) {
                std::cerr << "[ERROR]: Failed To Render Text Solid: " << TTF_GetError() << std::endl;
                throw std::runtime_error("Failed To Create Surface");
            }
            
            std::cout << "[INFO]: Successfully Created Surface From Text" << std::endl;
        }

        // Destructor
        ~sSurface() {
            if(Surface) {
                SDL_FreeSurface(Surface);
            }
        }

        SDL_Surface *GetSurface() const {
            return Surface;
        }

    private:
        SDL_Surface *Surface;
    };

    class sTexture {
    public:
        // Default Constructor


        // Constructor: Create A Texture From A Surface
        sTexture(const sRenderer& Renderer, const sSurface& Surface) {
            Texture = SDL_CreateTextureFromSurface(Renderer.GetRenderer(), Surface.GetSurface());
            if (Texture == nullptr) {
                std::cerr << "[ERROR]: Failed To Render Texture From Surface: " << TTF_GetError() << std::endl;
                throw std::runtime_error("Texture Creation Failed");
            }

            std::cout << "[INFO]: Successfully Created Texture From Surface" << std::endl;
        }
        
        // Destructor
        ~sTexture() {
            if (Texture) {
                SDL_DestroyTexture(Texture);
            }
        }
        
        // Getter For Texture
        SDL_Texture *GetTexture() const {
            return Texture;
        }
        
        // Render the Texture
        void Render(const sRenderer& Renderer, int x , int y , int width , int height) {
            SDL_Rect Rect = {x , y , width , height};
            SDL_RenderCopy(Renderer.GetRenderer(), Texture , nullptr , &Rect);
        }

    private:
        SDL_Texture *Texture;
    };
    
    class sBoard{
    public:
        // Constructor
        sBoard(String FilePath) {
            Grid *grid = grid_alloc();
            if (grid == nullptr) {
                throw std::runtime_error("Failed To Allocate Memory For Grid");
            }
            if(!read_file(FilePath, grid)) {
                grid_dealloc(grid);
                throw std::runtime_error("Failed To Read File To Grid");
            }
            InitBoard(grid, _Board);
        }
        
        // Destructor
        ~sBoard() {
            FreeBoard(_Board);
        }
        
        // Getter For Board
        CellPool (&GetBoard())[BOARD_ROWS][BOARD_COLS] {
            return _Board;
        }

    private:
        CellPool _Board[BOARD_ROWS][BOARD_COLS];
    };

    class Frame {
    public:
        Frame();
        ~Frame();
        int UpdateFrame();
        int RenderFrame();
        void HighlightCell(int row, int col, SDL_Color HighlightColor);
        void DrawString(String Text, SDL_Color Color , float alpha);
        bool DrawNumber(int row, int col, int number, SDL_Color color, float alpha);
        bool Solve();
        sBoard GetsBoard();
        sRenderer GetsRenderer();

    private:
        sWindow Window;
        sRenderer Renderer;
        sFont Font;
        sBoard _Board;
    };
};

// NOTE: Constructor
Sudoku::Frame::Frame()
    : Window(), Renderer(Window), Font("./assets/fonts/Iosevka-Regular.ttc", 50), _Board("data/grid1.txt")
{
    std::cout << "[INFO]: Successfully Initialized Frame." << std::endl;
}

// NOTE: Destructor
Sudoku::Frame::~Frame() {
    std::cout << "Frame Destroyed Successfully" << std::endl;
}

Sudoku::sBoard Sudoku::Frame::GetsBoard() {
    return _Board;
}

Sudoku::sRenderer Sudoku::Frame::GetsRenderer() {
    return Renderer;
}

// NOTE: Draw Frame on Window
int Sudoku::Frame::RenderFrame() {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        if (x % 3 == 0) {
            if (SDL_SetRenderDrawColor(Renderer.GetRenderer(), 200 , 200 , 200 , 255) < 0) {
                std::cout << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
                return -1;
            }
        } else {
           if (SDL_SetRenderDrawColor(Renderer.GetRenderer(), 100 , 100 , 100 , 255) < 0) {
                std::cout << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
                return -1;
            }
        }
        if (SDL_RenderDrawLine(Renderer.GetRenderer(), x*CELL_WIDTH , 0 , x*CELL_WIDTH , SCREEN_HEIGHT) < 0) {
            std::cout << "[ERROR]: Failed to Draw Line: " << SDL_GetError() << std::endl;
            return -1;
        }
    }

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        if (y % 3 == 0) {
            if (SDL_SetRenderDrawColor(Renderer.GetRenderer(), 200 , 200 , 200 , 255) < 0) {
                std::cout << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
                return -1;
            }
        } else {
           if (SDL_SetRenderDrawColor(Renderer.GetRenderer(), 100 , 100 , 100 , 255) < 0) {
                std::cout << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
                return -1;
            }
        }
        if (SDL_RenderDrawLine(Renderer.GetRenderer(), 0 , y*CELL_HEIGHT , SCREEN_WIDTH, y*CELL_HEIGHT) < 0) {
            std::cout << "[ERROR]: Failed to Draw Line: " << SDL_GetError() << std::endl;
            return -1;
        }
    }

    return 0;
}

// NOTE: Function that Updates Animation
int Sudoku::Frame::UpdateFrame() {
    bool Closed = false;
    bool Solved = false;

    while (!Closed) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                Closed = true;
                std::cout << "[INFO]: Successfully Closed the Application." << std::endl;
                return 0; // Exit immediately
            }
        }

        if (SDL_SetRenderDrawColor(Renderer.GetRenderer(), 0, 0, 0, 255) < 0) {
            std::cerr << "[ERROR]: Failed to Set Render Color: " << SDL_GetError() << std::endl;
            return -1;
        }

        if (SDL_RenderClear(Renderer.GetRenderer()) < 0) {
            std::cerr << "[ERROR]: Failed to Clear Background: " << SDL_GetError() << std::endl;
            return -1;
        }

        if (!Solved) {
            String InitializationInformation = "INITIAL STATE OF THE BOARD";
            SDL_Color StringColor = {255, 255, 255, 255};
            DrawString(InitializationInformation, StringColor, 1.0f);
            SDL_Delay(3000);

            if (RenderFrame() < 0) return -1;

            SDL_Color NumberColor = {255, 255, 255, 255};
            SDL_Color HighlightFilled = {90, 90, 90, 50};

            for (int i = 0; i < BOARD_WIDTH; ++i) {
                for (int j = 0; j < BOARD_HEIGHT; ++j) {
                    if (CheckCellStatus(_Board.GetBoard(), i, j)) {
                        HighlightCell(i, j, HighlightFilled);
                        int CurrentInt = _Board.GetBoard()[i][j].cell->value;
                        if (!DrawNumber(i, j, CurrentInt, NumberColor, 1.0f)) {
                            std::cerr << "[ERROR]: Failed To Draw Number: " << CurrentInt << std::endl;
                            Closed = true;
                            return -1;
                        }
                        SDL_RenderPresent(Renderer.GetRenderer());
                        SDL_Delay(100);
                    }
                }
            }

            if (!Solve()) {
                std::cerr << "[ERROR]: Unable To Solve Board" << std::endl;
                return -1;
            }

            Solved = true;
        } else {
            String SolvedInformation = "FINAL STATE OF THE BOARD";
            SDL_Color FinalStringColor = {255, 255, 255, 255};
            DrawString(SolvedInformation, FinalStringColor, 1.0f);
            SDL_Delay(3000);

            if (RenderFrame() < 0) return -1;

            SDL_Color NumberColor = {255, 255, 255, 255};
            SDL_Color HighlightFilled = {90, 90, 90, 50};

            for (int i = 0; i < BOARD_WIDTH; ++i) {
                for (int j = 0; j < BOARD_HEIGHT; ++j) {
                    if (CheckCellStatus(_Board.GetBoard(), i, j)) {
                        HighlightCell(i, j, HighlightFilled);
                        int CurrentInt = _Board.GetBoard()[i][j].cell->value;
                        if (!DrawNumber(i, j, CurrentInt, NumberColor, 1.0f)) {
                            std::cerr << "[ERROR]: Failed To Draw Number: " << CurrentInt << std::endl;
                            Closed = true;
                            return -1;
                        }
                        SDL_RenderPresent(Renderer.GetRenderer());
                        SDL_Delay(100);
                    }
                }
            }

            // Wait for user to close the window
            while (!Closed) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        Closed = true;
                        std::cout << "[INFO]: Successfully Closed the Application." << std::endl;
                        return 0; // Exit immediately
                    }
                }
                SDL_Delay(100);
            }
        }
    }

    PrintBoard(_Board.GetBoard());
    return 0;
}
// NOTE: Function for highlighting a cell When being Filled
void Sudoku::Frame::HighlightCell(int row, int col, SDL_Color HighlightColor) {
    SDL_Rect cellRect = { row * CELL_WIDTH, col * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT };
    SDL_SetRenderDrawColor(Renderer.GetRenderer(), HighlightColor.r, HighlightColor.g, HighlightColor.b, HighlightColor.a);
    SDL_RenderFillRect(Renderer.GetRenderer(), &cellRect); 
}

String int_to_cstr(int num) {
    static char Buffer[50];
    sprintf(Buffer, "%d" , num);
    return Buffer;
}

void Sudoku::Frame::DrawString(String Text, SDL_Color Color , float alpha) {
    // Set the color with the specified alpha
    SDL_SetRenderDrawColor(Renderer.GetRenderer(), Color.r, Color.g, Color.b, (Uint8)(alpha * Color.a));

    sSurface Surface(Font ,Text, Color);
    sTexture Texture(Renderer , Surface);

    int TextWidth, TextHeight;
    SDL_SetTextureAlphaMod(Texture.GetTexture(), (Uint8) alpha * 255);
 
   // Get the width and height of the texture
    SDL_QueryTexture(Texture.GetTexture(), nullptr, nullptr, &TextWidth, &TextHeight);

    int X = (SCREEN_WIDTH - TextWidth)   / 2;
    int Y = (SCREEN_HEIGHT - TextHeight) / 2;
    int W = TextWidth;
    int H = TextHeight;

    Texture.Render(Renderer , X , Y , W , H);
}

// NOTE: Function for Drawing Color On the Frame
bool Sudoku::Frame::DrawNumber(int row, int col, int number, SDL_Color Color, float alpha) {

    String NumText = int_to_cstr(number);

    // Set the color with the specified alpha
    SDL_SetRenderDrawColor(Renderer.GetRenderer(), Color.r, Color.g, Color.b, (Uint8)(alpha * Color.a));
    
    sSurface Surface(Font, NumText , Color);
    sTexture Texture(Renderer, Surface);

    int TextWidth, TextHeight;
    SDL_SetTextureAlphaMod(Texture.GetTexture(), (Uint8)alpha*255); 
 
    // Get the width and height of the texture
    if(SDL_QueryTexture(Texture.GetTexture(), nullptr, nullptr, &TextWidth, &TextHeight) != 0) {
        std::cerr << "[ERROR]: Failed To Query Texture For Number: " << number << " " << SDL_GetError() << std::endl;
        return false;
    }

    int X = row * CELL_WIDTH  + (CELL_WIDTH - TextWidth) / 2;
    int Y = col * CELL_HEIGHT + (CELL_HEIGHT - TextHeight) / 2;
    int W = TextWidth;
    int H = TextHeight;

    std::cout << "[DEBUG]: Rendering number " << number << " at (" << X << ", " << Y << ")" << std::endl;

    Texture.Render(Renderer , X , Y , W , H);
    return true;
}

bool Sudoku::Frame::Solve() {
    if (ValidBoard(_Board.GetBoard())) {
        return true;
    }

    SDL_Color Color = {200, 200, 200, 200};
    SDL_Color HighlightCandidate = {150, 150, 150, 255};

    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            if(!CheckCellStatus(_Board.GetBoard(), i , j)) {
                int Count;
                int *Candidates = GetCandidates(_Board.GetBoard(), i , j, &Count);
                for (int k = 0; k < Count; ++k) {
                    SetCell(_Board.GetBoard(), i , j , Candidates[k]);
                    HighlightCell(i , j , HighlightCandidate);
                    DrawNumber(i , j , Candidates[k], Color , 1.0f);
                    SDL_RenderPresent(Renderer.GetRenderer());
                    SDL_Delay(7);

                    // Process events to keep the application responsive
                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            free(Candidates);
                            return false; // Exit if the user closes the window
                        }
                    }

                    if(Solve()) {
                        free(Candidates);
                        return true;
                    }

                    FreeCell(_Board.GetBoard(), i , j);
                }
                free(Candidates);
                return false;
            }
        }
    }
    return true;
}

// NOTE: Main Function
int main(void) {
    Sudoku::Frame F;
    if(!F.UpdateFrame()) {
        return 1;
    }
    return 0;
}
