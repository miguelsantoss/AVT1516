#include <GL/glew.h>
#include <GL/freeglut.h>

#include "GameManager.h"

#define CAPTION "MicroMachines"

const int TABLE_WHITE_SQUARE = 0;
const int TABLE_BLUE_SQUARE = 1;
const int ORANGE = 2;
const int BUTTER = 3;
const int CAR_BODY = 4;
const int CAR_WHEEL = 5;
const int CHEERIO = 6;
const int Windows = 7;
const int CAR_HEADLIGHT = 8;
const int CAR_TAILIGHT = 9;
const int VIDA = 10;

int rotation = 0;
struct MyMesh mesh[11];
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GameManager::GameManager(void) {
}

GameManager::~GameManager(void) {}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//
// ------------------------------------------------------------

void GameManager::init(void)
{
	_elap = 0;

	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	camX = camY = camZ = 0;

	_car = new Car(Vector3(3.10f, 1.15f, 5.85f));
	_car->setDirection(1.0f, 0.0f, 0.0f);

	_oranges.push_back(new Orange(Vector3(17.0f, 2.0f, 5.0f)));
	_oranges.push_back(new Orange(Vector3(10.0f, 2.0f, 10.0f)));
	_oranges.push_back(new Orange(Vector3(4.0f, 2.0f, 17.0f)));

	_light = new PointLightSource();
	_light->setAmbient(new Vector4(1.0, 1.0, 1.0, 1.0));
	_light->setDiffuse(new Vector4(1.0, 1.0, 1.0, 1.0));
	_light->setSpecular(new Vector4(1.0, 1.0, 1.0, 1.0));
	_light->setPosition(new Vector4(5.0f, 3.0f, 4.0f, 1.0f));
	_light->setCutOff(60.0);
	_light->setExponent(4.0);
	_light->setState(true);
	_light->setDirection(new Vector4(0.0f, -3.0f, 0.0f, 1.0f));

	createTable();
	createButterPackets();
	createOranges();
	createCar();
	createCheerios();

	// Cameras
	_cameraLook = 1;
	setOrthoCamera(new OrthogonalCamera(0.0, 20.0, 0.0, 20.0, -10.0, 10.0));
	setPerspectiveCameraTop(new PerspectiveCamera(53.13f, 1.0f, 0.1f, 1000.0f));
	setPerspectiveCameraBehind(new PerspectiveCamera(53.13f, 1.0f, 0.1f, 1000.0f));
	_vidasPontosCamera = new OrthogonalCamera(0.0, 20.0, 0.0, 20.0, -10.0, 10.0);
	_activeCamera = _orthogonalCamera;

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	objId = VIDA;
	float amb[] = { 1.0f,0.0f,0.0f,1.0f };
	float diff[] = { 1.0f,0.0f,0.0f,1.0f };
	float spec[] = { 1.0f,0.0f,0.0f,1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 34.0f;
	int texcount = 0;

	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();
}

// ------------------------------------------------------------
//
// Render stufff
//
// ------------------------------------------------------------

void GameManager::renderScene(void)
{
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);	 //specifies which material parameters track the current color
	//glEnable(GL_NORMALIZE);

	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	// set the camera using a function similar to gluLookAt
	//lookAt(camX, camY, camZ, 0,0,0, 0,1,0);

	
	if (_cameraLook == 1 || _cameraLook == 2) {
		_activeCamera->computeProjectionMatrix();
	}
	
	else { // Camara do carro
		loadIdentity(PROJECTION);
		perspective(53.13f, 1.0f, 0.1f, 1000.0f);
		loadIdentity(VIEW);
		loadIdentity(MODEL);
		Vector3 direction = _car->getDirection();
		lookAt(_car->getPosition().getX() - 2 * direction.getX(), _car->getPosition().getY() + 1, _car->getPosition().getZ() - 2 * direction.getZ(),
				_car->getPosition().getX() + 5 * direction.getX() - camX  * direction.getX(), _car->getPosition().getY() - camY, _car->getPosition().getZ() + 5 * direction.getZ() - camZ  * direction.getZ(),
				0, 1, 0);
		//printDebugCameras();
	}

	// use our shader
	glUseProgram(shader.getProgramIndex());

	//send the light position in eye coordinates

	//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 

	//float res[4];
	//multMatrixPoint(VIEW, lightPos,res);   //lightPos definido em World Coord so is converted to eye space
	//glUniform4fv(lPos_uniformId, 1, res);

	setUpLights();

	drawTable();
	drawButterPackets();
	drawOranges();
	drawCar();
	drawCheerios();

	_vidasPontosCamera->computeProjectionMatrix();
	objId = VIDA;
	GLint loc;
	for (int i = 0; i < _vidas; i++) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, 19.3f, 1.0f, 15.3f + i);
		scale(MODEL, 0.5f, 0.5f, 0.5f);


		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}
	if (_paused) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, 5.0f, 5.0f, 5.0f);
		scale(MODEL, 10.0f, 0.5f, 10.0f);


		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}
	if (_gameOver) {
		//gameover texture
	}

	glutSwapBuffers();
}

