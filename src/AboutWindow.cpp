/*******************************************************************\
*  Bezier Patch Editor  ---  AboutWindow                            *
*                                                                   *
*  Mostra la finestra about dell'Editor!                            *
\*******************************************************************/

#include "AboutWindow.h"
#include "Logger.h"
#include "TextureMgr.h"

bool hehe = false;

//Font usato per le scitte 3D
const char fontfile[] = "font.ttf";


//
//Ci serve un meccanismo di timing per muovere le scritte
//a velocita' uniforme su tutti i sistemi....
//Sotto Win usiamo la timeGetTime della multimedia
//library, sotto gli altri sistemi (linux) la 
//famosa gettimeofday
//

#ifdef WIN32
inline unsigned long getTime()
{
  return timeGetTime();
}
#else
#include <sys/time.h>
inline unsigned long getTime()
{
  struct timeval now;
	unsigned long ticks;

	gettimeofday(&now, NULL);
	ticks = (now.tv_sec /*- start.tv_sec*/) * 1000 + 
          (now.tv_usec /*- start.tv_usec*/) / 1000;
	return ticks;

}
#endif


//------------ Message Map -------------
FXDEFMAP(AboutWindow) AboutWindowMap[] = 
{
	//________Message_Type_________ID______________________Message_Handler_______	
	
  FXMAPFUNC(SEL_CHORE,      AboutWindow::ID_IDLE,   AboutWindow::onIdle),
	FXMAPFUNC(SEL_PAINT,      AboutWindow::ID_CANVAS, AboutWindow::onOGLPaint),
	FXMAPFUNC(SEL_CONFIGURE,  AboutWindow::ID_CANVAS, AboutWindow::onOGLConfigure),
 	FXMAPFUNC(SEL_KEYPRESS,   0,                      AboutWindow::onKeyPress),
 	FXMAPFUNC(SEL_CLOSE,      0,                      AboutWindow::onClose),
 	FXMAPFUNC(SEL_COMMAND,    AboutWindow::ID_OK,     AboutWindow::onClose),

}; 

FXIMPLEMENT(AboutWindow, FXDialogBox, AboutWindowMap, ARRAYNUMBER(AboutWindowMap));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AboutWindow::AboutWindow(FXMainWindow* owner, FXGLVisual *vis) : FXDialogBox(owner, "About Bezier Patch Editor", DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE,0,0,250,120,0,0,0,0)
{
	FXVerticalFrame *win=new FXVerticalFrame(this,LAYOUT_FILL_Y|LAYOUT_FILL_X,5,5,5,5, 5,5,2,5);

  GLVisual = vis;

	FXLabel *lbl;	
	lbl = new FXLabel(win,"Bezier Patch Editor",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK);

  FXVerticalFrame* oglFrame = new FXVerticalFrame(win, FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
	OGLCanvas = new FXGLCanvas(oglFrame, GLVisual, this, ID_CANVAS, LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0);
	GLInitialized = false;

	butOk = new FXButton(win,"  Ok  ",NULL, this, ID_OK, FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT|LAYOUT_BOTTOM);	

  BlurTexture = 0;
  delta = 0.0f;
  nShow = 0;
  font = NULL;
  IdleHandle = NULL;
}

AboutWindow::~AboutWindow()
{

}

long AboutWindow::onKeyPress(FXObject* , FXSelector, void* ptr)
{																																																																																												FXEvent *ev=(FXEvent*)ptr;	int x,y; Logger *Log = Logger::getInstance(); if(hehe) return 1;translateCoordinatesTo(x,y,getRoot(),ev->win_x, ev->win_y);	if ((ev->state & CONTROLMASK) && (ev->state & SHIFTMASK) && (x==0 && y==0)) {hehe = true; Log->add("\nConiglietto            Coniglietto\n                            allucinato\n\n     (V)                   (V)\n    =°_°=                 =O_O=\n   (,,)(,,)              (,,)(,,)\n");Log->add("Editor :: nessun animale e' stato maltrattato\nnella produzione di questo editor.");getApp()->beep();}
	// serve all' interfaccia grafica...
	return 1;
}

// Due funzioni ausiliarie per settare una vista ortogonale...
inline void AboutWindow::ViewOrtho(int w, int h)
{
	glMatrixMode(GL_PROJECTION);							
	glLoadIdentity();											
	glOrtho(0, w, h, 0, -10, 10);						
	glMatrixMode(GL_MODELVIEW);									
	glLoadIdentity();		
}

// ...e una vista prospettica
inline void AboutWindow::ViewPerspective(int w, int h)	
{
  glViewport(0, 0, w, h);

 	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)w / (float)h , 0.5f, 300.0f);
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 	glTranslatef(500.0f - delta, -80.0f, -250.0f);
}

// Renderizza la scena e la copia su una texture
inline void AboutWindow::RenderToTexture(int w, int h)											
{
  // Set Our Viewport (Match Texture Size)
	glViewport(0, 0, 128, 128);      
	DrawWorld();												

	glBindTexture(GL_TEXTURE_2D, BlurTexture);         

	// Copy Our ViewPort To The Blur Texture (From 0,0 To 128,128... No Border)
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, 0, 128, 128, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.5);						
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glViewport(0, 0, w, h);  

}

// Sceglie la stringa da renderizzare e la... renderizza!
inline void AboutWindow::DrawWorld()
{
  if (font)
  {
    switch (nShow)
    {
    case 1:
      font->render("Lorenzo Dematte'");
      break;
    case 2:
      font->render("p    r    e    s    e    n    t");
      break;
    case 3 :
      font->render("The Bezier Patch Editor");
      break;                                                                                                                                                                                                                          case 99: font->render("The Ultimate Bezier Patch Editor ^_^'"); break;
    default:
	    font->render("Alessandro Santuari               &");
      break;
    }
  }
}

