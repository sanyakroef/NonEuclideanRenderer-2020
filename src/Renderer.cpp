//============================================================================
// Name        : Renderer.cpp
// Author      : Sanya (aka. Christiaan)
//============================================================================

#include <iostream>
#include "noise/Random.h"
#include "Ray.h"
#include "Matrix.h"

#include "rendrables/Triangle.h"
#include "rendrables/Sphere.h"
#include "rendrables/AABBcube.h"

#include "matirials/DirectColorMatirial.h"
#include "matirials/DebugMatirial.h"
#include "matirials/MirorMatirial.h"
#include "matirials/AddMatirial.h"
#include "matirials/CubeOutlineMatirial.h"
#include "matirials/LightsMatirial.h"
#include "matirials/CustomMatirial.h"

#include "matirials/nonedian/SphereEdgeNonedianMatirial.h"
#include "matirials/nonedian/AABBcubeInsideNonedianMatirial.h"

#include "renderEnviroments/DebugRenderEnv.h"
#include "renderEnviroments/TransparantRenderEnv.h"
#include "renderEnviroments/FogRenderEnv.h"

#include "cameras/PerspectiveCameraNoise.h"
#include "cameras/PerspectiveCamera.h"

#include "imagefiletypes/PPM.h"
#include "imagefiletypes/Raw255ImgDump.h"

#include "MovieDate.h"

#include <random>
#include <algorithm>
#include <cmath>
#include <string>
#include <thread>
#include <time.h>
#include <stdio.h>
#include <sstream>
using namespace std;

//const static double goldenRatio = (1+sqrt(5))/2;

void printMem(void* ptr, unsigned size) {
	for (unsigned i = 0;i<size;i++)
		cout << std::hex << (unsigned)(((unsigned char*)ptr)[i]) << ' ';
}
/*
bool maxStack(RenderStackFrame3* frame) {
	return frame->rayCount <= 10 && frame->noEndian <= 100 && frame->value > 0.0004;//10, 0.0004
	//return frame->allRayCount <= 40;
}
bool maxStack4(RenderStackFrame4* frame) {
	return frame->rayCount <= 10 && frame->noEndian <= 100 && frame->value > 0.0004;//10, 0.0004
	//return frame->allRayCount <= 40;
}
*/

/*
#define WIDTH2 300
#define HEIGHT2 200

/*
#define WIDTH2 1600
#define HEIGHT2 1000
*/

#define THEADCOUNT 8

void renderImageThread(unsigned width, unsigned height, unsigned subPixelSize, Camera3* cam, RenderEnvBasic3* env, Vector4* img, unsigned stepStart, int64_t seed) {
	unsigned subPixelVolume = subPixelSize*subPixelSize;
	Noise::Random r (seed);
	for (unsigned y = 0;y<height;y++)
		for (unsigned x = stepStart;x<width;x+=THEADCOUNT) {
			Vector4 c;
			for (unsigned subY = 0;subY<subPixelSize;subY++)
				for (unsigned subX = 0;subX<subPixelSize;subX++)
					c += LiniarColor(cam->renderPixel(x*subPixelSize+subX, y*subPixelSize+subY, env, r));
			c/=subPixelVolume;
			img[x+y*width] = c;
		}
}

void renderImage(unsigned width, unsigned height, unsigned subPixelSize, Camera3* cam, RenderEnvBasic3* env, Vector4* img, Noise::Random& r) {
	cam->setSize(width*subPixelSize, height*subPixelSize);

	std::thread threads[THEADCOUNT];
	for (unsigned i = 0;i<THEADCOUNT;i++)
		threads[i] = std::thread(&renderImageThread, width, height, subPixelSize, cam, env, img, i, r.nextInt64());
	for (unsigned i = 0;i<THEADCOUNT;i++)
		threads[i].join();
}


void renderImageThread4(unsigned width, unsigned height, unsigned subPixelSize, Camera4* cam, RenderEnv4* env, Vector4* img, unsigned stepStart, int64_t seed) {
	unsigned subPixelVolume = subPixelSize*subPixelSize;
	Noise::Random r (seed);
	for (unsigned y = 0;y<height;y++)
		for (unsigned x = stepStart;x<width;x+=THEADCOUNT) {
			Vector4 c;
			for (unsigned subY = 0;subY<subPixelSize;subY++)
				for (unsigned subX = 0;subX<subPixelSize;subX++)
					c += LiniarColor(cam->renderPixel(x*subPixelSize+subX, y*subPixelSize+subY, env, r));
			c/=subPixelVolume;
			img[x+y*width] = c;
		}
}