void GameManager::printDebugCameras(void) {
	//float angle = _car->getAngle();
	//Vector3 direction = _car->getDirection();
	//std::cout << "Car's position-> X:" << _car->getPosition().getX() << " Y: " << _car->getPosition().getY() << " Z: " << _car->getPosition().getZ() << std::endl;
	//std::cout << "Car's direction-> X:" << _car->getDirection().getX() << " Y: " << _car->getDirection().getY() << " Z: " << _car->getDirection().getZ() << std::endl;
	//std::cout << "Car's angle-> alfa: " << _car->getAngle() << std::endl;
	//std::cout << "Camera's position-> X:" << _car->getPosition().getX() - 2 * cos(angle) << " Y: " << _car->getPosition().getY() + 1 << " Z: " << _car->getPosition().getZ() -2 * sin(angle) << std::endl;
	//std::cout << "Camera's looking at-> X:" << _car->getPosition().getX() + 5 * direction.getX() - camX << " Y: " << _car->getPosition().getY() - camY << " Z: " << _car->getPosition().getZ() + 5 * direction.getZ() - camZ << std::endl;
}

// ------------------------------------------------------------
//
// Lights Functions
//
// ------------------------------------------------------------

void GameManager::initMainAmbientLight(){
	GLfloat amb[] = { 0.2f, 0.2f, 0.2f, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);		// Adds low ambient light (dark / night mode)

	_mainAmbientLight->setAmbient(new Vector4(0.25, 0.25, 0.25, 1.0));	// Sets light that comes from all directions to "day mode"
	_mainAmbientLight->setDiffuse(new Vector4(0.8, 0.8, 0.8, 1.0));	// Diffuse reflection reflects light in all directions
	_mainAmbientLight->setSpecular(new Vector4(1.0, 1.0, 1.0, 1.0)); 	// Sets the specular color (white)
	_mainAmbientLight->setDirection(new Vector4(10.0, -10.0, 10.0, 0.0)); // Direction from which the light is shining
}

// ------------------------------------------------------------
//
// Reshape Callback Function
//
// ------------------------------------------------------------

void GameManager::changeSize(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	if(h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	loadIdentity(PROJECTION);
	perspective(53.13f, ratio, 0.1f, 1000.0f);
}

void GameManager::timer()
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << getWinX() << "x" << getWinY() << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
}

void GameManager::increaseSpeed()
{
	std::vector<Orange*>::iterator to_up;
	Orange* o;
	Vector3* pos;
	for (to_up = _oranges.begin(); to_up != _oranges.end(); to_up++) {
		o = static_cast<Orange*>(*to_up);
		o->increaseSpeed();
	}
}

void GameManager::updateOranges() {
	std::vector<Orange*>::iterator to_up;
	Orange* o;
	Vector3* pos;
	for (to_up = _oranges.begin(); to_up != _oranges.end(); to_up++) {
		o = static_cast<Orange*>(*to_up);
		o->update(_delta_t);
		if (o->getPosition().getX() > 21.0 || o->getPosition().getX() < -1.0 || o->getPosition().getZ() > 21.0 || o->getPosition().getX() < -1.0) {
			o->reset(Vector3(rand() % 8,2.0f, rand() % 8), glutGet(GLUT_ELAPSED_TIME));
		}
	}
}

void GameManager::refresh()
{
	_elapsed = glutGet(GLUT_ELAPSED_TIME);
	_delta_t = _elapsed - _elap;
	_elap = _elapsed;

	update(_delta_t);
}

void GameManager::destroyCar() {
	//resetPosition;
	if (_vidas <= 0) {
		_gameOver = true;
	}
}

void GameManager::update(double delta_t) {
	if (_paused || _gameOver) { glutPostRedisplay(); return; }
	_car->update(_delta_t);
	updateOranges();
	glutPostRedisplay();
}


