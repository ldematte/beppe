/*******************************************************************\
*  Bezier Patch Editor  ---  Viewport                               *
*                                                                   *
*  Fornisce funzionalita' generiche di un Viewport.                 *
*  Crea una finestra opengl, gestisce l'input del mouse e crea      *
*  una toolbar con alcuni pulsanti di controllo per regolare la     *
*  modalita' di visualizzazione. Inoltre visualizza una griglia     *
*  per facilitare l'editing.                                        *
\*******************************************************************/

#include "Viewport.h"
#include "Icons.h"

#define TOOLBAR_SEPARATOR(x) new FXFrame(x, LAYOUT_TOP|LAYOUT_RIGHT|LAYOUT_FIX_WIDTH,0,0,8,0); new FXFrame(x, FRAME_SUNKEN|LAYOUT_CENTER_Y|LAYOUT_FIX_HEIGHT|LAYOUT_FIX_WIDTH|LAYOUT_RIGHT,0,0,2,22); new FXFrame(x, LAYOUT_TOP|LAYOUT_RIGHT|LAYOUT_FIX_WIDTH,0,0,8,0);


//------------ Message Map -------------
FXDEFMAP(Viewport) ViewportMap[] = 
{
	//________Message_Type_________ID______________________Message_Handler_______
	
	FXMAPFUNC(SEL_PAINT,              0,      Viewport::onPaint),
	FXMAPFUNC(SEL_MOTION,             0,      Viewport::onMotion),
	FXMAPFUNC(SEL_CONFIGURE,          0,      Viewport::onConfigure),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,    0,      Viewport::onLeftBtnPress),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE,  0,      Viewport::onLeftBtnRelease),
	FXMAPFUNC(SEL_RIGHTBUTTONPRESS,   0,      Viewport::onRightBtnPress),
	FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, 0,      Viewport::onRightBtnRelease),

	
	FXMAPFUNC(SEL_COMMAND,  Viewport::ID_CONTROL_POINTS,	Viewport::onCmdControlPoints),
	FXMAPFUNC(SEL_COMMAND,  Viewport::ID_CONTROL_POLYGON,	Viewport::onCmdControlPolygon),
	FXMAPFUNC(SEL_COMMAND,  Viewport::ID_PATCH,           Viewport::onCmdPatch),
  FXMAPFUNC(SEL_COMMAND,  Viewport::ID_CULL,            Viewport::onCmdCullFace),
  FXMAPFUNC(SEL_COMMAND,  Viewport::ID_SNAP,            Viewport::onCmdSnap),
  FXMAPFUNC(SEL_COMMAND,  Viewport::ID_GRID_PLUS,       Viewport::onCmdGridPlus),
  FXMAPFUNC(SEL_COMMAND,  Viewport::ID_GRID_MINUS,      Viewport::onCmdGridMinus),
  FXMAPFUNC(SEL_COMMAND,  Viewport::ID_SHOW_GRID,       Viewport::onCmdShowHideGrid),
  FXMAPFUNC(SEL_COMMAND,  Viewport::ID_SOLID,           Viewport::onCmdSolid),
  FXMAPFUNC(SEL_COMMAND,  Viewport::ID_TEXTURE,           Viewport::onCmdTexture),
  FXMAPFUNC(SEL_COMMAND,  Viewport::ID_CAMERA_TRACKBALL,Viewport::onCmdCameraTrackball)

};

// Object implementation
FXIMPLEMENT(Viewport, FXGLCanvas, ViewportMap, ARRAYNUMBER(ViewportMap));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



Viewport::Viewport(FXComposite* p, FXGLVisual *vis, const char* title, bool bottomToolbar) : FXGLCanvas(p, vis, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0)
{
	init(p, vis, title, bottomToolbar);
}

Viewport::Viewport(FXComposite* p, FXGLVisual *vis, Viewport *prev, const char* title, bool bottomToolbar) : FXGLCanvas(p, vis, prev, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0)
{
	init(p, vis, title, bottomToolbar);	
}

Viewport::~Viewport()
{
}