void renderImage4(unsigned width, unsigned height, unsigned subPixelSize, Camera4* cam, RenderEnv4* env, Vector4* img, Noise::Random& r) {
	cam->setSize(width*subPixelSize, height*subPixelSize);

	std::thread threads[THEADCOUNT];
	for (unsigned i = 0;i<THEADCOUNT;i++)
		threads[i] = std::thread(&renderImageThread4, width, height, subPixelSize, cam, env, img, i, r.nextInt64());
	for (unsigned i = 0;i<THEADCOUNT;i++)
		threads[i].join();
}

/*
const static Matrix4 rbgOverflow = Matrix4((double[4][4]){{1,0.001,0.000005,0},
	{0.001   ,1      ,0.00001,0},
	{0.000005,0.00001,1      ,0},
	{0,0,0,1}});
	*/
/*
const static Matrix4 rbgOverflow = Matrix4((double[4][4]){{1,0.1,0.005,0},
	{0.1   ,1      ,0.01,0},
	{0.005,0.01,1      ,0},
	{0,0,0,1}});
	*/
const static Matrix4 rbgOverflow = Matrix4((double[4][4]){{1,0.2,0.01,0},
	{0.2   ,1      ,0.02,0},
	{0.01,0.02,1      ,0},
	{0,0,0,1}});

void imageOverflowThread1(unsigned width, unsigned height, Vector4* img, unsigned stepStart) {
	for (unsigned y = 0;y<height;y++)
		for (unsigned x = stepStart;x<width;x+=THEADCOUNT)
			img[x+y*width] = img[x+y*width] * rbgOverflow;
}
void imageOverflowThread2(unsigned width, unsigned height, Vector4* imgIn, Vector4* imgOut, int sSize, unsigned stepStart, double strenght) {
	double widthInv = 600.0/width;
	for (unsigned y = 0;y<height;y++)
		for (unsigned x = stepStart;x<width;x+=THEADCOUNT) {
			Vector4 out;
			for (int y2 = -sSize;y2<=sSize;y2++) {
				int y3 = y+y2;
				if (y3 >= 0 && y3 < height) {
					for (int x2 = -sSize;x2<=sSize;x2++) {
						int x3 = x+x2;
						if (x3 >= 0 && x3 < width) {
							double x4 = x2*widthInv;
							double y4 = y2*widthInv;
							double l = x4*x4+y4*y4;//320000*320000
							Vector4 v = imgIn[x3+y3*width] / (l*l*strenght+1);
							out += v;
						}
					}
				}
			}
			out.m = imgIn[x+y*width].m;
			imgOut[x+y*width] = out;
		}
}

void imageOverflow(unsigned width, unsigned height, Vector4* imgIn, Vector4* imgOut, int sSize, double strenght) {
	strenght = 1/strenght;
	{
		std::thread threads[THEADCOUNT];
		for (unsigned i = 0;i<THEADCOUNT;i++)
			threads[i] = std::thread(&imageOverflowThread1, width, height, imgIn, i);
		for (unsigned i = 0;i<THEADCOUNT;i++)
			threads[i].join();
	}
	if (sSize > 1) {
		std::thread threads[THEADCOUNT];
		for (unsigned i = 0;i<THEADCOUNT;i++)
			threads[i] = std::thread(&imageOverflowThread2, width, height, imgIn, imgOut, sSize, i, strenght);
		for (unsigned i = 0;i<THEADCOUNT;i++)
			threads[i].join();
	}
}

//                                   raw  1080p 480p 720p, final test test2
static const unsigned width  = 1920;//600, 1920, 720, 1280, 1920, 150, 300,  480
static const unsigned height = 1080;//400, 1080, 480, 720,  1080, 100, 150,  270
static const unsigned subPixelSize = 1;//2                 2,    1,   1
static ImageFileType* imgType = new PPM();//new Raw255ImgDump();

static const unsigned startFrame = 0;//1, 461, 583, 461, 744
static const unsigned endFrame = 24*5;//96, 60, 200 ,460, 800, 675, 1100, 1213

static const unsigned debugFrameOfset = 0;//0, 460

static const float fps = 24;
static const float fpsI = 1/fps;

/*
 * mini / local
 * ~650 Renderer.cpp
 * 644 MovieDate.h
 * 337 MovieDate.h
 * 265 MovieDate.h
 */

static void renderSingle3(Noise::Random& r, RenderEnvBasic3* ren, Camera3* cam) {
	unsigned size = width*height;
	Vector4* img = new Vector4[size];

	cout << "rendering img..." << endl;

	renderImage(width, height, subPixelSize, cam, ren, img, r);

	cout << "rendering done." << endl;
/*
	{
		Vector4* img2 = new Vector4[size];
		imageOverflow(width, height, img, img2, 20, .8);//20, 1
		delete[] img;
		img = img2;
	}
*/
	cout << "writing img..." << endl;

	imgType->saveFile("../outputImg"+imgType->getExtention(), width, height, img);
	delete[] img;

	//TODO xor patren matirial (2 mats as input)

	cout << "done." << endl;
}

