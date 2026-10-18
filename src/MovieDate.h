/*
 * MovieDate.h
 *
 *  Created on: 12 Feb 2020
 *      Author: christiaan
 */

#ifndef MOVIEDATE_H_
#define MOVIEDATE_H_

#include <iostream>
#include "noise/Random.h"
#include "Ray.h"
#include "Matrix.h"

#include "rendrables/Triangle.h"
#include "rendrables/Sphere.h"
#include "rendrables/AABBcube.h"
#include "rendrables/VirtualSpace.h"

#include "matirials/DirectColorMatirial.h"
#include "matirials/DebugMatirial.h"
#include "matirials/MirorMatirial.h"
#include "matirials/AddMatirial.h"
#include "matirials/CubeOutlineMatirial.h"
#include "matirials/LightsMatirial.h"
#include "matirials/CustomMatirial.h"
#include "matirials/ImageMatirial.h"

#include "matirials/nonedian/SphereEdgeNonedianMatirial.h"
#include "matirials/nonedian/AABBcubeInsideNonedianMatirial.h"
#include "matirials/nonedian/MatrixNonedianMatirial.h"

#include "renderEnviroments/DebugRenderEnv.h"
#include "renderEnviroments/TransparantRenderEnv.h"
#include "renderEnviroments/FogRenderEnv.h"

#include "cameras/PerspectiveCameraNoise.h"
#include "cameras/PerspectiveCamera.h"

#include "imagefiletypes/PPM.h"
#include "imagefiletypes/Raw255ImgDump.h"

#include <random>
#include <algorithm>
#include <cmath>
#include <string>
#include <thread>
#include <time.h>
#include <stdio.h>
#include <sstream>
#include <forward_list>
using namespace std;

const static bool isLocal = false;

const static double goldenRatio = (1+sqrt(5))/2;
const static double goldenRatio2 = goldenRatio/(goldenRatio+1);//0.618034

const static double goldenRatio3 = goldenRatio2/2+0.5;//0.809017
const static double goldenRatio4 = (goldenRatio2+goldenRatio3)/2;//0.713525
bool maxStack(RenderStackFrame3* frame) {
	return frame->rayCount <= 10 && frame->noEndian <= 100 && frame->value > 0.0004;//10, 0.0004
	//return frame->allRayCount <= 40;
}
bool maxStack4(RenderStackFrame4* frame) {
	return frame->rayCount <= 10 && frame->noEndian <= 100 && frame->value > 0.0004;//10, 0.0004
	//return frame->allRayCount <= 40;
}

bool maxStack4_2(RenderStackFrame4* frame) {
	return frame->rayCount <= 100 && frame->value > 0.0004;//10, 0.0004
	//return frame->allRayCount <= 40;
}


//Matirial3* caracterBaceColor = new NormMatirial3();
DirectNormColorMatirial3* caracterBaceColor = new DirectNormColorMatirial3(Color(0.8, 0.8, 0.8));//0.6
DirectNormColorMatirial3* MisterPColor = new DirectNormColorMatirial3(Color(0, 1, 0));
DirectNormColorMatirial3* MCColor = new DirectNormColorMatirial3(Color(0, 1, 0));

DirectNormColorMatirial3* WallColor = new DirectNormColorMatirial3(Color(0.5, 0.5, 0.5));//0.4

static ImageMatirial3* imageMovie1Mat = new ImageMatirial3((Matrix4(Vector3(1/1.6/2,1.0/2,1.0/2)) * inverse(Matrix4().translate(Vector3(-6.22523, 15, 10)/2))).translateSafe(Vector3(1.6, 0, 1)), nullptr);//TODO

static void addMisterP(Matrix4 org, Matrix4 tailRot, RenderEnvBasic3* ren) {
	org = org * Matrix4(Vector3(1,goldenRatio3,goldenRatio2)) * Matrix4(0.25);

	Matrix4 orgI = inverse(org);
	ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-1,-1,-1), Vector3(1,1,1))), org, orgI)), caracterBaceColor));
	ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-0.9,0.9,-0.9), Vector3(0.9,1.1,0.9))), org, orgI)), MisterPColor));
	//ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(Vector3(-1,-1,-1), Vector3(1,1,1))), caracterBaceColor));

	Matrix4 tail = Matrix4();
	//tail.translate(Vector3(0,-1,0));
	tail = Matrix4(goldenRatio4) * tail;
	tail = tail * tailRot;

	//double s = -1/2.0;
	for (int i = 0;i<3;i++) {
		org = org * tail;
		//org.translateSafe(Vector3(0,s,0));
		org.translateSafe(Vector3(0,-2-goldenRatio3,0));
		//s *= goldenRatio3;
		Matrix4 orgI = inverse(org);
		ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-1,-1,-1), Vector3(1,1,1))), org, orgI)), caracterBaceColor));
	}
}

