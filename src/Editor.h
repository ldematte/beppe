/*******************************************************************\
*  Bezier Patch Editor  ---  Editor                                 *
*                                                                   *
*  Questa classe e' il punto di partenza dell'editor. Crea          *
*  l'interfaccia grafica e tutti i servizi (manager) necessari.     *
*  Le operazioni principali sono realizzate attraverso chiamate     *
*  al Patch Manager che si occupa di tenere traccia della scena.    *
\*******************************************************************/

// Fox toolkit includes
#include <fx.h>
#include <fx3d.h>
#include <fxkeys.h>

//OpenGL includes
#include <GL/gl.h>
#include <GL/glu.h>

#include "Logger.h"
#include "LogWindow.h"
#include "MatWindow.h"
#include "TexWindow.h"
#include "AboutWindow.h"
#include "PatchMgr.h"
#include "TextureMgr.h"
#include "MaterialMgr.h"
#include "Viewport.h"

//La nostra finestra principale
class EditorWindow : public FXMainWindow 
{
	FXDECLARE(EditorWindow)
protected:

	
	//---------- GUI ---------------

	// menu
	FXMenubar*         MenuBar;
	FXMenuPane*        FileMenu;
	FXMenuPane*        EditMenu;
	FXMenuPane*        ToolsMenu;
	FXMenuPane*        WindowMenu;
	FXMenuPane*        HelpMenu;

	FXStatusbar*     StatusBar;
	FX4Splitter*     Splitter;  
	Viewport*        View[4];
	
	FXGLVisual*			GLVisual;

	FXButton*			butSelector;
	FXButton*			butCreator;
	FXButton*			butMirror;
	FXButton*			butRotate;
	
	FXButton*			butCut;
	FXButton*			butCopy;
	FXButton*			butPaste;
	FXMenuCommand*		mnuCut;
	FXMenuCommand*		mnuCopy;
	FXMenuCommand*		mnuPaste;

	//---------- EDITOR ---------------
	
  //Le varie finestre ausiliarie
  //Log, materiali, texture e About
	LogWindow*      LogWin;
	MatWindow*      MatWin;
	TexWindow*      TexWin;
	AboutWindow*    AboutWin;

  //I nostri singleton
	Logger*       Log;
	PatchMgr*     PM;
	TextureMgr*   TexMgr;
	MaterialMgr*  MatMgr;

protected:
	EditorWindow()
	{
	}
	
public:
	enum 
	{
		ID_CANVAS_0 = FXMainWindow::ID_LAST,
		ID_CANVAS_1,
		ID_CANVAS_2,
		ID_CANVAS_3,
		ID_NEW_PROJECT,
    ID_SAVE_PROJECT,
    ID_LOAD_PROJECT,
	 ID_CUT,
	 ID_COPY,
	 ID_PASTE,
		ID_SELECT_ALL,
		ID_SELECT_NONE,
    ID_SELECT_PATCH,
    ID_DELETE_PATCH,
	 ID_HIDE,
	 ID_UNHIDE_ALL,
		ID_ACTIVATE_SELECTOR,
		ID_ACTIVATE_CREATOR,
		ID_ACTIVATE_MIRROR,
		ID_ACTIVATE_ROTATE,	
		ID_APPLY_MATERIAL,
		ID_APPLY_TEXTURE,
		ID_COLLAPSE_POINTS,
		ID_SHOW_LOG,
		ID_SHOW_MATERIALS,
		ID_SHOW_TEXTURES,
		ID_SHOW_ABOUT,
    ID_FLIP_NORMALS,
	 ID_SPLIT,
	 ID_JOIN_C0,
	 ID_JOIN_C1,
	 ID_INCREASE_TASSELLATION,
	 ID_DECREASE_TASSELLATION,
	 ID_RESET_VIEWS
	};

	//--------- Message Handlers -----------
	
	long onNewProject (FXObject* , FXSelector, void* );
long onSaveAs(FXObject*, FXSelector, void*);
  long onLoad(FXObject*, FXSelector, void*);
  
	long onCut (FXObject* , FXSelector, void* );
	long onCopy (FXObject* , FXSelector, void* );
	long onPaste (FXObject* , FXSelector, void* );
  
	long onActivateSelector (FXObject* , FXSelector, void* );
	long onActivateCreator (FXObject* , FXSelector, void* );
	long onActivateMirror(FXObject* , FXSelector, void* );
	long onActivateRotate(FXObject* , FXSelector, void* );

	long onCollapsePoints (FXObject* , FXSelector, void* );
	long onApplyMaterial (FXObject* , FXSelector, void* );
	long onApplyTexture (FXObject* , FXSelector, void* );
	
	long onShowLogWindow (FXObject* , FXSelector, void* );
	long onShowMatWindow (FXObject* , FXSelector, void* );
	long onShowTexWindow (FXObject* , FXSelector, void* );
	long onShowAboutWindow (FXObject* , FXSelector, void* );

	long onFlipNormals (FXObject* , FXSelector, void* );

	long onSelectAll (FXObject* , FXSelector, void* );
	long onSelectNone (FXObject* , FXSelector, void* );
	long onSelectPatch (FXObject* , FXSelector, void* );

	long onDeletePatch(FXObject* , FXSelector, void* );

	long onHide(FXObject* , FXSelector, void* );
	long onUnhideAll(FXObject* , FXSelector, void* );
	
	long onSplitPatch(FXObject* , FXSelector, void* );
	long onJoinPatchC0(FXObject* , FXSelector, void* );
	long onJoinPatchC1(FXObject* , FXSelector, void* );

	long onIncreaseTassellation(FXObject* , FXSelector, void* );
	long onDecreaseTassellation(FXObject* , FXSelector, void* );

	long onResetViews(FXObject* , FXSelector, void* );
  long onClose(FXObject*, FXSelector, void* );

  //Ridisegna il contenuto delle 4 viewport
	void redrawAllViewports();

  //Costruttore/distruttore/creatore
	EditorWindow(FXApp* app);
	virtual ~EditorWindow();
	virtual void create();

private:
  //Ricava le informazioni sul sottositema OpenGL
	void CheckOpenGLStrings();

  //Controlla se ci sono stati cambiamenti nel progetto, 
  //ed eventualmente chiede se si vuole salvarli
	bool SaveChanges();
  
  //Attacca 2 patch
	void JoinPatch(bool c1);	
};
 