static void renderSingle4(Noise::Random& r, RenderEnvBasic4* ren, Camera4* cam) {
	unsigned size = width*height;
	Vector4* img = new Vector4[size];

	cout << "rendering img..." << endl;

	renderImage4(width, height, subPixelSize, cam, ren, img, r);

	cout << "rendering done." << endl;
/*
	{
		Vector4* img2 = new Vector4[size];
		imageOverflow(width, height, img, img2, 20, .8);//20, 1
		delete[] img;
		img = img2;
	}
*/
	cout << "writing img..." << endl;

	imgType->saveFile("../outputImg"+imgType->getExtention(), width, height, img);
	delete[] img;

	//TODO xor patren matirial (2 mats as input)

	cout << "done." << endl;
}

static int render3dWorldTest() {
	int64_t seed = time(NULL);
	Noise::Random r (seed);

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	//RenderEnvBasic3* ren = new DebugRenderEnv3(&maxStack);
	//RenderEnvBasic3* ren = new TransparantRenderEnv3(&maxStack);
	//RenderEnvBasic3* ren = new FogRenderEnv3(&maxStack, Color(0.8), 0.003);//0.01
	RenderEnvBasic3* ren = new FogRenderEnv3(&maxStack, Color(0), 0.003);

	//Light3 lights[1] = {Light3(Sphere3(Vector3(-4,-3,-4.5), 0.1), 1000)};//Vector3(-4,-3,-4.5)
	Light3 lights[1] = {Light3(Sphere3(Vector3(-4,-3,-4.5), 0.1), 6700)};

	//DirectColorMatirial3 mat (Color(1, 0, 0));
	//Matirial3* mat = new DirectColorMatirial3(Color(1, 0, 0));
	Matirial3* mat = new NormMatirial3();
	Matirial3* mat2 = new MirorMatirial3();
	//Matirial3* mat3 = new AddMatirial3(new DirectColorMatirial3(Color(0, 0.05, 0.1, 0.1)), mat2);
	Matirial3* mat3 = new AddMatirial3(new DirectColorMatirial3(Color(0, 0.2, 0.4, 0.4)), mat2);
	Matirial3* mat4 = new CubeOutlineMatirial3(mat, Color(0), 0.1);
	Matirial3* mat5 = new DirectColorMatirial3(Color(0));
	Matirial3* mat6 = new CubeOutlineMatirial3(mat3, Color(0), 0.1);
	Matirial3* mat7 = new CubeOutlineMatirial3(new DirectColorMatirial3(Color(0.4)), Color(0), 0.1);
	//Matirial3* mat8 = new LightsMatirial3(mat4, lights, 1);
	Matirial3* mat8 = new LightsMatirialBounded3(new CubeOutlineMatirial3(mat, Color(0.2), .25/goldenRatio), lights, 1);//.5/goldenRatio
	//Matirial3* mat8 = new LightsMatirialGlobal3(new CubeOutlineMatirial3(mat, Color(0.2), .25/goldenRatio), Vector3(1/3.0,2/3.0,-1).normalize());
	Matirial3* mat9 = new CustomMatirail2_3(mat, mat3, 10, new int[4]{3, 5, 7, 11}, 4);
	Matirial3* mat9_2 = new LightsMatirialBounded3(mat9, lights, 1);
	Matirial3* mat9_3 = new LightsMatirialBounded3(new CubeOutlineMatirial3(mat9, Color(0.2), .25/goldenRatio), lights, 1);

	Matirial3* noneMat1 = new SphereEdgeNonedianMatirial3();
	Matirial3* noneMat2 = new AABBcubeInsideNonedianMatirial3();

	//Camera3* cam = new PerspectiveCameraNoise3();
	PerspectiveCamera3* cam = new PerspectiveCamera3();
	cam->Xof = r.nextBool() ? r.nextDouble() : -r.nextDouble();
	cam->Yof = r.nextBool() ? r.nextDouble() : -r.nextDouble();

	//Triangle3 triangles[30];


	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(0, 1);
/*
	double* dArr = (double*)(void*)triangles;
	for (int i = 0;i<30*3*3;i++) {
		dArr[i] = (dist(e2)*2-1)*200;
	}
	for (int i = 0;i<30;i++)
		triangles[i].setup();
*/
	/*
	ren->addGeromitry(triangles, mat, 5);
	ren->addGeromitry(triangles+5, mat3, 25);
	*/
	//ren->addGeromitry(triangles, mat3, 30);

	//Sphere3 sph (Vector3(0, 50, 0), 180);
	//Sphere3 sph (Vector3(0, 30, 0), 10);
	//ren->addGeromitry(FullGeromitry3(Geromitry3(&sph), noneMat1));//noneMat1
/*
	for (unsigned i = 0;i<100;i++) {//100
		Vector3 loc (dist(e2)*200-100, dist(e2)*200+10, dist(e2)*200-100);
		Sphere3* sph = new Sphere3 (loc, (dist(e2)+0.5)*loc.lenght()/20);
		Matirial3* matC = new AddMatirial3(new DirectColorMatirial3(LiniarColor(dist(e2), dist(e2), dist(e2), 0.7)), mat2);
		ren->addGeromitry(FullGeromitry3(Geromitry3(sph), matC));
	}*/
/*
	for (unsigned i = 0;i<100;i++) {//40
		Vector3 loc (dist(e2)*200-100, dist(e2)*200+10, dist(e2)*200-100);
		Sphere3* sph = new Sphere3 (loc, (dist(e2)+0.5)*loc.lenght()/25);
		ren->addGeromitry(FullGeromitry3(Geromitry3(sph), mat3));//noneMat1
	}*/
/*
	Sphere3 sph (Vector3(1,0.5,0), 0.55);
	ren->addGeromitry(FullGeromitry3(Geromitry3(&sph), mat3));

	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(0.5,3,-0.5))), mat));
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-0.5,4,-0.5))), mat));
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-1.5,3,-0.5))), mat));
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-0.5,3.5,-1.5))), mat));
*/
	//ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(20.5,3,-0.5), 30)), mat));
	//ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-200,-200,-200), Vector3(200,200,200))), noneMat2));
	//ren->addGeromitry(FullGeromitry3(Geromitry3(new Sphere3(Vector3(1,40,0), 4)), noneMat1));

	//ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(.5,.5,-0.5), 1)), mat));

	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-11,-9,-9), Vector3(11,-8,-8))), mat9_3));//mat4, mat8, mat9_3
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-9,-11,-9), Vector3(-8,11,-8))), mat9_3));
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-9,-9,-11), Vector3(-8,-8,11))), mat9_3));

	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-9.5,-9.5,-9.5), Vector3(-7.5,-7.5,-7.5))), mat9_3));

	//ren->addGeromitry(FullGeromitry3(Geromitry3(new Sphere3(Vector3(-8.5, -8.5, -8.5), sqrt(2))), noneMat1));
