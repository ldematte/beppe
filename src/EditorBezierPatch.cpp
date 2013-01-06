/*******************************************************************\
*  Bezier Patch Editor  ---  EditorBezierPatch                      *
*                                                                   *
*  Questa classe identifica una patch di bezier con tutti gli       *
*  strumenti necessari per essere utilizzata dall'editor.           *
*  Fornisce funzioni per selezionare/deselezionare punti, per       *
*  disegnarli e per disegnare il poligono di controllo.             *
\*******************************************************************/

#include <assert.h>
#include "EditorBezierPatch.h"
#include "Logger.h"

//#define	POS_OK(n)	assert(n>=0 && n<16 && "Patch: punto fuori range!");

// Macro per controllare (con le compilazioni di DEBUG)
// che il punto di controllo sia nel range valido (0..15)
// n dovrebbe essere sempre >= 0 perche usiamo UNSIGNED char

#define	POS_OK(n)	assert(n<16 && "Patch: punto fuori range!");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EditorBezierPatch::EditorBezierPatch(unsigned int id, unsigned int matsel, unsigned int matdesel, unsigned int matbinded, GLuint callSphere) : RenderedBezier(id)
{	
	matSel = matsel;
	matDesel = matdesel;
	matBinded = matbinded;
	lSphere = callSphere;
	Init();
}

EditorBezierPatch::EditorBezierPatch(unsigned int id, unsigned int matsel, unsigned int matdesel, unsigned int matbinded, point cpoints[][4], GLuint callSphere) : RenderedBezier(id, cpoints)
{
	myId = id;
	matSel = matsel;
	matDesel = matdesel;
	matBinded = matbinded;
	lSphere = callSphere;
	Init();
}

EditorBezierPatch::~EditorBezierPatch()
{
	SelectNone(); 	
}

//////////////////////////////////////////////////////////////////////
// Inizializza tutta la baracca

void EditorBezierPatch::Init()
{
	int i;

	nPointSelected = 0;

	for (i=0; i<16; ++i)
	{
		Selected[i] = false;
		Hided[i] = false;
		Binded[i] = false;
	}

	selColor[0] = MM->getColorRed(matSel);
	selColor[1] = MM->getColorGreen(matSel);
	selColor[2] = MM->getColorBlue(matSel);
	
	
	deselColor[0] = MM->getColorRed(matDesel);
	deselColor[1] = MM->getColorGreen(matDesel);
	deselColor[2] = MM->getColorBlue(matDesel);
}

//////////////////////////////////////////////////////////////////////
// Star Wars Episode II: Clone Attack

EditorBezierPatch *EditorBezierPatch::Clone()
{
	EditorBezierPatch *copy = new EditorBezierPatch(myId, matSel, matDesel, matBinded, Points, lSphere);

	copy->nDepth = nDepth;
	copy->bNormals = bNormals;
	copy->nPointSelected = 0;
	copy->bDirty = true;
	copy->material = material;
	copy->texture = texture;

	copy->SelectNone();
	
	return copy;
}

//////////////////////////////////////////////////////////////////////
// Disegna il poligono di controllo

void EditorBezierPatch::DrawCtrlPolygon()
{
  int i;
  int j;

  glEnable(GL_BLEND);	// per utilizzare l'antialiasing delle linee  
  
  //The control polygon
  glBegin(GL_LINES);
	for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 3; ++j)
    {
      glVertex3f(Points[i][j].x, Points[i][j].y, Points[i][j].z);
      glVertex3f(Points[i][j+1].x, Points[i][j+1].y, Points[i][j+1].z);
    }
  }

  for (i = 0; i < 3; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      glVertex3f(Points[i][j].x, Points[i][j].y, Points[i][j].z);
      glVertex3f(Points[i+1][j].x, Points[i+1][j].y, Points[i+1][j].z);
    }
  }
  glEnd();
  
  glDisable(GL_BLEND);
}

//////////////////////////////////////////////////////////////////////
// Disegna i punti di controllo in 3D (sferette)

void EditorBezierPatch::DrawCtrlPoints3d()
{	
	glPushAttrib(GL_POLYGON_BIT|GL_LIGHTING_BIT);

  // dava qualche prb in linux:
  // cliccando sulle finestre 2d, le patch venivano disegnate
  // con il GL_FILL
   
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);	

	for(int i = 0; i < 4; ++i)
		for (int j = 0 ; j < 4; ++j)
		{
			int idx = (i << 2) + j;

			if (Hided[idx])		// non dobbiamo disegnarlo
				continue;

			glPushName(idx);
			if (Selected[idx])
				MM->useMaterial(matSel);
			else if (Binded[idx])
         	MM->useMaterial(matBinded);
         else
				MM->useMaterial(matDesel);
			Drawpoint3d(Points[i][j]);
			glPopName();
		}
	glPopAttrib();  
}

