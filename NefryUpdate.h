#ifndef NefryUpdate_h
#define NefryUpdate_h


#include "NefryWebServer.h"
#include "inc/Update/src/Update.h"
#include "Nefry.h"


class Nefry_Update
{
public:
	void
		setupWebLocalUpdate();

};
extern Nefry_Update NefryUpdate;
#endif
