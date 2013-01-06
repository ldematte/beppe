/*******************************************************************\
*  Bezier Patch Editor  ---  PatchMgr                               *
*                                                                   *
*  Questa classe e' il vero cuore dell'applicazione. Gestigne un    *
*  insieme di patch, di materiali e di texture. Tutte le            *
*  funzionalita' principali dell'editor sono implementate in questa *
*  classe. Spesso i comandi disponibili con l'interfaccia grafica   *
*  sono implementati chiamando una singola funzione del PatchMgr.   *
\*******************************************************************/

#include "common.h"
#include "PatchMgr.h"
#include "Utils.h"
#include <string>
#include <math.h>

#ifndef WIN32	// In linux non esiste la funzione atan2f
inline float atan2f(float x, float y)
{
  return ((float)atan2((double)x, (double)y));
}
#endif //WIN32
  

//////////////////////////////////////////////////////////////////////
// static memebers initialization
//

PatchMgr* PatchMgr::instance = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//

PatchMgr::PatchMgr()
{
  log = Logger::getInstance();
  log->add("PatchMgr :: inizializzato!");

  SphereCallList = 0;	// verra' creata dopo

  // Prendo l'istanza di alcuni manager...
  MM = MaterialMgr::getInstance();
  TM = TextureMgr::getInstance();

  // Creo il colore "Selezionato" (ROSSO)
  matSel = MM->addMaterial("#Selezionato");
  MM->setColor(matSel, 255, 0, 0);
  MM->setLuminosity(matSel, 20);
  MM->setSpecular(matSel, 180);
  MM->setShininess(matSel, 80);

  MaterialList.push_back(matSel);

  // Creo il colore "Deselezionato" (BIANCO)
  matDesel = MM->addMaterial("#Deselezionato");
  MM->setColor(matDesel, 255, 255, 255);
  MM->setLuminosity(matDesel, 20);
  MM->setSpecular(matDesel, 50);
  MM->setShininess(matDesel, 80);
  MM->setEmission(matDesel, 20);

  MaterialList.push_back(matDesel);

  // Creo il colore "Legato" (AZZURRO)
  matBinded = MM->addMaterial("#Legato");
  MM->setColor(matBinded, 0, 100, 255);
  MM->setLuminosity(matBinded, 20);
  MM->setSpecular(matBinded, 150);
  MM->setShininess(matBinded, 80);  

  MaterialList.push_back(matBinded);
  
  // Creo il colore "Default" (ORO)
  matDefault = MM->addMaterial("#Default");
  MM->setColor(matDefault, 255, 200, 15);  
  MM->setLuminosity(matDefault, 100);
  MM->setShininess(matDefault, 80);
  MM->setSpecular(matDefault, 255);

  MaterialList.push_back(matDefault);

  // Numero della prossima patch da creare
  nNextPatch = 1;

  // Progetto non modificato
  bModified = false;

  ClearAccum();
}

/*
PatchMgr::~PatchMgr()
{
}
*/

//////////////////////////////////////////////////////////////////////
//
//

// Crea una nuova patch. I punti di controllo vengono posizionati
// in modo da riempire un rettangolo
EditorBezierPatch* PatchMgr::CreatePatch(point tempa, point tempb)
{	
  bModified = true;

	point a = tempa;
	point b = tempb;

	// mi assicuro che il punto a abbia coordinate minori rispetto a b
	if (tempa.x > tempb.x)
	{
		a.x = tempb.x;
		b.x = tempa.x;
	}
	if (tempa.y > tempb.y)
	{
		a.y = tempb.y;
		b.y = tempa.y;
	}
	if (tempa.z > tempb.z)
	{
		a.z = tempb.z;
		b.z = tempa.z;
	}
	
  // calcolo la distanza tra due punti di controllo
  float dx = (b.x - a.x) / 3.0f;
  float dy = (b.y - a.y) / 3.0f;
  float dz = (b.z - a.z) / 3.0f;

  point P[4][4];

  // Controllo se il rettangolo e' troppo piccolo
  if (ALMOST_ZERO(dx) && ALMOST_ZERO(dy) && ALMOST_ZERO(dz))
	  return NULL;

  if (ALMOST_ZERO(dz))
  {
    // genero i punti sul piano XY
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        P[i][j] = point((float)i * dx + a.x, (float)j * dy + a.y, a.z);
  }
  else if (ALMOST_ZERO(dx))
  {
    // genero i punti sul piano YZ
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        P[i][j] = point(a.x, (float)i * dy + a.y, (float)j * dz + a.z);
  }
  else if (ALMOST_ZERO(dy))
  {
    // genero i punti sul piano XZ
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        P[i][j] = point((float)i * dx + a.x, a.y, (float)j * dz + a.z);
  }
  else
  {
    log->add("PatchMgr :: invalid patch!");
    return NULL;
  }

  // Crea la patch
  EditorBezierPatch* patch= new EditorBezierPatch(nNextPatch, matSel, matDesel, matBinded, P, SphereCallList);
  patch->setMaterial(matDefault);

  // Inserisce la patch nel progetto
  patches[nNextPatch] = patch;

  // Incremento il numero della prossima patch da creare
  nNextPatch++;

  return patch;
}

// Ritorna il puntatore ad una patch dato il suo numero
EditorBezierPatch* PatchMgr::getPatch(int nPatch)
{
	return patches[nPatch];
}

// Applica il materiale a tutte le patch selezionate
void PatchMgr::SetMaterial(unsigned int mat)
{
  bModified = true;

  SELECTIONLIST::iterator i;
  PATCHMAP::iterator p;

  for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
  {
    p = patches.find((*i).nPatch);
    if (p != patches.end())
      (*p).second->setMaterial(mat);
  }
}

// Applica la texture a tutte le patch selezionate
void PatchMgr::SetTexture(unsigned int tex)
{
  bModified = true;

  SELECTIONLIST::iterator i;
  PATCHMAP::iterator p;

  for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
  {
    p = patches.find((*i).nPatch);
    if (p != patches.end())
      (*p).second->setTexture(tex);
  }
}

// Disegna la scena per un viewport 2d
void PatchMgr::DrawScene2D(bool drawCPoints, bool drawCPoly, bool drawPatch)
{
	PATCHMAP::iterator i;

	for(i = patches.begin(); i != patches.end(); ++i)
	{
		EditorBezierPatch *patch = (*i).second;
		if (drawPatch)				// devo disegnare la patch?
			patch->DrawPatch();

		if (drawCPoly)				// devo disegnare il poligono di controllo?
		{
			glColor3ub(50,200,50);
			patch->DrawCtrlPolygon();
		}
		if (drawCPoints)			// devo disegnare i punti di controllo?
		{
			// NOTA: prima disegno i punti deselezionati
			patch->DrawCtrlPoints2dDeselected();			
		}	  
	}

	// adesso disegno sopra tutti i punti sel
	if (drawCPoints)
	{
		for(i = patches.begin(); i != patches.end(); ++i)
		{
			EditorBezierPatch *patch = (*i).second;
			patch->DrawCtrlPoints2dSelected();
		}	
	}
}

