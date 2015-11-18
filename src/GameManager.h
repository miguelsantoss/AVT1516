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
#include "DirectionalLightSource.h"
#include "PointLightSource.h"
#include "ButterPacket.h"
#include "Cheerio.h"
#include "Particle.h"
#include "Flare.h"

class GameManager {
	public:
		GameManager(void);
		~GameManager(void);

		void init(void);
		void renderScene(void);
		void changeSize(int w, int h);
		void timer();
		void refresh();
		void increaseSpeed();
		void iterate();

		void keyUp(unsigned char key);

		void processKeys(unsigned char key, int xx, int yy);
		void processMouseButtons(int button, int state, int xx, int yy);
		void processMouseMotion(int xx, int yy);
		void mouseWheel(int wheel, int direction, int x, int y);
		void printDebugCameras(void);
		GLuint setupShaders(void);

		int getWinX() { return WinX; }
		int getWinY() { return WinY; }
		int getWindowHandle() { return WindowHandle; }
		void setWindowHandle(int i) { WindowHandle = i; }

	private:
		int WindowHandle = 0;
		int WinX = 800, WinY = 600;
		float tableXMin;
		float tableXMax;
		float tableZMin;
		float tableZMax;
		Vector3 carStartPos;
		Vector3 sunPos;

		bool _paused = false;
		bool _gameOver = false;
		bool _fogState = true;
		bool _fireworks = false;
		int _lives = 5;
		long unsigned int _score = 0;
		double _elapsed;
		double _delta_t;
		double _elap;
		float _fogColorDay[3], _fogColorNight[3], _fogColor[3], _fogDensity;
		float sun_pos_x, sun_pos_y;
		int _fogMode = 1;

		Car* _car;

		std::vector<Orange> _oranges;
		std::vector<ButterPacket> _butters;
		std::vector<Cheerio> _cheerios;
		std::vector<Particle> _particles;

		int _cameraLook;
		Camera* _activeCamera;
		PerspectiveCamera* _perspectiveTop;
		PerspectiveCamera* _perspectiveBehind;
		PerspectiveCamera* _perspectiveInside;
		OrthogonalCamera* _orthogonalCamera;
		OrthogonalCamera* _flaresCamera;
		OrthogonalCamera* _scoreCamera;

		std::vector<FlareElement> _flare;
		std::vector<LightSource*> _lights;

		VSShaderLib shader;
		/*
		struct MyMesh mesh[4];
		int objId=0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh
		*/
		GLint pvm_uniformId;
		GLint vm_uniformId;
		GLint normal_uniformId;
		GLint lPos_uniformId;
		GLint tex_loc_1, tex_loc_2;
		GLint texUse, useLights;
		GLint writeMode;
		GLint vWriteMode;
		GLint texMode;
		GLint fogState, fogMode, fogColor, fogDensity;
		GLint particleMode, sun, flares;

		GLuint TextureArray[12];
		GLuint _vaoID, _vaoFlareID;
		GLuint treeID;

		GLuint _texCoordBuffer, _ftexCoordBuffer;
		GLuint _vertexBuffer, _fvertexBuffer;
		GLuint uniColor;

		float _fontSize;
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
		//float lightPos[4] = {4.0f, 6.0f, 2.0f, 1.0f};
		/*
		/// The storage for matrices
		float mMatrix[COUNT_MATRICES][16];
		float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

		/// The normal matrix
		float mNormal3x3[9];*/
		void createTable(void);
		void createMilk(void);
		void createCar(void);
		void createOranges(void);
		void createButterPackets(void);
		void createCheerios(void);
		void createCandleSticks(void);
		void drawMilk(void);
		void createTreeSquare(void);
		void createParticleQuad(void);
		void createLensFlareQuad(void);
		void createSunQuad(void);

		void drawTable(void);
		void drawOranges(void);
		void drawCar(void);
		void drawCarReflect(void);
		void drawButterPackets(void);
		void drawButterPacketsReflect(void);
		void drawCheerios(void);
		void drawCheeriosReflect();
		void drawCandleSticks(void);
		void drawTreeSquare(void);
		void drawParticleQuad(void);
		void drawLensFlareQuad(void);
		void drawSunQuad(void);

		void setUpLights(void);
		void update(double delta_t);
		void updateButters(void);
		void updateCheerios(void);
		void destroyCar(void);
		void createLights(void);
		void updateOranges(void);
		void update_car_headlights(void);
		void initTextureMappedFont();
		void DrawString(float x, float y, const std::string& str);
		void restartGame();
		void createParticles(int nParticles);
		void initParticles();
		float frand();
		float frand(float min, float max);
		void updateParticles(float delta_t);
		void flareInit();

		void billboardRotation(float objPosX, float objPosY, float objPosZ);
		void normalize(float* v);
		void crossProduct(float* v1, float* v2, float* v3);
		float innerProduct(float* v1, float* v2);
};

#endif /* __MICROMACHINES_GM__ */
