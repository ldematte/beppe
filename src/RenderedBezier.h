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

#if !defined(RENDEREDBEZIER_H_INCLUDED)
#define RENDEREDBEZIER_H_INCLUDED

#include "Bezier.h"
#include "MaterialMgr.h"
#include "TextureMgr.h"

// Direzioni usate per l'unione delle patch
enum Direction
{
	UNKNOWN = -1,
	NORTH   = 0,
	EAST    = 1,
	SOUTH   = 2,
	WEST    = 3
};

class RenderedBezier : public CubicBezierPatch  
{
public:

	// Informazioni su un collegamento di patch
	struct BoundPatch
	{
		RenderedBezier *Patch;	// la patch a cui sono collegata
		Direction Dir;				// la sua direzione di collegamento
		bool c1;						// se il collegamento e' di classe C1
	};

	RenderedBezier(unsigned int id);
	RenderedBezier(unsigned int id, point cpoints[][4]);

	virtual ~RenderedBezier();

	unsigned int getId();
	void setId(unsigned int newId);

	unsigned int getMaterial();
	void setMaterial(unsigned int mat);

	unsigned int getTexture();
	void setTexture(unsigned int tex);
	
	// Disegna la patch utilizzando materiali e texture selezionati
	void DrawPatch();	

	// Settano un punto di controllo applicando i vincoli eventuali.
	void setPoint(unsigned char pos, point p);
   void setPoint(unsigned char i, unsigned char j, point p);

	// Setta il punto senza controllare i binding
   void _setPoint(unsigned char pos, point p);

	// Spostano un punto di controllo applicando i vincoli eventuali.
	void TranslatePoint(unsigned char pos, point p);
	void TranslatePoint(unsigned char i, unsigned char j, point p);

	// Ritorna le coordinate di un punto di controllo
	point getPoint(unsigned char pos);
	point getPoint(unsigned char i, unsigned char j);

	// Ritorna la patch collegata in direzione d
	RenderedBezier *getConnectedPatch(Direction d);

	// Ritorna una struct di informazioni sulla patch collegata in direzione d
	BoundPatch getConnected(Direction d);

	// Collega una nuova patch
	bool Attach(RenderedBezier *other, Direction myDir, Direction otherDir, bool c1);

	// Scollega da una patch
	bool Detach(Direction d);

	// Inverte le normali di questa patch
	void FlipNormals();

	//-------------- LOOK UP TABLES!!! ------------- (very quick!)
	struct PointsInfo
	{	  
		unsigned char Inside;
		unsigned char Border;	  
	};
  
	static const PointsInfo PointsInDir[4][4];

protected:

	// Informazioni relative ad un binding tra punti di controllo
	struct BoundPoint
	{
		RenderedBezier *Patch;		// la patch sulla quale il binding sara' applicato
		unsigned int nPoint;			// il punto sul quale il binding sara' applicato
		bool Inverse;					// se vero il binding e' applicato invertendo i movimenti
		RenderedBezier *Attached;	// questo collegamento e' stato creato a causa di quest' attach
	};
	
	// Informazioni temporanee utilizzate nell' applicazione dei bindings
	struct ActivationRecord
	{
		RenderedBezier *Target;		// patch sulla quale agire
		unsigned char nPoint;		// numero di punto
		point Diff;						// vettore di spostamento
	};

	MaterialMgr *MM;
	TextureMgr *TM;
	unsigned int myId;
	unsigned int material;
	unsigned int texture;

	bool	Binded[16];					// punti legati(blu)
	BoundPatch Connected[4];		// patch collegate (in ogni direzione)
	BoundPoint Binding[4][4][7];	// fino a otto bindings per punto --> [i][j][binding]
	bool BindingsApplied[16];		// serve per marcare la visita BFS

	vector3 *BorderNormals[4];		// le normali delle facce lungo i bordi

	// Aggiunge un nuovo binding
	void AddBinding(unsigned char nodo1, RenderedBezier* other, unsigned char nodo2, bool inv, RenderedBezier* attachment);	

	// Rimuove tutti i bindings su un punto causati dalla patch 'attachment'.
   void RemAllBindings(unsigned char nodo1, RenderedBezier* attachment);

	// Disegna i punti di controllo blu
	void DrawBindings(unsigned char pos, bool on);

	// Applica i vincoli sullo spostamento dei punti di controllo
  	void ApplyBindings(unsigned char i, unsigned char j, point diff);

	// Resetta i nodi marcati durante la visita BFS
	void ClearBindingsApplied(unsigned char i, unsigned char j);

	// Calcola le normali della patch (per illuminazione)
	void ComputeNormals();

	// Calcola le normali delle facce lungo i bordi della patch (per illuminazione)
	void DoNormalBorders();	

private:

	// Inizializza la patch
	void Init();

};

#endif // !defined(RENDEREDBEZIER_H_INCLUDED)
