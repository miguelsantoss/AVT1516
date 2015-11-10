#include <GL/glew.h>
#include <GL/freeglut.h>
#include <ctime> 

#include "GameManager.h"
#include "TGA.h"


#define CAPTION "MicroMachines"
#define ORANGE_RADIUS 1.0f

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
const int CANDLE_STICK = 10;

const int AMBIENT_LIGHT = 0;
const int CANDLE_0 = 1;
const int CANDLE_1 = 2;
const int CANDLE_2 = 3;
const int CANDLE_3 = 4;
const int CANDLE_4 = 5;
const int CANDLE_5 = 6;
const int CAR_HEADLIGHT_LEFT = 7;
const int CAR_HEADLIGHT_RIGHT = 8;

int angle = 0;
int rotation = 0;
struct MyMesh mesh[12];
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
float auxMatrix[16];

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
	srand((unsigned)time(0));
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	camX = camY = camZ = 0;

	_car = new Car(Vector3(1.9f, 1.15f, 3.0f));
	_car->setDirection(1.0f, 0.0f, 0.0f);

	_oranges.push_back(new Orange(Vector3(17.0f, 2.0f, 5.0f)));
	_oranges.push_back(new Orange(Vector3(10.0f, 2.0f, 10.0f)));
	_oranges.push_back(new Orange(Vector3(4.0f, 2.0f, 17.0f)));

	//scale(MODEL, 1.3f, 0.5f, 0.7f);
	_butters.push_back(new ButterPacket(Vector3(2.0f, 1.0f, 17.0f), 0.5,0.3));
	_butters.push_back(new ButterPacket(Vector3(13.0f, 1.0f, 10.0f), 0.5, 0.3));
	_butters.push_back(new ButterPacket(Vector3(18.0f, 1.0f, 14.0f), 0.5, 0.3));

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 2; j++) {
			_cheerios.push_back(new Cheerio(Vector3(10.0f - i * 0.95f, 1.10f, 4.3f - j *2.7f), -0.125, 0.125));
		}
	}

	glGenTextures(3, TextureArray);

	TGA_Texture(TextureArray, "stone.tga", 0);
	TGA_Texture(TextureArray, "lightwood.tga", 1);
	TGA_Texture(TextureArray, "fonts/font1.tga", 2);
	TGA_Texture(TextureArray, "tree.tga", 3);

	_fontSize = 16;

	createLights();
	createTable();
	createButterPackets();
	createOranges();
	createCar();
	createCheerios();
	createCandleSticks();
	initTextureMappedFont();
	createTreeSquare();

	// Cameras
	_cameraLook = 1;
	_orthogonalCamera = new OrthogonalCamera(0.0, 20.0, 0.0, 20.0, -10.0, 10.0);
	_perspectiveTop = new PerspectiveCamera(53.13f, 1.0f, 0.1f, 1000.0f);
	_perspectiveBehind = new PerspectiveCamera(53.13f, 1.0f, 0.1f, 1000.0f);
	_scoreCamera = new OrthogonalCamera(0.0f, float(WinX), 0.0f, float(WinY), -1.0f, 1.0f);
	_activeCamera = _orthogonalCamera;
	_fogColor[0] = 0.16f;
	_fogColor[1] = 0.62f;
	_fogColor[2] = 0.77f;
	_fogDensity = 0.1f;
	_fogMode = 2;
	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.16f, 0.62f, 0.77f, 1.0f);
}

