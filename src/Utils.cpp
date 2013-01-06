/*******************************************************************\
*  Bezier Patch Editor  ---  Utils                                  *
*                                                                   *
*  Alcune funzioni di utilita' generiche.                           *
\*******************************************************************/

#include <string.h>
#include <math.h>
#include "Logger.h"

char* FindProperty(char* source, const char* prop)
{
  if (source == NULL || prop == NULL)
    return NULL;

  char* pPos = strchr(source, '=');
  
  if (pPos != NULL)
  {	
    int iPos = pPos - source;
    if(strncmp(source, prop, strlen(prop)) == 0)
    {
      if ((source[strlen(source) - 2] == '\n') ||
          (source[strlen(source) - 2] == '\r') )
      {
        source[strlen(source) - 2] = '\0';
      }
      
      if ((source[strlen(source) - 1] == '\n') ||
          (source[strlen(source) - 1] == '\r') )
      {
        source[strlen(source) - 1] = '\0';
      }
        
      return (source + iPos + 1);
    }
  }

  return NULL;
}

void RoundTo(float val, float step , float &integer, float &fractx)
{
	double intx = 0;
	fractx = (float)modf(val / step, &intx);
      
   if (fractx > 0.5f)
   {
     intx += 1.0f;
     fractx -= 1.0f;
   }
   else if (fractx < -0.5f)
   {
     intx -= 1.0f;
     fractx += 1.0f;
   }     

   integer = (float)(intx * step);
   fractx *= step;	
}