#pragma once

#include "ofMain.h"
#include "../src/LaserMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        laser::LaserProgram * program;
        laser::Route * route;
        laser::Program * commands;

        // Black in grayscale.
        int color = 0;
        float strokeWidth = 2.0;

        // Example drawing functions that use output from the LaserProgram Optimizer.
        void ofApp::drawRoute();
        void ofApp::drawCommandList();
        void ofApp::optimizeRandomRoute(int width, int height);

        // Testing.
        int num_random_tests = 20; // How many optimization tests do you want to do using randomly generated routes?
        int seed = 0; // Which random seed do you want to use in your tests? Tests dependant on screen resolution...
        int num_paths = 30; // The number of paths you want to put in each of the random routes.
        int passes = 5; // Optimize through 5 swap passes, each of which attempt n choose 2 local swap procedures.
};
