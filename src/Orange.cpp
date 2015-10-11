#include "Orange.h"
#include "Vector3.h"
#include "GameManager.h"

Orange::Orange() {
	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 0.8f, 0.6f, 0.4f, 1.0f };
	float spec[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;

	std::vector<struct MyMesh> *mesh = this->getMesh();
	struct MyMesh aux;

	memcpy(aux.mat.ambient, amb, 4 * sizeof(float));
	memcpy(aux.mat.diffuse, diff, 4 * sizeof(float));
	memcpy(aux.mat.specular, spec, 4 * sizeof(float));
	memcpy(aux.mat.emissive, emissive, 4 * sizeof(float));
	aux.mat.shininess = shininess;
	aux.mat.texCount = texcount;
	createSphere(1.0f, 20);

	mesh->push_back(aux);
}

Orange::~Orange() {}

void Orange::draw() {}

float* Orange::boxLimits() {
	return nullptr;
}