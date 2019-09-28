#include "FacePainter_Drawing.hpp"

FacePainter_Drawing::FacePainter_Drawing() {
}

void FacePainter_Drawing::init() {
	prepareMouth();
	prepareChin();
	prepareNose();
	prepareEyebrows();
	prepareEyes();
}

void FacePainter_Drawing::paint() {
  ofFill();

  paintChin();
  paintEyebrows();
  paintNose();
  paintEyes();
  paintMouth();
}

 void FacePainter_Drawing::prepareChin() {
     std::tuple<int, int> range = _faceDescriptor->getRangeForRegion(Region::CHIN);
     int start = std::get<0>(range);
     int end = std::get<1>(range);
     auto markers = _faceDescriptor->getMarkers();
     glm::vec2 pos = markers[start];
     _paintstrokeChin.addPoint(glm::vec2(pos.x, pos.y));
     for (int i = start; i < end; i++) {
       pos = markers[i];
       _paintstrokeChin.addPoint(glm::vec2(pos.x, pos.y));
     }
     pos = markers[end];
     _paintstrokeChin.addPoint(glm::vec2(pos.x, pos.y));
 }

 void FacePainter_Drawing::prepareEyebrows() {
    // EYEBROW L
     std::tuple<int, int> range = _faceDescriptor->getRangeForRegion(Region::EYEBROW_L);
     int start = std::get<0>(range);
     int end = std::get<1>(range);
     auto markers = _faceDescriptor->getMarkers();
     glm::vec2 pos = markers[start];
     _paintstrokeEyebrowL.addPoint(glm::vec2(pos.x, pos.y));
     for (int i = start; i < end; i++) {
       glm::vec2 pos = markers[i];
       _paintstrokeEyebrowL.addPoint(glm::vec2(pos.x, pos.y));
     }
     pos = markers[end];
     _paintstrokeEyebrowL.addPoint(glm::vec2(pos.x, pos.y));

     // EYEBROW R
     range = _faceDescriptor->getRangeForRegion(Region::EYEBROW_R);
     start = std::get<0>(range);
     end = std::get<1>(range);
     pos = markers[start];
     _paintstrokeEyebrowR.addPoint(glm::vec2(pos.x, pos.y));
     for (int i = start; i < end; i++) {
       glm::vec2 pos = markers[i];
       _paintstrokeEyebrowR.addPoint(glm::vec2(pos.x, pos.y));
     }
     pos = markers[end];
     _paintstrokeEyebrowR.addPoint(glm::vec2(pos.x, pos.y));
 }

 void FacePainter_Drawing::prepareEyes() {
     // EYE: paramaters
     int eyeStrokeCount = 500;
     int eyeStrokeOffset = 50;
     // EYE LEFT
     std::tuple<int, int> range = _faceDescriptor->getRangeForRegion(Region::EYE_L);
     int start = std::get<0>(range);
     int end = std::get<1>(range);
     auto markers = _faceDescriptor->getMarkers();
     glm::vec2 pos = markers[start];
     int xSum = 0;
     int ySum = 0;
     _paintstrokeEyeL.addPoint(pos);
     for (int i = start; i < end; i++) {
       glm::vec2 pos = markers[i];
       xSum += pos.x;
       ySum += pos.y;
		_paintstrokeEyeL.addPoint(pos);
     }
     _eyeStrokesCenterL = glm::vec2(xSum / 6, ySum / 6);
     pos = markers[end];
	_paintstrokeEyeL.addPoint(pos);

     // EYE RIGHT
     range = _faceDescriptor->getRangeForRegion(Region::EYE_R);
     start = std::get<0>(range);
     end = std::get<1>(range);
     pos = markers[start];
     xSum = 0;
     ySum = 0;
	 _paintstrokeEyeR.addPoint(pos);
     for (int i = start; i < end; i++) {
       glm::vec2 pos = markers[i];
       xSum += pos.x;
       ySum += pos.y;
	   _paintstrokeEyeR.addPoint(pos);
     }
     _eyeStrokesCenterR = glm::vec2(xSum / 6, ySum / 6);
     pos = markers[end];
	 _paintstrokeEyeR.addPoint(pos);
 }

 void FacePainter_Drawing::prepareNose() {
    // NOSE VERTICAL
	auto markers = _faceDescriptor->getMarkers();
     int noseStrokeCount = 42;
     glm::vec2 faceL = markers[0];
     glm::vec2 faceR = markers[16];
     int faceWidth = glm::abs(faceR.x - faceL.x);
     int noseStrokeOffset = faceWidth / 4;
     std::tuple<int, int> range = _faceDescriptor->getRangeForRegion(Region::NOSE_V);
     int start = std::get<0>(range);
     int end = std::get<1>(range);
     glm::vec2 pos = markers[start];
     _paintstrokeNoseV.addPoint(pos);
     for (int i = start; i < end; i++) {
       glm::vec2 pos = markers[i];
       _paintstrokeNoseV.addPoint(pos);
     }
     pos = markers[end];
     _paintstrokeNoseV.addPoint(pos);
     glm::vec2 p1 = markers[39];
     glm::vec2 p2 = markers[42];
     glm::vec2 p3 = markers[33];
     int xSumNoseCenter = p1.x + p2.x + p3.x;
     int ySumNoseCenter = p1.y + p2.y + p3.y;
     _noseStrokeCenter = glm::vec2(xSumNoseCenter / 3, ySumNoseCenter / 3);

     // NOSE HORIZONTAL
     range = _faceDescriptor->getRangeForRegion(Region::NOSE_H);
     start = std::get<0>(range);
     end = std::get<1>(range);
     pos = markers[start];
     _paintstrokeNoseH.addPoint(pos);
     for (int i = start; i < end; i++) {
       glm::vec2 pos = markers[i];
       _paintstrokeNoseH.addPoint(pos);
     }
     pos = markers[end];
     _paintstrokeNoseH.addPoint(pos);
 }

 void FacePainter_Drawing::prepareMouth() {
   std::tuple<int, int> range = _faceDescriptor->getRangeForRegion(Region::MOUTH);
   int start = std::get<0>(range);
   int end = std::get<1>(range);
   auto markers = _faceDescriptor->getMarkers();
   glm::vec2 pos = markers[start];
   _paintstrokeMouth.addPoint(glm::vec2(pos.x, pos.y));
   for (int i = start; i < end; i++) {
     glm::vec2 pos = markers[i];
     _paintstrokeMouth.addPoint(glm::vec2(pos.x, pos.y));
   }
   pos = markers[end];
   _paintstrokeMouth.addPoint(glm::vec2(pos.x, pos.y));
 }

 void FacePainter_Drawing::paintMouth() {
	 _paintstrokeMouth.draw(3);
 }

 void FacePainter_Drawing::paintEyes() {
	 _paintstrokeEyeL.draw(3);
	 _paintstrokeEyeR.draw(3);
 }
 
 void FacePainter_Drawing::paintNose() {
  _paintstrokeNoseH.draw(3);
  _paintstrokeNoseV.draw(3);
 }
 
 void FacePainter_Drawing::paintEyebrows() {
	 _paintstrokeEyebrowL.draw(3);
	 _paintstrokeEyebrowR.draw(3);
 }
 
 void FacePainter_Drawing::paintChin() {
	 _paintstrokeChin.draw(3);
 }
