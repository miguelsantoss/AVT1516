#include "Table.h"


Table::Table() {
	float amb[] = { 0.3f, 0.0f, 0.0f, 1.0f };
	float diff[] = { 0.8f, 0.1f, 0.1f, 1.0f };
	float spec[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 500.0;
	int texcount = 0;

	std::vector<struct MyMesh> *mesh = this->getMesh();
	struct MyMesh aux;

	memcpy(aux.mat.ambient, amb, 4 * sizeof(float));
	memcpy(aux.mat.diffuse, diff, 4 * sizeof(float));
	memcpy(aux.mat.specular, spec, 4 * sizeof(float));
	memcpy(aux.mat.emissive, emissive, 4 * sizeof(float));
	aux.mat.shininess = shininess;
	aux.mat.texCount = texcount;
	createCylinder(1.5f, 0.5f, 20);

	mesh->push_back(aux);
}

Table::~Table() {
}


void Table::draw() {}