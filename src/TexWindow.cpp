/*******************************************************************\
*  Bezier Patch Editor  ---  TexWindow                              *
*                                                                   *
*  Una finestra di interfaccia per caricare e gestire texture.      *
\*******************************************************************/

#include "TexWindow.h"
#include "Logger.h"

// funzione dell'editor per ridisegnare tutte le viewport
extern void redrawAll();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//------------ Message Map -------------
FXDEFMAP(TexWindow) TexWindowMap[] = 
{
	
	//________Message_Type_________ID______________________Message_Handler_______	
	FXMAPFUNC(SEL_PAINT,		TexWindow::ID_CANVAS,TexWindow::onOGLPaint),
	FXMAPFUNC(SEL_CONFIGURE,	TexWindow::ID_CANVAS,TexWindow::onOGLConfigure),
	FXMAPFUNC(SEL_MOTION,             TexWindow::ID_CANVAS,      TexWindow::onOGLMotion),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,    TexWindow::ID_CANVAS,      TexWindow::onOGLLeftBtnPress),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE,  TexWindow::ID_CANVAS,      TexWindow::onOGLLeftBtnRelease),

	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_OK, TexWindow::onCmdOk),
	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_NEW, TexWindow::onCmdNew),
	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_DELETE, TexWindow::onCmdDelete),
	
	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_TEX_SELECTED,TexWindow::onTexSelected),
	
	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_NAME,TexWindow::onCmdName),
	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_WRAP_S,TexWindow::onCmdWrapS),
	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_WRAP_T,TexWindow::onCmdWrapT),
	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_QUALITY,TexWindow::onCmdQuality),
	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_ENV,TexWindow::onCmdEnvMode),
	FXMAPFUNC(SEL_COMMAND,   TexWindow::ID_MIPMAP,TexWindow::onCmdMipmap),	
	FXMAPFUNC(SEL_CLOSE,		0,TexWindow::onCmdClose)
	/*
	FXMAPFUNC(SEL_COMMAND,   MatWindow::ID_OK, MatWindow::onCmdOk),
	FXMAPFUNC(SEL_COMMAND,   MatWindow::ID_DELETE, MatWindow::onCmdDelete),

	FXMAPFUNC(SEL_COMMAND,   MatWindow::ID_NAME, MatWindow::onCmdName)*/
};

FXIMPLEMENT(TexWindow, FXDialogBox, TexWindowMap, ARRAYNUMBER(TexWindowMap));

