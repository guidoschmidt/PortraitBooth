#include "ofMain.h"
#include "ofApp.h"

int main( ){
	ofGLFWWindowSettings settings;
	settings.setGLVersion(4, 4);
	settings.setSize(1080, 1920);
	if (DEV)
		settings.windowMode = OF_WINDOW;
	else
		settings.windowMode = OF_FULLSCREEN;
	ofCreateWindow(settings);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
