#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Subtitles.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
   ///这里需要更新 m_subtitles
    void updateSay(float interval)
	{
		m_subtitles.update(interval);
	}
	
	///有新字符出现的回调
	void wordAppear(const std::string& who, const std::string& say, void* userData)
	{
		m_name->setString(who.c_str());
		m_say->setString(say.c_str());
	}

	///当前对话完全显示完的回调,这个Demo没有用到它
	void dialogueEnd(int,const DialogueData&){}

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

    CREATE_FUNC(HelloWorld);

private:
	Subtitles m_subtitles;
	cocos2d::Label* m_name;
	cocos2d::Label* m_say;

	///从json文件中加载对话,放到 m_chatDataList 中
	bool readTextFromJson();
};

#endif // __HELLOWORLD_SCENE_H__