TexWindow::TexWindow(FXMainWindow* owner, FXGLVisual *vis, FXGLCanvas *prevOGLCanvas) : FXDialogBox(owner, "Textures", DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE,0,0,450,450,0,0,0,0)
{
	TM = TextureMgr::getInstance();
	PM = PatchMgr::getInstance();
	GLVisual = vis;

	FXHorizontalFrame *win=new FXHorizontalFrame(this,LAYOUT_FILL_Y|LAYOUT_FILL_X,0,0,0,0, 0,0,10,0);

	FXGroupBox* texGroup=new FXGroupBox(win,"Elenco texture:", FRAME_GROOVE|LAYOUT_FILL_Y,0,0,0,0, 4,4,4,4, 4,4);
	FXHorizontalFrame *texButtonGroup=new FXHorizontalFrame(texGroup,LAYOUT_SIDE_BOTTOM|PACK_UNIFORM_WIDTH|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0, 4,4);
	butNew = new FXButton(texButtonGroup," Apri... \tApri un file di texture.",NULL,this,ID_NEW,FRAME_RAISED|FRAME_THICK|LAYOUT_LEFT);
	butDelete = new FXButton(texButtonGroup," Elimina \tElimina material",NULL,this,ID_DELETE,FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT);
	FXVerticalFrame* listFrame=new FXVerticalFrame(texGroup,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
	lstTextures = new FXList(listFrame,0,this,ID_TEX_SELECTED,LIST_BROWSESELECT|HSCROLLER_NEVER|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,120,0);

	FXGroupBox* parametersGroup=new FXGroupBox(win,"Parametri della texture:",FRAME_GROOVE|LAYOUT_RIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 4,4,4,4, 4,4);
		
	FXMatrix *elenco = new FXMatrix(parametersGroup,2, MATRIX_BY_COLUMNS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0, 4, 4);

	new FXLabel(elenco,"File:",NULL,JUSTIFY_LEFT|LAYOUT_FILL_ROW);
	txtFile = new FXTextField(elenco,30,this,ID_NAME,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|TEXTFIELD_LIMITED,0,0,0,0, 2,2,1,1);
	txtFile->setEditable(false);
	txtFile->disable();

	
	new FXLabel(elenco,"Nome:",NULL,JUSTIFY_LEFT|LAYOUT_FILL_ROW);
	txtName = new FXTextField(elenco,30,this,ID_NAME,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|TEXTFIELD_LIMITED,0,0,0,0, 2,2,1,1);

	new FXLabel(elenco,"Wrapping S:",NULL,JUSTIFY_LEFT|LAYOUT_FILL_ROW);
	cmbWrapS = new FXComboBox(elenco ,0,2,this,ID_WRAP_S, COMBOBOX_STATIC|LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
	cmbWrapS->appendItem("GL_REPEAT");
	cmbWrapS->appendItem("GL_CLAMP");	

	new FXLabel(elenco,"Wrapping T:",NULL,JUSTIFY_LEFT|LAYOUT_FILL_ROW);
	cmbWrapT = new FXComboBox(elenco ,0,2,this,ID_WRAP_T, COMBOBOX_STATIC|LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
	cmbWrapT->appendItem("GL_REPEAT");
	cmbWrapT->appendItem("GL_CLAMP");	

	new FXLabel(elenco,"Qualita':",NULL,JUSTIFY_LEFT|LAYOUT_FILL_ROW);
	cmbQuality = new FXComboBox(elenco ,0,4,this,ID_QUALITY, COMBOBOX_STATIC|LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
	cmbQuality->appendItem("Nearest (fastest)");
	cmbQuality->appendItem("Linear");
	cmbQuality->appendItem("Bilinear");
	cmbQuality->appendItem("Trilinear (nicest)");	

	new FXLabel(elenco,"Mapping:",NULL,JUSTIFY_LEFT|LAYOUT_FILL_ROW);	
	cmbEnvMode = new FXComboBox(elenco ,0,3,this,ID_ENV, COMBOBOX_STATIC|LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN);
	cmbEnvMode->appendItem("GL_REPLACE");
	cmbEnvMode->appendItem("GL_DECAL");
	cmbEnvMode->appendItem("GL_MODULATE");	
	
	new FXLabel(elenco,"Mipmapped:",NULL,JUSTIFY_LEFT|LAYOUT_FILL_ROW);
	chkMipmapped = new FXCheckButton(elenco, "" ,this,ID_MIPMAP,LAYOUT_FILL_COLUMN|LAYOUT_CENTER_Y);

	new FXLabel(elenco,"Utilizzo cache:",NULL,JUSTIFY_LEFT|LAYOUT_FILL_ROW);
	prgCachesize = new FXProgressBar(elenco,NULL, 0,PROGRESSBAR_PERCENTAGE|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_COLUMN);
	prgCachesize->showNumber();
	

	FXVerticalFrame *txtBottom=new FXVerticalFrame(parametersGroup,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 4,4);

	FXVerticalFrame* oglFrame=new FXVerticalFrame(txtBottom,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
	
	OGLCanvas = new FXGLCanvas(oglFrame, GLVisual, prevOGLCanvas, this, ID_CANVAS, LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0);

	new FXButton(txtBottom,"Ok",NULL,this,ID_OK,FRAME_RAISED|FRAME_THICK|LAYOUT_BOTTOM|LAYOUT_FILL_X);

	GLInitialized = false;
	LeftBtn = false;
	texSelected = 0;	
	updateControls();
}

TexWindow::~TexWindow()
{

}

void TexWindow::Open()
{
	trackball(curQuat, 0.0, 0.0, 0.0, 0.0);
	trackball(lastQuat, 0.0, 0.0, 0.0, 0.0);

	//identita'
	memset(&rotMatrix[0][0], 0, 16*sizeof(float));
	rotMatrix[0][0] = 1.0f;
	rotMatrix[1][1] = 1.0f;
	rotMatrix[2][2] = 1.0f;
	rotMatrix[3][3] = 1.0f;

	refreshTextureList();
	updateMemoryUsage();

	show(PLACEMENT_OWNER);
}

unsigned int TexWindow::getSelectedTexture()
{
	return texSelected;
}

long TexWindow::onCmdOk (FXObject* , FXSelector, void* )
{
	hide();
	redrawAll();
	return 1;
}

long TexWindow::onCmdNew (FXObject* , FXSelector, void* )
{
	
	FXFileDialog OpenDialog(this,"Apri Texture");
	OpenDialog.setSelectMode(SELECTFILE_EXISTING);	  

	OpenDialog.setPatternList("TGA 24-bit non compressi (*.tga)");
	//OpenDialog.setCurrentPattern();
	OpenDialog.setFilename("");

   if(!OpenDialog.execute())
		return 1;

	FXString FileName;
	FXString UserName;
  
  FileName = OpenDialog.getFilename(); 
  FileName = FXFile::relative(FXFile::getCurrentDirectory(), FileName);

  UserName = FileName;
  Logger::getInstance()->addf("Editor :: Texture name %s", FileName.text());

	int count = 0;
	
	char Buf[32];
	do
	{
		sprintf(Buf,"Nuova%d", count);
		count++;
	}
	while (TM->findTextureByUserName(Buf));

	OGLCanvas->makeCurrent();

	unsigned int newTex = TM->addResource(FileName.text(), Buf);
	if (!TM->texLoad(newTex))
	{
		TM->remResource(newTex);
		getApp()->beep();
		FXMessageBox::error(this, MBOX_OK, "Editor", "La texture non e' valida!");
		return 1;
	}

	OGLCanvas->makeNonCurrent();

	PM->TextureList.push_back(newTex);

	lstTextures->setFocus();
	lstTextures->appendItem(Buf);	
	
	lstTextures->setCurrentItem(lstTextures->getNumItems()-1, TRUE);
	lstTextures->makeItemVisible(lstTextures->getNumItems()-1);

	onTexSelected(NULL, 0, NULL);

	updateMemoryUsage();
	return 1;
}

long TexWindow::onCmdDelete (FXObject* , FXSelector, void* )
{
  OGLCanvas->makeCurrent();
	PM->RemoveTextureFromProject(texSelected);
  OGLCanvas->makeNonCurrent();
  
	lstTextures->removeItem(lstTextures->getCurrentItem());
	texSelected = 0;
	onTexSelected(NULL, 0, NULL);

	updateMemoryUsage();
	return 1;
}

long TexWindow::onTexSelected (FXObject* , FXSelector, void* )
{
	int item = lstTextures->getCurrentItem();
	if (item > -1)
		texSelected = TM->findTextureByUserName(lstTextures->getItemText(item).text());

	updateControls();
	onOGLPaint(NULL,0,NULL);
	return 1;
}

void TexWindow::updateControls()
{
	if (!texSelected)
	{
		txtFile->setText("");
		txtName->setText("");
		
		butDelete->disable();
		txtName->disable();
		cmbWrapS->disable();
		cmbWrapT->disable();
		cmbQuality->disable();
		cmbEnvMode->disable();
		chkMipmapped->disable();
	}
	else
	{
		const char *texName = TM->getUserName(texSelected);
		const char *texFile = TM->getFileName(texSelected);

		butDelete->enable();
		txtName->enable();
		
		cmbWrapS->enable();
		cmbWrapT->enable();
		cmbQuality->enable();
		cmbEnvMode->enable();
		chkMipmapped->enable();

		txtFile->setText(texFile);
		txtName->setText(texName);
		
		cmbWrapS->setCurrentItem(0);
		if (TM->texGetOption(texSelected, TEX_WRAP_S_MODE) == WRAP_CLAMP)
			cmbWrapS->setCurrentItem(1);

		cmbWrapT->setCurrentItem(0);
		if (TM->texGetOption(texSelected, TEX_WRAP_T_MODE) == WRAP_CLAMP)			
			cmbWrapT->setCurrentItem(1);

		switch (TM->texGetOption(texSelected, TEX_QUALITY))
		{
		case QUALITY_NEAREST: cmbQuality->setCurrentItem(0); break;
		case QUALITY_LINEAR: cmbQuality->setCurrentItem(1); break;
		case QUALITY_BILINEAR: cmbQuality->setCurrentItem(2); break;
		case QUALITY_TRILINEAR: cmbQuality->setCurrentItem(3); break;
		}
		
		switch (TM->texGetOption(texSelected, TEX_ENV_MODE))
		{
		case ENV_REPLACE: cmbEnvMode->setCurrentItem(0); break;
		case ENV_DECAL: cmbEnvMode->setCurrentItem(1); break;
		case ENV_MODULATE: cmbEnvMode->setCurrentItem(2); break;
		}

		if (TM->texGetOption(texSelected, TEX_MIPMAPPED))
			chkMipmapped->setCheck(true);
		else
			chkMipmapped->setCheck(false);

	}	
}

long TexWindow::onCmdName (FXObject* , FXSelector, void* )
{
	int pos;	
	FXString Name = txtName->getText();
	Name.trim();

	if (strlen(Name.text()) == 0 ||
		 Name.text()[0] == '#' ||
		 TM->findTextureByUserName(Name.text()) != 0)
	{
		// stringa non accettata
		txtName->setText(TM->getUserName(texSelected));
		getApp()->beep();
		return 1;
	}

	pos = lstTextures->findItem(TM->getUserName(texSelected));
	lstTextures->setItemText(pos, Name.text());

	TM->setUserName(texSelected, Name.text());	
	return 1;
}

long TexWindow::onCmdWrapS (FXObject* , FXSelector, void* )
{
	int val = WRAP_REPEAT;
	if (cmbWrapS->getCurrentItem() != 0)
		val = WRAP_CLAMP;

	OGLCanvas->makeCurrent();
	TM->texOption(texSelected, TEX_WRAP_S_MODE, val);
	OGLCanvas->makeNonCurrent();

	onOGLPaint(NULL,0,NULL);
	return 1;
}

long TexWindow::onCmdWrapT (FXObject* , FXSelector, void* )
{
	int val = WRAP_REPEAT;
	if (cmbWrapT->getCurrentItem() != 0)
		val = WRAP_CLAMP;

	OGLCanvas->makeCurrent();
	TM->texOption(texSelected, TEX_WRAP_T_MODE, val);
	OGLCanvas->makeNonCurrent();

	onOGLPaint(NULL,0,NULL);
	return 1;
}

long TexWindow::onCmdQuality(FXObject* , FXSelector, void* )
{
	int val;
	switch (cmbQuality->getCurrentItem())
	{
	case 0: val = QUALITY_NEAREST; break;
	case 1: val = QUALITY_LINEAR; break;
	case 2: val = QUALITY_BILINEAR; break;
	case 3: val = QUALITY_TRILINEAR; break;
	}

	OGLCanvas->makeCurrent();
	TM->texOption(texSelected, TEX_QUALITY, val);
	OGLCanvas->makeNonCurrent();

	onOGLPaint(NULL,0,NULL);
	return 1;
}

long TexWindow::onCmdEnvMode (FXObject* , FXSelector, void* )
{
	int val;
	switch (cmbEnvMode->getCurrentItem())
	{
	case 0: val = ENV_REPLACE; break;
	case 1: val = ENV_DECAL; break;
	case 2: val = ENV_MODULATE; break;	
	}

	OGLCanvas->makeCurrent();
	TM->texOption(texSelected, TEX_ENV_MODE, val);
	OGLCanvas->makeNonCurrent();
	onOGLPaint(NULL,0,NULL);
	return 1;
}

long TexWindow::onCmdMipmap (FXObject* , FXSelector, void* )
{
	int val = 0;
	if (chkMipmapped->getCheck())
		val = 1;

	OGLCanvas->makeCurrent();
	TM->texOption(texSelected, TEX_MIPMAPPED, val);

	// purtoppo la texture deve essere ricaricata
	TM->texUnload(texSelected);
	TM->texLoad(texSelected);
	updateMemoryUsage();

	OGLCanvas->makeNonCurrent();

	onOGLPaint(NULL,0,NULL);
	return 1;
}

void TexWindow::refreshTextureList()
{
	
	PatchMgr::TEXTURELIST::iterator i;

	lstTextures->clearItems();
	lstTextures->appendItem("#NULL");
	for(i = PM->TextureList.begin(); i != PM->TextureList.end(); ++i)
		lstTextures->appendItem(TM->getUserName((*i)));

	lstTextures->sortItems();
	if (lstTextures->getNumItems() > 0)
	{		
		if (texSelected)
		{
			// provo a riselezionare la texture di prima
			int pos = lstTextures->findItem(TM->getUserName(texSelected));
			if (pos != -1)
				lstTextures->setCurrentItem(pos, TRUE);
		}
		onTexSelected(NULL,0,NULL);	
	}
}

long TexWindow::onOGLConfigure (FXObject* , FXSelector, void* )
{
	if (!OGLCanvas->makeCurrent())
	return 0;

	w = OGLCanvas->getWidth();
	h = OGLCanvas->getHeight();
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)w/h , 0.5f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	
	static const GLfloat pos[] = {1.0f, 1.0f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	OGLCanvas->makeNonCurrent();	
	return 1;
}

long TexWindow::onOGLPaint (FXObject* , FXSelector, void* )
{
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
	
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	if (texSelected)
		TM->texUse(texSelected);
	else
		TM->texOff();

	glColor3ub(255,255,255);

	glTranslatef(0.0f,0.0f,-50.0f);
	glMultMatrixf(&rotMatrix[0][0]);

	//gluLookAt(0.0f, 40.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(-50.0f, 50.0f, 0.0f);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, -50.0f, 0.0f);
	glEnd();
	

	
	glFlush();
	OGLCanvas->swapBuffers();
	OGLCanvas->makeNonCurrent();
	return 1;
}

long TexWindow::onOGLLeftBtnPress(FXObject* , FXSelector, void* )
{
	LeftBtn = true;
	return 1;
}

long TexWindow::onOGLLeftBtnRelease(FXObject* , FXSelector, void* )
{
	LeftBtn = false;
	return 1;
}

long TexWindow::onOGLMotion(FXObject* , FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	int x = ev->win_x;
	int y = ev->win_y;
	int lastx = ev->last_x;
	int lasty = ev->last_y;

	if (!LeftBtn)
		return 1;

	// utilizzo la trackball
	trackball(lastQuat,
      (2.0f * lastx - w) / w,
      (h - 2.0f * lasty) / h,
      (2.0f * x - w) / w,
      (h - 2.0f * y) / h
      );
   
	add_quats(lastQuat, curQuat, curQuat);
	build_rotmatrix(rotMatrix, curQuat);  

	onOGLPaint(NULL,0,NULL);
	return 1;
}


long TexWindow::onCmdClose (FXObject* , FXSelector, void* )
{
	hide();
	redrawAll();
	return 1;
}

void TexWindow::updateMemoryUsage()
{
	prgCachesize->setTotal(TM->getCacheSize());
	prgCachesize->setProgress(TM->getUsedSpace());
}