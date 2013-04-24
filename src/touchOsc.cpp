#include "touchOsc.h"


void testApp::parseIpadOSCMessage(){
    ofxOscMessage m;
    ipadReceiver.getNextMessage(&m);
    
    if(!bSenderLive){
        ipadIP = m.getRemoteIp();
        ipadSender.setup(ipadIP, PORT_TO_IPAD);
    }
    
    
    string msg_string="";
    string raw_address;
    
    raw_address = m.getAddress();

    if(raw_address=="/1/connect"){
        int val = m.getArgAsInt32(0);
        cout<<"connect request received: "<<val<<endl;
        if(val==0){
            oscSendInitConfig();
        }
    }else if(raw_address=="/1/runMosaic"){
        bUpdateMosaic = m.getArgAsInt32(0);
    }else if(raw_address=="/1/mosaicSpeed"){
        mosaicProcess.speed = m.getArgAsFloat(0);
        oscSendFormatedFloat("/1/labelSpeed", mosaicProcess.speed, 4);
    }else if(raw_address=="/1/encoderMosaicSpeed"){
        int val = (int) m.getArgAsFloat(0);
        if(val==0){
            mosaicProcess.speed /= 1.005;
        }else if(val==1){
            mosaicProcess.speed *= 1.005;
        }else{
            cout<<"encoderMosaicSpeed sending run data"<<endl;
        }
        oscSendFloat("/1/mosaicSpeed", mosaicProcess.speed);
        oscSendFormatedFloat("/1/labelSpeed", mosaicProcess.speed, 4);

    }else if(raw_address=="/1/zoomOut"){
        mosaicProcess.bZoomOut = m.getArgAsInt32(0);
        string msg = mosaicProcess.bZoomOut ? "Zoom out" : "Zoom in";
        oscSendString("/1/labelZoom", msg);
    }else if(raw_address=="/1/snapImage"){
        int val=m.getArgAsFloat(0);
        if(val==1){
            cout<<"snap an image"<<endl;
            imageSet->addImage(grayImage);
            mosaicProcess.bUseEnd = true;
            imageSet->saveImage(grayImage);
        }
    }else if("/1/useVideo"){
        mosaicProcess.bUseVideo = m.getArgAsInt32(0);
    }else{
        cout<<"not handled: "<<raw_address<<endl;
    }

}

void testApp::oscSendInitConfig(){
    oscSendString("/1/connect/color", "green");
    oscSendInt("/1/runMosaic", bUpdateMosaic);
    oscSendFloat("/1/mosaicSpeed", mosaicProcess.speed);
    oscSendFormatedFloat("/1/labelSpeed", mosaicProcess.speed, 4);
    oscSendInt("/1/zoomOut", mosaicProcess.bZoomOut);
    oscSendInt("/1/useVideo", mosaicProcess.bUseVideo);
    
    string msg = mosaicProcess.bZoomOut ? "Zoom out" : "Zoom in";
    oscSendString("/1/labelZoom", msg);

}

void testApp::oscSendString(const string &address, const string &msg){
    ofxOscMessage m;
    m.setAddress(address);
    m.addStringArg(msg);
    ipadSender.sendMessage(m);
}

void testApp::oscSendFloat(const string &address, float msg){
    ofxOscMessage m;
    m.setAddress(address);
    m.addFloatArg(msg);
    ipadSender.sendMessage(m);
}

void testApp::oscSendFormatedFloat(const string &address, float msg, int precision){
    char st[24], formatst[20];
    sprintf(formatst, "%%.%df", precision);
    sprintf(st, formatst, msg);
    cout<<formatst<<" "<<st<<endl;
    ofxOscMessage m;
    m.setAddress(address);
    m.addStringArg(st);
    ipadSender.sendMessage(m);
}


void testApp::oscSendInt(const string &address, int msg){
    ofxOscMessage m;
    m.setAddress(address);
    m.addIntArg(msg);
    ipadSender.sendMessage(m);
}
