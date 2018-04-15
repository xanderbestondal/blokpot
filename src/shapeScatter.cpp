#include "shapeScatter.h"



void shapeScatter::setup()
{
	scatterMeshes.resize(scatterSurface.object.polyNormals.size());
	for (int i = 0; i < scatterSurface.object.polyNormals.size(); i++) {
		shape shp = shape();
		shp.setup();
		shp.cylinder(5, 1, 5, 10, false, true);
		scatterMeshes[i] = shp;
	}
}
void shapeScatter::setSurfaceShape(shape surf) {
	scatterSurface = surf;
	setup();
}
void shapeScatter::scatter(bool faces) {

	float len = 10;
	ofPushStyle();
	if (faces) {
		//scatterMeshes.resize(scatterSurface.object.polyNormals.size());

		for (int i = 0; i < scatterSurface.object.polyNormals.size(); i++) {
			ofPoint mid = (scatterSurface.object.mesh.getVertex(scatterSurface.object.mesh.getIndex(i * 3)) + scatterSurface.object.mesh.getVertex(scatterSurface.object.mesh.getIndex(i * 3 + 1)) + scatterSurface.object.mesh.getVertex(scatterSurface.object.mesh.getIndex(i * 3 + 2))) / 3;
			ofSetColor(255, 0, 0);
			ofLine(mid.x, mid.y, mid.z, mid.x + scatterSurface.object.polyNormals[i].x*len, mid.y + scatterSurface.object.polyNormals[i].y*len, mid.z + scatterSurface.object.polyNormals[i].z*len);
			//cout << scatterMeshes[i].object.polyNormals.size() << endl;
			scatterMeshes[i].show(mid);
		}
	}
	else {
		for (int i = 0; i < scatterSurface.object.vertNormals.size(); i++) {
			ofSetColor(255, 255, 255);
			ofLine(scatterSurface.object.mesh.getVertex(i).x,
				scatterSurface.object.mesh.getVertex(i).y,
				scatterSurface.object.mesh.getVertex(i).z,
				scatterSurface.object.mesh.getVertex(i).x + scatterSurface.object.mesh.getNormal(i).x*len,
				scatterSurface.object.mesh.getVertex(i).y + scatterSurface.object.mesh.getNormal(i).y*len,
				scatterSurface.object.mesh.getVertex(i).z + scatterSurface.object.mesh.getNormal(i).z*len);
		}
	}
	ofPopStyle();
}
