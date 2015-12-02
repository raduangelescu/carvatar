#include "FileSelect.h"

char *filterTexts[]={
    "Data File \t*.dat",
    "Log File \t*.log",
    "Raw Data File \t*.txt",
    "Neural Network File *.net"
};


char* FileSelect(char *Msg,FileType f,SelectType s)
{
    Fl_Native_File_Chooser *fnfc=new Fl_Native_File_Chooser();;
    fnfc->title(Msg);
    if(s== Open)
      fnfc->type(Fl_Native_File_Chooser::BROWSE_FILE);
    else
      fnfc->type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);

    fnfc->filter(filterTexts[f]);
    switch ( fnfc->show() ) {
      case -1:
          fl_alert("ERROR: %s\n", fnfc->errmsg());
          return NULL;
      break; // ERROR
      case 1:
           return NULL;
      break; // CANCEL
      default:
          return fnfc->filename();
      break; // FILE CHOSEN
    }

}

