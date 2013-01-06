/*******************************************************************\
*  Bezier Patch Editor  ---  MatWindow                              *
*                                                                   *
*  Una finestra di dialogo per gestire materiali. Utilizza le       *
*  funzioni messe a disposizione dal Material Manager.              *
\*******************************************************************/

#include "MatWindow.h"

// funzione dell'editor per ridisegnare tutte le viewport
extern void redrawAll();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//------------ Message Map -------------
FXDEFMAP(MatWindow) MatWindowMap[] = 
{
	//________Message_Type_________ID______________________Message_Handler_______	
	FXMAPFUNC(SEL_PAINT,		MatWindow::ID_CANVAS,MatWindow::onOGLPaint),
	FXMAPFUNC(SEL_CONFIGURE,	MatWindow::ID_CANVAS,MatWindow::onOGLConfigure),
	
	FXMAPFUNC(SEL_CHANGED,   MatWindow::ID_COLOR,MatWindow::onChgColor),
	FXMAPFUNC(SEL_CHANGED,   MatWindow::ID_LUMINOSITY,MatWindow::onChgLuminosity),
	FXMAPFUNC(SEL_CHANGED,   MatWindow::ID_SPECULAR,MatWindow::onChgSpecular),
	FXMAPFUNC(SEL_CHANGED,   MatWindow::ID_SHININESS,MatWindow::onChgShininess),
	FXMAPFUNC(SEL_CHANGED,   MatWindow::ID_EMISSION,MatWindow::onChgEmission),
	FXMAPFUNC(SEL_CHANGED,   MatWindow::ID_TRASPARENCY,MatWindow::onChgTrasparency),

	FXMAPFUNC(SEL_CLOSE,		0,MatWindow::onCmdClose),
	FXMAPFUNC(SEL_COMMAND,   MatWindow::ID_MAT_SELECTED,MatWindow::onMatSelected),
	FXMAPFUNC(SEL_COMMAND,   MatWindow::ID_NEW, MatWindow::onCmdNew),
	FXMAPFUNC(SEL_COMMAND,   MatWindow::ID_OK, MatWindow::onCmdOk),
	FXMAPFUNC(SEL_COMMAND,   MatWindow::ID_DELETE, MatWindow::onCmdDelete),

	FXMAPFUNC(SEL_COMMAND,   MatWindow::ID_NAME, MatWindow::onCmdName)
};

FXIMPLEMENT(MatWindow, FXDialogBox, MatWindowMap, ARRAYNUMBER(MatWindowMap));

//////////////////////////////////////////////////////////////////////
// Constructor / Destructor

