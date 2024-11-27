/* 
Este código implementa una simulación de reacción-difusión basada en el modelo de Gray-Scott, utilizando la biblioteca Raylib para renderizar los resultados en tiempo real. 
La simulación modela la interacción entre dos sustancias químicas (A y B), cuyas concentraciones evolucionan en el tiempo mediante difusión y reacción, generando patrones complejos.

Funciones del código:
- InitReactionDiffusion: Configura las condiciones iniciales de la simulación, asignando valores predeterminados a las sustancias A y B. Introduce zonas aleatorias ("piscinas") con alta concentración de B.
- AddNoise: Introduce variaciones aleatorias en las concentraciones de A y B para simular ruido y añadir variabilidad al sistema.
- UpdateReactionDiffusion: Calcula la evolución de las concentraciones de A y B utilizando las ecuaciones del modelo Gray-Scott. Implementa difusión, reacción y actualizaciones basadas en los parámetros F y K.
- DrawSimulation: Dibuja la simulación en la ventana de Raylib. Las concentraciones de A y B se convierten en colores visualizados en una cuadrícula.
*/


#include "raylib.h"

#define WIDTH 600
#define HEIGHT 600
#define SCALE 1

// Configuración de la simulación
#define A 0
#define B 1
#define F 0.03f  // F más bajo para reacciones más suaves
#define K 0.06f  // K moderado para un balance adecuado

// Matrices para las sustancias
float gridA[WIDTH][HEIGHT], gridB[WIDTH][HEIGHT];
float nextA[WIDTH][HEIGHT], nextB[WIDTH][HEIGHT];

// Inicialización de la simulación con más piscinas de B
void InitReactionDiffusion()
{
    // Rellenar con valores iniciales para A y B
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            gridA[x][y] = 1.0f;  // Sustancia A inicial
            gridB[x][y] = 0.0f;  // Sustancia B inicial
        }
    }

    // Introducir más "piscinas" de B en lugares aleatorios
    for (int i = 0; i < 100; i++) {  // Más piscinas de B
        int xStart = GetRandomValue(0, WIDTH - 20);
        int yStart = GetRandomValue(0, HEIGHT - 20);
        for (int x = xStart; x < xStart + 20; x++)
        {
            for (int y = yStart; y < yStart + 20; y++)
            {
                if (x < WIDTH && y < HEIGHT) {
                    gridB[x][y] = 1.0f;  // Zona de B en posiciones aleatorias
                }
            }
        }
    }
}

// Introducir ruido aleatorio de forma más agresiva
void AddNoise()
{
    for (int x = 1; x < WIDTH - 1; x++)
    {
        for (int y = 1; y < HEIGHT - 1; y++)
        {
            if (GetRandomValue(0, 100) < 5)  // Aumentamos la probabilidad de ruido
            {
                // Introducir ruido aleatorio en las concentraciones
                gridA[x][y] += GetRandomValue(-0.1f, 0.1f);  // Aumento mayor en el ruido de A
                gridB[x][y] += GetRandomValue(-0.1f, 0.1f);  // Aumento mayor en el ruido de B
            }
        }
    }
}

// Actualizar el sistema de reacción-difusión
void UpdateReactionDiffusion()
{
    for (int x = 1; x < WIDTH - 1; x++)
    {
        for (int y = 1; y < HEIGHT - 1; y++)
        {
            // Calcular las nuevas concentraciones de A y B usando el modelo de Gray-Scott
            float a = gridA[x][y];
            float b = gridB[x][y];

            // Difusión de A y B
            float lapA = gridA[x-1][y] + gridA[x+1][y] + gridA[x][y-1] + gridA[x][y+1] - 4.0f * a;
            float lapB = gridB[x-1][y] + gridB[x+1][y] + gridB[x][y-1] + gridB[x][y+1] - 4.0f * b;

            // Reacción de A y B
            float reaction = a * b * b;

            // Calcular nuevas concentraciones con más variabilidad
            nextA[x][y] = a + (lapA - reaction + F * (1.0f - a)) * 0.05f;
            nextB[x][y] = b + (lapB + reaction - (F + K) * b) * 0.05f;
        }
    }

    // Copiar los resultados a la matriz original
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            gridA[x][y] = nextA[x][y];
            gridB[x][y] = nextB[x][y];
        }
    }
}

// Dibuja el resultado en la pantalla
void DrawSimulation()
{
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            // Usar la sustancia A para el color de los píxeles
            float valueA = gridA[x][y];
            float valueB = gridB[x][y];

            // Mezclar A y B para obtener colores más complejos
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
    // Inicialización
    InitWindow(WIDTH * SCALE, HEIGHT * SCALE, "Raylib Reaction-Diffusion");
    SetTargetFPS(60);  // Mantener un FPS alto para un efecto más rápido

    // Inicializar la simulación
    InitReactionDiffusion();

    // Bucle principal
    while (!WindowShouldClose())
    {
        // Agregar ruido y nuevas "piscinas"
        AddNoise();

        // Actualizar simulación
        UpdateReactionDiffusion();

        // Dibujar
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawSimulation();

        DrawText("ESC para salir", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    // Cerrar ventana y liberar recursos
    CloseWindow();

    return 0;
}
