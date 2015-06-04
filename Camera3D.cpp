#include "Engine.h"

Camera3D::Camera3D(sf::Window* window)
{
	this->window = window;

	//Set some default values
	azemuth   = 0.0f;
	elevation = 0.0f;
}

void Camera3D::Activate()
{
	glViewport(VIEW_BORDER_SIZE, VIEW_BORDER_SIZE, WINDOW_WIDTH - VIEW_BORDER_SIZE*2, WINDOW_HEIGHT - VIEW_BORDER_SIZE*2);
	//Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//TODO: remove hard coded values
	gluPerspective(
		90.f, //Field of view
		float(window->getSize().x) / float(window->getSize().y),  //Aspect ratio
		2.f,  //Near clipping plane
		FOV_FAR); //Far clipping plane

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Rotate and place the camera in the scene
	//Convert angle to vector
	forward.PolarToVector(azemuth, elevation, 1.0f);
	strafe.PolarToVector(azemuth+90.0f, 0.0f, 1.0f);
	up = strafe.Cross(forward);

	//Rotate the camera
	glRotatef(elevation, 1.0f, 0.0f, 0.0f);
	glRotatef(azemuth, 0.0f, 1.0f, 0.0f);	
	glTranslatef(-pos.x, -pos.y, -pos.z);
}