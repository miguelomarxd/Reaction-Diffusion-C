/**********************************************************************************************
 * Simulación de reacción-difusión basada en el modelo de Gray-Scott, implementada con Raylib.
 * El programa genera patrones dinámicos y complejos simulando interacciones químicas entre 
 * dos sustancias virtuales (A y B). Los resultados se muestran en una ventana gráfica, 
 * actualizándose en tiempo real.
 *
 * Funciones:
 * - InitReactionDiffusion: Inicializa las concentraciones de las sustancias A y B en la matriz.
 * - AddNoise: Introduce ruido aleatorio en las concentraciones de las sustancias A y B.
 * - UpdateReactionDiffusion: Calcula la reacción y difusión, actualizando las concentraciones.
 * - DrawSimulation: Dibuja la simulación en la ventana, representando las concentraciones en colores.
 **********************************************************************************************/

#include "raylib.h"

// Dimensiones de la ventana de simulación
#define WIDTH 600
#define HEIGHT 600
#define SCALE 1  // Factor de escala para el dibujo de la simulación

// Parámetros de la simulación de reacción-difusión (modelo de Gray-Scott)
#define A 0
#define B 1
#define F 0.03f  // Tasa de alimentación (afecta a A)
#define K 0.06f  // Tasa de eliminación (afecta a B)

// Matrices para las sustancias
// Matrices que contienen las concentraciones de las sustancias A y B
float gridA[WIDTH][HEIGHT], gridB[WIDTH][HEIGHT];
// Matrices para almacenar las concentraciones calculadas en el siguiente paso
float nextA[WIDTH][HEIGHT], nextB[WIDTH][HEIGHT];

// Inicializa las concentraciones iniciales de las sustancias A y B
void InitReactionDiffusion()
{
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            gridA[x][y] = 1.0f;  // Establece la concentración inicial de A
            gridB[x][y] = 0.0f;  // Establece la concentración inicial de B
        }
    }

    // Crea zonas aleatorias con alta concentración de B
    for (int i = 0; i < 100; i++)
    {
        int xStart = GetRandomValue(0, WIDTH - 20);
        int yStart = GetRandomValue(0, HEIGHT - 20);
        for (int x = xStart; x < xStart + 20; x++)
        {
            for (int y = yStart; y < yStart + 20; y++)
            {
                if (x < WIDTH && y < HEIGHT)
                    gridB[x][y] = 1.0f;  // Asigna concentración alta de B
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
                (unsigned char)(valueA * 255),  // Canal rojo según A
                (unsigned char)(valueB * 255),  // Canal verde según B
                (unsigned char)((valueA * 255 + valueB * 255) / 2),  // Azul como mezcla de A y B
                255
            };
            DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, color);
        }
    }
}

int main()
{
    InitWindow(WIDTH * SCALE, HEIGHT * SCALE, "Reaction-Diffusion usando Raylib");
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

    CloseWindow();

    return 0;
}
