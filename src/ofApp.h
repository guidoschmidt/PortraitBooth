#pragma once

const bool DEV = false;

#include <dlib/opencv.h>
#include "ofMain.h"
#include "ofxDlib.h"
#include "ofxOpenCv.h"
#include "FaceDescriptor.hpp"
#include "BackgroundPainter.hpp"
#include "FacePainter.hpp"
#include "FacePainter_Drawing.hpp"
#include "painters/PointillismPainter.h"
#include "painters/OilPainter.h"
#include "painters/MeshPainter.h"

class ofApp : public ofBaseApp{
private:
	enum Mode { VideoPreview, CaptureCountdown, Loading, Gallery, ShootPhoto };
	Mode _currentMode;

	ofDirectory _dir_gallery;
	ofImage _img_galleryFront;
	ofImage _img_galleryBack;
	ofParameter<int> _showImageForSeconds = 10;
	float _time_imageGalleryStart = 0;
	int _galleryCurrentIndex = 0;

	int _camWidth;
	int _camHeight;
	float _camScale = 2.7;
	ofVideoGrabber _videoGrabber;
	ofxCvColorImage _img_Color;
	ofxCvGrayscaleImage _img_Gray;
	ofxCvContourFinder _contourFinder;
	ofPixels _videoPixles;
  
	ofFbo _offlineRenderFbo;
	bool _imageSavedToDisk = false;

	std::vector<dlib::rectangle> _faceRects;
	dlib::frontal_face_detector _faceDetector;
	dlib::shape_predictor _shapePredictor;
	std::vector<dlib::full_object_detection> _faceShapes;
	ofParameter<float> _detectorImageScale = 0.5;
	std::vector<FaceDescriptor> _faceDescriptors;
  
	ofShader _previewShader;

	float _time_captureStart;
	float _time_countdown;
	float _time_alphaFlash;
	ofParameter<int> _countdownFrom = 3;

	ofTrueTypeFont _font;
	ofTrueTypeFont _fontFaceIndex;

	BackgroundPainter _backgroundPainter;

public:
  void setup();
  void update();
  void draw();
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  void saveImageToDisk();
  void switchGalleryImage();
  void switchToLatestGalleryImage();
  void drawVideoPreview();
};
