#pragma once
#include "ofMain.h"

#include "Poco/Uri.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPResponse.h"

#include "ofxJSON.h"

#define OFXVINO_API_BASE "https://api.vineapp.com"
#define OFXVINO_MEDIA_BASE "https://media.vineapp.com"

#define OFXVINO_USER_AGENT "iphone/3.4.0 (iPhone; iOS 6.0.1; Scale/2.00)"
#define OFXVINO_CLIENT "ios/3.4.0"

struct ofxVinoUser {
    string userId;
    string email;
    string username;
    string avatarUrl;
    string key;
    string edition;
    bool authenticated;
};

struct ofxVinoPost {
    string videoUrl;
    ofVideoPlayer video;
    ofFile rawVideo;
    string thumbnailUrl;
    ofImage thumbnail;
    string description;
    string forsquareVenueId;
    string venueName;
    int channelId;
    bool mediaLoaded;
    // autor?
};

class ofxVino
{
    
    public:
        ofxVino();
        ~ofxVino();
    
        ofxVinoUser getVineUser();
    
        bool authenticate(string email, string password);
        bool unauthenticate();
        bool signup(string username, string password, string email, bool authenticate);
        ofxJSONElement getProfile();
        ofxJSONElement getProfile(string userId);
        bool setProfile(map<string, string> params);
        bool setAvatar(ofImage image);
        bool setPrivate(bool mode);
        ofxJSONElement getFollowers();
        ofxJSONElement getFollowers(string userId);
        ofxJSONElement getFollowing();
        ofxJSONElement getFollowing(string userId);
        bool createPost(ofFile video, ofImage thumbnail, string description, string forsquareVenueId, string venueName, int channelId);
        bool createPost(string videoUrl, string thumbnailUrl, string description, string forsquareVenueId, string venueName, int channelId);
        bool createPost(ofxVinoPost post);
        ofxJSONElement deletePost(string postId);
        ofxVinoPost getPost(string postId, bool loadMedia);

        string uploadAvatar(ofImage image);
        ofImage loadAvatar(string avatarUrl);
        string uploadThumbnail(ofImage image);
        ofImage loadThumbnail(string thumbnailUrl);
        string uploadVideo(ofFile video);
        ofVideoPlayer loadVideo(string videoUrl);
        ofFile loadRawVideo(string videoUrl);
    
        ofxJSONElement sendApiRequest(const string method, string endpoint);
        ofxJSONElement sendApiRequest(const string method, string endpoint, map<string, string> params);
        string sendMediaRequest(string endpoint, map<string, string> additionalHeader, ofBuffer mediaContent);

    private:
        ofxVinoUser vineUser;
};