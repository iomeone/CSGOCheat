#pragma once

#include "Entity.h"
#include "RandomStuff.h"

class IEntityList
{
public:
	virtual void				  UnknownFunction0(void) = 0;
	virtual void				  UnknownFunction1(void) = 0;
	virtual void                  UnknownFunction2(void) = 0;
	virtual entity*				  GetClientEntity(int entNum) = 0;
	virtual entity*				  GetClientEntityFromHandle(HANDLE hEnt) = 0;
	virtual int                   NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int                   GetHighestEntityIndex(void) = 0;
	virtual void                  SetMaxEntities(int maxEnts) = 0;
	virtual int                   GetMaxEntities() = 0;
};