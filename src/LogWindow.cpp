/*******************************************************************\
*  Bezier Patch Editor  ---  LogWindow                              *
*                                                                   *
*  Una finestra di testo scorrevole per mostrare messaggi di debug. *
\*******************************************************************/

#include "LogWindow.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

// Object implementation
FXIMPLEMENT(LogWindow, FXDialogBox, NULL, 0);

LogWindow::LogWindow(FXMainWindow* owner) : FXDialogBox(owner, "Log Window", DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE,0,0,500,250,0,0,0,0)
{
	FXPacker *frame = new FXPacker(this, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN);
	Scroll = new FXScrollWindow(frame,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
	TextArea = new FXLabel(Scroll, NULL, 0, LAYOUT_SIDE_TOP|LAYOUT_SIDE_LEFT|JUSTIFY_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
	TextArea->setFont(new FXFont(getApp(),"courier",6,FONTWEIGHT_REGULAR, 
                                 FONTSLANT_REGULAR, 
                                 FONTENCODING_DEFAULT, 
                                 FONTPITCH_FIXED));
		
}

LogWindow::~LogWindow()
{

}

//////////////////////////////////////////////////////////////////////
// Aggiunge una riga di testo alla finestra

void LogWindow::Append(const char *txt)
{
	TextArea->setText(TextArea->getText().append(txt));
	Scroll->moveContents(0,-Scroll->getContentHeight()*15);

  /* Se usiamo una textarea al posto della FXLabel
	TextArea->appendText(txt,strlen(txt),false);
	TextArea->makePositionVisible(TextArea->getBottomLine());
	*/
}