/*
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-5.5,-5.5,-8.9), Vector3(8.5,8.5,-8.1))), mat));
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-11,0.8,-8.7), Vector3(11,0.2,-8.3))), mat));
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(0.8,-11,-8.7), Vector3(0.2,11,-8.3))), mat));
*/
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-10,-10,-10), Vector3(10,10,10))), noneMat2));
	//ren->addGeromitry(FullGeromitry3(Geromitry3(new Sphere3(Vector3(1,1,1), .5)), mat3));
	//ren->addGeromitry(FullGeromitry3(Geromitry3(new Sphere3(Vector3(1.2,7,0), 1)), mat3));

	//ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(1.2,7,0), 1)), mat9_2));

	//cam->org = Vector3(1,-1,1);
	//cam->org = Vector3(1+r.nextDouble()/1000,-1+r.nextDouble()/1000,1+r.nextDouble()/1000);

	/*
	Matrix4 org;
	org.translate(Vector3(1+r.nextDouble()/1000,-1+r.nextDouble()/1000,1+r.nextDouble()/1000));
	org = org * Matrix4ROTzx(0.2 * M_PI);
	cam->setMatrix(org);

	renderSingle3(r, ren, cam);
	*/

	unsigned size = width*height;
	Vector4* img = new Vector4[size];
	Matrix4 org;
	org.translate(Vector3(1+r.nextDouble()/1000,-1+r.nextDouble()/1000,1+r.nextDouble()/1000));
	for (unsigned i = 1;i<=480;i++) {//120
		/*
		cam->org.x=i*10/60.0+0.00001;
		if (cam->org.x > 10) cam->org.x-=20;
		*/
		Matrix4 org2 = org * Matrix4ROTxy(i*(M_PI * 2 / 480));
		cam->setMatrix(org2);


		renderImage(width, height, subPixelSize, cam, ren, img, r);

		cout << "writing img..." << endl;

		std::stringstream ss;
		ss << "../tests/video/raw/img" << i;
		string file = ss.str();
		imgType->saveFile(file + imgType->getExtention(), width, height, img);
		if (system(("/usr/local/bin/ffmpeg -loglevel quiet -y -i "+ file+imgType->getExtention() + " " + file + ".png").c_str()) != -1) {
			system(("rm "+file+imgType->getExtention()).c_str());
		}

		cout << "done writing." << endl;
	}
	delete[] img;
	cout << "done all." << endl;