// Disegna la scena per un viewport 3d
void PatchMgr::DrawScene3D(bool drawCPoints, bool drawCPoly, bool drawPatch)
{
	TM->texOff();

	if (!SphereCallList)
	{
		// Devo crearmi una calllist per disegnare una sfera
		GLUquadric *pSphere = gluNewQuadric();
		SphereCallList = glGenLists(1);
		glNewList(SphereCallList, GL_COMPILE);

		gluSphere(pSphere, 0.08, 6, 4);

		glEndList();
		gluDeleteQuadric(pSphere);
	}

	PATCHMAP::iterator i;

	for(i = patches.begin(); i != patches.end(); ++i)
	{
		EditorBezierPatch *patch = (*i).second;
		//glColor3ub(255,255,255);
		if (drawPatch)
			patch->DrawPatch();
	}
	TM->texOff();

	if (drawCPoints)
			DrawCtrlPoints(true);

	if (drawCPoly)
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glColor3ub(0,255,0);	
		glDepthMask(GL_FALSE);
		glEnable(GL_LINE_SMOOTH);
			
		for(i = patches.begin(); i != patches.end(); ++i)
			(*i).second->DrawCtrlPolygon(); 
		
		glDisable(GL_LINE_SMOOTH);
		glDepthMask(GL_TRUE);
		glPopAttrib();
	}

}

// Disegna i punti di controllo. Se 3D vengono disegnate sfere, altrimenti punti
void PatchMgr::DrawCtrlPoints(bool treD)
{
  bool restore = !glIsEnabled(GL_CULL_FACE);
  glEnable(GL_CULL_FACE);

  glInitNames();
  glPushName(0);

  PATCHMAP::iterator i;

  for(i = patches.begin(); i != patches.end(); ++i)
  {
    EditorBezierPatch* patch = (*i).second;
    //Patch name (id)
    glLoadName(patch->getId());
	 if (treD)
		patch->DrawCtrlPoints3d();
	 else
	 {
	   patch->DrawCtrlPoints2dDeselected();
		patch->DrawCtrlPoints2dSelected();
	 }
  }

  if (restore)
	  glDisable(GL_CULL_FACE);
}

// Seleziona un punto
void PatchMgr::SelectPoint(int nPatch, int nPoint)
{
	if (patches[nPatch]->isSelected(nPoint))
		return;	// era gia' selezionato

	patches[nPatch]->Select(nPoint);	// Seleziona

	SelPoint sp;
	sp.nPatch = nPatch;
	sp.nPoint = nPoint;
	SelectedPointList.push_back(sp);	// Inserisce nella lista
}

// Deseleziona un punto
void PatchMgr::DeselectPoint(int nPatch, int nPoint)
{
	SELECTIONLIST::iterator i;
	if (patches[nPatch]->isSelected(nPoint) == false)
		return;	// era gia' deselezionato

	// Deseleziono
	patches[nPatch]->Deselect(nPoint);

	// Devo toglierlo dalla lista dei selezionati... O(n)
	for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
	{
		SelPoint sp = (*i);
		if (sp.nPatch == nPatch &&
			 sp.nPoint == nPoint)
		{
			// trovato! devo eliminarlo
			SelectedPointList.erase(i);				
			return;
		}		
	}
}

// Inverte la selezione di un punto
void PatchMgr::TogglePoint(int nPatch, int nPoint)
{
	bool isSel = patches[nPatch]->isSelected(nPoint);
	// se adesso il punto è selezionato va rimosso dalla lista
	// altrimenti deve essere aggiunto
	if (isSel)
		DeselectPoint(nPatch, nPoint);
	else
		SelectPoint(nPatch, nPoint);
}

// Tutti i punti vengono deselezionati
void PatchMgr::ClearSelection()
{
  SELECTIONLIST::iterator i;
  PATCHMAP::iterator p;

  for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
  {
    p = patches.find((*i).nPatch);
    if (p != patches.end())
      (*p).second->Deselect((*i).nPoint);
  }
  SelectedPointList.clear(); 
}

// Tutti i punti vengono selezionati
void PatchMgr::SelectAll()
{
	// selezionare tutti i punti di tutte le patch e metterli in lista

  SelectedPointList.clear();

  PATCHMAP::iterator p;

  for (p = patches.begin(); p != patches.end(); ++p)
  {
    EditorBezierPatch* patch = (*p).second;
    for (int pointIdx = 0; pointIdx < 16; ++pointIdx)
    {
      patch->Select(pointIdx);
      SelPoint sp;
      sp.nPatch = patch->getId();
      sp.nPoint = pointIdx;

      SelectedPointList.push_back(sp);
    }          
  }
}

// Riempie una lista con le posizioni di tutti i punti selezionati
void PatchMgr::FillWithSelectedPositions(std::list<point> &out)
{
	// inserisco in questa lista tutte le posizioni attuali
	// dei punti selezionati
	SELECTIONLIST::iterator i;
	PATCHMAP::iterator p;

	for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
	{
		p = patches.find((*i).nPatch);
		if (p != patches.end())
		{
			EditorBezierPatch* patch = (*p).second;
			out.push_back(patch->getPoint((*i).nPoint));
		}
	}

}

// Applica uno spostamento ai punti selezionati  (x, y, z)
void PatchMgr::MovePointsWithDelta(float x, float y, float z)
{
  bModified = true;

  SELECTIONLIST::iterator i;
  PATCHMAP::iterator p;

  std::list<point> prevPos;
  std::list<point>::iterator k;
  FillWithSelectedPositions(prevPos);

  for(i = SelectedPointList.begin(), k = prevPos.begin();
      i != SelectedPointList.end(); ++i, ++k)
  {
    p = patches.find((*i).nPatch);
    if (p != patches.end())
    {
      EditorBezierPatch* patch = (*p).second;      
		  patch->setPoint((*i).nPoint, (*k) + point(x,y,z));
      patch->ReCompute();
    }
  }
}

