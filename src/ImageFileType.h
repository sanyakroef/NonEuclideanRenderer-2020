/*
 * ImageFileType.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef IMAGEFILETYPE_H_
#define IMAGEFILETYPE_H_

#include "Vector.h"
#include <string>
#include "Image.h"

template<class T>
const T& clamp( const T& v, const T& lo, const T& hi )
{
    assert( !(hi < lo) );
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

class ImageFileType {
public:
	inline ImageFileType() {}
	virtual ~ImageFileType() {}
	virtual std::string getExtention() = 0;
	virtual void saveFile(std::string file, unsigned width, unsigned height, Vector4* img) = 0;
	virtual void* loadFile(std::string file) {return nullptr;};
};


#endif /* IMAGEFILETYPE_H_ */
