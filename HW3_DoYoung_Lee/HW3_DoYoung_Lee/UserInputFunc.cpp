#include "UserInputFunc.h"

void KeyboardCB(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		exit(0);
		break;
	case 'w':
	case 'W':
		move_axis[0] = 1;
		move_axis[2] = 0;
		break;
	case 's':
	case 'S':
		move_axis[0] = -1;
		move_axis[2] = 2;
		break;
	case 'a':
	case 'A':
		move_axis[1] = -1;
		move_axis[2] = 1;
		break;
	case 'd':
	case 'D':
		move_axis[1] = 1;
		move_axis[2] = 3;
		break;
	case '1':
		modelview_index = 1; // For first-person view
		break;
	case '2':
		modelview_index = 2; // For top view
		break;
	case '3':
		modelview_index = 3; // For third-person view
		break;
	default:
		break;
	}
}

void KeyboardUpCB(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
	case 'W':
	case 's':
	case 'S':
		move_axis[0] = 0;
		break;
	case 'a':
	case 'A':
	case 'd':
	case 'D':
		move_axis[1] = 0;
		break;
	default:
		break;
	}
}

void SpecialCB(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:

		break;
	case GLUT_KEY_DOWN:

		break;
	case GLUT_KEY_LEFT:

		break;
	case GLUT_KEY_RIGHT:

		break;
	}
}

void SpecialUpCB(int key, int x, int y) {

}