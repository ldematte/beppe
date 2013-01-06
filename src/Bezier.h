/*******************************************************************\
*  Bezier Patch Editor  ---  Bezier                                 *
*                                                                   *
*  Incapsula le superfici bi-cubiche di Bezier.                     *
\*******************************************************************/

#ifndef BEZIER_H_INCLUDED
#define BEZIER_H_INCLUDED

#include "common.h"

// Per vettori, punti...
#include "Mtxlib.h"


///////////////////////////////////////////////////////////////////
//class CubicBezierPatch
//
//

class CubicBezierPatch
{
public:

  //////////////// Constructors / Destructor /////////////////////

  CubicBezierPatch();  
  CubicBezierPatch(point cpoints[][4]);
  
  ~CubicBezierPatch();
      
  /////////////////////////// Draw ////////////////////////////////

  void DrawPatch();  
  
  ///////////////////////// Accessors /////////////////////////////

  // Mostra/nasconde i punti valutati
  void ShowPoints(bool b);  

  // Setta/ottiene il numero di volte in cui la patch viene valutata
  void SetDepth(int n);
  int Depth();
  
  // La patch deve essere ricomputata?
  bool IsDirty();
  
  // Setta il flag dirty (la patch deve essere ricalcolata) a true
  void ReCompute();

  // Gira la normali e la direzione delle facce
  virtual void FlipNormals();
  
  // Le nostre normali sono state girate?
  bool FlippedNormals();


protected:

  // Calcola le normali
  virtual void ComputeNormals();

  ////////////////////////////////////////////////////////////////////////
  // Lookup table per il numero TOTALE di punti (di controllo)
  // necessari se facciamo n iterazioni nel processo di valutazione
  // per suddivisione.
  //
  // La formula usata per calcolarene il numero e'
  // livello(n) = livello(n-1) * 2 - 1
  //
  // (Se avete una GeForce4 potreste volerne di piu'...
  //

  static const int LevelToNumberCtrlpoints2[]; 
  static const int LevelToNumberCtrlpoints3[];

  // I nostri punti di controllo (i 16 originali)
  point Points[4][4];
  bool bNormals;

  bool bShowEvalPoints;  

  bool bDirty;
  int  nDepth;
	/////////////////////// Vettore delle normali/////////////////////////
  vector3* n;  
  
private:  
  GLuint  nList;  

  // Inizializza le variabili membro
  void Init();
  
  ////////////////////////// Evalutors /////////////////////////////////  
 
  void CubicBezierSubdivide2(point G[], int level);
  point* CubicBezierPatchSubdivide2(point G[][4], int level);
  void DrawCubicBezierPatch(bool fCtrlPoints);


private:
  //////////////////////// I punti valutati /////////////////////////
  //
  // Il vettore ha dimensione LevelToNumberCtrlpoints3[nDepth]^2
  //
  point* p; 
 
  
};


#endif //BEZIER_H_INCLUDED

































/*
Coniglietto        Coniglietto
                      allucinato

     (\/)            (\/)
    =°_°=           =Q_Q=
   (,,)(,,)        (,,)(,,)

*/