// Applica uno spostamento ai punti selezionati rimanendo su una griglia
void PatchMgr::MovePointsWithDeltaAccum(float x, float y, float z, float gridSize)
{
  bModified = true;

  deltax += x;
  deltay += y;
  deltaz += z;

  //log->addf("Sposta init :: x=%f y=%f, z=%f", x, y, z);
  //log->addf("Sposta init :: dx=%f dy=%f, dz=%f", deltax, deltay, deltaz);

  float fractx = 0.0f;
  float fracty = 0.0f;
  float fractz = 0.0f;

  float intx = 0.0f;
  float inty = 0.0f;
  float intz = 0.0f;

  SELECTIONLIST::iterator i;
  PATCHMAP::iterator p;

  std::list<point> prevPos;
  std::list<point>::iterator k;
  FillWithSelectedPositions(prevPos);


  for(i = SelectedPointList.begin(),
	   k = prevPos.begin();
      i != SelectedPointList.end();
		++i, ++k)
  {
    p = patches.find((*i).nPatch);
    if (p != patches.end())
    {
      EditorBezierPatch* patch = (*p).second;
      
		point p = (*k);

      //log->addf("Sposta :: Points.x=%f ", patch->Points[ii][jj].x);
      
      float coord;		

		// Effettuo gli arrotondamenti alla griglia

		// X
		coord = p.x;
      coord += deltax;
		RoundTo(coord, gridSize, intx, fractx);      
     
		// Y
      coord = p.y;
      coord += deltay;
      RoundTo(coord, gridSize, inty, fracty);
      
		// Z
      coord = p.z;
      coord += deltaz;
		RoundTo(coord, gridSize, intz, fractz);		
     		
		// Muove il punto
		patch->setPoint((*i).nPoint, point(intx, inty, intz));

      patch->ReCompute();
    }
  }

  // Aggiorna gli accumulatori
  deltax = fractx;
  deltay = fracty;
  deltaz = fractz;

  //log->addf("Sposta exit :: dx=%f dy=%f, dz=%f", deltax, deltay, deltaz);

}

// Sposta tutti i punti selezionati al punto di coordinate (x,y,z)
void PatchMgr::MovePoints(float x, float y, float z)
{
  bModified = true;

	// muove tutti i punti selezionati qui
	SELECTIONLIST::iterator i;
  PATCHMAP::iterator p;

  point newPos = point(x,y,z);

  for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
  {
    p = patches.find((*i).nPatch);
    if (p != patches.end())
    {
      EditorBezierPatch* patch = (*p).second;
		patch->setPoint((*i).nPoint, newPos);
      patch->ReCompute();
    }
  }
}

// Salva il progetto su file
bool PatchMgr::Save(const char* filename)
{
  FILE* f;

  // Apro il file
  f = fopen(filename, "w");
  if (!f)
    return false;

  fprintf(f, "[Bezier Patch Editor versione 0.24]\r\n");

  // Salvo tutti i materiali
  MATERIALLIST::iterator i;
  for (i = MaterialList.begin(); i != MaterialList.end(); ++i)
    MM->save(f, *i);

  // Salvo le textures
  TEXTURELIST::iterator t;
  for (t = TextureList.begin(); t != TextureList.end(); ++t)
    TM->save(f, *t);
  

  // Salvo le patch
  PATCHMAP::iterator p;
  for (p = patches.begin(); p != patches.end(); ++p)
  {
    fprintf(f, "\r\n#Patch\r\n");
    EditorBezierPatch* patch = (*p).second;
	 unsigned int texId = patch->getTexture();

    fprintf(f, "PatchId=%i\r\n", patch->getId() );
    fprintf(f, "Materiale=%s\r\n", MM->getName(patch->getMaterial()) );	 
    fprintf(f, "Texture=%s\r\n", (texId ? TM->getUserName(texId) : "#"));
    for (int i = 0; i < 16; ++i)
    {
		 point p = patch->getPoint(i);
		 fprintf(f, "Punto%0.2i=%f, %f, %f\r\n", i, p.x, p.y, p.z);      
    }

    fprintf(f, "FlippedNormals=%i\r\n", patch->FlippedNormals());
    fprintf(f, "Tassellazione=%i\r\n", patch->Depth());

    //Patch collegate
    EditorBezierPatch::BoundPatch bp;
    bp = patch->getConnected(NORTH);        
    fprintf(f, "NORTH=%i\r\n",       (bp.Patch ? bp.Patch->getId() : 0));
    fprintf(f, "NORTHDir=%i\r\n",    patch->getConnected(NORTH).Dir);
    fprintf(f, "NORTHC=%i\r\n",      (bp.c1 ? 1 : 0));
    bp = patch->getConnected(SOUTH);
    fprintf(f, "SOUTH=%i\r\n",       (bp.Patch  ? bp.Patch->getId() : 0));
    fprintf(f, "SOUTHDir=%i\r\n",    patch->getConnected(SOUTH).Dir);
    fprintf(f, "SOUTHC=%i\r\n",      (bp.c1 ? 1 : 0));
    bp = patch->getConnected(WEST);
    fprintf(f, "WEST=%i\r\n",        (bp.Patch  ? bp.Patch->getId() : 0));
    fprintf(f, "WESTDir=%i\r\n",     patch->getConnected(WEST).Dir);
    fprintf(f, "WESTC=%i\r\n",       (bp.c1 ? 1 : 0));
    bp = patch->getConnected(EAST);
    fprintf(f, "EAST=%i\r\n",        (bp.Patch  ? bp.Patch->getId() : 0));
    fprintf(f, "EASTDir=%i\r\n",     patch->getConnected(EAST).Dir);
    fprintf(f, "EASTC=%i\r\n",       (bp.c1 ? 1 : 0));
    
  }

  fclose(f); 

  bModified = false;

  return true;
}

