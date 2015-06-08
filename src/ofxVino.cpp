#include "ofxVino.h"

//--------------------------------------------------------------
ofxVino::ofxVino() {
#if IS_DEBUG
    ofSetLogLevel(OF_LOG_NOTICE);
#endif
}

//--------------------------------------------------------------
ofxVino::~ofxVino() {}

//--------------------------------------------------------------
ofxVinoUser ofxVino::getVineUser() {
    return vineUser;    
}

//--------------------------------------------------------------
bool ofxVino::authenticate(string email, string password) {
    map<string, string> params;
    params["username"] = email;
    params["password"] = password;

    ofxJSONElement response = sendApiRequest(Poco::Net::HTTPRequest::HTTP_POST, "/users/authenticate", params);
    vineUser.authenticated = response["success"].asBool();
    
    if(vineUser.authenticated) {
        vineUser.userId = response["data"]["userId"].asString();
        vineUser.email = email;
        vineUser.username = response["data"]["username"].asString();
        vineUser.avatarUrl = response["data"]["avatarUrl"].asString();
        vineUser.key = response["data"]["key"].asString();
        vineUser.edition = response["data"]["edition"].asString();
    }

    return vineUser.authenticated;
}

//--------------------------------------------------------------
bool ofxVino::unauthenticate() {
    ofxJSONElement response = sendApiRequest(Poco::Net::HTTPRequest::HTTP_DELETE, "/users/authenticate");
    vineUser.authenticated = !response["success"].asBool();
    
    if(!vineUser.authenticated) {
        vineUser = ofxVinoUser();
    }
    
    return !vineUser.authenticated;
}

//--------------------------------------------------------------
bool ofxVino::signup(string username, string password, string email, bool authenticate = false) {
    map<string, string> params;
    params["username"] = username;
    params["password"] = password;
    params["email"] = email;
    params["authenticate"] = authenticate;
    
    ofxJSONElement response = sendApiRequest(Poco::Net::HTTPRequest::HTTP_POST, "/users", params);
}

//--------------------------------------------------------------
ofxJSONElement ofxVino::getProfile() {
    return sendApiRequest(Poco::Net::HTTPRequest::HTTP_GET, "/users/me");
}

//--------------------------------------------------------------
ofxJSONElement ofxVino::getProfile(string userId) {
    return sendApiRequest(Poco::Net::HTTPRequest::HTTP_GET, "/users/profiles/"+userId);
}

//--------------------------------------------------------------
bool ofxVino::setProfile(map<string, string> params) {
    ofxJSONElement response = sendApiRequest(Poco::Net::HTTPRequest::HTTP_PUT, "/users/"+vineUser.userId, params);

    return response["success"].asBool();
}

//--------------------------------------------------------------
bool ofxVino::setAvatar(ofImage image) {
    string avatarUrl = uploadAvatar(image);
    
    map<string, string> params;
    params["avatarUrl"] = avatarUrl;
    
    return setProfile(params);
}

//--------------------------------------------------------------
bool ofxVino::setPrivate(bool mode) {
    string requestMode;
    
    if(mode) {
        requestMode = Poco::Net::HTTPRequest::HTTP_POST;
    } else {
        requestMode = Poco::Net::HTTPRequest::HTTP_DELETE;
    }
    
    ofxJSONElement response = sendApiRequest(requestMode, "/users/"+vineUser.userId+"/explicit");
    return response["success"].asBool();
}

//--------------------------------------------------------------
ofxJSONElement ofxVino::getFollowers() {
    return sendApiRequest(Poco::Net::HTTPRequest::HTTP_GET, "/users/"+vineUser.userId+"/followers");
}

//--------------------------------------------------------------
ofxJSONElement ofxVino::getFollowers(string userId) {
    return sendApiRequest(Poco::Net::HTTPRequest::HTTP_GET, "/users/"+userId+"/followers");
}

//--------------------------------------------------------------
ofxJSONElement ofxVino::getFollowing() {
    return sendApiRequest(Poco::Net::HTTPRequest::HTTP_GET, "/users/"+vineUser.userId+"/following");
}

//--------------------------------------------------------------
ofxJSONElement ofxVino::getFollowing(string userId) {
    return sendApiRequest(Poco::Net::HTTPRequest::HTTP_GET, "/users/"+userId+"/following");
}

