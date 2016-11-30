#pragma once

#include "ofMain.h"
#include "../src/LaserMain.h"

struct PolylinePlus {
    ofPolyline polyline; // assume there will always be a field called 'polyline'
    float r;
    float g;
    float b;
    float lineThickness;
    float someOtherStuff;
};


class ofApp : public ofBaseApp {

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

    void populateExampleVectorOfPolylines();
    void optimizeDrawing(int passes = 4);
    float computeLengthOfDrawing(vector<PolylinePlus> aDrawing);

    vector<PolylinePlus>	theRawDrawing;
    vector<PolylinePlus>	tempDrawing;
    vector<PolylinePlus>	theOptimizedDrawing;

    float drawingLength;
    bool bDoit;
    bool closed = false;
    int passes = 4;

    // Conversion functions between Polyline Plus vectors and the Routes used in bryce_tsp.
    void convert_polyline_plus_to_route(vector<PolylinePlus> * path_list, bryce_tsp::Route * route);
    void convert_route_to_polyline_plus(vector<PolylinePlus>    * path_in,
                                        bryce_tsp::Route        * route_in,
                                        bryce_tsp::LaserProgram * permuter,
                                        vector<PolylinePlus>    * path_out);

    // Copies all of the polyline plus data, except for the ofPolyline data.
    void copy_extra_polyline_plus_data(PolylinePlus & src, PolylinePlus & dest);
};