MatWindow::MatWindow(FXMainWindow* owner, FXGLVisual *vis) : FXDialogBox(owner, "Materials", DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE,0,0,400,400,0,0,0,0)
{
  // Inizializza i puntatori ai manager
	MM = MaterialMgr::getInstance();
	PM = PatchMgr::getInstance();

	GLVisual = vis;
	pSphere = gluNewQuadric();

  // Crea i widget dell'interfaccia grafica
	FXHorizontalFrame *win=new FXHorizontalFrame(this,LAYOUT_FILL_Y|LAYOUT_FILL_X,0,0,0,0, 0,0,10,0);

	FXGroupBox* matGroup=new FXGroupBox(win,"Elenco materiali:", FRAME_GROOVE|LAYOUT_FILL_Y,0,0,0,0, 4,4,4,4, 4,4);
	FXHorizontalFrame *matButtonGroup=new FXHorizontalFrame(matGroup,LAYOUT_SIDE_BOTTOM|PACK_UNIFORM_WIDTH|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0, 4,4);
	butNew = new FXButton(matButtonGroup,"Nuovo\tNuovo materiale",NULL,this,ID_NEW,FRAME_RAISED|FRAME_THICK|LAYOUT_LEFT);
	butDelete = new FXButton(matButtonGroup,"Elimina\tElimina material",NULL,this,ID_DELETE,FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT);
	FXVerticalFrame* listFrame=new FXVerticalFrame(matGroup,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
	lstMaterials = new FXList(listFrame,0,this,ID_MAT_SELECTED,LIST_BROWSESELECT|HSCROLLER_NEVER|LAYOUT_FIX_WIDTH|LAYOUT_FILL_Y, 0,0,120,0);

	FXGroupBox* parametersGroup=new FXGroupBox(win,"Parametri del materiale:",FRAME_GROOVE|LAYOUT_RIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 4,4,4,4, 4,4);
	
	//FXHorizontalFrame *elenco;
	//elenco = new FXHorizontalFrame(parametersGroup,LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0, 4, 4);

	FXMatrix *elenco = new FXMatrix(parametersGroup,2, MATRIX_BY_COLUMNS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0, 4, 4);
	new FXLabel(elenco,"Nome:",NULL,JUSTIFY_LEFT|LAYOUT_FILL_ROW);

	txtName = new FXTextField(elenco,30,this,ID_NAME,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|TEXTFIELD_LIMITED,0,0,0,0, 2,2,1,1);
	
	new FXLabel(elenco,"Rosso:",NULL,JUSTIFY_LEFT);
	sldRed = new FXSlider(elenco, this, ID_COLOR, LAYOUT_FILL_X|LAYOUT_FILL_Y|SLIDER_HORIZONTAL|LAYOUT_FILL_COLUMN);
	sldRed->setRange(0, 255);
	sldRed->setValue(0);
	
	new FXLabel(elenco,"Verde:",NULL,JUSTIFY_LEFT);
	sldGreen = new FXSlider(elenco, this, ID_COLOR, LAYOUT_FILL_X|LAYOUT_FILL_Y|SLIDER_HORIZONTAL|LAYOUT_FILL_COLUMN);
	sldGreen->setRange(0, 255);
	sldGreen->setValue(0);

	new FXLabel(elenco,"Blu:",NULL,JUSTIFY_LEFT);
	sldBlue = new FXSlider(elenco, this, ID_COLOR, LAYOUT_FILL_X|LAYOUT_FILL_Y|SLIDER_HORIZONTAL|LAYOUT_FILL_COLUMN);
	sldBlue->setRange(0, 255);
	sldBlue->setValue(0);

	new FXLabel(elenco,"Luminosita':",NULL,JUSTIFY_LEFT);
	sldLuminosity = new FXSlider(elenco, this, ID_LUMINOSITY, LAYOUT_FILL_X|LAYOUT_FILL_Y|SLIDER_HORIZONTAL|LAYOUT_FILL_COLUMN);
	sldLuminosity->setRange(0, 255);
	sldLuminosity->setValue(0);

	new FXLabel(elenco,"Riflessione:",NULL,JUSTIFY_LEFT);
	sldSpecular = new FXSlider(elenco, this, ID_SPECULAR, LAYOUT_FILL_X|LAYOUT_FILL_Y|SLIDER_HORIZONTAL|LAYOUT_FILL_COLUMN);
	sldSpecular->setRange(0, 255);
	sldSpecular->setValue(0);

	new FXLabel(elenco,"Lucentezza:",NULL,JUSTIFY_LEFT);
	sldShininess = new FXSlider(elenco, this, ID_SHININESS, LAYOUT_FILL_X|LAYOUT_FILL_Y|SLIDER_HORIZONTAL|LAYOUT_FILL_COLUMN);
	sldShininess->setRange(0, 255);
	sldShininess->setValue(0);

	new FXLabel(elenco,"Emissione:",NULL,JUSTIFY_LEFT);
	sldEmission = new FXSlider(elenco, this, ID_EMISSION, LAYOUT_FILL_X|LAYOUT_FILL_Y|SLIDER_HORIZONTAL|LAYOUT_FILL_COLUMN);
	sldEmission->setRange(0, 255);
	sldEmission->setValue(0);

	new FXLabel(elenco,"Trasparenza:",NULL,JUSTIFY_LEFT);
	sldTrasparency = new FXSlider(elenco, this, ID_TRASPARENCY, LAYOUT_FILL_X|LAYOUT_FILL_Y|SLIDER_HORIZONTAL|LAYOUT_FILL_COLUMN);
	sldTrasparency->setRange(0, 255);
	sldTrasparency->setValue(0);

	FXVerticalFrame *matBottom=new FXVerticalFrame(parametersGroup,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 4,4);

	//FXGLVisual *GLVisual = new FXGLVisual(getApp(), VISUAL_DOUBLEBUFFER);	

	FXVerticalFrame* oglFrame=new FXVerticalFrame(matBottom,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
	OGLCanvas = new FXGLCanvas(oglFrame, GLVisual, this, ID_CANVAS, LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0);
	GLInitialized = false;
	
	new FXButton(matBottom,"Ok",NULL,this,ID_OK,FRAME_RAISED|FRAME_THICK|LAYOUT_BOTTOM|LAYOUT_FILL_X);

	//---------------------------
	matSelected = 0;	
	refreshMaterialList();
}

MatWindow::~MatWindow()
{

}

//////////////////////////////////////////////////////////////////////
// Mostra la finestra di dialogo

void MatWindow::Open()
{
	refreshMaterialList();
	show(PLACEMENT_OWNER);
}

//////////////////////////////////////////////////////////////////////
// Aggiorna i controlli (widget)

void MatWindow::updateControls()
{
	if (!matSelected)
	{
		txtName->setText("");		

		sldRed->setValue(0);
		sldGreen->setValue(0);
		sldBlue->setValue(0);
		sldLuminosity->setValue(0);
		sldSpecular->setValue(0);
		sldShininess->setValue(0);
		sldEmission->setValue(0);
		sldTrasparency->setValue(0);

		butDelete->disable();
		txtName->disable();
		sldRed->disable();
		sldGreen->disable();
		sldBlue->disable();
		sldLuminosity->disable();
		sldSpecular->disable();
		sldShininess->disable();
		sldEmission->disable();
		sldTrasparency->disable();		
	}
	else
	{
		const char *matName = MM->getName(matSelected);
		if (matName[0] == '#')
		{
			butDelete->disable();
			txtName->disable();
			sldTrasparency->disable();
		}
		else
		{
			butDelete->enable();
			txtName->enable();
			sldTrasparency->enable();
		}
		sldRed->enable();
		sldGreen->enable();
		sldBlue->enable();
		sldLuminosity->enable();
		sldSpecular->enable();
		sldShininess->enable();
		sldEmission->enable();		

		txtName->setText(matName);
		sldRed->setValue(MM->getColorRed(matSelected));
		sldGreen->setValue(MM->getColorGreen(matSelected));
		sldBlue->setValue(MM->getColorBlue(matSelected));
		sldLuminosity->setValue(MM->getLuminosity(matSelected));
		sldSpecular->setValue(MM->getSpecular(matSelected));
		sldShininess->setValue(MM->getShininess(matSelected));
		sldEmission->setValue(MM->getEmission(matSelected));
		sldTrasparency->setValue(MM->getTrasparency(matSelected));
	}
}

//////////////////////////////////////////////////////////////////////
// Aggiorna la lista dei materiali

void MatWindow::refreshMaterialList()
{
	PatchMgr::MATERIALLIST::iterator i;

	lstMaterials->clearItems();
	for(i = PM->MaterialList.begin(); i != PM->MaterialList.end(); ++i)
		lstMaterials->appendItem(MM->getName((*i)));

	lstMaterials->sortItems();
	
	if (matSelected)
	{
		// provo a riselezionare la texture di prima
		int pos = lstMaterials->findItem(MM->getName(matSelected));
		if (pos != -1)
			lstMaterials->setCurrentItem(pos, TRUE);
	}
	onMatSelected(NULL,0,NULL);
}

/*************************************************************\
|                                                             |
|    MESSAGE HANDLERS                                         |
|                                                             |
| Gestiscono gli eventi dell'interfaccia grafica, come        |
| pressioni dei tasti, spostamento di slider, selezioni       |
| di item da una lista...                                     |
|                                                             |
\*************************************************************/

long MatWindow::onMatSelected (FXObject* , FXSelector, void* )
{
	int item = lstMaterials->getCurrentItem();
	matSelected = MM->findMaterialByName(lstMaterials->getItemText(item).text());
	updateControls();
	onOGLPaint(NULL,0,NULL);
	return 1;
}

long MatWindow::onCmdNew (FXObject* , FXSelector, void* )
{	
	int count = 0;
	
	unsigned int newMat;
	char Buf[32];

	do
	{
		sprintf(Buf,"Nuovo%d", count);
		count++;
	}
	while (MM->findMaterialByName(Buf));


	newMat = MM->addMaterial(Buf);
	PM->MaterialList.push_back(newMat);
	lstMaterials->setFocus();			
	lstMaterials->appendItem(Buf);	
	
	lstMaterials->setCurrentItem(lstMaterials->getNumItems()-1, TRUE);
	lstMaterials->makeItemVisible(lstMaterials->getNumItems()-1);
	
	onMatSelected(NULL, 0, NULL);
	return 1;
}

long MatWindow::onCmdDelete (FXObject* , FXSelector, void* )
{
	PM->RemoveMaterialFromProject(matSelected);
	
	lstMaterials->removeItem(lstMaterials->getCurrentItem());
	matSelected = 0;
	onMatSelected(NULL, 0, NULL);
	return 1;
}

long MatWindow::onCmdName (FXObject* , FXSelector, void* )
{
	int pos;	
	
	if (strlen(txtName->getText().text()) == 0 ||
		txtName->getText().text()[0] == '#' ||
		MM->findMaterialByName(txtName->getText().text()) != 0)
	{
		// stringa non accettata
		txtName->setText(MM->getName(matSelected));
		getApp()->beep();
		return 1;
	}

	pos = lstMaterials->findItem(MM->getName(matSelected));
	lstMaterials->setItemText(pos, txtName->getText().text());

	MM->setName(matSelected, txtName->getText().text());
	return 1;
}

long MatWindow::onCmdClose (FXObject* , FXSelector, void* )
{
	hide();
	redrawAll();
	return 1;
}

long MatWindow::onCmdOk (FXObject* , FXSelector, void* )
{
	hide();
	redrawAll();
	return 1;
}

long MatWindow::onChgColor (FXObject* , FXSelector, void* )
{
	MM->setColor(matSelected,
		          sldRed->getValue(),
                sldGreen->getValue(),
					 sldBlue->getValue());
	onOGLPaint(NULL,0,NULL);
	return 1;	
}

long MatWindow::onChgLuminosity (FXObject* , FXSelector, void* )
{
	MM->setLuminosity(matSelected, sldLuminosity->getValue());
	onOGLPaint(NULL,0,NULL);
	return 1;
}

long MatWindow::onChgSpecular (FXObject* , FXSelector, void* )
{
	MM->setSpecular(matSelected, sldSpecular->getValue());
	onOGLPaint(NULL,0,NULL);
	return 1;
}

long MatWindow::onChgShininess (FXObject* , FXSelector, void* )
{
	MM->setShininess(matSelected, sldShininess->getValue());
	onOGLPaint(NULL,0,NULL);
	return 1;
}

long MatWindow::onChgEmission (FXObject* , FXSelector, void* )
{
	MM->setEmission(matSelected, sldEmission->getValue());
	onOGLPaint(NULL,0,NULL);
	return 1;
}

long MatWindow::onChgTrasparency (FXObject* , FXSelector, void* )
{
	MM->setTrasparency(matSelected, sldTrasparency->getValue());
	onOGLPaint(NULL,0,NULL);
	return 1;
}

long MatWindow::onOGLConfigure (FXObject* , FXSelector, void* )
{
	if (!OGLCanvas->makeCurrent())
		return 0;

	int w = OGLCanvas->getWidth();
	int h = OGLCanvas->getHeight();
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)w/h , 0.5f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	static const GLfloat pos[] = {1.0f, 1.0f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	OGLCanvas->makeNonCurrent();
	return 1;
}

long MatWindow::onOGLPaint (FXObject* , FXSelector, void* )
{
	int x,y;

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
	
	MM->useMaterial(matSelected);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glBegin(GL_QUADS);
	for (y = -4 ; y < 4; y++)
		for (x = -10 ; x < 10; x++)
		{
			unsigned char c = 127+((x+y)&1)*127;
			glColor3ub(c,c,c);			
			glVertex3f(x,y,   0.0f);
			glVertex3f(x+1,y ,0.0f);
			glVertex3f(x+1,y+1,0.0f);
			glVertex3f(x,y+1,0.0f);
		}
	glEnd();	
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);

	glTranslatef(0.0f, 0.0f, -5);
   gluSphere(pSphere, 2.0, 15, 15);

	glFlush();
	OGLCanvas->swapBuffers();
	OGLCanvas->makeNonCurrent();
	return 1;
}

//////////////////////////////////////////////////////
// Ritorna al chiamante l'id dell'ultimo materiale
// selezionato nel dialogo

unsigned int MatWindow::getSelectedMaterial()
{
	return matSelected;
}
