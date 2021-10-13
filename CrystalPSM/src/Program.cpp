/*
*	 Written by Eli Crystal
*/

#include <vitasdk.h>
#include <sstream>
#include <vector>
#include <cstdio>
#include "CrystalGui.hpp"

using namespace std;
namespace EliCrystal {
	/*
	*	Default Class
	*	Gets called at the start of the program.
	*/
	class Program{
		private:
			CrystalGui* Gui;
			/*
			*	Method to setup LibHttp and LibNet
			*/
			void InitHttp(){
				sceClibPrintf("Starting SceNet.\n");
				sceSysmoduleLoadModule(SCE_SYSMODULE_NET);	
				SceNetInitParam netInitParam;
				int size = 1*1024*1024;
				netInitParam.memory = malloc(size);
				netInitParam.size = size;
				netInitParam.flags = 0;
				sceNetInit(&netInitParam);
				sceNetCtlInit();
				
				sceClibPrintf("Starting SceHttp.\n");
				sceSysmoduleLoadModule(SCE_SYSMODULE_HTTPS);
				sceHttpInit(1*1024*1024);
			}

		public:
			Program(){
				this->InitHttp();
				this->Gui = new CrystalGui();
				/*
				*	Main program loop, Handles UI, Actions, Etc
				*/
				while(true){
					this->Gui->Update();
					this->Gui->Render();
				}
			}
	};
}


/*
*	Required entry point method.
*/
int main(int argc, char *argv[]) {	
	// Run program
	sceClibPrintf("CrystalPSMInstaller.\n");
	sceClibPrintf("(c) 2021 Eli Crystal\n");
	sceClibPrintf("Starting program...\n");
	EliCrystal::Program* program = new EliCrystal::Program();
    return 0;
}
