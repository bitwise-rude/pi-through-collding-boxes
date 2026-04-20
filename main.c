#include  "main.h"
#include  <stdio.h>
#include "raylib.h"

#define HEIGHT 600
#define WIDTH  900
#define VWALL_X 20
#define HWALL_Y HEIGHT/2 + 90

#define SIZE_PER_MASS 5
#define VEL -0.0009f

#define  MASS_RATIO 100000000

typedef struct {
    double x;
    long int mass;
    double vel;
    long int size;
}Box;


int main() {
    InitWindow(WIDTH, HEIGHT, "Colliding Boxes");

    Box left_box = {.x = VWALL_X + 10, .mass = 5, .vel = 0.0f};
    left_box.size = left_box.mass * SIZE_PER_MASS;

    // TODO: Use preprocessor for this thing
    Box right_box = {.x = VWALL_X + 50, .mass = 5*MASS_RATIO, .vel = VEL};
    if  (SIZE_PER_MASS * MASS_RATIO >=  100) {
        right_box.size = 100;
    }else {
        right_box.size = right_box.mass * SIZE_PER_MASS;
    }

    const  long int sum_of_mass =  left_box.mass + right_box.mass;

    int collisions = 0;
    int prev_collisions = 0;
    char str_collisions[19] = "COLLISIONS:";
    char str_ratio[50]="RATIO:";
    snprintf(str_ratio+6, sizeof(str_ratio) - 6, "%ld",MASS_RATIO);


    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // bottom surface
            DrawLine(0, HWALL_Y, WIDTH, HWALL_Y, BLACK);

            // vertical wall
            DrawLine(VWALL_X, HWALL_Y, 20, 0, BLACK);

            // draw only if on range
            // TODO: Optimize this belowk 
            // draw left mass
            if (left_box.x > (double)   VWALL_X) {
                DrawRectangle(left_box.x , HWALL_Y - left_box.size,left_box.size, left_box.size, RED);
            }else{
                DrawRectangle(VWALL_X , HWALL_Y - left_box.size,left_box.size, left_box.size, RED);
            }
            // draw right mass
            if (right_box.x > (left_box.x + left_box.size)) {
                DrawRectangle(right_box.x , HWALL_Y - right_box.size,right_box.size, right_box.size, RED);
            }else{
                DrawRectangle(left_box.x + left_box.size , HWALL_Y - right_box.size,right_box.size, right_box.size, RED);
            }

            //  show collisions
            if (prev_collisions < collisions) {
                snprintf(str_collisions+11, sizeof(str_collisions) - 11, "%d", collisions);
                prev_collisions = collisions;
            }
            DrawText(str_collisions, WIDTH - 800, 10, 50, GREEN);
            DrawText(str_ratio, 10, HEIGHT-100, 60, BLUE);
            

        EndDrawing();

        //  moves due to velocity

        right_box.x += right_box.vel;
        left_box.x += left_box.vel;
        
        // TOOD: optimize
        // collision of left box and right box
        if (right_box.x <= (left_box.x + left_box.size)) {
            double temp =  ((left_box.mass - right_box.mass) *  left_box.vel + 2 * right_box.mass * right_box.vel) /  sum_of_mass;
            right_box.vel =  ((right_box.mass -  left_box.mass) *  right_box.vel + 2 * left_box.mass * left_box.vel) /  sum_of_mass;
            left_box.vel = temp;
            collisions  ++;
        }

        // collision with wall
        if (left_box.x <= (double)   VWALL_X) {
            left_box.vel = -1 * left_box.vel;
            collisions  ++;
        }
    }

    CloseWindow();

    return 0;
}
