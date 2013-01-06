/*******************************************************************\
*  Bezier Patch Editor  ---  LogWindow                              *
*                                                                   *
*  Una finestra di testo scorrevole per mostrare messaggi di debug. *
\*******************************************************************/

#if !defined(LOGWINDOW_H_INCLUDED)
#define LOGWINDOW_H_INCLUDED

#include <fx.h>

class LogWindow : public FXDialogBox  
{
	FXDECLARE(LogWindow)
public:

  // Constructor / destructor
	LogWindow(FXMainWindow* owner);
	virtual ~LogWindow();

  // Aggiunge una riga di testo alla finestra
	void Append(const char *txt);

protected:

  // Serve alla gui
	LogWindow() {}		

	FXScrollWindow	*Scroll;
	FXLabel *TextArea;
};

#endif // !defined(LOGWINDOW_H_INCLUDED)
