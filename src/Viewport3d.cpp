/*******************************************************************\
*  Bezier Patch Editor  ---  Viewport3d                             *
*                                                                   *
*  Estende Viewport e si specializza nella visualizzazione in       *
*  prospettiva. E' possibile "muoversi" nel mondo utilizzando a     *
*  scelta una trackball o una telecamera. Viene disegnata una       *
*  griglia sul piano XZ e nell'angolo superiore destro del viewport *
*  visualizza gli assi cartesiani per far capire la rotazione       *
*  attuale della trackball/telecamera.                              *
\*******************************************************************/

#include "Viewport3d.h"

// funzione dell'editor per ridisegnare tutte le viewport
extern void redrawAll();																																																																																						extern bool hehe;

// queste solo le lettere x,y,z da mettere sugli assi.... sono un mago dell'esadecimale!
static GLubyte BitmapX[] = {0x00, 0x42, 0x24, 0x18 ,0x18, 0x24, 0x42, 0x00};
static GLubyte BitmapY[] = {0x00, 0x20, 0x10, 0x08 ,0x0c, 0x12, 0x21, 0x00};
static GLubyte BitmapZ[] = {0x00, 0x7e, 0x20, 0x10 ,0x08, 0x04, 0x7e, 0x00};	
static GLubyte BitmapO[] = {0x00, 0x3c, 0x42, 0x00 ,0x24, 0x24, 0x00, 0x00};	


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Viewport3d::Viewport3d(FXComposite* p, FXGLVisual *vis, const char* title, bool bottomToolbar) : Viewport(p, vis, title, bottomToolbar)
{		
	blackFog = 0;
	init();
}

Viewport3d::Viewport3d(FXComposite* p, FXGLVisual *vis, Viewport *prev, const char* title, bool bottomToolbar) : Viewport(p, vis, prev, title, bottomToolbar)
{	
	blackFog = 0;
	init();
}


Viewport3d::~Viewport3d()
{

}

void Viewport3d::init()
{
	LetteraX = BitmapX;
	LetteraY = BitmapY;
	LetteraZ = BitmapZ;	

	RenderMode.Solid			= true;
	RenderMode.Texture			= true;
	RenderMode.Cullface			= false;
	RenderMode.DepthTest		= true;
	
	RenderMode.ClearColor[0]	= 0.0f;
	RenderMode.ClearColor[1]	= 0.0f;
	RenderMode.ClearColor[2]	= 0.0f;
	RenderMode.ClearColor[3]	= 1.0f;

	RenderMode.ShowCtrlPoints	= true;
	RenderMode.ShowCtrlPolygon	= false;
	RenderMode.ShowPatch		= true;

	RenderMode.ShowGrid  = true;
  RenderMode.UseCamera = false;

	butSnapToGrid->hide();
	butGridPlus->hide();
	butGridMinus->hide();
	butCullFace->show();
	butShowGrid->show();
	butSolid->show();
	butTexture->show();
	butCameraTrackball->show();
  

	updateButtons();

	zoom = 10.0f;
	trackball(curQuat, 0.0, 0.0, 0.0, 0.0);
	trackball(lastQuat, 0.0, 0.0, 0.0, 0.0);

	//identita'
	memset(&rotMatrix[0][0], 0, 16*sizeof(float));
	rotMatrix[0][0] = 1.0f;
	rotMatrix[1][1] = 1.0f;
	rotMatrix[2][2] = 1.0f;
	rotMatrix[3][3] = 1.0f;

	Pick.Drawing3d();

  camera.PositionCamera(0.0, 2.0, 4.0,
                        0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0);

  	// nebbia
	if (!blackFog)
	{
		fogMgr = FogMgr::getInstance();
		blackFog = fogMgr->addFog();
		fogMgr->setColor(blackFog, 0, 0, 0);
		fogMgr->setMode(blackFog, FOG_EXP);
		fogMgr->setStart(blackFog, 50.0f);
		fogMgr->setDensity(blackFog, 0.018f);
		fogMgr->setEnd(blackFog, 100.0f);
	}
}

void Viewport3d::resetView()
{	
	init();
	xpos = 0.0f;
	ypos = 0.0f;
	gridSize = 1.0f;

	reproject = true;
}

void Viewport3d::prepareProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)w/h , 0.5f, 200.0f);
	glMatrixMode(GL_MODELVIEW);

}

void Viewport3d::prepareModelView()
{
	glLoadIdentity();
		 
	//glScalef(zoom, zoom, zoom);
	    
	if (!RenderMode.UseCamera)
  {
	  glTranslatef(xpos,ypos,-zoom);
	  glMultMatrixf(&rotMatrix[0][0]);
	
	  //glTranslatef(10,0,20);	// trackball target pos
  }
  else
    gluLookAt(camera.vPosition.x, camera.vPosition.y, camera.vPosition.z,
              camera.vView.x,     camera.vView.y,     camera.vView.z,
              camera.vUp.x,       camera.vUp.y,       camera.vUp.z);
	
	
	if (RenderMode.ShowGrid)
		drawGrid();	
}

void Viewport3d::RotateWithTrackball(int lastx, int lasty, int x, int y)
{
	trackball(lastQuat,
      (2.0f * lastx - w) / w,
      (h - 2.0f * lasty) / h,
      (2.0f * x - w) / w,
      (h - 2.0f * y) / h
      );
   
	add_quats(lastQuat, curQuat, curQuat);
	build_rotmatrix(rotMatrix, curQuat);  
}

