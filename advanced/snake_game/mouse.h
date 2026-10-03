#ifndef MOUSE_H
#define MOUSE_H
#include <stdint.h>

struct Mouse{
int xPosition;
int yPosition;
uint8_t size;
};
struct Mouse* createMouseAtRandomPositionIn(int width, int height,uint8_t size);
void destroyMouse(struct Mouse* mouse);
#endif