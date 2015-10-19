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
int rotation = 0;
struct MyMesh mesh[7];
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

	_car = new Car(Vector3(-3.10f, 1.15f, -5.85f));
	_car->setDirection(1.0f, 0.0f, 0.0f);

	_oranges.push_back(new Orange(Vector3(-7.0f, 2.0f, -7.0f)));
	_oranges.push_back(new Orange(Vector3(2.0f, 2.0f, 2.0f)));
	_oranges.push_back(new Orange(Vector3(8.0f, 2.0f, 3.0f)));

	createTable();
	createButterPackets();
	createOranges();
	createCar();
	createCheerios();

	// Cameras
	_cameraLook = 1;
	setOrthoCamera(new OrthogonalCamera(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0));
	setPerspectiveCameraTop(new PerspectiveCamera(53.13f, 1.0f, 0.1f, 1000.0f));
	setPerspectiveCameraBehind(new PerspectiveCamera(53.13f, 1.0f, 0.1f, 1000.0f));
	_activeCamera = _orthogonalCamera;

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}

// ------------------------------------------------------------
//
// Render stufff
//
// ------------------------------------------------------------

void GameManager::renderScene(void)
{
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
	/*
	else if (_cameraLook == 3) {
		_activeCamera->update(_car->getPosition().getX() - 2, _car->getPosition().getY() + 1, _car->getPosition().getZ() + 0.25,
			_car->getPosition().getX() + 5, _car->getPosition().getY(), _car->getPosition().getZ(),
			0, 1, 0);
		std::cout << "Camera 3" << std::endl;
	}*/
	
	else { // Camara do carro
		loadIdentity(PROJECTION);
		perspective(53.13f, 1.0f, 0.1f, 1000.0f);
		loadIdentity(VIEW);
		loadIdentity(MODEL);
		lookAt(_car->getPosition().getX() - 2, _car->getPosition().getY() + 1, _car->getPosition().getZ() + 0.25,
			_car->getPosition().getX() + 5 - camX, _car->getPosition().getY() - camY, _car->getPosition().getZ() - camZ,
			0, 1, 0);
		/*lookAt(_car->getPosition().getX() - _car->getDirection().getX()*2, _car->getPosition().getY() + 1, _car->getPosition().getZ() - _car->getDirection().getZ()*2,
			_car->getPosition().getX(), _car->getPosition().getY()+1, _car->getPosition().getZ(),
			0, 1, 0);*/
	}

	// use our shader
	glUseProgram(shader.getProgramIndex());

	//send the light position in eye coordinates

	//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 

	float res[4];
	multMatrixPoint(VIEW, lightPos,res);   //lightPos definido em World Coord so is converted to eye space
	glUniform4fv(lPos_uniformId, 1, res);

	drawTable();
	drawButterPackets();
	drawOranges();
	drawCar();
	drawCheerios();

	glutSwapBuffers();
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
		if (o->getPosition().getX() > 12.0 || o->getPosition().getX() < -12.0 || o->getPosition().getZ() > 12.0 || o->getPosition().getX() < -12.0) {
			o->reset(Vector3(rand() % 8,2.0f, rand() % 8), glutGet(GLUT_ELAPSED_TIME));
		}
	}
}

void GameManager::refresh()
{
	_elapsed = glutGet(GLUT_ELAPSED_TIME);
	_delta_t = _elapsed - _elap;
	_elap = _elapsed;

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
		case 'm': glEnable(GL_MULTISAMPLE); break;
		case 'n': glDisable(GL_MULTISAMPLE); break;
		case 'q': 
			_car->accelerationIncrease();
			break;
		case 'a': 
			//printf("backwards|stop\n");
			_car->accelerationDecrease();
			break; //backwards|stop movement
		case 'o': 
			//printf("left\n"); 
		
			_car->steerLeft();
			break; //steer left
		case 'p': 
			//printf("right\n"); 
			_car->steerRight();
			break; //steer right
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
				//_activeCamera = _perspectiveBehind;
				_cameraLook = 3;
				loadIdentity(PROJECTION);
				perspective(53.13f, 1.0f, 0.1f, 1000.0f);
				loadIdentity(VIEW);
				loadIdentity(MODEL);
				lookAt(_car->getPosition().getX() - 2, _car->getPosition().getY() + 1, _car->getPosition().getZ() + 0.25,
					_car->getPosition().getX() + 5, _car->getPosition().getY(), _car->getPosition().getZ(),
					0, 1, 0);
			}
			
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
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	
	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
	
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

	for (int i = -5; i <= 5; i++) {
		for (int j = -5; j <= 5; j++) {
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
	for (float i = -4.5; i <= 4.5; i++) {
		for (float j = -4.5; j <= 4.5; j++) {
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
	for (float i = -4.5; i <= 4.5; i++) {
		for (int j = -5; j <= 5; j++) {
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
	for (int i = -5; i <= 5; i++) {
		for (float j = -4.5; j <= 4.5; j++) {
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
	float x1[] = { -7.0f, -3.0f, 7.0f };
	float z1[] = { 2.0f, -3.0f, 4.0f };

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
			translate(MODEL, 5.0f - i * 0.95f, 1.10f, -4.3f - j *2.7f);
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