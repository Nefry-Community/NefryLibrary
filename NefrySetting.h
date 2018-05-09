#ifndef NefrySetting_h
#define NefrySetting_h
#include "NefrySettingBase.h"
typedef void(*FUNC_POINTER)(void); 
class NefrySetting
{
public:
	NefrySetting();
	NefrySetting(FUNC_POINTER _Func_setting);
	void setupSetting();
private:
 
};
#endif