static void addMC(Matrix4 org, Matrix4 headRot, Matrix4 larm, Matrix4 lhand1, Matrix4 lhand2, Matrix4 rarm, Matrix4 rhand1, Matrix4 rhand2, RenderEnvBasic3* ren) {

	ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-1,-1,-1), Vector3(1,1,1))), org, inverse(org))), caracterBaceColor));

	{//head
		Matrix4 head = org;
		head.translateSafe(Vector3(0,0,2));
		head = head * headRot;
		Matrix4 headI = inverse(head);
		ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-0.5,-0.5,-0.5), Vector3(0.5,0.5,0.5))), head, headI)), caracterBaceColor));
		ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-0.4,-0.4,-0.4), Vector3(0.4,0.6,0.4))), head, headI)), MCColor));
	}

	{//Larm
		Matrix4 arm = org;
		arm.translateSafe(Vector3(-1.3,0,0.2));
		arm = arm * larm;
		ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-0.1,0,-0.08), Vector3(0.1,2,0.08))), arm, inverse(arm))), caracterBaceColor));

		arm.translateSafe(Vector3(0,2.05,0));
		arm = arm * lhand1;
		arm.translateSafe(Vector3(0,0.05,0));
		ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-0.25,0,-0.08), Vector3(0.25,0.3,0.08))), arm, inverse(arm))), caracterBaceColor));

		arm.translateSafe(Vector3(0,0.35,0));
		arm = arm * lhand2;
		arm.translateSafe(Vector3(0,0.05,0));
		arm = arm * lhand2;
		arm.translateSafe(Vector3(0,0.05,0));
		ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-0.25,0,-0.08), Vector3(0.25,0.3,0.08))), arm, inverse(arm))), caracterBaceColor));
	}

	{//Larm
		Matrix4 arm = org;
		arm.translateSafe(Vector3(1.3,0,0.2));
		arm = arm * rarm;
		ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-0.1,0,-0.08), Vector3(0.1,2,0.08))), arm, inverse(arm))), caracterBaceColor));

		arm.translateSafe(Vector3(0,2.05,0));
		arm = arm * rhand1;
		arm.translateSafe(Vector3(0,0.05,0));
		ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-0.25,0,-0.08), Vector3(0.25,0.3,0.08))), arm, inverse(arm))), caracterBaceColor));

		arm.translateSafe(Vector3(0,0.35,0));
		arm = arm * rhand2;
		arm.translateSafe(Vector3(0,0.05,0));
		arm = arm * rhand2;
		arm.translateSafe(Vector3(0,0.05,0));
		ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(Vector3(-0.25,0,-0.08), Vector3(0.25,0.3,0.08))), arm, inverse(arm))), caracterBaceColor));
	}
}

namespace RoomLoader {

	Matirial3* loadedMatirials[6] = {WallColor,
			imageMovie1Mat,//tv1
			new DirectNormColorMatirial3(Color(1/1.5, 0.925/1.5, 0.063/1.5)),
			new DirectNormColorMatirial3(Color(0.8/1.5, 0.29/1.5, 0/1.5)),

			//sene3
			new MatrixNonedianMatirial3(Matrix4().translate((Vector3(7.83296, -0.007323, 1051.37) - Vector3(-78.2897, 0.124183, 316) + Vector3(1,0,0))/2)),
			new MatrixNonedianMatirial3(Matrix4().translate((Vector3(-78.2897, 0.124183, 316) - Vector3(7.83296, -0.007323, 1051.37) + Vector3(1,0,0))/2)),
			//new MatrixNonedianMatirial3(Matrix4().translate((Vector3(7.83296, -0.007323, 1051.37) - Vector3(-78.2897, 0.124183, 316) + Vector3(1,0,0))/2)),
	};
	//-78.2897m, 0.124183m, 316m
	//7.83296m, -0.007323m, 1051.37m

	struct __attribute__((packed)) VectorM {
		double x;
		double y;
		double z;
	};
	struct __attribute__((packed)) ObjPacked {
		unsigned char matirial;
		/*
		VectorM loc;
		VectorM rot;
		VectorM scale;
		*/
		double matrix[4*4];
	};
	union InputValue {
		ObjPacked data;
		char raw[sizeof(ObjPacked)];
	};

	static Matrix4 iRot(VectorM xyz) {
		return (Matrix4ROTyz(xyz.x) * Matrix4ROTzx(xyz.y)) * Matrix4ROTxy(xyz.z);
	}
	static Vector3 iLoc(VectorM xyz) {
		return Vector3(xyz.x/2, xyz.y/2, xyz.z/2);
	}

