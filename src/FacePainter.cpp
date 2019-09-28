#include "FacePainter.hpp"

FacePainter::FacePainter() {
}

void FacePainter::init() {
	prepareMouth();
	prepareChin();
	prepareNose();
	prepareEyebrows();
	prepareEyes();
}

void FacePainter::paint() {
  ofFill();

  paintChin();
  paintEyebrows();
  paintNose();
  paintEyes();
  paintMouth();

  _paintstrokeNoseH.draw(12);
  _paintstrokeNoseV.draw(12);
  //ofSetColor(250, 90, 120);
  //_faceDescriptor->drawDebug();
}

 void FacePainter::prepareChin() {
     std::tuple<int, int> range = _faceDescriptor->getRangeForRegion(Region::CHIN);
     int start = std::get<0>(range);
     int end = std::get<1>(range);
     auto markers = _faceDescriptor->getMarkers();
     glm::vec2 pos = markers[start];
     _paintstrokeChin.addPoint(glm::vec2(pos.x, pos.y));
     for (int i = start; i < end; i++) {
       pos = markers[i];
       _paintstrokeChin.addPoint(glm::vec2(pos.x, pos.y));
        int faceStrokeCount = 10;
        int faceStrokeOffset = 20;
        for (int i = start; i < end; i++) {
          glm::vec2 pos = markers[i];
          for (int s = 0; s < faceStrokeCount; s++) {
            int xO = ofRandom(-faceStrokeOffset, faceStrokeOffset);
            int yO = ofRandom(-faceStrokeOffset, faceStrokeOffset);
            float vectorScale = ofRandom(-0.9, 0.9);
            glm::vec2 vector = _faceDescriptor->getCenter() - pos + glm::vec2(xO, yO);
            glm::vec2 adjustedPos = _faceDescriptor->getCenter() + vectorScale * vector;
            ofColor c = _img_Color.getColor(pos.x + xO, pos.y + yO);
            c.setSaturation(c.getSaturation() * 1.2);
            c.setBrightness(c.getBrightness() * 2);
            glm::vec3 sPos = glm::vec3(adjustedPos.x,
                                       adjustedPos.y,
                                       ofRandom(0, 0));
            //_faceStrokes.push_back(std::make_tuple(sPos, c));
          }
        }
     }
     pos = markers[end];
     ofColor chinStrokeColor = _img_Color.getColor(pos.x, pos.y);
     chinStrokeColor.setBrightness(25);
     chinStrokeColor.setSaturation(200);
     _paintstrokeChin.setColor(chinStrokeColor);
     _paintstrokeChin.addPoint(glm::vec2(pos.x, pos.y));
 }

 void FacePainter::paintChin() {
	  for (auto s : _faceStrokes) {
	    Paintstroke stroke = std::get<0>(s);
	    ofColor c = std::get<1>(s);
	    ofSetColor(c);
	    stroke.draw(60);
	  }
	 _paintstrokeChin.draw(20);
 }

 void FacePainter::prepareEyebrows() {
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
     ofColor eyebrowLStrokeColor = _img_Color.getColor(pos.x, pos.y);
     eyebrowLStrokeColor.setBrightness(25);
     eyebrowLStrokeColor.setSaturation(200);
     _paintstrokeEyebrowL.setColor(eyebrowLStrokeColor);
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
     ofColor eyebrowRStrokeColor = _img_Color.getColor(pos.x, pos.y);
     eyebrowRStrokeColor.setBrightness(25);
     eyebrowRStrokeColor.setSaturation(200);
     _paintstrokeEyebrowR.setColor(eyebrowRStrokeColor);
     _paintstrokeEyebrowR.addPoint(glm::vec2(pos.x, pos.y));
 }

 void FacePainter::paintEyebrows() {
	 _paintstrokeEyebrowL.draw(8);
	 _paintstrokeEyebrowR.draw(8);
 }

 void FacePainter::prepareEyes() {
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
     for (int i = start; i < end; i++) {
       glm::vec2 pos = markers[i];
       xSum += pos.x;
       ySum += pos.y;
       for (int s = 0; s < eyeStrokeCount; s++) {
         int xO = ofRandom(-eyeStrokeOffset, eyeStrokeOffset);
         int yO = ofRandom(-eyeStrokeOffset, eyeStrokeOffset);
         ofColor c = _img_Color.getColor(pos.x + xO, pos.y + yO);
         c.setBrightness(c.getBrightness());
         glm::vec3 sPos = glm::vec3(pos.x + xO, pos.y + yO, c.getSaturation());
         _eyeStrokesL.push_back(std::make_tuple(sPos, c));
       }
     }
     _eyeStrokesCenterL = glm::vec2(xSum / 6, ySum / 6);
     pos = markers[end];

     // EYE RIGHT
     range = _faceDescriptor->getRangeForRegion(Region::EYE_R);
     start = std::get<0>(range);
     end = std::get<1>(range);
     pos = markers[start];
     xSum = 0;
     ySum = 0;
     for (int i = start; i < end; i++) {
       glm::vec2 pos = markers[i];
       xSum += pos.x;
       ySum += pos.y;
       for (int s = 0; s < eyeStrokeCount; s++) {
         int xO = ofRandom(-eyeStrokeOffset, eyeStrokeOffset);
         int yO = ofRandom(-eyeStrokeOffset, eyeStrokeOffset);
         ofColor c = _img_Color.getColor(pos.x + xO, pos.y + yO);
         c.setBrightness(c.getBrightness());
         glm::vec3 sPos = glm::vec3(pos.x + xO, pos.y + yO, c.getSaturation());
         _eyeStrokesR.push_back(std::make_tuple(sPos, c));
       }
     }
     _eyeStrokesCenterR = glm::vec2(xSum / 6, ySum / 6);
     pos = markers[end];
 }

 void FacePainter::paintEyes() {
	 paintEyeArea(_eyeStrokesL, _eyeStrokesCenterL);
	 paintEyeArea(_eyeStrokesR, _eyeStrokesCenterR);
 }

 void FacePainter::prepareNose() {
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
       for (int s = 0; s < noseStrokeCount; s++) {
         int xO = ofRandom(-noseStrokeOffset, noseStrokeOffset);
         int yO = ofRandom(-noseStrokeOffset, noseStrokeOffset);
         ofColor c = _img_Color.getColor(pos.x + xO, pos.y + yO);
         c.setBrightness(c.getBrightness());
         glm::vec3 sPos = glm::vec3(pos.x + xO, pos.y + yO, c.getSaturation());
         _noseStrokes.push_back(std::make_tuple(sPos, c));
       }
     }
     pos = markers[end];
     ofColor noseColorV = _img_Color.getColor(pos.x, pos.y);
     noseColorV.setBrightness(230);
     noseColorV.setSaturation(200);
     _paintstrokeNoseH.setColor(noseColorV);
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
     ofColor noseColorH = _img_Color.getColor(pos.x, pos.y);
     noseColorH.setBrightness(50);
     noseColorH.setSaturation(200);
     _paintstrokeNoseH.setColor(noseColorH);
     _paintstrokeNoseH.addPoint(pos);
 }

 void FacePainter::paintNose() {
	    for (auto s : _noseStrokes) {
	      glm::vec3 pos = std::get<0>(s);
	      ofColor c = std::get<1>(s);
	      c.setBrightness(c.getBrightness());
	      c.setSaturation(c.getSaturation() * 1.05);
	      ofSetColor(c);

	      int r = 5 + (pos.z / 255) * 10;

	      glm::vec2 position = pos;
	      glm::vec2 vector = (_noseStrokeCenter) - position;
	      float angle = glm::atan(vector.x, vector.y);

	      ofPushMatrix();
	      ofTranslate(glm::vec2(pos.x, pos.y));
	      ofRotateDeg(glm::degrees(angle));
	      ofDrawRectangle(glm::vec2(0, 0), r, r * 3);
	      ofPopMatrix();
	    }
 }

 void FacePainter::prepareMouth() {
   std::tuple<int, int> range = _faceDescriptor->getRangeForRegion(Region::MOUTH);
   int start = std::get<0>(range);
   int end = std::get<1>(range);
   auto markers = _faceDescriptor->getMarkers();
   glm::vec2 pos = markers[start];
   _paintstrokeMouth.addPoint(glm::vec2(pos.x, pos.y));
   for (int i = start; i < end; i++) {
     glm::vec2 pos = markers[i];
     _paintstrokeMouth.addPoint(glm::vec2(pos.x, pos.y));
     
	 //for (int s = 0; s < 100; s++) {
  //     int xO = ofRandom(-10, 10);
  //     int yO = ofRandom(-10, 10);
  //     ofColor c = _img_Color.getColor(pos.x + xO, pos.y + yO);
  //     c.setBrightness(c.getBrightness());
  //     glm::vec3 sPos = glm::vec3(pos.x + xO, pos.y + yO, c.getSaturation());
  //     _mouthStrokes.push_back(std::make_tuple(sPos, c));
  //   }
   }
   pos = markers[end];
   ofColor mouthColor = _img_Color.getColor(pos.x, pos.y);
   mouthColor.setBrightness(230);
   mouthColor.setSaturation(200);
   _paintstrokeMouth.setColor(mouthColor);
   _paintstrokeMouth.addPoint(glm::vec2(pos.x, pos.y));
 }

 void FacePainter::paintMouth() {
	 _paintstrokeMouth.draw(12);
 }