void Viewport::init(FXComposite* p, FXGLVisual *vis, const char* title, bool bottomToolbar)
{
	PM = PatchMgr::getInstance();
	
	FXVerticalFrame *vf = new FXVerticalFrame(p, LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN);
	vf->setPadLeft(0);
	vf->setPadTop(0);
	vf->setPadRight(0);
	vf->setPadBottom(0);
	vf->setHSpacing(0);
	vf->setVSpacing(0);

	if (bottomToolbar)
		reparent(vf);

	sideToolbar = new FXHorizontalFrame(vf,LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0, 0,0);

	if (!bottomToolbar)
		reparent(vf);

	new FXLabel(sideToolbar,title,NULL,LAYOUT_LEFT);

	butCPoints		= new FXToggleButton(sideToolbar,"\tPunti di controllo\tMostra o nasconde tutti i punti di controllo.", NULL, new FXGIFIcon(getApp(),IconCpoints_off), new FXGIFIcon(getApp(),IconCpoints), this, ID_CONTROL_POINTS ,FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);
  butCPolygon		= new FXToggleButton(sideToolbar,"\tPoligono di controllo\tMostra o nasconde tutti i poligoni di controllo.", NULL, new FXGIFIcon(getApp(),IconCpoly_off),new FXGIFIcon(getApp(),IconCpoly), this, ID_CONTROL_POLYGON,FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);
  butPatch			= new FXToggleButton(sideToolbar,"\tSuperficie Patch\tMostra o nasconde la superficie di tutte le patch.", NULL, new FXGIFIcon(getApp(),IconDpatch_off), new FXGIFIcon(getApp(),IconDpatch), this,ID_PATCH,FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);

  TOOLBAR_SEPARATOR(sideToolbar);

  butSnapToGrid	= new FXToggleButton(sideToolbar,"\tBlocca alla grigla\tSe attivato permette di muovere i punti solo lungo la griglia.", NULL, new FXGIFIcon(getApp(),IconSnap_off), new FXGIFIcon(getApp(),IconSnap), this,ID_SNAP,FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);
  butCullFace 	= new FXToggleButton(sideToolbar,"\tCull faces\tSe attivato rimuove le facce posteriori.", NULL, new FXGIFIcon(getApp(),IconCull_off), new FXGIFIcon(getApp(),IconCull), this, ID_CULL, FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);
  butGridMinus  = new FXToggleButton(sideToolbar,"\tDiminuisci griglia\tDiminuisce la tassellazione della griglia.", NULL, new FXGIFIcon(getApp(),IconMinus), new FXGIFIcon(getApp(),IconMinus), this, ID_GRID_MINUS, FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);
  butGridPlus   = new FXToggleButton(sideToolbar,"\tAumenta griglia\tAumenta la tassellazione della griglia.", NULL, new FXGIFIcon(getApp(),IconPlus), new FXGIFIcon(getApp(),IconPlus), this, ID_GRID_PLUS, FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);
  butSolid      = new FXToggleButton(sideToolbar,"\tMostra la patch piena o in wireframe\tMostra la patch piena o in wireframe.", NULL, new FXGIFIcon(getApp(),IconSolid_off), new FXGIFIcon(getApp(),IconSolid), this, ID_SOLID, FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);
  butTexture    = new FXToggleButton(sideToolbar,"\tMostra o nasconde le textures\tMostra o nasconde le textures.", NULL, new FXGIFIcon(getApp(),IconTex_off), new FXGIFIcon(getApp(),IconTex), this, ID_TEXTURE, FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);
  butShowGrid   = new FXToggleButton(sideToolbar,"\tMostra o nasconde la griglia\tMostra o nasconde la griglia.", NULL, new FXGIFIcon(getApp(),IconGrid_off), new FXGIFIcon(getApp(),IconGrid), this, ID_SHOW_GRID, FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);

  TOOLBAR_SEPARATOR(sideToolbar);
  
  butCameraTrackball  = new FXToggleButton(sideToolbar,"\tTrackball\tControllo telecamera con trackball.", "\tTelecamera\tControllo telecamera libero.", new FXGIFIcon(getApp(),IconTrackball),new FXGIFIcon(getApp(),IconCam), this, ID_CAMERA_TRACKBALL,FRAME_RAISED|LAYOUT_TOP|LAYOUT_RIGHT|TOGGLEBUTTON_TOOLBAR);

  butCullFace->hide();
  butShowGrid->hide();
  butSolid->hide();
  butTexture->hide();
  butCameraTrackball->hide();
  

  dragCursor = getApp()->getDefaultCursor(DEF_CROSSHAIR_CURSOR);

  Operation = OP_IDLE;
  Tool = TOOL_SELECTOR;

  reproject = true;
  LeftBtn = false;
  RightBtn = false;

  zoom = 50.0f;
  xpos = 0.0f;
  ypos = 0.0f;

  gridSize = 1.0f; 

}

