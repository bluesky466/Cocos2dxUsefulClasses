#ifndef _TEST_SUBTITLES_H_
#define _TEST_SUBTITLES_H_

#include "cocos2d.h"
#include "TestBase.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Subtitles.h"

class TestSubtitles : public TestBase
{
public:

    bool init();  
	
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

    CREATE_FUNC(TestSubtitles);
    CREATE_SCENE(TestSubtitles);

private:
	Subtitles m_subtitles;
	cocos2d::Label* m_name;
	cocos2d::Label* m_say;

	///从json文件中加载对话,放到 m_chatDataList 中
	bool readTextFromJson();
};

#endif
