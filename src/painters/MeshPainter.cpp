#include "MeshPainter.h"

void MeshPainter::init() {
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	_mesh.setMode(OF_PRIMITIVE_LINES);
	_mesh.enableColors();
	_mesh.enableIndices();

	_backgroundPainter.selectBackgroundColors();
	_backgroundPainter.darken();

	float w = _image_Color.getWidth();
	float h = _image_Color.getHeight();
	_density = min<float>(w, h) * 0.004;
	_samples = ofxPoissonDiskSampling::sample2D(w, h, _density, false);

	for (ofVec2f v : _samples) {
		int x = v.x;
		int y = v.y;
		_perlinFactor = ofRandom(0, 100) / 25.0f;
		if (x > 0 && x < _image_Color.getWidth() &&
			y > 0 && y < _image_Color.getHeight()) {
			ofColor c = _image_Color.getColor(x, y);
			float saturation = c.getSaturation();
			float lightness = c.getLightness();
			float hue = c.getHue();
			float distance = glm::distance(glm::vec2(v.x, v.y), _faceDescriptor->getCenter());
			float relativeDistance = 1.0 - (distance / _image_Color.getHeight());
			float relativeDistanceInv = (distance / _image_Color.getHeight());
			relativeDistance = glm::pow(relativeDistance, 2);
			relativeDistanceInv = glm::pow(relativeDistanceInv, 2);
			float perl = glm::perlin(glm::vec2(x / w * _perlinFactor, y / h * _perlinFactor));
			float boostColor = ofRandom(0, 2);
			c.setSaturation(c.getSaturation() * boostColor);
			if ((hue / 255.0f) * (lightness / 255.0f) <= 0.5) {
			   glm::vec3 pos(x, y, lightness / 30.0f);
			   _mesh.addVertex(pos);
			   _mesh.addColor(c);
			}
		}
	}

	  int numVerts = _mesh.getNumVertices();
	  for (int a = 0; a < numVerts; ++a) {
	    ofVec3f verta = _mesh.getVertex(a);
		_perlinFactor = ofRandom(0, 100) / 25.0f;
		float perl = glm::perlin(glm::vec2(verta.x / w * _perlinFactor, verta.y / h * _perlinFactor)) + 1.0;
	    for (int b = a + 1; b < numVerts; ++b) {
	      ofVec3f vertb = _mesh.getVertex(b);
	      float distance = verta.distance(vertb);
	      if (distance * perl <= 4) {
	        _mesh.addIndex(a);
	        _mesh.addIndex(b);
	      }
	    }
	  }
}

void MeshPainter::paint() {
	_backgroundPainter.paint();

	float w = _image_Color.getWidth();
	float h = _image_Color.getHeight();
	for (ofVec2f v : _samples) {
		int x = v.x;
		int y = v.y;
		_perlinFactor = ofRandom(0, 100) / 25.0f;
		if (x > 0 && x < _image_Color.getWidth() &&
			y > 0 && y < _image_Color.getHeight()) {
			ofColor c = _image_Color.getColor(x, y);
			float saturation = c.getSaturation();
			float lightness = c.getLightness();
			c.setSaturation(lightness);
			glm::vec2 center = _faceDescriptor->getCenter();
			float distance = glm::distance(glm::vec2(v.x, v.y), _faceDescriptor->getCenter());
			float relativeDistance = 1.0 - (distance / _image_Color.getHeight());
			float relativeDistanceInv = (distance / _image_Color.getHeight());
			relativeDistance = glm::pow(relativeDistance, 2);
			relativeDistanceInv = glm::pow(relativeDistanceInv, 2);
			c.setSaturation(saturation * 1.75);
			float perl = glm::perlin(glm::vec2((center.x + x) / w * _perlinFactor, 
											   (center.y + y) / h * _perlinFactor));
			ofSetColor(c);
			int r = ofRandom(0, 5);
			float simplex = glm::simplex(glm::vec2(center.x / w, center.y / h));
			ofDrawCircle(x, y, 1 + relativeDistance * 3 * perl * (r * simplex));
		}
	}

	ofSetLineWidth(2);
	_mesh.draw();
}