// Carica una patch da file
bool PatchMgr::LoadPatch(FILE* f)
{
  //Carica una patch

  char* retval;
  char buffer[512];
  
  //PatchId
  retval = fgets(buffer, sizeof(buffer), f);
  char* szPatchId;
  int nPatchId;
  
  if ( (szPatchId = FindProperty(retval, "PatchId")) == NULL )
    return false;
  
  nPatchId = atoi(szPatchId);
  //PatchId

  EditorBezierPatch* patch = new EditorBezierPatch(nPatchId, matSel, matDesel, matBinded, SphereCallList);


  //Materiale
  retval = fgets(buffer, sizeof(buffer), f);
  char* szMateriale;
  int nMateriale;
  if ( (szMateriale = FindProperty(retval, "Materiale")) == NULL )
    return false;
	
  nMateriale = MM->findMaterialByName(szMateriale);
  if (nMateriale == 0)
    patch->setMaterial(matDefault);
  else
    patch->setMaterial(nMateriale);

  //Materiale

    //Texture
  retval = fgets(buffer, sizeof(buffer), f);
  char* szTexture;
  if ( (szTexture = FindProperty(retval, "Texture")) == NULL )
    return false;
  
  if (szTexture[0] != '#')
  {
    unsigned int nTexture = TM->findTextureByUserName(szTexture);
    if (nTexture != 0)
    {
      patch->setTexture(nTexture);
    }
  }
  //Texture

   

  //Points
  for (int i = 0; i < 16; ++i)
  {
    // carica il punto
    retval = fgets(buffer, sizeof(buffer), f);
    char* value;
    float PointX, PointY, PointZ;
    if ( (value = FindProperty(retval, "Punto")) == NULL )
      return false;

	 PointX = atof(strtok(value,","));
	 PointY = atof(strtok(NULL,","));
	 PointZ = atof(strtok(NULL,"\r\n"));
    
    point p(PointX, PointY, PointZ); 

    patch->setPoint(i, p);
  }
  //Points

  //FlippedNormals
  retval = fgets(buffer, sizeof(buffer), f);
  char* szFlippedNormals;
  int nFlippedNormals;
  if ( (szFlippedNormals = FindProperty(retval, "FlippedNormals")) == NULL )
    return false;
  nFlippedNormals = atoi(szFlippedNormals);

  if (nFlippedNormals)
    patch->FlipNormals();
  //FlippedNormals

  //Tassellazione
  retval = fgets(buffer, sizeof(buffer), f);
  char* szTassellazione;
  int nTassellazione;
  if ( (szTassellazione = FindProperty(retval, "Tassellazione")) == NULL )
    return false;
  
  nTassellazione = atoi(szTassellazione);

  patch->SetDepth(nTassellazione);
  //Tassellazione

  TempBind binding;

  //NORTHId
  retval = fgets(buffer, sizeof(buffer), f);
  char* szNORTH;
  int nNORTHId;
  if ( (szNORTH = FindProperty(retval, "NORTH")) == NULL )
    return false;
  
  nNORTHId = atoi(szNORTH);
  //NORTHId

  //NORTHDir
  retval = fgets(buffer, sizeof(buffer), f);
  char* szNORTHDir;
  Direction nNORTHDir;
  if ( (szNORTHDir = FindProperty(retval, "NORTHDir")) == NULL )
    return false;
  
  nNORTHDir = (Direction)atoi(szNORTHDir);
  //NORTHDir

  //NORTHC
  retval = fgets(buffer, sizeof(buffer), f);
  char* szNORTHC;
  int nNORTHC;
  if ( (szNORTHC = FindProperty(retval, "NORTHC")) == NULL )
    return false;
  
  nNORTHC = atoi(szNORTHC);
  //NORTHC

  if(nNORTHId != 0)
  {
    binding.Patch1Dir = NORTH;
    binding.Patch1Id  = nPatchId;
    binding.Patch2Dir = nNORTHDir;
    binding.Patch2Id  = nNORTHId;
    binding.c1        = (nNORTHC ? true : false);
    BindList.push_back(binding);
  }

  //SOUTHId
  retval = fgets(buffer, sizeof(buffer), f);
  char* szSOUTH;
  int nSOUTHId;
  if ( (szSOUTH = FindProperty(retval, "SOUTH")) == NULL )
    return false;
  
  nSOUTHId = atoi(szSOUTH);
  //SOUTHId

  //SOUTHDir
  retval = fgets(buffer, sizeof(buffer), f);
  char* szSOUTHDir;
  Direction nSOUTHDir;
  if ( (szSOUTHDir = FindProperty(retval, "SOUTHDir")) == NULL )
    return false;
  
  nSOUTHDir = (Direction)atoi(szSOUTHDir);
  //SOUTHDir

  //SOUTHC
  retval = fgets(buffer, sizeof(buffer), f);
  char* szSOUTHC;
  int nSOUTHC;
  if ( (szSOUTHC = FindProperty(retval, "SOUTHC")) == NULL )
    return false;
  
  nSOUTHC = atoi(szSOUTHC);
  //SOUTHC

  if(nSOUTHId != 0)
  {
    binding.Patch1Dir = SOUTH;
    binding.Patch1Id  = nPatchId;
    binding.Patch2Dir = nSOUTHDir;
    binding.Patch2Id  = nSOUTHId;
    binding.c1        = (nSOUTHC ? true : false);
    BindList.push_back(binding);
  }

  //WESTId
  retval = fgets(buffer, sizeof(buffer), f);
  char* szWEST;
  int nWESTId;
  if ( (szWEST = FindProperty(retval, "WEST")) == NULL )
    return false;
  
  nWESTId = atoi(szWEST);
  //WESTId

  //WESTDir
  retval = fgets(buffer, sizeof(buffer), f);
  char* szWESTDir;
  Direction nWESTDir;
  if ( (szWESTDir = FindProperty(retval, "WESTDir")) == NULL )
    return false;
  
  nWESTDir = (Direction)atoi(szWESTDir);
  //WESTDir

  //WESTC
  retval = fgets(buffer, sizeof(buffer), f);
  char* szWESTC;
  int nWESTC;
  if ( (szWESTC = FindProperty(retval, "WESTC")) == NULL )
    return false;
  
  nWESTC = atoi(szWESTC);
  //WESTC

  if(nWESTId != 0)
  {
    binding.Patch1Dir = WEST;
    binding.Patch1Id  = nPatchId;
    binding.Patch2Dir = nWESTDir;
    binding.Patch2Id  = nWESTId;
    binding.c1        = (nWESTC ? true : false);
    BindList.push_back(binding);
  }

  //EASTId
  retval = fgets(buffer, sizeof(buffer), f);
  char* szEAST;
  int nEASTId;
  if ( (szEAST = FindProperty(retval, "EAST")) == NULL )
    return false;
  
  nEASTId = atoi(szEAST);
  //EASTId

  //EASTDir
  retval = fgets(buffer, sizeof(buffer), f);
  char* szEASTDir;
  Direction nEASTDir;
  if ( (szEASTDir = FindProperty(retval, "EASTDir")) == NULL )
    return false;
  
  nEASTDir = (Direction)atoi(szEASTDir);
  //EASTDir

  //EASTC
  retval = fgets(buffer, sizeof(buffer), f);
  char* szEASTC;
  int nEASTC;
  if ( (szEASTC = FindProperty(retval, "EASTC")) == NULL )
    return false;
  
  nEASTC = atoi(szEASTC);
  //EASTC

  if (nEASTId != 0)
  {
    binding.Patch1Dir = EAST;
    binding.Patch1Id  = nPatchId;
    binding.Patch2Dir = nEASTDir;
    binding.Patch2Id  = nEASTId;
    binding.c1        = (nEASTC ? true : false);
    BindList.push_back(binding);  
  }

  patches[nPatchId] = patch;

  if (nPatchId > nNextPatch)
    nNextPatch = nPatchId;

  log->addf("PatchMgr :: Caricata pezza nr. %i", nPatchId);

  return true;
}

