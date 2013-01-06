/*******************************************************************\
*  Bezier Patch Editor  ---  TextureMgr                             *
*                                                                   *
*  Questo manager raccoglie un insieme di texture con vari          *
*  parametri. Le texture vengono caricate e scaricate dalla memoria *
*  dell'acceleratore 3d automaticamente, utilizzando un             *
*  algoritmo LRU (Last Recently Used). Le texture sono identificate *
*  tramite degli Id.                                                *
\*******************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "TextureMgr.h"
#include "TexResource.h"
#include "ImageTGA.h"
#include "Utils.h"

// una macro che controlla (in debug) se un texId è valido
#define TEX_ID_OK(n)	assert((n>0 && n<=numResources) && "texId non valido!");

TextureMgr* TextureMgr::instance = NULL;

TextureMgr* TextureMgr::getInstance()
{
	if (!instance)
		instance = new TextureMgr();
	
	return instance;
}

TextureMgr::TextureMgr()
{
	log = Logger::getInstance();
	log->add("TextureMgr :: inizializzazione.");

	glTexBound = 0;
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

	cached = NULL;
	cacheSize = 4194304;		// 4 MB
	//cacheSize = 500000;
	usedSpace = 0;
	reset();
}

TextureMgr::~TextureMgr()
{
	instance = NULL;
	unloadAll();
}

void TextureMgr::reset()
{
	int i;
	numResources = 0;
	numCached = 0;

	// pulisco il vettore delle risorse
	for (i=0;i<MAX_RESOURCES;i++)
		resource[i] = NULL;	

	// creo la lista delle texture in cache
	cached = new zList<TexCached>();
}

int TextureMgr::addResource(const char* texFileName, const char* userName)
{
	// devo trovare la prima posizione libera...
	int pos = 1;

	while (resource[pos] != NULL)
	{
		pos++;
		if (pos >= MAX_RESOURCES)	// non abbiamo spazio
			return 0;
	}

	numResources++;
	
	resource[pos] = new TexResource(texFileName, userName);
	log->addf("TextureMgr :: aggiunta risorsa '%s' : %s.", texFileName, userName);

	return pos;
}


void TextureMgr::remResource(unsigned int texId)
{
	TEX_ID_OK(texId);
	log->addf("TextureMgr :: cancellata risorsa '%s'.", resource[texId]->name);
	resource[texId]->locked = false;
	texUnload(texId);

	delete resource[texId];
	resource[texId] = NULL;
}

void TextureMgr::unloadAll()
{	
	log->add("TextureMgr :: svuoto la cache delle textures...");

	unsigned int scaricate = 0;
	unsigned int i = 1;

	while (scaricate < numResources)
	{
		if (resource[i])
		{
			++scaricate;
			resource[i]->locked = false;
			texUnload(i);
		}
		++i;
	}	
	
}

const char *TextureMgr::getFileName(unsigned int texId)
{
	TEX_ID_OK(texId);
	return resource[texId]->filename;
}

const char *TextureMgr::getUserName(unsigned int texId)
{
	TEX_ID_OK(texId);
	return resource[texId]->name;
}

void TextureMgr::setUserName(unsigned int texId, const char *username)
{
	TEX_ID_OK(texId);
	resource[texId]->setName(username);
}

unsigned int TextureMgr::findTextureByUserName(const char *username)
{
	assert (username != NULL);
	unsigned int i = 1;
	unsigned int controllati = 0;

	while (controllati < numResources)
	{			
		if (resource[i] && strcmp(resource[i]->name, username) == 0)
			return i;
		else
			controllati++;
		i++;
	}
	return 0;
}

bool TextureMgr::texLoad(unsigned int texId)
{
	TEX_ID_OK(texId);
	
	bool loadOk = true;

	if (isCached(texId))		// se era già in cache non faccio niente
		return true;

	TexResource *res = resource[texId];
	
	// carico l'immagine in ram	
	Image *img = new ImageTGA();
	if(!img->load(res->filename))
	{
		loadOk = false;
		log->addf("TextureMgr :: impossibile caricare '%s'! Uso dummy texture.", res->name);
		delete img;
		img = Image::makeDummy();
	}
	else
		log->addf("TextureMgr :: loading '%s'...", res->filename);
	
	unsigned long texSize = guessTextureSize(res, img);
	

	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// verifica spazio libero

	if (usedSpace + texSize > cacheSize)	// devo liberare spazio!
	{		
		log->addf("TextureMgr :: devo avere almeno %d bytes liberi...", texSize);
		if(!freeBytes(texSize))
		{
			// nooooooo! pur avendo liberato l'intera cache non
			// ho spazio per il nuovo arrivato!
			log->add("TextureMgr :: ATTENZIONE! non c'e' abbastanza spazio per la texture!");
			delete img;
			img = Image::makeDummy();
		}
	}

	
	usedSpace += texSize;

	//----------------------------- TEXTURE OPENGL ----------------------------
	// creo la texture OpenGl
	GLuint newTex;
	glGenTextures(1, &newTex);
	glBindTexture(GL_TEXTURE_2D, newTex);

	// applico tutte le opzioni OpenGL della texture
	res->applyAllOptions();
	
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	if (res->options[TEX_MIPMAPPED] == 0)		// tex normale
	{
		glTexImage2D(GL_TEXTURE_2D, 0, res->options[TEX_COLOR_MODE], img->getWidth(), img->getHeight(),
					0, GL_RGBA, GL_UNSIGNED_BYTE, img->getPixels());
	}
	else
	{
		// uso glu mip
		gluBuild2DMipmaps(GL_TEXTURE_2D, res->options[TEX_COLOR_MODE], img->getWidth(), img->getHeight(), 
			GL_RGBA, GL_UNSIGNED_BYTE, img->getPixels());
	}
	
	// elimino la fonte originale (ormai la tex è uploadata)
	delete img;

	// ricollego la texture che c'era prima
	glBindTexture(GL_TEXTURE_2D, glTexBound);

	//------------------------- INSERISCO NELLE STRUTTURE DATI --------------------------------

	if (!newTex)
	{
		log->addf("TextureMgr :: impossibile creare la texture '%s'...", resource[texId]);
		return false;
	}
	
	TexCached *tc = new TexCached(texId);
	tc->glTexName = newTex;
	tc->size = texSize;

	zList<TexCached>::zNode *node = cached->insertBack(tc);
	resource[texId]->toCache = node;

	log->addf("TextureMgr :: usedSpace = %d...", usedSpace);
	return loadOk;
}

void TextureMgr::texUnload(unsigned int texId)
{
	TEX_ID_OK(texId);
	if (!isCached(texId))	// se non è in cache non faccio niente
		return;

	log->addf("TextureMgr :: freeing '%s'...", resource[texId]->name);

	zList<TexCached>::zNode *node = resource[texId]->toCache;
	
	if (glTexBound == node->Elem->glTexName)
	{
		glTexBound = 0;	// scolleghiamo la texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// incremento lo spazio libero
	usedSpace -= node->Elem->size;

	glDeleteTextures(1, &(node->Elem->glTexName));	// elimino la texture OpenGL
	delete node->Elem;										// elimino l'oggetto TexCached
	node->remove();											// elimino il nodo della lista

	resource[texId]->toCache = NULL;
	// unlock della texture
	resource[texId]->locked = false;

	log->addf("TextureMgr :: usedSpace = %d...", usedSpace);
}

void TextureMgr::texUse(unsigned int texId)
{
	TEX_ID_OK(texId);
	if (isCached(texId) == false)		// se non era in cache
		texLoad(texId);					// la carico

	zList<TexCached>::zNode *node = resource[texId]->toCache;
	node->moveToBack() ; // perchè è stato appena usato --> il più giovane

	glTexBound = node->Elem->glTexName;
	glBindTexture(GL_TEXTURE_2D, glTexBound);
	
	// devo riapplicare quest'opzione perchè ha un effetto globale
	// e non viene collegata automaticamente alla texture.
	resource[texId]->applyOption(TEX_ENV_MODE);
}

void TextureMgr::texLock(unsigned int texId)
{
	TEX_ID_OK(texId);

	if (isCached(texId) == false)		// se non era in cache
		texLoad(texId);					// la carico
	
	log->addf("TextureMgr :: lock '%s'.", resource[texId]->name);

	resource[texId]->locked = true;
}

void TextureMgr::texUnlock(unsigned int texId)
{
	TEX_ID_OK(texId);

	log->addf("TextureMgr :: unlock '%s'.", resource[texId]->name);

	resource[texId]->locked = false;
}


void TextureMgr::texOption(unsigned int texId, int optName, int optValue)
{
	TEX_ID_OK(texId);

	resource[texId]->setOption(optName, optValue);
	
	// ricollego la texture che c'era prima
	glBindTexture(GL_TEXTURE_2D, glTexBound);
}


int TextureMgr::texGetOption(unsigned int texId, int optName)
{
	TEX_ID_OK(texId);
	return resource[texId]->options[optName];
}

void TextureMgr::texOff()
{
	glTexBound = 0;
	glBindTexture(GL_TEXTURE_2D, glTexBound);
}

unsigned int TextureMgr::getNumResources()
{
	return numResources;
}

unsigned int TextureMgr::getNumCached()
{
	return numCached;
}

void TextureMgr::setCacheSize(unsigned int size)
{
	cacheSize = size;	
}

unsigned int TextureMgr::getCacheSize()
{
	return cacheSize;
}

unsigned int TextureMgr::getUsedSpace()
{
	return usedSpace;
}

unsigned int TextureMgr::getFreeSpace()
{
	return (cacheSize - usedSpace);
}

void TextureMgr::save(FILE* f, unsigned int texId)
{
	TEX_ID_OK(texId);
	const char* texName;
	if ((texName = getUserName(texId)) && (texName[0] != '#'))
  {
    int val;
    fprintf(f, "\r\n#Texture\r\n");
    fprintf(f, "UserName=%s\r\n", getUserName(texId));
	 fprintf(f, "FileName=%s\r\n", getFileName(texId));
    val = (texGetOption(texId, TEX_WRAP_S_MODE) == WRAP_REPEAT) ? 1 : 2;
	 fprintf(f, "WrapS=%d\r\n", val);
    
    val = (texGetOption(texId, TEX_WRAP_T_MODE) == WRAP_REPEAT) ? 1 : 2;
	 fprintf(f, "WrapT=%d\r\n", val);
    
    switch (texGetOption(texId, TEX_ENV_MODE))
    {
    case ENV_REPLACE:  val = 1;break;
    case ENV_DECAL:    val = 2;break;
    case ENV_MODULATE: val = 3;break;
    }
	 fprintf(f, "Mapping=%d\r\n", val);
    
    switch (texGetOption(texId, TEX_QUALITY))
    {
    case QUALITY_NEAREST:   val = 1;break;
    case QUALITY_LINEAR:    val = 2;break;
    case QUALITY_BILINEAR:  val = 3;break;
    case QUALITY_TRILINEAR: val = 4;break;
    }
	 fprintf(f, "Quality=%d\r\n", val);
    
	 fprintf(f, "Mipmapped=%d\r\n", texGetOption(texId, TEX_MIPMAPPED));
  }
}

unsigned int TextureMgr::load(FILE* f)
{
	char buffer[512];
	char* retval = NULL;
	unsigned int texId = INVALID_TEXTURE;

	retval = fgets(buffer, sizeof(buffer), f);
	char userName[128];
	char* fileName;
	if ( (retval = FindProperty(retval, "UserName")) != NULL)
	{
		strcpy(userName, retval);
		retval = fgets(buffer, sizeof(buffer), f);
		if ( (fileName = FindProperty(retval, "FileName")) != NULL)
			texId = addResource(fileName, userName);
		else
			return INVALID_TEXTURE;
	}

	

	fgets(buffer, sizeof(buffer), f);
	if ( (retval = FindProperty(buffer, "WrapS")) != NULL)
   	texOption(texId, TEX_WRAP_S_MODE, (atoi(retval) == 1) ? WRAP_REPEAT : WRAP_CLAMP);
	else return INVALID_TEXTURE;

	fgets(buffer, sizeof(buffer), f);
	if ( (retval = FindProperty(buffer, "WrapT")) != NULL)
		texOption(texId, TEX_WRAP_T_MODE, (atoi(retval) == 1) ? WRAP_REPEAT : WRAP_CLAMP);
	else return INVALID_TEXTURE;

	fgets(buffer, sizeof(buffer), f);
	if ( (retval = FindProperty(buffer, "Mapping")) != NULL)
	{
		unsigned int env;
		switch(atoi(retval))
		{
		case 1: env = ENV_REPLACE; break;
		case 2: env = ENV_DECAL; break;
		case 3: env = ENV_MODULATE; break;
		default:
			return INVALID_TEXTURE;
		}
      texOption(texId, TEX_ENV_MODE, env);
	}
	else return INVALID_TEXTURE;

	fgets(buffer, sizeof(buffer), f);
	if ( (retval = FindProperty(buffer, "Quality")) != NULL)
	{
		unsigned int q;
		switch(atoi(retval))
		{      
		case 1: q = QUALITY_NEAREST; break;
		case 2: q = QUALITY_LINEAR; break;
		case 3: q = QUALITY_BILINEAR; break;
		case 4: q = QUALITY_TRILINEAR; break;
		default:
			return INVALID_TEXTURE;
		}
		texOption(texId, TEX_QUALITY, q);
	}
	else return INVALID_TEXTURE;

	fgets(buffer, sizeof(buffer), f);
	if ( (retval = FindProperty(buffer, "Mipmapped")) != NULL)
		texOption(texId, TEX_MIPMAPPED, (atoi(retval) != 0) ? 1 : 0);
	else return INVALID_TEXTURE;

	return texId;
}
//------------------------- FUNZIONI PRIVATE ----------------------------

inline bool TextureMgr::isCached(unsigned int texId)
{
	TEX_ID_OK(texId);
	return (resource[texId]->toCache != NULL);
}

unsigned int TextureMgr::guessTextureSize(TexResource *res, Image *img)
{
	assert(res != NULL);
	assert(img != NULL);

	unsigned int size;
	// calcolo quanti pixel contiene l'immagine
	size = img->getWidth() * img->getHeight();

	// caricata con i mipmap??
	if (res->options[TEX_MIPMAPPED] != 0)
	{
		// devo aggiungere l'overhead dei mipmap
		size = (unsigned int)(size * 1.333333f);
	}

	// RGB o RGBA ??
	if (res->options[TEX_COLOR_MODE] == COL_RGBA)
		size *= 4;	// 4 bytes per pixel
	else if (res->options[TEX_COLOR_MODE] == COL_RGB)
		size *= 3;	// 3 bytes per pixel

	return size;
}

bool TextureMgr::freeBytes(unsigned int bytes)
{
	// devo liberare questi bytes
	// algoritmo LRU : il primo della lista e' il piu vecchio

	unsigned long freeSpace = cacheSize - usedSpace;
	zList<TexCached>::zNode *vittimaNode = cached->begin();
	unsigned int vittimaId; 

	while((freeSpace < bytes) && !cached->isEmpty() && !vittimaNode->isEnd())
	{		
		vittimaId = vittimaNode->Elem->texId;
		if (resource[vittimaId]->locked == false)	
		{
			// devo scaricarla dalla cache
			texUnload(vittimaId);
			freeSpace = cacheSize - usedSpace;
			vittimaNode = cached->begin();			
		}
		else
		{				
			// proviamo la tex successiva
			vittimaNode = vittimaNode->Next;
		}
	}

	if ((cacheSize - usedSpace) < bytes)
		return false;
	else
		return true;
}

// Create An Empty Texture
unsigned int TextureMgr::EmptyTexture (int Dimension = 128)											
{
	GLuint txtnumber;										
	unsigned int* data;

	data = (unsigned int*)new GLuint[((Dimension * Dimension)* 4 * sizeof(unsigned int))];
	memset(data, 0, ((Dimension * Dimension)* 4 * sizeof(unsigned int)));	

	glGenTextures(1, &txtnumber);								
	glBindTexture(GL_TEXTURE_2D, txtnumber);				
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);						
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	delete [] data;												// Release data

	return txtnumber;											// Return The Texture ID
}





