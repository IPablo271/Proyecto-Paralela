# Screensaver con SDL en C++ Secuencial

Este proyecto es un programa simple de "screensaver" desarrollado en C++ utilizando la librería Simple DirectMedia Layer (SDL). El programa crea una serie de círculos que se mueven por la pantalla, rebotando en los bordes y generando un efecto visual similar a un screensaver clásico.

## Funcionalidad

El programa permite:

- Generar y mostrar círculos en la pantalla.
- Controlar la cantidad de círculos generados desde la línea de comandos.
- Animar los círculos, haciendo que se muevan y reboten en los bordes.
- Mostrar el número de fotogramas por segundo (FPS) en los que se está ejecutando el programa.

## Funciones Principales

El código del programa incluye las siguientes funciones principales:

1. **init()**: Inicializa la librería SDL y crea una ventana y un renderer.

2. **close()**: Cierra la ventana y el renderer, liberando los recursos de SDL.

3. **generateRandomCircles(int numCircles)**: Genera círculos aleatorios con coordenadas, velocidades y colores aleatorios.

4. **moveCircles()**: Actualiza las posiciones de los círculos y maneja los rebotes en los bordes de la pantalla.

5. **drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, const SDL_Color& color)**: Dibuja un círculo relleno utilizando puntos para simular la apariencia.

6. **render()**: Borra la pantalla, dibuja los círculos y presenta el resultado en la ventana.

7. **main(int argc, char* argv[])**: Función principal del programa, donde se inicializa SDL, se manejan los argumentos de la línea de comandos, se genera la animación de los círculos y se controla la lógica del programa.

## Uso

Para compilar y ejecutar el programa, sigue los siguientes pasos:

1. Asegúrate de tener instalada la librería SDL.
```bash
sudo apt install libsdl2-dev

brew install sdl2
```
2. Compila el programa utilizando el compilador de C++ (g++ u otro similar) con el flag `-lSDL2`.
```bash
g++ -o screenSeq ScreenSeq.cpp -lSDL2
```
3. Ejecuta el programa y proporciona el número de círculos como argumento en la línea de comandos.
```bash
./ screenSeq 100
```

## Autores

#### Pablo Gonzalez  iPablo271
#### Jose Hernandez  her20053
#### Javier Mombiela javim7