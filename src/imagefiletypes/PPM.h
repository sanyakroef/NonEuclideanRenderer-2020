/*
 * PPM.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef IMAGEFILETYPES_PPM_H_
#define IMAGEFILETYPES_PPM_H_

#include "../ImageFileType.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class PPM : public ImageFileType {
public:
	inline PPM() {}
	virtual ~PPM() {}
	virtual std::string getExtention() {
		return ".ppm";
	}
	virtual void saveFile(std::string file, unsigned width, unsigned height, Vector4* img) {
		ofstream myFile;
		myFile.open (file, ios::out | ios::binary);

		std::stringstream ss;
		ss << "P6\n" << width << ' ' << height << "\n255\n";
		std::string header = ss.str();
		myFile.write(header.c_str(), header.size());

		unsigned size = width*height;
		char* fileData = new char[size*3];

		for (unsigned i = 0;i<size;i++) {
			Vector4 c = img[i];
			unsigned i2 = i*3;
			fileData[i2]   = (char)(unsigned)clamp<double>(c.x*255, 0, 255);
			fileData[i2+1] = (char)(unsigned)clamp<double>(c.y*255, 0, 255);
			fileData[i2+2] = (char)(unsigned)clamp<double>(c.z*255, 0, 255);
		}

		myFile.write(fileData, size*3);
		delete[] fileData;
		myFile.flush();
		myFile.close();
	}

	virtual void* loadFile(std::string file) {
		ifstream myFile;
		myFile.open (file, ios::in | ios::binary);
		if (myFile.bad())
			return nullptr;

		{//validize
			char c[2];
			myFile.read(c, 2);
			if (c[0] != 'P' || c[1] != '6') {
				myFile.close();
				return nullptr;
			}
		}
		unsigned width = 0;
		unsigned height = 0;
		{//read width and height and depixency
			char c;
			do {myFile.read(&c, 1);} while (c == ' ' || c == '\n' || c == '\r' || c == '\t');

			do {
				width = width*10 + (c-'0');
				myFile.read(&c, 1);
			} while (!(c == ' ' || c == '\n' || c == '\r' || c == '\t'));

			do {myFile.read(&c, 1);} while (c == ' ' || c == '\n' || c == '\r' || c == '\t');

			do {
				height = height*10 + (c-'0');
				myFile.read(&c, 1);
			} while (!(c == ' ' || c == '\n' || c == '\r' || c == '\t'));

			do {myFile.read(&c, 1);} while (c == ' ' || c == '\n' || c == '\r' || c == '\t');

			unsigned depixency = 0;
			do {
				depixency = depixency*10 + (c-'0');
				myFile.read(&c, 1);
			} while (!(c == ' ' || c == '\n' || c == '\r' || c == '\t'));

			//std::cout << width << ' ' << height << ' ' << depixency << endl;

			if (depixency != 255) {
				myFile.close();
				return nullptr;
			}
		}

		void* image = createImage<charColor>(width, height);
		char* data = (char*) (void*) getImageData<charColor>(image);
		//cout << "t" << (void*)data << image << endl;
		myFile.read(data, width*height*3);
		myFile.close();
		return image;
	}
};

static PPM* PPMfileType = new PPM();


#endif /* IMAGEFILETYPES_PPM_H_ */
