#pragma once

#include "Painter.h"
#include "ofxPoissonDiskSampling.h"
#include "BackgroundPainter.hpp"

class MeshPainter : public Painter
{
private:
	ofMesh _mesh;
	ofImage _image_Color;
	BackgroundPainter _backgroundPainter;
	
	float _perlinFactor;
	float _density;
	std::vector<ofVec2f> _samples;

public:
	void init();
	void paint();

	inline void setColorImage(ofImage img) { _image_Color = img;  }
};