	class BuilderOBJ {
	public:
		bool addMatrix;
		Matirial3* mat;
		Matrix4* matrix;
		Vector3 loc1;
		Vector3 loc2;
		inline BuilderOBJ(ObjPacked input) {
			mat = loadedMatirials[input.matirial];
/*
			addMatrix = !iLoc(input.rot).isEmpty();
			if (addMatrix) {
				Vector3 size = iLoc(input.scale).abs();
				loc1 = Vector3()-size;
				loc2 = size;
				Matrix4 mat = iRot(input.rot);
				mat.translate(iLoc(input.loc));
				matrix = new Matrix4(mat);
			} else {
				matrix = nullptr;
				Vector3 of = iLoc(input.loc);
				Vector3 size = iLoc(input.scale);
				Vector3 com1 = of - size;
				Vector3 com2 = of + size;
				loc1 = com1.min(com2);
				loc2 = com1.max(com2);
			}*/
			Matrix4 matrix (input.matrix);
			addMatrix = !matrix.noRot();
			if (addMatrix) {
				//this->matrix = new Matrix4(Matrix4(2)*matrix);
				this->matrix = new Matrix4(Matrix4(.5)*matrix);
				loc1 = Vector3(-1);
				loc2 = Vector3(1);
			} else {
				this->matrix = nullptr;
				Vector3 of = matrix.asLoc();
				Vector3 size = matrix.asScale();
				Vector3 com1 = of - size;
				Vector3 com2 = of + size;
				loc1 = com1.min(com2);
				loc2 = com1.max(com2);

				loc1 = loc1/2;
				loc2 = loc2/2;
			}
		}
		inline BuilderOBJ(const BuilderOBJ& copy) : addMatrix(copy.addMatrix), mat(copy.mat), loc1(copy.loc1), loc2(copy.loc2) {
			matrix = copy.addMatrix ? new Matrix4(copy.matrix->data) : nullptr;
		}
		inline ~BuilderOBJ() {
			if (addMatrix)
				delete matrix;
		}
	};

	static std::vector<BuilderOBJ> loadRoom(const char* file) {
		ifstream is(file, ios::binary);
		is.seekg (0, ios::end);
		unsigned length = is.tellg()/sizeof(ObjPacked);
		is.seekg (0, ios::beg);
		std::vector<BuilderOBJ> out;

		for (unsigned headP = 0;headP < length;headP++) {

			InputValue input;
			is.read (input.raw,sizeof(ObjPacked));

			BuilderOBJ obj (input.data);
			out.push_back(obj);
		}

		is.close();

		return out;
	}
	static std::vector<BuilderOBJ> loadRoom2(std::string file) {
		if (isLocal)
			file = "/Volumes/Christiaans Drive/anders/school/ckv/2/animation/" + file;//local
		return loadRoom(file.c_str());
	}


	static void importRoom(std::vector<BuilderOBJ> roomData, RenderEnvBasic3* ren) {
		for (BuilderOBJ obj : roomData) {
			if (obj.addMatrix) {
				Matrix4 mat = *(obj.matrix);
				ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(obj.loc1, obj.loc2)), mat, inverse(mat))), obj.mat));
			}
			else
				ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(obj.loc1, obj.loc2)), obj.mat));
		}
	}

	static void importRoom(std::vector<BuilderOBJ> roomData, RenderEnvBasic3* ren, Vector3 ofset) {
		for (BuilderOBJ obj : roomData) {
			if (obj.addMatrix) {
				Matrix4 mat = *(obj.matrix);

				ren->addGeromitry(FullGeromitry3(Geromitry3(new VirtualSpace3(Geromitry3(new AABBcube3(obj.loc1+ofset, obj.loc2+ofset)), mat, inverse(mat))), obj.mat));
			}
			else
				ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(obj.loc1+ofset, obj.loc2+ofset)), obj.mat));
		}
	}

	std::vector<BuilderOBJ> rooms[5] = {loadRoom2("room1.rmb"),
			loadRoom2("room2.rmb"),
			loadRoom2("room3.rmb"),
			loadRoom2("room4.rmb"),
			loadRoom2("roomRocked.rmb")};

}

static void addRoom1(float time, RenderEnvBasic3* ren) {
	importRoom(RoomLoader::rooms[0], ren);
}

static std::vector<Matirial3*> toDelete;
static MatrixNonedianMatirial3* makeMatrixNonedianMatirial3(Matrix4 mat) {
	MatrixNonedianMatirial3* out = new MatrixNonedianMatirial3(mat);
	toDelete.push_back(out);
	return out;
}

static void makeNonCube(Vector3 loc1, Vector3 loc2, Vector3 nonOfset, RenderEnvBasic3* ren) {
	Matrix4 mat;// = Matrix4(Vector3(-1,1,1));
	mat.translate(nonOfset/2);
	ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(loc1.min(loc2)/2, loc1.max(loc2)/2)), makeMatrixNonedianMatirial3(mat)));
}

static void addRoom2(float time, RenderEnvBasic3* ren) {
	importRoom(RoomLoader::rooms[1], ren);

	makeNonCube(Vector3(-20,-55,0), Vector3(-18,-39,32), Vector3(18*2,14,0), ren);
	makeNonCube(Vector3(20,-55+14,0), Vector3(18,-39+14,32), Vector3(-18*2,-14,0), ren);

	makeNonCube(Vector3(12+0.8,-59+0.8,0), Vector3(-12-0.8,-59-0.8,32), Vector3(0,-259-0.8,0)-Vector3(0,-59+0.8,0), ren);
}

