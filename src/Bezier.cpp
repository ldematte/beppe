/*******************************************************************\
*  Bezier Patch Editor  ---  Bezier                                 *
*                                                                   *
*  Incapsula le superfici bi-cubiche di Bezier.                     *
\*******************************************************************/

#include <stdlib.h>
#include "Bezier.h"

// Initialization of static members
//

////////////////////////////////////////////////////////////////////////
//
// Lookup table per il numero TOTALE di punti (di controllo)
// necessari se facciamo n iterazioni nel processo di valutazione
// per suddivisione.
//
// La formula usata per calcolarene il numero e'
// livello(n) = livello(n-1) * 2 - 1
//
// (Se avete una GeForce4 potreste volerne di piu'...)
//
// il 2 sta per "quadratic" (curve/pezze di grado 2)
// il 3 sta per "cubic" (curve/pezze di grado 3)
//

const int CubicBezierPatch::LevelToNumberCtrlpoints2[] = {3, 5, 9, 17, 33, 65, 129};
const int CubicBezierPatch::LevelToNumberCtrlpoints3[] = {4, 7, 13, 25, 49, 97, 193};

////////////////////////////////////////////////////////////////////////

CubicBezierPatch::CubicBezierPatch()
{
	Init();    
}

CubicBezierPatch::CubicBezierPatch(point cpoints[][4])
{
	Init();
    memcpy((void*)Points, (const void*)cpoints, sizeof(point) * 16);  	
}

CubicBezierPatch::~CubicBezierPatch()
{    
	glDeleteLists(nList, 1);
}

////////////////////////////////////////////////////////////////////////

void CubicBezierPatch::Init()
{	
  bShowEvalPoints = false;
  bDirty = true;
  bNormals = false;
  nDepth = 2; 
  p = NULL; 
  n = NULL;
  nList = glGenLists(1);
}

////////////////////////////////////////////////////////////////////////

void CubicBezierPatch::DrawPatch()
{
	
  int j;

  //Draw the Bezier Patch

  glFrontFace(bNormals ? GL_CW : GL_CCW);

  if (bDirty)
  {
    glDeleteLists(nList, 1);
    nList = glGenLists(1);
    
    // calcola i punti e le normali
    ComputeNormals();
    bDirty = false;
  
    int width = LevelToNumberCtrlpoints3[nDepth];
	  float step = 1.0f / (width - 1);

    // Mettiamo il tutto in una CallList per aumentare l'efficienza (vedi note)
	  glNewList(nList, GL_COMPILE);

    for (int i = 0; i < width - 1; ++i)
    {
		  float x = 0.0;
		  float y = i * step;

      // Disegnamo un singolo pezzo di patch piatto
      // Usiamo triangoli invece che quadrilateri perche' non
      // e' detto che i 4 punti siano complanari, e questo
      // puo' essere un problema per qualche implementazione di OpenGL
      
		  glBegin(GL_TRIANGLE_STRIP);
		  for (j = 0; j < width; ++j) 
      {
 			  //texture
        glTexCoord2f(x, y);
        glNormal3f(n[i * width + j].x, n[i * width + j].y, n[i * width + j].z);
			  glVertex3f(p[i * width + j].x, p[i * width + j].y, p[i * width + j].z);
        //texture
 			  glTexCoord2f(x, y + step);
        glNormal3f(n[(i + 1) * width + j].x, n[(i + 1) * width + j].y, n[(i + 1) * width + j].z);
			  glVertex3f(p[(i + 1) * width + j].x, p[(i + 1) * width + j].y, p[(i + 1) * width + j].z);
			  x += step;
      }
		  glEnd();

      x = 0.0;
		  y = i * step;
      
      // Adesso se richiesto disegnamo i punti valutati dall'algoritmo
      // (utile per il debug)
      if (bShowEvalPoints)
      {
        glPointSize(3.0);

        glPushAttrib(GL_CURRENT_BIT);

        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_POINTS);
		    for(j = 0; j < width; j++) 
        {
			    glVertex3f(p[i * width + j].x, p[i * width + j].y, p[i * width + j].z);
			    glVertex3f(p[(i + 1) * width + j].x, p[(i + 1) * width + j].y, p[(i + 1) * width + j].z);
			    x += step;
        }
		    glEnd();

        glPopAttrib();
      }
      
    }
    
    glEndList();
  }
  
  glCallList(nList);
  glFrontFace(GL_CCW);

}

