#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

typedef char u8;

typedef struct Freelist {
    struct Freelist *next;
} Freelist;

typedef struct{
    u8* buffer;
    size_t size;
    size_t capacity;
    Freelist* freelist;
} Arena;

typedef struct{
    char* Name;
    int x;
    int y;
    u8 size;
    u8 r;
    u8 g;
    u8 b;
} Ball;

void InitArena(Arena* arena, size_t capacity){
    arena->buffer = (u8*)malloc(capacity);
    arena->capacity = capacity;
    arena->size = 0;
    arena->freelist = NULL;
}

void* ArenaAlloc(Arena* arena, size_t sizeOfAllocation){
    if(arena->freelist){
        Freelist* first = arena->freelist;
        arena->freelist = arena->freelist->next;
        return first;
    }

    if (arena->size + sizeOfAllocation > arena->capacity) {
        return NULL;
    }

    void* ptr = arena->buffer + arena->size;
    arena->size+= sizeOfAllocation;
    return ptr;
}

void ArenaDealloc(Arena* arena, Ball* ball){
    u8* ballByte = (u8*) ball;
    for(int i = 0; i < sizeof(Ball); i++){
         ballByte[i] = 0;
    }

    Freelist* curr = (Freelist*) ball;
    curr->next = arena->freelist;
    arena->freelist = curr;
}

void ResetArena(Arena* arena){
    arena->size = 0;
    arena->freelist = NULL;

    for(int i = 0; i < arena->capacity; i++){
        arena->buffer[i] = 0;
    }
}

int main(){
    const int WIDTH = 1024;
    const int HEIGHT = 900;

    InitWindow(WIDTH, HEIGHT, "Ball Arena Demo");
    SetTargetFPS(144);

    Arena arena = {0};
    InitArena(&arena, 8 * 1024);
    printf("Arena initialized at: %p\n", arena.buffer);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            Ball* ball = ArenaAlloc(&arena, sizeof(Ball));
            if (ball) {
                ball->Name = "NewBall";
                ball->x = GetRandomValue(0, WIDTH);
                ball->y = GetRandomValue(0, HEIGHT);
                ball->size = GetRandomValue(10, 50);
                ball->r = GetRandomValue(50, 255);
                ball->g = GetRandomValue(50, 255);
                ball->b = GetRandomValue(50, 255);
                
                printf("Created ball at (%d,%d)\n", ball->x, ball->y);
                printf("Memory region for the ball is: %p\n", ball);
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            size_t ball_count = arena.size / sizeof(Ball);

            for (size_t i = 0; i < ball_count; i++) {
                Ball* b = (Ball*)(arena.buffer + i * sizeof(Ball));
                // we delete the name when we zero out a ball.
                // So checking if a name is defined is checking if a ball is in use or was cleared
                if (b->Name) {
                    DrawCircle(b->x, b->y, b->size, 
                              (Color){b->r, b->g, b->b, 255});
                }
            }
            
            DrawText("Press SPACE to create balls", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
    
    CloseWindow();
    free(arena.buffer);
    return 0;
}