#ifndef _MODEL_H
#define _MODEL_H

#include "Vec3.h"

#define MODEL_TYPE_V 0
#define MODEL_TYPE_VT 1
#define MODEL_TYPE_VTN 2

typedef struct tagVERTEX {
	float x, y, z;
} VERTEX;

typedef struct tagUV {
	float u, v;
} UV;

typedef struct tagNormal {
	float x, y, z;
} NORMAL;

typedef struct tagTRIANGLE {
	VERTEX vertex[3];
	UV uv[3];
	NORMAL normal[3];
} TRIANGLE;

class Model
{
public:
	Model();
	Model(const int);
	Model(const char*, int);
	Model(const char*);

	int GetModelType() const { return modelType; }
	void SetModelType(int);

	Vec3 GetScale() const { return scale; }
	void SetScale(Vec3 val) { scale = val; }
	void SetPos(float, float, float);
	void SetP0(float, float, float);
	Vec3 GetP0() const { return p0; }
	void SetP1(float, float, float);
	Vec3 GetP1() const { return p1; }

	bool loadObj(const char*);
	bool loadTexture(const char*);
	void draw();

	Vec3 p0;
	Vec3 p1;

	Vec3 pos;
	Vec3 scale;

	float speed;

private:
	void readLine(FILE *, char *);

	std::vector <VERTEX> vertices;
	std::vector <UV> uvs;
	std::vector <NORMAL> normals;
	std::vector <TRIANGLE> triangles;

	int modelType;
	bool isLoaded;

	GLuint objTex;

};

#endif
