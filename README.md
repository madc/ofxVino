# ofxVino

ofxVino is a simple wrapper for connecting your openFrameworks app to the Vine api. The endpoints are documented at the [VineApi repository](https://github.com/VineAPI/VineAPI).

It has been tested on:
- Mac OS X 10.9 with oF 0.8 and Xcode 6.1
- Raspberry Pi B+ with oF 0.8 and Raspbian 

## Usage

Not all endpoints from the API are represented in this addon yet, but some will eventually be added in the near future. Feel free to create a PR, if you added a new endpoint or want to see you favorite endpoint in this addon.
Meanwhile, you can call all endpoints directly via the `sendApiRequest` method

    // Get trending timeline directly
    ofxJSONElement response = sendApiRequest(Poco::Net::HTTPRequest::HTTP_GET, "/timelines/popular");
	
See [example-basic/](example-basic/) and [example_videoPlayer/](example_videoPlayer/) for further details and information.

## Dependencies

- [ofxJSON](https://github.com/jefftimesten/ofxJSON) by Jeff Crouse
- [openFrameworks 0.8](http://openframeworks.cc)
- Some computer

## Acknowledgment

The [circle packed openFrameworks logo](https://www.flickr.com/photos/95943853@N00/5616680866/) is property of [Nick Hardeman](https://github.com/NickHardeman).

## License
ofxVino is available under the [MIT License](https://en.wikipedia.org/wiki/Mit_license).