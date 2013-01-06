/*******************************************************************\
*  Bezier Patch Editor  ---  PatchMgr                               *
*                                                                   *
*  Questa classe e' il vero cuore dell'applicazione. Gestigne un    *
*  insieme di patch, di materiali e di texture. Tutte le            *
*  funzionalita' principali dell'editor sono implementate in questa *
*  classe. Spesso i comandi disponibili con l'interfaccia grafica   *
*  sono implementati chiamando una singola funzione del PatchMgr.   *
\*******************************************************************/

#if !defined(PATCHMGR_H__INCLUDED_)
#define PATCHMGR_H__INCLUDED_

#include "common.h"
#include "Logger.h"
#include "MaterialMgr.h"
#include "TextureMgr.h"
#include "EditorBezierPatch.h"

#include <list>
#include <map>

class PatchMgr  
{
public:

  // Informazioni su un punto selezionato
  struct SelPoint
  {
    unsigned int nPatch;		// Numero di patch
    unsigned int nPoint;		// Numero del punto [0, 15]
  };

  // Ritorna l'istanza del Patch Manager <<singleton>>
  static inline PatchMgr* getInstance()
  {
    if (!instance)
      instance = new PatchMgr();
    return instance;
  }

  // Elimina tutte le patch (chiamato sul New Project)
  void ResetWorkspace();	

  // Crea una nuova patch
  EditorBezierPatch* CreatePatch(point ul, point br);

  // Ritorna il puntatore ad una patch dato il suo numero
  EditorBezierPatch* getPatch(int nPatch);
  
  // Applica il materiale a tutte le patch selezionate
  void SetMaterial(unsigned int mat);

  // Applica la texture a tutte le patch selezionate
  void SetTexture(unsigned int tex);

  // Disegna i punti di controllo
  void DrawCtrlPoints(bool treD);

  // Disegna la scena per un viewport 2d
  void DrawScene2D(bool drawCPoints, bool drawCPoly, bool drawPatch);

  // Disegna la scena per un viewport 3d
  void DrawScene3D(bool drawCPoints, bool drawCPoly, bool drawPatch);

  // Seleziona un punto
  void SelectPoint(int nPatch, int nPoint);

  // Deseleziona un punto
  void DeselectPoint(int nPatch, int nPoint);

  // Inverte la selezione di un punto
  void TogglePoint(int nPatch, int nPoint);

  // Tutti i punti vengono deselezionati
  void ClearSelection();

  // Tutti i punti vengono selezionati
  void SelectAll();

  // Seleziona completamente le patch che hanno gia' almeno un punto selezionato
  void SelectPatch();

  // Elimina tutte le patch che hanno punti selezionati
  void DeletePatch();

  // Nasconde i punti di controllo selezionati (non potranno piu' essere pickati)
  void Hide();

  // Mostra tutti i punti di controllo
  void UnhideAll();

  // Copia le patch selezionate nella clipboard e le rimuove dalla scena (taglia)
  void Cut();

  // Copia le patch selezionate nella clipboard (copia)
  void Copy();

  // Inserisce nel progetto le patch della clipboard (incolla)
  void Paste();

  // Verifica se la clipboard e' vuota
  bool isClipboardEmpty();

  // Verifica se esistono punti selezionati
  bool isSelectionEmpty();

  // Sposta tutti i punti selezionati nel loro baricentro geometrico
  void CollapsePoints();

  // Applica uno spostamento ai punti selezionati  (dx, dy, dz)
  void MovePointsWithDelta(float dx, float dy, float dz);

  // Sposta tutti i punti selezionati al punto di coordinate (x,y,z)
  void MovePoints(float x, float y, float z);  

  // Applica uno spostamento ai punti selezionati rimanendo su una griglia
  void MovePointsWithDeltaAccum(float x, float y, float z, float gridSize);

  // Restta gli accumulatori usati per lo spostamento su griglia (Snap to grid)
  void ClearAccum();

  // Inverte le normali di tutte le patch selezionate
  void FlipNormals();

  // Rimove il materiale dalle patch nel progetto e dalle patch in clipboard
  void RemoveMaterialFromProject(unsigned int mat);

  // Rimove la texture dalle patch nel progetto e dalle patch in clipboard
  void RemoveTextureFromProject(unsigned int tex);

  // Analizza i punti selezionati e verifica se e' una selezione corretta per un unione
  bool CheckJoinPatchSelection(int &Patch1, Direction &Dir1, int &Patch2, Direction &Dir2);