// ------------------------------------------------------------
//
// Events from the Keyboard
//
// ------------------------------------------------------------
void GameManager::keyUp(unsigned char key) {

	//Up
	if (key == 'q' || key == 'Q') {
		_car->accelerationStop();
	}//Down
	else if (key == 'a' || key == 'A') {
		_car->accelerationStop();
	}//Left
	else if (key == 'o' || key == 'O') {
		_car->steerStop();
	}//Right
	else if (key == 'p' || key == 'P') {
		_car->steerStop();
	}

}

void GameManager::processKeys(unsigned char key, int xx, int yy)
{
	Vector3 a = _car->getPosition();
	switch(key) {

		case 27:
			glutLeaveMainLoop();
			break;

		case 'c': 
			printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
			break;
		case 'q': 
			_car->accelerationIncrease();
			printDebugCameras();
			break;
		case 'a': 
			//printf("backwards|stop\n");
			_car->accelerationDecrease();
			printDebugCameras();
			break; //backwards|stop movement
		case 'o': 
			//printf("left\n"); 
			_car->steerLeft();
			printDebugCameras();
			break; //steer left
		case 'p': 
			//printf("right\n"); 
			_car->steerRight();
			printDebugCameras();
			break; //steer right
		case 's':
			_paused = !_paused;
			break;
		case '1':
			if (_cameraLook != 1) {
				_activeCamera = _orthogonalCamera;
				_cameraLook = 1;
				_activeCamera->computeProjectionMatrix();
			}
			break;
		case '2':
			if (_cameraLook != 2) {
				_activeCamera = _perspectiveTop;
				_cameraLook = 2;
				_activeCamera->computeProjectionMatrix();
			}
			break;
		case '3': 
			if (_cameraLook != 3) {
				_cameraLook = 3;
			}
			break;
		case 'n':
			/*if (glIsEnabled(GL_LIGHTING)) {
				if (_mainAmbientLight->getState()) {
					_mainAmbientLight->setState(false);
					glDisable(_mainAmbientLight->getNum());
				}
				else {
					_mainAmbientLight->setState(true);
					glEnable(_mainAmbientLight->getNum());
				}
			}*/
			break;
			
	}
}


// ------------------------------------------------------------
//
// Mouse Events
//
// ------------------------------------------------------------

void GameManager::processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}


void GameManager::processMouseMotion(int xx, int yy) 
{
	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX =  - xx + startX;
	deltaY =    yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

//  uncomment this if not using an idle func
//	glutPostRedisplay();
}
void GameManager::mouseWheel(int wheel, int direction, int x, int y)
{

	r += direction * 0.1f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

//  uncomment this if not using an idle func
//	glutPostRedisplay();
}

// --------------------------------------------------------
//
// Shader Stuff
//
// --------------------------------------------------------

GLuint GameManager::setupShaders(void)
{
	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/pointlight.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/pointlight.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "VertexPosition");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "VertexNormal");
	//glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	
	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

	//while (1);
	return(shader.isProgramValid());
}

