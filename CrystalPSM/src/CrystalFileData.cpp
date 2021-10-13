/*
*	 Written by Eli Crystal
*/

#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include "CrystalFileData.hpp"

/*
*	File Data Class
*	Stores information about file read operations.
*/
namespace EliCrystal {
	/*
	*	Class Constructor
	*	This gets called when the class is initalized.
	*/
	CrystalFileData::CrystalFileData(uint64_t sz, char* data){
		this->FileSize = sz;
		this->FileData = data;
	}

}

