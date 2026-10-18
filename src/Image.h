/*
 * Image.h
 *
 *  Created on: 21 Jan 2020
 *      Author: christiaan
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdlib.h>

/*
template <class T>
class Image {
	unsigned width;
	unsigned height;
	T* data;
	unsigned size;
	inline Image(unsigned width, unsigned height) : width(width), height(height), size(width*height) {
		data = new T[size];
	}
	inline ~Image() {
		delete[] data;
	}
};
*/
template <class T>
static void* createImage(unsigned width, unsigned height) {
	unsigned size = width*height;
	void* img = malloc(sizeof(unsigned)*3+sizeof(T)*size);
	//void* img = (void*) new char[sizeof(unsigned)*3+sizeof(T)*size];
	unsigned* imgU = (unsigned*)img;
	imgU[0] = width;
	imgU[1] = height;
	imgU[2] = size;
	return (void*) &(imgU[3]);
}
template <class T>
static void deleteImage(void* img) {
	unsigned* imgU = ((unsigned*)img)-3;
	//unsigned size = imgU[2];
	free(imgU);
}
static inline unsigned getImageWidth(void* img) {
	unsigned* imgU = (unsigned*)img;
	//imgU-=3;
	return imgU[-3];
}
static inline unsigned getImageHeight(void* img) {
	unsigned* imgU = (unsigned*)img;
	//imgU-=3;
	return imgU[-2];
}
static inline unsigned getImageSize(void* img) {
	unsigned* imgU = (unsigned*)img;
	//imgU-=3;
	return imgU[-1];
}
template <class T>
static inline T* getImageData(void* img) {
	return (T*) img;
}

template <class T>
static inline T getPixelData(void* img, unsigned x, unsigned y) {
	return getImageData<T>(img)[x+y*getImageWidth(img)];
}
template <class T>
static inline T getPixelData(void* img, double x, double y) {
	return getPixelData<T>(img, (unsigned)(x * getImageWidth(img)), (unsigned)(y * getImageHeight(img)));
}

template <class T>
static inline T getPixelDataSafe(void* img, double x, double y, T defaultValue) {
	/*
	if (x < 0 || y < 0 || x > 1 || y > 1)
		return defaultValue;
	else
		return getPixelData<T>(img, x, y);
		*/
	int width = getImageWidth(img);
	int height = getImageHeight(img);
	int x2 = x*width;
	int y2 = y*height;

	if (x2 < 0 || y2 < 0 || x2 >= width || y2 >= height)
		return defaultValue;

	unsigned i = x2+y2*width;
	return getImageData<T>(img)[i];
}

struct __attribute__((packed)) charColor {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

static charColor charColorBLACK = {0,0,0};



#endif /* IMAGE_H_ */