/*
	//217 107
	cam->setSize(width*subPixelSize, height*subPixelSize);
	Color c = cam->renderPixel(217*2, 107*2, ren, r);
	cout << c.red << ' ' << c.green << ' ' << c.blue;
*/

	//system("ls ..");

	return 0;
}

static int render4dWorldTest() {
	int64_t seed = time(NULL);
	Noise::Random r (seed);

	cout << "!!!Hello World!!!" << endl;

	RenderEnvBasic4* ren = new DebugRenderEnv4(&maxStack4);

	Matirial4* mat = new NormMatirial4();

	ren->addGeromitry(FullGeromitry4(Geromitry4(new AABBcube4(Vector4(0,0,0,0), Vector4(1,1,1,1))), mat));

	//ren->addGeromitry(FullGeromitry4(Geromitry4(new AABBcube4(Vector4(-2,0,0,-.5), Vector4(1,1,1,.5))), mat));

	PerspectiveCamera4* cam = new PerspectiveCamera4();
	cam->Xof = r.nextBool() ? r.nextDouble() : -r.nextDouble();
	cam->Yof = r.nextBool() ? r.nextDouble() : -r.nextDouble();

	//Triangle3 triangles[30];


	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(0, 1);

	Matrix5 org;


	org = org * Matrix5ROTxy(-0.25 * M_PI);//yaw
	org = org * Matrix5ROTyz(-0.25/2 * M_PI);//pitch

	org = org * Matrix5ROTzm(0.2 * M_PI);

	org.translate(Vector4(r.nextDouble()/1000,r.nextDouble()/1000,r.nextDouble()/1000, r.nextDouble()/1000));

	org.translate(Vector4(-2,-2,2,0.5));
	/*
	org = org * Matrix5ROTzx(1/4 * M_PI);
	org = org * Matrix5ROTxy(-0.25 * M_PI);
	org = org * Matrix5ROTyz(-1/4 * M_PI);
	*/

	//org = org * Matrix5ROTzm(0.2 * M_PI);
	cam->setMatrix(org);

	renderSingle4(r, ren, cam);

	return 0;
}
static int render4dWorldTest2() {
	int64_t seed = time(NULL);
	Noise::Random r (seed);

	cout << "!!!Hello World!!!" << endl;

	RenderEnvBasic4* ren = new DebugRenderEnv4(&maxStack4);

	Matirial4* mat = new NormMatirial4();

	ren->addGeromitry(FullGeromitry4(Geromitry4(new AABBcube4(Vector4(0,0,0,0), Vector4(1,1,1,1))), mat));

	//ren->addGeromitry(FullGeromitry4(Geromitry4(new AABBcube4(Vector4(-2,0,0,-.5), Vector4(1,1,1,.5))), mat));

	ren->addGeromitry(FullGeromitry4(Geromitry4(new Sphere4(Vector4(0,0,0,0), 0.5)), mat));


	PerspectiveCamera4* cam = new PerspectiveCamera4();
	cam->Xof = r.nextBool() ? r.nextDouble() : -r.nextDouble();
	cam->Yof = r.nextBool() ? r.nextDouble() : -r.nextDouble();

	//Triangle3 triangles[30];


	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(0, 1);

	unsigned size = width*height;
	Vector4* img = new Vector4[size];
	Vector4 off = Vector4(r.nextDouble()/1000,r.nextDouble()/1000,r.nextDouble()/1000, r.nextDouble()/1000);
	for (unsigned i = 1;i<=120;i++) {//120
		Matrix5 org;

		org = org * Matrix5ROTxy(-0.25 * M_PI);//yaw
		org = org * Matrix5ROTyz(-0.25/2 * M_PI);//pitch

		//org = org * Matrix5ROTzm(i*(M_PI * 2 / 120));
		org = org * Matrix5ROT<0,3>(i*(M_PI * 2 / 120));
		//org = org * Matrix5ROT<1,3>(i*(M_PI * 2 / 120));

		org.translate(Vector4(-2,-2,2,0.5));
		cam->setMatrix(org);


		renderImage4(width, height, subPixelSize, cam, ren, img, r);

		cout << "writing img..." << endl;

		std::stringstream ss;
		ss << "../tests/video/raw/img" << i;
		string file = ss.str();
		imgType->saveFile(file + imgType->getExtention(), width, height, img);
		if (system(("/usr/local/bin/ffmpeg -loglevel quiet -y -i "+ file+imgType->getExtention() + " " + file + ".png").c_str()) != -1) {
			system(("rm "+file+imgType->getExtention()).c_str());
		}

		cout << "done writing." << endl;
	}
	delete[] img;
	cout << "done all." << endl;

	return 0;
}

static int render3dMovie() {
	int64_t seed = time(NULL);
	Noise::Random r (seed);

	cout << "!!!Hello World!!!" << endl;

	RenderEnvBasic3* ren;
	Camera3* cam;

	RenderEnvBasic3** renP = &ren;
	Camera3** camP = &cam;

	createEnvMovie(0.0f, 1, &r, renP, camP);

	renderSingle3(r, ren, cam);

	return 0;
}

