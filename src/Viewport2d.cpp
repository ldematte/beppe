/*******************************************************************\
*  Bezier Patch Editor  ---  Viewport2d                             *
*                                                                   *
*  Estende Viewport e si specializza nella visualizzazione in       *
*  modalita' ortografica. Si puo' creare un Viewport2d con vista    *
*  dall'alto, laterale e frontale.                                  *
\*******************************************************************/

#include "Viewport2d.h"
#include "Icons.h"
#include "Utils.h"

// funzione dell'editor per ridisegnare tutte le viewport
extern void redrawAll();


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Viewport2d::Viewport2d(FXComposite* p, FXGLVisual *vis, const char* title, bool bottomToolbar) : Viewport(p, vis, title, bottomToolbar)
{
	init();
}

Viewport2d::Viewport2d(FXComposite* p, FXGLVisual *vis, Viewport *prev, const char* title, bool bottomToolbar) : Viewport(p, vis, prev, title, bottomToolbar)
{		
	init();
}

Viewport2d::~Viewport2d()
{

}

void Viewport2d::init()
{
	RenderMode.Solid				= false;
	RenderMode.Texture			= false;
	RenderMode.Cullface			= false;
	RenderMode.DepthTest			= false;

	RenderMode.ClearColor[0]	= 0.2f;
	RenderMode.ClearColor[1]	= 0.2f;
	RenderMode.ClearColor[2]	= 0.2f;
	RenderMode.ClearColor[3]	= 1.0f;

	RenderMode.ShowCtrlPoints	= true;
	RenderMode.ShowCtrlPolygon	= true;
	RenderMode.ShowPatch			= false;

	RenderMode.SnapToGrid		= true;
	RenderMode.ShowGrid			= true;

	Pick.Drawing2d();

 	updateButtons();	
}

void Viewport2d::resetView()
{
	init();
	zoom = 50.0f;
	xpos = 0.0f;
	ypos = 0.0f;
	gridSize = 1.0f;

	reproject = true;
}

void Viewport2d::setTopView()
{
	dir = TopView;
}

void Viewport2d::setSideView()
{
	dir = SideView;
}

void Viewport2d::setFrontView()
{
	dir = FrontView;
}


void Viewport2d::prepareProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/zoom, w/zoom, -h/zoom, h/zoom, -400, 400);
	glMatrixMode(GL_MODELVIEW);

}

void Viewport2d::prepareModelView()
{
	glLoadIdentity();	
	
	switch(dir)
	{
	case TopView:
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
//		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(xpos, 100.0f, -ypos);		
		break;
	case SideView:
		glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(-100.0f, ypos, -xpos);		
		break;
	case FrontView:
		glTranslatef(xpos, ypos, 100.0f);
	}		
	
	if (RenderMode.ShowGrid)
	{
		// la griglia deve sempre apparire di fronte
		glPushMatrix();

		switch(dir)
		{
		case TopView:
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			break;
		case SideView:
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			break;
		}
		drawGrid();

		glPopMatrix();
	}	
}

// Disegna la griglia sullo sfondo
void Viewport2d::drawGrid()
{	
	glBegin(GL_LINES);

  int i;

  int upper = (int)(128.f / gridSize);
  int lower = -upper;
  float delta = -128.0f;

  glColor3ub(100,100,100);
	for (i = lower; i <= upper; ++i)
	{
    glVertex3f((float)delta, -128.0f, 0.0f);
		glVertex3f((float)delta, 128.0f, 0.0f);

		glVertex3f(-128.0f, (float)delta, 0.0f);
		glVertex3f(128.0f , (float)delta, 0.0f);

    delta += gridSize;
	}

  glColor3ub(150,150,150);
  for (i = -128; i <= 128; i = i + 8)
	{
    glVertex3f((float)i, -128.0f, 0.0f);
		glVertex3f((float)i, 128.0f, 0.0f);

		glVertex3f(-128.0f, (float)i, 0.0f);
		glVertex3f(128.0f , (float)i, 0.0f);
  }

	glColor3ub(200,200,200);
	glVertex3f(0.0f, -128.0f, 0.0f);
	glVertex3f(0.0f, 128.0f, 0.0f);
	glVertex3f(-128.0f, 0.0f, 0.0f);
	glVertex3f(128.0f, 0.0f, 0.0f);

	glEnd();	
}

