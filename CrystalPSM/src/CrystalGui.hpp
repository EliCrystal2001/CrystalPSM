/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_GUI_
#define _CRYSTAL_GUI_
#include "CrystalRender.hpp"
#include "CrystalPackageDownloader.hpp"

using namespace std;

namespace EliCrystal {

	enum CrystalGuiState{
		CrystalStateDownloading,
		CrystalStateLoadModules,
		CrystalStateLoadPkgInstaller,
		CrystalStateDownloadFail
	};

	class CrystalGui{
		private:
			CrystalTexture Background;
			CrystalGuiState GuiState;
			
			CrystalRender* Renderer;
			CrystalPackageDownloader* RuntimeDownloader;
			CrystalPackageDownloader* RuntimeUpdateDownloader;

			void InitCrystalPackageDownloader();
			void InitCrystalRender();
			void InitCrystalGui();
		public:
			CrystalGui();
			void Update();
			void Render();
	};
}


#endif
