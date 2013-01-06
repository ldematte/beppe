/*******************************************************************\
*  Bezier Patch Editor  ---  Editor                                 *
*                                                                   *
*  Questa classe e' il punto di partenza dell'editor. Crea          *
*  l'interfaccia grafica e tutti i servizi (manager) necessari.     *
*  Le operazioni principali sono realizzate attraverso chiamate     *
*  al Patch Manager che si occupa di tenere traccia della scena.    *
\*******************************************************************/

#include "Editor.h"
#include "PatchMgr.h"
#include "EditorBezierPatch.h"
#include "Viewport.h"
#include "Viewport2d.h"
#include "Viewport3d.h"
#include "Trackball.h"
#include "Icons.h"

#include "Logger.h"

//Macro per disegnare un separatore verticale nelle toolbar
//Il parametro e' il puntatore all'oggetto toolbar
#define VERTICAL_SEPARATOR(n) new FXFrame(n, LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,8,0); new FXFrame(n, FRAME_SUNKEN|LAYOUT_CENTER_Y|LAYOUT_FIX_HEIGHT|LAYOUT_FIX_WIDTH,0,0,2,22); new FXFrame(n, LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FIX_WIDTH,0,0,8,0);

//Il nostro solo e unico Editor
EditorWindow *theEditor;

//Funzione richiamabile dall'esterno (p.e. dalla MaterialWindow
//ridesegna tutte le viewport
void redrawAll()
{
	theEditor->redrawAllViewports();
}

//------------ Message Map -------------
FXDEFMAP(EditorWindow) EditorWindowMap[] = 
{
	//________Message_Type_________ID______________________Message_Handler_______	
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_ACTIVATE_SELECTOR,EditorWindow::onActivateSelector),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_ACTIVATE_CREATOR ,EditorWindow::onActivateCreator),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_ACTIVATE_MIRROR,  EditorWindow::onActivateMirror),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_ACTIVATE_ROTATE,  EditorWindow::onActivateRotate),

	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_NEW_PROJECT,      EditorWindow::onNewProject),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_SAVE_PROJECT,     EditorWindow::onSaveAs),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_LOAD_PROJECT,     EditorWindow::onLoad),

	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_CUT,					EditorWindow::onCut),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_COPY,					EditorWindow::onCopy),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_PASTE,				EditorWindow::onPaste),

	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_SELECT_ALL,       EditorWindow::onSelectAll),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_SELECT_NONE,      EditorWindow::onSelectNone),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_SELECT_PATCH,     EditorWindow::onSelectPatch),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_DELETE_PATCH,     EditorWindow::onDeletePatch),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_HIDE,				EditorWindow::onHide),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_UNHIDE_ALL,     EditorWindow::onUnhideAll),

  FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_FLIP_NORMALS,     EditorWindow::onFlipNormals),

	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_APPLY_MATERIAL,   EditorWindow::onApplyMaterial),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_APPLY_TEXTURE,   EditorWindow::onApplyTexture),

	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_COLLAPSE_POINTS,  EditorWindow::onCollapsePoints),

	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_JOIN_C0,  EditorWindow::onJoinPatchC0),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_JOIN_C1,  EditorWindow::onJoinPatchC1),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_SPLIT,    EditorWindow::onSplitPatch),

	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_INCREASE_TASSELLATION,  EditorWindow::onIncreaseTassellation),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_DECREASE_TASSELLATION,  EditorWindow::onDecreaseTassellation),
	
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_SHOW_LOG,		     	EditorWindow::onShowLogWindow),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_SHOW_MATERIALS,   EditorWindow::onShowMatWindow),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_SHOW_TEXTURES,   EditorWindow::onShowTexWindow),
	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_SHOW_ABOUT,   EditorWindow::onShowAboutWindow),

	FXMAPFUNC(SEL_COMMAND,   EditorWindow::ID_RESET_VIEWS,   EditorWindow::onResetViews),
	FXMAPFUNC(SEL_CLOSE,     0,                              EditorWindow::onClose)		
};

// Object implementation
FXIMPLEMENT(EditorWindow, FXMainWindow, EditorWindowMap, ARRAYNUMBER(EditorWindowMap));


