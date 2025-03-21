/*
*	 Written by Eli Crystal
*/

#include <vitasdk.h>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <functional>

#include "CrystalPackageDownloader.hpp"

/*
*	Class for handling of Downloading Packages
*/
using namespace std;
namespace EliCrystal {
			
	/*
	*	Sets up the template, and connection
	*/
	void CrystalPackageDownloader::SetupHttp(){
		sceClibPrintf("Setting up HTTP Templates and Connections\n"); 
		this->Template = sceHttpCreateTemplate("CrystalPackageDownloader", 1, 1);
		this->Connection = sceHttpCreateConnectionWithURL(this->Template, this->PackageUrl.c_str(), 0);
		this->Request = sceHttpCreateRequestWithURL(this->Connection, SCE_HTTP_METHOD_GET, this->PackageUrl.c_str(), 0);
	}
	/*
	*	Saves the downloaded file to the memory card
	*/
	bool CrystalPackageDownloader::SaveFile(){
		if(this->DownloadComplete)
			return true;
		
		unsigned char Data[0x8000];
		int TotalRead = sceHttpReadData(this->Request, &Data, sizeof(Data));
		
		/*
		*	Error checking..
		*/
		if(TotalRead < 0)
			return false;
		
		sceIoWrite(this->OutputFD, Data, TotalRead);
		this->DownloadedBytes += TotalRead;
			
		if(this->DownloadedBytes >= this->TotalBytes)
		{
			this->DownloadComplete = true;
			sceIoClose(this->OutputFD);
		}
		return true;
	}
	
	/*
	*	Initalize, Set what package to be downloaded
	*/
	CrystalPackageDownloader::CrystalPackageDownloader(string Url, string OutputFile) { 
		sceClibPrintf("Setting up CrystalPackageDownloader..\n");
		this->PackageUrl = Url;
		this->SaveAs = OutputFile;
		this->DownloadComplete = false;
		this->DownloadedBytes = 0;
		this->TotalBytes = 0;
		this->SetupHttp();
	}
	
	/*
	*	Start downloading the package
	*	Return: true on success, false on fail.
	*/
	bool CrystalPackageDownloader::StartDownload()
	{
		sceClibPrintf("Starting Download...\n");
		if(sceHttpSendRequest(this->Request, NULL, 0) < 0)
			return false;
		
		// C buffer stuff, i hate when i have to do this...
		unsigned int HeadersLen = 0;
		char *Headers;
		
		const char *HeaderValue;
		unsigned int HeaderLen = 0;
		
		if(sceHttpGetAllResponseHeaders(this->Request, &Headers, &HeadersLen) < 0)
			return false;
		if(sceHttpParseResponseHeader(Headers, HeadersLen, "Content-Length", &HeaderValue, &HeaderLen) < 0)
			return false;
		
		char* HeaderValueBuf = (char*)malloc(HeaderLen+1);
		memset(HeaderValueBuf, 0x00, HeaderLen+1);
		strncpy(HeaderValueBuf, HeaderValue, HeaderLen);
		this->TotalBytes = atoll(HeaderValueBuf);
		
		if(TotalBytes <= 0)
			return false;
		
		this->OutputFD = sceIoOpen(this->SaveAs.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
		
		return true;
	}
	
	/*
	*	Get progress for package download.
	*/
	CrystalProgress CrystalPackageDownloader::GetDownloadProgress(){
		return (100.0*((float)this->DownloadedBytes/(float)this->TotalBytes));
	}
}