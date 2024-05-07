
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#define PARTICLES_NUM 6
#define PARTICLE_DEADZONE 0.01f
#define VECTOR2_ZERO (Vector2) { 0, 0 }

#define BOX_BOUNDS 1024
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define MAP_SIZE 128

#define ELEMENTARY_C 1.602176634e-3f
#define PERMITTIVITY 8.854187813e-12f

#define debugv2(v) printf(#v " = <%f, %f>\n", v.x, v.y);

struct particle {
    Vector2 p, v;
    float q;
};

static float clampf(float x, float min, float max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}


int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "electric00");

    struct particle particles[PARTICLES_NUM];
    particles[0] = (struct particle) { (Vector2) { 25.0f, 50.0f }, (Vector2) { 0.0f, 5.0f }, 1.0f * ELEMENTARY_C };
    particles[1] = (struct particle) { (Vector2) { 50.0f, 50.0f }, VECTOR2_ZERO, -1.0f * ELEMENTARY_C };
    particles[2] = (struct particle) { (Vector2) { 35.0f, 70.0f }, (Vector2) { 0.0f, 5.0f }, 1.0f * ELEMENTARY_C };
    particles[3] = (struct particle) { (Vector2) { 60.0f, 70.0f }, VECTOR2_ZERO, -1.0f * ELEMENTARY_C };
    particles[4] = (struct particle) { (Vector2) { 75.0f, 90.0f }, (Vector2) { 0.0f, 5.0f }, 1.0f * ELEMENTARY_C };
    particles[5] = (struct particle) { (Vector2) { 100.0f, 90.0f }, VECTOR2_ZERO, -1.0f * ELEMENTARY_C };

    Image map_img = GenImageColor(MAP_SIZE, MAP_SIZE, RED);
    Texture2D map_txt = LoadTextureFromImage(map_img);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);

            float box_x = (WINDOW_WIDTH - BOX_BOUNDS) / 2.0f;
            float box_y = (WINDOW_HEIGHT - BOX_BOUNDS) / 2.0f;

            float dt = GetFrameTime();
            
            for (int i = 0; i < PARTICLES_NUM; i++) {
                struct particle *A = &particles[i];
                for (int j = 0; j < PARTICLES_NUM; j++) {
                    struct particle *B = &particles[j];
                    if (A == B) continue;
                    Vector2 r12 = Vector2Subtract(A->p, B->p);
                    Vector2 rhat12 = Vector2Normalize(r12);
                    float dist = Vector2LengthSqr(r12);
                    if (dist < PARTICLE_DEADZONE) continue;
                    A->v = Vector2Add(A->v, Vector2Scale(rhat12, (dt * A->q * B->q) / (4 * M_PI * PERMITTIVITY * Vector2LengthSqr(r12))));
                }
            }
            for (int i = 0; i < PARTICLES_NUM; i++) {
                struct particle *P = &particles[i];
                P->p = Vector2Add(P->p, Vector2Scale(P->v, dt));

                if (P->p.x <= 0 || P->p.x >= BOX_BOUNDS) {
                    P->p.x = clampf(P->p.x, 0, BOX_BOUNDS);
                    P->v.x = -P->v.x * 0.8;   
                }
                if (P->p.y <= 0 || P->p.y >= BOX_BOUNDS) {
                    P->p.y = clampf(P->p.y, 0, BOX_BOUNDS);
                    P->v.y = -P->v.y * 0.8;   
                }

                DrawCircle(P->p.x + box_x, P->p.y + box_y, 2.0f, P->q < 0.0f ? BLUE : RED);
            }

            float map_scale = BOX_BOUNDS / (float) MAP_SIZE;
            Color *pixels = map_img.data;
            for (int i = 0; i < MAP_SIZE; i++) {
                for (int j = 0; j < MAP_SIZE; j++) {
                    Vector2 ref = (Vector2) { map_scale * (i + 0.5f), map_scale * (j + 0.5f) };
                    float qe = 0;
                    for (int k = 0; k < PARTICLES_NUM; k++) {
                        struct particle *P = &particles[k];
                        qe += P->q / Vector2Distance(P->p, ref);
                    }
                    unsigned char qe_c = (unsigned char) (clampf(fabsf(qe) * 10000000.0f, 0.0f, 255.0f));
                    pixels[j * MAP_SIZE + i].r = qe < 0 ? 0 : qe_c;
                    pixels[j * MAP_SIZE + i].g = 0;
                    pixels[j * MAP_SIZE + i].b = qe > 0 ? 0 : qe_c;
                    pixels[j * MAP_SIZE + i].a = qe_c;
                }
            }

            UpdateTexture(map_txt, pixels);

            DrawRectangleLines(box_x, box_y, BOX_BOUNDS, BOX_BOUNDS, BLACK);
            DrawTextureEx(map_txt, (Vector2) { box_x, box_y }, 0.0f, BOX_BOUNDS / (float) MAP_SIZE, (Color) { 255, 255, 255, 127 });
        EndDrawing();
    }

    return 0;
}
