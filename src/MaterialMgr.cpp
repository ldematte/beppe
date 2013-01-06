/*******************************************************************\
*  Bezier Patch Editor  ---  MaterialMgr                            *
*                                                                   *
*  Il Material Manager gestisce una collezione di materiali. Ai     *
*  materiali viene assegnato un nome ed un numero (ID) che viene    *
*  utilizzato per riferirsi in maniera univoca ad un materiale.     *
\*******************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "MaterialMgr.h"
#include "Utils.h"

// una macro che controlla (in debug) se un matId è valido
#define MAT_ID_OK(n)	assert((n>0 && material[n] != NULL) && "matId non valido!");

// I membri statici
MaterialMgr* MaterialMgr::instance = NULL;

MaterialMgr* MaterialMgr::getInstance()
{
	if (!instance)
		instance = new MaterialMgr();

	return instance;	
}

///////////////////////////////////////////////////////
// Construstor / Destructor

MaterialMgr::MaterialMgr() 
{
	log = Logger::getInstance();
	log->add("MaterialMgr :: inizializzazione.");
	
	numMaterials = 0;

	for (int i = 0; i< MAX_MATERIALS;i++)
		material[i] = NULL;
}

MaterialMgr::~MaterialMgr()
{
	
}

///////////////////////////////////////////////////////
// Ritorna il numero di materiali creati

int MaterialMgr::getNumMaterials()
{
	return numMaterials;
}

///////////////////////////////////////////////////////
// Crea un nuovo materiale.

unsigned int MaterialMgr::addMaterial(const char* name)
{
	assert(name != NULL);

	// devo trovare la prima posizione libera...
	int pos = 1;

	while (material[pos] != NULL)
		pos++;

	numMaterials++;
	material[pos] = new MaterialInfo(name);

	log->addf("MaterialMgr :: nuovo materiale '%s'.", name);
	
	return pos;
}

///////////////////////////////////////////////////////
// Rimuove un materiale.

void MaterialMgr::remMaterial(unsigned int matId)
{
	MAT_ID_OK(matId);
	
	log->addf("MaterialMgr :: rimuovo materiale '%s'.", material[matId]->name);
	delete material[matId]->name;
	material[matId] = NULL;

}

///////////////////////////////////////////////////////
// Rende attivo un materiale. Utilizza le chiamate OpenGl 
// per settare il materiale corrente.

void MaterialMgr::useMaterial(unsigned int matId)
{
	MAT_ID_OK(matId);
	material[matId]->applyAllParameters();	
}

///////////////////////////////////////////////////////
// Setta il nome del materiale

void MaterialMgr::setName(unsigned int matId, const char* txt)
{
	MAT_ID_OK(matId);
	material[matId]->setName(txt);
}

///////////////////////////////////////////////////////
// Questo è il colore "principale" del materiale.

void MaterialMgr::setColor(unsigned int matId, unsigned char r, unsigned char g, unsigned char b)
{
	MAT_ID_OK(matId);
	MaterialInfo *mat = material[matId];
	mat->glDiffuse[0] = r / 255.0f;
	mat->glDiffuse[1] = g / 255.0f;
	mat->glDiffuse[2] = b / 255.0f;

	// devo aggiornare le altre proprietà che utilizzano il colore di diffuse
	setLuminosity(matId, mat->luminosity);
	setEmission(matId, mat->emission);
}

///////////////////////////////////////////////////////
// La luminosità del materiale viene implementata con la 
// luce ambient.

void MaterialMgr::setLuminosity(unsigned int matId, unsigned char lum)
{
	MAT_ID_OK(matId);
	MaterialInfo *mat = material[matId];

	mat->luminosity = lum;

	mat->glAmbient[0] = mat->glDiffuse[0] * lum / 255.0f;
	mat->glAmbient[1] = mat->glDiffuse[1] * lum / 255.0f;
	mat->glAmbient[2] = mat->glDiffuse[2] * lum / 255.0f;
}

///////////////////////////////////////////////////////
// Più è alto questo valore più si nota l'highlight.

void MaterialMgr::setSpecular(unsigned int matId, unsigned char amount)
{
	MAT_ID_OK(matId);
	MaterialInfo *mat = material[matId];

	mat->specular = amount;

	mat->glSpecular[0] = amount / 255.0f;
	mat->glSpecular[1] = amount / 255.0f;
	mat->glSpecular[2] = amount / 255.0f;
	
}

///////////////////////////////////////////////////////
// Specifica quanto è concentrato l'highlight.

void MaterialMgr::setShininess(unsigned int matId, unsigned char amount)
{
	MAT_ID_OK(matId);
	MaterialInfo *mat = material[matId];

	mat->shininess = amount;

	mat->glShininess = amount / 2.0f;			// deve essere nell'intervallo [0.0. 128.0]
}

///////////////////////////////////////////////////////
// Specifica la quantità di luce emessa.

void MaterialMgr::setEmission(unsigned int matId, unsigned char amount)
{
	MAT_ID_OK(matId);
	MaterialInfo *mat = material[matId];

	mat->emission = amount;

	mat->glEmission[0] = mat->glDiffuse[0] * amount / 255.0f;
	mat->glEmission[1] = mat->glDiffuse[1] * amount / 255.0f;
	mat->glEmission[2] = mat->glDiffuse[2] * amount / 255.0f;
}

///////////////////////////////////////////////////////
// Setta la trasparenza del materiale (0 = opaco).

void MaterialMgr::setTrasparency(unsigned int matId, unsigned char amount)
{
	MAT_ID_OK(matId);
	MaterialInfo *mat = material[matId];
	// è implementata con l'alpha del parametro glDiffuse
	
	mat->trasparency = amount;

	mat->glDiffuse[3] = (255-amount) / 255.0f;
}

///////////////////////////////////////////////////////
// Ritorna il nome definito dall'utente del materiale

const char* MaterialMgr::getName(unsigned int matId)
{
	MAT_ID_OK(matId);
	return material[matId]->name;
}

///////////////////////////////////////////////////////
// Ritorna la luminosità del materiale (implementata con la 
// luce ambient)

unsigned char MaterialMgr::getLuminosity(unsigned int matId)
{
	MAT_ID_OK(matId);
	return material[matId]->luminosity;
}

///////////////////////////////////////////////////////
// Ritorna la componente rossa del colore principale

unsigned char MaterialMgr::getColorRed(unsigned int matId)
{
	MAT_ID_OK(matId);
	return (unsigned char)(material[matId]->glDiffuse[0] * 255);
}

///////////////////////////////////////////////////////
// Ritorna la componente verde del colore principale

unsigned char MaterialMgr::getColorGreen(unsigned int matId)
{
	MAT_ID_OK(matId);
	return (unsigned char)(material[matId]->glDiffuse[1] * 255);
}

///////////////////////////////////////////////////////
// Ritorna la componente blu del colore principale

unsigned char MaterialMgr::getColorBlue(unsigned int matId)
{
	MAT_ID_OK(matId);
	return (unsigned char)(material[matId]->glDiffuse[2] * 255);
}

///////////////////////////////////////////////////////
// Ritorna il grado di highlights del materiale

unsigned char MaterialMgr::getSpecular(unsigned int matId)
{
	MAT_ID_OK(matId);
	return material[matId]->specular;
}

///////////////////////////////////////////////////////
// Ritorna la lucentezza del materiale

unsigned char MaterialMgr::getShininess(unsigned int matId)
{
	MAT_ID_OK(matId);
	return material[matId]->shininess;
}

///////////////////////////////////////////////////////
// Ritorna la quantità di luce emessa.

unsigned char MaterialMgr::getEmission(unsigned int matId)
{
	MAT_ID_OK(matId);
	return material[matId]->emission;
}

///////////////////////////////////////////////////////
// Ritorna la trasparenza del materiale (0 = opaco).

unsigned char MaterialMgr::getTrasparency(unsigned int matId)
{
	MAT_ID_OK(matId);
	return 255 - (unsigned char)(material[matId]->glDiffuse[3] * 255);
}

///////////////////////////////////////////////////////
// Ritorna l'ID del materiale a partire dal nome definito 
// dall'utente 

unsigned char MaterialMgr::findMaterialByName(const char *name)
{
	assert (name != NULL);
	unsigned int i = 1;
	unsigned int controllati = 0;

	while (controllati < numMaterials)
	{			
		if (material[i] && strcmp(material[i]->name, name) == 0)
			return i;
		else
			controllati++;
		i++;
	}
	return 0;
}

///////////////////////////////////////////////////////
// Salva il materiale su file

void MaterialMgr::save(FILE* f, unsigned int matId)
{
  MAT_ID_OK(matId);
  const char* matName;
  if ((matName = getName(matId)) && (matName[0] != '#'))
  {
    fprintf(f, "\r\n#Material\r\n");

    fprintf(f, "Name=%s\r\n", getName(matId));
    fprintf(f, "ColoreR=%i\r\n", getColorRed(matId));
    fprintf(f, "ColoreG=%i\r\n", getColorGreen(matId));
    fprintf(f, "ColoreB=%i\r\n", getColorBlue(matId));
    fprintf(f, "Specular=%i\r\n", getSpecular(matId));
    fprintf(f, "Shininess=%i\r\n", getShininess(matId));
    fprintf(f, "Luminosity=%i\r\n", getLuminosity(matId));
    fprintf(f, "Emission=%i\r\n", getEmission(matId));
    fprintf(f, "Trasparency=%i\r\n", getTrasparency(matId));
  }
}

///////////////////////////////////////////////////////
// Carica il materiale da file

unsigned int MaterialMgr::load(FILE* f)
{
  char buffer[512];
  char* retval = NULL;
  unsigned int matId =  INVALID_MATERIAL;

  retval = fgets(buffer, sizeof(buffer), f);
  char* name;
  if ( (name = FindProperty(retval, "Name")) != NULL)
  {
    matId = addMaterial(name);

    retval = fgets(buffer, sizeof(buffer), f);
    char* coloreR;
    int ColoreR;
    if ( (coloreR = FindProperty(retval, "ColoreR")) == NULL)
    {
      remMaterial(matId);
      return INVALID_MATERIAL;
    }
    ColoreR = atoi(coloreR);

    retval = fgets(buffer, sizeof(buffer), f);
    char* coloreG;
    int ColoreG;
    if ( (coloreG = FindProperty(retval, "ColoreG")) == NULL)
    {
      remMaterial(matId);
      return INVALID_MATERIAL;
    }
    ColoreG = atoi(coloreG);

     
    retval = fgets(buffer, sizeof(buffer), f);
    char* coloreB;
    int ColoreB;
    if ( (coloreB = FindProperty(retval, "ColoreB")) == NULL)  
    {
      remMaterial(matId);
      return INVALID_MATERIAL;
    }
    ColoreB = atoi(coloreB);

    setColor(matId, ColoreR, ColoreG, ColoreB);

    //Specular
    retval = fgets(buffer, sizeof(buffer), f);
    char* szSpecular;
    int nSpecular;
    if ( (szSpecular = FindProperty(retval, "Specular")) == NULL)
    {
      remMaterial(matId);
      return INVALID_MATERIAL;
    }
    nSpecular = atoi(szSpecular);

    setSpecular(matId, nSpecular);
    //Specular


    //Shininess
    retval = fgets(buffer, sizeof(buffer), f);
    char* szShininess;
    int nShininess;
    if ( (szShininess = FindProperty(retval, "Shininess")) == NULL)
    {
      remMaterial(matId);
      return INVALID_MATERIAL;
    }
    nShininess = atoi(szShininess);

    setShininess(matId, nShininess);
    //Shininess


    //Luminosity
    retval = fgets(buffer, sizeof(buffer), f);
    char* szLuminosity;
    int nLuminosity;
    if ( (szLuminosity = FindProperty(retval, "Luminosity")) == NULL)
    {
      remMaterial(matId);
      return INVALID_MATERIAL;
    }
    nLuminosity = atoi(szLuminosity);

    setLuminosity(matId, nLuminosity);
    //Luminosity


    //Emission
    retval = fgets(buffer, sizeof(buffer), f);
    char* szEmission;
    int nEmission;
    if ( (szEmission = FindProperty(retval, "Emission")) == NULL)
    {
      remMaterial(matId);
      return INVALID_MATERIAL;
    }
    nEmission = atoi(szEmission);
    setEmission(matId, nEmission);
    //Emission


    //Trasparency
    retval = fgets(buffer, sizeof(buffer), f);
    char* szTrasparency;
    int nTrasparency;
    if ( (szTrasparency = FindProperty(retval, "Trasparency")) == NULL)
    {
      remMaterial(matId);
      return INVALID_MATERIAL;
    }
    nTrasparency = atoi(szTrasparency);

    setTrasparency(matId, nTrasparency);
    //Trasparency    
  }

  return matId;
}


