/*
*	 Written by Eli Crystal
*/

#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include "CrystalIo.hpp"

/*
*	IO Class
*	Handles all rendering...
*/
namespace EliCrystal {

	
	/*
	*	Method to copy a file, from one path to another
	*/
	bool CrystalIo::CopyFile(string filename, string destfilename){		
		CrystalFileData* Data = CrystalIo::ReadFile(filename);
		if(Data->FileSize <= 0)
			return false;
		return CrystalIo::WriteFile(destfilename, Data);
	}
	
	/*
	*	Method to get Filesize of a file,
	*/
	uint64_t CrystalIo::FileSize(string filename){		
		SceUID Fd = sceIoOpen(filename.c_str(), SCE_O_RDONLY, 0);
		if (Fd < 0)
			return Fd;
		uint64_t FileSize = sceIoLseek(Fd, 0, SCE_SEEK_END);
		sceIoClose(Fd);
		return FileSize;
	}
	
	/*
	*	Method to determine if a file exists.
	*/
	bool CrystalIo::FileExist(string filename){
		if(CrystalIo::FileSize(filename) >= 0){
			return true;
		}
		return false;
	}
	
	/*
	*	Method to remove a file.
	*/
	bool CrystalIo::DeleteFile(string filename){
		int ret = sceIoRemove(filename.c_str());
		if(ret >= 0)
			return true;
		return false;
	}
	
	/*
	*	Method to read data from a file
	*/
	CrystalFileData* CrystalIo::ReadFile(string filename){
		uint64_t Sz = CrystalIo::FileSize(filename);
		char* Buf = (char*)malloc(Sz);
		
		SceUID Fd = sceIoOpen(filename.c_str(), SCE_O_RDONLY, 0777);
		if (Fd < 0){
			sceClibPrintf("ERROR! %x\n", Fd);
			return new CrystalFileData(0, NULL);
		}
		sceIoRead(Fd, Buf, Sz);
		sceIoClose(Fd);

		CrystalFileData* FileData = new CrystalFileData(Sz, Buf);
		return FileData;
	}
	
	/*
	*	Method to write data to a file
	*/
	bool CrystalIo::WriteFile(string filename, CrystalFileData* data){		
		uint64_t Sz = data->FileSize;
		char* Buf = data->FileData;
		
		SceUID Fd = sceIoOpen(filename.c_str(), SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
		if (Fd < 0)
			return false;
		sceIoWrite(Fd, Buf, Sz);
		sceIoClose(Fd);
		
		return true;
	}
	
}

