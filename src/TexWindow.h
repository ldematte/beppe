/*******************************************************************\
*  Bezier Patch Editor  ---  TexWindow                              *
*                                                                   *
*  Una finestra di interfaccia per caricare e gestire texture.      *
\*******************************************************************/

#if !defined(TEXWINDOW_H_INCLUDED)
#define TEXWINDOW_H_INCLUDED

#include <fx.h>
#include <fx3d.h>
#include <list>

#include "TextureMgr.h"
#include "PatchMgr.h"
#include "Trackball.h"

class TexWindow : public FXDialogBox  
{
	FXDECLARE(TexWindow)

public:
	enum 
	{
		ID_TEX_SELECTED = FXMainWindow::ID_LAST,
		ID_NEW,
		ID_DELETE,
		ID_OK,
		ID_NAME,
		ID_CANVAS,
		ID_WRAP_S,
		ID_WRAP_T,
		ID_QUALITY,
		ID_ENV,
		ID_MIPMAP
	};

	TexWindow(FXMainWindow* owner, FXGLVisual *vis, FXGLCanvas *prevOGLCanvas);
	virtual ~TexWindow();

	long onOGLPaint (FXObject* , FXSelector, void* );
	long onOGLConfigure (FXObject* , FXSelector, void* );
	long onOGLLeftBtnPress(FXObject* , FXSelector, void* );
	long onOGLLeftBtnRelease(FXObject* , FXSelector, void* );
	long onOGLMotion(FXObject* , FXSelector, void* );
	

	long onCmdClose (FXObject* , FXSelector, void* );
	long onCmdOk (FXObject* , FXSelector, void* );
	long onCmdNew (FXObject* , FXSelector, void* );
	long onCmdDelete (FXObject* , FXSelector, void* );
	long onCmdName (FXObject* , FXSelector, void* );
	long onTexSelected (FXObject* , FXSelector, void* );

	long onCmdWrapS (FXObject* , FXSelector, void* );
	long onCmdWrapT (FXObject* , FXSelector, void* );
	long onCmdQuality (FXObject* , FXSelector, void* );
	long onCmdEnvMode (FXObject* , FXSelector, void* );
	long onCmdMipmap (FXObject* , FXSelector, void* );

	void Open();
	unsigned int getSelectedTexture();

protected:
	TexWindow() {}		// serve alla gui
	
	void updateControls();
	void updateMemoryUsage();
	void refreshTextureList();

private:

	TextureMgr *TM;
	PatchMgr *PM;
	
	FXGLVisual *GLVisual;

	bool GLInitialized;

	// id della texture attualmente scelta. 0 --> nessuna
	unsigned int texSelected;

	FXList *lstTextures;

	FXButton *butNew;
	FXButton *butDelete;
	
	FXTextField *txtFile;
	FXTextField *txtName;
	FXComboBox *cmbWrapS;
	FXComboBox *cmbWrapT;
	FXComboBox *cmbQuality;
	FXComboBox *cmbEnvMode;
	FXCheckButton *chkMipmapped;

	FXGLCanvas* OGLCanvas;
	FXProgressBar *prgCachesize;

	// mouse flags
	bool LeftBtn;
	int w,h;

	float curQuat[4];
	float lastQuat[4];	
	float rotMatrix[4][4];
};

#endif // !defined(TEXWINDOW_H_INCLUDED)
