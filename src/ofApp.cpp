#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	int frameRate = 60;
	_camWidth = 960;
	_camHeight = 720;

	vector<ofVideoDevice> devices = _videoGrabber.listDevices();
	for (size_t i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}
		else {
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName
				<< " - unavailable ";
		}
	}
	_videoGrabber.setVerbose(true);
	_videoGrabber.setPixelFormat(ofPixelFormat::OF_PIXELS_YUY2);
	_videoGrabber.setDeviceID(0);
	_videoGrabber.setDesiredFrameRate(frameRate);
	_videoGrabber.initGrabber(_camWidth, _camHeight);
	_videoGrabber.videoSettings();

	ofSetVerticalSync(true);

	_img_Color.allocate(_camWidth, _camHeight);
	_img_Gray.allocate(_camWidth, _camHeight);

	int width = _videoGrabber.getWidth();
	int height = _videoGrabber.getHeight();
	std::cout << width << ", " << height << std::endl;

	_faceDetector = dlib::get_frontal_face_detector();
	dlib::deserialize(ofToDataPath("shape_predictor_68_face_landmarks.dat", true)) >> _shapePredictor;
}

//--------------------------------------------------------------
void ofApp::update() {
	_videoGrabber.update();
	if (_videoGrabber.isFrameNew()) {
		shapes.clear();
		_videoPixles = _videoGrabber.getPixels();
		_img_Color.setFromPixels(_videoPixles);
		_img_Gray = _img_Color;
		_faceRects = _faceDetector(_videoPixles);
		for (std::size_t j = 0; j < _faceRects.size(); ++j)
		{
			dlib::full_object_detection shape = _shapePredictor(_videoPixles, _faceRects[j]);
			shapes.push_back(shape);
		}
	
		_img_Gray.threshold(30);
		_contourFinder.findContours(_img_Gray, 5, (_camWidth * _camHeight) / 4, 4, false, true);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofPushMatrix();
	float scale = 1;
	ofTranslate(
		ofGetWidth() / 2 - (_videoGrabber.getWidth() / 2) * scale, 
		ofGetHeight() / 2 - (_videoGrabber.getHeight() / 2) * scale);
	ofScale(scale);
	_videoGrabber.draw(
		0, 
		0, 
		_videoGrabber.getWidth(), 
		_videoGrabber.getHeight());
	ofNoFill();
	ofSetLineWidth(3);
	for (auto faceRect : _faceRects) {
		ofSetColor(ofColor::white);
		ofDrawRectangle(ofxDlib::toOf(faceRect));
	}
	for (auto& shape : shapes)
	{
		ofSetColor(240, 200, 170);
		ofDrawRectangle(ofxDlib::toOf(shape.get_rect()));
		for (std::size_t i = 0; i < shape.num_parts(); ++i) 
		{
			ofDrawCircle(ofxDlib::toOf(shape.part(i)), 1);
		}
	}

	for (int i = 0; i < _contourFinder.nBlobs; i++) {
		ofRectangle r = _contourFinder.blobs.at(i).boundingRect;
		//r.x += 320; r.y += 240;
		ofDrawRectangle(r);
	}

	ofPopMatrix();

	ofDrawBitmapString(std::to_string(_faceRects.size()), 100, 100);
	ofDrawBitmapString(std::to_string(shapes.size()), 100, 150);

	//_img_Gray.draw(200, 0, -200, 200);
	//_contourFinder.draw(320, 0, 320, 240);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