void GameManager::restartGame() {
	_elap = 0;
	_gameOver = false;

	delete(_car);

	_car = new Car(Vector3(1.9f, 1.15f, 3.0f));
	_car->setDirection(1.0f, 0.0f, 0.0f);

	_score = 0;
	_lives = 5;
	_oranges[0]->setPosition(Vector3(17.0f, 2.0f, 5.0f));
	_oranges[1]->setPosition(Vector3(10.0f, 2.0f, 10.0f));
	_oranges[2]->setPosition(Vector3(4.0f, 2.0f, 17.0f));

	_activeCamera = _orthogonalCamera;


	_lights[AMBIENT_LIGHT]->setState(false);
	_lights[CANDLE_0]->setState(true);
	_lights[CANDLE_1]->setState(true);
	_lights[CANDLE_2]->setState(true);
	_lights[CANDLE_3]->setState(true);
	_lights[CANDLE_4]->setState(true);
	_lights[CANDLE_5]->setState(true);
	_lights[CAR_HEADLIGHT_LEFT]->setState(true);
	_lights[CAR_HEADLIGHT_RIGHT]->setState(true);
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureArray[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureArray[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureArray[2]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureArray[3]);

	glUniform1f(fogDensity, _fogDensity);
	glUniform1i(fogState, _fogState);
	glUniform1i(fogMode, _fogMode);
	glUniform3fv(fogColor, 1, _fogColor);

	glUniform1i(texMode, 1);
	glUniform1i(tex_loc_1, 0);
	glUniform1i(tex_loc_2, 1);
	glUniform1i(texUse, 1);
	drawTable();
	glUniform1i(tex_loc_1, 1);
	glUniform1i(texMode, 2);
	drawOranges();
	glUniform1i(tex_loc_1, 3);
	drawTreeSquare();
	//billboardEnd();
	glUniform1i(texUse, 0);
	drawButterPackets();
	drawCheerios();
	
	glDisable(GL_CULL_FACE);
	drawCandleSticks();
	drawCar();
	glEnable(GL_CULL_FACE);
	glUniform1i(writeMode, 1);
	glUniform1i(vWriteMode, 1);

	_scoreCamera->computeProjection();
	glUniform1i(useLights, 0);
	glUniform1i(texUse, 1);
	glUniform1i(tex_loc_1, 2);
	_fontSize = 16;
	std::string s = "Score:" + std::to_string(_score);
	DrawString(635, 560, s);

	s = "Lives:" + std::to_string(_lives);
	DrawString(635, 580, s);
	_fontSize = 25;
	if (_paused) {
		DrawString(345, 280, "Paused!");
	}
	if (_gameOver) {
		DrawString(315, 320, "Game Over!");
		DrawString(135, 280, "Press R to Restart the game");
	}
	glUniform1i(writeMode, 0);
	glUniform1i(vWriteMode, 0);
	glUniform1i(useLights, 1);
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

void GameManager::createLights(void) {
	Vector4* ambient_candle = new Vector4(1.0f, 0.57f, 0.16f, 1.0f);
	Vector4* diffuse_candle = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4* specular_candle = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	Vector4* ambient_headlight = new Vector4(0.78f, 0.88f, 1.0f, 1.0f);
	Vector4* diffuse_headlight = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4* specular_headlight = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	DirectionalLightSource* ambient = new DirectionalLightSource(AMBIENT_LIGHT);
	PointLightSource* candle_0 = new PointLightSource(CANDLE_0); candle_0->setPosition(new Vector4(4.0f, 2.0f, 6.0f, 1.0f));
	PointLightSource* candle_1 = new PointLightSource(CANDLE_1); candle_1->setPosition(new Vector4(15.0f, 2.0f, 3.0f, 1.0f));
	PointLightSource* candle_2 = new PointLightSource(CANDLE_2); candle_2->setPosition(new Vector4(15.0f, 2.0f, 10.0f, 1.0f));
	PointLightSource* candle_3 = new PointLightSource(CANDLE_3); candle_3->setPosition(new Vector4(5.0f, 2.0f, 10.0f, 1.0f));
	PointLightSource* candle_4 = new PointLightSource(CANDLE_4); candle_4->setPosition(new Vector4(5.0f, 2.0f, 18.0f, 1.0f));
	PointLightSource* candle_5 = new PointLightSource(CANDLE_5); candle_5->setPosition(new Vector4(15.0f, 2.0f, 18.0f, 1.0f));
	PointLightSource* car_headlight_left = new PointLightSource(CAR_HEADLIGHT_LEFT);
	PointLightSource* car_headlight_right = new PointLightSource(CAR_HEADLIGHT_RIGHT);

	_lights.push_back(ambient);
	_lights.push_back(candle_0);
	_lights.push_back(candle_1);
	_lights.push_back(candle_2);
	_lights.push_back(candle_3);
	_lights.push_back(candle_4);
	_lights.push_back(candle_5);
	_lights.push_back(car_headlight_left);
	_lights.push_back(car_headlight_right);

	ambient->setPosition(new Vector4(1.0f, -1.0f, 0.0f, 0.0f));

	ambient->setAmbient(new Vector4(0.4f, 0.4f, 0.4f, 1.0f));
	ambient->setDiffuse(new Vector4(0.3f, 0.3f, 0.3f, 1.0f));
	ambient->setSpecular(new Vector4(0.3f, 0.3f, 0.3f, 1.0f));
	ambient->setState(false);
	ambient->setIsLocal(false);

	int k = 1;
	for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 3; j++) {
			PointLightSource* candle = dynamic_cast<PointLightSource*>(_lights[k]);
			candle->setAmbient(ambient_candle);
			candle->setDiffuse(diffuse_candle);
			candle->setSpecular(specular_candle);

			candle->setState(true);
			candle->setIsSpot(false);
			candle->setIsLocal(true);

			candle->setConstantAttenuation(0.01);
			candle->setLinearAttenuation(0);
			candle->setQuadraticAttenuation(1);

			k++;
		}
	}

	car_headlight_left->setAmbient(ambient_headlight);
	car_headlight_left->setDiffuse(diffuse_headlight);
	car_headlight_left->setSpecular(specular_headlight);
	car_headlight_right->setAmbient(ambient_headlight);
	car_headlight_right->setDiffuse(diffuse_headlight);
	car_headlight_right->setSpecular(specular_headlight);

	car_headlight_left->setIsLocal(true);
	car_headlight_left->setIsSpot(true);
	car_headlight_left->setState(true);
	car_headlight_right->setIsLocal(true);
	car_headlight_right->setIsSpot(true);
	car_headlight_right->setState(true);

	car_headlight_left->setConstantAttenuation(0.01);
	car_headlight_left->setLinearAttenuation(0);
	car_headlight_left->setQuadraticAttenuation(0.1);
	car_headlight_right->setConstantAttenuation(0.01);
	car_headlight_right->setLinearAttenuation(0);
	car_headlight_right->setQuadraticAttenuation(0.1);


	Vector3 car_pos = _car->getPosition();
	Vector4* spot_position_left = new Vector4(car_pos.getX() + 0.55f, car_pos.getY(), car_pos.getZ() - 0.15f, 1.0f);
	Vector4* spot_position_right = new Vector4(car_pos.getX() + 0.55f, car_pos.getY(), car_pos.getZ() + 0.15f, 1.0f);
	Vector4* spot_direction = new Vector4(2.0f, -0.7f, 0.0f, 0.0f);
	
	car_headlight_left->setCutOff(70.0f);
	car_headlight_left->setExponent(4.0f);
	car_headlight_right->setCutOff(70.0f);
	car_headlight_right->setExponent(4.0f);
	update_car_headlights();

}

