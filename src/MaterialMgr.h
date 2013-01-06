/*******************************************************************\
*  Bezier Patch Editor  ---  MaterialMgr                            *
*                                                                   *
*  Il Material Manager gestisce una collezione di materiali. Ai     *
*  materiali viene assegnato un nome ed un numero (ID) che viene    *
*  utilizzato per riferirsi in maniera univoca ad un materiale.     *
\*******************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_MATERIALMGR_3CC9956103B6_INCLUDED
#define _INC_MATERIALMGR_3CC9956103B6_INCLUDED

#include "MaterialInfo.h"
#include "Logger.h"

#define	MAX_MATERIALS				256
#define  INVALID_MATERIAL          0

//
// <<SINGLETON>>
// Il Material Manager si occupa di gestire informazioni su 
// una collezione di materiali. L'utente puo' attivare un 
// materiale con la funzione useMaterial().
//
// I materiali vengono gestiti in un modo piu' sintetico
// di quello nativo OpenGL. Invece di usare un colore per 
// ogni caratteristica del materiale, viene scelto un colore 
// principale e poi gli altri vengono formati a partire da 
// questo modulandoli con dei parametri passati dall'utente
//

class MaterialMgr 
{
public:
	
	// Ritorna l'istanza del singleton.
	static MaterialMgr* getInstance();

	virtual ~MaterialMgr();

  // Ritorna il numero di materiali creati
	int getNumMaterials();

	// Crea un nuovo materiale.
	unsigned int addMaterial(const char* name);

	// Rimuove un materiale.
	void remMaterial(unsigned int matId);

	// Rende attivo un materiale. Utilizza le chiamate OpenGl 
	// per settare il materiale corrente.
	void useMaterial(unsigned int matId);

	// La luminosità del materiale viene implementata con la 
	// luce ambient.
	void setLuminosity(unsigned int matId, unsigned char lum);

	// Questo è il colore "principale" del materiale.
	void setColor(unsigned int matId, unsigned char r, unsigned char g, unsigned char b);

	// Più è alto questo valore più si nota l'highlight.
	void setSpecular(unsigned int matId, unsigned char amount);

	// Specifica quanto è concentrato l'highlight.
	void setShininess(unsigned int matId, unsigned char amount);

	// Specifica la quantità di luce emessa.
	void setEmission(unsigned int matId, unsigned char amount);

	// Setta la trasparenza del materiale (0 = opaco).
	void setTrasparency(unsigned int matId, unsigned char amount);

  // Setta il nome del materiale
	void setName(unsigned int matId, const char* txt);

  // Ritorna il nome definito dall'utente del materiale
	const char*	getName(unsigned int matId);

  // Ritorna l'ID del materiale a partire dal nome definito 
  // dall'utente 
	unsigned char findMaterialByName(const char *name);

  // Ritorna la luminosità del materiale (implementata con la 
	// luce ambient)
	unsigned char getLuminosity	(unsigned int matId);

  // Ritorna la componente rossa del colore principale
	unsigned char getColorRed		(unsigned int matId);

  // Ritorna la componente verde del colore principale
	unsigned char getColorGreen	(unsigned int matId);

  // Ritorna la componente blu del colore principale
	unsigned char getColorBlue		(unsigned int matId);

  // Ritorna il grado di highlights del materiale
	unsigned char getSpecular		(unsigned int matId);

  // Ritorna la lucentezza del materiale
	unsigned char getShininess		(unsigned int matId);

  // Ritorna la quantità di luce emessa.
	unsigned char getEmission		(unsigned int matId);

  // Ritorna la trasparenza del materiale (0 = opaco).
	unsigned char getTrasparency	(unsigned int matId);

  // Salva il materiale su file
  void save(FILE* f, unsigned int matId);

  // Carica il materiale da file
  unsigned int load(FILE* f);

private:
	
	MaterialMgr();

	// L'istanza singleton.
	static MaterialMgr *instance;

	// Il numero di materiali inseriti nel MaterialMgr.
	unsigned int numMaterials;

  // C'e' sempre bisogno di un logger...
	Logger* log;

  // Il nostro buffer privato
	MaterialInfo* material[MAX_MATERIALS];

};

#endif /* _INC_MATERIALMGR_3CC9956103B6_INCLUDED */