// disegna il rettangolo di selezione
void Viewport2d::drawSweep()
{	
	glColor3ub(255,255,0);
	if (Tool == TOOL_MIRROR ||
	    Tool == TOOL_ROTATE)
	{
		float dx = EndSweepWorld.x - StartSweepWorld.x;
		float dz = EndSweepWorld.z - StartSweepWorld.z;
		float len = sqrt(dx*dx+dz*dz);

		if (ALMOST_ZERO(len)) return;
	
		dx *= (500/len);
		dz *= (500/len);

		glBegin(GL_LINES);		
		glVertex3f(StartSweepWorld.x - dx,	StartSweepWorld.z - dz, 0.0f);
		glVertex3f(EndSweepWorld.x + dx,	EndSweepWorld.z + dz, 0.0f);

		if (Tool == TOOL_ROTATE)
		{
			glColor3ub(127,127,255);
			glVertex3f(-1000.0f,	StartSweepWorld.z, 0.0f);
			glVertex3f( 1000.0f,	StartSweepWorld.z, 0.0f);
		}

		glEnd();
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		glVertex3f(StartSweepWorld.x,	StartSweepWorld.z, 0.0f);
		glVertex3f(StartSweepWorld.x,	EndSweepWorld.z, 0.0f);
		glVertex3f(EndSweepWorld.x,	EndSweepWorld.z, 0.0f);
		glVertex3f(EndSweepWorld.x,	StartSweepWorld.z, 0.0f);
		glEnd();
	}
}

void Viewport2d::checkOperation(FXEvent *ev)
{		
	if (RightBtn)
	{
		if (LeftBtn)		
			Operation = OP_SCALING;
		else
			Operation = OP_PANNING;
	}
	else
	if (LeftBtn)
	{
		// picking o sweeping??
		switch(Tool)
		{
		case TOOL_CREATOR:
			Operation = OP_SWEEPING;
			initSweep(ev->win_x, ev->win_y);
			break;
		
		case TOOL_SELECTOR:		
			makeCurrent();			
			if (Pick.VerifySelected(ev->win_x,ev->win_y))	// ha cliccato su un punto già sel
			{
        if (ev->state & CONTROLMASK) // control premuto?
          Pick.Click(ev->win_x, ev->win_y, true);	
        
        // moving (mantengo la selezione corrente)
				Operation = OP_PICK_MOVE;
			}
			else	// ha cliccato su un punto non ancora selezionato
			if (Pick.Click(ev->win_x,ev->win_y,(ev->state&CONTROLMASK) != 0))
			{
				Operation = OP_PICK_MOVE;
			}
			else	// non ha cliccato su alcun punto
			{			
				Operation = OP_SWEEPING;
				initSweep(ev->win_x, ev->win_y);
			}
			break;

		case TOOL_MIRROR:
		case TOOL_ROTATE:
			Operation = OP_SWEEPING;			// in realtà serve per disegnare una retta
			initSweep(ev->win_x, ev->win_y);
			break;
		}
		makeNonCurrent();
		redrawAll();
			
		
	}
	else
	{		
		// non è premuto alcun pulsante
		if (Operation == OP_SWEEPING)
		{
			// fine di un rettangolo di selezione!!
			Operation = OP_IDLE;

			switch(Tool)
			{
			case TOOL_SELECTOR:
				makeCurrent();				
				Pick.Sweep(StartSweepWindowX, StartSweepWindowY, 
				           EndSweepWindowX, EndSweepWindowY,
			              (ev->state&CONTROLMASK) != 0);
				makeNonCurrent();
				break;
			case TOOL_CREATOR:
				FixSweepCoordinates(StartSweepWorld, EndSweepWorld);
				makeCurrent();
				PM->CreatePatch(StartSweepWorld, EndSweepWorld);
            makeNonCurrent();
				break;

			case TOOL_MIRROR:
				// terminata la linea... ora dobbiamo riflettere i punti
				FixSweepCoordinates(StartSweepWorld, EndSweepWorld);
				switch(dir)
				{
				case TopView:	PM->MirrorTopView(StartSweepWorld, EndSweepWorld);break;
				case FrontView:PM->MirrorFrontView(StartSweepWorld, EndSweepWorld);break;
				case SideView:	PM->MirrorSideView(StartSweepWorld, EndSweepWorld);break;
				}				
				break;

			case TOOL_ROTATE:
				// terminata la linea... ora dobbiamo riflettere i punti
				FixSweepCoordinates(StartSweepWorld, EndSweepWorld);
				switch(dir)
				{
				case TopView:	PM->RotateTopView(StartSweepWorld, EndSweepWorld);break;
				case FrontView:PM->RotateFrontView(StartSweepWorld, EndSweepWorld);break;
				case SideView:	PM->RotateSideView(StartSweepWorld, EndSweepWorld);break;
				}				
				break;
			}

			
			
			redrawAll();
		}
		else if (Operation == OP_PICK_MOVE)
		{
			redrawAll();
		}

		Operation = OP_IDLE;
	}
	setCorrectCursor();
}

