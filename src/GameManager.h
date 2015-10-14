#ifndef __MICROMACHINES_GM__
#define __MICROMACHINES_GM__

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"
#include "Camera.h"
#include "PerspectiveCamera.h"
#include "OrthogonalCamera.h"
#include "DynamicObject.h"
#include "Car.h"
#include "Orange.h"

class GameManager {
	public:
		GameManager(void);
		~GameManager(void);

		void init(void);
		void renderScene(void);
		void changeSize(int w, int h);
		void timer();
		void increaseSpeed();
		void updateOranges();
		void refresh();

		void processKeys(unsigned char key, int xx, int yy);
		void processMouseButtons(int button, int state, int xx, int yy);
		void processMouseMotion(int xx, int yy);
		void mouseWheel(int wheel, int direction, int x, int y);

		GLuint setupShaders(void);

		int getWinX() { return WinX; }
		int getWinY() { return WinY; }
		int getWindowHandle() { return WindowHandle; }
		void setWindowHandle(int i) { WindowHandle = i; }

		void setOrthoCamera(OrthogonalCamera* orthogonalCamera) {
			_orthogonalCamera = orthogonalCamera;
		}

		void setPerspectiveCameraTop(PerspectiveCamera* perspectiveCamera) {
			_perspectiveTop = perspectiveCamera;
		}

		void setPerspectiveCameraBehind(PerspectiveCamera* perspectiveCamera) {
			_perspectiveBehind = perspectiveCamera;
		}

	private:
		int WindowHandle = 0;
		int WinX = 800, WinY = 600;

		double _elapsed;
		double _delta_t;
		double _elap;

		Car* _car;

		std::vector<Orange*> _oranges;

		int _cameraLook;
		Camera* _activeCamera;
		PerspectiveCamera* _perspectiveTop;
		PerspectiveCamera* _perspectiveBehind;
		OrthogonalCamera* _orthogonalCamera;

		VSShaderLib shader;

		std::vector<GameObject*> test;

		/*
		struct MyMesh mesh[4];
		int objId=0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh
		*/
		GLint pvm_uniformId;
		GLint vm_uniformId;
		GLint normal_uniformId;
		GLint lPos_uniformId;
			
		// Camera Position
		float camX, camY, camZ;

		// Mouse Tracking Variables
		int startX, startY, tracking = 0;

		// Camera Spherical Coordinates
		float alpha = 39.0f, beta = 51.0f;
		float r = 10.0f;

		// Frame counting and FPS computation
		unsigned int FrameCount = 0;
		long myTime,timebase = 0,frame = 0;
		char s[32];
		float lightPos[4] = {4.0f, 6.0f, 2.0f, 1.0f};
		/*
		/// The storage for matrices
		float mMatrix[COUNT_MATRICES][16];
		float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

		/// The normal matrix
		float mNormal3x3[9];*/
		void createTable(void);
		void createCar(void);
		void createOranges(void);
		void createButterPackets(void);

		void drawTable(void);
		void drawOranges(void);
		void drawCar(void);
		void drawButterPackets(void);


};

#endif /* __MICROMACHINES_GM__ */
