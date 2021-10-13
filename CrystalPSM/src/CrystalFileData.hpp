/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_FILE_DATA_
#define _CRYSTAL_FILE_DATA_
#include <vita2d.h>
#include <vitasdk.h>

using namespace std;

namespace EliCrystal {

	class CrystalFileData{
		public:
			CrystalFileData(uint64_t sz, char* data);
			uint64_t FileSize;
			char* FileData;
	};
}


#endif


