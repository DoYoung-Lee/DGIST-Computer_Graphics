#ifndef USER_INPUT_FUNC_H
#define USER_INPUT_FUNC_H

#include <GL/glew.h>
#include <GL/freeglut.h>

extern int move_axis[2];
extern int modelview_index;

void KeyboardCB(unsigned char key, int x, int y);
void SpecialCB(int key, int x, int y);
void SpecialUpCB(int key, int x, int y);

#endif