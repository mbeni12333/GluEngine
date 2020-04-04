#include "Anim_Direction.h"
#include "SpriteSheet.h"


void Anim_Direction::FrameStep(){
	if(m_startFrame<m_endFrame){ ++m_currentFrame; }
	else{ --m_currentFrame; }

	if((m_startFrame < m_endFrame && m_currentFrame > m_endFrame)||
	   (m_startFrame > m_endFrame && m_currentFrame < m_endFrame)){
		
		if(m_loop){
			m_currentFrame = m_startFrame;
			return;
		}

		m_currentFrame = m_endFrame;
		Pause();
		m_spriteSheet->setAnimation(m_nextAnimation, true, true);
	}

}

void Anim_Direction::CropSprite(){
	
		sf::IntRect rect(
			m_spriteSheet->GetSpriteSize().x*m_currentFrame,
			m_spriteSheet->GetSpriteSize().y*(m_rowFrame/*+(unsigned int)m_spriteSheet->GetDirection()*/),
			m_spriteSheet->GetSpriteSize().x,
			m_spriteSheet->GetSpriteSize().y);
		m_spriteSheet->CropSprite(rect);
	
}

void Anim_Direction::ReadIn(std::stringstream& l_stream){
	l_stream 
		>> m_startFrame 
		>> m_endFrame
		>>m_rowFrame
		>>m_frameTime
		>> m_frameActionStart
		>> m_frameActionEnd;
		
}
