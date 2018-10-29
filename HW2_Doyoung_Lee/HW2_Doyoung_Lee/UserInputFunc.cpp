#include <GL/glew.h>
#include <GL/freeglut.h>

void KeyboardCB(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		exit(0);
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