#include "Engine.h"

bool CompareParticles( Particle p1, Particle p2 )
{
	return (p1.dist > p2.dist);
}

void DrawParticle( Camera3D* cam, Vec3 pos, float scale )
{
	static Vec3 tempV;

	glPushMatrix();

	//Translate the particle on the X, Y and Z axis accordingly
	glTranslatef(pos.x, pos.y, pos.z);

	//Draw Particle 
	//**too much math, shaders are faster
	glBegin(GL_QUADS);
	tempV = cam->strafe*-scale - cam->up*scale;
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(tempV.x, tempV.y, tempV.z);

	tempV = cam->strafe*scale - cam->up*scale;
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(tempV.x, tempV.y, tempV.z);

	tempV = cam->strafe*scale + cam->up*scale;
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(tempV.x, tempV.y, tempV.z);

	tempV = cam->strafe*-scale + cam->up*scale;
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(tempV.x, tempV.y, tempV.z);
	glEnd();

	//End the changes to the current object
	glPopMatrix();
}

float Random( float min, float max )
{
	float test = (float(rand()%3001)/3000.0f) * (max-min) + min; 
	return test;
}


ParticleEngine::ParticleEngine(Camera3D* camera)
{
	cam = camera;
}

GLuint ParticleEngine::loadTexture( const char* filename)
{
	GLuint texture = 0;

	sf::Image texImg;
	if(texImg.loadFromFile(filename))
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texImg.getSize().x, texImg.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, texImg.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	return texture;
}

void ParticleEngine::addParticle(const GLuint texture, const float x, const float y, const float z)
{
	Particle swirlPart;
	swirlPart.blendSource = GL_SRC_ALPHA;
	swirlPart.blendDest = GL_ONE;
	//swirlPart.blendDest = GL_ONE_MINUS_SRC_ALPHA;
	swirlPart.scale = 250.f;
	swirlPart.texture = texture;

	swirlPart.pos.x = x;
	swirlPart.pos.y = y;
	swirlPart.pos.z = z;

	swirlPart.col =  Vec3(1.f,1.f,0.f);
	//swirlPart.col =  (swirlPart.col.Random()*0.5f) + Vec3(0.5f,0.5f, 0.5f);
	swirlPart.dist = (swirlPart.pos - cam->pos).GetLength();

	particles.push_back (swirlPart);
}

void ParticleEngine::generateRandom(const GLuint texture, const float x, const float y, const float z)
{
	Particle swirlPart;
	swirlPart.blendSource = GL_SRC_ALPHA;
	swirlPart.blendDest = GL_ONE;
	swirlPart.scale = 100.f;
	swirlPart.texture = texture;

	for (int i = 0; i < 1; i++)
	{
		swirlPart.pos.x = Random(0.f, x);
		swirlPart.pos.y = Random(0.f, y);
		swirlPart.pos.z = Random(0.f, z);

		swirlPart.col = (swirlPart.col.Random()*0.5f) + Vec3(0.5f,0.5f, 0.5f);
		swirlPart.dist = (swirlPart.pos - cam->pos).GetLength();

		particles.push_back (swirlPart);
	}
}

void ParticleEngine::draw()
{
	//Disable Depth Testing
	//**prevent overlapping (eg. particles)
	glDepthMask(false);
	//**prevent see-through (eg. hill to particles)
	glDisable(GL_DEPTH_TEST);

	//Enable blending
	glEnable (GL_BLEND);

	//Render the particle
	glEnable(GL_TEXTURE_2D);

	particles.sort(CompareParticles);
	for (std::list<Particle>::iterator i = particles.begin(); i != particles.end(); i++)
	{
		i->dist = (i->pos - cam->pos).GetLength();
		glColor3f(i->col.x, i->col.y, i->col.z);

		glBindTexture(GL_TEXTURE_2D, i->texture);
		glBlendFunc(i->blendSource, i->blendDest);
		DrawParticle(cam, i->pos, i->scale);
	}

	//Re-enable Depth Testing
	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

}

void ParticleEngine::clear()
{
	particles.clear();
}