void Viewport::setTool(ToolType t)
{
	Tool = t;
	Operation = OP_IDLE;

	setCorrectCursor();
		
}

void Viewport::redraw()
{
  onPaint(NULL,0,NULL);
}

long Viewport::onPaint(FXObject*,FXSelector,void*)
{
	//---------------- inizializzo la finestra per il paint ------------
	makeCurrent();
		
	w = getWidth();
	h = getHeight();
	glViewport(0, 0, w, h);

	// pulisco lo schermo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (reproject)
	{
		prepareProjection();
		Pick.Reshape();
		reproject = false;
	}
	prepareModelView();

	//------------------------- disegna il mondo -----------------------
	drawWorld();

	//---------------------------- finalizza... ------------------------	

	glFlush();
	swapBuffers();
	makeNonCurrent();	

	return 1;
}

long Viewport::onConfigure(FXObject*,FXSelector,void*)
{
	glInit();
	reproject = true;

	return 1;
}

long Viewport::onLeftBtnPress(FXObject* , FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	LeftBtn = true;	
	
	checkOperation(ev);
	grab();
	
	return 1;
}
long Viewport::onLeftBtnRelease(FXObject* , FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	LeftBtn = false;	

	checkOperation(ev);
  PM->ClearAccum();
	if (!RightBtn)
		ungrab();
	return 1;
}
long Viewport::onRightBtnPress(FXObject* , FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;

	RightBtn = true;	
	checkOperation(ev);
	grab();
	return 1;
}
long Viewport::onRightBtnRelease(FXObject* , FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;

	RightBtn = false;	
	checkOperation(ev);
	if (!LeftBtn)
		ungrab();
	return 1;
}

long Viewport::onCmdControlPoints(FXObject* sender, FXSelector, void* )
{
	RenderMode.ShowCtrlPoints = !RenderMode.ShowCtrlPoints;
	updateButtons();
	redraw();
	return 1;
}

long Viewport::onCmdControlPolygon(FXObject* , FXSelector, void* )
{
	RenderMode.ShowCtrlPolygon = !RenderMode.ShowCtrlPolygon;
	updateButtons();
	redraw();
	return 1;
}

long Viewport::onCmdPatch(FXObject* , FXSelector, void* )
{
	RenderMode.ShowPatch = !RenderMode.ShowPatch;
	updateButtons();
   glInit();
	redraw();
	return 1;
}

long Viewport::onCmdCullFace(FXObject* , FXSelector, void* )
{
  RenderMode.Cullface = !RenderMode.Cullface;
  updateButtons();
	glInit();
  redraw();
	return 1;
}

long Viewport::onCmdSnap(FXObject* , FXSelector, void* )
{
  RenderMode.SnapToGrid = !RenderMode.SnapToGrid;
  updateButtons();
	return 1;
}

long Viewport::onCmdGridPlus(FXObject* , FXSelector, void* )
{
  if (gridSize > 0.1f)
  {
    gridSize /= 2.0f;
    redraw();
  }
  return 1;
}

long Viewport::onCmdGridMinus(FXObject* , FXSelector, void* )
{
  if (gridSize < 4.0f)
  {
    gridSize *= 2.0f;
    redraw();
  }
  return 1;
}

long Viewport::onCmdShowHideGrid(FXObject* , FXSelector, void* )
{
	RenderMode.ShowGrid = !RenderMode.ShowGrid;
	updateButtons();
	redraw();
	return 1;
}

long Viewport::onCmdSolid(FXObject* , FXSelector, void* )
{
	RenderMode.Solid = !RenderMode.Solid;
	updateButtons();
   glInit();
	redraw();
	return 1;
}

