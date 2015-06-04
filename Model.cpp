#include "Engine.h"
#include <exception>

Model::Model()
{
	isLoaded = false;
	modelType = MODEL_TYPE_VT;

	scale.x = 1;
	scale.y = 1;
	scale.z = 1;
}

Model::Model(const int type)
{
	isLoaded = false;
	SetModelType(type);

	scale.x = 1;
	scale.y = 1;
	scale.z = 1;
}

Model::Model(const char* filename)
{
	isLoaded = false;
	modelType = MODEL_TYPE_VT;

	scale.x = 1;
	scale.y = 1;
	scale.z = 1;

	loadObj(filename);
}

Model::Model(const char* filename, const int type)
{
	isLoaded = false;

	scale.x = 1;
	scale.y = 1;
	scale.z = 1;

	SetModelType(type);
	loadObj(filename);
}

void Model::SetModelType(int type)
{
	if (0 <= type && type <= MODEL_TYPE_VTN)
		modelType = type;
}

bool Model::loadObj(const char* filename)
{
	bool result = false;

	std::vector <float> triVertA, triVertB, triVertC;
	std::vector <float> triUVA, triUVB, triUVC;
	std::vector <float> triNormA, triNormB, triNormC;

	float vertA, uvA, normA, vertB, uvB, normB, vertC, uvC, normC;

	char line[255];
	char header[3];

	FILE* file = fopen(filename, "r");
	if (file == nullptr)
		return false;
	try
	{
		while (!feof(file))
		{
			VERTEX tempVert;
			UV tempUV;
			NORMAL tempNorm;

			readLine(file, line);

			sscanf (line, "%s", &header);

			if (strcmp(header, "v") == 0)
			{
				sscanf(line, "%*s %f %f %f", &tempVert.x, &tempVert.y, &tempVert.z);
				vertices.push_back(tempVert);
			}
			else if (strcmp(header, "vt") == 0)
			{
				sscanf(line, "%*s %f %f", &tempUV.u, &tempUV.v);
				uvs.push_back(tempUV);
			}
			else if (strcmp(header, "vn") == 0)
			{
				sscanf(line, "%*s %f %f %f", &tempNorm.x, &tempNorm.y, &tempNorm.z);
				normals.push_back(tempNorm);
			}
			else if(strcmp(header, "f") == 0)
			{
				if (modelType == MODEL_TYPE_V)
				{
					sscanf(line, "%*s %f %f %f", &vertA, &vertB, &vertC);
				}
				else if (modelType == MODEL_TYPE_VT)
				{
					sscanf(line, "%*s %f/%f %f/%f %f/%f", &vertA, &uvA, &vertB, &uvB, &vertC, &uvC);
			
					triUVA.push_back(uvA);
					triUVB.push_back(uvB);
					triUVC.push_back(uvC);
				}
				else if (modelType == MODEL_TYPE_VTN)
				{
					sscanf(line, "%*s %f/%f/%f %f/%f/%f %f/%f/%f", &vertA, &uvA, &normA, &vertB, &uvB, &normB, &vertC, &uvC, &normC);

					triUVA.push_back(uvA);
					triUVB.push_back(uvB);
					triUVC.push_back(uvC);

					triNormA.push_back(normA);
					triNormB.push_back(normB);
					triNormC.push_back(normC);
				}

				triVertA.push_back(vertA);
				triVertB.push_back(vertB);
				triVertC.push_back(vertC);
			}
		}
		fclose(file);

		//assign vertices to faces(triangle)
		for (int i = 0; i < triVertA.size(); i++)
		{
			TRIANGLE tempTri;

			// Vertices
			tempTri.vertex[0].x = vertices.at(triVertA[i] - 1).x;
			tempTri.vertex[0].y = vertices.at(triVertA[i] - 1).y;
			tempTri.vertex[0].z = vertices.at(triVertA[i] - 1).z;

			tempTri.vertex[1].x = vertices.at(triVertB[i] - 1).x;
			tempTri.vertex[1].y = vertices.at(triVertB[i] - 1).y;
			tempTri.vertex[1].z = vertices.at(triVertB[i] - 1).z;

			tempTri.vertex[2].x = vertices.at(triVertC[i] - 1).x;
			tempTri.vertex[2].y = vertices.at(triVertC[i] - 1).y;
			tempTri.vertex[2].z = vertices.at(triVertC[i] - 1).z;

			if (modelType == MODEL_TYPE_VT || modelType == MODEL_TYPE_VTN)
			{
				// Textures
				tempTri.uv[0].u = uvs.at(triUVA[i] - 1).u;
				tempTri.uv[0].v = uvs.at(triUVA[i] - 1).v;

				tempTri.uv[1].u = uvs.at(triUVB[i] - 1).u;
				tempTri.uv[1].v = uvs.at(triUVB[i] - 1).v;

				tempTri.uv[2].u = uvs.at(triUVC[i] - 1).u;
				tempTri.uv[2].v = uvs.at(triUVC[i] - 1).v;

				// Normals
				if (modelType == MODEL_TYPE_VTN)
				{
					tempTri.normal[0].x = normals.at(triNormA[i] - 1).x;
					tempTri.normal[0].y = normals.at(triNormA[i] - 1).y;
					tempTri.normal[0].z = normals.at(triNormA[i] - 1).z;

					tempTri.normal[1].x = normals.at(triNormB[i] - 1).x;
					tempTri.normal[1].y = normals.at(triNormB[i] - 1).y;
					tempTri.normal[1].z = normals.at(triNormB[i] - 1).z;

					tempTri.normal[2].x = normals.at(triNormC[i] - 1).x;
					tempTri.normal[2].y = normals.at(triNormC[i] - 1).y;
					tempTri.normal[2].z = normals.at(triNormC[i] - 1).z;
				}
			}

			triangles.push_back(tempTri);
		}

		isLoaded = result = true;
	}
	catch(...)
	{
		vertices.clear();
		uvs.clear();
		normals.clear();
	}

	return result;
}

void Model::draw()
{
	if (isLoaded)
	{
		glPushMatrix();

		glTranslatef(pos.x, pos.y, pos.z);

		glScalef(scale.x, scale.y, scale.z);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, objTex);
		glEnable(GL_RESCALE_NORMAL);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < triangles.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (modelType == MODEL_TYPE_VT || modelType == MODEL_TYPE_VTN )
				{
					if (modelType == MODEL_TYPE_VTN)
					{
						glNormal3f(triangles[i].normal[j].x,
							triangles[i].normal[j].y,
							triangles[i].normal[j].z);
					}
					glTexCoord2f(triangles[i].uv[j].u, -triangles[i].uv[j].v);
				}
				glVertex3f(triangles[i].vertex[j].x,
					triangles[i].vertex[j].y,
					triangles[i].vertex[j].z);
			}
		}
		glEnd();
		glDisable(GL_RESCALE_NORMAL);
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}
}

void Model::readLine(FILE * fp, char * string)
{
	do
	{
		fgets(string, 255, fp);

	} while (string[0] == '\n');
	return;
}

bool Model::loadTexture(const char* filename)
{
	bool result = false;

	sf::Image texImg;
	if(texImg.loadFromFile(filename))
	{
		glGenTextures(1, &objTex);
		glBindTexture(GL_TEXTURE_2D, objTex);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texImg.getSize().x, texImg.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, texImg.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		result = true;
	}

	return true;
}

void Model::SetPos( float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void Model::SetP0( float x, float y, float z)
{
	p0.x = x;
	p0.y = y;
	p0.z = z;
}

void Model::SetP1( float x, float y, float z)
{
	p1.x = x;
	p1.y = y;
	p1.z = z;
}