Matirial3* noneMatRoom3_0 = new AABBcubeInsideNonedianMatirial3();
static void addRoom3(float time, RenderEnvBasic3* ren) {
	importRoom(RoomLoader::rooms[2], ren);

	{
		Vector3 loc1 (0-150+33/2,-278-150,0-150);
		Vector3 loc2 (0+150+33/2,-278+150,0+150);
		ren->addGeromitry(FullGeromitry3(Geromitry3(new AABBcube3(loc1.min(loc2)/2, loc1.max(loc2)/2)), noneMatRoom3_0));
	}

	makeNonCube(Vector3(12+0.8,-59+0.8-200,0.1), Vector3(-12-0.8,-59-0.8-200,32-0.1), Vector3(0,-59+0.8,0)-Vector3(0,-259-0.8,0), ren);

	//makeNonCube(Vector3(-20,-55,0), Vector3(-18,-39,32), Vector3(17*2,14,0), ren);
	//makeNonCube(Vector3(20,-55+14,0), Vector3(18,-39+14,32), Vector3(-17*2,-14,0), ren);

	//makeNonCube(Vector3(10 + 0.5, -278 + 8, 36 + 20), Vector3(10 - 0.5, -278 - 8, 36), Vector3(-166, 0, 300)-Vector3(10, -278, 36) + Vector3(1,0,0), ren);
	makeNonCube(Vector3(10, -278 + 8, 36 + 20), Vector3(10 - 0.5, -278 - 8, 36), Vector3(-166, 0, 300)-Vector3(10, -278, 36) + Vector3(1,0,0), ren);

//{(166, 0, 300) doorOut} {(10, -278, 36) doorIn}
}

static void addRoom4(float time, RenderEnvBasic3* ren) {
	importRoom(RoomLoader::rooms[3], ren);
}

static void addRocked(float time, RenderEnvBasic3* ren) {
	if (time <= 45.833)
		importRoom(RoomLoader::rooms[4], ren);
	else if (time <= 50) {
		importRoom(RoomLoader::rooms[4], ren, Vector3(0,0,(time-45.833)*150));
	} else {
		importRoom(RoomLoader::rooms[4], ren, Vector3(0,(time-50)*500,(time-45.833)*150));
	}
}

//TODO room3 door
//fix room3-stairs bug


namespace Movement {
	ifstream* isMovement;

	static void initMovie() {
		//local
		if (isLocal)
			isMovement = new ifstream("/Volumes/Christiaans Drive/anders/school/ckv/2/animation/movement.mvb", ios::binary );
		//mac mini
		else
			isMovement = new ifstream("movement.mvb", ios::binary );
	}
	static void closeMovie() {
		isMovement->close();
	}

	struct __attribute__((packed)) VectorM {
		double x;
		double y;
		double z;
	};
	inline VectorM operator+(VectorM lhs, const Vector3& rhs) {
		VectorM out = {lhs.x+rhs.x*2, lhs.y+rhs.y*2, lhs.z+rhs.z*2};
		return out;
	}
	struct __attribute__((packed)) MCArmMovement {
		VectorM mainRot;
		VectorM hand1Rot;
		VectorM hand2Rot;
	};
	struct __attribute__((packed)) MCMovement {
		VectorM orgLoc;
		VectorM orgRot;
		VectorM head;
		MCArmMovement left;
		MCArmMovement right;
	};
	struct __attribute__((packed)) MPMovement {
		VectorM orgLoc;
		VectorM orgRot;
		VectorM tailRot;
	};
	struct __attribute__((packed)) CameraMovement {
		VectorM loc;
		VectorM rot;
	};
	struct __attribute__((packed)) MovementTotal {
		MCMovement mc;
		MPMovement mp;
		CameraMovement camera;
		VectorM colorMC;
		VectorM colorMP;
	};

	static MovementTotal readMovement() {
		union {
			MovementTotal data;
			char raw[sizeof(MovementTotal)];
		} input;
		isMovement->read (input.raw,sizeof(MovementTotal));
		return input.data;
	}
	static void skipMovement(unsigned cound) {
		isMovement->ignore(cound*sizeof(MovementTotal));
	}

	static Matrix4 iRot(VectorM xyz) {
		//return (Matrix4ROTyz(xyz.x) * Matrix4ROTzx(xyz.y)) * Matrix4ROTxy(xyz.z);
		return Matrix4ROTxy(xyz.z) * (Matrix4ROTzx(xyz.y) * Matrix4ROTyz(xyz.x));
	}
	static Vector3 iLoc(VectorM xyz) {
		return Vector3(xyz.x/2, xyz.y/2, xyz.z/2);
	}
	static Matrix4 iLocRot(VectorM loc, VectorM rot) {
		Matrix4 out = iRot(rot);
		out.translate(iLoc(loc));
		return out;
	}
	static Color iColor(VectorM xyz) {
		return Color(xyz.x, xyz.y, xyz.z);
	}