long Viewport::onCmdTexture(FXObject* , FXSelector, void* )
{
	RenderMode.Texture = !RenderMode.Texture;
	updateButtons();
   glInit();
	redraw();
	return 1;
}

long Viewport::onCmdCameraTrackball(FXObject* , FXSelector, void* )
{
	if (butCameraTrackball->getState())
		RenderMode.UseCamera = true;
	else
		RenderMode.UseCamera = false;
  redraw();
  return 1;
}


void Viewport::prepareProjection()
{
}

void Viewport::prepareModelView()
{
}

void Viewport::glInit()
{
	// Esegue dei comandi opengl per rendere attive le impostazioni
	// nella struct RenderMode.

	makeCurrent();

	(RenderMode.Texture)  ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
	if (RenderMode.Solid)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_COLOR_MATERIAL);
		//glColor3ub(255,255,255);
	}
	
	(RenderMode.Cullface)  ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	(RenderMode.DepthTest) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	
	glClearColor(RenderMode.ClearColor[0],
	             RenderMode.ClearColor[1],
					 RenderMode.ClearColor[2],
					 RenderMode.ClearColor[3]);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glPointSize(5.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glAlphaFunc(GL_GEQUAL, 0.5f);
	
	makeNonCurrent();
	
}

void Viewport::modifyZoom(float factor)
{
	zoom += factor;
	if (zoom <= 0.0f)
		zoom = 0.01f;
}

void Viewport::setCorrectCursor()
{
	dragCursor = getApp()->getDefaultCursor(DEF_ARROW_CURSOR);
	defaultCursor = getApp()->getDefaultCursor(DEF_ARROW_CURSOR);
	
	if (!RenderMode.UseCamera)
	{
		switch (Operation)
		{
		case OP_IDLE:
			if (Tool == TOOL_MIRROR ||
				Tool == TOOL_ROTATE)
				defaultCursor = getApp()->getDefaultCursor(DEF_CROSSHAIR_CURSOR);
			break;
		case OP_SCALING:
			dragCursor = getApp()->getDefaultCursor(DEF_DRAGH_CURSOR);
				break;
		case OP_ROTATING:
			dragCursor = getApp()->getDefaultCursor(DEF_ROTATE_CURSOR);
			break;
		case OP_PANNING:
			dragCursor = getApp()->getDefaultCursor(DEF_MOVE_CURSOR);
			break;
		case OP_SWEEPING:	
			dragCursor = getApp()->getDefaultCursor(DEF_CROSSHAIR_CURSOR);		
			break;
      default:;
		}
	}
}

void Viewport::updateButtons()
{	
	butCPoints->handle(this,MKUINT((RenderMode.ShowCtrlPoints) ? ID_CHECK:ID_UNCHECK ,SEL_COMMAND),NULL);
	butCPolygon->handle(this,MKUINT((RenderMode.ShowCtrlPolygon) ? ID_CHECK:ID_UNCHECK ,SEL_COMMAND),NULL);
	butPatch->handle(this,MKUINT((RenderMode.ShowPatch) ? ID_CHECK:ID_UNCHECK ,SEL_COMMAND),NULL);	

	butSnapToGrid->handle(this,MKUINT((RenderMode.SnapToGrid) ? ID_CHECK:ID_UNCHECK ,SEL_COMMAND),NULL);
	butCullFace->handle(this,MKUINT((RenderMode.Cullface) ? ID_UNCHECK : ID_CHECK, SEL_COMMAND),NULL);
	
	butShowGrid->handle(this,MKUINT((RenderMode.ShowGrid) ? ID_CHECK : ID_UNCHECK, SEL_COMMAND),NULL);
	butSolid->handle(this,MKUINT((RenderMode.Solid) ? ID_CHECK : ID_UNCHECK, SEL_COMMAND),NULL);
	butTexture->handle(this,MKUINT((RenderMode.Texture) ? ID_CHECK : ID_UNCHECK, SEL_COMMAND),NULL);

	butCameraTrackball->handle(this,MKUINT((RenderMode.UseCamera) ? ID_CHECK:ID_UNCHECK ,SEL_COMMAND),NULL);	
}