  // Aumenta la tassellazione delle patch selezionate
  void IncreaseTassellation();

  // Diminuisce la tassellazione delle patch selezionate
  void DecreaseTassellation();

  // Effettua una riflessione sul viewport Top
  void MirrorTopView(point Start, point End);

  // Effettua una riflessione sul viewport Front
  void MirrorFrontView(point Start, point End);

  // Effettua una riflessione sul viewport Side
  void MirrorSideView(point Start, point End);

  // Effettua una rotazione sul viewport Top
  void RotateTopView(point Start, point End);

  // Effettua una rotazione sul viewport Front
  void RotateFrontView(point Start, point End);

  // Effettua una rotazione sul viewport Side
  void RotateSideView(point Start, point End);

  // Salva il progetto su file
  bool Save(const char* filename);

  // Carica il progetto da file
  bool Load(const char* filename);  

  // Unisce due patch (continuita' c0 o c1)
  bool JoinPatches(int Patch1, Direction Dir1, int Patch2, Direction Dir2, bool c1);

  // Separa due patch che erano unite
  bool SplitPatches(int Patch1, Direction Dir1, int Patch2, Direction Dir2);

  // Ritorna vero se il progetto e' stato modificato dall'ultimo salvataggio
  bool isModified() { return bModified; }

  // Lista dei materiali utilizzati nel progetto
  typedef std::list<unsigned int> MATERIALLIST;
  MATERIALLIST MaterialList;

  // Lista delle texture utilizzate nel progetto
  typedef std::list<unsigned int> TEXTURELIST;
  TEXTURELIST TextureList;


private:
  PatchMgr();

  static PatchMgr* instance;		// Istanza singleton

  Logger* log;
  MaterialMgr* MM;
  TextureMgr* TM;
  
  // Contiene le informazioni sui binding 
  // (utilizzata in fase di caricamento da file)
  struct TempBind
  {
    unsigned int Patch1Id;		// numero della prima patch
    unsigned int Patch2Id;		// numero della seconda patch
    Direction Patch1Dir;		// direzione di collegamento prima patch
    Direction Patch2Dir;		// direzione di collegamento seconda patch
    bool c1;						// grado di continuita' richiesta (c0/c1)
  };

  ////////////////////////// Typedefs //////////////////////////////////
  typedef std::map<unsigned int, EditorBezierPatch*> PATCHMAP;
  typedef std::list<EditorBezierPatch*> PATCHLIST;
  typedef std::list<SelPoint> SELECTIONLIST;
  typedef std::list<TempBind> BINDLIST;

  ////////////////////////// Materials /////////////////////////////////

  unsigned int matSel;			// Materiale dei punti di controllo selezionati
  unsigned int matDesel;		// Materiale dei punti di controllo deselezionati
  unsigned int matBinded;		// Materiale dei punti di controllo legati
  unsigned int matDefault;		// Materiale di default delle patch

  GLuint SphereCallList;		// Call list per disegnare una sfera (x punti di controllo)
  
  ///////////////////////// Patch lists ////////////////////////////////

  SELECTIONLIST SelectedPointList;	// Lista dei punti selezionati
  PATCHMAP patches;						// Insieme du tutte le patch presenti nel progetto
  PATCHLIST Clipboard;					// Patch nella clipboard (x copia incolla)
  BINDLIST BindList;						// Utilizzato in fase di caricamento

  ////////////////// For MovePointsWithDeltaAccum //////////////////////
  float deltax;
  float deltay;
  float deltaz;

  unsigned int nNextPatch;	// il numero della prossima patch da creare
  bool bModified;				// vero se il progetto è stato modificato dall'ultimo salvataggio

  // Svuota la clipboard x il copia-incolla
  void ClearClipboard();	

  // Carica una patch da file
  bool LoadPatch(FILE *f);

  // Riempie una lista con le posizioni di tutti i punti selezionati
  void FillWithSelectedPositions(std::list<point> &out);

  // Funzione di utilita' per effettuare le riflessioni
  bool mirror(float start_x, float start_y, float diff_x, float diff_y, float &dx, float &dy, float &sin_a, float &cos_a);

  // Funzione di utilita' per effettuare le rotazioni
  bool rotate(float start_x, float start_y, float diff_x, float diff_y, float &sin_a, float &cos_a);

};

#endif // !defined(PATCHMGR_H__INCLUDED_)
