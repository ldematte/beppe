/*******************************************************************\
*  Bezier Patch Editor  ---  EditorBezierPatch                      *
*                                                                   *
*  Questa classe identifica una patch di bezier con tutti gli       *
*  strumenti necessari per essere utilizzata dall'editor.           *
*  Fornisce funzioni per selezionare/deselezionare punti, per       *
*  disegnarli e per disegnare il poligono di controllo.             *
\*******************************************************************/

#if !defined(EDITORBEZIERPATCH_H_INCLUDED)
#define EDITORBEZIERPATCH_H_INCLUDED

#include <list>
#include "RenderedBezier.h"

class EditorBezierPatch : public RenderedBezier  
{
public:
	
  //////////////////////////// Constructor/ Destructor /////////////////////////////////
  EditorBezierPatch(unsigned int id, unsigned int matsel, unsigned int matdesel, unsigned int matbinded, GLuint callSphere);
	EditorBezierPatch(unsigned int id, unsigned int matsel, unsigned int matdesel, unsigned int matbinded, point cpoints[][4], GLuint callSphere);

	virtual ~EditorBezierPatch();

  // Disegna il poligono di controllo
	void DrawCtrlPolygon();
  
  // Disegna i punti di controllo in 3D (sferette)
	void DrawCtrlPoints3d();
  
  // Disegna i punti di controllo selezionati in 2D (punti rossi)
	void DrawCtrlPoints2dSelected();
  
  // Disegna i punti di controllo non selezionati in 2D (punti bianchi)
	void DrawCtrlPoints2dDeselected();

	//-------------------- selezione ----------------------
  // Tutte le funzioni sono duplicate : prendono un unico
  // parametro (punto da 0 a 15) oppure 2 (punto alle 
  // ccordinate della griglia del punti di controllo
  // 0..3, 0..3)

  // Seleziona
	void Select(unsigned char pos);
	void Select(unsigned char i, unsigned char j);
  void SelectAll();
  
  // Deseleziona
	void Deselect(unsigned char pos);
  void Deselect(unsigned char i, unsigned char j);
	void SelectNone();

  // E' selezionato?
	bool isPatchSelected();
	bool isSelected(unsigned char pos);
	bool isSelected(unsigned char i, unsigned char j);

  // Scambia selezione/non selezione e viceversa
	void Toggle(unsigned char pos);
	void Toggle(unsigned char i, unsigned char j);
  
	// Nascondi/mostra di nuovo punti di controllo
	void HideSelected();
	void UnhideAll();
   
	//@@@@@@@@@@@@@@ TODO @@@@@@@@@@@@@@@@@@@@@@@@
	// magari usare un costruttore di copia (Ale)
  // magari un'altra volta.. (Dema)
	EditorBezierPatch *Clone();

protected:
		
  // materiale per i punti di controllo selezionati
	unsigned int matSel;		

  // materiale per i punti di controllo deselezionati
	unsigned int matDesel;	
  
  // materiale per i punti di controllo collegati (blu)
	unsigned int matBinded;	

  // punti selezionati (rosso)
	bool	Selected[16];
  
  // punti nascosti		
	bool	Hided[16];		
	
 	void Drawpoint3d(point P);

	unsigned int nPointSelected;

private:
    
  // Inizializza tutta la baracca
	void Init();

	unsigned char selColor[3];
	unsigned char deselColor[3];

  // call list di una sferetta
	GLuint lSphere;	
};

#endif // !defined(EDITORBEZIERPATCH_H_INCLUDED)
