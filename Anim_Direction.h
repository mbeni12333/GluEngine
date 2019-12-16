#pragma once
#include "Anim_Base.h"
#include "Direction.h"

class Anim_Direction:
	public Anim_Base{

protected:
	void FrameStep();
	void CropSprite();
	void ReadIn(std::stringstream& l_stream);
	
};