// Carica il progetto da file
bool PatchMgr::Load(const char* filename)
{
	// Prima svuoto il progetto corrente
  ResetWorkspace();

  FILE* f;
  char buffer[512];
  char* retval;

  f = fopen(filename, "r");
  if (!f)
    return false;

  log->addf("PatchMgr :: Sto processsando il file %s...", filename);
  retval = fgets(buffer, sizeof(buffer), f);
  
  if ( (retval == NULL) || (strncmp(buffer, "[Bezier", 7) != 0) )
  {
    
    fclose(f);
    return false;
  }
  
  log->addf("PatchMgr :: File Header: -> %s", buffer);

  unsigned int matId;
  unsigned int texId;
 
  bool go = true;
  while (go)
  {
    retval = fgets(buffer, sizeof(buffer), f);
    if (retval != NULL)
    {
      if (strncmp("#P", buffer, 2) == 0)
      {
        LoadPatch(f);
      }

      if (strncmp("#M", buffer, 2) == 0)
      {
        if ( (matId = MM->load(f)) != INVALID_MATERIAL )
        {
          MaterialList.push_back(matId);
          log->addf("PatchMgr :: Caricato materiale nr. %i", matId);
        }
      }

		if (strncmp("#T", buffer, 2) == 0)
      {
        if ( (texId = TM->load(f)) != INVALID_TEXTURE )
        {
          TextureList.push_back(texId);
          log->addf("PatchMgr :: Caricata texture nr. %i", texId);
        }
      }
    }
    else
    {
      go = false;
    }
  }

  // nel bene o nel male, abbiamo finito col file
  fclose(f);

  // Applichiamo i binding....
  log->addf("PatchMgr :: Applying bindings...");
  BINDLIST::iterator i;
  PATCHMAP::iterator p1, p2;
  for(i = BindList.begin(); i != BindList.end(); ++i)
  {
    TempBind bind = *i;
    if ( (p1 = patches.find(bind.Patch1Id)) != patches.end() &&
         (p2 = patches.find(bind.Patch2Id)) != patches.end() )
    {
      EditorBezierPatch* patch1 = (*p1).second;
      EditorBezierPatch* patch2 = (*p2).second;

      patch1->Attach(patch2, bind.Patch1Dir, bind.Patch2Dir, bind.c1);
      //e' necessario anche il secondo?
      //patch2->Attach(patch1, bind.Patch2Dir, bind.Patch1Dir, true);      
    }
  }
  log->addf("PatchMgr :: ... Done!");


  BindList.clear();

  log->addf("PatchMgr :: ... File '%s' caricato!", filename);

  return true;
}

// Sposta tutti i punti selezionati nel loro baricentro geometrico
void PatchMgr::CollapsePoints()
{
  bModified = true;
	// spostare tutti i punti selezionati nel baricentro

	vector3 baricentro = vector3(0.0f, 0.0f, 0.0f);
	SELECTIONLIST::iterator i;
	PATCHMAP::iterator p;

	for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
	{
		p = patches.find((*i).nPatch);
		if (p != patches.end())
		{
			EditorBezierPatch* patch = (*p).second;			
			point ctrlPoint = patch->getPoint((*i).nPoint);
			baricentro.x += ctrlPoint.x;
			baricentro.y += ctrlPoint.y;
			baricentro.z += ctrlPoint.z;
		}    
	}
	baricentro /= SelectedPointList.size();

	MovePoints(baricentro.x, baricentro.y, baricentro.z);
}

// Elimina tutte le patch (chiamato sul New Project)
void PatchMgr::ResetWorkspace()
{
  bModified = false;

	// elimina tutte le patch (new)
  PATCHMAP::iterator p;

  for (p = patches.begin(); p != patches.end(); ++p)
  {
    EditorBezierPatch* patch = (*p).second;
    //I'm paranoic
    if (patch) delete patch;
  }

  nNextPatch = 0;

  patches.clear();
  SelectedPointList.clear();

  MATERIALLIST::iterator i;

  i = MaterialList.begin();
  while (i != MaterialList.end())
  {
    if ( (MM->getName(*i))[0] != '#')
	 {
       MM->remMaterial(*i);
       i = MaterialList.erase(i);
	 }	
    else
      ++i;
  }

  TEXTURELIST::iterator t;

  t = TextureList.begin();
  while (t != TextureList.end())
  {
    TM->remResource(*t);
    t = TextureList.erase(t);
	 
  }

  // pulisco la clipboard dai materiali/texture
  PATCHLIST::iterator l;
  for(l = Clipboard.begin(); l != Clipboard.end(); ++l)
  {
    EditorBezierPatch* patch = *l;
	 patch->setMaterial(MM->findMaterialByName("#Default"));
	 patch->setTexture(0);
  }

}

// Seleziona completamente le patch che hanno gia' almeno un punto selezionato
void PatchMgr::SelectPatch()
{
	// esamina la lista dei punti selezionati...
	// per ogni punto sel, seleziona TUTTA la sua patch
  SELECTIONLIST::iterator i;
  PATCHMAP::iterator p;

  for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
  {
    int patchIdx = (*i).nPatch;

    p = patches.find(patchIdx);
    if (p != patches.end())
    {
      EditorBezierPatch* patch = (*p).second;
      for (int pointIdx = 0; pointIdx < 16; ++pointIdx)
      {
        if (!(patch->isSelected(pointIdx) ))
        {
			 // ok lo seleziono
          patch->Select(pointIdx);
          SelPoint sp;
          sp.nPatch = patchIdx;
          sp.nPoint = pointIdx;

          SelectedPointList.push_back(sp);
        }
      }      
    }
  }
}

// Elimina tutte le patch che hanno punti selezionati
void PatchMgr::DeletePatch()
{
  bModified = true;

  SELECTIONLIST::iterator i;
  PATCHMAP::iterator p;

  for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
  {
    p = patches.find((*i).nPatch);
    if (p != patches.end())
    {
      EditorBezierPatch* patch = (*p).second;
      //I'm paranoic
      if (patch) delete patch;
      patches.erase((*i).nPatch);
    }
  }
  
  SelectedPointList.clear();
}

// Nasconde i punti di controllo selezionati (non potranno piu' essere pickati)
void PatchMgr::Hide()
{
	PATCHMAP::iterator p;  

  for (p = patches.begin(); p != patches.end(); ++p)
  {   
    EditorBezierPatch* patch = (*p).second;
	 if (patch->isPatchSelected())
    {
      patch->HideSelected();
      patch->ReCompute();
    }
  }
}

// Mostra tutti i punti di controllo
void PatchMgr::UnhideAll()
{
	PATCHMAP::iterator p;  

	for (p = patches.begin(); p != patches.end(); ++p)
	{   
		EditorBezierPatch* patch = (*p).second;
	   patch->UnhideAll();
      patch->ReCompute();
	}
}

