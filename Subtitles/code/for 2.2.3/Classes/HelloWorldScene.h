#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include"cocos-ext.h"  
#include "Subtitles.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();


	//这里需要更新 m_subtitles
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

	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    CREATE_FUNC(HelloWorld);

private:
	Subtitles m_subtitles;
	cocos2d::CCLabelTTF* m_name;
	cocos2d::CCLabelTTF* m_say;

	///从json文件中加载对话,放到 m_chatDataList 中
	bool readTextFromJson();
};

#endif // __HELLOWORLD_SCENE_H__
