/**
Nefry lib

Copyright (c) 2016 wami

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#include "NefrySetting.h"

FUNC_POINTER fsetup=nullptr;
NefrySetting::NefrySetting(){}

NefrySetting::NefrySetting(FUNC_POINTER _Func_setup){
	fsetup = _Func_setup;
}

void NefrySetting::setupSetting()
{
	if (fsetup != nullptr) {
		fsetup();
	}
}