void GameManager::update_car_headlights(void) {
	Vector3 car_pos = _car->getPosition();
	Vector3 car_dir = _car->getDirection();
	float angle = _car->getAngle();

	float newXleft = 0.55f * cos(angle * 3.14f / 180.0f) + sin(angle * 3.14f / 180.0f) * (-0.15f) + car_pos.getX();
	float newZleft = -sin(angle * 3.14f / 180.0f) * 0.55f + cos(angle * 3.14f / 180.0f) * (-0.15f) + car_pos.getZ();

	float newXright = 0.55f * cos(angle * 3.14f / 180.0f) + sin(angle * 3.14f / 180.0f) * 0.15f + car_pos.getX();
	float newZright = -sin(angle * 3.14f / 180.0f) * 0.55f + cos(angle * 3.14f / 180.0f) * 0.15f + car_pos.getZ();

	Vector4* spot_position_left = new Vector4(newXleft, car_pos.getY(), newZleft, 1.0f);
	Vector4* spot_position_right = new Vector4(newXright, car_pos.getY(), newZright, 1.0f);
	Vector4* spot_direction = new Vector4(car_dir.getX(), -0.7f, car_dir.getZ(), 0.0f);

	_lights[CAR_HEADLIGHT_LEFT]->setPosition(spot_position_left);
	_lights[CAR_HEADLIGHT_LEFT]->setDirection(spot_direction);
	_lights[CAR_HEADLIGHT_RIGHT]->setPosition(spot_position_right);
	_lights[CAR_HEADLIGHT_RIGHT]->setDirection(spot_direction);
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
		if (o->getPosition().getX() > 21.0 || o->getPosition().getX() < 0.0 || o->getPosition().getZ() > 21.0 || o->getPosition().getZ() < 0.0) {
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
void GameManager::updateButters(void)
{
	ButterPacket* o;
	std::vector<ButterPacket*>::iterator to_up;
	for (to_up = _butters.begin(); to_up != _butters.end(); to_up++) {
		o = static_cast<ButterPacket*>(*to_up);
		o->update(_delta_t);
	}
}
void GameManager::updateCheerios(void)
{
	Cheerio* o;
	std::vector<Cheerio*>::iterator to_up;
	for (to_up = _cheerios.begin(); to_up != _cheerios.end(); to_up++) {
		o = static_cast<Cheerio*>(*to_up);
		o->update(_delta_t);
	}
}

void GameManager::destroyCar() {
	//resetPosition;
	--_lives;
	if (_lives <= 0) {
		_gameOver = true;
	}
}

void GameManager::update(double delta_t) {
	if (_paused || _gameOver) { glutPostRedisplay(); return; }
	_car->update(_delta_t);
	if (_car->getPosition().getX() > 20.0 || _car->getPosition().getX() < 0.0 || _car->getPosition().getZ() > 20.0 || _car->getPosition().getZ() < 0.0) {
		_lives -= 1;
		_car = new Car(Vector3(1.9f, 1.15f, 3.0f));
		_car->setDirection(1.0f, 0.0f, 0.0f);
	}
	for (int i = 0; i < 3; i++) {
		if (_car->checkColision(_oranges[i])) {
			_lives -= 1;
			_car = new Car(Vector3(1.9f, 1.15f, 3.0f));
			_car->setDirection(1.0f, 0.0f, 0.0f);
			
		}
		if (_car->checkColision(_butters[i])) {
			_butters[i]->dealColision(_car->getDirection(), _car->getAcceleration(), _car->getSpeed());
			_car->accelerationStop();
			_car->dealColision();
		}
	}
	for (int i = 0; i < 20; i++) {
		if (_car->checkColision(_cheerios[i])) {
			_cheerios[i]->dealColision(_car->getDirection(), _car->getAcceleration(), _car->getSpeed());
			_car->accelerationStop();
			_car->dealColision();
		}
	}

	if (_lives == 0) _gameOver = true;
	_score += static_cast<int>(_car->getDistanceDone() * 100);
	update_car_headlights();
	updateOranges();
	updateButters();
	updateCheerios();
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

		case 'd': 
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
		case 't':
			_lives = 0;
			_gameOver = true;
			break;
		case 'r':
			if (_lives == 0) {
				restartGame();
			}
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
			_lights[AMBIENT_LIGHT]->setState(!_lights[AMBIENT_LIGHT]->getState());
			break;
		case 'c':
			_lights[CANDLE_0]->setState(!_lights[CANDLE_0]->getState());
			_lights[CANDLE_1]->setState(!_lights[CANDLE_1]->getState());
			_lights[CANDLE_2]->setState(!_lights[CANDLE_2]->getState());
			_lights[CANDLE_3]->setState(!_lights[CANDLE_3]->getState());
			_lights[CANDLE_4]->setState(!_lights[CANDLE_4]->getState());
			_lights[CANDLE_5]->setState(!_lights[CANDLE_5]->getState());
			break;
		case 'h':
			_lights[CAR_HEADLIGHT_LEFT]->setState(!_lights[CAR_HEADLIGHT_LEFT]->getState());
			_lights[CAR_HEADLIGHT_RIGHT]->setState(!_lights[CAR_HEADLIGHT_RIGHT]->getState());
			break;
		case 'i':
			_fogState = !_fogState;
			break;
		case '8':
			_fogMode = 0;
			break;
		case '9':
			_fogMode = 1;
			break;
		case '0':
			_fogMode = 2;
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
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_ATTRIB1, "vVertex");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_ATTRIB2, "vtexCoord");
	glLinkProgram(shader.getProgramIndex());

	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	tex_loc_1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc_2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");
	texUse = glGetUniformLocation(shader.getProgramIndex(), "useTextures");
	useLights = glGetUniformLocation(shader.getProgramIndex(), "useLights");
	writeMode = glGetUniformLocation(shader.getProgramIndex(), "writingMode");
	vWriteMode = glGetUniformLocation(shader.getProgramIndex(), "vWritingMode");
	texMode = glGetUniformLocation(shader.getProgramIndex(), "texMode");
	fogState = glGetUniformLocation(shader.getProgramIndex(), "fogState");
	fogMode = glGetUniformLocation(shader.getProgramIndex(), "fogMode");
	fogColor = glGetUniformLocation(shader.getProgramIndex(), "fogColor");
	fogDensity = glGetUniformLocation(shader.getProgramIndex(), "fogDensity");
	
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

	float amb3[] = { 0.26f,0.25f,1.0f,0.3f };
	float diff3[] = { 0.52f,1.0f,1.0f,0.3f };
	float spec3[] = { 1.0f,1.0f,1.0f,0.3f };
	float emissive3[] = { 0.0f, 0.0f, 0.0f, 0.3f };
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
	createSphere(ORANGE_RADIUS, 20);
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

void GameManager::createCheerios(void) {
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

void GameManager::createCandleSticks(void) {
	float amb[] = { 0.8f,0.8f,0.79f,0.95f };
	float diff[] = { 0.8f,0.74f,0.42f,0.95f };
	float spec[] = { 0.83f,0.8f,0.49f,0.95f };
	float emissive[] = { 1.0f, 0.57f, 0.16f, 0.95f };
	float shininess = 76.8f;
	int texcount = 0;
	objId = CANDLE_STICK;

	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCylinder(2.0f, 1.0f, 30);
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
	translate(MODEL, 0.13f, 1.55f, 0.13f);
	scale(MODEL, 0.74f, 0.05f, 0.74f);

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
	float circumference = 2 * 3.14f * ORANGE_RADIUS;
	float angle;

	for (int i = 0; i < 3; i++) {
		angle = _oranges[i]->getAngle();
		angle += _oranges[i]->getDistanceDone() / circumference * 360;
		_oranges[i]->setDistanceDone(0);
		//std::cout << "test: " << angle << std::endl;
		_oranges[i]->setAngle(angle);
		float normalized = _oranges[i]->getSpeed().getX() * _oranges[i]->getSpeed().getX() + _oranges[i]->getSpeed().getZ() * _oranges[i]->getSpeed().getZ();
		normalized = sqrt(normalized);
		Vector3 normalized_speed = Vector3(_oranges[i]->getSpeed().getX() / normalized, 0.0f, _oranges[i]->getSpeed().getZ() / normalized);
		Vector3 rotation = Vector3(normalized_speed.getZ(), 0.0f, -normalized_speed.getX());
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
		rotate(MODEL, angle, rotation.getX(), rotation.getY(), rotation.getZ());
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
		translate(MODEL, _butters[i]->getPosition().getX(), _butters[i]->getPosition().getY(), _butters[i]->getPosition().getZ());

		pushMatrix(MODEL);
		translate(MODEL, -0.65, 0.0, -0.35);
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
		popMatrix(MODEL);
	}
}

void GameManager::drawCheerios() {
	GLint loc;
	objId = CHEERIO;
	// send the material
	for (int i = 0; i < 20; i++) {
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
			translate(MODEL, _cheerios[i]->getPosition().getX(), _cheerios[i]->getPosition().getY(), _cheerios[i]->getPosition().getZ());
			translate(MODEL, -0.125, 0.0, -0.125);
			scale(MODEL, 0.25f, 0.25f, 0.25f);

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

void GameManager::drawCandleSticks() {
	GLint loc;
	objId = CANDLE_STICK;
	// send the material
	for (int i = CANDLE_0; i <= CANDLE_5; i++) {
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			translate(MODEL, _lights[i]->getPosition()->getX(), 1.0f, _lights[i]->getPosition()->getZ());
			scale(MODEL, 0.3f, 0.7f, 0.3f);
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


void GameManager::setUpLights(void) {
	GLint loc;

	float res[4];
	multMatrixPoint(VIEW, _lights[0]->getPosition()->getArray(), res);

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].direction");
	glUniform4fv(loc, 1, res);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].ambient");
	glUniform4fv(loc, 1, _lights[0]->getAmbient()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].diffuse");
	glUniform4fv(loc, 1, _lights[0]->getDiffuse()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].specular");
	glUniform4fv(loc, 1, _lights[0]->getSpecular()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isEnabled");
	glUniform1i(loc, _lights[0]->getState());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isLocal");
	glUniform1i(loc, _lights[0]->getIsLocal());


	std::string beg = "lights[";
	std::string end = "].";
	std::string types[] = { "isLocal", "isSpot", "isEnabled", "ambient", "diffuse", "specular", "position_point",
		"constantAttenuation", "linearAttenuation", "quadraticAttenuation" };
	std::string fin = "";

	for (int i = 1; i < 7; i++) {
		PointLightSource* light = dynamic_cast<PointLightSource*>(_lights[i]);
		fin = beg + std::to_string(i) + end + types[0];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform1i(loc, light->getIsLocal());

		fin = beg + std::to_string(i) + end + types[1];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform1i(loc, light->getIsSpot());

		fin = beg + std::to_string(i) + end + types[2];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform1i(loc, light->getState());

		fin = beg + std::to_string(i) + end + types[3];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform4fv(loc, 1, light->getAmbient()->getArray());

		fin = beg + std::to_string(i) + end + types[4];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform4fv(loc, 1, light->getDiffuse()->getArray());

		fin = beg + std::to_string(i) + end + types[5];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform4fv(loc, 1, light->getSpecular()->getArray());

		multMatrixPoint(VIEW, light->getPosition()->getArray(), res);
		fin = beg + std::to_string(i) + end + types[6];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform3fv(loc, 1, res);

		fin = beg + std::to_string(i) + end + types[7];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform1f(loc, light->getConstantAttenuation());
		fin = beg + std::to_string(i) + end + types[8];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform1f(loc, light->getLinearAttenuation());
		fin = beg + std::to_string(i) + end + types[9];
		loc = glGetUniformLocation(shader.getProgramIndex(), fin.c_str());
		glUniform1f(loc, light->getQuadraticAttenuation());
	}


	PointLightSource* car_headlight_left = dynamic_cast<PointLightSource*>(_lights[CAR_HEADLIGHT_LEFT]);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].isEnabled");
	glUniform1i(loc, car_headlight_left->getState());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].isLocal");
	glUniform1i(loc, car_headlight_left->getIsLocal());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].isSpot");
	glUniform1i(loc, car_headlight_left->getIsSpot());

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].ambient");
	glUniform4fv(loc, 1, car_headlight_left->getAmbient()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].diffuse");
	glUniform4fv(loc, 1, car_headlight_left->getDiffuse()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].specular");
	glUniform4fv(loc, 1, car_headlight_left->getSpecular()->getArray());

	multMatrixPoint(VIEW, car_headlight_left->getPosition()->getArray(), res);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].position");
	glUniform4fv(loc, 1, res);
	multMatrixPoint(VIEW, car_headlight_left->getDirection()->getArray(), res);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].direction");
	glUniform4fv(loc, 1, res);

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].spotCosCutoff");
	glUniform1f(loc, cos(car_headlight_left->getCutOff()));
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].spotExponent");
	glUniform1f(loc, car_headlight_left->getExponent());

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].constantAttenuation");
	glUniform1f(loc, car_headlight_left->getConstantAttenuation());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].linearAttenuation");
	glUniform1f(loc, car_headlight_left->getLinearAttenuation());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].quadraticAttenuation");
	glUniform1f(loc, car_headlight_left->getQuadraticAttenuation());



	PointLightSource* car_headlight_right = dynamic_cast<PointLightSource*>(_lights[CAR_HEADLIGHT_RIGHT]);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].isEnabled");
	glUniform1i(loc, car_headlight_right->getState());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].isLocal");
	glUniform1i(loc, car_headlight_right->getIsLocal());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].isSpot");
	glUniform1i(loc, car_headlight_right->getIsSpot());

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].ambient");
	glUniform4fv(loc, 1, car_headlight_right->getAmbient()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].diffuse");
	glUniform4fv(loc, 1, car_headlight_right->getDiffuse()->getArray());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].specular");
	glUniform4fv(loc, 1, car_headlight_right->getSpecular()->getArray());

	multMatrixPoint(VIEW, car_headlight_right->getPosition()->getArray(), res);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].position");
	glUniform4fv(loc, 1, res);
	multMatrixPoint(VIEW, car_headlight_right->getDirection()->getArray(), res);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].direction");
	glUniform4fv(loc, 1, res);

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].spotCosCutoff");
	glUniform1f(loc, cos(car_headlight_right->getCutOff()));
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].spotExponent");
	glUniform1f(loc, car_headlight_right->getExponent());

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].constantAttenuation");
	glUniform1f(loc, car_headlight_right->getConstantAttenuation());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].linearAttenuation");
	glUniform1f(loc, car_headlight_right->getLinearAttenuation());
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].quadraticAttenuation");
	glUniform1f(loc, car_headlight_right->getQuadraticAttenuation());
}

