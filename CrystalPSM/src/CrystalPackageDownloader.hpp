/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_PACKAGE_DOWNLOADER_
#define _CRYSTAL_PACKAGE_DOWNLOADER_
#include <vita2d.h>
#include <vitasdk.h>
#include <iostream>

using namespace std;

namespace EliCrystal {
		
	typedef int SceHttpTemplate;
	typedef int SceHttpConnection;
	typedef int SceHttpRequest;

	typedef float CrystalProgress;
	typedef uint64_t CrystalBytesDownloaded;
	typedef uint64_t CrystalTotalSize;
	
	class CrystalPackageDownloader{
		private:
			void SetupHttp();
			string PackageUrl;
			SceHttpTemplate Template;
			SceHttpConnection Connection;

			string SaveAs;
			SceHttpRequest Request;
			
			SceUID OutputFD;			
		public:
			CrystalBytesDownloaded DownloadedBytes;
			CrystalTotalSize TotalBytes;
			CrystalPackageDownloader(string Url, string OutputFile);
			bool DownloadComplete;
			bool SaveFile();
			bool StartDownload();
			CrystalProgress GetDownloadProgress();
	};
}


#endif
