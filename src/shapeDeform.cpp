#include "shapeDeform.h"

void shapeDeform::setMeshBase(shapeBase shp) {
	meshBase = shp;
	mesh = shp.mesh;

	vertsBasePos = meshBase.vertsBasePos;
	vertsBaseNormals = meshBase.mesh.getNormals();
}

void shapeDeform::setMap(vector<float> map) {
	vertMap = map;
}

ofMesh shapeDeform::solidDeform(int iterations) {
	for (int i = 0; i < mesh.getNumVertices(); i += 3) {
		ofVec3f deformedPos(0, ofRandom(30), 0);
		mesh.setVertex(i, (mesh.getVertex(i) + deformedPos));
		mesh.setVertex(i + 1, (mesh.getVertex(i + 1) + deformedPos));
		mesh.setVertex(i + 2, (mesh.getVertex(i + 2) + deformedPos));
	}
	return mesh;
}
ofMesh shapeDeform::deform(int iterations, float Yfavor) {
	vertsDeformeFrom = vertsBasePos;
	normalsDeformeFrom = vertsBaseNormals;
	for (int iter = 0; iter < iterations; iter++) {
		vertsDeformedPos.resize(0);
		for (int i = 0; i < mesh.getNumVertices(); i++) {
			float yNormal = 1.0;
			if (Yfavor != 0) {
				yNormal = normalsDeformeFrom[i].y * Yfavor;
			}
			ofPoint melt = ofPoint(0, Yfavor*(deformMultiplier / 2), 0)*(vertMap[i]);
			ofVec3f deformedPos((vertMap[i] - .5) *deformMultiplier * normalsDeformeFrom[i] + melt + vertsDeformeFrom[i] + normalsDeformeFrom[i] * deformMultiplier + melt);
			mesh.setVertex(i, deformedPos);
			vertsDeformedPos.push_back(deformedPos);
		}
		vertsDeformeFrom = vertsDeformedPos;
		meshBase.setNormals(mesh);
		normalsDeformeFrom = mesh.getNormals();
	}
	return mesh;
}
void shapeDeform::blenshapeDeform(float strength) {
	if (blendshapeLoaded) {
		for (int i = 0; i < mesh.getNumVertices(); i++) {

			ofVec3f deformedPos(vertsBasePos[i] * (1 - strength) + blendshapeMesh.getVertex(i) * strength);
			mesh.setVertex(i, deformedPos);
		}
		meshBase.setNormals(mesh);
	}

}