//--------------------------------------------------------------
bool ofxVino::createPost(ofFile video, ofImage thumbnail, string description, string forsquareVenueId = "", string venueName = "", int channelId = 0) {
    string thumbnailUrl = uploadThumbnail(thumbnail);
    string videoUrl = uploadVideo(video);

    ofxVinoPost post;
    post.videoUrl = videoUrl;
    post.thumbnailUrl = thumbnailUrl;
    post.description = description;
    post.forsquareVenueId = forsquareVenueId;
    post.venueName = venueName;
    post.channelId = channelId;
    
    return createPost(post);
}

//--------------------------------------------------------------
//  Channel Ids (http://stackoverflow.com/a/28094010/709769)
//  'comedy'                => 1,
//  'art-and-experimental'  => 2,
//  'nature'                => 5,
//  'family'                => 7,
//  'special-fx'            => 8,
//  'sports'                => 9,
//  'food'                  => 10,
//  'music'                 => 11,
//  'beauty-and-fashion'    => 12,
//  'health-and-fitness'    => 13,
//  'news-and-politics'     => 14,
//  'animals'               => 17
bool ofxVino::createPost(string videoUrl, string thumbnailUrl, string description, string forsquareVenueId = "", string venueName = "", int channelId = 0) {
    ofxVinoPost post;
    post.videoUrl = videoUrl;
    post.thumbnailUrl = thumbnailUrl;
    post.description = description;
    post.forsquareVenueId = forsquareVenueId;
    post.venueName = venueName;
    post.channelId = channelId;
    
    return createPost(post);
}

//--------------------------------------------------------------
bool ofxVino::createPost(ofxVinoPost post) {
    map<string, string> params;
    params["videoUrl"] = post.videoUrl;
    params["thumbnailUrl"] = post.thumbnailUrl;
    params["description"] = post.description;
    params["entities"] = "";
    if(!post.forsquareVenueId.empty()) {
        params["forsquareVenueId"] = post.forsquareVenueId;
    }
    if(!post.venueName.empty()) {
        params["venueName"] = post.venueName;
    }
    if(post.channelId != 0) {
        params["channelId"] = post.channelId;
    }
    
    ofxJSONElement response = sendApiRequest(Poco::Net::HTTPRequest::HTTP_POST, "/posts", params);
    return response["success"].asBool();
}

//--------------------------------------------------------------
ofxJSONElement ofxVino::deletePost(string postId) {
    return sendApiRequest(Poco::Net::HTTPRequest::HTTP_DELETE, "/posts/"+postId);
}

//--------------------------------------------------------------
ofxVinoPost ofxVino::getPost(string postId, bool loadMedia = false) {
    ofxJSONElement response = sendApiRequest(Poco::Net::HTTPRequest::HTTP_GET, "/timelines/posts/"+postId);
    
    ofxVinoPost post;
    post.videoUrl = response["data"]["records"][0]["videoUrl"].asString();
    post.thumbnailUrl = response["data"]["records"][0]["thumbnailUrl"].asString();
    post.description = response["data"]["records"][0]["description"].asString();
    post.forsquareVenueId = response["data"]["records"][0]["forsquareVenueId"].asString();
    post.venueName = response["data"]["records"][0]["venueName"].asString();
    
    if(loadMedia) {
        post.video = loadVideo(post.videoUrl);
        post.thumbnail = loadThumbnail(post.thumbnailUrl);

        post.mediaLoaded = true;
    }
    
    return post;
}

