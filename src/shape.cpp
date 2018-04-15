#include "shape.h"



void shape::setup()
{
	object.setup();
}

ofMesh shape::loadObj(string filename) {
	ifstream in(filename, ios::in);
	if (!in)
	{
		cerr << "Cannot open " << filename << endl;
		//exit(1);
	}

	object.resetMesh();

	loadedModel.loadModel(filename);
	object.mesh = loadedModel.getMesh(0);

	//cout << testMesh.getNumVertices() << endl;

	object.vertsBasePos = object.mesh.getVertices();
	object.vertsDeformedPos = object.mesh.getVertices();
	object.vertsDeformeFrom = object.mesh.getVertices();
	object.vertNormals = object.mesh.getNormals();
	object.setNormals(object.mesh);
	object.vertsDeformeFrom = object.vertsBasePos;
	object.vertsBaseNormals = object.mesh.getNormals();

	return object.mesh;
}
ofMesh shape::loadObjBlendshape(const char* filename) {
	ifstream in(filename, ios::in);
	if (!in)
	{
		cerr << "Cannot open " << filename << endl;
		//exit(1);
	}

	loadedModel.loadModel(filename);
	object.blendshapeMesh = loadedModel.getMesh(0);
	object.blendshapeLoaded = true;
	return object.blendshapeMesh;
}
void shape::circle(int steps, float radius, ofVec3f pos) {
	float angle = 0;
	for (int r = 0; r < steps; r++) {

		float x = sin(angle)*radius;
		float z = cos(angle)*radius;

		ofPoint newPos(x + pos.x, 0 + pos.y, z + pos.z);
		object.mesh.addVertex(newPos);
		object.vertsBasePos.push_back(newPos);
		object.vertsDeformedPos.push_back(newPos);
		//object.vertMap.push_back(0);

		object.mesh.addTexCoord(ofVec2f(0, 0));

		angle += TWO_PI / steps;

	}
}

ofMesh shape::plane(int widthSegments, int heightSegments, float width, float height) {
	object.resetMesh();
	widthSegments += 1;
	heightSegments += 1;
	object.tris = 0;
	ofVec3f pos(0, 0, 0);
	object.vertsTris.resize(heightSegments*widthSegments);
	object.vertNormals.resize(heightSegments*widthSegments);
	// set points
	for (int y = 0; y < heightSegments; y++) {
		for (int x = 0; x < widthSegments; x++) {
			pos.y = float(y) / heightSegments * height - height / 2 + (height / heightSegments) / 2;
			pos.x = (float(x) / (widthSegments - 1))*(width)-width / 2;// - width / 2 + (width / widthSegments) / 2;

			ofPoint newPos(pos.x, pos.y, pos.z);
			object.mesh.addVertex(newPos);

			object.vertsBasePos.push_back(newPos);
			object.vertsDeformedPos.push_back(newPos);
			//object.vertMap.push_back(0);

			object.mesh.addTexCoord(ofVec2f(0, 0));
		}
	}
	// set tris
	for (int i = 0; i < (heightSegments - 1)*widthSegments; i++) {
		if (!(i != 0 && (i + 1) % widthSegments == 0)) {
			object.makeTri(i, i + widthSegments, i + 1);
			object.makeTri(i + 1, i + widthSegments, i + 1 + widthSegments);
		}
	}
	//tex.loadScreenData(0, 0, 1, 1);
	object.setNormals(object.mesh);
	object.vertsDeformeFrom = object.vertsBasePos;
	object.vertsBaseNormals = object.mesh.getNormals();
	return object.mesh;
}

ofMesh shape::cylinder(int steps, int heightSegments, float height, float radius, bool sphere, bool cone) {

	object.resetMesh();

	float angle = 0;
	object.tris = 0;
	ofVec3f pos(0, 0, 0);

	//testMesh.setMode( OF_PRIMITIVE_LINES );

	object.vertsTris.resize((heightSegments + 1)*steps);
	object.vertNormals.resize((heightSegments + 1)*steps);

	for (int i = 0; i < heightSegments + 1; i++) {
		float sphereRadius = radius;
		float circlePos = (float(i) / heightSegments) * -height+height / 2;

		if (sphere) {
			sphereRadius = sin(float(i) / heightSegments * PI)*radius;
			circlePos = cos(float(i) / heightSegments * PI)*radius;
		}

		pos.y = -circlePos;
		circle(steps, sphereRadius, pos);
	}

	if (cone) {
		for (int i = 0; i < heightSegments; i++) {
			pos.y = float(i) / heightSegments * height;
			circle(steps, -float(heightSegments - 1 - i) / heightSegments * radius, pos);
		}
	}

	for (int i = 0; i < (heightSegments)*steps; i++) {
		if (i != 0 && (i + 1) % steps == 0) {
			object.makeTri(i + steps, i, i + 1 - steps);
			object.makeTri(i + 1 - steps, i + 1, i + steps);
		}
		else {
			object.makeTri(i + steps, i, i + 1);
			object.makeTri(i + steps, i + 1, i + 1 + steps);
		}
	}
	//tex.loadScreenData(0, 0, 1, 1);
	object.setNormals(object.mesh);
	object.vertsDeformeFrom = object.vertsBasePos;
	object.vertsBaseNormals = object.mesh.getNormals();
	return object.mesh;
}

void shape::show(ofPoint translate, ofVec3f rotate) {
	ofPushMatrix();

	ofTranslate(translate);
	
	object.show();

	ofPopMatrix();
}