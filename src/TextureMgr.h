/*******************************************************************\
*  Bezier Patch Editor  ---  TextureMgr                             *
*                                                                   *
*  Questo manager raccoglie un insieme di texture con vari          *
*  parametri. Le texture vengono caricate e scaricate dalla memoria *
*  dell'acceleratore 3d automaticamente, utilizzando un             *
*  algoritmo LRU (Last Recently Used). Le texture sono identificate *
*  tramite degli Id.                                                *
\*******************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_TEXTUREMGR_3CB5C6E302EE_INCLUDED
#define _INC_TEXTUREMGR_3CB5C6E302EE_INCLUDED

#include "zList.h"
#include "Logger.h"
#include "TexResource.h"
#include "Image.h"

#define	MAX_RESOURCES			512
#define  INVALID_TEXTURE        0

class TextureMgr 
{
public:
	virtual ~TextureMgr();

	static TextureMgr* getInstance();

	//Setta la dimensione della cache.
	void setCacheSize(unsigned int size);

	//Ritorna la dimensione della cache.
	unsigned int getCacheSize();

	//Ritorna la quantità di spazio libero in cache.
	unsigned int getFreeSpace();

	//Ritorna la quantità di spazio usato in cache.
	unsigned int getUsedSpace();

	//Avverte il TextureManager della presenza di una nuova 
	//risorsa specificata dal file name (il file di una 
	//texture). Il file non viene caricato, ma il 
	//TextureManager ritorna un intero (id univoco per questa 
	//risorsa) che serve per tutte le successive chiamate 
	//alle funzioni tex*.
	int addResource(const char* texFileName, const char* userName);

	// elimina una risorsa dal texture manager 
	// (e scarica la texture se presente);
	void remResource(unsigned int texId);

	//Scarica completamente la cache (polverizza anche le 
	//texture lockate) e libera tutte le risorse conosciute 
	//dal TextureManager. Tutti i texId in uso non sono più 
	//validi. Insomma un mega-reset.
	void reset();

	//Scarica completamente la cache ma non distrugge 
	//l'elenco delle risorse conosciute. Tutti i texId sono 
	//ancora validi.
	void unloadAll();

	//Ritorna il numero di risorse conosciute dal Texture 
	//Manager. Per fini statistici.
	unsigned int getNumResources();

	//Ritorna il numero di texture attualmente in cache. Per 
	//fini statistici.
	unsigned int getNumCached();

	//Ritorna il filename di una risorsa.
	const char *getFileName(unsigned int texId);

	//Ritorna il nome utente di una risorsa.
	const char *getUserName(unsigned int texId);

	//Cambia il nome utente di una risorsa.
	void setUserName(unsigned int texId, const char *username);

	unsigned int findTextureByUserName(const char *username);

	//setta un opzione (glTexParameter) per una texture. Se 
	//la texure si trova in cache l'opzione viene settata 
	//istantaneamente, altrimenti verrà settata la prima 
	//volta che si carica la texture.
	void texOption(unsigned int texId, int optName, int optValue);

	//ritorna il valore di un opzione
	int texGetOption(unsigned int texId, int optName);

	//Carica in cache la nuova texture (eliminando 
	//eventualmente altre textures in cache). Se non è 
	//possibile fare spazio per la nuova texture, viene 
	//caricata una texture dummy.
	bool texLoad(unsigned int texId);

	//Scavalca l'algoritmo LRU ed obbliga il TextureManager a 
	//scaricare la texture specificata. NOTA: la texture 
	//viene scaricata anche se lockata.
	void texUnload(unsigned int texId);

	//Se la texture non è in cache la carica e poi la usa 
	//effettuando il glBindTexture.
	void texUse(unsigned int texId);

	//Impedisce alla texture di essere scaricata dalla cache. 
	//NOTA: se la texture non era presente in cache, essa 
	//viene caricata.
	void texLock(unsigned int texId);

	//Sblocca una texture, che potrà essere scaricata dalla 
	//cache se poco usata.
	void texUnlock(unsigned int texId);

	//Disattiva la texture
	void texOff();

	//Salva le informazioni della texture su file
  void save(FILE* f, unsigned int texId);

  //Carica le informazioni della texture da file
  unsigned int load(FILE* f);

  //Fa una texture vuota di dimensione Dimension * Dimension
  unsigned int EmptyTexture(int Dimension);										



private:
		
	//Dimensione massima della cache (in bytes).
	unsigned int cacheSize;

	//Memoria utilizzata (in bytes).
	unsigned int usedSpace;

	//L'istanza singleton del Texture Manger.
	static TextureMgr* instance;

	Logger *log;

	//Il numero di risorse conosciute dal TextureManager.
	unsigned int numResources;

	//Il numero di texture presenti in cache.
	unsigned int numCached;

	//Texture usata da opengl (zero = nessuna)
	GLuint glTexBound;

	TexResource *resource[MAX_RESOURCES];
	zList<TexCached> *cached;

	TextureMgr();

	bool isCached(unsigned int texId);

	//Calcola quanto spazio occuperà la texture.
	unsigned int guessTextureSize(TexResource *res, Image *img);

	//Scarica texture dalla cache finchè non viene liberato 
	//questo spazio Le vittime vengono scelte con l'algoritmo 
	//LRU.
	bool freeBytes(unsigned int bytes);

};

#endif /* _INC_TEXTUREMGR_3CB5C6E302EE_INCLUDED */