void GameManager::initTextureMappedFont() {
	float vertices[] = {
		0.0f, 0.0f,
		_fontSize, 0.0f,
		_fontSize, _fontSize,
		0.0f, _fontSize
	};

	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);
	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTEX_ATTRIB1);
	glVertexAttribPointer(VERTEX_ATTRIB1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//Just initialize with something for now, the tex coords are updated
		//for each character printed
		float texCoords[] = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f
	};

	glGenBuffers(1, &_texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &texCoords[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(VERTEX_ATTRIB2);
	glVertexAttribPointer(VERTEX_ATTRIB2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//set the orthographic projection matrix
	//ortho(0.0f, float(WinX), 0.0f, float(WinY), -1.0f, 1.0f);
}



void GameManager::DrawString(float x, float y, const std::string& str) {
	GLint test[1];
	glGetIntegerv(GL_DEPTH_TEST, test);
	glDisable(GL_DEPTH_TEST);

	float texCoords[8];

	pushMatrix(MODEL);
	translate(MODEL, x, y, 0.0f);
	glBindVertexArray(_vaoID);
	// glTranslatef(x, y, 0.0); //Position our text
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		const float oneOverSixteen = 1.0f / 16.0f;

		int ch = int(str[i]);
		float xPos = float(ch % 16) * oneOverSixteen;
		float yPos = float(ch / 16) * oneOverSixteen;

		texCoords[0] = xPos;
		texCoords[1] = 1.0f - yPos - oneOverSixteen;

		texCoords[2] = xPos + oneOverSixteen;
		texCoords[3] = 1.0f - yPos - oneOverSixteen;

		texCoords[4] = xPos + oneOverSixteen;
		texCoords[5] = 1.0f - yPos - 0.001f;

		texCoords[6] = xPos;
		texCoords[7] = 1.0f - yPos - 0.001f;

		glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, &texCoords[0]);

		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		translate(MODEL, _fontSize * 0.8f, 0.0f, 0.0f);
	}
	glBindVertexArray(0);
	popMatrix(MODEL);

	glEnable(GL_DEPTH_TEST);
}

