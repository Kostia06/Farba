#include "farba.hpp"

int main() {
    Farba farba = Farba();
    farba.importFont("default", "regular.otf", 22);
    farba.setFont("default", 30);

    while(farba.update(0)){
        farba.setColor(255, 0, 0);
        farba.text("Hello, World!", 100, 100);
        farba.setColor(0, 255, 0);
        farba.rect(200, 200, 100, 100, false);
        farba.setColor(0, 0, 255, 100);
        farba.circle(300, 300, 150, true);
        farba.setColor(255, 255, 255); 
        farba.line(0, 0, 640, 480, 50, 50); 
    }

    return 0;
}