// Inverte le normali di tutte le patch selezionate
void PatchMgr::FlipNormals()
{
	//@@@@@@@@@@@@@@@@ FIXED @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// utilizzo la nuova isPatchSelected() delle EditorBezierPatch
  PATCHMAP::iterator p;  

  for (p = patches.begin(); p != patches.end(); ++p)
  {
    //bool bSelected = false;
    EditorBezierPatch* patch = (*p).second;
	 if (patch->isPatchSelected())
    {
      patch->FlipNormals();
      patch->ReCompute();
    }
  }
}

// Rimove il materiale dalle patch nel progetto e dalle patch in clipboard
void PatchMgr::RemoveMaterialFromProject(unsigned int mat)
{
	bModified = true;

	// se qualche patch utilizza questo materiale 
	// setto il materiale di defalult
	PATCHMAP::iterator p;
	for (p = patches.begin(); p != patches.end(); ++p)
	{
		EditorBezierPatch* patch = (*p).second;
		if (patch->getMaterial() == mat)
			patch->setMaterial(matDefault);
	}

	// controllo anche eventuali patch nella clipboard
	PATCHLIST::iterator i;
	for(i = Clipboard.begin(); i != Clipboard.end(); ++i)
	{
		EditorBezierPatch* patch = *i;
		if (patch->getMaterial() == mat)
			patch->setMaterial(matDefault);
	}

	// ora rimuovo il materiale dal Material Manager
	MM->remMaterial(mat);
	MaterialList.remove(mat);
}

// Rimove la texture dalle patch nel progetto e dalle patch in clipboard
void PatchMgr::RemoveTextureFromProject(unsigned int tex)
{
	bModified = true;

	// se qualche patch utilizza questa texture
	// setto a texture vuota
	PATCHMAP::iterator p;
	for (p = patches.begin(); p != patches.end(); ++p)
	{
		EditorBezierPatch* patch = (*p).second;
		if (patch->getTexture() == tex)
			patch->setTexture(0);
	}

	// controllo anche eventuali patch nella clipboard
	PATCHLIST::iterator i;
	for(i = Clipboard.begin(); i != Clipboard.end(); ++i)
	{
		EditorBezierPatch* patch = *i;
		if (patch->getTexture() == tex)
			patch->setTexture(0);
	}

	// ora rimuovo il materiale dal Material Manager
	TM->remResource(tex);
	TextureList.remove(tex);
}

// Restta gli accumulatori usati per lo spostamento su griglia (Snap to grid)
void PatchMgr::ClearAccum()
{
  deltax = 0.0f;
  deltay = 0.0f;
  deltaz = 0.0f;
}

// Aumenta la tassellazione delle patch selezionate
void PatchMgr::IncreaseTassellation()
{
	bModified = true;

	PATCHMAP::iterator p;  

	for (p = patches.begin(); p != patches.end(); ++p)
	{		
		EditorBezierPatch* patch = (*p).second;
		if (patch->isPatchSelected())
			patch->SetDepth(patch->Depth()+1);
	}

}

// Diminuisce la tassellazione delle patch selezionate
void PatchMgr::DecreaseTassellation()
{
	bModified = true;

	PATCHMAP::iterator p;  

	for (p = patches.begin(); p != patches.end(); ++p)
	{		
		EditorBezierPatch* patch = (*p).second;
		if (patch->isPatchSelected())
			patch->SetDepth(patch->Depth()-1);
	}
}

// Analizza i punti selezionati e verifica se e' una selezione corretta per un unione
bool PatchMgr::CheckJoinPatchSelection(int &Patch1, Direction &Dir1, int &Patch2, Direction &Dir2)
{
	/*
	+------NORTH------+
	|  0,  1,  2,  3, |
	W  4,  5,  6,  7, E
	|  8,  9, 10, 11, |
	| 12, 13, 14, 15  |
	+------SOUTH------+

   NOTA: tutti i parametri sono di uscita

   Ritorno Vero se la selezione e' valida per un unione e ho riempito i parametri
	con i valori corretti.
	*/
	Patch1 = -1;
	Patch2 = -1;
	Dir1 = UNKNOWN;
	Dir2 = UNKNOWN;

	SELECTIONLIST::iterator i;
	
	int row, col;
	Direction *tempDir;

	// scandisco tutti i punti selezionati
	for(i = SelectedPointList.begin(); i != SelectedPointList.end(); ++i)
	{
		SelPoint p = *i;

		if (Patch1 == -1)
			Patch1 = p.nPatch;	// trovato la prima patch
		else if (Patch2 == -1 && p.nPatch != Patch1)
			Patch2 = p.nPatch;	// trovato la seconda patch

		row = p.nPoint >> 2;	// diviso 4
		col = p.nPoint & 3;  // modulo 4

		if (p.nPoint == 5 ||
			 p.nPoint == 6 ||
			 p.nPoint == 9 ||
			 p.nPoint == 10)
		{
			// sono selezionati punti interni ad una patch
			return false;
		}

		if (p.nPatch == Patch1)
			tempDir = &Dir1;
		else if (p.nPatch == Patch2)
			tempDir = &Dir2;
		else
		{
			// errore! sono selezionati punti di 3 patch o piu!
			return false;
		}

		switch(*tempDir)
		{
		case NORTH:	// accettiamo solo punti del confine nord
			if (row != 0)
				return false;	
			break;

		case EAST:	// accettiamo solo punti del confine est
			if (col != 3)
				return false;	
			break;

		case SOUTH:	// accettiamo solo punti del confine sud
			if (row != 3)
				return false;	
			break;

		case WEST:	// accettiamo solo punti del confine ovest
			if (col != 0)
				return false;	
			break;

		default:		// abbiamo ancora da decidere una direzione
			if (p.nPoint == 1 || p.nPoint == 2)
				*tempDir = NORTH;
			else if (p.nPoint == 7 || p.nPoint == 11)
				*tempDir = EAST;
			else if (p.nPoint == 13 || p.nPoint == 14)
				*tempDir = SOUTH;
			else if (p.nPoint == 4 || p.nPoint == 8)
				*tempDir = WEST;
		}

	}

	// verifica dei risultati ...
	if (Dir1 == UNKNOWN || Dir2 == UNKNOWN)
		return false;

	// @@@@@@@@@@@@@@@@@@@@@ TODO @@@@@@@@@@@@@@@@@@@@
	// volendo si possono fare controlli aggiuntivi
	// es: tutto il confine deve essere selezionato (4 punti di controllo)
	//
	// @@@@@@@@ E' piu' funzionale cosi' (Ale)

	log->addf("PatchMgr :: Join Patch %d dir %d <==> %d dir %d", Patch1, Dir1, Patch2, Dir2);
	return true;
}

// Copia le patch selezionate nella clipboard e le rimuove dalla scena (taglia)
void PatchMgr::Cut()
{
	bModified = true;
	Copy();
	DeletePatch();
}

