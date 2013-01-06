/*******************************************************************\
*  Bezier Patch Editor  ---  AboutWindow                            *
*                                                                   *
*  Mostra la finestra about dell'Editor!                            *
\*******************************************************************/

#if !defined(ABOUTWINDOW_H_INCLUDED)
#define ABOUTWINDOW_H_INCLUDED

#include <fx.h>
#include <fx3d.h>

// Include delle libreria FTGL per le font 3D
#include <FTGLExtrdFont.h>
#include <FTGLOutlineFont.h>
#include <FTGLPolygonFont.h>
#include <FTGLTextureFont.h>
#include <FTGLPixmapFont.h>
#include <FTGLBitmapFont.h>


class AboutWindow : public FXDialogBox  
{
	FXDECLARE(AboutWindow)

public:
	enum 
	{
		ID_OK = FXMainWindow::ID_LAST,
    ID_CANVAS,
    ID_IDLE
	};

	AboutWindow(FXMainWindow* owner, FXGLVisual *vis);
	virtual ~AboutWindow();

  // Event handlers
	long onKeyPress(FXObject* , FXSelector, void* );
	long onOGLPaint (FXObject* , FXSelector, void* );
	long onOGLConfigure (FXObject* , FXSelector, void* );
	long onIdle(FXObject* , FXSelector, void* );
	long onClose(FXObject* , FXSelector, void* );
	long onCmdOK(FXObject* , FXSelector, void* );

protected:
	AboutWindow() {} // serve alla gui

private:
	FXButton* butOk;
  
  // l'handle al nostro idle task (il disegno delle scritte)
	FXChore* IdleHandle;

	bool GLInitialized;

 	FXGLVisual* GLVisual;
 	FXGLCanvas* OGLCanvas;

  FTGLTextureFont* font;
  
	GLuint BlurTexture;

	long startTime;
	int nShow;

	// Setta una vista ortografica
  void ViewOrtho(int w, int h);
  
  // Setta una vista prospettica
	void ViewPerspective(int w, int h);
  
  // Renderizza la scena su una texture
	void RenderToTexture(int w, int h);			
  
  // Renderizza la scena
	void DrawWorld();
  
  // Renderizza le luci volumetriche
	void DrawBlur(int times, float inc, int w, int h);

	float delta;
};

#endif // !defined(ABOUTWINDOW_H_INCLUDED)
