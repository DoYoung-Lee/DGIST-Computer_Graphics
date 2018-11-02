#ifndef USER_INPUT_FUNC_H
#define USER_INPUT_FUNC_H

#include <GL/glew.h>
#include <GL/freeglut.h>

extern int move_axis[2];
extern int modelview_index;

void KeyboardCB(unsigned char, int, int);
void KeyboardUpCB(unsigned char, int, int);
void SpecialCB(int, int, int);
void SpecialUpCB(int, int, int);

#endif