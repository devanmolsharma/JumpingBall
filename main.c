#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "./helpers.h"
#include <pthread.h>

struct drawableobject
{
    int x;
    int y;
    int height;
    int width;
    char *data;
};

struct spawnBallParams
{
    struct drawableobject *player;
    struct drawableobject *ball;
    int *points;
};

// this creates a drawable object that i want to render on screen
struct drawableobject newObject(int height, int width, char symbol)
{
    struct drawableobject obj;
    obj.x = 0;
    obj.y = 10;
    obj.height = height;
    obj.width = width;
    char *c = (char *)malloc(height * (width + 1) * sizeof(char));
    for (int i = 0; i < height * (width + 1); i++)
    {
        c[i] = symbol;
    }

    for (int i = 0; i < height * (width + 1); i += (width + 1))
    {
        c[i] = '\n';
    }

    obj.data = c;

    return obj;
}

// render platfrom and player
void render(struct drawableobject *obj, int *points, struct drawableobject *ball)
{
    printf("Points: %d\n", *points);

    for (int i = 0; i < obj->y; i++)
    {
        if (i == 3)
        {
            for (int w = 0; w < ball->x; w++)
            {
                printf(" ");
            }
            printf("%s", "%%");
        }
        printf("\n");
    }
    for (int i = 0; i < obj->height * (obj->width + 1); i++)
    {
        printf("%c", obj->data[i]);
        if (obj->data[i] == '\n')
        {
            for (int i = 0; i < obj->x; i++)
            {
                printf(" ");
            }
        }
    }
    printf("\n");

    for (int i = 0; i < 10 - obj->y; i++)
    {
        printf("\n");
    }
    // draw the platform
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

void *jump(void *ptr)

{
    struct drawableobject *obj = (struct drawableobject *)ptr;
    if (obj->y == 10)
    {
        while (obj->y > 1)
        {
            obj->y--;
            usleep(100000);
        }

        while (obj->y < 10)
        {
            obj->y++;
            usleep(100000);
        }
    }
}

void *handleBall(void *ptr)

{
    struct spawnBallParams *obj = (struct spawnBallParams *)ptr;
    while (1)
    {
        if (obj->ball->x - obj->player->x < obj->player->width && obj->ball->x - obj->player->x > -obj->player->width && obj->player->y - obj->ball->y <= 2)
        {
            *(obj->points) += 1;
            obj->ball->x = rand() % (80 + 1 - 2) + 2;
            sleep(1);

        }
    }
}
int main(int argc)
{
    int running = 1;
    int points = 0;
    struct drawableobject obj = newObject(4, 4, '#');
    struct drawableobject ball = newObject(1, 2, '%');
    struct spawnBallParams params;
    params.ball = &ball;
    params.player = &obj;
    params.points = &points;
    ball.x = 10;
    ball.y = 3;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, *handleBall, (void *)&params);
    clock_t start = time(NULL);
    while (running)
    {
        char key = getKeyPressed();
        clearScr();
        if (key == 'c')
        {
            break;
        }
        if (key == '2')
        {
            if (obj.y < 10)
                obj.y++;
        }
        if (key == '6')
        {
            obj.x++;
        }

        if (key == '4')
        {
            if (obj.x > 0)
                obj.x--;
        }
        if (key == '8')
        {
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, *jump, (void *)&obj);
        }
        render(&obj, &points, &ball);
        usleep(10000);
    }
}