//--------------------------------------------------------------
string ofxVino::uploadAvatar(ofImage image) {
    ofBuffer buffer;
    ofSaveImage(image.getPixelsRef(), buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
    
    map<string, string> additionalHeaders;
    additionalHeaders["Content-Type"] = "image/jpeg";
    
    return sendMediaRequest("/upload/avatars/1.3.1.jpg", additionalHeaders, buffer);
}

//--------------------------------------------------------------
ofImage ofxVino::loadAvatar(string avatarUrl) {
    ofImage avatar;
    avatar.loadImage(avatarUrl);
    
    return avatar;
}

//--------------------------------------------------------------
string ofxVino::uploadThumbnail(ofImage image) {
    ofBuffer buffer;
    ofSaveImage(image.getPixelsRef(), buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
    
    map<string, string> additionalHeaders;
    additionalHeaders["Content-Type"] = "image/jpeg";
    
    return sendMediaRequest("/upload/thumbs/1.3.1.mp4.jpg", additionalHeaders, buffer);
}

//--------------------------------------------------------------
ofImage ofxVino::loadThumbnail(string thumbnailUrl) {
    ofImage thumbnail;
    thumbnail.loadImage(thumbnailUrl);
    
    return thumbnail;
}

//--------------------------------------------------------------
string ofxVino::uploadVideo(ofFile video) {
    ofBuffer buffer = video.readToBuffer();
    
    map<string, string> additionalHeaders;
    additionalHeaders["Content-Type"] = "video/mp4";
    
    return sendMediaRequest("/upload/videos/1.3.1.mp4", additionalHeaders, buffer);
}

//--------------------------------------------------------------
ofVideoPlayer ofxVino::loadVideo(string videoUrl) {
    ofVideoPlayer video;
    video.loadMovie(videoUrl);
    
    return video;
}

//--------------------------------------------------------------
ofFile ofxVino::loadRawVideo(string videoUrl) {
    ofFile video;

    ofHttpResponse response = ofLoadURL(videoUrl);
    //video.writeFroimBuffer(response.data);
    response.data.writeTo(video);
    
    return video;
}


//--------------------------------------------------------------
ofxJSONElement ofxVino::sendApiRequest(const string method, string endpoint) {
    map<string, string> params;
    return sendApiRequest(method, endpoint, params);
}

//--------------------------------------------------------------
ofxJSONElement ofxVino::sendApiRequest(const string method, string endpoint, map<string, string> params) {
    try {
        const Poco::URI uri(OFXVINO_API_BASE);
        const Poco::Net::Context::Ptr context(new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));
        
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
        Poco::Net::HTTPRequest request(method, endpoint, Poco::Net::HTTPMessage::HTTP_1_1);

        request.setKeepAlive(true);
        request.set("User-Agent", OFXVINO_USER_AGENT);
		request.set("X-Vine-Client", OFXVINO_CLIENT);
		request.set("Accept-Language", "en;q=1, fr;q=0.9, de;q=0.8, ja;q=0.7, nl;q=0.6, it;q=0.5");
        request.set("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
        if(vineUser.authenticated) {
            request.set("vine-session-id", vineUser.key);
        }
        
#if IS_DEBUG
        request.write(cout);
#endif
        
        Poco::Net::HTMLForm form;
        for(map<string, string>::const_iterator it = params.begin(); it != params.end(); ++it) {
            form.add(it->first, it->second);
        }
        form.prepareSubmit(request);

    
        ostream& os = session.sendRequest(request);
        if(!params.empty()) {
            form.write(os);
        }
        
        Poco::Net::HTTPResponse response;
        istream& is = session.receiveResponse(response);
        
        string responseData;
        Poco::StreamCopier::copyToString(is, responseData);
        
        ofLogVerbose() << "Status: " << response.getStatus() << " " << response.getReason() << endl;
        ofLogVerbose() << responseData << endl;
        
        ofxJSONElement jsonResponseData;
        if(jsonResponseData.parse(responseData)) {
            return jsonResponseData;
        }
    } catch(const std::exception& e) {
        cerr << e.what() << endl;;
    }
}

//--------------------------------------------------------------
string ofxVino::sendMediaRequest(string endpoint, map<string, string> additionalHeaders, ofBuffer mediaContent) {
    try {
        const Poco::URI uri(OFXVINO_MEDIA_BASE);
        const Poco::Net::Context::Ptr context(new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));
        
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_PUT, endpoint, Poco::Net::HTTPMessage::HTTP_1_1);
        
        request.setKeepAlive(true);
        request.set("User-Agent", OFXVINO_USER_AGENT);
        request.set("X-Vine-Client", OFXVINO_CLIENT);
        request.set("Accept-Language", "en;q=1, fr;q=0.9, de;q=0.8, ja;q=0.7, nl;q=0.6, it;q=0.5");
        request.set("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
        request.set("Content-Length", ofToString(mediaContent.size()));
        if(vineUser.authenticated) {
            request.set("vine-session-id", vineUser.key);
        }
        for(map<string, string>::const_iterator it = additionalHeaders.begin(); it != additionalHeaders.end(); ++it) {
            request.set(it->first, it->second);
        }
        
#if IS_DEBUG
        request.write(cout);
#endif
        
        ostream& os = session.sendRequest(request);
        mediaContent.writeTo(os);
        
        Poco::Net::HTTPResponse response;
        istream& is = session.receiveResponse(response);
        
        string responseData;
        Poco::StreamCopier::copyToString(is, responseData);
        
        ofLogVerbose() << "Status: " << response.getStatus() << " " << response.getReason() << endl;
        ofLogVerbose() << responseData << endl;
        ofLogVerbose() << "X-Upload-Key: " << response.get("X-Upload-Key") << endl;

        if(response.getStatus() == 200) {
            return response.get("X-Upload-Key");
        }
        
        return "";
    } catch(const std::exception& e) {
        cerr << e.what() << endl;;
    }
}
