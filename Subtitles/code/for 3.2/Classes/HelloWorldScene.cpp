#include "HelloWorldScene.h"

using namespace  rapidjson;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	//创建Label
	
	m_name = Label::create();
	m_name->setAnchorPoint(Vec2::ZERO);
	m_name->setPosition(Vec2(30.0f,200.0f));
	this->addChild(m_name);

	m_say = Label::create();
	m_say->setAnchorPoint(Vec2(0.0f,1.0f));
	m_say->setPosition(Vec2(30.0f,200.0f));
	this->addChild(m_say);
    
	//从json中读取对话,然后传入 m_subtitles 
    if(readTextFromJson())
	{
		//开始运行
		m_subtitles.setRunning(true);

		//每0.1秒出现一个新字符
		m_subtitles.setWordInterval(0.1f);

		//设置新字符出现的回调,在这里设置label的显示字符串
		m_subtitles.setWordAppearEvent(CC_CALLBACK_3(HelloWorld::wordAppear,this));

		//当前对话完全显示完的回调,这个Demo没有用到它
		m_subtitles.setDialogueEndEvent(CC_CALLBACK_2(HelloWorld::dialogueEnd,this));
	}

	//设置触碰触碰事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan     = CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
	listener->onTouchEnded     = CC_CALLBACK_2(HelloWorld::onTouchEnded,this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    return true;
}


bool HelloWorld::readTextFromJson()
{
	Data data = FileUtils::getInstance()->getDataFromFile("text.json");
	if(data.isNull())
		return false;

	char*p = (char*)data.getBytes();
	
	std::string json;

	for(int i = 0 ;i<data.getSize() ; i++)
		json+=*(p+i);

	rapidjson::Document doc;
	doc.Parse<0>(json.c_str());

	std::vector<DialogueData> list;

	if (doc.IsObject()) 
	{
		std::string name = doc["name"].GetString();
		int numDialogue = doc["say"].Size();

		for(int i = 0 ; i<numDialogue ; i++)
			list.push_back(DialogueData(name, doc["say"][(rapidjson::SizeType)i].GetString(),0));
	}
	else
		return false;
	
	//传人对话
	m_subtitles.setDialogueList(list);

	return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void HelloWorld::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
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