////////////////////////////////////////////////////////////////////////
//
// Valutazione di una curva di Bezier cubica, seconda versione
// Dalla versione 1, miglioramento di efficienza: viene
// usato un vettore dei nodi unico e allocato fin dall'inizio
// invece di continuare ad allocare, separare e fondere vettori
//
// Il codice e' u  po' piu' complesso
//
void CubicBezierPatch::CubicBezierSubdivide2(point G[], int level)
{
	// Non suddividiamo piu'
	if(level == 0)
		return;

	// Calcoliamo dove stanno i 4 punti di controllo per questa iterazione
  // all'interno del vettore totale (che contiene/conterra' TUTTI punti di
  // controllo, gia' nella loro giusta posizione
  
	int width = LevelToNumberCtrlpoints3[level] - 1;
	
  // Alcuni indici che ci servono...
  
  // Il punto tra il primo terzo e il secondo terzo
  int mid1 = width / 3; 
  
  // Il punto tra il secondo terzo e la fine
  int mid2 = mid1 * 2;

  // Il centro del primo terzo
  int i = mid1 / 2;
  
  // Il centro del secondo terzo
  int ii = (mid1 + mid2) / 2;
  
  // Il centro dell'ultimo terzo
  int iii = (width + mid2) / 2;
  
  // Nei commenti seguenti: le istruzioni della prima versione
  // (aiutano a leggere il codice)
  
  // Splittiamo il vettore dei punti in 2 parti, che saranno passate
  // alla funzione ricorsivamente.
  // Prima pero' perr ognuna delle 2 meta', calcola i nuovi punti di 
  // controllo infilandoli nel loro posto nel vettorone (grazie agli
  // indici che ci siamo appena calcolati

  //Middlepoint = (P[1] + P[2]) * 0.5f;
  point Middlepoint = (G[mid1] + G[mid2]) * 0.5f;

  //L[0] = P[0];
  //R[3] = P[3];
  //L[1] = (P[0] + P[1]) * 0.5f;
  G[i] = (G[0] + G[mid1]) * 0.5f;

  //L[2] = (L[1] + Middlepoint) * 0.5f;
  G[mid1] = (G[i] + Middlepoint) * 0.5f;

  //R[2] = (P[2] + P[3]) * 0.5f;
  G[iii] = (G[mid2] + G[width]) * 0.5f;

  //R[1] = (Middlepoint + R[2]) * 0.5f;
  G[mid2] = (Middlepoint + G[iii]) * 0.5f;
  
  //L[3] = R[0] = (L[2] + R[1]) * 0.5f;
  G[ii] = (G[mid1] + G[mid2]) * 0.5f;

	//Call recursively for left and right halves
  //TODO: explicit recursion of one half (if possible)
	CubicBezierSubdivide2(G, --level);
	CubicBezierSubdivide2(&G[ii], level);
}

////////////////////////////////////////////////////////////////////////
//
// L'analogo della procedura precedente, solo che agisce su patch.
//
// Si pensi che i 16 punti di controllo iniziali siano inseriti in una 
// matrice 4x4 i cui indici di riga e di colonna sono rispettivamente s e t,
// points[s][t]
// Quello che si fa e' generare i punti in due fasi lungo le due direzioni 
// s e t, usando l'algoritmo dato sopra:
// 
// A) si mantiene s costante e si incrementa t (dando alla 
//    CubicBezierSubdivide il vettore riga points[s][])
// 
// B) si mantiene t costante e si incrementa s (dando alla 
//    CubicBezierSubdivide il vettore colonna points[][t])
//