static void printMatrix(Matrix4 mat) {
	for (int i1 = 0;i1<4;i1++) {
		for (int i2 = 0;i2<4;i2++) {
			cout << mat.data[i1][i2] << ' ';
		}
		cout << endl;
	}
}

static int render3dMovie2() {
	int64_t seed = time(NULL);
	Noise::Random r (seed);

	cout << "!!!Hello World!!!" << endl;

	Movement::initMovie();

	/*{
		auto data = Movement::readMovement();
		cout << data.mc.head.z << endl;
	}*/
	//cout << goldenRatio4 << endl;

	RenderEnvBasic3* ren;
	Camera3* cam;

	RenderEnvBasic3** renP = &ren;
	Camera3** camP = &cam;

	//float fps = 24;
	//float fpsI = 1/fps;
	//float time = 0;//1
	//unsigned frames = (unsigned)(time*fps);
	//unsigned frames = 48;//20, 48

	{
		int ofsetFrame = startFrame - debugFrameOfset;
		if (ofsetFrame > 1) {
			Movement::skipMovement(ofsetFrame-1);
		}
	}

	unsigned size = width*height;
	Vector4* img = new Vector4[size];
	//for (unsigned i = 0;i<frames;) {
	for (unsigned i = startFrame;i<=endFrame;i++) {
		createEnvMovie(i*fpsI, i, &r, renP, camP);

		/*{//test

			cout << (int) ren->geromitry[ren->geromitry.size()-3].gerom.type << endl;
			VirtualSpace3* v = (VirtualSpace3*) ren->geromitry[ren->geromitry.size()-3].gerom.data;
			Vector3 loc = v->mat.asLoc();
			cout << loc.x << ' ' << loc.y << ' ' <<loc.z << endl;
			printMatrix(v->mat);

			loc = Vector3(-0.5) * v->mat;
			cout << loc.x << ' ' << loc.y << ' ' <<loc.z << endl;
		}*/
		/*{
			for (auto obj : ren->geromitry) {
				//cout << obj.mat << endl;
				cout << obj.gerom.type << ' ' << obj.gerom.data << endl;
				if (obj.gerom.type == 4) {
					VirtualSpace3* v = (VirtualSpace3*) obj.gerom.data;
					//cout << v->mat
					printMatrix(v->mat);
					printMatrix(v->matI);
					cout << endl;
				}
			}
		}*/
		/*{
			PerspectiveCamera3* cam2 = (PerspectiveCamera3*) cam;
			printMatrix(cam2->matrix);
		}

		cout << (long long) imageMovie1 << endl;*/

		//renderSingle3(r, ren, cam);
		renderImage(width, height, subPixelSize, cam, ren, img, r);

		delete ren;
		delete cam;

		//cout << "writing img..." << endl;

		//i++;

		std::stringstream ss;
		//local
		if (isLocal)
			ss << "../tests/video/raw/img" << i;//local
		//mac mini
		else
			ss << "raw/img" << i;//mac mini

		string file = ss.str();
		imgType->saveFile(file + imgType->getExtention(), width, height, img);
		if (system(("/usr/local/bin/ffmpeg -loglevel quiet -y -i "+ file+imgType->getExtention() + " " + file + ".png").c_str()) != -1) {
			system(("rm "+file+imgType->getExtention()).c_str());
		}
		//cout << "done writing." << endl;

		cout << "done frame: " << i << endl;
	}

	Movement::closeMovie();

	cout << "done" << endl;
	return 0;
}