void GameManager::createTreeSquare() {
	float amb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diff[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 0.25f;
	int texcount = 0;
	objId = 11;

	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createSquare();
}

void GameManager::drawTreeSquare() {
	GLint loc;
	objId = 11;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);

	pushMatrix(MODEL);
	
	translate(MODEL, 10.0f, 1.0f, 10.0f);
	translate(MODEL, 0.5f, 0.0f, 1.0f);
	//rotate(MODEL, angle, 0.0f, 1.0f, 0.0f);
	billboardRotation(10.0f, 1.0f, 10.0f);
	translate(MODEL, -0.5f, 0.0f, -1.0f);
	scale(MODEL, 1.4f, 2.0f, 1.4f);

	
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
	translate(MODEL, 5.0f, 1.0f, 15.0f);
	translate(MODEL, 0.5f, 0.0f, 1.0f);
	//rotate(MODEL, angle, 0.0f, 1.0f, 0.0f);
	billboardRotation(5.0f, 1.0f, 15.0f);
	translate(MODEL, -0.5f, 0.0f, -1.0f);
	scale(MODEL, 1.4f, 2.0f, 1.4f);
	//billboardCylindricalBegin(10.0f, 1.0f, 5.5f);


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
	translate(MODEL, 15.0f, 1.0f, 5.0f);
	translate(MODEL, 0.5f, 0.0f, 1.0f);
	//rotate(MODEL, angle, 0.0f, 1.0f, 0.0f);
	billboardRotation(15.0f, 1.0f, 5.0f);
	translate(MODEL, -0.5f, 0.0f, -1.0f);
	scale(MODEL, 1.4f, 2.0f, 1.4f);
	//billboardCylindricalBegin(10.0f, 1.0f, 5.5f);


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

void GameManager::billboardRotation(float objPosX, float objPosY, float objPosZ) {

	float lookAt[3], objToCamProj[3], upAux[3];
	float angleCosine;

	// objToCamProj is the vector in world coordinates from the 
	// local origin to the camera projected in the XZ plane
	Vector3 direction = _car->getDirection();
	float xcamX, xcamZ;
	if (_cameraLook == 1) {
		xcamX = 0;
		xcamZ = 0;
	}
	else if (_cameraLook == 2) {
		xcamX = -5;
		xcamZ = -5;
	}
	else if (_cameraLook == 3) {
		xcamX = _car->getPosition().getX() - 2 * direction.getX();
		xcamZ = _car->getPosition().getZ() - 2 * direction.getZ();
	}

	objToCamProj[0] = xcamX - objPosX;
	objToCamProj[1] = 0;
	objToCamProj[2] = xcamZ - objPosZ;

	// This is the original lookAt vector for the object 
	// in world coordinates
	lookAt[0] = 0;
	lookAt[1] = 0;
	lookAt[2] = 1;


	// normalize both vectors to get the cosine directly afterwards
	normalize(objToCamProj);

	// easy fix to determine wether the angle is negative or positive
	// for positive angles upAux will be a vector pointing in the 
	// positive y direction, otherwise upAux will point downwards
	// effectively reversing the rotation.

	crossProduct(upAux, lookAt, objToCamProj);

	// compute the angle
	angleCosine = innerProduct(lookAt, objToCamProj);

	// perform the rotation. The if statement is used for stability reasons
	// if the lookAt and objToCamProj vectors are too close together then 
	// |angleCosine| could be bigger than 1 due to lack of precision
	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		rotate(MODEL, acos(angleCosine) * 180 / 3.14, upAux[0], upAux[1], upAux[2]);
}

void GameManager::normalize(float* v) {
	int len = 3;
	float norm = 0.0f;
	for (int i = 0; i < len; i++) {
		norm += v[i] * v[i];
	}
	norm = sqrt(norm);
	for (int i = 0; i < len; i++) {
		v[i] = v[i] / norm;
	}
}

void GameManager::crossProduct(float* v1, float* v2, float* v3) {
	v1[0] = v2[1] * v3[2] - v2[2] * v3[1];
	v1[1] = v2[2] * v3[0] - v2[0] * v3[2];
	v1[2] = v2[0] * v3[1] - v2[1] * v3[0];
}

float GameManager::innerProduct(float* v1, float* v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}