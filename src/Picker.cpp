/*******************************************************************\
*  Bezier Patch Editor  ---  Picker                                 *
*                                                                   *
*  Questa classe racchiude le operazioni Opengl necessarie per      *
*  effettuare il picking. Comunica direttamente con il patch        *
*  manager per selezionare e deselezionare i punti.                 *
\*******************************************************************/

#include "Picker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Picker::Picker() : FingerSize(6)
{
  PM = PatchMgr::getInstance();
  log = Logger::getInstance();

  DrawSpheres = false;
}

Picker::~Picker()
{

}

// Controlla se in questa posizione ci sono punti
bool Picker::Verify(int x, int y)
{
	return (DoPick(x, y, FingerSize, FingerSize) > 0);
}

// Controlla se in questa posizione ci sono punti selezionati
bool Picker::VerifySelected(int x, int y)
{	
	GLint nHits = DoPick(x, y, FingerSize, FingerSize);
	if (!nHits) return false;

	HitRecord* p = (HitRecord*)SelectBuffer;
	for (int i = 0; i < nHits; ++i)
	{
		if (p[i].nNames != 2)
		{
			log->addf("Picker :: Wrong name stack (%d)", p[i].nNames);
		}
		else
		{
 			if (PM->getPatch(p[i].nPatch)->isSelected(p[i].nPoint))
				return true;
	   }
   }
	return false;
}


// Esegue un picking 'a click' nella posizione (x,y)
// ctrl e' vero se si sta premendo il tasto CTRL
bool Picker::Click(int x, int y, bool ctrl)
{
  GLint nHits = DoPick(x, y, FingerSize, FingerSize);

  if (ctrl)
    MultiSelect(nHits, true);
  else
    SingleSelect(nHits);

  return (nHits ? true : false);
}

// Esegue un picking a rettangolo. Il rettangolo e' definito
// dai punti (x1,y1) e (x2,y2)
// ctrl e' vero se si sta premendo il tasto CTRL
bool Picker::Sweep(int x1t, int y1t, int x2t, int y2t, bool ctrl)
{
	// controllo che il punto (x1,y1) sia quello superiore sinistro e
	// il punto (x2,y2) sia inferiore destro.
  int x1 = x1t;
  int x2 = x2t;
  int y1 = y1t;
  int y2 = y2t;

	if (x1t > x2t)
	{
		x1 = x2t;
		x2 = x1t;
	}
	if (y1t > y2t)
	{
		y1 = y2t;
		y2 = y1t;
	}

	// calcolo il centro
	int xCenter = (x1 + x2) / 2;
	int yCenter = (y1 + y2) / 2;

	// .. e le dimensioni
  int w = x2 - x1;
  int h = y2 - y1;

  GLint nHits = DoPick(xCenter, yCenter, w, h);

  if (!ctrl)
    PM->ClearSelection();
  
  MultiSelect(nHits, false);
  
  return (nHits ? true : false);  
}

/////////////////////////////// Private //////////////////////////////////////

// Esegue il picking. (x,y) e' il punto cliccato e w:h sono
// le dimensioni del dito. Ritorna il numero di hits rilevati.
inline GLint Picker::DoPick(int x, int y, int w, int h)
{
  if (w <= 0)  w = 1;
  if (h <= 0)  h = 1;

  //Init
  glSelectBuffer(BUFFERSIZE, SelectBuffer);
  glRenderMode(GL_SELECT);

  //Setup
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glGetFloatv(GL_PROJECTION_MATRIX, ProjMatrix);

  glLoadIdentity();
  gluPickMatrix((GLdouble)x, (GLdouble)(Viewport[3] - y), (GLdouble)w, (GLdouble)h, Viewport);
  glMultMatrixf(ProjMatrix);

  glMatrixMode(GL_MODELVIEW);

  PM->DrawCtrlPoints(DrawSpheres);	// se DrawSpheres si disegna sfere, altrimenti punti

  //Restore
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glFlush();
  glMatrixMode(GL_MODELVIEW);

  return glRenderMode(GL_RENDER);
}

// Analizza il buffer degli hits e seleziona il punto piu' vicino
// (quello con profondita' minore)
void Picker::SingleSelect(int nHits)
{
  //read the buffer, call the patch manager
  //and select the points

	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
  //PM->ClearSelection();	// rimossa perchè non permetteva alcune cose
									// il viewport ha la responabilità di annullare la selezione
  if (!nHits) return;

  HitRecord* p = (HitRecord*)SelectBuffer;
  HitRecord Front = p[0];

  for (int i = 1; i < nHits; ++i)
  {
    if (p[i].nNames != 2)
    {
      log->addf("Picker :: Wrong name stack (%d)", p[i].nNames);
    }
    else
    {
      if (p[i].ZMin < Front.ZMin)
        Front = p[i];
    }
  }

  if (!PM->getPatch(Front.nPatch)->isSelected(Front.nPoint))
    PM->ClearSelection();

  PM->SelectPoint(Front.nPatch, Front.nPoint);
  /*
  log->addf("Picker :: Sel patch(%d).point(%d) minz %f maxz %f", 
            Front.nPatch, Front.nPoint, 
            (float)Front.ZMin / (float)(0xffffffff), 
            (float)Front.ZMax / (float)(0xffffffff));
  */
}

// Analizza il buffer degli hits e seleziona tutti i punti toccati.
// Se togglingMode e' vero la selezione dei punti toccati viene invertita.
void Picker::MultiSelect(int nHits, bool togglingMode)
{
  //read the buffer, call the patch manager
  //and select the points

  HitRecord* p = (HitRecord*)SelectBuffer;

  for (int i = 0; i < nHits; ++i)
  {
    if (p[i].nNames != 2)
    {
      log->addf("Picker :: Wrong name stack (%d)", p[i].nNames);
    }
    else
    {
      if (togglingMode)
        PM->TogglePoint(p[i].nPatch, p[i].nPoint);
      else
        PM->SelectPoint(p[i].nPatch, p[i].nPoint);
    }
  }
}

void Picker::Drawing2d()
{
	DrawSpheres = false;
}

void Picker::Drawing3d()
{
	DrawSpheres = true;
}