long Viewport2d::onMotion(FXObject* , FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	int x = ev->win_x;
	int y = ev->win_y;
	int lastx = ev->last_x;
	int lasty = ev->last_y;
	
	bool redraw = true;
	float xvalue, yvalue;

	switch(Operation)
	{
	case OP_SCALING:
		zoom -= (y-lasty)/2.0f;
		if (zoom <= 5.0f)
			zoom = 5.0f;
		else 
		if (zoom >= 400.0f)
			zoom = 400.0f;

		reproject = true;
		break;

	case OP_PANNING:
		xpos += 2.0f*(x-lastx)/zoom;
		ypos -= 2.0f*(y-lasty)/zoom;
		break;

	case OP_SWEEPING:
		// devo sempre ridisegnare...
		// adesso devo rilevare la fine dello sweep
		EndSweepWindowX = x;
		EndSweepWindowY = y;
		ScreenToWorld(EndSweepWindowX, EndSweepWindowY, EndSweepWorld);

		if(RenderMode.SnapToGrid && Tool != TOOL_SELECTOR)
		{
			float dummy;
			RoundTo(EndSweepWorld.x, gridSize, EndSweepWorld.x, dummy);
			RoundTo(EndSweepWorld.y, gridSize, EndSweepWorld.y, dummy);
			RoundTo(EndSweepWorld.z, gridSize, EndSweepWorld.z, dummy);
		}
		break;

	case OP_PICK_MOVE:	// moving
    
    xvalue =  2.0f * (x - lastx) / zoom;
    yvalue = -2.0f * (y - lasty) / zoom;
 
		
    if (!RenderMode.SnapToGrid)
    {
      switch(dir)
      {
		  case TopView:   PM->MovePointsWithDelta(xvalue, 0.0f  , -yvalue); break;
		  case SideView:  PM->MovePointsWithDelta(0.0f  , yvalue, -xvalue); break;
		  case FrontView: PM->MovePointsWithDelta(xvalue, yvalue, 0.0f); break;
      }
    }
    else
    {
	    switch(dir)
      {
		  case TopView:   PM->MovePointsWithDeltaAccum(xvalue, 0.0f  , -yvalue, gridSize); break;
		  case SideView:  PM->MovePointsWithDeltaAccum(0.0f  , yvalue, -xvalue, gridSize); break;
		  case FrontView: PM->MovePointsWithDeltaAccum(xvalue, yvalue, 0.0f, gridSize); break;
      }
    }
		
		break;
	default:
		redraw = false;
	}
	
	if (redraw)
		onPaint(NULL, 0, NULL);

	return 1;
}

void Viewport2d::ScreenToWorld(int sx, int sy, point &p)
{
	// trasforma le coordinate del mouse nel viewport 
	// in coordinate del mondo su un piano xz
	int w2 = w>>1;
	int h2 = h>>1;

	p.x = - xpos + (sx-w2)*2.0f/zoom;
	p.z = - ypos - (sy-h2)*2.0f/zoom;
	p.y = 0.0f;
	
	if(dir == TopView)
		p.z *= -1;
	
}

void Viewport2d::FixSweepCoordinates(point &start, point &end)
{
	// le coordinate in ingresso sono tutte sul piano xz.
	// devo convertirle in base alla vista di questo viewport
	// (per la top view sono gia' ok)

	switch (dir)
	{
	case FrontView:	// aggiusto le coordinate per la front view
		start.y = start.z;
		end.y = end.z;
		start.z = 0.0f;
		end.z = 0.0f;
		break;
	case SideView:		// aggiusto le coordinate per la side view
			
		start.y = start.z;
		end.y = end.z;
		start.z = -start.x;
		end.z = -end.x;
		start.x = 0.0f;
		end.x = 0.0f;
		break;
	}
   
}

void Viewport2d::initSweep(int sx, int sy)
{
	StartSweepWindowX = EndSweepWindowX = sx;
	StartSweepWindowY = EndSweepWindowY = sy;
	ScreenToWorld(StartSweepWindowX, StartSweepWindowY ,StartSweepWorld);
	
	if(RenderMode.SnapToGrid && Tool != TOOL_SELECTOR)
	{
		float dummy;
		RoundTo(StartSweepWorld.x, gridSize, StartSweepWorld.x, dummy);
		RoundTo(StartSweepWorld.y, gridSize, StartSweepWorld.y, dummy);
		RoundTo(StartSweepWorld.z, gridSize, StartSweepWorld.z, dummy);
	}

	EndSweepWorld = StartSweepWorld;
	
}

void Viewport2d::drawWorld()
{
	PM->DrawScene2D(RenderMode.ShowCtrlPoints,RenderMode.ShowCtrlPolygon,RenderMode.ShowPatch);

	// devo disegnare il rettangolo di selezione? (sweep)
	if (Operation == OP_SWEEPING)
	{
		glPushMatrix();

		switch(dir)
		{
		case TopView:
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			break;
		case SideView:
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			break;
		}

		drawSweep();
		glPopMatrix();
	}

}