point* CubicBezierPatch::CubicBezierPatchSubdivide2(point G[][4], int level)
{
	// Calculate some indexes
	int width = LevelToNumberCtrlpoints3[level];

	int mid1 = (width - 1) / 3;
  int mid2 = mid1 * 2;

  int total = width * width;
  int i;


  // Alloca lo spazio per i punti sulla superficie e per l'interpolazione dei 
  // punti di controllo
  // Alla fine avremo 'total' punti di controll; inoltre abbiamo bisogno di 
  // 4 * width punti come spazio temporaneo per la computazione
	point* p = new point[total + 4 * width];

	// Interpola i punti di controllo usando la matrice geometrica
  for(i = 0; i < 4; i++) 
  {
    // Distribuisce i punti di controllo nello spazio usato per 
    // l'interpolazione		
    p[total] = G[i][0];
		p[total + mid1] = G[i][1];
    p[total + mid2] = G[i][2];
		p[total + width - 1] = G[i][3];
		
    
    // Prima interpolazione dei punti di controllo
    // Interpola lungo l'asse s 
    CubicBezierSubdivide2(&p[total], level);
		
    // ricopia i punti nella loro giusta posizione 
    point* index = p + i * mid1;
		for(int j = 0; j < width; j++)
    {
			*index = p[total + j];
			index += width;
		}

		total += width;
	}

	// Seconda interpolazione dei punti di controllo
  // Interpola lungo l'asse t 
	for(i = 0; i < width; i++) 
  {
		CubicBezierSubdivide2(&p[i * width], level);
	}

	// Ritorna il vettore di punti valutati
  // Deve essere liberato (con delete []) dal chiamante!
	return p;
}

