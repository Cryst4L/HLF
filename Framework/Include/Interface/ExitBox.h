#pragma once

#include "GUI.h"

namespace HLF
{
class ExitBox
{
  private:  	
    bool m_cancel;

  public:
	ExitBox();
	void run();
	~ExitBox();
};
}