/*
 * RawImgDump.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef IMAGEFILETYPES_RAW255IMGDUMP_H_
#define IMAGEFILETYPES_RAW255IMGDUMP_H_

#include "../ImageFileType.h"
#include <fstream>

using namespace std;

class Raw255ImgDump : public ImageFileType {
public:
	inline Raw255ImgDump() {}
	virtual ~Raw255ImgDump() {}
	virtual std::string getExtention() {
		return ".raw";
	}
	virtual void saveFile(std::string file, unsigned width, unsigned height, Vector4* img) {
		ofstream myFile;
		myFile.open (file, ios::out | ios::binary);

		unsigned size = width*height;
		char* fileData = new char[size*4];

		for (unsigned i = 0;i<size;i++) {
			Vector4 c = img[i];
			unsigned i2 = i*4;
			fileData[i2+3]   = (char)(unsigned)clamp<double>(c.x*255, 0, 255);
			fileData[i2+2] = (char)(unsigned)clamp<double>(c.y*255, 0, 255);
			fileData[i2+1] = (char)(unsigned)clamp<double>(c.z*255, 0, 255);
			fileData[i2] = (char)(unsigned)clamp<double>(c.m*255, 0, 255);
		}

		myFile.write(fileData, size*4);
		delete[] fileData;
		myFile.flush();
		myFile.close();
	}
};



#endif /* IMAGEFILETYPES_RAWIMGDUMP_H_ */
