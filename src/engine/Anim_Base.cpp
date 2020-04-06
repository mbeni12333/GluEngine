#include "Anim_Base.h"
#include "SpriteSheet.h"


Anim_Base::Anim_Base():
	m_currentFrame(0),
	m_startFrame(0),
	m_endFrame(0),
	m_rowFrame(0),
	m_frameTime(0.f),
	m_elapsedTime(0.f),
	m_frameActionStart(-1),
	m_frameActionEnd(-1),
	m_loop(false),
	m_playing(false),
	m_nextAnimation("Idle"){}

Anim_Base::~Anim_Base(){}

void Anim_Base::SetLooping(const bool & l_loop){
	m_loop = l_loop;
}

void Anim_Base::SetSpriteSheet(SpriteSheet * l_spriteSheet){
	m_spriteSheet = l_spriteSheet;
}

void Anim_Base::SetFrame(const unsigned int & l_frame){
	if((l_frame>=m_startFrame&&l_frame<=m_endFrame)||
		(l_frame>=m_endFrame&&l_frame<=m_startFrame)){
		m_currentFrame = l_frame;
	}
}

void Anim_Base::SetFrameTime(const float & l_time){
	m_frameTime = l_time;
}

void Anim_Base::SetName(const std::string & l_name){
	m_name = l_name;
	if(m_nextAnimation=="")m_nextAnimation = l_name;
}

void Anim_Base::SetNext(const std::string & l_name){
	m_nextAnimation = l_name;
}

bool Anim_Base::IsInAction(){
	// -1 means action always performed
	if(m_frameActionStart==-1||m_frameActionEnd==-1){
		return true;
	}

	return (m_currentFrame>=m_frameActionStart&&
			m_currentFrame<=m_frameActionEnd);
}

void Anim_Base::Play(){ m_playing = true; }

void Anim_Base::Stop(){ m_playing = false; Reset(); }

void Anim_Base::Pause(){ m_playing = false; }

void Anim_Base::Reset(){
	m_currentFrame = m_startFrame;
	m_elapsedTime = 0.0f;
	CropSprite();
}

void Anim_Base::Update(const float & l_dT){

	if(!m_playing){ return; }

	m_elapsedTime += l_dT;

	if(m_elapsedTime>=m_frameTime){
		FrameStep();
		CropSprite();
		m_elapsedTime = 0;
	}
}
