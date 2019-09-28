#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	int frameRate = 60;
	_camWidth = 960;
	_camHeight = 720;

	ofSetFrameRate(frameRate);
	ofEnableAlphaBlending();
	ofSetCircleResolution(100);
	ofHideCursor();

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
	_videoGrabber.setPixelFormat(OF_PIXELS_NATIVE);
	_videoGrabber.setDeviceID(0);
	_videoGrabber.setDesiredFrameRate(frameRate);
	_videoGrabber.initGrabber(_camWidth, _camHeight);
	_videoGrabber.setVerbose(true);
	_videoGrabber.videoSettings();

	_img_Color.allocate(_camWidth, _camHeight);
	_img_Gray.allocate(_camWidth, _camHeight);

	int width = _videoGrabber.getWidth();
	int height = _videoGrabber.getHeight();
	std::cout << width << ", " << height << std::endl;

	_faceDetector = dlib::get_frontal_face_detector();
	dlib::deserialize(ofToDataPath("shape_predictor_68_face_landmarks.dat", true)) >> _shapePredictor;

	// Timer
	_time_countdown = 0;

	// Fonts
	ofTrueTypeFont::setGlobalDpi(72);
	_font.load("fonts/consolab.ttf", 120, true, true);
	_fontFaceIndex.load("fonts/consolab.ttf", 20, true, true);

	// Shader
	_previewShader.load("shader/videopreview.vert.glsl",
		"shader/videopreview.frag.glsl");

	// Mode
	_currentMode = Mode::VideoPreview;

	// Gallery
	string galleryPath = ofToDataPath("gallery");
	std::cout << "Gallery path: " << galleryPath << std::endl;
	_dir_gallery.open(galleryPath);
	for (int i = 0; i < _dir_gallery.listDir(); i++) {
		std::cout << _dir_gallery.getPath(i) << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::switchToLatestGalleryImage() {
	// Update gallery info
	string galleryPath = ofToDataPath("gallery");
	std::cout << "Gallery path: " << galleryPath << std::endl;
	_dir_gallery.open(galleryPath);
	// Load latest image
	_galleryCurrentIndex = _dir_gallery.listDir() - 1;
	_time_imageGalleryStart = ofGetElapsedTimeMillis();
	std::string imagePath = _dir_gallery.getPath(_galleryCurrentIndex) + "/render.jpg";
	std::cout << "Presenting image from " << imagePath << std::endl;
	_img_galleryFront.load(imagePath);
	_galleryCurrentIndex = (_galleryCurrentIndex + 1) % _dir_gallery.listDir();
	_currentMode = Mode::Gallery;
}

//--------------------------------------------------------------
void ofApp::switchGalleryImage() {
	_time_imageGalleryStart = ofGetElapsedTimeMillis();
	std::string imagePath = _dir_gallery.getPath(_galleryCurrentIndex) + "/render.jpg";
	std::cout << "Presenting image from " << imagePath << std::endl;
	_img_galleryFront.load(imagePath);
	_galleryCurrentIndex = (_galleryCurrentIndex + 1) % _dir_gallery.listDir();
}

//--------------------------------------------------------------
void ofApp::update() {
	switch (_currentMode) {
	case Mode::VideoPreview:
	case Mode::CaptureCountdown:
	{
		_videoGrabber.update();
		if (_videoGrabber.isFrameNew()) {
			_img_Color.setFromPixels(_videoGrabber.getPixels());
			_img_Color.updateTexture();
			_img_Gray.setFromPixels(_img_Color.getPixels().getChannel(1));
			float scaleBy = 1.0 / _detectorImageScale;
			_img_Gray.resize(_camWidth / scaleBy, _camHeight / scaleBy);
			dlib::cv_image<uchar> img(_img_Gray.getCvImage());
			_faceRects = _faceDetector(img);
		}
		// Countdown
		_time_countdown = ofGetElapsedTimeMillis() - _time_captureStart;
	}
	break;

	case Mode::Gallery:
	{
		if (_dir_gallery.listDir() == 0)
			_currentMode = Mode::VideoPreview;
		// Gallery counter
		float timeDiffGallery = ofGetElapsedTimeMillis() - _time_imageGalleryStart;
		if (timeDiffGallery >= _showImageForSeconds * 1000.0) {
			switchGalleryImage();
		}
	}
	break;
	}
}

//--------------------------------------------------------------
void ofApp::drawVideoPreview() {
		_img_Color.getTextureReference().bind(0);
		_previewShader.begin();
			ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);
			_previewShader.setUniform1f("u_scaleFactor", _camScale);
			_previewShader.setUniformTexture("u_videoTexture", _img_Color.getTexture(), 0);
			ofFill();
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		_previewShader.end();
		_img_Color.getTextureReference().unbind();
		
		ofSetRectMode(ofRectMode::OF_RECTMODE_CORNER);
		ofSetColor(255, 255, 255);
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2 - (_videoGrabber.getWidth() / 2) * _camScale, 
			        ofGetHeight() / 2 - (_videoGrabber.getHeight() / 2) * _camScale);
		ofScale(_camScale);
		ofNoFill();
		ofSetLineWidth(20);
		float scaleBy = 1.0 / _detectorImageScale;
		for (int f = 0; f < _faceRects.size(); f++) {
			auto& faceRect = _faceRects[f];
			auto rect = ofxDlib::toOf(faceRect);
			ofSetColor(ofColor::fromHsb(f * 255/5.0, 200, 200));
			_fontFaceIndex.drawString(std::to_string(f), 
									  rect.getLeft() * scaleBy + 20,
									  rect.getTop() * scaleBy + 20);
			//ofDrawBitmapString(std::to_string(f), 
			//				   rect.getLeft() * scaleBy + 20, 
			//				   rect.getTop() * scaleBy + 20);
			ofDrawRectangle(rect.x * scaleBy, 
							rect.y * scaleBy, 
							rect.width * scaleBy, 
							rect.height * scaleBy);
		}
		ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::draw() {
	// PREVIEW SEQUENCE: preview found faces

	switch (_currentMode) {
	case Mode::Loading:
	{
		ofPushMatrix();
		ofBackground(0);
		ofSetColor(250);
		_fontFaceIndex.drawString("Processing...", 100, ofGetHeight() - 150);
		ofPopMatrix();

		ofSetRectMode(ofRectMode::OF_RECTMODE_CORNER);
		_currentMode = Mode::ShootPhoto;
	}
	break;

	case Mode::Gallery:
	{
		float timeDiffGallery = glm::abs(ofGetElapsedTimeMillis() - _time_imageGalleryStart);
		float normalizedTime = timeDiffGallery / (float)(_showImageForSeconds * 1000);
		int width = ofGetWidth() * normalizedTime;
		ofFill();
		ofSetColor(255, 255, 255, 200);
		_img_galleryFront.draw(0, 0, ofGetWidth(), ofGetHeight());
		ofDrawRectangle(glm::vec2(0, 0), width, 80);
	}
	break;

	case Mode::VideoPreview:
		drawVideoPreview();
		break;

	case Mode::CaptureCountdown:
	{
		drawVideoPreview();
		int countdownDigit = _countdownFrom - (int)glm::floor(_time_countdown / 1000);
		ofSetColor(255);
		if (countdownDigit <= 3 && countdownDigit >= 1) {
			_font.drawString(std::to_string(countdownDigit), ofGetWidth() / 2 - 60, 300);
			_time_alphaFlash = ofGetElapsedTimeMillis();
		}
		if (countdownDigit == 1) {
			ofSetColor(255, 42, 29);
			ofFill();
			ofDrawCircle(ofGetWidth() / 2 - 25, 125, 50);
		}
		if (countdownDigit == 0) {
			ofSetColor(255, 42, 29);
			ofFill();
			ofDrawCircle(ofGetWidth() / 2 - 25, 125, 50);
			ofFill();
			float alphaTime = glm::abs(_time_alphaFlash - ofGetElapsedTimeMillis()) / 100.0;
			std::cout << alphaTime << std::endl;
			if (alphaTime >= 1)
				_currentMode = Mode::Loading;
			float alpha = glm::sin((float)alphaTime * glm::pi<float>()) * 255;
			ofSetColor(255, 255, 255, alpha);
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		}

	}
	break;
	case Mode::ShootPhoto:
	{
		if (_imageSavedToDisk == false) {
			_currentMode = Mode::Loading;
			saveImageToDisk();
		}
	}
	break;
	}
	// Debug drawing
	if (DEV) {
		ofFill();
		ofSetColor(255);
		ofDrawBitmapString(std::to_string(ofGetFrameRate()), 100, 100);
		ofDrawBitmapString(std::to_string(_faceShapes.size()), 100, 150);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	std::cout << "Key released: " << key << std::endl;
	switch (key) {
	case ' ':
		std::cout << "Capture image " << key << std::endl;
		_faceDescriptors.clear();
		_time_captureStart = ofGetElapsedTimeMillis();
		_imageSavedToDisk = false;
		_currentMode = Mode::CaptureCountdown;
		break;
	case 'r':
		_previewShader.load("shader/videopreview.vert.glsl",
							"shader/videopreview.frag.glsl");
		break;
	case 13: // Enter
		_currentMode = Mode::VideoPreview;
		break;
	case 'g':
		_currentMode = Mode::Gallery;
		break;
	}
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

//--------------------------------------------------------------
void ofApp::saveImageToDisk() {
	const std::string TIMESTAMP = ofGetTimestampString();
	float scaleBy = 1.0 / _detectorImageScale;

	//_img_Gray.threshold(30);
	//_contourFinder.findContours(_img_Gray, 5, (_camWidth * _camHeight) / 4, 4, false, true);

	for (std::size_t j = 0; j < _faceRects.size(); ++j) {
		dlib::full_object_detection shape = _shapePredictor(_img_Color.getPixels(), _faceRects[j]);
		FaceDescriptor faceDescriptor;
		for (std::size_t i = 0; i < shape.num_parts(); ++i) {
			glm::vec2 point = ofxDlib::toOf(shape.part(i));
			point *= scaleBy;
			std::cout << point.x << ", " << point.y << std::endl;
			faceDescriptor.addMarkerPoint(point);
		}
		faceDescriptor.calculateCenter();
		_faceDescriptors.push_back(faceDescriptor);
	}
	std::cout << "Face descriptors: " << _faceDescriptors.size() << std::endl;
	
	if (_faceRects.size() == 0 ||
		_faceDescriptors.size() == 0)
	{
		_currentMode = Mode::VideoPreview;
		return;
	}
	
	// Offscreen rendering
	_backgroundPainter.selectBackgroundColors();

	ofPixels outPixels;
	outPixels.allocate(1080, 1920, OF_IMAGE_COLOR);
	outPixels.clear();
	_offlineRenderFbo.allocate(1080, 1920, GL_RGB);
	_offlineRenderFbo.begin();


	ofImage imageColor;
	imageColor.setFromPixels(_img_Color.getPixels());

	ofEnableSmoothing();
	ofEnableAntiAliasing();

	ofTranslate(ofGetWidth() / 2 - (_videoGrabber.getWidth() / 2) * _camScale, 
		        ofGetHeight() / 2 - (_videoGrabber.getHeight() / 2) * _camScale);
	ofScale(_camScale);
	ofFill();
	ofSetColor(255);
	ofClear(128);
	ofPushMatrix();

	int r = ofRandom(0, 3);
	std::cout << "PAINTER MODE: " << r << std::endl;
	switch (r)
	{
	case 0:
	{
		// PAINTER 1:
		PointillismPainter painter;
		painter.setColorImage(imageColor);
		for (auto descriptor : _faceDescriptors) {
			painter.setFaceDescriptor(&descriptor);
			painter.init();
			painter.paint();
		}
	}
	break;
	case 1:
	{
		// PAINTER 2:
		OilPainter painter;
		painter.setColorImage(imageColor);
		for (auto descriptor : _faceDescriptors) {
			painter.setFaceDescriptor(&descriptor);
			painter.init();
			painter.paint();
		}
	}
	break;
	case 2:
	{
		MeshPainter painter;
		painter.setColorImage(imageColor);
		for (auto descriptor : _faceDescriptors) {
			painter.setFaceDescriptor(&descriptor);
			painter.init();
			painter.paint();
		}
	}
	break;
	}

	// OLD PAINTER:
	//FacePainter_Drawing facePainter = FacePainter_Drawing();
	//_backgroundPainter.paint();
	//FacePainter facePainter = FacePainter();
	//ofImage colorImage;
	//colorImage.setFromPixels(_img_Color.getPixels());
	//for (auto descriptor : _faceDescriptors) {
	//	facePainter.setDescriptor(&descriptor);
	//	facePainter.setColorImage(colorImage);
	//	facePainter.init();
	//	facePainter.paint();
	//}
	
	_offlineRenderFbo.end();
	_offlineRenderFbo.readToPixels(outPixels);

	// Save to file
	std::string path =
		"gallery/" +
		TIMESTAMP + "/";
	ofSaveImage(outPixels, path + "render.jpg", OF_IMAGE_QUALITY_BEST);

	ofSaveImage(_img_Color.getPixels(), path + "capture.jpg", OF_IMAGE_QUALITY_BEST);
	_imageSavedToDisk = true;

	_faceShapes.clear();
	switchToLatestGalleryImage();
}