ostream& operator<<(ostream& os, const Movement::VectorM& xyz)
{
    os << '[' << xyz.x << ' ' << xyz.y << ' ' << xyz.z << ']';
    return os;
}
Movement::VectorM rotRL(Movement::VectorM rot) {
	Movement::VectorM out;
	out.x = rot.x / M_PI * 180;
	out.y = rot.y / M_PI * 180;
	out.z = rot.z / M_PI * 180;
	return out;
}
Movement::VectorM colorRL(Movement::VectorM rot) {
	/*
	Movement::VectorM out;
	out.x = rot.x * 255;
	out.y = rot.y * 255;
	out.z = rot.z * 255;
	return out;
	*/
	return rot;
}
void printArm(const Movement::MCArmMovement& arm) {
	cout << "\tMainRot: " << rotRL(arm.mainRot) << endl;
	cout << "\tRot1: " << rotRL(arm.hand1Rot) << endl;
	cout << "\tRot2: " << rotRL(arm.hand2Rot) << endl;
}
static int render3dMovie2ReadOUT() {
	int64_t seed = time(NULL);
	Noise::Random r (seed);

	Movement::initMovie();

	//Movement::skipMovement(endFrame-1);
	Movement::skipMovement(1099);//692, 753, 800

	auto data = Movement::readMovement();
/*
	data.mp.orgLoc.x+=18*2;
	data.mp.orgLoc.y+=14;

	data.mc.orgLoc.y+=-100.8*2;
	data.mp.orgLoc.y+=-100.8*2;
	data.camera.loc.y+=-100.8*2;
	*/
	data.mc.orgLoc.x -= 300;

	//Vector3 ofset = (Vector3(-166, 0, 300)-Vector3(10, -278, 36) + Vector3(1,0,0))/2;
	Vector3 ofset;

	cout << "MC:" << endl;
	cout << "loc:" << (data.mc.orgLoc+ofset) << endl;
	cout << "rot:" << rotRL(data.mc.orgRot) << endl;
	cout << "rotHead:" << rotRL(data.mc.head) << endl;
	cout << "arml" << endl;
	printArm(data.mc.left);
	cout << "armr" << endl;
	printArm(data.mc.right);
	cout << "color:" << colorRL(data.colorMC) << endl;

	cout << "\n\nMP:" << endl;
	cout << "loc:" << (data.mp.orgLoc+ofset) << endl;
	cout << "rot:" << rotRL(data.mp.orgRot) << endl;
	cout << "rotTail:" << rotRL(data.mp.tailRot) << endl;
	cout << "color:" << colorRL(data.colorMP) << endl;

	cout << "\n\nCamara:" << endl;
	cout << "loc:" << (data.camera.loc+ofset) << endl;
	//cout << "rot:" << rotRL(data.camera.rot) << endl;
	cout << "rot:" << data.camera.rot << endl;

	Movement::closeMovie();
	return 0;
}

namespace MovementEditor {

static Movement::MovementTotal readMovement(ifstream* isMovement) {
	union {
		Movement::MovementTotal data;
		char raw[sizeof(Movement::MovementTotal)];
	} input;
	isMovement->read (input.raw,sizeof(Movement::MovementTotal));
	return input.data;
}

//movementEditor
void buildMovement() {
	ofstream fileOutput("/Volumes/Christiaans Drive/anders/school/ckv/2/animation/movement.mvb", ios::binary);

	ifstream file1("/Volumes/Christiaans Drive/anders/school/ckv/2/animation/movement sene1.mvb", ios::binary);
	ifstream file2("/Volumes/Christiaans Drive/anders/school/ckv/2/animation/movement sene2.mvb", ios::binary);
	ifstream file3("/Volumes/Christiaans Drive/anders/school/ckv/2/animation/movement sene3.mvb", ios::binary);

	for (unsigned i = 1;i<=1100;i++) {
		union {
			Movement::MovementTotal data;
			char raw[sizeof(Movement::MovementTotal)];
		} input;

		if (i <= 460) {
			auto v1 = readMovement(&file1);
			//auto v2 = readMovement(&file2);
			input.data = v1;

		} else if (i < 692) {
			auto v2 = readMovement(&file2);
			input.data = v2;

		} else if (i < 753) {
			auto v2 = readMovement(&file2);
			auto v3 = readMovement(&file3);
			input.data = v2;
			input.data.mc = v3.mc;
			input.data.colorMC = v3.colorMC;

		} else if (i <= 800) {
			auto v2 = readMovement(&file2);
			auto v3 = readMovement(&file3);
			input.data = v3;
			input.data.camera = v2.camera;

		} else if (i <= 1100) {
			auto v3 = readMovement(&file3);
			input.data = v3;

		}

		fileOutput.write (input.raw,sizeof(Movement::MovementTotal));
	}
}
//692, 753, 800
}

inline double slope1(double v) {
	return v/(v+1);//1-1/(v+1);
}


