#include "TextureHelper.h"

USING_NS_CC;

static TextureHelper* textureHelper = NULL;

CCTexture2D* TextureHelper::addImageFromPlist(const char* plist)
{
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(plist);
	CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());
	CCDictionary *metadataDict = (CCDictionary*)dict->objectForKey("metadata");
	std::string pngPath = metadataDict->valueForKey("realTextureFileName")->getCString();
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(pngPath.c_str());
	CCDictionary *framesDict = (CCDictionary*)dict->objectForKey("frames");
	int format = 0;
	CCSize textureSize = texture->getContentSize();

	if(metadataDict != NULL) 
	{
		format = metadataDict->valueForKey("format")->intValue();
	}

	CCAssert(format >=0 && format <= 3, "format is not supported for CCSpriteFrameCache addSpriteFramesWithDictionary:textureFilename:");

	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(framesDict, pElement)
	{
		CCRect rect;
		CCDictionary* frameDict = (CCDictionary*)pElement->getObject();
		std::string spriteFrameName = pElement->getStrKey();
	
		if(format == 0) 
		{
			float x = frameDict->valueForKey("x")->floatValue();
			float y = frameDict->valueForKey("y")->floatValue();
			float w = frameDict->valueForKey("width")->floatValue();
			float h = frameDict->valueForKey("height")->floatValue();
			float ox = frameDict->valueForKey("offsetX")->floatValue();
			float oy = frameDict->valueForKey("offsetY")->floatValue();
			int ow = frameDict->valueForKey("originalWidth")->intValue();
			int oh = frameDict->valueForKey("originalHeight")->intValue();
			if(!ow || !oh)
			{
				CCLOGWARN("cocos2d: WARNING: originalWidth/Height not found on the CCSpriteFrame. AnchorPoint won't work as expected. Regenrate the .plist");
			}
			ow = abs(ow);
			oh = abs(oh);
			rect = CCRectMake(x + ox,textureSize.height - y + oy,ow,oh);
		 } 
		 else if(format == 1 || format == 2) 
		 {
			  CCRect frame = CCRectFromString(frameDict->valueForKey("frame")->getCString());
			  frame.origin.y = textureSize.height - frame.origin.y;
			  bool rotated = false;

			  if (format == 2)
			  {
				  rotated = frameDict->valueForKey("rotated")->boolValue();
			  }

			  CCPoint offset = CCPointFromString(frameDict->valueForKey("offset")->getCString());
			  CCSize sourceSize = CCSizeFromString(frameDict->valueForKey("sourceSize")->getCString());
			  rect = frame;
		  } 
		  else if (format == 3)
		  {
			  CCSize spriteSize = CCSizeFromString(frameDict->valueForKey("spriteSize")->getCString());
			  CCPoint spriteOffset = CCPointFromString(frameDict->valueForKey("spriteOffset")->getCString());
			  CCSize spriteSourceSize = CCSizeFromString(frameDict->valueForKey("spriteSourceSize")->getCString());
			  CCRect textureRect = CCRectFromString(frameDict->valueForKey("textureRect")->getCString());
			  bool textureRotated = frameDict->valueForKey("textureRotated")->boolValue();
			  rect = textureRect;
		  }
		 rectMap.insert(std::pair<std::string,CCRect>(spriteFrameName,rect));
	}
	return texture;
}


CCRect TextureHelper::getRectByName( const char* textureName )
{
	return rectMap[std::string(textureName)];
}

TextureHelper::TextureHelper()
{
	
}

TextureHelper* TextureHelper::shareTextureHelper()
{
	if(textureHelper == NULL){
		textureHelper = new TextureHelper();
		return textureHelper;
	}
	return textureHelper;
}

