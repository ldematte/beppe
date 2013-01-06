/*******************************************************************\
*  Bezier Patch Editor  ---  Logger                                 *
*                                                                   *
*  Il Log Manager serve principalmente per effettuare debug.        *
*  Registra i messaggi inviati con le funzioni add(..) ed addf(..)  *
*  in un file e li visualizza in una finestra apposita (LogWindow)  *
\*******************************************************************/

#include <assert.h>
#include <stdarg.h>
#include <time.h>

#include "Logger.h"

/////////////////////////////////////////////////////////////
// Inizializzazione membri statici

Logger* Logger::Instance = NULL;

/////////////////////////////////////////////////////////////
// Construction / destruction

Logger::Logger()
{
	logWin = NULL;
	out = fopen("log.txt", "wa");
	assert(out);

	add("-------------------------------------------");
}

Logger::~Logger()
{
	fclose(out);
	Instance = NULL;
}

/////////////////////////////////////////////////////////////
// Aggancia il logger a una finestra per la visualizzazione
// in tempo reale	

void Logger::setLogWindow(LogWindow *l)
{
	logWin = l;
	time_t now;
	time(&now);

	addf("%s",ctime(&now));
	add("Logger :: inizializzato.");
}

/////////////////////////////////////////////////////////////
// Aggiunge una stringa al log

void Logger::add(const char *txt)
{
	fprintf(out,"%s\n",txt);
	fflush(out);

	if (logWin)
	{
		logWin->Append(txt);
		logWin->Append("\n");
	}

}

/////////////////////////////////////////////////////////////
// Aggiunge una stringa formattata alla printf al log

void Logger::addf(const char *format, ...)
{
	char buf[1024];

	va_list ap;
	va_start(ap, format);
	vsprintf( buf, format, ap); 
	//uvszprintf(buf, sizeof(buf), format, ap);
	va_end(ap);
	
	add(buf);
}
	

