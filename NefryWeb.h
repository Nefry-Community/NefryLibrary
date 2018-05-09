#ifndef NefryWeb_h
#define NefryWeb_h


#include "Nefry.h"
#include "NefryDataStore.h"
#include "NefryWebServer.h"
#include "./inc/DNSServer/src/DNSServer.h"

class Nefry_Web
{
public:
	void
		begin(),
		run(),
		setIndexLink(const char title[32], const char url[32]);

	String createHtml(String title, String head, String body);
private:
	DNSServer _dnsServer;
	void
		beginWeb(),
		setupCaptivePortal();
};
extern Nefry_Web NefryWeb;
#endif
