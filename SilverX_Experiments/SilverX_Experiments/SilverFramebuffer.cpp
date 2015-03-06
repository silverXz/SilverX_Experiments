#include "stdafx.h"
#include "SilverFramebuffer.h"



SilverFramebuffer::SilverFramebuffer() : m_hHandle(0)
{

}

SilverFramebuffer::~SilverFramebuffer()
{
	if(m_hHandle > 0)
		glDeleteFramebuffers(1,&m_hHandle);
}
