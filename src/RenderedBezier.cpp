/*******************************************************************\
*  Bezier Patch Editor  ---  RenderedBezier                         *
*                                                                   *
*  Questa classe estende le bezier "matematiche" per aggiungere     *
*  nuove funzionalita'. Queste patch possono essere collegate tra   *
*  loro con continuita' c0 o c1. Le chiamate alle funzioni          *
*  setPoint() e TranslatePoint() muovono un punto di controllo      *
*  mantenendo i vincoli di continuita'.                             *
*  Inoltre questa patch puo' avere un materiale ed una texture.     *
\*******************************************************************/

#include <deque>
#include "RenderedBezier.h"

const RenderedBezier::PointsInfo RenderedBezier::PointsInDir[4][4] =
{	
	// vedi struct PointsInfo
	//  unsigned char Inside;
	//  unsigned char Border;
  
	{ {4, 0}, {5, 1}, {6, 2 }, {7, 3,} },  // NORTH   = 0
	{ {2, 3}, {6, 7}, {10,11}, {14,15} },  // EAST    = 1
	{ {8,12}, {9,13}, {10,14}, {11,15} },  // SOUTH   = 2
	{ {1, 0}, {5, 4}, {9, 8 }, {13,12} }   // WEST    = 3
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RenderedBezier::RenderedBezier(unsigned int id) : CubicBezierPatch()
{
	myId = id;
	Init();
}

RenderedBezier::RenderedBezier(unsigned int id, point cpoints[][4]) : CubicBezierPatch(cpoints)
{
	myId = id;
	Init();
}

RenderedBezier::~RenderedBezier()
{
	// Voglio essere sicuro di non lasciare orfani
	if (getConnectedPatch(NORTH))	Detach(NORTH);
	if (getConnectedPatch(SOUTH))	Detach(SOUTH);
	if (getConnectedPatch(EAST))	Detach(EAST);
	if (getConnectedPatch(WEST))	Detach(WEST);
}

void RenderedBezier::Init()
{
	int i,j,k;

	material = 0;
	texture = 0;
	
	MM = MaterialMgr::getInstance();
	TM = TextureMgr::getInstance();

	for (i=0; i<16; ++i)
	{
		BindingsApplied[i] = false;
		Binded[i] = false;
	}
	
	for (i=0; i<4; i++)
	{
		Connected[i].Patch = NULL;	// non sono collegato
		Connected[i].Dir = UNKNOWN;

		BorderNormals[i] = NULL;

		for (j=0; j<4; ++j)
			for (k=0; k<7; ++k)
			{
				Binding[i][j][k].Patch = NULL;
				Binding[i][j][k].nPoint = 0;
			}
	}	
}

unsigned int RenderedBezier::getId()
{
	return myId;
}

void RenderedBezier::setId(unsigned int newId)
{
	myId = newId;
}

unsigned int RenderedBezier::getMaterial()
{
	return material;
}

void RenderedBezier::setMaterial(unsigned int mat)
{
	material = mat;	
}

unsigned int RenderedBezier::getTexture()
{
	return texture;
}

void RenderedBezier::setTexture(unsigned int tex)
{
	texture = tex;
}

void RenderedBezier::DrawPatch()
{	
	bool blend = false;

	// Se il mio materiale e' trasparente ho bisogno del blending...
	if (MM->getTrasparency(material)>0)
		blend = true;

	MM->useMaterial(material);
	if (blend)
	{
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
	}

	if (texture && glIsEnabled(GL_TEXTURE_2D))
		TM->texUse(texture);
	else
		TM->texOff();

	// Disegno la patch!
	CubicBezierPatch::DrawPatch();

	if (blend)
	{
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
}

void RenderedBezier::setPoint(unsigned char pos, point p)
{
	setPoint(pos >> 2, pos & 3, p);
}

void RenderedBezier::setPoint(unsigned char i, unsigned char j, point p)
{
	point Diff = p - Points[i][j];	// lo spostamento

	// Applico i vincoli
	ApplyBindings(i, j, Diff);

	Points[i][j] = p;

	// Resetto le marcature della BFS
	ClearBindingsApplied(i,j);
}
  
// Resetto le marcature BFS a partire dal punto (i,j) di questa patch
void RenderedBezier::ClearBindingsApplied(unsigned char i, unsigned char j)
{
	BindingsApplied[(i<<2)+j] = false;
	
	int k = 0;
	while(Binding[i][j][k].Patch)
	{
		BoundPoint bp = Binding[i][j][k];		

		if (bp.Patch->BindingsApplied[bp.nPoint])
			bp.Patch->ClearBindingsApplied(bp.nPoint >> 2, bp.nPoint & 3);

		++k;
	}

}

// Applico i bindings considerando che il punto (i,j) di questa patch
// si muove di 'diff'
void RenderedBezier::ApplyBindings(unsigned char i, unsigned char j, point diff)
{	
	// Utilizzo una visita di grafo BFS (Breadth First Search)
	// Per far cio' ho bisogno di una coda

	int k = 0;
		
	std::deque<ActivationRecord> Coda;

	ActivationRecord start;
	start.Target = this;
	start.nPoint = (i<<2)+j;
	start.Diff = diff;
  
	
	Coda.push_back(start);

	while(!Coda.empty())
	{
		// PASSO 1: Estraggo il primo della coda
		ActivationRecord e = *(Coda.begin());
		Coda.pop_front();

		// PASSO 2: se non e' gia stato visitato applico i bindings
		if (e.Target->BindingsApplied[e.nPoint])
			continue;
		
		k = 0;

		point adjustedPos = e.Target->getPoint(e.nPoint);
		adjustedPos = adjustedPos + e.Diff;

		e.Target->_setPoint(e.nPoint, adjustedPos);

		// lo marco visitato
		e.Target->BindingsApplied[e.nPoint] = true;

		unsigned char ii = e.nPoint >> 2;
		unsigned char jj = e.nPoint &  3;

		while(e.Target->Binding[ii][jj][k].Patch)
		{		
			BoundPoint bp = e.Target->Binding[ii][jj][k];
				
			if (!bp.Patch->BindingsApplied[bp.nPoint])
			{
				ActivationRecord newAR;
				point diff;
	
				if (bp.Inverse)
					diff = e.Diff * -1;
				else
					diff = e.Diff;

				
				newAR.Target = bp.Patch;
				newAR.nPoint = bp.nPoint;
				newAR.Diff = diff;

				Coda.push_back(newAR);
				bp.Patch->ReCompute();
			}
			
			++k;
		}

	}
}

void RenderedBezier::TranslatePoint(unsigned char pos, point p)
{
	int i,j;
	i = pos >> 2;
	j = pos & 3;
	
   setPoint(i,j, Points[i][j] + p);
}

void RenderedBezier::TranslatePoint(unsigned char i, unsigned char j, point p)
{
	setPoint(i,j, Points[i][j] + p);
}

point RenderedBezier::getPoint(unsigned char i, unsigned char j)
{
  return Points[i][j];
}

point RenderedBezier::getPoint(unsigned char pos)
{
  return Points[pos >> 2][pos & 3];
}

void RenderedBezier::_setPoint(unsigned char pos, point p)
{
	Points[pos >> 2][pos & 3] = p;
}

RenderedBezier *RenderedBezier::getConnectedPatch(Direction d)
{
	return Connected[d].Patch;
}

RenderedBezier::BoundPatch RenderedBezier::getConnected(Direction d)
{
	return Connected[d];
}

// Calcola le normali della patch
void RenderedBezier::ComputeNormals()
{
	DoNormalBorders();
	
	int j;

	int width = LevelToNumberCtrlpoints3[nDepth];

	// adesso se siamo collegati interpoliamo le normali sui bordi
	BoundPatch other;

	other = getConnected(NORTH);
	if (other.Patch && other.c1)
	{
		other.Patch->DoNormalBorders();
		for (j=0; j<width; ++j)
		{
			n[j*width] = BorderNormals[NORTH][j] + other.Patch->BorderNormals[other.Dir][j];
			n[j*width].normalize();			
		}
	}
	
	other = getConnected(SOUTH);
	if (other.Patch && other.c1)
	{
		other.Patch->DoNormalBorders();
		for (j=0; j<width; ++j)
		{
			n[j*width+width-1] = BorderNormals[SOUTH][j] + other.Patch->BorderNormals[other.Dir][j];
			n[j*width+width-1].normalize();			
		}
	}

	other = getConnected(WEST);
	if (other.Patch && other.c1)
	{
		other.Patch->DoNormalBorders();
		for (j=0; j<width; ++j)
		{
			n[j] = BorderNormals[WEST][j] + other.Patch->BorderNormals[other.Dir][j];
			n[j].normalize();
		}
	}

	other = getConnected(EAST);
	if (other.Patch && other.c1)
	{
		other.Patch->DoNormalBorders();
		for (j=0; j<width; ++j)
		{
			n[j+(width-1)*width] = BorderNormals[EAST][j] + other.Patch->BorderNormals[other.Dir][j];
			n[j+(width-1)*width].normalize();
		}
	}

}

void RenderedBezier::FlipNormals()
{
	int i;
	CubicBezierPatch::FlipNormals();

	// devo far ricalcolare le normali ai miei vicini

	for (i=0; i<4; i++)
	{
		RenderedBezier *other;
		if (other = Connected[i].Patch)
			other->ReCompute();
	}
}

void RenderedBezier::DoNormalBorders()
{
	CubicBezierPatch::ComputeNormals();
	
	int width = LevelToNumberCtrlpoints3[nDepth];
	int i,j;

	for (i=0; i<4; i++)
	{
		if (BorderNormals[i]) 
			delete BorderNormals[i];

		BorderNormals[i] = new vector3[width];
	}

	
	// adesso copio tutte le normali dei bordi...

	// NORTH!
	for (j=0; j<width; ++j)
		BorderNormals[WEST][j] = n[j];

	// SOUTH!
	for (j=0; j<width; ++j)
		BorderNormals[EAST][j] = n[j+(width-1)*width];

	// WEST!
	for (j=0; j<width; ++j)
		BorderNormals[NORTH][j] = n[j*width];

	// EAST!
	for (j=0; j<width; ++j)
		BorderNormals[SOUTH][j] = n[j*width+width-1];

}

// Mi collego con un altra patch
bool RenderedBezier::Attach(RenderedBezier *other, Direction myDir, Direction otherDir, bool c1)
{
// NOTA: Attach deve essere chiamata su una sola delle due patch

/*
	other					altra patch
	myDir					la mia direzione di collegamento
	otherDir				la direzione dell'altra patch
	c1						collegamento di tipo C1
*/
	int i;


  if (getConnectedPatch(myDir))
    return false;           // sono gia' impegnato...

  int nMyConnections = 0;
  int nOtherConnections = 0;

  //chi e' il piu' connesso?
  for (i = 0; i < 4; ++i)
  {
    if (getConnectedPatch((Direction)i))
      ++nMyConnections;
    if (other->getConnectedPatch((Direction)i))
      ++nOtherConnections;
  }

  //spostiamo i punti "a mano"
  //in modo che siano gia' pronti per l'unione
  if (nMyConnections < nOtherConnections)
  {
    //io sono il piu' libero
    for (i = 0; i < 4; ++i)
    {
      unsigned char nodoMio = PointsInDir[myDir][i].Border;
      unsigned char nodoAltrui = PointsInDir[otherDir][i].Border;

      unsigned char nodoMioDentro = PointsInDir[myDir][i].Inside;
      unsigned char nodoAltruiDentro = PointsInDir[otherDir][i].Inside;

      setPoint(nodoMio, other->getPoint(nodoAltrui));
      point PuntoDentroNew = other->getPoint(nodoAltruiDentro) - 
                             other->getPoint(nodoAltrui);
      PuntoDentroNew = getPoint(nodoMio) - PuntoDentroNew;
      setPoint(nodoMioDentro, PuntoDentroNew);
    }
  }
  else
  {
    //il mio partner è piu' libero
    for (i = 0; i < 4; ++i)
    {
      unsigned char nodoMio = PointsInDir[myDir][i].Border;
      unsigned char nodoAltrui = PointsInDir[otherDir][i].Border;

      unsigned char nodoMioDentro = PointsInDir[myDir][i].Inside;
      unsigned char nodoAltruiDentro = PointsInDir[otherDir][i].Inside;

      other->setPoint(nodoAltrui, getPoint(nodoMio));
      point PuntoDentroNew = getPoint(nodoMioDentro) - 
                             getPoint(nodoMio);
      PuntoDentroNew = other->getPoint(nodoAltrui) - PuntoDentroNew;
      other->setPoint(nodoAltruiDentro, PuntoDentroNew);
    }
  }

  Connected[myDir].Patch = other;		// il mio partner
  Connected[myDir].Dir = otherDir;
  Connected[myDir].c1 = c1;
   
  other->Connected[otherDir].Patch = this;
  other->Connected[otherDir].Dir = myDir;
  other->Connected[otherDir].c1 = c1;


	// collegamento C0
	for (i = 0; i<4; ++i)
	{
		unsigned char nodo1bordo = PointsInDir[myDir][i].Border;
		unsigned char nodo2bordo = PointsInDir[otherDir][i].Border;

		unsigned char nodo1dentro = PointsInDir[myDir][i].Inside;
		unsigned char nodo2dentro = PointsInDir[otherDir][i].Inside;

		point medio;

		if (!c1)
			medio = (getPoint(nodo1bordo) + other->getPoint(nodo2bordo)) * 0.5f;
		else
			medio = (getPoint(nodo1dentro) + other->getPoint(nodo2dentro)) * 0.5f;
		
		AddBinding(nodo1bordo, other, nodo2bordo, false, other);
		
		setPoint(nodo1bordo, medio);
		other->setPoint(nodo2bordo, medio);

		other->AddBinding(nodo2bordo, this, nodo1bordo, false, this);
		
		// se c1 ho bisogno anche di questi vincoli
		if (c1)
		{
			AddBinding(nodo1dentro, other, nodo2dentro, true, other);
			AddBinding(nodo1bordo, this, nodo1dentro, false, other);
			AddBinding(nodo1bordo, other, nodo2dentro, false, other);

			other->AddBinding(nodo2dentro, this, nodo1dentro, true, this);
			other->AddBinding(nodo2bordo, other, nodo2dentro, false, this);
			other->AddBinding(nodo2bordo, this,  nodo1dentro, false, this);
			
		}
	}

	/*
	// verifico che entrambi le patch abbiano la stezza °direzione° delle normali
	if (FlippedNormals() != other->FlippedNormals())
		other->FlipNormals();		
	*/

	// applica i cambiamenti!
	ReCompute();
	other->ReCompute();

	//... controllo se l'unione e' andata a buon fine
	for (i = 0; i<4; ++i)
	{
		unsigned char nodo1bordo = PointsInDir[myDir][i].Border;
		unsigned char nodo2bordo = PointsInDir[otherDir][i].Border;

		point diff = getPoint(nodo1bordo) - other->getPoint(nodo2bordo);
		if (!ALMOST_ZERO(diff.x) ||
			 !ALMOST_ZERO(diff.y) ||
			 !ALMOST_ZERO(diff.z))
		{
			// non siamo riusciti ad unire la patch!! =(
			Detach(myDir);
			return false;
		}
	}

	return true;
}

bool RenderedBezier::Detach(Direction d)
{
	// NOTA: Detach deve essere chiamata da una sola delle due patch

	if (!getConnectedPatch(d))
		return false;					// non ho nessuno al moooondoo...

	// devo rimuovere tutti i bindings in questa direzione
	RenderedBezier *other = Connected[d].Patch;
  
	int i;
	for (i = 0; i<4; ++i)
	{
		unsigned char nodobordo = PointsInDir[d][i].Border;
		unsigned char nododentro = PointsInDir[d][i].Inside;

		// devo annullare tutti i binding che riguardano l'altra patch

		RemAllBindings(nodobordo, other);
      RemAllBindings(nododentro, other);      
	}

	// mi scollego	
	Connected[d].Patch = NULL;
	Direction Dir = Connected[d].Dir;
	Connected[d].Dir = UNKNOWN;
	
	// faccio scollegare anche il mio partner
	if (other->getConnectedPatch(Dir) == this)
		return other->Detach(Dir);
	
	// tutto ok.
	return true;
}

// Rimuove tutti i bindings sul punto (i,j) causati dalla patch 'attachment'.
void RenderedBezier::RemAllBindings(unsigned char nodo1, RenderedBezier* attachment)
{
	int i = nodo1 >> 2;
	int j = nodo1 & 3;
	int k = 0;
   
   int nBindings = 0;

   while(Binding[i][j][nBindings].Patch)
   	++nBindings;

   while(Binding[i][j][k].Patch)
	{
   	BoundPoint bp = Binding[i][j][k];
      if (bp.Attached == attachment)
      {
			if (k == nBindings-1)
			{
				Binding[i][j][k].Patch = NULL;
				break;
			}

      	Binding[i][j][k] = Binding[i][j][nBindings-1];
         Binding[i][j][nBindings-1].Patch = NULL;
         nBindings--;
         if (nBindings == 0)
         	break;
      }
      else
	   	++k;
   }

}

// un nuovo binding
void RenderedBezier::AddBinding(unsigned char nodo1, RenderedBezier* other, unsigned char nodo2, bool inv, RenderedBezier* attachment)
{
/*
	nodo1				il nodo che genera il vincolo
	other				puntatore alla patch su cui e' presente il vincolo
	nodo2				numero del nodo che subira' l'azione del vincolo
	inv				se vero i movimenti di nodo2 saranno inveriti rispetto a quelli del nodo1
	attachment		puntantore alla patch che ha causato questo binding 
						(utile per rimuovere il binding dopo che questa patch viene scollegata)
*/

	int i = nodo1 >> 2;
	int j = nodo1 & 3;
	int k = 0;

	while(Binding[i][j][k].Patch)
	{
		// devo controllare che non sia gia' presente
		if (Binding[i][j][k].Patch == other &&
			 Binding[i][j][k].nPoint == nodo2)
			 
			 return;
		++k;
	}

	Binding[i][j][k].Attached = attachment;
	Binding[i][j][k].Patch = other;
	Binding[i][j][k].nPoint = nodo2;
	Binding[i][j][k].Inverse = inv;
	
}

void RenderedBezier::DrawBindings(unsigned char pos, bool on)
{
   // devo far diventare di color blu tutti i punti legati a questo
	int k = 0;
   int i = pos>>2;
   int j = pos&3;
   
   BindingsApplied[pos] = true;
   
	while(Binding[i][j][k].Patch)
	{      
		BoundPoint bp = Binding[i][j][k];
      
      if (!bp.Patch->BindingsApplied[bp.nPoint])
      {
         bp.Patch->BindingsApplied[bp.nPoint] = true;
	      bp.Patch->Binded[bp.nPoint] = on;
			bp.Patch->DrawBindings(bp.nPoint, on);
      }
		++k;
	}  
}
