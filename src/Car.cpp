#include "Car.h"

Car::Car() {
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
	createPawn();

	mesh->push_back(aux);
}

Car::~Car() {}
void Car::draw() {}

float* Car::boxLimits() {
	return nullptr;
}