
// Librerias utiliazadas
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Constantes de tamaño y ajustes del programa
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 1000;
const int MAX_CIRCLES = 750;
const int CIRCLE_RADIUS = 20;
const int MAX_SPEED = 5;

// Definición de la estructura Circle
struct Circle
{
    int x, y;        // Coordenadas del centro del círculo
    int dx, dy;      // Velocidad del círculo en cada eje
    SDL_Color color; // Color del círculo
};

SDL_Window *window = nullptr;     // Puntero a la ventana
SDL_Renderer *renderer = nullptr; // Puntero al renderer
std::vector<Circle> circles;      // Vector de círculos
bool quit = false;                // Variable para controlar el bucle principal

// Función para inicializar SDL
bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    // Crear ventana y renderer

    window = SDL_CreateWindow("ScreensaverSeq", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}
// Función para cerrar SDL
void close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
// Función para generar círculos aleatorios
void generateRandomCircles(int numCircles)
{
    circles.clear();
    std::srand(std::time(nullptr));

    // Generar círculos aleatorios
    for (int i = 0; i < numCircles; ++i)
    {
        Circle circle;
        circle.x = std::rand() % (SCREEN_WIDTH - CIRCLE_RADIUS * 2) + CIRCLE_RADIUS;
        circle.y = std::rand() % (SCREEN_HEIGHT - CIRCLE_RADIUS * 2) + CIRCLE_RADIUS;
        circle.dx = std::rand() % (MAX_SPEED * 2 + 1) - MAX_SPEED;
        circle.dy = std::rand() % (MAX_SPEED * 2 + 1) - MAX_SPEED;
        circle.color = {static_cast<Uint8>(std::rand() % 256), static_cast<Uint8>(std::rand() % 256), static_cast<Uint8>(std::rand() % 256)};
        circles.push_back(circle);
    }
}
// Función para mover los círculos ( se cambia la velocidad cuando tocan el borde)
void moveCircles()
{
    for (Circle &circle : circles)
    {
        circle.x += circle.dx;
        circle.y += circle.dy;

        if (circle.x <= CIRCLE_RADIUS || circle.x >= SCREEN_WIDTH - CIRCLE_RADIUS)
        {
            circle.dx = -circle.dx; // Cambio de dirección en el eje x
        }

        if (circle.y <= CIRCLE_RADIUS || circle.y >= SCREEN_HEIGHT - CIRCLE_RADIUS)
        {
            circle.dy = -circle.dy; // Cambio de dirección en el eje y
        }
    }

    // Verificar colisiones entre círculos
    for (size_t i = 0; i < circles.size(); ++i)
    {
        for (size_t j = i + 1; j < circles.size(); ++j)
        {
            int dx = circles[i].x - circles[j].x;
            int dy = circles[i].y - circles[j].y;
            int distanceSquared = dx * dx + dy * dy;

            if (distanceSquared <= 4 * CIRCLE_RADIUS * CIRCLE_RADIUS) // 2 * radio porque estamos comparando centros
            {
                // Invertir direcciones
                circles[i].dx = -circles[i].dx;
                circles[i].dy = -circles[i].dy;
                circles[j].dx = -circles[j].dx;
                circles[j].dy = -circles[j].dy;

                // Calcular la distancia real y el desplazamiento necesario para corregir la colisión
                float distance = sqrt(distanceSquared);
                float overlap = 2 * CIRCLE_RADIUS - distance;

                // Normalizar el vector de dirección
                float dxn = dx / distance;
                float dyn = dy / distance;

                // Mover los círculos fuera de la colisión
                circles[i].x += (overlap / 2) * dxn;
                circles[i].y += (overlap / 2) * dyn;
                circles[j].x -= (overlap / 2) * dxn;
                circles[j].y -= (overlap / 2) * dyn;
            }
        }
    }
}

// Función para dibujar un círculo relleno
void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, const SDL_Color &color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

    // Llenar el circulo con puntos dentro del radio
    for (int y = -radius; y <= radius; ++y)
    {
        for (int x = -radius; x <= radius; ++x)
        {
            if (x * x + y * y <= radius * radius)
            {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

// Función para renderizar la escena
void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Dibujar los círculos con la funcion drawFilledCircle
    for (const Circle &circle : circles)
    {
        drawFilledCircle(renderer, circle.x + CIRCLE_RADIUS, circle.y + CIRCLE_RADIUS, CIRCLE_RADIUS, circle.color);
    }

    SDL_RenderPresent(renderer);
}

// Función principal del programa
int main(int argc, char *argv[])
{
    std::vector<Uint32> executionTimes;
    if (!init())
    {
        return 1;
    }
    // Comprobar argumentos
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <numCircles>" << std::endl;
        return 1;
    }
    // Comprobar que el número de círculos es válido
    int numCircles = std::atoi(argv[1]);
    if (numCircles < 1 || numCircles > MAX_CIRCLES)
    {
        std::cerr << "Invalid number of circles. Please use a value between 1 and " << MAX_CIRCLES << std::endl;
        return 1;
    }
    // Generar círculos aleatorios
    generateRandomCircles(numCircles);

    // Variables para medir el tiempo
    Uint32 startTime = SDL_GetTicks();
    Uint32 prevTime = startTime;
    Uint32 totalTime = 0;
    int frames = 0;
    int totalFrames = 0;

    // Bucle principal
    while (!quit)
    {
        // Calcular el tiempo transcurrido y gestionar eventos SDL
        Uint32 startTime2 = SDL_GetTicks();
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Mover y renderizar los círculos
        moveCircles();
        render();

        // Calcular y mostrar FPS
        frames++;
        totalFrames++;
        totalTime += deltaTime;
        if (currentTime - startTime >= 1000)
        {
            Uint32 endTime = SDL_GetTicks();
            Uint32 elapsedTime = endTime - startTime2;
            executionTimes.push_back(elapsedTime);
            std::cout << "FPS: " << frames << std::endl;
            startTime = currentTime;
            frames = 0;
        }

        // Esperar para mantener 60 FPS
        if (deltaTime < 1000 / 120)
        {
            SDL_Delay(1000 / 120 - deltaTime);
        }
    }

    

    if (totalTime > 0) {
        float avgFPS = 1000.0f * totalFrames / totalTime;
        std::cout << "Average FPS: " << avgFPS << std::endl;
    } else {
        std::cout << "No frames were rendered." << std::endl;
    }
    Uint32 totalExecutionTime = 0;
    for (Uint32 time : executionTimes) {
        totalExecutionTime += time;
    }
    if (!executionTimes.empty()) {
        float avgExecutionTime = static_cast<float>(totalExecutionTime) / executionTimes.size();
        std::cout << "Average Execution Time: " << avgExecutionTime << " milliseconds" << std::endl;
    } else {
        std::cout << "No execution times recorded." << std::endl;
    }

    

    // Cerrar SDL
    close();
    // Salir del programa
    return 0;
}
