#pragma once

#include "ofMain.h"
#include "FaceDescriptor.hpp"

class Painter
{
protected:
	FaceDescriptor* _faceDescriptor;

public:
	Painter();
	
	inline virtual void init() {};
	inline virtual void paint() {};

	inline void setFaceDescriptor(FaceDescriptor* fd) { _faceDescriptor = fd; }
};

