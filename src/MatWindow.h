/*******************************************************************\
*  Bezier Patch Editor  ---  MatWindow                              *
*                                                                   *
*  Una finestra di dialogo per gestire materiali. Utilizza le       *
*  funzioni messe a disposizione dal Material Manager.              *
\*******************************************************************/

#if !defined(MATWINDOW_H_INCLUDED)
#define MATWINDOW_H_INCLUDED

#include <fx.h>
#include <fx3d.h>
#include <list>

#include "MaterialMgr.h"
#include "PatchMgr.h"

class MatWindow : public FXDialogBox  
{
	FXDECLARE(MatWindow)
public:
	enum 
	{
		ID_MAT_SELECTED = FXMainWindow::ID_LAST,
		ID_NEW,
		ID_DELETE,
		ID_OK,
		ID_NAME,
		ID_CANVAS,
		ID_COLOR,
		ID_LUMINOSITY,
		ID_SPECULAR,
		ID_SHININESS,
		ID_EMISSION,
		ID_TRASPARENCY
	};

  // Constructor / Destructor
	MatWindow(FXMainWindow* owner, FXGLVisual *vis);
	virtual ~MatWindow();

  // Ritorna il materiale selezionato
	unsigned int getSelectedMaterial();

  // Message Handlers
	long onCmdClose (FXObject* , FXSelector, void* );	
	long onCmdOk (FXObject* , FXSelector, void* );
	long onMatSelected (FXObject* , FXSelector, void* );
	long onOGLPaint (FXObject* , FXSelector, void* );
	long onOGLConfigure (FXObject* , FXSelector, void* );

	long onChgColor (FXObject* , FXSelector, void* );
	long onChgLuminosity (FXObject* , FXSelector, void* );
	long onChgSpecular (FXObject* , FXSelector, void* );
	long onChgShininess (FXObject* , FXSelector, void* );
	long onChgEmission (FXObject* , FXSelector, void* );
	long onChgTrasparency (FXObject* , FXSelector, void* );

	long onCmdNew (FXObject* , FXSelector, void* );
	long onCmdDelete (FXObject* , FXSelector, void* );
	long onCmdName (FXObject* , FXSelector, void* );	

  // Mostra la finestra di dialogo
	void Open();

protected:

  // Serve alla gui
  MatWindow() {}

  // Per rispondere ai cambiamenti
  
  // Aggiorna i controlli (widget)
	void updateControls();

  // Aggiorna la lista dei materiali
	void refreshMaterialList();

private:

  // Puntatori ai managers
	MaterialMgr *MM;
	PatchMgr *PM;
	

  // OpenGL stuff
	bool GLInitialized;
	GLUquadric* pSphere;

	FXGLVisual *GLVisual;
 	FXGLCanvas* OGLCanvas;

	// id del materiale attualmente scelto. 0 --> nessuno
	unsigned int matSelected;

  // GUI components
	FXList *lstMaterials;

	FXButton *butNew;
	FXButton *butDelete;

	FXTextField *txtName;

	FXSlider	*sldRed;
	FXSlider	*sldGreen;
	FXSlider	*sldBlue;

	FXSlider	*sldLuminosity;
	FXSlider	*sldSpecular;
	FXSlider	*sldShininess;
	FXSlider	*sldEmission;
	FXSlider	*sldTrasparency;

};

#endif // !defined(MATWINDOW_H_INCLUDED)
