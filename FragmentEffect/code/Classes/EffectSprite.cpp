#include "EffectSprite.h"

EffectSprite* EffectSprite::create(const char *pszFileName)
{
    EffectSprite *pobSprite = new EffectSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

bool EffectSprite::initWithFile(const char *pszFilename)
{
	if(!CCSprite::initWithFile(pszFilename))
		return false;

	if(!m_effect.setShaderProgram(this))
		return false;

	return true;
}

void EffectSprite::setEffect(int es)
{
	m_effect.setEffect(es);
}

