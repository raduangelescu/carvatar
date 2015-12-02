/********************************************************************/
/********************************************************************/

/* fl_ext_file_chooser()
 *
 * This code released into the public domain by Don Zimmer
 * with permission from Baird Software, October 2004
 * provided as-is, use at your own risk
 *
 */

/********************************************************************/
/********************************************************************/


#include "fl_ext_file_chooser.h"


#include <stdlib.h>
#include <string.h>


#ifdef WIN32
  #include <FL/Fl.H>
  #include <FL/x.H>
  #include <commdlg.h>    /* includes common dialog functionality */
  #include <dlgs.h>       /* includes common dialog template defines */
  #include <cderr.h>      /* includes the common dialog error codes */
  #include <string>
  using namespace std;
#else
  #include <FL/Fl.H>
  #include <FL/Fl_File_Chooser.H>
#endif


/********************************************************************/
/********************************************************************/


static char fs_path[256];


#ifdef WIN32
static char *buildFilterString(char **pDesc, char **pExt, int nExt, int eExt);
#endif


/********************************************************************/
/********************************************************************/


char *fl_ext_file_chooser(char *pTitle, char **pDesc, char **pExt, int nExt, int eExt, int eMode, char def[256])
{
  int rc;

#ifndef WIN32

  char *ret;

  ret = fl_file_chooser((pTitle)?pTitle:"Select File", (pExt)?pExt[0]:"*", def);
  if(!ret)
  {
    rc = 0;
  }
  else
  {
    strcpy(fs_path, ret);
  }

#else

  OPENFILENAME spec;

  char *pFilter, *ch, dir[256], *slash;
  int ext_idx;

  if(def)
  {
    strcpy(dir, def);
    slash = strrchr(dir, '/');
    if(slash)
    {
      slash[0] = '\0';
      strcpy(fs_path, slash+1);
    }
    else
    {
      dir[0] = '\0';
      strcpy(fs_path, def);
    }
  }
  else
  {
    dir[0] = '\0';
    strcpy(fs_path, "");
  }

  ext_idx = 1;

  if(pExt && def)
  {
    char *dot = strrchr(fs_path, '.');
    char *slash = strrchr(fs_path, '/');
    if(dot && dot > slash)
    {
      int i;
      for(i=0; i<nExt; i++) if(strstr(pExt[0], dot)) break;
      if(i != nExt)
      {
        ext_idx = 1+i;
        if(eMode != EXT_VALID_ONLY) ext_idx++;
      }
    }
  }

  pFilter = buildFilterString(pDesc, pExt, nExt, eExt);

  spec.lStructSize       = sizeof(OPENFILENAME);
  spec.hwndOwner         = GetForegroundWindow();
  spec.hInstance         = GetModuleHandle(NULL);
  spec.lpstrFilter       = pFilter;
  spec.lpstrCustomFilter = NULL;
  spec.nMaxCustFilter    = 0;
  spec.nFilterIndex      = ext_idx;
  spec.lpstrFile         = fs_path;
  spec.nMaxFile          = sizeof(fs_path);
  spec.lpstrFileTitle    = NULL;
  spec.nMaxFileTitle     = 0;
  spec.lpstrInitialDir   = (dir[0])?dir:NULL;
  spec.lpstrTitle        = (pTitle)?pTitle:"Select File";
  spec.nFileOffset       = 0;
  spec.nFileExtension    = 0;
  spec.lpstrDefExt       = NULL;
  spec.lCustData         = (LPARAM)NULL;
  spec.lpfnHook          = NULL;
  spec.lpTemplateName    = NULL;
  spec.Flags             = OFN_EXPLORER|OFN_ENABLESIZING|OFN_NOCHANGEDIR;

  switch(eMode)
  {
  case FIL_WRITE:     /* select a new or existing file for writing */
    spec.Flags |= OFN_OVERWRITEPROMPT;
    rc = GetSaveFileName(&spec);
    break;

  case FIL_READWRITE: /* select an existing file for read-write with option for read-only */
    spec.Flags |= OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
    rc = GetOpenFileName(&spec);
    break;

  case FIL_READ:      /* select an existing file for read-only */
  default:
    spec.Flags |= OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
    rc = GetOpenFileName(&spec);
    break;
  }

  /* in order for the readonly checkbox to be useful, we need to
   * check spec.Flags for OFN_NOREADONLYRETURN
   * currently this is not done... it would require an API change
   */

  free(pFilter);

  if(rc)
  {
    strcpy(fs_path, spec.lpstrFile);
    ch = fs_path;
    while(*ch)
    {
      if(*ch == '\\') *ch = '/';
      ch++;
    }
    if(def) strcpy(def, fs_path);
  }

#endif

  return (rc)?fs_path:NULL;
}


/********************************************************************/
/* support                                                          */
/********************************************************************/


#ifdef WIN32
static char *buildFilterString(char **pDesc, char **pExt, int nExt, int eExt)
{
  char    *comma_spc = ", ";
  char    *pipe      = "|";
  char    *semicolon = ";";
  char    *star      = "*";
  char    *start_all = "All Files (";
  char    *end_all   = ")|";
  char    *all       = "All Files (*.*)|*.*|";

  string   tmp;
  int      i;
  char    *pFilter, *ch;

  if (!pDesc || ! pExt || nExt < 1)
  {
    eExt = EXT_ALLOW_ALL;
    nExt = 0;
  }

  switch (eExt)
  {
  case EXT_ALLOW_ALL:
    tmp += all;
    break;

  case EXT_VALID_ALL:
    tmp += start_all;
    for (i = 0; i < nExt - 1; i++)
    {
      tmp += star;
      tmp += pExt[i];
      tmp += comma_spc;
    }
    tmp += star;
    tmp += pExt[i];
    tmp += end_all;
    for (i = 0; i < nExt - 1; i++)
    {
      tmp += star;
      tmp += pExt[i];
      tmp += semicolon;
    }
    tmp += star;
    tmp += pExt[i];
    tmp += pipe;
    break;

  case EXT_VALID_ONLY:
  default:
    break;
  }

  for (i = 0; i < nExt; i++)
  {
    tmp += pDesc[i];
    tmp += pipe;
    tmp += star;
    tmp += pExt[i];
    tmp += pipe;
  }

  tmp += pipe;

  pFilter = strdup(tmp.c_str());
  ch = pFilter;
  while(*ch)
  {
    if(*ch == '|') *ch = '\0';
    ch++;
  }

  return pFilter;
}
#endif


/********************************************************************/
/********************************************************************/


