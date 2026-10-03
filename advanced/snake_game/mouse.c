#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mouse.h"



struct Mouse *createMouseAtRandomPositionIn(int width, int height, uint8_t size)
{
    int half   = size / 2;
    int rangeW = width  - 2*size;
    int rangeH = height -2*size;
    if (rangeW <= 0 || rangeH <= 0) {
        puts("area is too small for the mouse");
        return NULL;
    }

    struct Mouse* mouse = malloc(sizeof(struct Mouse));
    if (mouse == NULL) {
        puts("mouse allocation failed");
        return NULL;
    }

    mouse->size      = size;
    mouse->xPosition = half + rand() % (rangeW + 1);   // half .. width - half
    mouse->yPosition = half + rand() % (rangeH + 1);   // half .. height - half
    return mouse;
}

void destroyMouse(struct Mouse *mouse)
{
    free(mouse);
}