void GameManager::createTable(void) {
	float amb[] = { 0.09f, 0.09f, 0.47f, 1.0f };
	float diff[] = { 0.09f, 0.09f, 0.47f, 1.0f };
	float spec[] = { 0.09f, 0.09f, 0.47f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;
	objId = TABLE_WHITE_SQUARE;

	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	float amb1[] = { 0.94f, 0.94f, 0.94f, 1.0f };
	float diff1[] = { 0.94f, 0.94f, 0.94f, 1.0f };
	float spec1[] = { 0.94f, 0.94f, 0.94f, 1.0f };

	objId = TABLE_BLUE_SQUARE;
	memcpy(mesh[objId].mat.ambient, amb1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();
}

void GameManager::createCar(void) { 
	float amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diff[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	float spec[] = { 0.7f, 0.6f, 0.6f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 0.25f;
	int texcount = 0;
	objId = CAR_BODY;

	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	float amb1[] = { 0.02f, 0.02f, 0.02f, 1.0f };
	float diff1[] = { 0.01f, 0.01f, 0.01f, 1.0f };
	float spec1[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	float shininess1 = 0.78125f;
	objId = CAR_WHEEL;

	memcpy(mesh[objId].mat.ambient, amb1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess1;
	mesh[objId].mat.texCount = texcount;
	createTorus(0.25f, 1.0f, 20, 20);

	float amb3[] = { 0.26f,0.25f,1.0f,1.0f };
	float diff3[] = { 0.52f,1.0f,1.0f,1.0f };
	float spec3[] = { 1.0f,1.0f,1.0f,1.0f };
	float emissive3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess3 = 128.0f;
	int texcount3 = 0;
	objId = Windows;

	memcpy(mesh[objId].mat.ambient, amb3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive3, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess3;
	mesh[objId].mat.texCount = texcount3;
	createCube();

	float amb4[] = { 1.0f,1.0f,0.34f,1.0f };
	float diff4[] = { 1.0f,1.0f,0.39f,1.0f };
	float spec4[] = { 1.0f,1.0f,0.4f,1.0f };
	float emissive4[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess4 = 76.8f;
	int texcount4 = 0;
	objId = CAR_HEADLIGHT;

	memcpy(mesh[objId].mat.ambient, amb4, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff4, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec4, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive4, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess4;
	mesh[objId].mat.texCount = texcount4;
	createTorus(0.5f, 0.0f, 20, 20);

	float amb5[] = { 0.6f,0.0f,0.0f,1.0f };
	float diff5[] = { 0.6f,0.0f,0.0f,1.0f };
	float spec5[] = { 0.6f,0.0f,0.0f,1.0f };
	float emissive5[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess5 = 76.8f;
	int texcount5 = 0;
	objId = CAR_TAILIGHT;

	memcpy(mesh[objId].mat.ambient, amb5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive5, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess5;
	mesh[objId].mat.texCount = texcount5;
	createTorus(0.5f, 0.0f, 20, 20);
}

void GameManager::createOranges(void) {
	float amb[] = { 0.42f,0.2f,0.0f,1.0f };
	float diff[] = { 0.98f,0.48f,0.0f,1.0f };
	float spec[] = { 1.0f,0.55f,0.01f,1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 104.0f;
	int texcount = 0;
	objId = ORANGE;

	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createSphere(1.0f, 20);
}

void GameManager::createButterPackets(void) {
	float amb[] = { 0.8f, 0.8f, 0.03f, 1.0f };
	float diff[] = { 0.5f, 0.5f, 0.0f, 1.0f };
	float spec[] = { 0.84f, 0.84f, 0.0f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;
	objId = BUTTER;

	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();
}

void GameManager::createCheerios() {
	float amb[] = { 0.0f,0.0f,0.0f,1.0f };
	float diff[] = { 1.0f,0.0f,1.0f,1.0f };
	float spec[] = { 1.0f,0.0f,1.0f,1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 34.0f;
	int texcount = 0;
	objId = CHEERIO;

	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createTorus(0.25f, 1.0f, 20, 20);
}

void GameManager::drawTable(void) {
	GLint loc;
	objId = TABLE_WHITE_SQUARE;

	for (int i = 0; i <= 10; i++) {
		for (int j = 0; j <= 10; j++) {
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			translate(MODEL, i*2.0f, 0.0f, j*2.0f);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}
	}
	for (float i = 0.5; i <= 9.5; i++) {
		for (float j = 0.5; j <= 9.5; j++) {
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			translate(MODEL, i*2.0f, 0.0f, j*2.0f);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}
	}
	objId = TABLE_BLUE_SQUARE;
	for (float i = 0.5; i <= 9.5; i++) {
		for (int j = 0; j <= 10; j++) {
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			translate(MODEL, i*2.0f, 0.0f, j*2.0f);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}
	}
	for (int i = 0; i <= 10; i++) {
		for (float j = 0.5; j <= 9.5; j++) {
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			translate(MODEL, i*2.0f, 0.0f, j*2.0f);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}
	}
}

void GameManager::drawCar(void) {
	GLint loc;
	objId = CAR_BODY;
	Vector3 a = _car->getPosition();
	double angle = _car->getAngle();
	float translatef[] = { 0.0, 0.0, 0.0 };
	float scalef = 0.3f;
	float rotatef[] = { 90.0f, 1.0, 0.0, 0.0f };

	float wheel_r[] = { 0.3f, 1.1f };
	pushMatrix(MODEL);
	
	translate(MODEL, a.getX(), a.getY(), a.getZ());
	scale(MODEL, 0.5f, 0.5f, 0.5f);

	rotate(MODEL, angle, 0, 1, 0);
	translate(MODEL, -0.7, 0.0, -0.5);
	// send the material
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	scale(MODEL, 1.4f, 0.5f, 1.0f);
	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	pushMatrix(MODEL);
	translate(MODEL, 0.15, 1.1, 0.15);
	scale(MODEL, 0.7f, 0.45f, 0.7f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	popMatrix(MODEL);

	objId = Windows;
	{
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, 0.15f, 1.1f, 0.15f);
		rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
		scale(MODEL, 0.7f, 0.02f, 0.45f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);

		pushMatrix(MODEL);
		translate(MODEL, 0.15f, 1.55f, 0.85f);
		rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
		scale(MODEL, 0.7f, 0.02f, 0.45f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);

		pushMatrix(MODEL);
		translate(MODEL, 0.15f, 1.55f, 0.13f);
		rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);
		scale(MODEL, 0.74f, 0.02f, 0.45f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);

		pushMatrix(MODEL);
		translate(MODEL, 0.87f, 1.55f, 0.13f);
		rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);
		scale(MODEL, 0.74f, 0.02f, 0.45f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);
	}

	objId = CAR_HEADLIGHT;
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	{
		pushMatrix(MODEL);
		translate(MODEL, 1.0f, 0.7f, 0.2f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);
		scale(MODEL, scalef, scalef, scalef);
		scale(MODEL, 0.6, 0.6, 0.6);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		pushMatrix(MODEL);
		translate(MODEL, 0.0f, 0.0f, 3.3f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);
	}
	objId = CAR_TAILIGHT;
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	{
		pushMatrix(MODEL);
		translate(MODEL, 0.0f, 5.6f, 0.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		pushMatrix(MODEL);
		translate(MODEL, 0.0f, 0.0f, 3.3f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);
		popMatrix(MODEL);
	}

	
	popMatrix(MODEL);
	popMatrix(MODEL);

	
	objId = CAR_WHEEL;
	for (int i = 0; i < 4; i++) {
		// send the material
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);

		pushMatrix(MODEL);
		if (i <= 1) translate(MODEL, wheel_r[i], 0.15, -0.1);
		else translate(MODEL, wheel_r[i-2], 0.15, 1.1f);
		if (i == 1 || i == 3) {
			rotate(MODEL, _car->getWeelAngle(), 0.0, 1.0, 0.0);
		}
		scale(MODEL, scalef, scalef, scalef);
		rotate(MODEL, rotatef[0], rotatef[1], rotatef[2], rotatef[3]);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}
	popMatrix(MODEL);
}

void GameManager::drawOranges(void) {
	GLint loc;
	float x[] = { -7.0f, 2.0f, 8.0f };
	float z[] = { -7.0f, 2.0f, 3.0f };

	objId = ORANGE;

	for (int i = 0; i < 3; i++) {
		// send the material
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, _oranges[i]->getPosition().getX(), _oranges[i]->getPosition().getY(), _oranges[i]->getPosition().getZ());

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}
}

void GameManager::drawButterPackets(void) {
	GLint loc;
	float x1[] = { 2.0f, 13.0f, 18.0f };
	float z1[] = { 17.0f, 10.0f, 14.0f };

	objId = BUTTER;

	for (int i = 0; i < 3; i++) {
		// send the material
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, x1[i], 1.0f, z1[i]);
		scale(MODEL, 1.3f, 0.5f, 0.7f);
		
		
		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}
}

void GameManager::drawCheerios() {
	GLint loc;
	objId = CHEERIO;
	// send the material
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 2; j++) {
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			//translate(MODEL, 5.5f - j * 2.7, 1.10f, -7.3f + i * 0.95);
			translate(MODEL, 10.0f - i * 0.95f, 1.10f, 4.3f - j *2.7f);
			scale(MODEL, 0.5f, 0.5f, 0.5f);
			scale(MODEL, 0.5f, 0.5f, 0.5f);


			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}
	}
}

void GameManager::setUpLights(void) {
	GLint loc;

	float res[4];
	multMatrixPoint(VIEW, _light->getPosition()->getArray(), res);

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isLocal");
	glUniform1i(loc, true);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isSpot");
	glUniform1i(loc, false);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isEnabled");
	glUniform1i(loc, true);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].ambient");
	glUniform4fv(loc, 1, _light->getAmbient()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].diffuse");
	glUniform4fv(loc, 1, _light->getDiffuse()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].specular");
	glUniform4fv(loc, 1, _light->getSpecular()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].position");
	glUniform3fv(loc, 1, res);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].coneDirection");
	glUniform3fv(loc, 1, _light->getDirection()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].coneDirection");
	glUniform3fv(loc, 1, _light->getDirection()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].spotCosCutoff");
	glUniform1f(loc, _light->getCutOff());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].spotExponent");
	glUniform1f(loc, _light->getExponent());

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].constantAttenuation");
	glUniform1f(loc, 1.2f);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].linearAttenuation");
	glUniform1f(loc, 0.03f);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].quadraticAttenuation");
	glUniform1f(loc, 0.0009f);
}