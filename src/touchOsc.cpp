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
    }else if(raw_address=="/1/stay"){
        mosaicProcess.bStay = m.getArgAsInt32(0);
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
    }else if(raw_address=="/1/useVideo"){
        mosaicProcess.bUseVideo = m.getArgAsInt32(0);
        cout<<"bUseVideo "<<mosaicProcess.bUseVideo<<endl;
    }else if(raw_address=="/1/presetOnly"){
        mosaicProcess.bUsePresetOnly = m.getArgAsInt32(0);
        cout<<"bUsePresetOnly "<<mosaicProcess.bUsePresetOnly<<endl;
    }else if(raw_address=="/1/brightness"){
        mosaicProcess.brightness = m.getArgAsFloat(0);
        oscSendFloat("/1/labelBrightness", mosaicProcess.brightness);
        cout<<"brightness "<<mosaicProcess.brightness<<endl;
    }else if(raw_address=="/1/contrast"){
        mosaicProcess.contrast = m.getArgAsFloat(0);
        oscSendFloat("/1/labelContrast", mosaicProcess.contrast);
        cout<<"contrast "<<mosaicProcess.contrast<<endl;
    }else if(raw_address=="/1/hesitation"){
        mosaicProcess.hesitation = m.getArgAsFloat(0);
        oscSendFloat("/1/labelHesitation", mosaicProcess.hesitation);
        cout<<"contrast "<<mosaicProcess.hesitation<<endl;
// Music player
// Add musicPosition, music msToTime() in testApp.h
    }else if(raw_address=="/1/musicVolume"){
        music.setVolume(m.getArgAsFloat(0));
        oscSendFloat("/1/labelVolume", music.getVolume());
    }else if(raw_address=="/1/musicPlaying"){
        bool playing = m.getArgAsInt32(0);
        if(playing){
            music.play();
            music.setPosition(musicPosition);
        }else{
            musicPosition = music.getPosition();
            music.stop();
        }
    }else if(raw_address=="/1/musicReset"){
        if(m.getArgAsInt32(0)==0){
            musicPosition = 0.f;
            music.setPosition(musicPosition);
            oscSendFloat("/1/musicPosition", 0.f);
            oscSendFloat("/1/labelMusicPositionMs", 0.f);
        }
    }else if(raw_address=="/1/musicPosition"){
        musicPosition = m.getArgAsFloat(0);
        music.setPosition(musicPosition);
        oscSendFloat("/1/MusicPosition", musicPosition);
        oscSendString("/1/labelMusicPositionMs", msToTime(music.getPositionMS()));
        // End Music Player
    }else if(raw_address=="/1/zoom"){
        float zoom = m.getArgAsFloat(0);
        mosaicProcess.display_w = (int) (mosaicProcess.max_display_w*zoom);
        if(mosaicProcess.display_w>1024) mosaicProcess.display_w = 1024;
        if(mosaicProcess.display_w<2) mosaicProcess.display_w = 2;
        mosaicProcess.display_h = mosaicProcess.display_w * 3 / 4;
    }else if(raw_address=="/1/reset"){
        int val=m.getArgAsFloat(0);
        if(val==1){
            cout<<"reset brightness and contrast"<<endl;
            mosaicProcess.brightness = 0;
            mosaicProcess.contrast = 0;
            oscSendFloat("/1/brightness", 0);
            oscSendFloat("/1/contrast", 0);
            oscSendFloat("/1/labelBrightness", 0);
            oscSendFloat("/1/labelContrast", 0);
            mosaicProcess.speed = 0.01;
            oscSendFloat("/1/mosaicSpeed", mosaicProcess.speed);
            oscSendFormatedFloat("/1/labelSpeed", mosaicProcess.speed, 4);
        }
    }else{
        cout<<"not handled: "<<raw_address<<endl;
    }

}

void testApp::oscSendInitConfig(){
    oscSendString("/1/connect/color", "green");
    oscSendInt("/1/runMosaic", bUpdateMosaic);
    oscSendInt("/1/stay", mosaicProcess.bStay);
    oscSendFloat("/1/mosaicSpeed", mosaicProcess.speed);
    oscSendFormatedFloat("/1/labelSpeed", mosaicProcess.speed, 4);
    oscSendInt("/1/zoomOut", mosaicProcess.bZoomOut);
    oscSendInt("/1/useVideo", mosaicProcess.bUseVideo);
    oscSendInt("/1/presetOnly", mosaicProcess.bUsePresetOnly);
    oscSendFloat("/1/brightness", mosaicProcess.brightness);
    oscSendFloat("/1/labelBrightness", mosaicProcess.brightness);
    oscSendFloat("/1/contrast", mosaicProcess.contrast);
    oscSendFloat("/1/labelContrast", mosaicProcess.contrast);
    oscSendFloat("/1/hesitation", mosaicProcess.hesitation);
    oscSendFloat("/1/labelHesitation", mosaicProcess.hesitation);
    
    oscSendFloat("/1/musicVolume", music.getVolume());
    oscSendInt("/1/musicPlaying", music.getIsPlaying());
    oscSendFloat("/1/labelMusicPositionMs", music.getPositionMS());
    oscSendFloat("/1/musicPosition", music.getPosition());
    
    oscSendFloat("/1/zoom", mosaicProcess.display_w*1.0/mosaicProcess.max_display_w);
    
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



string testApp::msToTime(int ms){
    
    int sec = ms/1000;
    std::ostringstream s;
    s<<sec/60<<":"<<sec%60;
    
    return s.str();
}

