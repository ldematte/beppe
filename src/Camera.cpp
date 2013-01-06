/*******************************************************************\
*  Bezier Patch Editor  ---  Camera                                 *
*                                                                   *
*  Una semplice interfaccia per gestire una telecamera.             *
*  La telecamera puo' muoversi avanti ed indietro, lateralmente e   *
*  puo' ruotare su se stessa.                                       *
*  Agisce sulla matrice MODELVIEW utilizzando la funzione glLookAt  *
\*******************************************************************/

#include "Camera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Camera::Camera()
{

}

Camera::~Camera()
{

}
//////////////////////////////////////////////////////////////////////
// Setta in maniera diretta la posizione della telecamera

void Camera::PositionCamera(float positionX, float positionY, float positionZ,
                            float viewX,     float viewY,     float viewZ,
                            float upVectorX, float upVectorY, float upVectorZ)
{
	vPosition = vector3(positionX, positionY, positionZ);
	vView     = vector3(viewX,     viewY,     viewZ);
	vUp       = vector3(upVectorX, upVectorY, upVectorZ);
}



//////////////////////////////////////////////////////////////////////
// Motion functions
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// muove la telecamera lateralmente

void Camera::StrafeCamera(float speed)
{
  vector3 vViewVector = vView - vPosition;

  vector3 vCross = CrossProduct(vViewVector, vUp);

	vPosition.x += vCross.x * speed;
	vPosition.z += vCross.z * speed;

	vView.x += vCross.x * speed;
	vView.z += vCross.z * speed;
}

//////////////////////////////////////////////////////////////////////
// muove la telecamera avanti e indietro

void Camera::MoveCamera(float speed)
{
	vector3 vViewVector = vView - vPosition;

	vPosition.x += vViewVector.x * speed;
	vPosition.z += vViewVector.z * speed;	
	vView.x += vViewVector.x * speed;
	vView.z += vViewVector.z * speed;

}

//////////////////////////////////////////////////////////////////////
//  Ruota la vista

void Camera::RotateView(float x, float y, float z)
{
  vector3 vViewVector = vView - vPosition;

  if(x) 
  {
		vView.z = (float)(vPosition.z + sin(x) * vViewVector.y + cos(x) * vViewVector.z);
		vView.y = (float)(vPosition.y + cos(x) * vViewVector.y - sin(x) * vViewVector.z);
	}
	
  if(y) 
  {
		vView.z = (float)(vPosition.z + sin(y) * vViewVector.x + cos(y) * vViewVector.z);
		vView.x = (float)(vPosition.x + cos(y) * vViewVector.x - sin(y) * vViewVector.z);
	}
	
  if(z) 
  {
		vView.x = (float)(vPosition.x + sin(z) * vViewVector.y + cos(z) * vViewVector.x);		
		vView.y = (float)(vPosition.y + cos(z) * vViewVector.y - sin(z) * vViewVector.x);
	}

}

//////////////////////////////////////////////////////////////////////
// cattura lo spostamento del mouse, traducendolo in uno
// spostamento della telecamera (Rotate)

void Camera::HandleMouse(int deltaX, int deltaY)
{
	float fDeltaY  = 0.0f;
	float fRotateY = 0.0f;
	
	fRotateY = (float)( (deltaX) ) / 1000;		
	fDeltaY  = (float)( (deltaY) ) / 1000;

	vView.y += fDeltaY * 15;

	// Check if the distance of our view exceeds 10 from our position, if so, stop it. (UP)
	if( (vView.y - vPosition.y) >  10)  vView.y = vPosition.y + 10;

	// Check if the distance of our view exceeds -10 from our position, if so, stop it. (DOWN)
	if( (vView.y - vPosition.y) < -10)  vView.y = vPosition.y - 10;

	// Here we rotate the view along the X avis depending on the direction (Left of Right)
	RotateView(0, -fRotateY, 0);
}

//////////////////////////////////////////////////////////////////////
// muove la telecamera verticalmente

void Camera::FlyCamera(float speed)
{
  vector3 vViewVector = vView - vPosition;

	vPosition.y += vViewVector.y * speed;
	vView.y += vViewVector.y * speed;
}



