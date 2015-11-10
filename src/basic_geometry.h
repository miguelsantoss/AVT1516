#ifndef __BASIC_GEOMETRY_H__
#define __BASIC_GEOMETRY_H__

#include <GL/glew.h>
#include <GL/freeglut.h>

#define MAX_TEXTURES_ 4

struct Material{
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float shininess;
	int texCount;
};

// A model can be made of many meshes. Each is stored  in the following structure
struct MyMesh {
		GLuint vao;
		GLuint texUnits[MAX_TEXTURES_];
		GLuint texTypes[MAX_TEXTURES_];
		float transform[16];
		int numIndexes;
		unsigned int type;
		struct Material mat;
	};

void createCube();
void createSquare();
void createSphere(float radius, int divisions);
void createTorus(float innerRadius, float outerRadius, int rings, int sides);
void createCylinder(float height, float radius, int sides);
void createCone(float height, float baseRadius, int sides);
void createPawn();
void computeVAO(int numP, float *p, float *pfloatoints, int sides, float smoothCos);
void create (float *p, int numP, int sides, int closed, float smoothCos);
int revSmoothNormal2(float *p, float *nx, float *ny, float smoothCos, int beginEnd);
float *circularProfile(float minAngle, float maxAngle, float radius, int divisions, float transX= 0.0f, float transY = 0.0f);

#endif