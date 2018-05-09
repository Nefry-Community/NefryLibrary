#ifndef NefryConsole_h
#define NefryConsole_h

#include "NefryWebServer.h"
#include "NefryWeb.h"
class Nefry_Console
{
public:
	void
		beginWeb(),
		clearConsole(),
		
		println(float text),
		println(double text),
		println(char text),
		println(int text),
		println(long text),
		println(unsigned char text),
		println(unsigned int text),
		println(unsigned long text),
		print(float text),
		print(double text),
		print(char text),
		print(int text),
		print(long text),
		print(unsigned char text),
		print(unsigned int text),
		print(unsigned long text),
		print(String text, int ln = 0),
		println(String text);

	int
		available();

	String
		read();

private:
	String input_console;
};
extern Nefry_Console NefryConsole;
#endif
