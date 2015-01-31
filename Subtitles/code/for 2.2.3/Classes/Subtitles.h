/****************************************************************************
 for cocos2d-x 2.2.3

 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/
#ifndef _SUBTITLES_H_
#define _SUBTITLES_H_

#include "cocos2d.h"
#include <string>
#include <vector>

struct DialogueData
{
	DialogueData(const std::string& who,
				 const std::string& say, 
				 void* userData):
						_who(who),
						_say(say),
						_userData(userData)
	{
	}

	std::string _who;
	std::string _say;
	void* _userData;
};

///每个字出现都会调用,第一个参数存放_who,第二个参数存放_say,第三个参数存放_userData
typedef void (cocos2d::CCObject::*SEL_WordAppearEvent)(const std::string&,const std::string&,void*);
#define WordAppearEvent_selector(_SELECTOR) (SEL_WordAppearEvent)(&_SELECTOR)

///当一段对话完全显示的时候调用，第一个参数存放该对话的序号,第二个参数存放该段对话的信息
typedef void (cocos2d::CCObject::*SEL_DialogueEndEvent)(int,const DialogueData&);
#define DialogueEnd_selector(_SELECTOR) (SEL_DialogueEndEvent)(&_SELECTOR)

/**
 *  一个字幕类
 *  能够一个字一个字的打印出多段对话
 *  仅支持utf8编码
 */
class Subtitles
{
public:
	Subtitles();

	///每一帧调用,进行计算
	void update(float interval);

	///输入对话列表,传人所有对话
	void setDialogueList(const std::vector<DialogueData>& dialogueList);

	///显示该段对话的所有内容
	void showWholeDialogue();

	///跳到下一段对话
	bool nextDialogue();

	///这段对话是否已经完全显示
	bool isShowWholeDialogue()			 {return m_numByteCopy>=m_dialogueList[m_dialogueIndex]._say.size();}

	///设置字符出现的时间间隔
	void setWordInterval(float interval) {m_wordInterval = interval;}

	///设置开始或停止打印对话
	void setRunning(bool isRunning)      {m_isRunning = isRunning;}

	///是不是最后一段对话
	bool isEndofDialogueList()			 {return m_dialogueIndex>=m_dialogueList.size()-1;}

	///跳到某一段对话
	bool jumpToDialogue(int index);

	///当有新的字符出现的时候的回调函数
	void setWordAppearEvent(cocos2d::CCObject* target, SEL_WordAppearEvent selector)
	{
		m_wordAppearTarget   = target;
		m_wordAppearSelector = selector;
	}

	///当一段对话全部打印出来的时候的回调函数
	void setDialogueEndEvent(cocos2d::CCObject* target, SEL_DialogueEndEvent selector)
	{
		m_dialogueEndTarget   = target;
		m_dialogueEndSelector = selector;
	}

	const DialogueData& getDialogueData(int index)
	{
		return m_dialogueList[index];
	}

	const std::vector<DialogueData>& dialogueDataList()
	{
		return m_dialogueList;
	}

private:
	///对话列表
	std::vector<DialogueData> m_dialogueList;

	bool  m_isRunning;
	float m_timeAccumulate;  ///<时间的累积器,当m_timeAccumulate大于等于m_wordInterval时增加多打印一个字,并清零
	float m_wordInterval;    ///<字幕每一个字出现的间隔
	int   m_dialogueIndex;   ///<当前是第几段字幕
	char* m_dialogue;        ///<当前显示的对话
	int   m_numByteCopy;     ///<复制了多少个字节的字幕(utf8的字符不是固定长度的,所以需要记录)
	
	SEL_WordAppearEvent m_wordAppearSelector;
	cocos2d::CCObject* m_wordAppearTarget;
	
	SEL_DialogueEndEvent m_dialogueEndSelector;
	cocos2d::CCObject* m_dialogueEndTarget;

	/**
	 *  复制src的第一个utf8字符
	 *@return 该字符所占的字节数
	 */
	int copyUtf8Word(char* desc,const char* src);
};

#endif