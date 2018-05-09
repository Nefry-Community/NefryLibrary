#ifndef NefryDataStore_h
#define NefryDataStore_h

#include "./inc/Preferences/src/Preferences.h"
#include "Nefry.h"
/*
ModuleID	: Nefry���W���[���� & Nefry�����M����SSID
ModulePass	: Nefry�����M����WiFi�p�X���[�h
ModuleClass	: Nefry�̃��W���[����������N���X
user		: Nefry�Ń��O�C���@�\���g���Ƃ��Ɏg�p���郆�[�U��
userPass	: Nefry�Ń��O�C���@�\���g���Ƃ��Ɏg�p����p�X���[�h
StorageValue: Nefry�̊��ϐ���ۑ�����Ƃ��Ɏg�p����B(0-7)�͈̔͂ŕۑ�����B���[�ɐ������ǉ������ long�^
StorageStr	: Nefry�̊��ϐ���ۑ�����Ƃ��Ɏg�p����B(0-7)�͈̔͂ŕۑ�����B���[�ɐ������ǉ������ String�^
ConnectSSID	: Nefry���ڑ�����WiFi��SSID��ۑ�����Ƃ��Ɏg�p����B5�܂ŕۑ��B���[�ɐ������ǉ������ String�^
ConnectPass	: Nefry���ڑ�����WiFi�̃p�X���[�h��ۑ�����Ƃ��Ɏg�p����B5�܂ŕۑ��B���[�ɐ������ǉ������ String�^
ConnectWiFi : �ڑ�����WiFi��L��or���������� 
bootSelector: BootMode�����߂�
--------------
BootMode
1 : WriteMode�ؑւ�����
*/

class NefryDataStore_lib
{
public:
	void begin();
	//void clear();
		
	bool
		setModuleID(String id),
		setModulePass(String pass),
		setModuleClass(String className),
		setUser(String user),
		setUserPass(String pass),
		setStorageValue(long value, int pointer),
		setStorageStr(String str, int pointer),
		setConnectSSID(String str, int pointer),
		setConnectPass(String str, int pointer),
		setBootSelector(int mode);
		
	String
		getModuleID(),
		getModulePass(),
		getModuleClass(),
		getUser(),
		getUserPass(),
		getStorageStr(int pointer),
		getConnectSSID(int pointer),
		getConnectPass(int pointer);

	int
		getBootSelector();

	long
		getStorageValue(int pointer);
private:
	bool _readCacheFlg[31];
	String _readCacheStr[20];
	long _readCacheLong[11];
	Preferences nefryDataStorePreferences;
};
extern NefryDataStore_lib NefryDataStore;
#endif
