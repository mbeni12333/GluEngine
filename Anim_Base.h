#pragma once
#include <string>

class SpriteSheet;
using Frame = unsigned int;

class Anim_Base{
	friend class SpriteSheet;
public:
	Anim_Base();
	virtual ~Anim_Base();

	// setters getters
	void SetLooping(const bool& l_loop);
	void SetSpriteSheet(SpriteSheet* l_spriteSheet);
	void SetFrame(const unsigned int& l_frame);
	void SetName(const std::string& l_name);
	bool IsInAction();

	void Play();
	void Stop();
	void Pause();
	void Reset();

	virtual void Update(const float& l_dT);

	friend std::stringstream& operator >>(std::stringstream& l_stream, Anim_Base& a){
		a.ReadIn(l_stream);
		return l_stream;
	}


protected:
	virtual void FrameStep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream& l_stream) = 0;

	Frame m_currentFrame;
	Frame m_startFrame;
	Frame m_endFrame;
	Frame m_rowFrame;
	
	int m_frameActionStart;
	int m_frameActionEnd;

	float m_frameTime;
	float m_elapsedTime;
	
	bool m_loop;
	bool m_playing;

	std::string m_name;
	SpriteSheet* m_spriteSheet;

};

