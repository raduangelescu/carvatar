// CarGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Application.h"

Application *g_Application;

int _tmain(int argc, _TCHAR* argv[])
{
	//Memory leak detection
#ifdef _DEBUG
	//_crtBreakAlloc = 490;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	g_Application = new Application();

	g_Application->init();
	g_Application->run();
	delete g_Application;

	LOG("Program normal termination.");
	return 0;
}

