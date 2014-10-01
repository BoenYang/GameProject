#ifndef TextureHelper_h__
#define TextureHelper_h__
#include "cocos2d.h"
#include <map>

class TextureHelper{
private:
	std::map<std::string,cocos2d::CCRect> rectMap;
	cocos2d::CCDictionary* rectDir;
	TextureHelper();

public:
	static TextureHelper* shareTextureHelper();
	cocos2d::CCTexture2D* addImageFromPlist(const char* plist);
	cocos2d::CCRect getRectByName(const char* textureName);
};

#endif // TextureHelper_h__