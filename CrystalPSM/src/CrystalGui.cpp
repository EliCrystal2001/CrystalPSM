/*
*	 Written by Eli Crystal
*/

#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include <taihen.h>
#include "CrystalGui.hpp"
#include "CrystalIo.hpp"


/*
*	Gui Class
*	Handles Graphical User Interface
*/
namespace EliCrystal {
	/*
	*	Method to initalize CrystalRender;
	*/
	void CrystalGui::InitCrystalRender(){
		sceClibPrintf("Setting up CrystalRender..\n");
		this->Renderer = new CrystalRender();
	}
	
	void CrystalGui::InitCrystalGui(){
		sceClibPrintf("Setting up CrystalGui..\n");
		// Set some defaults.
		this->Renderer->SetDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
		this->Renderer->SetDrawSize(1.0F);
		this->Renderer->SetDrawFont(CrystalPgfFont);
		this->GuiState = CrystalStateDownloading;
		
		// Load background image.
		sceClibPrintf("Loading background image...\n");
		this->Background = this->Renderer->LoadImage("app0:/bg.png"); 
		sceClibPrintf("Done!\n");
	}
	/*
	*	Initalize CrystalPackageDownloader 
	*	For Update and Game PKG. & Start Downloading.
	*/
	void CrystalGui::InitCrystalPackageDownloader(){		
		sceIoMkdir("ux0:/data", 0777);
		this->RuntimeDownloader = new CrystalPackageDownloader("http://ares.dl.playstation.net/psm-runtime/IP9100-PCSI00011_00-PSMRUNTIME000000.pkg", "ux0:/data/Runtime1.00.pkg");
		this->RuntimeUpdateDownloader = new CrystalPackageDownloader("http://gs.ww.np.dl.playstation.net/ppkg/np/PCSI00011/PCSI00011_T8/286a65ec1ebc2d8b/IP9100-PCSI00011_00-PSMRUNTIME000000-A0201-V0100-e4708b1c1c71116c29632c23df590f68edbfc341-PE.pkg", "ux0:/data/Runtime2.01.pkg");
		
		bool SuccessRuntime = this->RuntimeDownloader->StartDownload();
		bool SuccessRuntimeUpdate = this->RuntimeUpdateDownloader->StartDownload();
		
		if(!(SuccessRuntime && SuccessRuntimeUpdate))
			this->GuiState = CrystalStateDownloadFail;
	}
	
	/*
	*	Class Constructor
	*	This gets called when the class is initalized.
	*/
	CrystalGui::CrystalGui(){
		this->InitCrystalPackageDownloader();
		this->InitCrystalRender();
		this->InitCrystalGui();
	}
	
	/*
	*	Update method
	*	Handles Gui Updates (controls, etc) 
	*/
	void CrystalGui::Update(){
		bool RuntimeSaveFile = true;
		bool RuntimeUpdateSaveFile = true;
		switch(this->GuiState){
			case CrystalStateDownloading:
				// If download complete, set mode to CrystalLoadModules
				if(this->RuntimeDownloader->DownloadComplete && this->RuntimeUpdateDownloader->DownloadComplete){
					this->GuiState = CrystalStateLoadModules;
					break;
				}
				
				RuntimeSaveFile = this->RuntimeDownloader->SaveFile();
				RuntimeUpdateSaveFile = this->RuntimeUpdateDownloader->SaveFile();
				
				// If error occured, display fail screen.
				if(!(RuntimeSaveFile && RuntimeUpdateSaveFile))
					this->GuiState = CrystalStateDownloadFail;
				
				break;
			case CrystalStateLoadModules:
				// Copy CrystalDriver.skprx to ux0:/data
				if(CrystalIo::CopyFile("app0:/CrystalDriver.skprx", "ux0:/data/CrystalDriver.skprx")){
					// Start the kernel module (Enables the package installer)
					taiLoadStartKernelModule("ux0:/data/CrystalDriver.skprx", 0, NULL, 0);
					this->GuiState = CrystalStateLoadPkgInstaller;
				}
				break;
			case CrystalStateLoadPkgInstaller:
				// Start the package installer
				sceAppMgrLaunchAppByName(0x60000, "NPXS10031", "[BATCH]host0:/package/Runtime1.00.pkg\nhost0:/package/Runtime2.01.pkg");
				break;
			case CrystalStateDownloadFail:
				break;
		}
	}
	
	/*
	*	Render method
	*	Renders the current GUI to the screen.
	*/
	void CrystalGui::Render(){
		this->Renderer->DrawStart(); 
		this->Renderer->DrawImage(this->Background, 0, 0); // Draw background

		/*
		*	Draw Top
		*/
		this->Renderer->SetDrawColor(255, 0, 255, 255);
		this->Renderer->SetDrawSize(1.3F);
		this->Renderer->DrawText("**** PSM Runtime Installer ****", 113, 115); 
		
		CrystalProgress DownloadProgress = 100;
		CrystalProgress DownloadUpdateProgress = 100;
		
		switch(this->GuiState){
			case CrystalStateDownloading:
				/*
				*	Draw text
				*/
				
				this->Renderer->SetDrawSize(1.0F);
				this->Renderer->SetDrawColor(0, 0, 0, 255);
				
				DownloadProgress = this->RuntimeDownloader->GetDownloadProgress();
				DownloadUpdateProgress = this->RuntimeUpdateDownloader->GetDownloadProgress();
				
				this->Renderer->DrawText("Downloading PSM Runtime 1.00 PKG: " + to_string((int)DownloadProgress) + "%", 130, 197); 
				this->Renderer->DrawText("Downloading PSM Runtime 2.01 PKG: " + to_string((int)DownloadUpdateProgress) + "%", 130, 353); 

				break;
			case CrystalStateLoadModules:
			case CrystalStateLoadPkgInstaller:
				this->Renderer->SetDrawSize(1.0F);
				this->Renderer->SetDrawColor(255, 255, 255, 255);
				
				this->Renderer->DrawText("Please Wait..", 130, 197); 
				break;
			case CrystalStateDownloadFail:
				this->Renderer->SetDrawSize(1.0F);
				this->Renderer->SetDrawColor(255, 255, 255, 255);
				
				this->Renderer->DrawText("Something went wrong", 130, 197); 
			
		}
		
		/*
		*	Draw progress bar
		*/

		// Draw Back 
		this->Renderer->SetDrawColor(0, 0, 0, 128);
		this->Renderer->DrawRectangle(130, 234, 500, 30); 				
		this->Renderer->DrawRectangle(130, 390, 500, 30); 
		
		// Draw Front
		this->Renderer->SetDrawColor(181, 230, 210, 128);
		this->Renderer->DrawRectangle(130, 234, DownloadProgress*5, 30); 				
		this->Renderer->DrawRectangle(130, 390, DownloadUpdateProgress*5, 30);

		this->Renderer->DrawEnd();
	}

}

