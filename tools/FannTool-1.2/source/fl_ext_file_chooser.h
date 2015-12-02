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


#ifndef FL_EXT_FILE_CHOOSER_H
#define FL_EXT_FILE_CHOOSER_H


#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
extern "C" {
#endif


/********************************************************************/
/********************************************************************/

/*
  EXT_VALID_ONLY means only use the passed extensions
  EXT_VALID_ALL  means add All files (comprising only passed extensions)
  EXT_ALLOW_ALL  means add All files *.*
*/
enum { EXT_ALLOW_ALL, EXT_VALID_ALL, EXT_VALID_ONLY };


enum { FIL_READ, FIL_WRITE, FIL_READWRITE };


/********************************************************************/
/********************************************************************/


char *fl_ext_file_chooser(char *pTitle, char **pDesc, char **pExt, int nExt, int eExt, int eMode, char def[256]);


/********************************************************************/
/********************************************************************/


#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif


#endif /* FL_EXT_FILE_CHOOSER_H */


/********************************************************************/
/********************************************************************/