////////////////////////////////////////////////////////////////////////
//
// Si calcola le normali, interpolando per ogni vertice le normali
// analitiche alle sei superfici (trinagoli) adiacenti
//
void CubicBezierPatch::ComputeNormals()
{
  
  int i, j;

  if (p) delete[] p;
	  p = CubicBezierPatchSubdivide2(Points, nDepth);

  if (n) delete[] n;

	int width = LevelToNumberCtrlpoints3[nDepth];

  typedef vector3** PPVECTOR;
  typedef vector3* PVECTOR;

  
  //t[riga][colonna][triangolo alto/basso]

  // temporaneo ^_^'
  // Tiene le normali analitiche per ogni triangolo
  vector3 ***t = (vector3***)malloc(sizeof(vector3**) * width);

  for (i = 0; i < width; ++i)
  {
    t[i] = (vector3**)malloc(sizeof(vector3*) * width);
    for (j = 0; j < width; ++j)
    {
      t[i][j] = (vector3*)malloc(sizeof(vector3) * 2);
    }
  }
  

	n = new vector3[width * width];

  for (i = 0; i < width - 1; ++i)
  {
		for (j = 0; j < width - 1; ++j) 
    {
 			vector3 a = vector3(p[i * width + j].x - p[(i + 1) * width + j].x, 
                          p[i * width + j].y - p[(i + 1) * width + j].y,
                          p[i * width + j].z - p[(i + 1) * width + j].z);

      vector3 b = vector3(p[i * width + j].x - p[i * width + j + 1].x, 
                          p[i * width + j].y - p[i * width + j + 1].y,
                          p[i * width + j].z - p[i * width + j + 1].z);

      vector3 c = vector3(p[(i + 1) * width + j + 1].x - p[i * width + j + 1].x, 
                          p[(i + 1) * width + j + 1].y - p[i * width + j + 1].y,
                          p[(i + 1) * width + j + 1].z - p[i * width + j + 1].z);

      vector3 d = vector3(p[(i + 1) * width + j + 1].x - p[(i + 1) * width + j].x, 
                          p[(i + 1) * width + j + 1].y - p[(i + 1) * width + j].y,
                          p[(i + 1) * width + j + 1].z - p[(i + 1) * width + j].z);

      t[i][j][0] = CrossProduct(a, b);
      t[i][j][1] = CrossProduct(c, d);
		}
  }

  //First angle
  n[0] = t[0][0][0];
  n[0].normalize();
  //Second angle
  n[width - 1] = t[0][width - 2][0] + t[0][width - 2][1];
  n[width - 1].normalize();
  //Third angle
  n[width * (width - 1)] = t[width - 2][0][0] + 
                           t[width - 2][0][1];
  n[width * (width - 1)].normalize();
  //Fourth angle
  n[width * width - 1] = t[width - 2][width - 2][1];
  n[width * width - 1].normalize();

  //North boundary
  for (j = 1; j < width - 1; ++j)
  {
    n[j] = t[0][j][0] + t[0][j - 1][0] + t[0][j - 1][1];
    n[j].normalize();
  }

  //South boundary
  for (j = 1; j < width - 1; ++j)
  {
    n[j + (width - 1) * width] = t[width - 2][j][1] +t[width - 2][j][0] + t[width - 2][j - 1][1];
    n[j + (width - 1) * width].normalize();
  }

  //West boundary
  for (i = 1; i < width - 1; ++i)
  {
    n[i * width] = t[i - 1][0][0] + t[i][0][0] + t [i][0][1];
    n[i * width].normalize();
  }

  //East boundary
  for (i = 1; i < width - 1; ++i)
  {
    int j = width - 2;

    n[i * width + j + 1] = t[i - 1][j][1] + t[i][j][1] + t[i-1][j][0];
    n[i * width + j + 1].normalize();
  }

  //Interpolating internal points...
  for (i = 1; i < width - 1; ++i)
  {
		for (j = 1; j < width - 1; ++j) 
    {
  		n[i * width + j] = t[i - 1][j - 1][1] + 
                         t[i - 1][j][0] + t[i - 1][j][1] +
                         t[i][j][0] + 
                         t[i][j - 1][1] + t[i][j - 1][0];
        
      n[i * width + j].normalize();
		}
  }

  if (bNormals)
  {
    for (i = 0; i < width * width; ++i)
      n[i] *= -1.0f;
  }

  //delete!!!
  for (i = 0; i < width; ++i)
  {
    for (j = 0; j < width; ++j)
      free(t[i][j]);
 
    free(t[i]);
  }
  free(t);
  
}

////////////////////////////////////////////////////////////////////////

void CubicBezierPatch::DrawCubicBezierPatch(bool bEvalPoints)
{}

////////////////////////////////////////////////////////////////////////
//
// Gestione orientamento facce
//

bool CubicBezierPatch::FlippedNormals()
{
  return bNormals;
}

void CubicBezierPatch::FlipNormals()
{
  bNormals  = !bNormals;
}

////////////////////////////////////////////////////////////////////////
//
// Disegnare i punti valutati?
//

void CubicBezierPatch::ShowPoints(bool b)
{
  bShowEvalPoints = b;
  ReCompute();
}

////////////////////////////////////////////////////////////////////////
//
// Gestione grado di tassellazione
//

void CubicBezierPatch::SetDepth(int n)
{
	int prevDepth = nDepth;
  if (n < 0)
    nDepth = 0;
  else if (n > 6)
    nDepth = 6;
  else
    nDepth = n;

  if (nDepth != prevDepth)
    ReCompute();
}

int CubicBezierPatch::Depth()
{
  return nDepth;
}

////////////////////////////////////////////////////////////////////////
//
// Lazy recomputing :-) Non ricalcolo nulla fino a quando
// non e' strettamente necessario
//

bool CubicBezierPatch::IsDirty()
{
  return bDirty;
}

void CubicBezierPatch::ReCompute()
{
  bDirty = true;
}