//////////////////////////////////////////////////////////////////////
// Disegna i punti di controllo selezionati in 2D (punti rossi)
//
// I punti di controllo selszionati vanno disegnati separatamente
// da quelli selezionati (e DOPO di questi) perche' vogliamo vedere
// i punti selezionati anche se questi sono dietro (nascosti da) 
// quelli non selezionati

void EditorBezierPatch::DrawCtrlPoints2dSelected()
{	
	int i,j;
	point p;
  
 // per utilizzare l'antialiasing dei punti
	glEnable(GL_BLEND);	

	glColor3ubv(selColor);
  
	//glPushName(0);
	for(i = 0; i < 4; ++i)
		for (j = 0 ; j < 4; ++j)
		{
			p = Points[i][j];
			int idx = (i<<2)+j;
			
			if (Selected[idx] && !Hided[idx])
			{
				glPushName(idx);
				glBegin(GL_POINTS);
				glVertex3f(p.x, p.y, p.z);
				glEnd();
				glPopName();
			}
		}	

	glDisable(GL_BLEND);
}

//////////////////////////////////////////////////////////////////////
// Disegna i punti di controllo non selezionati in 2D (punti bianchi)

void EditorBezierPatch::DrawCtrlPoints2dDeselected()
{
	int i,j;
	point p;
	
  // per utilizzare l'antialiasing dei punti
	glEnable(GL_BLEND);	

	//glPushName(0);
	for(i = 0; i < 4; ++i)
		for (j = 0 ; j < 4; ++j)
		{
			p = Points[i][j];
			int idx = (i<<2)+j;
			
			if (!Selected[idx] && !Hided[idx])
			{
				if (Binded[idx])
					MM->useMaterial(matBinded);
				else
					MM->useMaterial(matDesel);

				glPushName(idx);		// push name deve essere fuori da un glBegin...
				glBegin(GL_POINTS);
				glVertex3f(p.x, p.y, p.z);
				glEnd();
				glPopName();
			}
			
		}
	
	glDisable(GL_BLEND);
}

//////////////////////////////////////////////////////////////////////
// Funzione ausiliaria : Disegna le sferette

void EditorBezierPatch::Drawpoint3d(point P)
{
	glPushMatrix();
   
	glTranslatef(P.x, P.y, P.z);
   	
	glCallList(lSphere);

   glPopMatrix();
}

//////////////////////////////////////////////////////////////////////
// Selezione

void EditorBezierPatch::Select(unsigned char pos)
{
	POS_OK(pos);
   
	if (!Selected[pos])
   {
		nPointSelected++;
      Selected[pos] = true;
      DrawBindings(pos, true);
      ClearBindingsApplied(pos>>2, pos&3);
   }
}

void EditorBezierPatch::Select(unsigned char i, unsigned char j)
{	
	Select((i<<2)+j);
}

void EditorBezierPatch::SelectAll()
{
	for (int i=0; i<16; i++)    
		Select(i);
}

//////////////////////////////////////////////////////////////////////
// Deselezione

void EditorBezierPatch::Deselect(unsigned char pos)
{
	POS_OK(pos);
   
	if (Selected[pos])
   {
		nPointSelected--;
     	Selected[pos] = false;
      DrawBindings(pos, false);
      ClearBindingsApplied(pos>>2, pos&3);
   }
}

void EditorBezierPatch::Deselect(unsigned char i, unsigned char j)
{
	Deselect((i<<2)+j);
}

void EditorBezierPatch::SelectNone()
{
	for (int i=0; i<16; i++)    
		Deselect(i);

}

//////////////////////////////////////////////////////////////////////
// E' selezionato?

bool EditorBezierPatch::isSelected(unsigned char pos)
{
	POS_OK(pos);
	return Selected[pos];
}
bool EditorBezierPatch::isSelected(unsigned char i, unsigned char j)
{
	return isSelected((i<<2)+j);
}

bool EditorBezierPatch::isPatchSelected()
{
	return (nPointSelected>0);
}

//////////////////////////////////////////////////////////////////////
// Scambia selezione/non selezione e viceversa

void EditorBezierPatch::Toggle(unsigned char pos)
{
	if (isSelected(pos))
    Deselect(pos);
  else
    Select(pos);
}
void EditorBezierPatch::Toggle(unsigned char i, unsigned char j)
{	
	if (isSelected(i, j))
    Deselect(i, j);
  else
    Select(i, j);
}

//////////////////////////////////////////////////////////////////////
//  Nascondi/mostra di nuovo punti di controllo

void EditorBezierPatch::HideSelected()
{
	for (int i=0; i<16; ++i)
	{
		if (isSelected(i))
		{
			Deselect(i);
			Hided[i] = true;
		}
	}
}

void EditorBezierPatch::UnhideAll()
{
	for (int i=0; i<16; ++i)
	{
		Hided[i] = false;
	}
}