//
// Renderizza la scena sfumata 
// Parametri: times: numero di volte in cui renderizzare la texture
//                   della scena; piu' volte, piu' lento ma piu' bello!
//                   se avete un pc decente, provate qualcosa come 25
//            inc: l'incremento del canale alpha (diminuizione trasparenza)
//                 ad ogni giro del disegno
//            w, h: larghezza e altezza della finestra
//
inline void AboutWindow::DrawBlur(int times, float inc, int w, int h)
{
	float spost = 0.0f;               // Starting Texture Coordinate Offset
	float alphainc = 0.9f / times;    // Fade Speed For Alpha Blending
	float alpha = 0.3f;              // Starting Alpha Value

	// Disable AutoTexture Coordinates
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_2D);                  
	glDisable(GL_DEPTH_TEST);									
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);					
	glEnable(GL_BLEND);											  
	glBindTexture(GL_TEXTURE_2D, BlurTexture);

	ViewOrtho(w, h);												      

	alphainc = alpha / times;									

	glBegin(GL_QUADS);											  
		for (int num = 0; num < times; ++num) 
		{
			glColor4f(1.0f, 1.0f, 1.0f, alpha);					
			glTexCoord2f(spost, 1 - spost);						
			glVertex2f(0, 0);									

			glTexCoord2f(spost, spost);						
			glVertex2f(0, h);								

			glTexCoord2f(1 - spost, spost);
			glVertex2f(w, h);								

			glTexCoord2f(1 - spost, 1 - spost);			
			glVertex2f(w, 0);									

			spost += inc;										
			alpha = alpha - alphainc;			
		}
	glEnd();				

  ViewPerspective(w, h);	
	

	glEnable(GL_DEPTH_TEST);								
	glDisable(GL_TEXTURE_2D);								
	glDisable(GL_BLEND);									
	glBindTexture(GL_TEXTURE_2D, 0);							
}


//
// Inizilizza i parametri OpenGL
//
long AboutWindow::onOGLConfigure (FXObject* , FXSelector, void* )
{
	if (!OGLCanvas->makeCurrent())
		return 0;

	int w = OGLCanvas->getWidth();
	int h = OGLCanvas->getHeight();
	glViewport(0, 0, w, h);

  glShadeModel(GL_SMOOTH);		
  glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

  if (font)
    delete font;

  font = new FTGLTextureFont;//FTGLPixmapFont;//FTGLTextureFont;//FTGLPolygonFont;
  bool fontInit = false;

  if(!font->Open(fontfile, false))
    Logger::getInstance()->addf("Editor :: Failed to open font %s", fontfile);
  else
  {
    if(!font->FaceSize(144))
      Logger::getInstance()->add("Editor :: Failed to set size");
    
    else
    {
      font->Depth(20);
	    font->CharMap(ft_encoding_unicode);
      fontInit = true;
    }
  }

  if (!fontInit)
  {
    delete font;
    font = NULL;
  }

  if (BlurTexture == 0)
    BlurTexture = TextureMgr::getInstance()->EmptyTexture(128);

  ViewPerspective(w, h);

	OGLCanvas->makeNonCurrent();

	return 1;
}

//
// Gestisce l'evanto di paint (generato quando la nostra finestra e'
// stata "sporcata"
//
long AboutWindow::onOGLPaint (FXObject* , FXSelector, void* )
{
  if (IdleHandle == NULL)
    IdleHandle = getApp()->addChore(this, ID_IDLE);
  startTime = getTime();
	return 1;
}


//
// Idle processing: ovvero cosa fare quando non hai nient'altro
// da fare
//
long AboutWindow::onIdle(FXObject* , FXSelector, void* )
{
  IdleHandle = NULL;

  int w = OGLCanvas->getWidth();
	int h = OGLCanvas->getHeight();

	if (!GLInitialized)
	{
		if(onOGLConfigure(NULL, 0, NULL))
			GLInitialized = true;
		else
			return 0;
	}

	if (!OGLCanvas->makeCurrent())
		return 0;	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
  if (delta > 3100.0f)
  {
    startTime = getTime();
    ++nShow;                                                                                                                                                    
    if (nShow > 3) nShow = 0;                                                                                                                                         if (nShow == 3 && hehe) nShow = 99;
  }
  delta = ((getTime() - startTime) / 4.0f);
  ViewPerspective(w, h);

  glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);


  glColor3f(1.0f, 1.0f, 1.0f);
  RenderToTexture(w, h);	
  DrawBlur(10, 0.03f, w, h);
  
  glColor3ub(255, 240, 32);
  glEnable(GL_TEXTURE_2D);
  DrawWorld();

  
  glFlush();
	OGLCanvas->swapBuffers();
	OGLCanvas->makeNonCurrent();

  IdleHandle = getApp()->addChore(this, ID_IDLE);
	return 1;
}

//
// Gestore della chiusura della finestra, fa un po' di cleanup
//
long AboutWindow::onClose(FXObject* o, FXSelector sel, void* pv)
{
  getApp()->removeChore(IdleHandle);
  IdleHandle = NULL;
  GLInitialized = false;
  
  OGLCanvas->makeCurrent();
  if (BlurTexture)
	  glDeleteTextures(1, &BlurTexture);
  OGLCanvas->makeNonCurrent();
  
  BlurTexture = 0;
  hide();
  return 1;
}







