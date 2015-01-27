#include "HelloWorldScene.h"

USING_NS_CC;
USING_NS_CC_EXT;  
using namespace rapidjson;  

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	//创建Label
	m_name = CCLabelTTF::create();
	m_name->setAnchorPoint(ccp(0.0f,0.0f));
	m_name->setPosition(ccp(30.0f,200.0f));
	this->addChild(m_name);

	m_say = CCLabelTTF::create();
	m_say->setAnchorPoint(ccp(0.0f,1.0f));
	m_say->setPosition(ccp(30.0f,200.0f));
	this->addChild(m_say);
    
	//从json中读取对话,然后传入 m_subtitles 
    if(readTextFromJson())
	{
		std::vector<DialogueData> list;
		std::vector<ChatData>::iterator i = m_chatDataList.begin();
		while(i!=m_chatDataList.end())
		{
			list.push_back(DialogueData(i->_say,&i->_name));
			i++;
		}

		//传人对话
		m_subtitles.setDialogueList(list);

		//开始运行
		m_subtitles.setRunning(true);

		//每0.1秒出现一个新字符
		m_subtitles.setWordInterval(0.1f);

		//设置新字符出现的回调,在这里设置label的显示字符串
		m_subtitles.setWordAppearEvent(this,WordAppearEvent_selector(HelloWorld::wordAppear));

		//当前对话完全显示完的回调,这个Demo没有用到它
		m_subtitles.setDialogueEndEvent(this,DialogueEnd_selector(HelloWorld::dialogueEnd));
	}

	this->schedule(schedule_selector(HelloWorld::updateSay));
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);

    return true;
}


bool HelloWorld::readTextFromJson()
{
	unsigned long size;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData("text.json","rt",&size);
	*(data+size)='\0';

	if(!data)
		return false;
	
	std::string json = data;

	rapidjson::Document doc;
	doc.Parse<0>(json.c_str());

	m_chatDataList.clear();
	if (doc.IsObject()) 
	{
		std::string name = doc["name"].GetString();
		int numDialogue = doc["say"].Size();

		for(int i = 0 ; i<numDialogue ; i++)
			m_chatDataList.push_back(ChatData(name, doc["say"][(rapidjson::SizeType)i].GetString()));
	}
	else
		return false;

	return true;
}

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	//如果该段对话已经显示完全
	if(m_subtitles.isShowWholeDialogue())
	{
		//若是最后一段对话,跳回第一段对话
		if(m_subtitles.isEndofDialogueList())
			m_subtitles.jumpToDialogue(0);

		//跳到下一段对话
		else
			m_subtitles.nextDialogue();
	}
	else
	{
		//如果该段对话没有显示完全,则将它完全显示
		m_subtitles.showWholeDialogue();
	}
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}