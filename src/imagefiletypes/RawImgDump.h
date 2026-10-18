/*
 * RawImgDump.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef IMAGEFILETYPES_RAWIMGDUMP_H_
#define IMAGEFILETYPES_RAWIMGDUMP_H_

#include "../ImageFileType.h"
#include <fstream>

using namespace std;

class RawImgDump : public ImageFileType {
public:
	inline RawImgDump() {}
	virtual ~RawImgDump() {}
	virtual std::string getExtention() {
		return ".raw";
	}
	virtual void saveFile(std::string file, unsigned width, unsigned height, Vector4* img) {
		ofstream myFile;
		myFile.open (file, ios::out | ios::binary);

		myFile.write((char*)(void*)img, width*height*sizeof(Vector4));
		myFile.flush();
		myFile.close();
	}
};



#endif /* IMAGEFILETYPES_RAWIMGDUMP_H_ */
