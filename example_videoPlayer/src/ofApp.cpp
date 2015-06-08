#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Fetching metadata for post with the ID 995407761754161152
    post = bot.getPost("995407761754161152", true);
    
    // Play video in a loop
    post.video.setLoopState(OF_LOOP_NORMAL);
}

//--------------------------------------------------------------
void ofApp::update(){
    post.video.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Play the video, when the mouse is over the oF window
    if(mouseX > 0 && mouseX < 480 && mouseY > 0 && mouseY < 480) {
        post.video.play();
    } else { // otherwise stop.
        post.video.stop();
    }

    post.video.draw(0, 0);
    ofDrawBitmapString(post.description, 20, 460);
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