// Copia le patch selezionate nella clipboard (copia)
void PatchMgr::Copy()
{
	// Prima svuoto la clipboard
	ClearClipboard();

	PATCHMAP::iterator p;  
	for (p = patches.begin(); p != patches.end(); ++p)
	{
		EditorBezierPatch* patch = (*p).second;

		// se la patch e' selezionata, la copio nella clipboard
		if (patch->isPatchSelected())
			Clipboard.push_back(patch->Clone());
	}
}

// Inserisce nel progetto le patch della clipboard (incolla)
void PatchMgr::Paste()
{
	if (Clipboard.empty())
		return;

	ClearSelection();

	bModified = true;
	PATCHLIST::iterator i;
	point one = point(1.0f, 1.0f, 1.0f);

	for(i = Clipboard.begin(); i != Clipboard.end(); ++i)
	{
		EditorBezierPatch* newPatch = (*i)->Clone();
		++nNextPatch;
		newPatch->setId(nNextPatch);
		patches[nNextPatch] = newPatch;

		// per spostarlo un po' da dove era stato copiato
		for (int j = 0;j<16;j++)
			newPatch->TranslatePoint(j, one);

		// seleziono almeno un punto per patch
		SelectPoint(nNextPatch,0);
	}

	// seleziono tutte le patch appena incollate
	SelectPatch();
}

// Verifica se la clipboard e' vuota  
bool PatchMgr::isClipboardEmpty()
{
	return Clipboard.empty();
}

// Verifica se esistono punti selezionati
bool PatchMgr::isSelectionEmpty()
{
	return SelectedPointList.empty();
}

// Svuota la clipboard x il copia-incolla
void PatchMgr::ClearClipboard()
{
	PATCHLIST::iterator i;

	for(i = Clipboard.begin(); i != Clipboard.end(); ++i)	
		if (*i) delete *i;

	Clipboard.clear();
}

// Effettua una riflessione sul viewport Top
void PatchMgr::MirrorTopView(point Start, point End)
{
//	Start ed End sono due punti che identificano il piano di riflessione

	float dx,dy;
	float sin_a, cos_a;

	point Diff = End - Start;
	if (!mirror(Start.x, Start.z, Diff.x, Diff.z, dx, dy, sin_a, cos_a))
		return;

	bModified = true;

	std::list<point> prevPos;
	std::list<point>::iterator k;
	FillWithSelectedPositions(prevPos);

	SELECTIONLIST::iterator i;
	PATCHMAP::iterator p;
	for(i = SelectedPointList.begin(),
		 k = prevPos.begin(); i != SelectedPointList.end(); ++i, ++k)
	{
		p = patches.find((*i).nPatch);
		if (p != patches.end())
		{
			EditorBezierPatch* patch = (*p).second;
			point pos = (*k);
			
			// Sposto il punto attorno all'origine
			pos.x -= dx; pos.z -= dy;

			// Applico la matrice di riflessione
			pos = point(pos.x * cos_a + pos.z * sin_a,
			            pos.y,
							pos.x * sin_a - pos.z * cos_a);

			// Rimetto i punti nella loro posizione originaria
			pos.x += dx; pos.z += dy;

			patch->setPoint((*i).nPoint, pos);			
			patch->ReCompute();
		}
	}

}

// Effettua una riflessione sul viewport Front
void PatchMgr::MirrorFrontView(point Start, point End)
{
	//	Start ed End sono due punti che identificano il piano di riflessione
	float dx,dy;
	float sin_a, cos_a;

	point Diff = End - Start;
	if(!mirror(Start.x, Start.y, Diff.x, Diff.y, dx, dy, sin_a, cos_a))
		return;

	bModified = true;

	std::list<point> prevPos;
	std::list<point>::iterator k;
	FillWithSelectedPositions(prevPos);

	SELECTIONLIST::iterator i;
	PATCHMAP::iterator p;
	for(i = SelectedPointList.begin(),
		 k = prevPos.begin(); i != SelectedPointList.end(); ++i, ++k)
	{
		p = patches.find((*i).nPatch);
		if (p != patches.end())
		{
			EditorBezierPatch* patch = (*p).second;
			point pos = (*k);
			
			// Sposto il punto attorno all'origine
			pos.x -= dx; pos.y -= dy;

			// Applico la riflessione
			pos = point(pos.x * cos_a + pos.y * sin_a,
			            pos.x * sin_a - pos.y * cos_a,
							pos.z);

			// Ripristino la posizione del punto
			pos.x += dx; pos.y += dy;
			
			patch->_setPoint((*i).nPoint, pos);			
			patch->ReCompute();
		}
	}
}

// Effettua una riflessione sul viewport Side
void PatchMgr::MirrorSideView(point Start, point End)
{
	//	Start ed End sono due punti che identificano il piano di riflessione
	float dx,dy;
	float sin_a, cos_a;

	point Diff = End - Start;
	if (!mirror(Start.z, Start.y, Diff.z, Diff.y, dx, dy, sin_a, cos_a))
		return;

	bModified = true;

	std::list<point> prevPos;
	std::list<point>::iterator k;
	FillWithSelectedPositions(prevPos);

	SELECTIONLIST::iterator i;
	PATCHMAP::iterator p;
	for(i = SelectedPointList.begin(),
		 k = prevPos.begin(); i != SelectedPointList.end(); ++i, ++k)
	{
		p = patches.find((*i).nPatch);
		if (p != patches.end())
		{
			EditorBezierPatch* patch = (*p).second;
			point pos = (*k);
			
			// Sposto il punto attorno all'origine
			pos.z -= dx; pos.y -= dy;

			// Applico la riflessione
			pos = point(pos.x,							
			            pos.z * sin_a - pos.y * cos_a,
							pos.z * cos_a + pos.y * sin_a);
							
			// Ripristino la posizione del punto
			pos.z += dx; pos.y += dy;

			patch->_setPoint((*i).nPoint, pos);			
			patch->ReCompute();
		}
	}
}

// Effettua una rotazione sul viewport Top
void PatchMgr::RotateTopView(point Start, point End)
{
	// Start definisce l'asse di rotazione, con End ci calcoliamo l'angolo di rotazione
	float sin_a, cos_a;

	point Diff = End - Start;
	if (!rotate(Start.x, Start.z, Diff.x, Diff.z, sin_a, cos_a))
		return;

	bModified = true;

	std::list<point> prevPos;
	std::list<point>::iterator k;
	FillWithSelectedPositions(prevPos);

	SELECTIONLIST::iterator i;
	PATCHMAP::iterator p;
	for(i = SelectedPointList.begin(),
		 k = prevPos.begin(); i != SelectedPointList.end(); ++i, ++k)
	{
		p = patches.find((*i).nPatch);
		if (p != patches.end())
		{
			EditorBezierPatch* patch = (*p).second;
			point pos = (*k);
			pos.x -= Start.x; pos.z -= Start.z;

			pos = point(pos.x * cos_a - pos.z * sin_a,
			            pos.y,
							pos.x * sin_a + pos.z * cos_a);
			pos.x += Start.x; pos.z += Start.z;

			patch->setPoint((*i).nPoint, pos);			
			patch->ReCompute();
		}
	}

}

