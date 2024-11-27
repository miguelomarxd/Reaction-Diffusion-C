/* 
Este código implementa una simulación de reacción-difusión basada en el modelo de Gray-Scott, utilizando la biblioteca Raylib para renderizar los resultados en tiempo real. La simulación emplea dos sustancias (A y B), cuyos comportamientos interactivos generan patrones dinámicos. La simulación está influenciada por una imagen base que define las concentraciones iniciales de A y B, y se actualiza con parámetros ajustables que controlan la difusión, la reacción y el ruido aleatorio.

Funciones del código:
- InitReactionDiffusion: Configura las condiciones iniciales de la simulación basándose en una imagen, asignando valores iniciales a las sustancias A y B.
- AddNoise: Introduce ruido aleatorio en las concentraciones de las sustancias para aumentar la variabilidad del sistema.
- UpdateReactionDiffusion: Actualiza las concentraciones de las sustancias utilizando las ecuaciones del modelo Gray-Scott, calculando difusión, reacción y efectos de los parámetros.
- DrawSimulation: Renderiza la simulación en la ventana de Raylib, convirtiendo las concentraciones de A y B en colores.
*/

#include "raylib.h"

// Dimensiones de la ventana de simulación
#define WIDTH 600
#define HEIGHT 600
#define SCALE 1  // Factor de escala para el dibujo de la simulación

// Configuración de la simulación
#define A 0
#define B 1
#define F 0.03f  // Influye en la tasa de reacción
#define K 0.06f  // Controla la remoción de B

// Matrices para las sustancias
// Matrices que contienen las concentraciones de las sustancias A y B
float gridA[WIDTH][HEIGHT], gridB[WIDTH][HEIGHT];
// Matrices para almacenar las concentraciones calculadas en el siguiente paso
float nextA[WIDTH][HEIGHT], nextB[WIDTH][HEIGHT];

// Cargar la imagen para usarla como base
Image logoImage;

/* 
Configura las condiciones iniciales de la simulación utilizando una imagen. 
Los píxeles oscuros corresponden a valores altos de A y bajos de B.
*/
void InitReactionDiffusion()
{
    // Carga la imagen desde la carpeta "images" y se ajusta la imagen al tamaño de la simulación
    logoImage = LoadImage("images/Batman-Logo.png"); 
    ImageResize(&logoImage, WIDTH, HEIGHT);
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            Color pixelColor = GetImageColor(logoImage, x, y);

            // Determina los valores iniciales basados en el color
            if (pixelColor.r < 128) 
            {
                gridA[x][y] = 1.0f; 
                gridB[x][y] = 0.0f; 
            }
            else
            {
                gridA[x][y] = 0.0f; 
                gridB[x][y] = 1.0f; 
            }
        }
    }
}

// Introduce ruido aleatorio en las concentraciones de las sustancias A y B
void AddNoise()
{
    for (int x = 1; x < WIDTH - 1; x++)
    {
        for (int y = 1; y < HEIGHT - 1; y++)
        {
            // Agrega ruido con un 5% de probabilidad
            if (GetRandomValue(0, 100) < 5) 
            {
                // Modifica A ligeramente
                gridA[x][y] += GetRandomValue(-0.1f, 0.1f); 
                // Modifica B ligeramente
                gridB[x][y] += GetRandomValue(-0.1f, 0.1f); 
            }
        }
    }
}

// Calcula la difusión y la reacción química, actualizando las concentraciones de A y B
void UpdateReactionDiffusion()
{
    for (int x = 1; x < WIDTH - 1; x++)
    {
        for (int y = 1; y < HEIGHT - 1; y++)
        {
            float a = gridA[x][y];
            float b = gridB[x][y];

            float lapA = gridA[x-1][y] + gridA[x+1][y] + gridA[x][y-1] + gridA[x][y+1] - 4.0f * a;
            float lapB = gridB[x-1][y] + gridB[x+1][y] + gridB[x][y-1] + gridB[x][y+1] - 4.0f * b;

            float reaction = a * b * b;

            nextA[x][y] = a + (lapA - reaction + F * (1.0f - a)) * 0.05f;
            nextB[x][y] = b + (lapB + reaction - (F + K) * b) * 0.05f;
        }
    }

    // Copia las concentraciones calculadas en la matriz principal
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            gridA[x][y] = nextA[x][y];
            gridB[x][y] = nextB[x][y];
        }
    }
}

/* 
Dibuja la simulación en la ventana, convirtiendo las concentraciones de A y B 
en valores de color para los píxeles.
*/
void DrawSimulation()
{
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            float valueA = gridA[x][y];
            float valueB = gridB[x][y];

            Color color = (Color){
                (unsigned char)(valueA * 255),
                (unsigned char)(valueB * 255),
                (unsigned char)(valueA * 255 + valueB * 255) / 2,
                255
            };
            DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, color);
        }
    }
}

int main()
{
    InitWindow(WIDTH * SCALE, HEIGHT * SCALE, "Reaction-Diffusion formando un logo");
    SetTargetFPS(60);

    InitReactionDiffusion();

    while (!WindowShouldClose())
    {
        AddNoise();
        UpdateReactionDiffusion();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawSimulation();
        DrawText("ESC para salir", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    UnloadImage(logoImage);
    CloseWindow();

    return 0;
}
