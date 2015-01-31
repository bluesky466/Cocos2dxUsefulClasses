#include "Subtitles.h"

Subtitles::Subtitles():
	m_wordInterval(0.1f),
	m_dialogue(NULL),
	m_numByteCopy(0),
	m_dialogueIndex(0),
	m_isRunning(false),
	m_wordAppearTarget(NULL),
	m_wordAppearSelector(NULL),
	m_dialogueEndTarget(NULL),
	m_dialogueEndSelector(NULL)
{

}

void Subtitles::setDialogueList(const std::vector<DialogueData>& dialogueList)
{
	m_dialogueList   = dialogueList;
	m_dialogueIndex  = 0;
	m_numByteCopy    = 0;
	m_timeAccumulate = 0.0f;

	if(m_dialogue)
		delete[] m_dialogue;

	if(dialogueList.size()>0)
		m_dialogue = new char[m_dialogueList[m_dialogueIndex]._say.length()+1];
}

void Subtitles::showWholeDialogue()
{
	CCAssert(m_dialogueList.size()>0,"Subtitles::m_dialogueList::size() must >0");

	m_numByteCopy = m_dialogueList[m_dialogueIndex]._say.size()+1;
	memcpy(m_dialogue,m_dialogueList[m_dialogueIndex]._say.c_str(),m_numByteCopy);

	//字出现
	if(m_wordAppearTarget && m_wordAppearSelector)
		(m_wordAppearTarget->*m_wordAppearSelector)(m_dialogueList[m_dialogueIndex]._who, 
													m_dialogue, 
													m_dialogueList[m_dialogueIndex]._userData);

	//这段对话已经显示完毕
	if(m_numByteCopy>=m_dialogueList[m_dialogueIndex]._say.size() && m_dialogueEndTarget && m_dialogueEndSelector)
		(m_dialogueEndTarget->*m_dialogueEndSelector)(m_dialogueIndex,m_dialogueList[m_dialogueIndex]);
}

void Subtitles::update(float interval)
{
	if(!m_isRunning || m_dialogueList.size()==0)
		return;

	m_timeAccumulate += interval;

	if(m_timeAccumulate >= m_wordInterval && m_numByteCopy < m_dialogueList[m_dialogueIndex]._say.size())
	{
		int numByte = copyUtf8Word(m_dialogue + m_numByteCopy,
								   m_dialogueList[m_dialogueIndex]._say.c_str() + m_numByteCopy);

		CCAssert(numByte>0,"copyUtf8Word must return >0");

		m_numByteCopy += numByte;
		m_dialogue[m_numByteCopy] = '\0';

		m_timeAccumulate = 0.0f;
		
		//字出现
		if(m_wordAppearTarget && m_wordAppearSelector)
			(m_wordAppearTarget->*m_wordAppearSelector)(m_dialogueList[m_dialogueIndex]._who, 
														m_dialogue, 
														m_dialogueList[m_dialogueIndex]._userData);

		//这段对话已经显示完毕
		if(m_numByteCopy>=m_dialogueList[m_dialogueIndex]._say.size() && m_dialogueEndTarget && m_dialogueEndSelector)
			(m_dialogueEndTarget->*m_dialogueEndSelector)(m_dialogueIndex,m_dialogueList[m_dialogueIndex]);
	}
}

bool Subtitles::nextDialogue()
{
	if(m_dialogueList.size()!=0 && m_dialogueIndex+1<m_dialogueList.size())
	{
		m_dialogueIndex++;
		m_numByteCopy    = 0;
		m_timeAccumulate = 0.0f;

		if(m_dialogue)
			delete[] m_dialogue;

		m_dialogue = new char[m_dialogueList[m_dialogueIndex]._say.length()+1];

		return true;
	}

	return false;
}

int Subtitles::copyUtf8Word(char* desc,const char* src)
{
	/*
	对于UTF-8编码中的任意字节B，如果B的第一位为0，则B为ASCII码，并且B独立的表示一个字符;
	如果B的第一位为1，第二位为0，则B为一个非ASCII字符（该字符由多个字节表示）中的一个字节，并且不为字符的第一个字节编码;
	如果B的前两位为1，第三位为0，则B为一个非ASCII字符（该字符由多个字节表示）中的第一个字节，并且该字符由两个字节表示;
	如果B的前三位为1，第四位为0，则B为一个非ASCII字符（该字符由多个字节表示）中的第一个字节，并且该字符由三个字节表示;
	如果B的前四位为1，第五位为0，则B为一个非ASCII字符（该字符由多个字节表示）中的第一个字节，并且该字符由四个字节表示;
	*/
	
	char c  = *src;
	int cnt = 0;

	while(true)
	{
		if(0 == (0x80 & c))
		{
			*desc = *src;
			return cnt+1;
		}
		else
		{
			if(0 == (0x40 & c))
			{
				cnt++;
				src++;
				c  = *src;
				continue;
			}

            switch (0xF0&c)
            {
			//该字符由2个字节表示
            case 0xC0:
				desc[0] = src[0];
				desc[1] = src[1];
                return cnt + 2;

			// 该字符由3个字节表示
            case 0xE0:
				desc[0] = src[0];
				desc[1] = src[1];
				desc[2] = src[2];
                return cnt + 3;

			//该字符由4个字节表示
            case 0xF0:
				desc[0] = src[0];
				desc[1] = src[1];
				desc[2] = src[2];
				desc[3] = src[3];
                return cnt + 4;

			default:
				return 0;
            }
		}
	}

	//其实根本来不了这里
	return 0;
}

bool Subtitles::jumpToDialogue(int index)
{
	
	if(m_dialogueList.size()!=0 && index>=0 && index<m_dialogueList.size())
	{
		m_dialogueIndex  = index;
		m_numByteCopy    = 0;
		m_timeAccumulate = 0.0f;

		if(m_dialogue)
			delete[] m_dialogue;

		m_dialogue = new char[m_dialogueList[m_dialogueIndex]._say.length()+1];

		return true;
	}

	return false;
}