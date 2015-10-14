#include <iostream>
#include <stdio.h>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>
#include "GameManager.h"
#define CAPTION "MicroMachines"

GameManager* gm;

void timer(int value)
{
	gm->timer();
	glutTimerFunc(1000, timer, 0);
}

void increaseSpeed(int value)
{
	gm->increaseSpeed();
	glutTimerFunc(100000, increaseSpeed, 0);
}
void refresh(int value)
{
	gm->refresh();
	glutTimerFunc(1000 / 60, refresh, 0);
}

void keyUp(unsigned char key, int x, int y) { gm->keyUp(key);}
void processKeys(unsigned char key, int xx, int yy) { gm->processKeys(key, xx, yy); }
void processMouseButtons(int button, int state, int xx, int yy) { gm->processMouseButtons(button, state, xx, yy); }
void processMouseMotion(int xx, int yy) { gm->processMouseMotion(xx, yy); }
void mouseWheel(int wheel, int direction, int x, int y) { gm->mouseWheel(wheel, direction, x, y); }
void renderScene() { gm->renderScene(); }
void changeSize(int w, int h) { gm->changeSize(w, h); }

int main(int argc, char **argv) {

	gm = new GameManager();

//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE );
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(gm->getWinX(), gm->getWinY());
	gm->setWindowHandle(glutCreateWindow(CAPTION));

//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//glutIdleFunc(renderScene);

//	Mouse and Keyboard Callbacks
	glutKeyboardUpFunc(keyUp);
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel) ;
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(0, refresh, 0);


//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));

	if (!gm->setupShaders())
		return(1);

	gm->init();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}