	static void putCharacters(RenderEnvBasic3* ren, Camera3* cam, Noise::Random* r) {
		MovementTotal data = readMovement();

		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot), iRot(data.mp.tailRot), ren);

		{//camera
			VectorM rot = data.camera.rot;
			rot.x-=M_PI*0.5;
			cam->setMatrix(iLocRot(data.camera.loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
			/*
			Matrix4 out = (Matrix4ROTyz(data.camera.rot.x-M_PI*0.5) * Matrix4ROTzx(data.camera.rot.y)) * Matrix4ROTxy(data.camera.rot.z);
			out.translate(iLoc(data.camera.loc));
			out.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
			cam->setMatrix(out);
			*/
		}

		{
			MCColor->color = iColor(data.colorMC);
			MisterPColor->color = iColor(data.colorMP);
		}
	}

	static void putCharactersLone(MovementTotal data, RenderEnvBasic3* ren, Camera3* cam, Noise::Random* r) {
		{//camera
			VectorM rot = data.camera.rot;
			rot.x-=M_PI*0.5;
			cam->setMatrix(iLocRot(data.camera.loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
			/*
			Matrix4 out = (Matrix4ROTyz(data.camera.rot.x-M_PI*0.5) * Matrix4ROTzx(data.camera.rot.y)) * Matrix4ROTxy(data.camera.rot.z);
			out.translate(iLoc(data.camera.loc));
			out.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
			cam->setMatrix(out);
			*/
		}

		{
			MCColor->color = iColor(data.colorMC);
			MisterPColor->color = iColor(data.colorMP);
		}
	}

	static void putCharacters(MovementTotal data, RenderEnvBasic3* ren, Camera3* cam, Noise::Random* r) {
		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot), iRot(data.mp.tailRot), ren);

		{//camera
			VectorM rot = data.camera.rot;
			rot.x-=M_PI*0.5;
			cam->setMatrix(iLocRot(data.camera.loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
			/*
			Matrix4 out = (Matrix4ROTyz(data.camera.rot.x-M_PI*0.5) * Matrix4ROTzx(data.camera.rot.y)) * Matrix4ROTxy(data.camera.rot.z);
			out.translate(iLoc(data.camera.loc));
			out.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
			cam->setMatrix(out);
			*/
		}

		{
			MCColor->color = iColor(data.colorMC);
			MisterPColor->color = iColor(data.colorMP);
		}
	}

	static void putCharactersPC(RenderEnvBasic3* ren, Camera3* cam, Noise::Random* r, Vector3 duplicate) {
		MovementTotal data = readMovement();

		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot).translate(duplicate), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot), iRot(data.mp.tailRot), ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot).translate(duplicate), iRot(data.mp.tailRot), ren);

		{//camera
			VectorM rot = data.camera.rot;
			rot.x-=M_PI*0.5;
			cam->setMatrix(iLocRot(data.camera.loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
			/*
			Matrix4 out = (Matrix4ROTyz(data.camera.rot.x-M_PI*0.5) * Matrix4ROTzx(data.camera.rot.y)) * Matrix4ROTxy(data.camera.rot.z);
			out.translate(iLoc(data.camera.loc));
			out.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
			cam->setMatrix(out);
			*/
		}

		{
			MCColor->color = iColor(data.colorMC);
			MisterPColor->color = iColor(data.colorMP);
		}
	}

	static void putCharactersPC(MovementTotal data, RenderEnvBasic3* ren, Camera3* cam, Noise::Random* r, Vector3 duplicate) {
		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot).translate(duplicate), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot), iRot(data.mp.tailRot), ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot).translate(duplicate), iRot(data.mp.tailRot), ren);

		{//camera
			VectorM rot = data.camera.rot;
			rot.x-=M_PI*0.5;
			cam->setMatrix(iLocRot(data.camera.loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
			/*
			Matrix4 out = (Matrix4ROTyz(data.camera.rot.x-M_PI*0.5) * Matrix4ROTzx(data.camera.rot.y)) * Matrix4ROTxy(data.camera.rot.z);
			out.translate(iLoc(data.camera.loc));
			out.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
			cam->setMatrix(out);
			*/
		}

		{
			MCColor->color = iColor(data.colorMC);
			MisterPColor->color = iColor(data.colorMP);
		}
	}

	static void putCharactersP(RenderEnvBasic3* ren, Camera3* cam, Noise::Random* r, Vector3 duplicate) {
		MovementTotal data = readMovement();

		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot), iRot(data.mp.tailRot), ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot).translate(duplicate), iRot(data.mp.tailRot), ren);

		{//camera
			VectorM rot = data.camera.rot;
			rot.x-=M_PI*0.5;
			cam->setMatrix(iLocRot(data.camera.loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
			/*
			Matrix4 out = (Matrix4ROTyz(data.camera.rot.x-M_PI*0.5) * Matrix4ROTzx(data.camera.rot.y)) * Matrix4ROTxy(data.camera.rot.z);
			out.translate(iLoc(data.camera.loc));
			out.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
			cam->setMatrix(out);
			*/
		}

		{
			MCColor->color = iColor(data.colorMC);
			MisterPColor->color = iColor(data.colorMP);
		}
	}

	static void putCharactersOP(RenderEnvBasic3* ren, Camera3* cam, Noise::Random* r, Vector3 duplicate) {
		MovementTotal data = readMovement();

		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot).translate(duplicate), iRot(data.mp.tailRot), ren);

		{//camera
			VectorM rot = data.camera.rot;
			rot.x-=M_PI*0.5;
			cam->setMatrix(iLocRot(data.camera.loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
			/*
			Matrix4 out = (Matrix4ROTyz(data.camera.rot.x-M_PI*0.5) * Matrix4ROTzx(data.camera.rot.y)) * Matrix4ROTxy(data.camera.rot.z);
			out.translate(iLoc(data.camera.loc));
			out.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
			cam->setMatrix(out);
			*/
		}

		{
			MCColor->color = iColor(data.colorMC);
			MisterPColor->color = iColor(data.colorMP);
		}
	}

	static void putCharactersC(RenderEnvBasic3* ren, Camera3* cam, Noise::Random* r, Vector3 duplicate) {
		MovementTotal data = readMovement();

		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot).translate(duplicate), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot), iRot(data.mp.tailRot), ren);

		{//camera
			VectorM rot = data.camera.rot;
			rot.x-=M_PI*0.5;
			cam->setMatrix(iLocRot(data.camera.loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
			/*
			Matrix4 out = (Matrix4ROTyz(data.camera.rot.x-M_PI*0.5) * Matrix4ROTzx(data.camera.rot.y)) * Matrix4ROTxy(data.camera.rot.z);
			out.translate(iLoc(data.camera.loc));
			out.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
			cam->setMatrix(out);
			*/
		}

		{
			MCColor->color = iColor(data.colorMC);
			MisterPColor->color = iColor(data.colorMP);
		}
	}

	static void putCharactersOC(RenderEnvBasic3* ren, Camera3* cam, Noise::Random* r, Vector3 duplicate) {
		MovementTotal data = readMovement();

		addMC(iLocRot(data.mc.orgLoc, data.mc.orgRot).translate(duplicate), iRot(data.mc.head),
				iRot(data.mc.left.mainRot), iRot(data.mc.left.hand1Rot), iRot(data.mc.left.hand2Rot),
				iRot(data.mc.right.mainRot), iRot(data.mc.right.hand1Rot), iRot(data.mc.right.hand2Rot),
				ren);

		addMisterP(iLocRot(data.mp.orgLoc, data.mp.orgRot), iRot(data.mp.tailRot), ren);

		{//camera
			VectorM rot = data.camera.rot;
			rot.x-=M_PI*0.5;
			cam->setMatrix(iLocRot(data.camera.loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
			/*
			Matrix4 out = (Matrix4ROTyz(data.camera.rot.x-M_PI*0.5) * Matrix4ROTzx(data.camera.rot.y)) * Matrix4ROTxy(data.camera.rot.z);
			out.translate(iLoc(data.camera.loc));
			out.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
			cam->setMatrix(out);
			*/
		}

		{
			MCColor->color = iColor(data.colorMC);
			MisterPColor->color = iColor(data.colorMP);
		}
	}
}

static void* imageMovie1 = nullptr;
static void loadImageMovie1(unsigned frame) {
	std::stringstream ss;
	if (isLocal)
		ss << "/Volumes/Christiaans Drive/anders/school/ckv/2/videoORG1/img" << frame << ".ppm";//local
	else
		ss << "videoORG1/img" << frame << ".ppm";//mini
	std::string file = ss.str();
	imageMovie1 = PPMfileType->loadFile(file);

	//cout << file << endl;

	if (imageMovie1 != nullptr) {
		if (imageMovie1Mat->image != nullptr)
			deleteImage<charColor>(imageMovie1Mat->image);
		imageMovie1Mat->image = imageMovie1;
	}

	else if (imageMovie1Mat->image == nullptr) {//debug
		imageMovie1Mat->image = createImage<charColor>(1,1);
	}
}

static void createEnvMovie(float time, unsigned frame, Noise::Random* r, RenderEnvBasic3** env, Camera3** camP) {
	RenderEnvBasic3* ren = new FogRenderEnv3(&maxStack, Color(0), 0.001);//0.003, 0,001
	*env = ren;
	PerspectiveCamera3* cam = new PerspectiveCamera3();
	*camP = cam;
	cam->Xof = r->nextBool() ? r->nextDouble() : -r->nextDouble();
	cam->Yof = r->nextBool() ? r->nextDouble() : -r->nextDouble();
/*
	{
		//Matrix4 org = Matrix4ROTxy(M_PI+time*(M_PI * 2));
		//Matrix4 org = Matrix4();
		Matrix4 org = Matrix4ROTxy(M_PI);

		org.translateSafe(Vector3(0,-6,2));
		//org = Matrix4ROTxy(time*(M_PI * 2)) * org;

		org.translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000));
		cam->setMatrix(org);
	}
*/
	/*{
		//Matrix4 org = Matrix4();
		Matrix4 org = Matrix4ROTxy(time*(M_PI * 2));
		org.translateSafe(Vector3(0,0,cos(time*(M_PI * 4))));
		//org = org * Matrix4(0.75+0.25*sin(time*(M_PI * 6)));
		addMisterP(org, Matrix4(), ren);
		//org = org * Matrix4(0.5);
		//Matrix4 rot;
		//double c = cos(time*(M_PI * 6))*1;
		//addMC(org, rot, Matrix4(), Matrix4ROTyz(c), Matrix4ROTyz(-c), Matrix4(), Matrix4ROTyz(c), Matrix4ROTyz(-c), ren);
	}*/

	loadImageMovie1(frame);

	{
		if (frame < 343)
			Movement::putCharacters(ren, cam, r);
		else if (frame < 396)
			Movement::putCharactersP(ren, cam, r, Vector3(18*2,14,0)/2);
		else if (frame < 412)
			Movement::putCharactersOP(ren, cam, r, Vector3(18*2,14,0)/2);
		else if (frame < 461) {
			auto data = Movement::readMovement();
			data.mp.orgLoc.x+=18*2;
			data.mp.orgLoc.y+=14;
			Movement::putCharactersPC(data, ren, cam, r, Vector3(0,-100.8,0));

			if (cam->matrix.asLoc().y < (-59+0.8)/2)
				cam->matrix.translate(Vector3(0,-100.8,0));
		} else if (frame < 583)
			Movement::putCharacters(ren, cam, r);
		else if (frame < 593)
			Movement::putCharactersC(ren, cam, r, Vector3(-150, 0, 0));
		/*
		else if (frame < 674)
			Movement::putCharactersOC(ren, cam, r, Vector3(-150, 0, 0));
		/*
		else if (frame < 764) {
			auto data = Movement::readMovement();

			data.mc.orgLoc.x -= 300;

			Movement::putCharactersPC(data, ren, cam, r, (Vector3(-166, 0, 300)-Vector3(10, -278, 36) + Vector3(1,0,0))/2);
		} else {
			auto data = Movement::readMovement();

			data.mc.orgLoc.x -= 300;

			Movement::putCharactersPC(data, ren, cam, r, (Vector3(-166, 0, 300)-Vector3(10, -278, 36) + Vector3(1,0,0))/2);

			//if (cam->matrix.asLoc().x > 9.5)
				cam->matrix.translate((Vector3(-166, 0, 300)-Vector3(10, -278, 36) + Vector3(1,0,0))/2);
		}*/
		else if (frame < 692)
			Movement::putCharactersOC(ren, cam, r, Vector3(-150, 0, 0));
		else if (frame < 740)
			Movement::putCharacters(ren, cam, r);
		else if (frame < 753) {//753
			//Movement::putCharacters(ren, cam, r);
			Movement::putCharactersP(ren, cam, r, (Vector3(-166, 0, 300)-Vector3(10, -278, 36) + Vector3(1,0,0))/2);
		} else if (frame < 764) {
			Movement::putCharacters(ren, cam, r);
			//cam->matrix.translate((Vector3(-166, 0, 300)-Vector3(10, -278, 36) + Vector3(1,0,0))/2);
		} else if (frame <= 800) {
			Movement::putCharacters(ren, cam, r);
			cam->matrix.translate((Vector3(-166, 0, 300)-Vector3(10, -278, 36) + Vector3(1,0,0))/2);
		} else if (frame <= 1100)
			Movement::putCharacters(ren, cam, r);
		else if (frame <= 1900){
			Movement::VectorM loc = {84.9288, 432.605, 1522.37};
			Movement::VectorM rot = {0.677188, -0, -3.00546};
			rot.x-=M_PI*0.5;

			loc.z += (time - 45.833)*100;

			cam->setMatrix(iLocRot(loc, rot).translate(Vector3(r->nextDouble()/1000,r->nextDouble()/1000,r->nextDouble()/1000)));
		}
	}

	if (frame < 461)
		addRoom1(time, ren);
	//addRoom2(time, ren);
	//addRoom3(time, ren);

	if (time > 9 && frame < 507)
		addRoom2(time, ren);

	if (time > 9 && frame < 764)
		addRoom3(time, ren);

	if (frame > 510)
		addRoom4(time, ren);

	if (frame > 510) {
		addRocked(time, ren);
	}
}

class MatrixNonedianMatirial4 : public Matirial4 {
public:

	inline MatrixNonedianMatirial4() {}
	virtual ~MatrixNonedianMatirial4() {}
	virtual Color doShadeColorRay(Ray4* ray, Ray4* ecadianRay, RenderEnv4* env, RenderStackFrame4* frame) {
		Vector4 loc = (ray->loc+ray->dir*ray->maxDist);
		Vector4 dir = ray->dir;
		loc = loc + dir*0.001;

		Ray4 newRay (loc, dir);

		return env->doRayColor(&newRay, ecadianRay, frame);
	}
};

MatrixNonedianMatirial4* nonMovMat1 = new MatrixNonedianMatirial4 ();
/*
DirectNormColorMatirial4* mon4mat1 = new DirectNormColorMatirial4(Color(1/1.5, 0.925/1.5, 0.063/1.5));
DirectNormColorMatirial4* mon4mat2 = new DirectNormColorMatirial4(Color(1/1.5, 0.925/1.5, 1/1.5));
*/
DirectNormColorMatirial4** mon4mat = nullptr;

static ImageMatirial4* imageMovie2Mat;

struct colorSetupS {
	double off;
	double cosStrengt;
	double cosSpeed;
	double cosTimeOff;
};
colorSetupS colorSetupSV[16*3];

inline double cosValue(colorSetupS colorV, double time) {
	return colorV.off + colorV.cosStrengt * cos((time + colorV.cosTimeOff) * colorV.cosSpeed);
}

const double movie2R = 1920.0 / 1080.0;

void setupColor4d(double time) {
	if (mon4mat == nullptr) {
		mon4mat = new DirectNormColorMatirial4*[16];
		for (int i = 0;i<16;i++)
			mon4mat[i] = new DirectNormColorMatirial4(Color(0,0,0));


		Matrix5 matrx = Matrix5ROT<0,2>(M_PI/2);
		matrx.translate(Vector4(0.5,0,0.5,0));
		matrx = matrx * Matrix5(Vector4(1.0/2,1,1.0/(movie2R*2),1));

		Matrix5 matrx2;
		matrx2.data[2][2] = -1;

		matrx = matrx * matrx2;

		imageMovie2Mat = new ImageMatirial4(matrx, nullptr);


		std::string file;
		if (isLocal)
			file = "/Volumes/Christiaans Drive/anders/school/ckv/2/videoORG2/img.ppm";//local
		else
			file = "videoORG2/img.ppm";//mini
		imageMovie2Mat->image = PPMfileType->loadFile(file);
	}

	//mon4mat1->color = Color(cos(time), sin(time+0.6), 0.8);
	for (int i = 0;i<16;i++) {
		int i2 = i*3;
		mon4mat[i]->color = Color(
				cosValue(colorSetupSV[i2], time),
				cosValue(colorSetupSV[i2+1], time),
				cosValue(colorSetupSV[i2+2], time)
			);
	}
}

inline int floorShort(double v) {
	return (int)(v + 32768.) - 32768;
}

//forward_list<Geromitry4*> move4dDelete;

class MovRenderEnv4 : public RenderEnv4 {
public:

	inline MovRenderEnv4(RayVerivirer4 alowRay) : RenderEnv4(alowRay) {}
	virtual ~MovRenderEnv4() {}
	virtual void flushEnv(Ray4* ray) {
		int x = floorShort(ray->loc.x/10);
		int y = floorShort(ray->loc.y/10);
		int z = floorShort(ray->loc.z/10);
		int m = floorShort(ray->loc.m/10);

		int v = (x ^ y) ^ (z ^ m);

		//if (v != 0)
			//std::cout << "hit " << v << endl;

		Vector4 loc = Vector4(x,y,z,m) * 10;

		Geromitry4* ger1 = new Geromitry4(new AABBcube4(loc, loc+Vector4(10,10,10,10)));
		ray->parseGeromitry(ger1, 1, nonMovMat1);
		//move4dDelete.push_front(ger1);

		Geromitry4* ger2 = new Geromitry4(new AABBcube4(loc+Vector4(2,2,2,2), loc+Vector4(8,8,8,8)));
		//move4dDelete.push_front(ger2);
		/*
		if ((v & 1) == 0)
			ray->parseGeromitry(ger2, 1, mon4mat1);
		else
			ray->parseGeromitry(ger2, 1, mon4mat2);
			*/

		unsigned v2 = (v&1) | ((v%5==0)<<1) | ((v%7==0)<<2) | ((v%3==0)<<3);
		//unsigned v2 = (v&1);
		ray->parseGeromitry(ger2, 1, mon4mat[v2]);

		if (ray->loc.dotProduct(ray->loc) < 400) {
			Geromitry4* ger3 = new Geromitry4(new AABBcube4(Vector4(-1,-1+2,-movie2R,-1), Vector4(1,1+2,movie2R,1)));
			//move4dDelete.push_front(ger3);

			ray->parseGeromitry(ger3, 1, imageMovie2Mat);
			//ray->parseGeromitry(ger3, 1, mon4mat[0]);
		}
	}


	virtual Color doRayColor(Ray4* ray, Ray4* ecadianRay, RenderStackFrame4* frame) {
		if (alowRay(frame)) {
			flushEnv(ray);
			if (ray->maxDist == DBL_MAX)
				return Color(0);

			double fog = pow(M_E, ray->maxDist*-0.001);
			ray->finish();
			if (ecadianRay == nullptr) {
				RenderStackFrame4 cframe = frame->addRay(ray, ColorRayType, fog);
				fog = 1-fog;
				ray->norm = ray->norm.normalize();
				Color c = ray->geromitry.mat->doShadeColorRay(ray, nullptr, this, &cframe);
				//return c;
				return Color(0, 0, 0, fog).combine(c);
			} else {
				RenderStackFrame4 cframe = frame->addNonEdian(ray->maxDist, fog);
				fog = 1-fog;
				Ray4 ecadianRay2 (*ecadianRay);
				ecadianRay2.maxDist+=ray->maxDist;
				ray->norm = ray->norm.normalize();
				Color c = ray->geromitry.mat->doShadeColorRay(ray, &ecadianRay2, this, &cframe);
				return Color(0, 0, 0, fog).combine(c);
			}
		} else
			return Color(1);
	}

	virtual double doShadowRay(Ray4* ray, RenderStackFrame4* frame, Sphere4* dest) {
		return 0;
	}


};

#endif /* MOVIEDATE_H_ */
