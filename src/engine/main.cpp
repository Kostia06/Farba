#include "include.hpp"

#define DEFAULT_COLOR  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
// mouse input
static int mouse[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int mouse_pressed[3] = {0, 0, 0};

// keyboard input
static int  current_key = 0;
static bool keys[KEY_NUM] = {false};
static bool keys_wait[KEY_NUM] = {false};

// window and renderer are global variables
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static int fps = 60;
static Uint32 current_time = SDL_GetTicks();
static Uint32 last_time = current_time;
static float dt = 0.0f;

// set fonts
static TTF_Font* current_font = nullptr;
static std::map<std::string, TTF_Font*> fonts;

// current color
static SDL_Color color = {255, 255, 255, 255};

// shadowing the functions
void HandleMouse(SDL_Event event);
void HandleKeyboard(SDL_Event event);
bool HandleEvent(SDL_Event event);

// sets the window and renderer
int InitWindow(int width, int height, std::string title){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){ return -1; }
    window = SDL_CreateWindow(
        title.c_str(), 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if(window == nullptr){ return -1; }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr){ return -1; }
    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return 0;
}
// sets the image
int InitImage(){
    if(IMG_Init(IMG_INIT_PNG) < 0){ return -1; }
    return 0;
}
// sets the font
int InitFont(){
    if(TTF_Init() < 0){ return -1; }
    return 0;
}
// quits the window
void QuitWindow(){
    if(renderer != nullptr){ SDL_DestroyRenderer(renderer); }
    if(window != nullptr){ SDL_DestroyWindow(window); }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
// updates window, updates the window, sets the fps, delta time and handles the events
bool UpdateWindow(size_t r, size_t b, size_t g, size_t a){
    // reset the values
    mouse[MOUSE_LEFT_PRESSED] = 0;
    mouse[MOUSE_MIDDLE_PRESSED] = 0;
    mouse[MOUSE_RIGHT_PRESSED] = 0;
    mouse[MOUSE_SCROLL_X] = 0;
    mouse[MOUSE_SCROLL_Y] = 0;
    current_key = 0;
    // handle the events
    SDL_Event event;
    if(SDL_PollEvent(&event)){
        if(!HandleEvent(event)){ return false; } 
    }
    // Set FPS
    current_time = SDL_GetTicks();
    dt = (current_time - last_time) / 1000.0f;
    last_time = current_time;

    SDL_SetRenderDrawColor(renderer, r, b, g, a);
    SDL_RenderClear(renderer);
    return true;
}
// ends the update window and sets the delay
void PresentWindow(){
   int delay = 1000 / fps;
    if (SDL_GetTicks() - current_time < delay) {
        SDL_Delay(delay - (SDL_GetTicks() - current_time));
    }
    SDL_RenderPresent(renderer);
}
int* GetWindowSize(){
    static int size[2];
    SDL_GetWindowSize(window, &size[0], &size[1]);
    size[0] *= 2;
    size[1] *= 2;
    return size;
}
// sets the fps
void SetFPS(int fps){ ::fps = fps; }
// gets the delta time
float GetDeltaTime(){ return dt; }
// handles the mouse input
void HandleMouse(SDL_Event event){
    if(event.type == SDL_MOUSEBUTTONDOWN){
        if(event.button.button == SDL_BUTTON_LEFT){ 
            mouse[MOUSE_LEFT] = 1; 
            if(mouse_pressed[MOUSE_LEFT] == 0){ 
                mouse_pressed[MOUSE_LEFT] = 1;  mouse[MOUSE_LEFT_PRESSED] = 1;
            }
        }
        if(event.button.button == SDL_BUTTON_MIDDLE){ 
            mouse[MOUSE_MIDDLE] = 1; 
            if(mouse_pressed[MOUSE_MIDDLE] == 0){ 
                mouse_pressed[MOUSE_MIDDLE] = 1;  mouse[MOUSE_MIDDLE_PRESSED] = 1;
            }
        }
        if(event.button.button == SDL_BUTTON_RIGHT){ 
            mouse[MOUSE_RIGHT] = 1;
            if(mouse_pressed[MOUSE_RIGHT] == 0){ 
                mouse_pressed[MOUSE_RIGHT] = 1; mouse[MOUSE_RIGHT_PRESSED] = 1;
            }
        }
    }
    if(event.type == SDL_MOUSEBUTTONUP){
        if(event.button.button == SDL_BUTTON_LEFT){ 
            mouse[MOUSE_LEFT] = 0;  mouse_pressed[MOUSE_LEFT] = 0;
        }
        if(event.button.button == SDL_BUTTON_MIDDLE){ 
            mouse[MOUSE_MIDDLE] = 0;  mouse_pressed[MOUSE_MIDDLE] = 0;
        }
        if(event.button.button == SDL_BUTTON_RIGHT){ 
            mouse[MOUSE_RIGHT] = 0;   mouse_pressed[MOUSE_RIGHT] = 0;
        }
    }
    // mouse roll down and roll up
    if(event.type == SDL_MOUSEWHEEL){
        mouse[MOUSE_SCROLL_X] = event.wheel.x;  mouse[MOUSE_SCROLL_Y] = event.wheel.y;
    }
    // mouse position
    if(event.type == SDL_MOUSEMOTION){
        mouse[MOUSE_X] = event.motion.x*2;  mouse[MOUSE_Y] = event.motion.y*2;
    }
}
// handles the keyboard input
void HandleKeyboard(SDL_Event event){
    if(event.type == SDL_KEYDOWN){
        current_key= event.key.keysym.sym;
        if(current_key > 255){ current_key = current_key % 10000; }
        if(keys_wait[current_key] == false){
            keys[current_key] = true;
        }
    }
    if(event.type == SDL_KEYUP){
        int key = event.key.keysym.sym;
        if(key > 255){ key = key % 10000; }
        keys[key] = false;
        keys_wait[key] = false;
    }
}
// handles the events
bool HandleEvent(SDL_Event event){
    if(event.type == SDL_QUIT){ return false; }
    HandleMouse(event);
    HandleKeyboard(event);
    return true;
}
// gets the mouse input
int MouseGet(int button){ return mouse[button]; }
// checks if the mouse is hovering
bool MouseHovering(int x, int y, int w, int h){return InRange(mouse[MOUSE_X], mouse[MOUSE_Y], x, y, w, h); }
// gets the keyboard input
bool* KeysGet(){ return keys; }
// gets the keyboard
int KeyGet(){ return current_key; }
// make keyboard wait for a key to go up
void Keywait(int key){ keys_wait[key] = true; keys[key] = false; }
// creates the font
bool CreateFont(std::string name, std::string path, int size){
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if(font == nullptr){ return false; }
    fonts[name] = font;
    return true;
}
// set color
void SetColor(int r, int g, int b, int a){ color.r = r; color.g = g; color.b = b; color.a = a;  }
void SetColor(int hex){ 
    color.r = (hex >> 16) & 0xFF;
    color.g = (hex >> 8) & 0xFF;
    color.b = hex & 0xFF;
    color.a = 255;
}
// sets the font
bool SetFont(std::string name){
    if(fonts[name] == nullptr){ return false; }
    current_font = fonts[name];
    return true;
}
// gets the size of the font / text
int* FontGetSize(std::string text){ 
    static int size[2];
    TTF_SizeText(current_font, text.c_str(), &size[0], &size[1]);
    return size;
}
// set size of the font
void SetFontSize(int size){  TTF_SetFontSize(current_font, size); }
// display the text with x, y and size of the text
void DrawText(std::string text, int x, int y, int text_size){
    SDL_Surface* surface = TTF_RenderText_Blended(current_font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w*text_size, surface->h*text_size};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
// draw rect with size
void DrawRect(int x, int y, size_t w, size_t h){
    DEFAULT_COLOR;
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_RenderFillRect(renderer, &rect);
}
// draw circle with size
void DrawCircle(int center_x, int center_y, size_t d, size_t fill){
    DEFAULT_COLOR;
    int times = d * fill / 100;
    for(int i = 0; i < times; i++){
        for( int j = 0; j < 360; j++){
            int radius = (d -i) / 2 ;
            float x = center_x + radius * cos(j * M_PI / 180);
            float y = center_y + radius * sin(j * M_PI / 180);
            SDL_RenderDrawPoint(renderer, x, y); 
        }
    }
}
// draw line with size
void DrawLine(int x1, int y1, int x2, int y2, int line_size){  
    DEFAULT_COLOR;
    int half = line_size / 2;
    if( line_size % 2 != 0){ half += 1; }
    for(int i = -half; i < half; i++){
        for(int j = -half; j < half; j++){
            SDL_RenderDrawLine(renderer, x1 + i, y1 + j, x2 + i, y2 + j);
        }
    }
}
