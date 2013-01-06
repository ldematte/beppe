/*******************************************************************\
*  Bezier Patch Editor  ---  Logger                                 *
*                                                                   *
*  Il Log Manager serve principalmente per effettuare debug.        *
*  Registra i messaggi inviati con le funzioni add(..) ed addf(..)  *
*  in un file e li visualizza in una finestra apposita (LogWindow)  *
\*******************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_LOGGER_3CB6F27A038E_INCLUDED
#define _INC_LOGGER_3CB6F27A038E_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "LogWindow.h"	// per la finestra di log

class Logger
{
public:

  // Construction / destruction
  // Il Logger e' un singleton
	virtual ~Logger();
	static inline Logger* Logger::getInstance()
	{
		if (!Instance)	Instance = new Logger();
		return Instance;
	}

  // Aggancia il logger a una finestra per la visualizzazione
  // in tempo reale
	void setLogWindow(LogWindow *l);

  // Aggiunge una stringa al log
	void add(const char *txt);

  // Aggiunge una stringa formattata alla printf al log
	void addf(const char *format, ...);
	
private:

	// L'istanza singleton del Logger.
	static Logger* Instance;

	LogWindow *logWin;
	FILE *out;

  // Il Logger e' un singleton, non permettiamo la creazione
  // diretta di istanze
	Logger();

};


#endif /* _INC_LOGGER_3CB6F27A038E_INCLUDED */

