/*******************************************************************\
*  Bezier Patch Editor  ---  Camera                                 *
*                                                                   *
*  Una semplice interfaccia per gestire una telecamera.             *
*  La telecamera puo' muoversi avanti ed indietro, lateralmente e   *
*  puo' ruotare su se stessa.                                       *
*  Agisce sulla matrice MODELVIEW utilizzando la funzione gluLookAt *
\*******************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "common.h"
#include "Mtxlib.h"

class Camera  
{
public:
	Camera();
	virtual ~Camera();

  // Setta in maniera diretta la posizione della telecamera
  void PositionCamera(float positionX, float positionY, float positionZ,
                      float viewX,     float viewY,     float viewZ,
                      float upVectorX, float upVectorY, float upVectorZ);

  //  Ruota la vista
  void RotateView(float X, float Y, float Z);
  
  // muove la telecamera lateralmente
  void StrafeCamera(float speed);
  
  // muove la telecamera avanti e indietro
  void MoveCamera(float speed);
  
  // cattura lo spostamento del mouse, traducendolo in uno
  // spostamento della telecamera (Rotate)
  void HandleMouse(int deltaX, int deltaY);
  
  // muove la telecamera verticalmente
  void FlyCamera(float speed);

public:
    
  // i vettori per la posizione, saranno dati
  // in input alla gluLookAt
  vector3 vPosition;
  vector3 vView;
  vector3 vUp;

};

#endif //CAMERA_H
