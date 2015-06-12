#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // For extended documentation of the Vine API see:
    // https://github.com/VineAPI/VineAPI/blob/master/endpoints.md
    
    // Authenticate with the Vine API
    bool status = bot.authenticate("<your@email.address>", "<your-hexxxor-password>");
    cout << "Authenticated: " << status << endl;
    
    if(status) {
        // Upload new avatar (160x160)
        ofImage avatar;
        avatar.allocate(160, 160, OF_IMAGE_COLOR);
        avatar.loadImage("example_avatar.jpg");
        
        string avatarUrl = bot.uploadAvatar(avatar);
        
        // Update profile details
        map<string, string> params;
        params["avatarUrl"] = avatarUrl;
        params["description"] = "I have two guns, one for each of ya.";
        
        status = bot.setProfile(params);
        cout << "Profile updated: " << status << endl;
        
        // Unauthenticate again...
        status = bot.unauthenticate();
        cout << "Unauthenticated: " << status << endl;
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

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
