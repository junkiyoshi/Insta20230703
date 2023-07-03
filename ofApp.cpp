#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_SUBTRACT);
	ofSetCircleResolution(60);
	ofSetLineWidth(2);

	for (int i = 0; i < 1; i++) {

		this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int index = 0;
	auto radius = 30;
	ofColor color;
	for (auto& noise_seed : this->noise_seed_list) {

		auto deg = ofGetFrameNum() * 6 + index * 120;
		auto next_deg = deg + 3;

		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		auto next = glm::vec2(radius * cos(next_deg * DEG_TO_RAD), radius * sin(next_deg * DEG_TO_RAD));

		auto distance = location - next;
		distance *= 1;

		for (int i = 0; i < 2; i++) {

			auto future = location + distance * 8;
			auto random_deg = ofRandom(360);
			future += glm::vec2(10 * cos(random_deg * DEG_TO_RAD), 10 * sin(random_deg * DEG_TO_RAD));
			auto future_distance = future - location;

			this->location_list.push_back(location);
			this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));

			//color.setHsb(ofRandom(255), 255, 255, ofRandom(100, 150));
			auto hue = (int)(ofGetFrameNum() * 3 + ofRandom(64)) % 255;
			color.setHsb(hue, 255, 255, ofRandom(30, 50));
			this->color_list.push_back(color);
		}

		index++;
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;

		if (glm::distance(glm::vec2(), this->location_list[i]) > 1000) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);
	ofRotateY(180);

	ofFill();
	int index = 0;
	for (auto& location : this->location_list) {

		ofSetColor(this->color_list[index++], 150);

		ofPushMatrix();
		ofTranslate(location);

		auto size = ofMap(glm::length(location), 30, 360, 3, 38);
		ofDrawCircle(glm::vec2(), size);

		ofPopMatrix();
	}

	ofNoFill();
	index = 0;
	for (auto& location : this->location_list) {

		ofSetColor(this->color_list[index++], 255);

		ofPushMatrix();
		ofTranslate(location);

		auto size = ofMap(glm::length(location), 30, 360, 3, 38);
		ofDrawCircle(glm::vec2(), size);

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}