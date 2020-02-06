#pragma once

#include "Entity.h"

class IMoveHelper
{
public:
	virtual	void _vpad() = 0;
	virtual void SetHost(entity* host) = 0;
};