void FacePainter::paintEyeArea(std::vector<std::tuple<glm::vec3, ofColor>> strokes, glm::vec2 center) {
	   // Paint coarse eye structures
	   for (auto s : strokes) {
	     glm::vec3 pos = std::get<0>(s);

	     ofColor c = std::get<1>(s);
		 ofColor bw_a = 200; // _img_BW_a.getColor(pos.x, pos.y); TODO
	     float darkenFactorA = bw_a.getBrightness() / 5;
		 ofColor bw_b = 100; //  _img_BW_b.getColor(pos.x, pos.y); TODO
	     float darkenFactorB = bw_b.getBrightness() / 10;
	     float saturation = c.getSaturation();
	     c.setSaturation(saturation * 1.1);
	     c.setBrightness(c.getBrightness() + darkenFactorA + darkenFactorB);

	     int r = 10 + (c.getBrightness() / 255) * 20;

	     glm::vec2 position = glm::vec2(pos.x, pos.y);
	     glm::vec2 vector = (center + glm::vec2(pos.z, 0)) - position;
	     float angle = glm::atan(vector.y, vector.x);
	     float distance = glm::distance(center, position) / 50.0;

	     ofPushMatrix();
	     ofSetColor(c);
	     ofTranslate(glm::vec2(pos.x, pos.y));
	     ofRotateDeg(glm::degrees(angle));
	     ofDrawRectangle(glm::vec2(0, 0), r, 3 + 10 * distance);
	     ofPopMatrix();
	   }

	   // Paint fine eye details
	   for (auto s : strokes) {
	     glm::vec3 pos = std::get<0>(s);

	     ofColor c = std::get<1>(s);
		 ofColor bw_a = 200; // _img_BW_a.getColor(pos.x, pos.y); TODO
	     float darkenFactorA = bw_a.getBrightness() / 8;
		 ofColor bw_b = 100; // _img_BW_b.getColor(pos.x, pos.y); TODO
	     float darkenFactorB = bw_b.getBrightness() / 16;
	     float saturation = c.getSaturation();
	     c.setSaturation(saturation * 1.2);
	     c.setBrightness(c.getBrightness() + darkenFactorA + darkenFactorB);

	     int r = 3 + (c.getBrightness() / 255) * 3;

	     glm::vec2 position = glm::vec2(pos.x, pos.y);
	     glm::vec2 vector = (center + glm::vec2(pos.z, 0)) - position;
	     float angle = glm::atan(vector.y, vector.x);
	     float distance = vector.length();

	     ofPushMatrix();
	     ofSetColor(c);
	     ofTranslate(glm::vec2(pos.x, pos.y));
	     ofRotateDeg(glm::degrees(angle));
	     ofDrawRectangle(glm::vec2(0, 0), r, 5);
	     ofSetColor(c);
	     ofDrawEllipse(glm::vec2(0, 0), 3, 3);
	     ofPopMatrix();
	   }

}