// Make some windows...
EditorWindow::EditorWindow(FXApp *app) : FXMainWindow(app,"Editor",NULL,NULL,DECOR_ALL,0,0,800,600,0,0)
{
	FXMenuCommand*			mnuFileNew;
	FXMenuCommand*			mnuFileOpen;
	FXMenuCommand*			mnuFileSave;
	FXMenuCommand*			mnuFileExit;

	FXMenuCommand*			mnuEditSelAll;
	FXMenuCommand*			mnuEditSelNone;
	FXMenuCommand*			mnuEditSelPatch;
	FXMenuCommand*			mnuEditDelPatch;
	FXMenuCommand*			mnuEditHide;
	FXMenuCommand*			mnuEditUnhideAll;

	FXMenuCommand*			mnuToolsMaterial;
	FXMenuCommand*			mnuToolsTexture;

	FXMenuCommand*			mnuWindowLog;

	FXMenuCommand*			mnuHelpAbout;	

	FXToolbar*			 Toolbar;

	LogWin = new LogWindow(this);
	Log = Logger::getInstance();
	Log->setLogWindow(LogWin);

	PM	= PatchMgr::getInstance();
	TexMgr = TextureMgr::getInstance();
	MatMgr = MaterialMgr::getInstance();

	Log->add("Editor :: sto creando la GUI...");

	// Menu bar
	MenuBar=new FXMenubar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
  
	// File menu
	FileMenu = new FXMenuPane(this);

	// Status bar
	StatusBar = new FXStatusbar(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|STATUSBAR_WITH_DRAGCORNER);
	StatusBar->getStatusline()->setNormalText("");


	// ToolBar
	new FXHorizontalSeparator(this,LAYOUT_SIDE_TOP|SEPARATOR_GROOVE|LAYOUT_FILL_X);	
	Toolbar = new FXToolbar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0, 4,4,4,4, 1,0);

	// 4 splitter
	Splitter = new FX4Splitter(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y); //|FOURSPLITTER_TRACKING
	Splitter->setBarSize(3);
	
  //Menu File
	new FXMenuTitle(MenuBar,"&File",NULL,FileMenu);
	mnuFileNew = new FXMenuCommand(FileMenu,"&Nuovo\tCtrl-N\tCrea un nuovo file vuoto.",NULL, this,ID_NEW_PROJECT);
   mnuFileOpen = new FXMenuCommand(FileMenu,"&Apri...\t\tApre un file esistente.",NULL, this, ID_LOAD_PROJECT);
	mnuFileSave = new FXMenuCommand(FileMenu,"&Salva...\tCtrl-S\tSalva il file corrente.",NULL, this,ID_SAVE_PROJECT);
	new FXMenuSeparator(FileMenu);
	mnuFileExit = new FXMenuCommand(FileMenu,"&Esci\tAlt-F4\tEsci dall'editor.",NULL,getApp(),FXApp::ID_QUIT);

  //Menu Edit
	EditMenu = new FXMenuPane(this);
	new FXMenuTitle(MenuBar,"&Modifica",NULL,EditMenu);
	mnuCut = new FXMenuCommand(EditMenu,"&Taglia\tCtrl-X\tTaglia le patch selezionate.",NULL,this,ID_CUT);
	mnuCopy = new FXMenuCommand(EditMenu,"&Copia\tCtrl-C\tCopia le patch selezionate.",NULL,this,ID_COPY);
	mnuPaste = new FXMenuCommand(EditMenu,"&Incolla\tCtrl-V\tIncolla le patch selezionate.",NULL,this,ID_PASTE);
	mnuCut->disable();
	mnuCopy->disable();
	mnuPaste->disable();

	new FXMenuSeparator(EditMenu);
	mnuEditSelAll = new FXMenuCommand(EditMenu,"&Seleziona tutto\tCtrl-A\tSeleziona tutti i punti di controllo.",NULL,this,ID_SELECT_ALL);
	mnuEditSelNone = new FXMenuCommand(EditMenu,"&Annulla selezione\t\tAnnulla la selezione dei punti di controllo.",NULL,this,ID_SELECT_NONE);
	mnuEditSelPatch = new FXMenuCommand(EditMenu,"Seleziona &Patch\tCtrl-P\tSeleziona tutti i punti di controllo di una patch.",NULL,this,ID_SELECT_PATCH);
	mnuEditDelPatch = new FXMenuCommand(EditMenu,"&Cancella Patch\tCtrl-D\tCancella le patch selezionate.",NULL,this,ID_DELETE_PATCH);
	new FXMenuSeparator(EditMenu);
	mnuEditHide = new FXMenuCommand(EditMenu,"&Nascondi\tCtrl-H\tNasconde i punti di controllo selezionati.",NULL,this,ID_HIDE);
	mnuEditUnhideAll = new FXMenuCommand(EditMenu,"&Mostra tutto\tCtrl-U\tMostra tutti i punti di controllo.",NULL,this,ID_UNHIDE_ALL);

  //Menu Strumenti
	ToolsMenu = new FXMenuPane(this);
	new FXMenuTitle(MenuBar,"&Strumenti",NULL,ToolsMenu);
	mnuToolsMaterial = new FXMenuCommand(ToolsMenu,"&Materiali...\t\tPermette di gestire dei materiali.",NULL,this,ID_SHOW_MATERIALS);
	mnuToolsTexture = new FXMenuCommand(ToolsMenu,"&Texture...\t\tOperazioni sulle texture.",NULL,this,ID_SHOW_TEXTURES);	
	
  //Menu Finestre
	WindowMenu = new FXMenuPane(this);
	new FXMenuTitle(MenuBar,"Fi&nestre",NULL,WindowMenu);
	new FXMenuCommand(WindowMenu,"&Resetta tutte le viste\t\tRipristina lo stato di tutti i viewport.", NULL,this, ID_RESET_VIEWS);
	new FXMenuSeparator(WindowMenu);
	new FXMenuCommand(WindowMenu,"&Top  View (XZ)\t1\tIngrandisce la vista dall'alto.",NULL,Splitter, FX4Splitter::ID_EXPAND_TOPLEFT);
	new FXMenuCommand(WindowMenu,"&Front View (XY)\t2\tIngrandisce la vista di fronte.",NULL,Splitter, FX4Splitter::ID_EXPAND_TOPRIGHT);
	new FXMenuCommand(WindowMenu,"&Side View (XY)\t3\tIngrandisce la vista di lato.",NULL,Splitter, FX4Splitter::ID_EXPAND_BOTTOMLEFT);
	new FXMenuCommand(WindowMenu,"3&D View\t4\tIngrandisce la vista 3D (prospettica).",NULL,Splitter, FX4Splitter::ID_EXPAND_BOTTOMRIGHT);
	new FXMenuCommand(WindowMenu,"&All View\t5\tTorna alla visualizzazione normale.",NULL,Splitter, FX4Splitter::ID_EXPAND_ALL);
	new FXMenuSeparator(WindowMenu);
	mnuWindowLog = new FXMenuCommand(WindowMenu,"&Mostra log\tF12\tVisualizza la finestra di log (debug).",NULL,this, ID_SHOW_LOG);

	//Menu Help
	HelpMenu=new FXMenuPane(this);	
	new FXMenuTitle(MenuBar,"&Help",NULL,HelpMenu,LAYOUT_RIGHT);
	mnuHelpAbout = new FXMenuCommand(HelpMenu,"About",NULL,this,ID_SHOW_ABOUT);	
	
  //Il GLVisul mantiene lo stato degli oggetti OpenGL condivisi 
  //attraverso i viewport (texture, call-lists...)
	GLVisual = new FXGLVisual(getApp(), VISUAL_DOUBLEBUFFER);

	
	// creo i quattro viewport
	for (int i = 0; i<4; i++)
	{		
		// verifico se la toolbar va in cima o in fondo...
		bool bottom;
		if (i >= 2)
			bottom = true;
		else
			bottom = false;

		// creo un nome per il viewport i-esimo...
		char *ViewName;
		switch(i)
		{
		case 0: ViewName = "  Top View (XZ)"; break;		
		case 1: ViewName = "  Side View (YZ)"; break;		
		case 2: ViewName = "  Front View (XY)"; break;
		case 3: ViewName = "  3D View"; break;
		}

		// trovo il viewport precedente (eventuale)...
		// questo permette di condividere le stesse display list!
		Viewport *prev;
		if (i!=0)
			prev = View[i-1];
		else
			prev = NULL;

		//------ crea i viewport!
		Log->addf("Editor :: creo viewport #%d =>%s", i+1, ViewName);
		switch(i)
		{
		case 0:
			View[i] = new Viewport2d(Splitter, GLVisual, ViewName, bottom);break;			
		case 1:			
		case 2:
			View[i] = new Viewport2d(Splitter, GLVisual, prev, ViewName, bottom);break;
		case 3:
			View[i] = new Viewport3d(Splitter, GLVisual, prev, ViewName, bottom);break;
			
		}		
	}
	((Viewport2d*)View[0])->setTopView();
	((Viewport2d*)View[1])->setSideView();
	((Viewport2d*)View[2])->setFrontView();	

  
  //Creo le finestr per i manager e l'About
	AboutWin = new AboutWindow(this, GLVisual);
	MatWin = new MatWindow(this, GLVisual);
	TexWin = new TexWindow(this, GLVisual, View[3]);
	
	//------------------- T O O L B A R ---------------------
	new FXButton(Toolbar,"\tNuovo\tNuovo progetto.",new FXGIFIcon(getApp(),IconNew),this,ID_NEW_PROJECT,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	new FXButton(Toolbar,"\tApri\tCarica un progetto esistente.",new FXGIFIcon(getApp(),IconOpen),this,ID_LOAD_PROJECT,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	new FXButton(Toolbar,"\tSalva\tSalva su file.",new FXGIFIcon(getApp(),IconSave),this,ID_SAVE_PROJECT,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);

	VERTICAL_SEPARATOR(Toolbar);

	butCut = new FXButton(Toolbar,"\tTaglia (Ctrl+X)\tTaglia le patch selezionate.",new FXGIFIcon(getApp(),IconCut),this,ID_CUT,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	butCopy = new FXButton(Toolbar,"\tCopia (Ctrl+C)\tCopia le patch selezionate.",new FXGIFIcon(getApp(),IconCopy),this,ID_COPY,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	butPaste = new FXButton(Toolbar,"\tIncolla (Ctrl+V)\tIncolla le patch selezionate.",new FXGIFIcon(getApp(),IconPaste),this,ID_PASTE,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	butCut->disable();
	butCopy->disable();
	butPaste->disable();

	VERTICAL_SEPARATOR(Toolbar);

	butSelector = new FXButton(Toolbar,"\tSeleziona\tAttiva la modalita' selezione.", new FXGIFIcon(getApp(),IconSelect), this,ID_ACTIVATE_SELECTOR,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	butCreator  = new FXButton(Toolbar,"\tNuova patch\tCrea una nuova Bezier Patch.",new FXGIFIcon(getApp(),IconPatch),this,ID_ACTIVATE_CREATOR,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	butMirror   = new FXButton(Toolbar,"\tRifletti\tRiflette su un piano i punti selezionati.",new FXGIFIcon(getApp(),IconMirror),this,ID_ACTIVATE_MIRROR,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	butRotate   = new FXButton(Toolbar,"\tRuota\tRuota i punti selezionati attorno ad un asse.",new FXGIFIcon(getApp(),IconRotate),this,ID_ACTIVATE_ROTATE,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);

	butMirror->disable();
	butRotate->disable();

	onActivateSelector(NULL,0,NULL);

	VERTICAL_SEPARATOR(Toolbar);

	new FXButton(Toolbar,"\tSeleziona patch\tSeleziona tutti i punti di controllo delle patch selezionate.",new FXGIFIcon(getApp(),IconSelectpatch),this,ID_SELECT_PATCH,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);

	new FXButton(Toolbar,"\tInverti normali\tInverte le normali delle patch selezionate.",new FXGIFIcon(getApp(),IconFlip),this,ID_FLIP_NORMALS,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	new FXButton(Toolbar,"\tAumenta tassellazione\tAumenta la tassellazione delle patch selezionate.",new FXGIFIcon(getApp(),IconInc_tass),this,ID_INCREASE_TASSELLATION,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	new FXButton(Toolbar,"\tDiminuisci tassellzione\tDiminuisce la tassellazione delle patch selezionate.",new FXGIFIcon(getApp(),IconDec_tass),this,ID_DECREASE_TASSELLATION,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	
	new FXButton(Toolbar,"\tUnisci punti\tCollassa tutti i punti selezionati nel loro baricentro.",new FXGIFIcon(getApp(),IconCollapse),this,ID_COLLAPSE_POINTS,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);

	new FXButton(Toolbar,"\tApplica materiale\tApplica un materiale alle patch selezionate.",new FXGIFIcon(getApp(),IconPaint),this,ID_APPLY_MATERIAL,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	new FXButton(Toolbar,"\tApplica texture\tApplica una texture alle patch selezionate.",new FXGIFIcon(getApp(),IconTexture),this,ID_APPLY_TEXTURE,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	
	new FXButton(Toolbar,"\tCancella Patch\tCancella le patch selezionate.",new FXGIFIcon(getApp(),IconDelete),this,ID_DELETE_PATCH,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);

	VERTICAL_SEPARATOR(Toolbar);
	
	new FXButton(Toolbar,"\tUnisci patch C0\tUnisce due patch per i bordi selezionati assicurando continuita' C0.",new FXGIFIcon(getApp(),IconJoin_c0),this,ID_JOIN_C0,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	new FXButton(Toolbar,"\tUnisci patch C1\tUnisce due patch per i bordi selezionati assicurando continuita' C1.",new FXGIFIcon(getApp(),IconJoin_c1),this,ID_JOIN_C1,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	new FXButton(Toolbar,"\tDividi patch\tDivide due patch per i bordi selezionati.",new FXGIFIcon(getApp(),IconSplit),this,ID_SPLIT,FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT|BUTTON_TOOLBAR);
	
	new FXTooltip(getApp());

}

////////////////////////////////////////////////////////////////////
// Clean up  

EditorWindow::~EditorWindow()
{
	Log->add("Editor :: byez!");

	
	delete        FileMenu;
	delete        EditMenu;
	delete        ToolsMenu;
	delete        WindowMenu;
	delete        HelpMenu;
	delete         MenuBar;

	delete			MatWin;
	delete			TexWin;
	delete			LogWin;
	delete			AboutWin;
/*
	delete mnuFileNew; 
	delete mnuFileOpen;
	delete mnuFileSave;
	delete mnuFileExit;

	delete mnuEditSelAll;
	delete mnuEditSelNone;
	delete mnuEditSelPatch;

	delete mnuToolsMaterial;
	delete mnuToolsTexture;

	delete mnuWindowLog;

	delete mnuHelpAbout;
*/
}

////////////////////////////////////////////////////////////////////
// Ottiene informazioni sul sottosistema OpenGL

void EditorWindow::CheckOpenGLStrings()
{
	// abbiamo bisogno di almeno un contesto opengl
	// attivo per eseguire le query
	View[0]->makeCurrent();
	Log->add("Editor :: controllo le informazioni del driver OpenGL");

	Log->addf("Editor :: GL_VENDOR = \"%s\"", glGetString(GL_VENDOR));
	Log->addf("Editor :: GL_RENDERER = \"%s\"", glGetString(GL_RENDERER));
	Log->addf("Editor :: GL_VERSION = \"%s\"", glGetString(GL_VERSION));
	View[0]->makeNonCurrent();
}

////////////////////////////////////////////////////////////////////
// Dopo la creazione, avvia e mostra la finestra
// dell'Editor

void EditorWindow::create()
{
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);

  CheckOpenGLStrings();
  Log->add("Editor :: ready!");

}

////////////////////////////////////////////////////////////////////
// Ridisegna tutti e 4 i viewport, settando i bottoni delle
// toolbar in maniera appropriata

void EditorWindow::redrawAllViewports()
{
	View[0]->redraw();
	View[1]->redraw();
	View[2]->redraw();
	View[3]->redraw();

	// controllo i bottoni copia-incolla
	if (PM->isClipboardEmpty())
	{
		mnuPaste->disable();
		butPaste->disable();
	}
	else
	{
		mnuPaste->enable();
		butPaste->enable();		
	}
	if (PM->isSelectionEmpty())
	{
		mnuCut->disable();
		butCut->disable();
		mnuCopy->disable();
		butCopy->disable();

		if (butMirror->getState() ||
			 butRotate->getState())
          onActivateSelector(NULL,0,NULL);

		butMirror->disable();
		butRotate->disable();

		
	}
	else
	{
		mnuCut->enable();
		butCut->enable();
		mnuCopy->enable();
		butCopy->enable();

		butMirror->enable();
		butRotate->enable();
	}
}

////////////////////////////////////////////////////////////////////
//
// Controlla se il progetto e' stato modificato (chiedendolo al
// Patch Manager). Se lo e', chiede se si vuole salvare.
//
// Valore di ritorno:
//     Se l'utente risponde in modo regolare (portando alla fine 
//     il salvataggio o decidendo di scartare le modifiche) il
//     valore di ritorno e' true.
//     Se l'utente cancella l'operazione, il valore di ritorno 
//     e' false.
//


bool EditorWindow::SaveChanges()
{
  if (PM->isModified())
  {
    FXuint retval = FXMessageBox::question(this, MBOX_YES_NO_CANCEL, "Editor", "Il tuo progetto e' stato modificato.\nVuoi salvare prima che sia troppo tardi?");
    if (retval == MBOX_CLICKED_YES)
    {
      if (!onSaveAs(NULL, 0, NULL))
		    return false;
    }
    else if (retval == MBOX_CLICKED_CANCEL)
      return false;
  }
  return true;
}


/******************************************************************\
|                 M E S S A G E     H A N D L E R S                |
|                                                                  |
|  Gestiscono messeggi (comandi) generati da pulsanti della        |
|  toolbar, da menu e da altre parti dell'editor.                  |
|  Per ogni handler viene data una breve descizione di cosa fa.    |
\******************************************************************/

////////////////////////////////////////////////////////////////////
// Nuovo progetto

long EditorWindow::onNewProject (FXObject* pFXO, FXSelector FXS, void* pv)
{
	// vuoi salvare prima??
  bool Continue = SaveChanges();
  
  if (Continue)
  {
	  View[0]->makeCurrent();
	  PM->ResetWorkspace();
	  View[0]->makeNonCurrent();

	  redrawAllViewports();
  }
	return 1;
}

////////////////////////////////////////////////////////////////////
// Chiusura dell'applicazione

long EditorWindow::onClose(FXObject* pFXO, FXSelector FXS, void* pv)
{
	// vuoi salvare prima??
  bool Continue = SaveChanges();
  
  if (Continue)
  {
    getApp()->exit();
	}
	return 1;
}

////////////////////////////////////////////////////////////////////
// Taglia

long EditorWindow::onCut (FXObject* , FXSelector, void* )
{
	PM->Cut();
	
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Copia

long EditorWindow::onCopy (FXObject* , FXSelector, void* )
{
	PM->Copy();
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Incolla

long EditorWindow::onPaste (FXObject* , FXSelector, void* )
{
	PM->Paste();
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Scelta tool Selezione

long EditorWindow::onActivateSelector (FXObject* , FXSelector, void* )
{
	butSelector->handle(this,MKUINT(ID_CHECK ,SEL_COMMAND),NULL);
	butCreator->handle (this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	butMirror->handle (this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	butRotate->handle (this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);

	for (int i = 0; i<3;i++)
		View[i]->setTool(TOOL_SELECTOR);

	return 1;
}

////////////////////////////////////////////////////////////////////
// Scelta tool Creazione

long EditorWindow::onActivateCreator (FXObject* , FXSelector, void* )
{
	butSelector->handle(this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	butCreator->handle (this,MKUINT(ID_CHECK ,SEL_COMMAND),NULL);
	butMirror->handle (this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	butRotate->handle (this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);

	for (int i = 0; i<3;i++)
		View[i]->setTool(TOOL_CREATOR);

	return 1;
}

////////////////////////////////////////////////////////////////////
// Scelta tool Riflessione

long EditorWindow::onActivateMirror(FXObject* , FXSelector, void* )
{
	if (PM->isSelectionEmpty())		// non dovrebbe mai accadere
		return 1;

	butSelector->handle(this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	butCreator->handle (this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	butMirror->handle (this,MKUINT(ID_CHECK ,SEL_COMMAND),NULL);
	butRotate->handle (this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	
	for (int i = 0; i<3;i++)
		View[i]->setTool(TOOL_MIRROR);
	
	return 1;
}

////////////////////////////////////////////////////////////////////
// Scelta tool Rotazione

long EditorWindow::onActivateRotate(FXObject* , FXSelector, void* )
{
	if (PM->isSelectionEmpty())		// non dovrebbe mai accadere
		return 1;
	
	butSelector->handle(this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	butCreator->handle (this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	butMirror->handle (this,MKUINT(ID_UNCHECK ,SEL_COMMAND),NULL);
	butRotate->handle (this,MKUINT(ID_CHECK ,SEL_COMMAND),NULL);

	for (int i = 0; i<3;i++)
		View[i]->setTool(TOOL_ROTATE);

	return 1;
}

////////////////////////////////////////////////////////////////////
// Applica materiale corrente

long EditorWindow::onApplyMaterial (FXObject* , FXSelector, void* )
{
	unsigned int mat = MatWin->getSelectedMaterial();
	if (mat)
	{
		PM->SetMaterial(mat);
		redrawAllViewports();
	}
	return 1;
}

////////////////////////////////////////////////////////////////////
// Applica texture corrente

long EditorWindow::onApplyTexture (FXObject* , FXSelector, void* )
{
	unsigned int tex = TexWin->getSelectedTexture();
	PM->SetTexture(tex);
	View[3]->redraw();	
	return 1;
}

////////////////////////////////////////////////////////////////////
// Collassa i punti selezionati nel baricentro

long EditorWindow::onCollapsePoints (FXObject* , FXSelector, void* )
{
	PM->CollapsePoints();
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Mostra la finestra di log

long EditorWindow::onShowLogWindow (FXObject* , FXSelector, void* )
{
	LogWin->show(PLACEMENT_OWNER);
	return 1;
}

////////////////////////////////////////////////////////////////////
// Mostra la finestra dei materiali

long EditorWindow::onShowMatWindow (FXObject* , FXSelector, void* )
{
	MatWin->Open();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Mostra la finestra delle texture

long EditorWindow::onShowTexWindow (FXObject* , FXSelector, void* )
{
	TexWin->Open();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Mostra il dialogo About...

long EditorWindow::onShowAboutWindow (FXObject* , FXSelector, void* )
{
	AboutWin->show(PLACEMENT_OWNER);
	return 1;
}

////////////////////////////////////////////////////////////////////
// Seleziona tutti i punti di controllo

long EditorWindow::onSelectAll (FXObject* , FXSelector, void* )
{	
	PM->SelectAll();
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Deseleziona tutti i punti di controllo

long EditorWindow::onSelectNone (FXObject* , FXSelector, void* )
{
	PM->ClearSelection();
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Seleziona tutti i punti di controllo delle patch gia'
// parzialmente selezionate

long EditorWindow::onSelectPatch (FXObject* , FXSelector, void* )
{
  PM->SelectPatch();
  redrawAllViewports();
  return 1;
}

////////////////////////////////////////////////////////////////////
// Nasconde i punti di controllo

long EditorWindow::onHide(FXObject* , FXSelector, void* )
{
	PM->Hide();
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Mostra di nuovo tutti i punti prima nascosti

long EditorWindow::onUnhideAll(FXObject* , FXSelector, void* )
{
	PM->UnhideAll();
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Salva con nome...

long EditorWindow::onSaveAs(FXObject*, FXSelector, void*)
{
  FXFileDialog SaveDialog(this, "Salva progetto");

  FXString FileName;
  
  SaveDialog.setSelectMode(SELECTFILE_ANY);
  SaveDialog.setPatternList("Bezier Patch Project (*.bzp)");
  SaveDialog.setFilename("Project.bzp");
  if(SaveDialog.execute())
  {
    FileName = SaveDialog.getFilename();
    if(FXFile::exists(FileName))
    {
      if(MBOX_CLICKED_NO == FXMessageBox::question(this, MBOX_YES_NO, "Progetto esistente", "Sovrascrivere il progetto: %s?", FileName.text()))
        return 1;
    } 
    Log->addf("Editor :: Sto salvando %s", FileName.text());
    PM->Save(FileName.text());
  }
  else return 0;

  return 1;
}

////////////////////////////////////////////////////////////////////
// Cancella la patch (le patch) selezionata (e)
// Come altrove, abbiamo bisogno di avere un contesto
// OpenGL valido per questa operazione, quindi
// scegliamo un viewport arbitrario (il primo)
// come contesto corrente

long EditorWindow::onDeletePatch(FXObject* o , FXSelector sel, void* pv)
{
	View[0]->makeCurrent();
  PM->DeletePatch();
  View[0]->makeNonCurrent();
  redrawAllViewports();
  return 1;
}

////////////////////////////////////////////////////////////////////
// Flippa le normali

long EditorWindow::onFlipNormals (FXObject* , FXSelector, void* )
{
  PM->FlipNormals();
  redrawAllViewports();
  return 1;
}

////////////////////////////////////////////////////////////////////
// Congiunge 2 patch (C0 e C1)

long EditorWindow::onJoinPatchC0(FXObject* , FXSelector, void* )
{
	JoinPatch(false);
	return 1;
}

long EditorWindow::onJoinPatchC1(FXObject* , FXSelector, void* )
{
	JoinPatch(true);
	return 1;
}

void EditorWindow::JoinPatch(bool c1)
{
	int Patch1, Patch2;
	Direction Dir1, Dir2;

	if(!PM->CheckJoinPatchSelection(Patch1, Dir1, Patch2, Dir2))
	{
		getApp()->beep();
		FXMessageBox::error(this, MBOX_OK, "Editor", "Selezione non valida!");
		return;
	}
	
	if (!PM->JoinPatches(Patch1, Dir1, Patch2, Dir2, c1))
	{
		redrawAllViewports();

		getApp()->beep();
		FXMessageBox::warning(this, MBOX_OK, "Editor", "Impossibile unire correttamente\n le patch, sorry!");
		
		return;
	}
	redrawAllViewports();	
}

////////////////////////////////////////////////////////////////////
// Divide 2 patch prima unite

long EditorWindow::onSplitPatch(FXObject* , FXSelector, void* )
{
	int Patch1, Patch2;
	Direction Dir1, Dir2;

	if(!PM->CheckJoinPatchSelection(Patch1, Dir1, Patch2, Dir2))
	{
		getApp()->beep();
		FXMessageBox::question(this, MBOX_OK, "Editor", "Selezione non valida!");
		return 1;
	}
	PM->SplitPatches(Patch1, Dir1, Patch2, Dir2);
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Aumenta/diminuisce la tassellazione

long EditorWindow::onIncreaseTassellation(FXObject* , FXSelector, void* )
{
	PM->IncreaseTassellation();
	redrawAllViewports();
	return 1;
}

long EditorWindow::onDecreaseTassellation(FXObject* , FXSelector, void* )
{
	PM->DecreaseTassellation();
	redrawAllViewports();
	return 1;
}

////////////////////////////////////////////////////////////////////
// Carica da file

long EditorWindow::onLoad(FXObject*, FXSelector, void*)
{
  bool Continue = SaveChanges();
  
  if (Continue)
  {
    FXString FileName;
    FXFileDialog OpenDialog(this,"Apri Progetto");
    OpenDialog.setSelectMode(SELECTFILE_EXISTING);
    OpenDialog.setPatternList("Bezier Patch Project (*.bzp)");
    //OpenDialog.setCurrentPattern();
    OpenDialog.setFilename("");

    if(OpenDialog.execute())
    {
      FileName = OpenDialog.getFilename();
		View[0]->makeCurrent();
      PM->Load(FileName.text());
		View[0]->makeNonCurrent();
		redrawAllViewports();
    }
  }  
  return 1;
}

////////////////////////////////////////////////////////////////////
// Resetta tutte le viste

long EditorWindow::onResetViews(FXObject* , FXSelector, void* )
{
	for (int i = 0;i<4;++i)
		View[i]->resetView();

	getApp()->beep();
	redrawAllViewports();

	return 1;
}

/*********************************************************************\
          M              A              I               N             |
\*********************************************************************/


// Start the whole thing
int main(int argc,char *argv[])
{
  FXApp application("Editor","Editor");
  application.init(argc,argv);
  theEditor = new EditorWindow(&application);
  application.create();
 
  return application.run();
}