// Effettua una rotazione sul viewport Front
void PatchMgr::RotateFrontView(point Start, point End)
{
	// Start definisce l'asse di rotazione, con End ci calcoliamo l'angolo di rotazione
	float sin_a, cos_a;

	point Diff = End - Start;
	if (!rotate(Start.x, Start.y, Diff.x, Diff.y, sin_a, cos_a))
		return;

	bModified = true;

	std::list<point> prevPos;
	std::list<point>::iterator k;
	FillWithSelectedPositions(prevPos);

	SELECTIONLIST::iterator i;
	PATCHMAP::iterator p;
	for(i = SelectedPointList.begin(),
		 k = prevPos.begin(); i != SelectedPointList.end(); ++i, ++k)
	{
		p = patches.find((*i).nPatch);
		if (p != patches.end())
		{
			EditorBezierPatch* patch = (*p).second;
			point pos = (*k);
			pos.x -= Start.x; pos.y -= Start.y;

			pos = point(pos.x * cos_a - pos.y * sin_a,
			            pos.x * sin_a + pos.y * cos_a,
							pos.z);
			pos.x += Start.x; pos.y += Start.y;

			patch->setPoint((*i).nPoint, pos);			
			patch->ReCompute();
		}
	}
}

// Effettua una rotazione sul viewport Side
void PatchMgr::RotateSideView(point Start, point End)
{
	// Start definisce l'asse di rotazione, con End ci calcoliamo l'angolo di rotazione
	float sin_a, cos_a;

	point Diff = End - Start;
	if (!rotate(Start.z, Start.y, Diff.z, -Diff.y, sin_a, cos_a))
		return;

	bModified = true;

	std::list<point> prevPos;
	std::list<point>::iterator k;
	FillWithSelectedPositions(prevPos);

	SELECTIONLIST::iterator i;
	PATCHMAP::iterator p;
	for(i = SelectedPointList.begin(),
		 k = prevPos.begin(); i != SelectedPointList.end(); ++i, ++k)
	{
		p = patches.find((*i).nPatch);
		if (p != patches.end())
		{
			EditorBezierPatch* patch = (*p).second;
			point pos = (*k);
			pos.z -= Start.z; pos.y -= Start.y;

			pos = point(pos.x,							
			            pos.z * sin_a + pos.y * cos_a,
							pos.z * cos_a - pos.y * sin_a);
							
			pos.z += Start.z; pos.y += Start.y;

			patch->setPoint((*i).nPoint, pos);			
			patch->ReCompute();
		}
	}
}


bool PatchMgr::mirror(float start_x, float start_y, float diff_x, float diff_y, float &dx, float &dy, float &sin_a, float &cos_a)
{
/*
	(startx, starty)
	(startx + diff_x, starty + diff_y) 
	sono due punti sulla retta di riflessione

	Uscita:
	
	dx, dy --> lo spostamento da applicare al punto prima di applicare la riflessione
*/
	if (ALMOST_ZERO(diff_x) && 
	    ALMOST_ZERO(diff_y))		 
		 return false;						// i due punti devono essere distinti

	if (ALMOST_ZERO(diff_x))
	{
		dx = start_x;
		dy = 0.0f;
	}
	else
	{
		float m = diff_y / diff_x;
		dx = 0.0f;

		if (ALMOST_ZERO(m))
			dy = start_y;	
		else
			dy = start_y -m*start_x;
	}

	float angle;
	
	// atan2f(y,x)
	angle = atan2f(diff_y, diff_x);
		
	// cos(a)	sen(a)		esegue una riflessione di a/2
	// sen(a)  -cos(a)

	angle *= 2;
	sin_a = sin(angle);
	cos_a = cos(angle);

	return true;
}

bool PatchMgr::rotate(float start_x, float start_y, float diff_x, float diff_y, float &sin_a, float &cos_a)
{
	// (startx, starty)  e' il centro di rotazione

	// (startx, starty)
	// (startx + diff_x, starty + diff_y) 
	// sono due punti che identificano una retta, l'angolo di rotazione viene calcolato
	// rispetto alla retta y = 0

	if (ALMOST_ZERO(diff_x) && 
	    ALMOST_ZERO(diff_y))		 
		 return false;						// i due punti devono essere distinti
	
	float angle;
	
	// atan2f(y,x)
	angle = atan2f(diff_y, diff_x);
		
	// cos(a)   -sen(a)		esegue una rotazione di a
	// sen(a)	 cos(a)

	sin_a = sin(angle);
	cos_a = cos(angle);

	return true;
}

// Unisce due patch (continuita' c0 o c1)
bool PatchMgr::JoinPatches(int Patch1, Direction Dir1, int Patch2, Direction Dir2, bool c1)
{
/*
     Patch 1       Patch 2
	+--+--+--+    +--+--+--+
	|       D|    |D       |
	+  +  + I+    +I +  +  +
	|       R|    |R       |
	+  +  +  +    +  +  +  +
	|       1|    |2       |
	+--+--+--+    +--+--+--+

   In questo esempio la patch 1 si lega a est con la patch 2 a ovest
*/
	ClearSelection();

	// Prendo i puntatori alle patch
	// @@@@@@@@@@@@@@@@@@ FIX @@@@@@@@@@@@@@@@@@@@
	// Usare la funzione find invece che l'operatore []
	
	EditorBezierPatch *patch1 = patches[Patch1];
	EditorBezierPatch *patch2 = patches[Patch2];

	if (!patch1 || !patch2)
		return false;

	// se erano gia' collegate.... le scollego
	SplitPatches(Patch1, Dir1, Patch2, Dir2);

	// adesso proviamo ad attaccarle... =)
	return patch1->Attach(patch2, Dir1, Dir2, c1);
}

// Separa due patch che erano unite
bool PatchMgr::SplitPatches(int Patch1, Direction Dir1, int Patch2, Direction Dir2)
{
	// Prendo i puntatori alle patch
	// @@@@@@@@@@@@@@@@@@ FIX @@@@@@@@@@@@@@@@@@@@
	// Usare la funzione find invece che l'operatore []

	EditorBezierPatch *patch1 = patches[Patch1];
	EditorBezierPatch *patch2 = patches[Patch2];

	if (!patch1 || !patch2)
		return false;

	bModified = true;

	ClearSelection();

	// se erano collegate.... le scollego
	if (patch1->getConnectedPatch(Dir1))
		patch1->Detach(Dir1);

	if (patch2->getConnectedPatch(Dir2))
		patch2->Detach(Dir2);

	return true;
}