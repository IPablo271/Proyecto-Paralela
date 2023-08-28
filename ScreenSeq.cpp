#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAX_CIRCLES = 100;
const int CIRCLE_RADIUS = 20;
const int MAX_SPEED = 5;

struct Circle {
    int x, y;
    int dx, dy;
    SDL_Color color;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
std::vector<Circle> circles;
bool quit = false;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Screensaver", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void generateRandomCircles(int numCircles) {
    circles.clear();
    std::srand(std::time(nullptr));
    
    for (int i = 0; i < numCircles; ++i) {
        Circle circle;
        circle.x = std::rand() % (SCREEN_WIDTH - CIRCLE_RADIUS * 2) + CIRCLE_RADIUS;
        circle.y = std::rand() % (SCREEN_HEIGHT - CIRCLE_RADIUS * 2) + CIRCLE_RADIUS;
        circle.dx = std::rand() % (MAX_SPEED * 2 + 1) - MAX_SPEED;
        circle.dy = std::rand() % (MAX_SPEED * 2 + 1) - MAX_SPEED;
        circle.color = {static_cast<Uint8>(std::rand() % 256), static_cast<Uint8>(std::rand() % 256), static_cast<Uint8>(std::rand() % 256)};
        circles.push_back(circle);
    }
}

void moveCircles() {
    for (Circle& circle : circles) {
        circle.x += circle.dx;
        circle.y += circle.dy;
        
        if (circle.x <= 0 || circle.x >= SCREEN_WIDTH - CIRCLE_RADIUS * 2) {
            circle.dx = -circle.dx;
        }
        
        if (circle.y <= 0 || circle.y >= SCREEN_HEIGHT - CIRCLE_RADIUS * 2) {
            circle.dy = -circle.dy;
        }
    }
}

void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, const SDL_Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (const Circle& circle : circles) {
        drawFilledCircle(renderer, circle.x + CIRCLE_RADIUS, circle.y + CIRCLE_RADIUS, CIRCLE_RADIUS, circle.color);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (!init()) {
        return 1;
    }

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <numCircles>" << std::endl;
        return 1;
    }

    int numCircles = std::atoi(argv[1]);
    if (numCircles < 1 || numCircles > MAX_CIRCLES) {
        std::cerr << "Invalid number of circles. Please use a value between 1 and " << MAX_CIRCLES << std::endl;
        return 1;
    }

    generateRandomCircles(numCircles);

    Uint32 startTime = SDL_GetTicks();
    Uint32 prevTime = startTime;
    int frames = 0;

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        moveCircles();
        render();

        frames++;
        if (currentTime - startTime >= 1000) {
            std::cout << "FPS: " << frames << std::endl;
            startTime = currentTime;
            frames = 0;
        }

        if (deltaTime < 1000 / 60) {
            SDL_Delay(1000 / 60 - deltaTime);
        }
    }

    close();

    return 0;
}
