#include "farba.hpp"

// constructor
Farba::Farba(string title, size_t window_width, size_t window_height) : title(title), window_width(window_width), window_height(window_height){
    assert(SDL_Init(SDL_INIT_VIDEO) >= 0 && "SDL_Init failed"); 
    assert(IMG_Init(IMG_INIT_PNG) >= 0 && "IMG_Init failed");
    assert(TTF_Init() >= 0 && "TTF_Init failed");
    // keys
    keys = new bool[MAX_KEYS];
    keys_wait = new bool[MAX_KEYS];
    // window and renderer
    window = SDL_CreateWindow(
        title.c_str(), 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        window_width, window_height, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    assert(window != nullptr && "SDL Window failed");
    renderer = SDL_CreateRenderer(
        window, 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    assert(renderer != nullptr && "SDL Renderer failed");
    // renderer settings
    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

// destructor
Farba::~Farba(){
    if(renderer != nullptr){ SDL_DestroyRenderer(renderer); }
    if(window != nullptr){ SDL_DestroyWindow(window); }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// main loop
bool Farba::update(int fixed_fps){
    // show the window
    SDL_RenderPresent(renderer);
    // handle the events
    handle_event();
    // calculate the delta time
    current_time = SDL_GetPerformanceCounter();
    delta_time = static_cast<double>(current_time - last_time) / frequency;
    last_time = current_time;
    // clear the window
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // reset the settings
    mouse[LEFT_PRESSED] = 0;
    mouse[MIDDLE_PRESSED] = 0;
    mouse[RIGHT_PRESSED] = 0;
    mouse[SCROLL_X] = 0;
    mouse[SCROLL_Y] = 0;
    return running;
}

// handle events
void Farba::handle_event(){
    SDL_Event event;
    if(!SDL_PollEvent(&event)){ return; }
    switch(event.type){
        // quit the window
        case SDL_QUIT:{ running = false; break; }
        // handle the mouse events
        case SDL_MOUSEBUTTONDOWN:{
            if(event.button.button == SDL_BUTTON_LEFT){ 
                mouse[MOUSE_LEFT] = 1; 
                if(mouse_wait[MOUSE_LEFT] == 0){ 
                    mouse_wait[MOUSE_LEFT] = 1;  mouse[LEFT_PRESSED] = 1;
                }
            }
            if(event.button.button == SDL_BUTTON_MIDDLE){ 
                mouse[MOUSE_MIDDLE] = 1; 
                if(mouse_wait[MOUSE_MIDDLE] == 0){ 
                    mouse_wait[MOUSE_MIDDLE] = 1;  mouse[MIDDLE_PRESSED] = 1;
                }
            }
            if(event.button.button == SDL_BUTTON_RIGHT){ 
                mouse[MOUSE_RIGHT] = 1;
                if(mouse_wait[MOUSE_RIGHT] == 0){ 
                    mouse_wait[MOUSE_RIGHT] = 1; mouse[RIGHT_PRESSED] = 1;
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:{
            if(event.button.button == SDL_BUTTON_LEFT){ 
                mouse[MOUSE_LEFT] = 0;  mouse_wait[MOUSE_LEFT] = 0;
            }
            if(event.button.button == SDL_BUTTON_MIDDLE){ 
                mouse[MOUSE_MIDDLE] = 0;  mouse_wait[MOUSE_MIDDLE] = 0;
            }
            if(event.button.button == SDL_BUTTON_RIGHT){ 
                mouse[MOUSE_RIGHT] = 0;   mouse_wait[MOUSE_RIGHT] = 0;
            }
            break;
        }
        case SDL_MOUSEWHEEL:{
            mouse[SCROLL_X] = event.wheel.x;  mouse[SCROLL_Y] = event.wheel.y;
            break;
        }
        case SDL_MOUSEMOTION:{
            mouse[X] = event.motion.x*2;  mouse[Y] = event.motion.y*2;
            break;
        }    
        case SDL_KEYDOWN:{
            int key = event.key.keysym.sym;
            if(keys_wait[key] == false){ keys[key] = true; }
            break;
        }
        case SDL_KEYUP:{
            int key = event.key.keysym.sym;
            keys[key] = false;
            keys_wait[key] = false;
            break;
        }
    }
}

// check if the key is pressed
bool Farba::key_pressed(int key){
    if(key < MAX_KEYS){ return keys[key]; }
    return false;
}

// check if the mouse button is pressed
bool Farba::mouse_pressed(int button){
    if(button < MAX_MOUSE){ return mouse[button]; }
    return false;
}

// get the delta time
double Farba::get_delta_time(){ return delta_time; }
