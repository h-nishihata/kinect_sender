#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    sender.setup(HOST, PORT);
    sender_2.setup(HOST_2, PORT_2);
    kinect.init();
    kinect.open();
    
    colorImg.allocate(kinect.width,kinect.height);
	grayImage.allocate(kinect.width,kinect.height);
	grayBg.allocate(kinect.width,kinect.height);
	grayDiff.allocate(kinect.width,kinect.height);
    
	bLearnBakground = true;
	threshold = 5;

    ofSetFrameRate(1);
    
}

//--------------------------------------------------------------
void testApp::update(){
    
	ofBackground(100,100,100);
    bool bNewFrame = false;
    
    kinect.update();
    bNewFrame = kinect.isFrameNew();
    
	if (bNewFrame){
        
        colorImg.setFromPixels(kinect.getPixels(), kinect.width,kinect.height);
        
        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;
			bLearnBakground = false;
		}
        
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
        
		contourFinder.findContours(grayDiff, 20, (kinect.width*kinect.height)/3, 10, true);
	}
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
	ofFill();
	ofSetHexColor(0x333333);
	ofRect(0,0,kinect.width,kinect.height);
	ofSetHexColor(0xffffff);
    
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(0,0);
        ofSetColor(255);
        
        if(contourFinder.blobs[i].hole){
            if (contourFinder.nBlobs>7) {
                    ofDrawBitmapString("hole",
                                       contourFinder.blobs[i].boundingRect.getCenter().x,
                                       contourFinder.blobs[i].boundingRect.getCenter().y);
                    ofxOscMessage m;
                    m.setAddress("/holes");
                    m.addStringArg("found");
                    sender.sendMessage( m );
                    sender_2.sendMessage( m );
            }
        }else{
            ofxOscMessage m;
            m.setAddress("/holes");
            m.addStringArg("notFound");
            sender.sendMessage( m );
            sender_2.sendMessage( m );
        }
        
    }
    
    
    
    if (contourFinder.blobs.size() > 3) {
        
        ofxOscMessage m;
        m.setAddress("/blobs");
        m.addStringArg("found");
        sender.sendMessage( m );
        sender_2.sendMessage( m );
        
    }else{
        
        ofxOscMessage m;
        m.setAddress("/blobs");
        m.addStringArg("notFound");
        sender.sendMessage( m );
        sender_2.sendMessage( m );
        
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' ') {
        
        ofxOscMessage m;
        m.setAddress("/start_mov");
        m.addIntArg(key);
        sender.sendMessage(m);
        sender_2.sendMessage(m);
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}