static int render4dMovie2(int argc, char **argv) {
	if (argc <= 1)
		return -2;

	int64_t seed = time(NULL);
	Noise::Random r (seed);
	Noise::Random r2 (95669l);

	cout << "!!!Hello World!!!" << endl;

	MovRenderEnv4* ren = new MovRenderEnv4(&maxStack4_2);
	Camera4* cam = new PerspectiveCamera4();

	Vector4 of = Vector4(r.nextDouble()/1000,r.nextDouble()/1000,r.nextDouble()/1000,r.nextDouble()/1000);

	double firsts[7];
	for (int i = 0;i<7;i++) firsts[i] = (r2.nextDouble()+0.5)/2;
	double firsts2[7];
	for (int i = 0;i<7;i++) firsts2[i] = r2.nextDouble()+0.5;

	for (int i = 0;i<16*3;i++) {
		double min = r2.nextDouble();
		double max = r2.nextDouble();
		if (max < min) {
			min = r2.nextDouble();
			max = min;
		}

		colorSetupSV[i].off = (min+max)/2;
		colorSetupSV[i].cosStrengt = (max-min)/2;

		colorSetupSV[i].cosSpeed = r2.nextDouble()*3+0.5;
		colorSetupSV[i].cosTimeOff = r2.nextDouble()*6;
	}

	int i;
	sscanf(argv[1], "%d", &i);

	unsigned size = width*height;
	Vector4* img = new Vector4[size];
	//for (unsigned i = 0;i<frames;) {
	//for (unsigned i = startFrame;i<=endFrame;i++) {
		double time = i*fpsI;

		setupColor4d(time);
		//createEnvMovie4(i*fpsI, i, &r, renP, camP);

		/*{//test

			cout << (int) ren->geromitry[ren->geromitry.size()-3].gerom.type << endl;
			VirtualSpace3* v = (VirtualSpace3*) ren->geromitry[ren->geromitry.size()-3].gerom.data;
			Vector3 loc = v->mat.asLoc();
			cout << loc.x << ' ' << loc.y << ' ' <<loc.z << endl;
			printMatrix(v->mat);

			loc = Vector3(-0.5) * v->mat;
			cout << loc.x << ' ' << loc.y << ' ' <<loc.z << endl;
		}*/
		/*{
			for (auto obj : ren->geromitry) {
				//cout << obj.mat << endl;
				cout << obj.gerom.type << ' ' << obj.gerom.data << endl;
				if (obj.gerom.type == 4) {
					VirtualSpace3* v = (VirtualSpace3*) obj.gerom.data;
					//cout << v->mat
					printMatrix(v->mat);
					printMatrix(v->matI);
					cout << endl;
				}
			}
		}*/
		/*{
			PerspectiveCamera3* cam2 = (PerspectiveCamera3*) cam;
			printMatrix(cam2->matrix);
		}

		cout << (long long) imageMovie1 << endl;*/

		//renderSingle3(r, ren, cam);

		Matrix5 matrx = Matrix5ROT<0,2>(M_PI/2);
/*
		matrx = matrx * Matrix5ROT<0,1>(r.nextDouble()*M_PI*2);
		matrx = matrx * Matrix5ROT<1,2>(r.nextDouble()*M_PI*2);
		matrx = matrx * Matrix5ROT<2,3>(r.nextDouble()*M_PI*2);
		matrx = matrx * Matrix5ROT<3,0>(r.nextDouble()*M_PI*2);
*/
		matrx = matrx * Matrix5ROT<0,1>(slope1(time*firsts[0])*firsts2[0] * time);
		matrx = matrx * Matrix5ROT<0,2>(slope1(time*firsts[1])*firsts2[1] * time);
		matrx = matrx * Matrix5ROT<0,3>(slope1(time*firsts[2])*firsts2[2] * time);
		matrx = matrx * Matrix5ROT<1,2>(slope1(time*firsts[3])*firsts2[3] * time);
		matrx = matrx * Matrix5ROT<1,3>(slope1(time*firsts[4])*firsts2[4] * time);
		matrx = matrx * Matrix5ROT<2,3>(slope1(time*firsts[5])*firsts2[5] * time);
		matrx = matrx * Matrix5ROT<3,0>(slope1(time*firsts[6])*firsts2[6] * time);

		//matrx.translate(of+Vector4(1,1,1,3));
		matrx.translate(of+Vector4(time*time*4,-0.8-time*time,0,0));
		cam->setMatrix(matrx);


		renderImage4(width, height, subPixelSize, cam, ren, img, r);

		//clean
		/*
		while (!move4dDelete.empty()) {
			Geromitry4* first = move4dDelete.front();
			move4dDelete.pop_front();
			delete first;
		}*/

		//cout << "writing img..." << endl;

		//i++;

		std::stringstream ss;
		//local
		if (isLocal)
			ss << "../tests/video/raw2/img" << i;//local
		//mac mini
		else
			ss << "raw2/img" << i;//mac mini

		string file = ss.str();
		imgType->saveFile(file + imgType->getExtention(), width, height, img);
		if (system(("/usr/local/bin/ffmpeg -loglevel quiet -y -i "+ file+imgType->getExtention() + " " + file + ".png").c_str()) != -1) {
			system(("rm "+file+imgType->getExtention()).c_str());
		}
		//cout << "done writing." << endl;

		cout << "done frame: " << i << endl;
	//}

	cout << "done" << endl;
	return 0;
}


int main(int argc, char **argv) {
	/*
	if (isLocal)
		MovementEditor::buildMovement();
	//return render3dWorldTest();
	//return render4dWorldTest2();

	return render3dMovie2();
	//return render3dMovie2ReadOUT();
	//return 0;
	 */

	return render4dMovie2(argc, argv);
}
