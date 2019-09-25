#pragma once

#include "ofMain.h"
#include "ofxDlib.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{
private:
  ofVideoGrabber _videoGrabber;
  ofxCvColorImage _img_Color;
  ofxCvGrayscaleImage _img_Gray;
  ofxCvContourFinder _contourFinder;
  ofPixels _videoPixles;
  ofTexture _videoTexture;
  int _camWidth;
  int _camHeight;

  std::vector<dlib::rectangle> _faceRects;
  dlib::frontal_face_detector _faceDetector;
  dlib::shape_predictor _shapePredictor;
  std::vector<dlib::full_object_detection> shapes;

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
};