void Viewport3d::drawGrid()
{
	glEnable(GL_FOG);
	fogMgr->useFog(blackFog);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	int i;
	
	glBegin(GL_LINES);
	for (i = -100; i<100; i+=5)
	{
		if ((i%25)==0)
			glColor3ub(0,0,255);
		else
			glColor3ub(0,0,150);

		glVertex3f((float)i, 0, -100.0f);
		glVertex3f((float)i, 0, -50.0f);
		glVertex3f((float)i, 0, -50.0f);
		glVertex3f((float)i, 0,  0.0f);
		glVertex3f((float)i, 0,  0.0f);
		glVertex3f((float)i, 0, 50.0f);
		glVertex3f((float)i, 0, 50.0f);
		glVertex3f((float)i, 0, 100.0f);

		glVertex3f(-100.0f, 0, (float)i);
		glVertex3f(-50.0f, 0, (float)i);
		glVertex3f(-50.0f, 0, (float)i);
		glVertex3f(0.0f , 0, (float)i);
		glVertex3f(0.0f, 0, (float)i);
		glVertex3f(50.0f, 0, (float)i);
		glVertex3f(50.0f, 0, (float)i);
		glVertex3f(100.0f , 0, (float)i);
	}
/*
	glColor3ub(0,255,0);
	glVertex3f(0.0f,  0.0f, 0.0f);
	glVertex3f(0.0f , 5.0f, 0.0f);
*/
	glEnd();	
	glDisable(GL_FOG);

	if (RenderMode.Solid)
	{
		glEnable(GL_LIGHTING);
		(RenderMode.Texture)  ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
	}
	else
		glDisable(GL_LIGHTING);	
	
}

void Viewport3d::checkOperation(FXEvent *ev)
{	
	Operation = OP_IDLE;
	if (RightBtn)
	{
		if (LeftBtn)
			Operation = OP_SCALING;
		else
			Operation = OP_ROTATING;		
	}
	else
	if (LeftBtn)
	{
		// panning o picking??
		makeCurrent();			
		if (RenderMode.ShowCtrlPoints && Pick.Click(ev->win_x, ev->win_y, (ev->state&CONTROLMASK) != 0))
		{
			// picking
			Operation = OP_PICK_MOVE;	//in 3d non si muovono punti			
		}
		else
		{			
			Operation = OP_PANNING;
		}
		makeNonCurrent();
		redrawAll();
	}	

	setCorrectCursor();
}


long Viewport3d::onMotion(FXObject* , FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	int x = ev->win_x;
	int y = ev->win_y;
	int lastx = ev->last_x;
	int lasty = ev->last_y;


	bool redraw = true;
   
  if (!RenderMode.UseCamera)
  {	
    switch(Operation)
    {
  	case OP_SCALING:
	  	modifyZoom ((y-lasty)/10.f);		
	  	break;

	  case OP_ROTATING:
		  RotateWithTrackball(lastx, lasty, x,y);
		  break;

	  case OP_PANNING:
		  xpos += ((x-lastx)/15.f);
		  ypos += ((-y+lasty)/15.f);		
		  break;

	  default:
		  redraw = false;
    }
	}
  else
  {	
	  // tengo fermo il mouse!
	  //setCursorPosition(lastx,lasty);

    switch(Operation)
    {
  	case OP_SCALING:	// in questo caso.. su e giu'
	  	camera.FlyCamera((y-lasty)/20.f);
	  	break;

	  case OP_PANNING:
      camera.MoveCamera((-y + lasty) / 50.0f);
      camera.StrafeCamera((x - lastx) / 50.0f);		
		break;

	  case OP_ROTATING:
      camera.HandleMouse((- x + lastx) * 3, (-y + lasty) * 3);
		  break;

	  default:
		  redraw = false;
    }
  }
	
	if (redraw)
		onPaint(NULL, 0, NULL);
	
	return 1;
}

void Viewport3d::drawWorld()
{
	PM->DrawScene3D(RenderMode.ShowCtrlPoints,RenderMode.ShowCtrlPolygon,RenderMode.ShowPatch);																																																																													if (hehe){LetteraX = BitmapO;LetteraY = BitmapO;LetteraZ = BitmapO;}

	// disegno gli assi nell' angolo superiore destro del viewport.

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();	// salvo la vecchia matrice di proiezione

	glLoadIdentity();
	glOrtho(0, w, 0, h, -50, 50);	
	glMatrixMode(GL_MODELVIEW);		
	
	// salvo anche la model view
	glPushMatrix();

	glLoadIdentity();
  glTranslatef(w-30,h-30,-30);


  if (!RenderMode.UseCamera)
    // applico la rotazione
	  glMultMatrixf(&rotMatrix[0][0]);
  else
  {
    vector3 vViewVector = camera.vView - camera.vPosition;
    gluLookAt(0.0,           0.0,           0.0,
              vViewVector.x, vViewVector.y, vViewVector.z,
              camera.vUp.x,  camera.vUp.y,  camera.vUp.z);
  }
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	
	glColor3ub(255,0,0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(20.0f, 0.0f, 0.0f);

	glColor3ub(0,255,0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 20.0f, 0.0f);

	glColor3ub(0,0,255);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 20.0f);

	glEnd();

	glDisable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glColor3ub(255,255,255);
	
	glRasterPos3f(20.0f, 0.0f, 0.0f);glBitmap(8, 8, 0,0, 0,0, LetteraX);
	glRasterPos3f(0.0f, 20.0f, 0.0f);glBitmap(8, 8, 0,0, 0,0, LetteraY);
	glRasterPos3f(0.0f, 0.0f, 20.0f);glBitmap(8, 8, 0,0, 0,0, LetteraZ);

	if(RenderMode.DepthTest)
		glEnable(GL_DEPTH_TEST);
	if(RenderMode.Solid)
		glEnable(GL_LIGHTING);

	// ripristino la matrice di proiezione originale
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);		

	// ripristino la matrice model view originale
	glPopMatrix();
	
}
