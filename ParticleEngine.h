#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "Vec3.h"
#include "Camera3D.h"

struct Particle
{
	Vec3 pos;
	Vec3 col;
	float scale;
	unsigned int blendSource;
	unsigned int blendDest;
	GLint texture;
	float dist;

};

bool CompareParticles(Particle, Particle);
void DrawParticle(Camera3D*, Vec3, float);
float Random(float, float);

class ParticleEngine
{
public:
	ParticleEngine(Camera3D*);

	void addParticle(const GLuint texture, const float x, const float y, const float z);
	void generateRandom(const GLuint, const float, const float, const float);
	void clear();
	void draw();

	GLuint loadTexture(const char*);

private:
	Camera3D* cam;
	std::list<Particle> particles;
};

#endif