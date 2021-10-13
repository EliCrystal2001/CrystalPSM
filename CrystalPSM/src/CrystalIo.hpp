/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_IO_
#define _CRYSTAL_IO_
#include <vita2d.h>
#include <vitasdk.h>
#include "CrystalFileData.hpp"

using namespace std;

namespace EliCrystal {

	class CrystalIo{
		public:
			static bool CopyFile(string filename, string destfilename);
			static bool FileExist(string filename);
			static bool DeleteFile(string filename);
			static uint64_t FileSize(string filename);
			static CrystalFileData* ReadFile(string filename);
			static bool WriteFile(string filename, CrystalFileData* data);

	};
}


#endif
