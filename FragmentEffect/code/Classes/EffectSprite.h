#ifndef __SIMPLE_SPRITE_H__
#define __SIMPLE_SPRITE_H__

#include "FragmentEffect.h"

class EffectSprite : public CCSprite
{
public:
	static EffectSprite* create(const char *pszFileName);

	virtual bool initWithFile(const char *pszFilename);

	void setEffect(int es);
private:
	FragmentEffect m_effect;
};

#endif