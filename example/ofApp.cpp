#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // The screen that I test this on was 1920 by 1080, in case anybody out there wants to know...
    int width  = ofGetWidth();
    int height = ofGetHeight();

    // Minnimal testing code. Interior points in polylines are ommitted,
    // because they are currently not considered in the algorithm.
    
    // We generate a random 10 route, using a random seed for consistent testing.
    ofSeedRandom(seed);

    for (int i = 0; i < num_random_tests; i++)
    {
        optimizeRandomRoute(width, height);
    }

}

void ofApp::optimizeRandomRoute(int width, int height)
{
    laser::Route * route = new laser::Route();

    int path_num = this -> num_paths;

    for (int i = 0; i < path_num; i++)
    {
        laser::Polyline * path = new laser::Polyline();
        for (int i = 0; i < 2; i++)
        {
            float x = ofRandom(width);
            float y = ofRandom(height);
            path -> push_back(ofPoint(x, y));
            
            int offset = 40;

            // Extra Points to test out internal processing.
            path -> push_back(ofPoint(x + offset, y));
            path -> push_back(ofPoint(x + offset, y + offset));
            path -> push_back(ofPoint(x    , y + offset));
        }
        route -> push_back(path);
    }

    program = new laser::LaserProgram(route);
    program -> optimize(this -> passes);
    this -> route = program -> getRoute();

    this -> commands = program -> getCommandList();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    // Draw From Route DataStructure.
    //drawRoute();

    // Draw from a sequence of ON, OFF, and MOVE commands.
    drawCommandList();
}

void ofApp::drawRoute()
{
    // Draw the route each frame.
    ofPath p;// The path.
    p.setStrokeColor(color);
    p.setStrokeWidth(strokeWidth);
    p.setFilled(false);
    

    ofPath p_connectors;// The connection paths.
    p_connectors.setStrokeColor(128);
    p_connectors.setStrokeWidth(strokeWidth / 2);
    p_connectors.setFilled(false);


    // Draws the given paths, while also drawing the connecting routes between them in dual.
    auto iter = route -> cbegin();
    auto first_line_iter = iter;
    auto first_point_iter = (*first_line_iter) -> cbegin();
    p_connectors.moveTo(*first_point_iter);
    for (; iter != route -> cend(); ++iter)
    {
        laser::Polyline * polyline = *iter;

        auto point_iter = polyline -> cbegin();

        // Notice that p always moves while p_connectos draws ("lineTo"), and vise versa.
        // There is beautiful combinatorial cyclic symmetry here.
        p_connectors.lineTo(*point_iter);
        p.moveTo(*point_iter);

        for (; point_iter != polyline -> cend(); ++point_iter)
        {
            p.lineTo(*point_iter);
            p_connectors.moveTo(*point_iter);
        }
    }
    p_connectors.lineTo(*first_point_iter);

    p.draw();
    p_connectors.draw();
}

void ofApp::drawCommandList()
{
    ofPath p;
    p.setFilled(false);

    ofPath p_connectors;
    p_connectors.setStrokeColor(128);
    p_connectors.setStrokeWidth(strokeWidth / 2);

    bool draw = false;
    bool start = false;
    ofPoint point_start;

    for (auto iter = commands -> cbegin(); iter != commands -> cend(); ++iter)
    {
        laser::LaserCommand command = *iter;

        switch (command.type)
        {
            case laser::OFF:

                p.draw();
                // Prepare to draw a new path.
                draw = false;

                p.clear();
                // Use your favorite original path index to color conversion procedure.
                
                p.setStrokeColor((command.ID*23) % 100);
                p.setStrokeWidth(6 + (command.ID * 23) % 5);
                
                continue;

            case laser::ON:
                draw = true;
                continue;

            case laser::MOVETO:

                if (!start)
                {
                    point_start = command.pt;
                    start = true;
                    p_connectors.moveTo(command.pt);
                }

                if (draw)
                {
                    p.lineTo(command.pt);
                    p_connectors.moveTo(command.pt);
                }
                else
                {
                    p.moveTo(command.pt);
                    p_connectors.lineTo(command.pt);
                }

                continue;
        }
    }

    p_connectors.lineTo(point_start);

    p.draw();
    p_connectors.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
