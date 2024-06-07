#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>
#include <assert.h>
#include <map>

#include "input.hpp"

using namespace std;

class Farba{
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        int windowWidth = 800;
        int windowHeight = 600;
        
        int mouse[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int mouseWait[3] = {0, 0, 0};

        bool* keys;
        bool* keysWait;

        double startDeltaTime = SDL_GetPerformanceCounter();

        TTF_Font* currentFont = nullptr;
        std::map<std::string, TTF_Font*> fonts;

        SDL_Color color = {255, 255, 255, 255};
        // handle events
        void handleEvent(){
            SDL_Event event;
            if(!SDL_PollEvent(&event)) return; 
            switch(event.type){
                // quit the window
                case SDL_QUIT:{ running = false; break; }
                // handle the mouse events
                case SDL_MOUSEBUTTONDOWN:{
                    if(event.button.button == SDL_BUTTON_LEFT){ 
                        mouse[MOUSE_LEFT] = 1; 
                        if(mouseWait[MOUSE_LEFT] == 0){ 
                            mouseWait[MOUSE_LEFT] = 1;  mouse[LEFT_PRESSED] = 1;
                        }
                    }
                    if(event.button.button == SDL_BUTTON_MIDDLE){ 
                        mouse[MOUSE_MIDDLE] = 1; 
                        if(mouseWait[MOUSE_MIDDLE] == 0){ 
                            mouseWait[MOUSE_MIDDLE] = 1;  mouse[MIDDLE_PRESSED] = 1;
                        }
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT){ 
                        mouse[MOUSE_RIGHT] = 1;
                        if(mouseWait[MOUSE_RIGHT] == 0){ 
                            mouseWait[MOUSE_RIGHT] = 1; mouse[RIGHT_PRESSED] = 1;
                        }
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP:{
                    if(event.button.button == SDL_BUTTON_LEFT){ 
                        mouse[MOUSE_LEFT] = 0;  mouseWait[MOUSE_LEFT] = 0;
                    }
                    if(event.button.button == SDL_BUTTON_MIDDLE){ 
                        mouse[MOUSE_MIDDLE] = 0;  mouseWait[MOUSE_MIDDLE] = 0;
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT){ 
                        mouse[MOUSE_RIGHT] = 0;   mouseWait[MOUSE_RIGHT] = 0;
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
                    if(keysWait[key] == false) keys[key] = true; 
                    break;
                }
                case SDL_KEYUP:{
                    int key = event.key.keysym.sym;
                    keys[key] = false;
                    keysWait[key] = false;
                    break;
                }
            }
        }
    public:

        string title;
        bool running = true;

        double deltaTime = 0;
        double fps = 0;

        // constructor
        Farba(string title = "Farba", size_t windowWidth = 800, size_t windowHeight = 600) : title(title), windowWidth(windowWidth), windowHeight(windowHeight){
            assert(SDL_Init(SDL_INIT_VIDEO) >= 0 && "SDL_Init failed"); 
            assert(IMG_Init(IMG_INIT_PNG) >= 0 && "IMG_Init failed");
            assert(TTF_Init() >= 0 && "TTF_Init failed");
            // keys
            keys = new bool[MAX_KEYS];
            keysWait = new bool[MAX_KEYS];
            // window and renderer
            window = SDL_CreateWindow(
                title.c_str(), 
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                windowWidth, windowHeight, 
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
        ~Farba(){
            if(renderer != nullptr) SDL_DestroyRenderer(renderer); 
            if(window != nullptr) SDL_DestroyWindow(window); 
            TTF_Quit();
            IMG_Quit();
            SDL_Quit();
        }

        // main loop
        bool update(int fixedFps = 60){
            // reset the settings
            mouse[LEFT_PRESSED] = 0;
            mouse[MIDDLE_PRESSED] = 0;
            mouse[RIGHT_PRESSED] = 0;
            mouse[SCROLL_X] = 0;
            mouse[SCROLL_Y] = 0;
            // calculate the delta time
            double desiredDeltaTime = 1000.0f / fixedFps;
            deltaTime = static_cast<double>(SDL_GetPerformanceCounter() - startDeltaTime) / SDL_GetPerformanceFrequency();
            fps = 1.0f / deltaTime;
            startDeltaTime = SDL_GetPerformanceCounter();
            if(deltaTime < desiredDeltaTime && fixedFps > 0) SDL_Delay((desiredDeltaTime - deltaTime));
            // clear the renderer
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderPresent(renderer);
            SDL_RenderClear(renderer);
            // handle the events
            handleEvent();
            return running;
        }

        // check if the key is pressed
        bool keyPressed(int key){
            if(key < MAX_KEYS) return keys[key]; 
            return false;
        }

        // check if the mouse button is pressed
        bool mousePressed(int button){
            if(button < MAX_MOUSE) return mouse[button]; 
            return false;
        }

        // set color
        void setColor(int r, int g, int b, int a = 255){ 
            color.r = r; color.g = g; color.b = b; color.a = a; 
        }
        void setColor(int hex){
            color.r = (hex >> 16) & 0xFF;
            color.g = (hex >> 8) & 0xFF;
            color.b = hex & 0xFF;
            color.a = 255;
        }
        // set the font
        bool setFont(string fontName, int fontSize){
            if(fonts.find(fontName) == fonts.end()) return false;
            currentFont = fonts[fontName];
            TTF_SetFontSize(currentFont, fontSize);
            return true;
        }
        // import font
        bool importFont(string fontName, string fontPath, int fontSize){
            TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
            if(font == nullptr) return false;
            fonts[fontName] = font;
            return true;
        }
        // draw text
        bool text(string text, int x, int y){
            if(currentFont == nullptr) return false;
            SDL_Surface* surface = TTF_RenderText_Blended(currentFont, text.c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect rect = {x, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
            return true;
        }
        // draw rectangle
        void rect(int x, int y, size_t w, size_t h, bool fill = true, size_t width = 1){
            SDL_Rect rect = {x, y, (int)w, (int)h};
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            if(fill){ SDL_RenderFillRect(renderer, &rect);  return; }
            for(int i = 0; i < width; i++){
                SDL_RenderDrawRect(renderer, &rect);
                rect.x++; rect.y++; rect.w -= 2; rect.h -= 2;
            }
        }
        // draw circle
        void circle(int x, int y, size_t r, bool fill = true, size_t width = 1){
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            if(fill){
                for(int i = 0; i < r; i++){
                    for(int j = 0; j < 360; j++){
                        double angle = j * M_PI / 180;
                        SDL_RenderDrawPoint(renderer, x + i * cos(angle), y + i * sin(angle));
                    }
                }
                return;
            }
            for(int i = 0; i < width; i++){
                for(int j = 0; j < 360; j++){
                    double angle = j * M_PI / 180;
                    SDL_RenderDrawPoint(renderer, x + r * cos(angle), y + r * sin(angle));
                }
                r--;
            }
        }
        // draw a line
        void line(int x1, int y1, int x2, int y2, size_t wThickness = 1, size_t hThickness = 1){
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            for(int i = 0; i < wThickness; i++){
                for(int j = 0; j < hThickness; j++){
                    SDL_RenderDrawLine(renderer, x1 + i, y1 + j, x2 + i, y2 + j);
                }
            }
        }
        // draw image
        void image(string path, int x, int y, int w = 0, int h = 0){
            SDL_Surface* surface = IMG_Load(path.c_str());
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect rect = {x, y, w == 0 ? surface->w : w, h == 0 ? surface->h : h};
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
        // get font size
        int getFontWidth(string text = "A"){
            int w, h;
            TTF_SizeText(currentFont, text.c_str(), &w, &h);
            return w;
        }
        int getFontHeight(string text = "A"){
            int w, h;
            TTF_SizeText(currentFont, text.c_str(), &w, &h);
            return h;
        }
};
