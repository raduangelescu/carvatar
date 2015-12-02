const char VERSION_FL_PLOTXY[]="V1.0.13";
/******************************************************************
*                        FL_PlotXY.cxx
*
* Multi line X-Y data set plotter for FLTK
*
* Each line has independant scale and colour control.
*
*
*
* Author: Michael Pearce
*
* Started: 6 July 2004
*
* Copyright: Copyright 2004 Michael Pearce
*            All Rights Reserved
*
* Licence: LGPL with exceptions same as FLTK license Agreement
*          http://www.fltk.org/COPYING.php
*
* This widget based inpart on the work of the FLTK project
* http://www.fltk.org/
*
*******************************************************************
*                   Version Information
*******************************************************************
* Version 1.0.13 - 14 April 2005
* Start Adding Vertical value finder.
*******************************************************************
* Version 1.0.12 - 2 August 2004
*  Added version string return
*******************************************************************
* Version 1.0.11 - 28 July 2004
*  Added more Time Formatters for display.
*******************************************************************
* Version 1.0.10 - 26 July 2004
*  Added savexyyy and load xyyy to load and save csv files with
*   first column X and remainder Y's
*  Added linetitle functions so can add/read titles.
*  Some bug fixes.
*******************************************************************
* Version 1.0.9 - 23 July 2004
*  Added more debug printf's
*  Some bug fixes.
*  Work on making it work with MINGW/Win32
*******************************************************************
* Version 1.0.8 - 19 July 2004
*  Fixed a bug or two.
*  Added Formatter adjustment for X and Y values.
*   yvalformat, xvalformat
*  Include Rotated.H and Rotated.C from Bill Spitzak's flwm
*******************************************************************
* Version 1.0.7 - 16 July 2004
* Added size adjustability of Font Face and size as well as
*  Value margin sizes.
*******************************************************************
* Version 1.0.6 - 15 July 2004
* Add Values to X and Y Axis
*******************************************************************
* Version 1.0.5 - 14 July 2004
* Work on adding X and Y Graduation markings
*******************************************************************
* Version 1.0.4 - 13 July 2004
* Add the following:
*  lineshow    - Show the Line
*  linehide    - Hide the Line
*  linevisable - Check if line shown or not
*
*******************************************************************
* Version 1.0.3 - 12 July 2004
* Update some functions to use pointers to data instead of direct
* Fix realloc faults
* Fix Scaling Fault
*******************************************************************
* Version 1.0.2 - 8 July 2004
* Added the following features:
*   Fl_Color linecolor(int);
*   int used(int);
*
*******************************************************************
* Version 1.0.1 - 7 July 2004
* Added the following features:
*  int getrange(int,double*,double*,double*,double*);
*  int change(int,unsigned long int,double,double);
*  int read (int,unsigned long int,double*,double*);
*  unsigned long int datasize(int);
*  int linemode(int, int);
*
*******************************************************************
* Version 1.0.0 - 6 July 2004
* Complete Re-Write of original version
* Some Code from V0.0.1 made it across but has been modified!
* Not all features implimented yet!
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fl_PlotXY.H"



#ifdef WIN32
 #include <windows.h>
#else
 #include "Rotated.H"
#endif




int P_DEBUG =0;


/************************************************************************
*                          Fl_PlotXY
************************************************************************/
Fl_PlotXY:: Fl_PlotXY(int X,int Y,int W,int H,const char *l)
: Fl_Widget(X,Y,W,H,l)
{
 int count;

 /* Border box to start with */
 box(FL_EMBOSSED_BOX);
 color(FL_WHITE);


 /* Set Basic Fonts and Sizes */
 AxisFontSize=12;
 AxisFontFace=FL_HELVETICA;
 /* Other Options are FL_HELVETICA, FL_COURIER, FL_TIMES, FL_SCREEN */


 AxisTextSpaceX=15;  /* Text space for X Axis Display */
 AxisTextSpaceY=60;  /* Text Spaceing for Y Axis Display */


 /* Initialise Data */
 for(count=0;count<FL_PLOTXY_MAXLINES;count++)
 {
  LineData[count].LineNumber=-1;     /* Line Number for this array        */
  LineData[count].DrawMode=FL_PLOTXY_LINE;  /* Line Drawing Mode          */
  LineData[count].ScaleMode=FL_PLOTXY_AUTO; /* Scaling Mode               */

  LineData[count].Xmax=0.0;          /* Maximum Axis Value for this line  */
  LineData[count].Ymax=0.0;
  LineData[count].Xmin=0.0;          /* Minimum Axis Value for this line  */
  LineData[count].Ymin=0.0;
  LineData[count].title=NULL;        /* Pointer to string array for title */
  LineData[count].C=FL_BLACK;        /* Line and title Colour             */

  LineData[count].DataSize=0;        /* Number of Points in array         */
  LineData[count].data=NULL;         /* Pointer to data for this line     */


  LineData[count].XLabel=NULL;       /* Pointer to string for X label     */
  LineData[count].YLabel=NULL;       /* Pointer to string for Y label     */


  sprintf(LineData[count].XValFormat,"%%0.02lf");   /* Formatter for X Value display */
  sprintf(LineData[count].YValFormat,"%%0.02lf");   /* Formatter for Y Value display */

 }

 /* Dont have any line shown */
 vlinepos(0);
 vlineenable(0);
 vlinesel(-1);

}


/************************************************************************
*                          ~Fl_PlotXY
************************************************************************/
Fl_PlotXY::~Fl_PlotXY(void)
{
 removeall(); /* Free All allocated memory */
}



/************************************************************************
*                          version
************************************************************************/
const char* Fl_PlotXY::version(void)
{
 return(VERSION_FL_PLOTXY);
}

/************************************************************************
*                          getselected
* returns pointer to data selected by int
* returns NULL if invalid
************************************************************************/
Fl_PlotXY_Line* Fl_PlotXY:: getselected(int line)
{
 if(P_DEBUG > 1) printf("Fl_PlotXY::getselected(int line)\n");

 if(line >= FL_PLOTXY_MAXLINES || line <0) return(NULL);

 if(LineData[line].LineNumber==-1) return(NULL);

 return(&LineData[line]);
}


/************************************************************************
*                          newline
* Create a new Data set
* returns descripter to line
************************************************************************/
int Fl_PlotXY:: newline(double Xmin,double Ymin,double Xmax,double Ymax,int scalemode, Fl_Color C,const char *t)
{
 int count;
 Fl_PlotXY_Line* L;

 if(P_DEBUG > 1) printf("Fl_PlotXY::newline(double,double,double,double,Fl_Color,cont char*\n");

 /* Scan array for first free data */
 for(count=0;count< FL_PLOTXY_MAXLINES;count++)
 {
  /* Create the New Data */
  if(LineData[count].LineNumber!=count)
  {
   L = &LineData[count];

   L->LineNumber=count;
   L->DrawMode=FL_PLOTXY_LINE;   /* Line Drawing Mode         */

   L->ScaleMode=scalemode;

   L->Displayed=1;              /* Turn Line Display ON */


   L->XMark=0;             /* X Graduation Marks off         */
   L->YMark=0;             /* Y Graduation Marks off         */
   L->XMarkSize=0.0;         /* X Graduation step  off         */
   L->YMarkSize=0.0;         /* Y Graduation step  off         */

   L->XValType=0;          /* Type of data for X Axis Value     */
   L->YValType=0;          /* Type of data for Y Axis Value     */


   if(Xmin < Xmax)
   {
    L->Xmax=Xmax;
    L->Xmin=Xmin;
   }
   else
   {
    L->Xmax=Xmin;
    L->Xmin=Xmax;
   }

   if(Ymin < Ymax)
   {
    L->Ymax=Ymax;
    L->Ymin=Ymin;
   }
   else
   {
    L->Ymax=Ymin;
    L->Ymin=Ymax;
   }


   L->title=t;     /* Pointer to string array for title */
   L->C=C;         /* Line and title Colour             */

   L->DataSize=0;  /* Number of Points in array         */

   /* Allocate at least one lot of data even though not used */
   L->data=(Fl_PlotXY_Data*)realloc(L->data,sizeof(Fl_PlotXY_Data));

   return(count);
  }
 }

 if(P_DEBUG > 0) printf(" ERROR: Out of line allocations\n");
 return(-1);
}

/************************************************************************
*                            add
* Add data to selected line
************************************************************************/
int Fl_PlotXY::add(int line,double X,double Y)
{
 unsigned long int ds;
 Fl_PlotXY_Line* L;
 Fl_PlotXY_Data* D;

 if(P_DEBUG > 1) printf("Fl_PlotXY::add(int,double,double)\n");

 if ((L=getselected(line))==NULL) return 0; /* Get Data */

 ds=L->DataSize;                    /* Get Data Size   */
 D=L->data;                         /* Pointer to data */

 /* Go To Data Location */
 while(ds >0)
 {
  ds--;
  D++;
 }

 /* Add New Data */
 D->X=X;
 D->Y=Y;

 L->DataSize++;

 if(P_DEBUG > 1) printf(" DataSize=%ld  -> %ld bytes\n",L->DataSize, L->DataSize * sizeof(Fl_PlotXY_Data));



 /* Allocate More memory */
 D=(Fl_PlotXY_Data*)realloc(L->data,sizeof(Fl_PlotXY_Data)* (L->DataSize+1));

 if(D == NULL)
 {
  if(P_DEBUG > 0) printf(" ERROR: Not Enough Space\n");
  /* Not Enough Space!! */
  L->DataSize--;
  return(0);
 }
 else
 {
  if(P_DEBUG > 1) printf(" New Memory allocated\n");
  /* Point to new Allocated Memory */
  L->data=D;
 }
 return(1);
}


/************************************************************************
*                          linecolor
* Set Line Colour
************************************************************************/
int Fl_PlotXY::linecolor(int line,Fl_Color C)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 1) printf("Fl_PlotXY::linecolor(int line,Fl_Color C)\n");
 if ((L=getselected(line))==NULL) return 0; /* Get Data */
 L->C=C;                                    /* Set Colour */
 return(1);
}


/************************************************************************
*                          linecolor
* Return Line colour
************************************************************************/
Fl_Color Fl_PlotXY::linecolor(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 1) printf("Fl_PlotXY::linecolor(int line)\n");
 if ((L=getselected(line))==NULL) return (FL_BLACK); /* Get Data */
 return(L->C);
}



/************************************************************************
*                          setrange
*Set lines range
************************************************************************/
int Fl_PlotXY::setrange(int line,double Xmin,double Ymin,double Xmax,double Ymax)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 1) printf("Fl_PlotXY::setrange(int line,double Xmin,double Ymin,double Xmax,double Ymax)\n");
 if ((L=getselected(line))==NULL) return 0; /* Get Data */

 /* Set the Min and Max values - check for correct way around */
 if(Xmin < Xmax)
 {
  L->Xmin=Xmin;
  L->Xmax=Xmax;
 }
 else
 {
  L->Xmin=Xmax;
  L->Xmax=Xmin;
 }

 if(Ymin < Ymax)
 {
  L->Ymin=Ymin;
  L->Ymax=Ymax;
 }
 else
 {
  L->Ymin=Ymax;
  L->Ymax=Ymin;
 }
 return(1);
}

/************************************************************************
*                          getrange
* Returns range from current mode
************************************************************************/
int Fl_PlotXY::getrange(int line,double* Xmin,double* Ymin,double* Xmax,double* Ymax)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 1) printf("Fl_PlotXY::getrange(int line, double* X,double* Y,double* X2,double* Y2)\n");

 if ((L=getselected(line))==NULL) return 0; /* Get Data */

 switch(L->ScaleMode)
 {
  default:
  case FL_PLOTXY_AUTO:
   minmax(L);
   *Xmin=MinX;
   *Xmax=MaxX;
   *Ymin=MinY;
   *Ymax=MaxY;
   break;

  case FL_PLOTXY_FIXED:
   *Xmin=L->Xmin;
   *Xmax=L->Xmax;
   *Ymin=L->Ymin;
   *Ymax=L->Ymax;
   break;
 }
 return(1);
}



/************************************************************************
*                          scalemode
*Sets the scale mode
************************************************************************/
int Fl_PlotXY:: scalemode(int line , int mode)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 1) printf("Fl_PlotXY::scalemode(int line, int mode)\n");
 if ((L=getselected(line))==NULL) return (0); /* Get Data */
 L->ScaleMode=mode;
 return(1);
}


/************************************************************************
*                          linemode
*Sets the line mode
************************************************************************/
int Fl_PlotXY:: drawmode(int line , int mode)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 1) printf("Fl_PlotXY::drawmode(int line , int mode)\n");
 if ((L=getselected(line))==NULL) return (0); /* Get Data */
 L->DrawMode=mode;
 return(1);

}




/************************************************************************
*                          clear
*Clear the lines data
************************************************************************/
void Fl_PlotXY::clear(int line)
{
 Fl_PlotXY_Line* L;

 if(P_DEBUG > 1) printf("Fl_PlotXY::clear(int line)\n");
 if ((L=getselected(line))==NULL) return; /* Check for valid line */

 if(P_DEBUG > 1)printf(" Freeing Memory\n");

  /* Free the Memory Used */
 if(L->data  != NULL)free(L->data);
// L->data=NULL;


 if(P_DEBUG > 1)printf(" Allocating Memory\n");

 /* Allocate one spot of memory */
 L->data=(Fl_PlotXY_Data*) realloc(NULL,sizeof(Fl_PlotXY_Data));

 L->DataSize=0;                /* Set Data size to 0 */

}


/************************************************************************
*                          clearall
* Clear all lines data
************************************************************************/
void Fl_PlotXY::clearall(void)
{
 int count;
 if(P_DEBUG > 1) printf("Fl_PlotXY::clearall(void)\n");

 for(count=0;count < FL_PLOTXY_MAXLINES;count++)
 {
  clear(count);
 }
}


/************************************************************************
*                          remove
* Remove individual line
************************************************************************/
void Fl_PlotXY::remove(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 1) printf("Fl_PlotXY::remove(int line)\n");
 if ((L=getselected(line))==NULL) return; /* Check for valid line */

 /* Free any memory used */
 clear(line);

 /* Indicate Not used any more */
 L->LineNumber=-1;
 L->DataSize=0;                /* Set Data size to 0        */
 L->Displayed=0;               /* Turn Line Display OFF     */
 L->DrawMode=FL_PLOTXY_LINE;   /* Line Drawing Mode         */
 L->ScaleMode=FL_PLOTXY_AUTO;
 L->Xmin=0.0;
 L->Xmax=0.0;
 L->Ymin=0.0;
 L->Ymax=0.0;
 L->title=NULL;     /* Pointer to string array for title */
 L->C=FL_BLACK;         /* Line and title Colour             */


}


/************************************************************************
*                          removeall
* Remove All Lines
************************************************************************/
void Fl_PlotXY::removeall(void)
{
 int count;
 if(P_DEBUG > 1) printf("Fl_PlotXY::removeall(void)\n");
 for(count=0;count < FL_PLOTXY_MAXLINES;count++)
 {
  remove(count);
 }
}


/************************************************************************
*                          change
* Change value of a point
************************************************************************/
int Fl_PlotXY:: change(int line ,unsigned long int p,double X,double Y)
{
 Fl_PlotXY_Line* L;
 Fl_PlotXY_Data* D;


 if(P_DEBUG > 1) printf("Fl_PlotXY::change(int,unsigned long int,double,double)\n");

 /* Get Data */
 if ((L=getselected(line))==NULL) return 0; /* Data Set not avaliable */

 /* Check  if change is in range */
 if(p >= L->DataSize)return 0; /* Out of range */

 if(P_DEBUG > 1) printf(" Changing Data\n");

 D=L->data;

 /* Change the Data */
 while(p >0){D++,p--;}

 D->X=X;
 D->Y=Y;

 if(P_DEBUG > 1) printf(" Done\n");
 return(1);
}



/************************************************************************
*                          read
* Read value of a point
************************************************************************/
int Fl_PlotXY:: read (int line ,unsigned long int p,double *X,double *Y)
{
 Fl_PlotXY_Line* L;
 Fl_PlotXY_Data* D;

 *X=0.0;
 *Y=0.0;

 if(P_DEBUG > 1) printf("Fl_PlotXY::read(int,unsigned long int,double*,double*)\n");

 /* Get Data */
 if ((L=getselected(line))==NULL) return 0; /* Data Set not avaliable */

 /* Check  if change is in range */
 if(p >= L->DataSize)return 0; /* Out of range */

 if(P_DEBUG > 1) printf(" Reading Data\n");

 D=L->data;

 /* Change the Data */
 while(p >0){D++,p--;}

 *X = D->X;
 *Y = D->Y;

 return(1);
}


/************************************************************************
*                          datasize
*Return current Data position
************************************************************************/
unsigned long int Fl_PlotXY:: datasize(int line)
{
 Fl_PlotXY_Line* L;

 if(P_DEBUG > 1) printf("Fl_PlotXY::datasize(int)\n");

 /* Get Data */
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 if(P_DEBUG > 1) printf(" DataSize=%ld\n",L->DataSize);

 return(L->DataSize);
}


/************************************************************************
*                          used
* Indicates if line number is in use
************************************************************************/
int Fl_PlotXY:: used(int line)
{
 Fl_PlotXY_Line* L;

 if(P_DEBUG > 1) printf("Fl_PlotXY::used(int)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */
 return(1);
}


/************************************************************************
*                          lineshow
*Turn a line on
************************************************************************/
int Fl_PlotXY:: lineshow(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 1) printf("Fl_PlotXY::lineshow(int)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->Displayed=1;

 return(1);
}


/************************************************************************
*                          linehide
*turn Line off
************************************************************************/
int Fl_PlotXY:: linehide(int line)
{
 Fl_PlotXY_Line* L;

 if(P_DEBUG > 1) printf("Fl_PlotXY::linehide(int)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->Displayed=0;

 return(1);
}

/************************************************************************
*                          linevisible
*Check if line is currently visible
************************************************************************/
int Fl_PlotXY:: linevisible(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 1) printf("Fl_PlotXY::linevisible(int)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (-1); /* Data Set not avaliable */

 return(L->Displayed);
}





/************************************************************************
*                          xmarkon(int)
*check if X marks are On or Off
************************************************************************/
int Fl_PlotXY::xmarkon(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::xmarkon(int)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (-1); /* Data Set not avaliable */

 return(L->XMark);
}




/************************************************************************
*                            xmarkon(int,int)
*Turn X marks on or Off
************************************************************************/
int Fl_PlotXY::xmarkon(int line,int mode)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::xmarkon(int,int)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->XMark=mode;

 return(1);
}




/************************************************************************
*                          xmarkstep(int,double)
*Set the X mark step size
************************************************************************/
int Fl_PlotXY::xmarkstep(int line,double size)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::xmarkstep(int,double)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->XMarkSize=size;

 return(1);

}



/************************************************************************
*                          xmarklabel(int,const char*)
*Set the X Axis Label
************************************************************************/
int Fl_PlotXY::xmarklabel(int line,const char* xlabel)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::xmarklabel(int,const char*)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->XLabel=xlabel;

 return(1);

}

/************************************************************************
*                          const char* xmarklabel(int)
*Return the X Axis Label
************************************************************************/
const char* Fl_PlotXY::xmarklabel(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::xmarklabel(int,const char*)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return NULL; /* Data Set not avaliable */

 return(L->XLabel);

}


/************************************************************************
*                     xvaltype(int,int)
* Set the X value type (time, date etc)
************************************************************************/
int Fl_PlotXY:: xvaltype(int line ,int type)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::xvaltype(int,int)\n");
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->XValType=type;

 return(1);
}


/************************************************************************
*                     xvaltype(int,int)
* return X value type (time, date etc)
************************************************************************/
int Fl_PlotXY:: xvaltype(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::xvaltype(int,int)\n");
 if ((L=getselected(line))==NULL) return (-1); /* Data Set not avaliable */

 return(L->XValType);
}




/************************************************************************
*                          ymarkon(int)
*check if Y marks are On or Off
************************************************************************/
int Fl_PlotXY::ymarkon(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::ymarkon(int)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (-1); /* Data Set not avaliable */

 return(L->YMark);

}




/************************************************************************
*                          ymarkon(int,int)
*Turn Y marks on or Off
************************************************************************/
int Fl_PlotXY::ymarkon(int line ,int mode)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::ymarkon(int,int)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->YMark=mode;

 return(1);

}




/************************************************************************
*                     ymarkstep(int,double)
* Set the Y mark step size
************************************************************************/
int Fl_PlotXY::ymarkstep(int line,double size)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::ymarkstep(int,double)\n");
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->YMarkSize=size;

 return(1);
}




/************************************************************************
*                          ymarklabel(int,const char*)
*Set the Y Axis Label
************************************************************************/
int Fl_PlotXY::ymarklabel(int line,const char* ylabel)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::ymarklabel(int,const char*)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->YLabel=ylabel;

 return(1);
}

/************************************************************************
*                    const char* ymarklabel(int)
*Return the X Axis Label
************************************************************************/
const char* Fl_PlotXY::ymarklabel(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::xmarklabel(int,const char*)\n");
 /* Get Data */
 if ((L=getselected(line))==NULL) return NULL; /* Data Set not avaliable */

 return(L->YLabel);

}


/************************************************************************
*                     yvaltype(int,int)
* Set the Y value type (time, date etc)
************************************************************************/
int Fl_PlotXY:: yvaltype(int line,int type)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::yvaltype(int,int)\n");
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->YValType=type;

 return(1);
}


/************************************************************************
*                     yvaltype(int)
* Return the Y value type (time, date etc)
************************************************************************/
int Fl_PlotXY:: yvaltype(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::yvaltype(int,int)\n");
 if ((L=getselected(line))==NULL) return (-1); /* Data Set not avaliable */

 return(L->YValType);
}




/************************************************************************
*                     xvalformat(int,int,int)
* Set the X value format  %0whole.0pointslf
************************************************************************/
int Fl_PlotXY::xvalformat(int line ,int whole, int points)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::xvalformat(int,int,int)\n");
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 if(whole < 10 && whole >=0 && points < 10 && points >=0)
 {
  sprintf(L->XValFormat,"%%0%d.0%dlf",whole,points);
  return(1);
 }
 return(0);
}



/************************************************************************
*                     yvalformat(int,int,int)
* Set the Y value format  %0whole.0pointslf
************************************************************************/
int Fl_PlotXY::yvalformat(int line, int whole ,int points)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::yvalformat(int,int,int)\n");
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 if(whole < 10 && whole >=0 && points < 10 && points >=0)
 {
  sprintf(L->YValFormat,"%%0%d.0%dlf",whole,points);
  return(1);
 }
 return(0);
}


/************************************************************************
*                linetitle(int , const char* )
*
************************************************************************/
int Fl_PlotXY:: linetitle(int line, const char* title)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::yvalformat(int,int,int)\n");
 if ((L=getselected(line))==NULL) return (0); /* Data Set not avaliable */

 L->title=title;
 return(1);
}


/************************************************************************
*                         linetitle(int)
*
************************************************************************/
const char* Fl_PlotXY:: linetitle(int line)
{
 Fl_PlotXY_Line* L;
 if(P_DEBUG > 2) printf("Fl_PlotXY::yvalformat(int,int,int)\n");
 if ((L=getselected(line))==NULL) return (NULL); /* Data Set not avaliable */

 return(L->title);
}







/************************************************************************
*                              draw
************************************************************************/
void Fl_PlotXY:: draw()
{
 int count;

 Fl_PlotXY_Line* L;

 Fl_Color col = color();
 draw_box(box(), col);

 if(P_DEBUG > 2) printf("Fl_PlotXY:: draw()\n");


 /* Stop any drawing outside of the boxed area */
 fl_push_clip(x()+2,y()+2,w()-4,h()-4);

 /* Calculate the plotable area */
 calc_plot_area();

 /* Clip the graph only area */
 fl_push_clip(PX,PY,PW,PH+1);

 /* Check all lines for drawing */
 for(count=0;count < FL_PLOTXY_MAXLINES;count++)
 {

  /* Check for Valid data to plot and if it is on or off */
  if ((L=getselected(count))!=NULL )
  {
   if(L->Displayed == 1)
   {
    switch(L->ScaleMode)
    {
     default:
     case FL_PLOTXY_AUTO:
      if(P_DEBUG > 1) printf(" FL_PLOTXY_AUTO\n");
      minmax(L);
      break;

     case FL_PLOTXY_FIXED:
      if(P_DEBUG > 1) printf(" FL_PLOTXY_FIXED\n");
      MinX=L->Xmin;
      MaxX=L->Xmax;
      MinY=L->Ymin;
      MaxY=L->Ymax;
      break;
    }

    if(P_DEBUG > 1) printf(" MaxX=%lf, MaxY=%lf\n MinX=%lf, MinY=%lf\n",MaxX,MaxY,MinX,MinY);

    /* Plot the Graph on the Display */
    if(L->data != NULL)
    {
     switch(L->DrawMode)
     {
      /* Plot Using Dots */
      default:
      case FL_PLOTXY_DOT:
       plot_dot(L);
       break;


      /* Plot Using Lines */
      case FL_PLOTXY_LINE:
       plot_line(L);
       break;
     }
    }
   }
  }
 }


 /* Draw the Vertical Line */
 if(vlineenable())
 {
  if((L=getselected(vlinesel()))!=NULL)
  {
   draw_vline(L);
  }
 }


 /* Pop the Graph only area */
 fl_pop_clip();

 /* Draw the Axis Markings and Labels */
 for(count=0;count < FL_PLOTXY_MAXLINES;count++)
 {
  if((L=getselected(count))!=NULL)
  {
   draw_scales(L); /* Draw Axis       */
   draw_axislabel(L); /* Draw Axis Label */
  }
 }

 /* Draw the Line Labels */
 for(count=0;count < FL_PLOTXY_MAXLINES;count++)
 {
  if((L=getselected(count))!=NULL)
  {
   draw_linelabel(L);
  }
 }



 /* Pop from the clipped area */
 fl_pop_clip();

 /* Draw the label last so it is on the TOP */
 draw_label();
}


/************************************************************************
*                           calc_plot_area
************************************************************************/
void Fl_PlotXY::calc_plot_area(void)
{
 Fl_PlotXY_Line* L;
 int count;
 int tx,ty,tw,th,ux,uy,uw,uh;
 int mw,mh;

 if(P_DEBUG>1)printf("Fl_PlotXY::calc_plot_area(void)\n");

 /* Check if using graduations etc
 *  and calc Plot Area required (Scans ALL Values)*/

 ux=PX=x()+2;
 uy=PY=y()+2;
 uw=PW=w()-5;
 uh=PH=h()-5;

 if(P_DEBUG>1)printf(" Basic => PX=%d, PY=%d, PW=%d, PH=%d\n",PX,PY,PW,PH);


 fl_font(AxisFontFace,AxisFontSize);


 for(count=0;count < FL_PLOTXY_MAXLINES;count++)
 {
  if( (L=getselected(count))!=NULL && L->Displayed )
  {
   tx=PX;ty=PY;tw=PW;th=PH;

   mw=0;mh=0;fl_measure("H",mw,mh,1);   /* Get Text Height */

   if(L->XMark & FL_PLOTXY_MARK_ON)
   {
    th -= 10; /* Size of the Marking */
    if((L->XMark & FL_PLOTXY_MARK_LABEL) && L->XLabel !=NULL) th -= (mh+2);
    if(L->XMark & FL_PLOTXY_MARK_VAL)th -= (mh+2);
    //if(L->XMark & FL_PLOTXY_MARK_NAME)  tw -= 40;
   }

   if(L->YMark & FL_PLOTXY_MARK_ON)
   {
    tx += 10;tw-=10;
    if( (L->YMark & FL_PLOTXY_MARK_LABEL) && L->YLabel != NULL)
    {
     mw=0;mh=0;fl_measure(L->YLabel,mw,mh,1);
     tx += (mh+2);tw-=(mh+2);
    }
    if(L->YMark & FL_PLOTXY_MARK_VAL)   {tx += AxisTextSpaceY;tw-=AxisTextSpaceY;}
    //if((L->YMark & FL_PLOTXY_MARK_NAME))continue;
   }

   /* Check for changes to min/max range */
   if(tx > ux) ux=tx;
   if(ty > uy) uy=ty;
   if(tw < uw) uw=tw;
   if(th < uh) uh=th;
  }
 }

 /* Set the GRAPH drawing Area */
 PX=ux;
 PY=uy;
 PW=uw;
 PH=uh;

 if(P_DEBUG>1)printf(" After => PX=%d, PY=%d, PW=%d, PH=%d\n",PX,PY,PW,PH);

}




/************************************************************************
*                           plot_dot
************************************************************************/
void Fl_PlotXY::plot_dot(Fl_PlotXY_Line* L)
{

 int xx,yy;
 Fl_PlotXY_Data *TempData;
 unsigned long int count;

 if(P_DEBUG > 3) printf("Fl_PlotXY::plot_dot()\n");

 TempData = L->data;

 /* Check for Valid Data */
 if(TempData == NULL)return;

 /* Set up zoom scale etc */
 setscale(L);


 /* Set Line Colour */
 fl_color(L->C);

 /* Plot the Data */
 for(count=0;count<L->DataSize;count++)
 {
  xx= PX + (int)((TempData->X + L->X_Left) * L->X_Scale);
  yy= PY+PH-(int) ((TempData->Y + L->Y_Bottom) * L->Y_Scale);

  fl_point(xx, yy);
  TempData++;
 }
}







/************************************************************************
*                            plot_line
************************************************************************/
void Fl_PlotXY::plot_line(Fl_PlotXY_Line* L)
{
 int xx,yy,lx,ly;
 Fl_PlotXY_Data *TempData;
 unsigned long int count;

 if(P_DEBUG > 3) printf("Fl_PlotXY::plot_line()\n");

 TempData = L->data;

 /* Check for Data */
 if(TempData == NULL)return;

 /* Set up zoom scale etc */
 setscale(L);


 /* Get First Point of Data*/

 lx=PX + (int)((TempData->X + L->X_Left) * L->X_Scale);
 ly=PY+PH-(int) ((TempData->Y + L->Y_Bottom) * L->Y_Scale);

 TempData++;


 /* Set Line Colour */
 fl_color(L->C);

 /* Plot the Data */
 for(count=1;count < L->DataSize;count++)
 {
  xx=PX+(int)((TempData->X+L->X_Left)*L->X_Scale);
  yy=PY+PH-(int)((TempData->Y+L->Y_Bottom)*L->Y_Scale);
  fl_line(lx,ly,xx, yy);
  lx=xx;
  ly=yy;
  TempData++;
 }
}



/************************************************************************
*                          draw_scales
************************************************************************/
void Fl_PlotXY::draw_scales(Fl_PlotXY_Line* L)
{
 double start,finish,step,tmp;

 int xx,yy,tw,th,h,m,s,r,tt;
 char str[20];

 fl_font(AxisFontFace,AxisFontSize);



 if(P_DEBUG >1)printf("Fl_PlotXY::draw_scales(Fl_PlotXY_Line* L)\n");

 /***** XAXIS ******/
 if( (L->XMark & FL_PLOTXY_MARK_ON) && (L->XMarkSize != 0.0) && L->Displayed==1)
 {
  /* Calculate the graduations using limits and scale etc */

  step=L->XMarkSize;

  start = (int)(L->Xmin / L->XMarkSize) * L->XMarkSize ;

  finish= L->Xmax;

  if(P_DEBUG >1)
  {
   printf(" Xmin=%lf, Xmax=%lf, XMarkSize=%lf, X_Scale=%lf\n",L->Xmin,L->Xmax,L->XMarkSize,L->X_Scale);
   printf(" step=%lf, start=%lf, finish=%lf\n",step,start,finish);
  }

  /* Set Line Colour */
  fl_color(L->C);


  for(;start<finish;start+=step)
  {
   xx=PX+(int)((start + L->X_Left )* L->X_Scale);
   yy=PY+PH+3;
   fl_line(xx,yy,xx,yy+5);

   /* Print Value */
   if( (L->XMark & FL_PLOTXY_MARK_VAL))
   {

    tt=(int)start;

    r=(int)((start-(double)tt)*100);       /* hundReds */

    h=tt/3600;        /* Hours */
    tt=tt-(h*3600);
    m=tt/60;          /* Minutes */
    tt=tt-(m*60);
    s=tt;             /* Seconds */
    tt-=s;



    switch(L->XValType)
    {
     default:
     case FL_PLOTXY_VAL_NUMBER:
      //sprintf(str,"%0.3f",start);
      sprintf(str,L->XValFormat,start);
      break;


     case FL_PLOTXY_VAL_TIMEh:
      sprintf(str,"%02d",r);
      break;

     case FL_PLOTXY_VAL_TIMESh:
      sprintf(str,"%02d.%02d",s,r);
      break;

     case FL_PLOTXY_VAL_TIMEMSh:
      sprintf(str,"%02d:%02d.%02d",m,s,r);
      break;

     case FL_PLOTXY_VAL_TIMEHMSh:
      sprintf(str,"%02d:%02d:%02d.%02d",h,m,s,r);
      break;

     case FL_PLOTXY_VAL_TIMES:
      sprintf(str,"%02d",s);
      break;

     case FL_PLOTXY_VAL_TIMEMS:
      sprintf(str,"%02d:%02d",m,s);
      break;

     case FL_PLOTXY_VAL_TIMEHMS:
      sprintf(str,"%02d:%02d:%02d",h,m,s);
      break;

     case FL_PLOTXY_VAL_TIMEHM:
      sprintf(str,"%02d:%02d",h,m);
      break;
    }

    tw=0;th=0;
    fl_measure(str,tw,th,1);
    xx=xx-(tw/2);
    yy=yy+th+2;
    fl_draw(str,xx,yy);
   }
  }
 }

 /***** Y AXIS ******/
 if( (L->YMark & FL_PLOTXY_MARK_ON) && (L->YMarkSize != 0.0) && L->Displayed==1)
 {
  /* Calculate the graduations using limits and scale etc */

  step = L->YMarkSize;

  start = (int)(L->Ymin / L->YMarkSize) * L->YMarkSize ;

  finish = L->Ymax;

  if(P_DEBUG >1)
  {
   printf(" Ymin=%lf, Ymax=%lf, YMarkSize=%lf, Y_Scale=%lf\n",L->Ymin,L->Ymax,L->YMarkSize,L->Y_Scale);
   printf(" step=%lf, start=%lf, finish=%lf\n",step,start,finish);
  }

  /* Set Line Colour */
  fl_color(L->C);


  if(start > finish)
  {
   tmp=start;
   start=finish;
   finish=tmp;
  }

  for(;start<finish;start+=step)
  {
   xx=PX-8;
   yy=PY+PH-(int)((start + L->Y_Bottom) * L->Y_Scale);
   fl_line(xx, yy, xx+5, yy);

    /* Print Value */
   if( (L->YMark & FL_PLOTXY_MARK_VAL))
   {
    switch(L->YValType)
    {
     default:
     case FL_PLOTXY_VAL_NUMBER:
      //sprintf(str,"%0.3lf",start);
      sprintf(str,L->YValFormat,start);
      break;

     case FL_PLOTXY_VAL_TIMEHM:
      tt=(int)start;
      h=tt/3600;
      tt=tt-(h*3600);
      m=tt/60;
      sprintf(str,"%02d:%02d",h,m);
      break;

     case FL_PLOTXY_VAL_TIMEHMS:
      tt=(int)start;
      h=tt/3600;
      tt=tt-(h*3600);
      m=tt/60;
      tt=tt-(m*60);
      s=tt;
      sprintf(str,"%02d:%02d:%02d",h,m,s);
      break;

     case FL_PLOTXY_VAL_DATE:
      sprintf(str,"Date");
      break;
    }

    tw=0;th=0;
    fl_measure(str,tw,th,1);
    xx=xx-(tw)-2;
    yy=yy+(th/2);
    fl_draw(str,xx,yy);
   }
  }
 }

}

/************************************************************************
*                          draw_axislabel
************************************************************************/
void Fl_PlotXY::draw_axislabel(Fl_PlotXY_Line* L)
{
 int xx,yy,tw,th;

 if(P_DEBUG > 3) printf("Fl_PlotXY::draw_axislabel()\n");

 /* Set Line Colour */
 fl_color(L->C);

 fl_font(AxisFontFace,AxisFontSize);

 if(L->Displayed==0) return;

 fl_color(L->C);

 /* X AXIS LABEL */
 if( (L->XMark & FL_PLOTXY_MARK_ON) && (L->XMark & FL_PLOTXY_MARK_LABEL) && L->XLabel != NULL)
 {
  xx=PX+(PW/2);
  //yy=PY+PH+3;
  yy=y()+h()-5;

  tw=0;th=0;

  fl_measure(L->XLabel,tw,th,1);

  xx=xx-(tw/2);
  //yy=yy+th+2;



 // if(L->XMark & FL_PLOTXY_MARK_VAL) /* If values displayed as well - Add text height */
 // {
 //  yy+=th+2;
 // }

  fl_draw(L->XLabel,xx,yy);

  //printf("X=%d, Y=%d, Label=\"%s\"\n",xx,yy,L->XLabel);
 }

 /* Y AXIS LABEL */
 if( (L->YMark & FL_PLOTXY_MARK_ON) && (L->YMark & FL_PLOTXY_MARK_LABEL) && L->YLabel != NULL)
 {
  //xx=PX-AxisTextSpaceY-2;
  xx=x()+4;

  yy=PY+(PH/2);

  tw=0;th=0;
  fl_measure(L->YLabel,tw,th,1);

  //xx=xx-th-2;
  xx+=th;
  yy=yy+(tw/2);

  /* Draw Y Label - Wrapped  THIS NEEDS WORK - Should be rotated 90 degrees!!*/
  #ifdef WIN32

  #else
   draw_rotated(L->YLabel,xx, yy, 90);
  #endif

 }
}


/************************************************************************
*                          draw_linelabel
************************************************************************/
void Fl_PlotXY::draw_linelabel(Fl_PlotXY_Line* L)
{
 int xx,yy,tw=0,th=0;

 if(P_DEBUG > 2) printf("Fl_PlotXY::draw_linelabel()\n");

 if(P_DEBUG > 2)printf(" Processing Line %d\n",L->LineNumber);

 if(L->Displayed==0)
 {
  if(P_DEBUG > 2)printf(" Line %d Not displayed!\n",L->LineNumber);
  return;
 }

 if(L->XMark & FL_PLOTXY_MARK_NAME !=0)
 {
  fl_font(AxisFontFace,AxisFontSize);

  fl_color(L->C);

  fl_measure(L->title,tw,th,1);

  xx=PX+PW-tw-4;
  yy=PY+5+( (L->LineNumber+1) * (th+2));

  fl_draw(L->title,xx,yy);


  if(P_DEBUG > 2)printf(" X=%d, Y=%d, Title= %s\n",xx,yy ,L->title);

 }
 else
 {
   if(P_DEBUG > 2)printf(" Title Not enabled!");
 }
}


/***********************************************************************
*                         draw_vline
***********************************************************************/
void Fl_PlotXY:: draw_vline(Fl_PlotXY_Line* L)
{
 char str[20];
 int xx,yy,tw,th;
 Fl_PlotXY_Data *TempData;

 if(P_DEBUG > 3) printf("Fl_PlotXY::draw_vline(Fl_PlotXY_Line* L)\n");

 TempData = L->data;

 /* Check for Data */
 if(TempData == NULL)return;

 if(VLinePos >= L->DataSize) VLinePos = L->DataSize;
 TempData+=VLinePos;


  /* Get Data*/

 xx=PX + (int)((TempData->X + L->X_Left) * L->X_Scale);
 yy=PY+PH-(int) ((TempData->Y + L->Y_Bottom) * L->Y_Scale);

 /* Set Line Colour */
 fl_color(fl_lighter(L->C));

 /* Draw the vertical line */
 fl_line(xx,PY,xx,PY+PH);

 /* Draw the circle */
 fl_circle(xx,yy,5);

 /* Draw the value amount */
 sprintf(str,L->YValFormat,TempData->Y);

 fl_font(AxisFontFace,AxisFontSize);
 //fl_color(L->C);
 //fl_measure(str,tw,th,1);

 fl_draw(str,xx+5,yy);

}




/************************************************************************
*                            minmax
************************************************************************/
void Fl_PlotXY::minmax(Fl_PlotXY_Line* L)
{
 Fl_PlotXY_Data *TempData;
 unsigned long int count;

 if(P_DEBUG > 3) printf("Fl_PlotXY::minmax()\n");

 /* Use Temp Pointer to data */
 TempData=L->data;


 if(TempData !=NULL)
 {
  /* Use First Values as Reference */
  MinX=MaxX=TempData->X;
  MinY=MaxY=TempData->Y;

  /* Scan Data for Min/Max X and Min/Max Y values */
  for(count=0;count< L->DataSize;count++)
  {

   if(MinX > TempData->X) MinX=TempData->X;
   if(MaxX < TempData->X) MaxX=TempData->X;

   if(MinY > TempData->Y) MinY=TempData->Y;
   if(MaxY < TempData->Y) MaxY=TempData->Y;
   TempData++;
  }

  L->Xmin=MinX;
  L->Xmax=MaxX;
  L->Ymin=MinY;
  L->Ymax=MaxY;
 }
}

/************************************************************************
*                            setscale
************************************************************************/
void Fl_PlotXY::setscale(Fl_PlotXY_Line* L)
{
 double mx,MX,my,MY;

 if(P_DEBUG > 1)printf("Fl_PlotXY::setscale(Fl_PlotXY_Line* L)\n");

 /* Ensure Min and Max values are correct */

 if(L->Xmax > L->Xmin){mx=L->Xmin;MX=L->Xmax;}
 else{ mx=L->Xmax;MX=L->Xmin;}

 if(L->Ymax > L->Ymin){ my=L->Ymin;MY=L->Ymax;}
 else{ my=L->Ymax;MY=L->Ymin;}


 /* Calculate the Multiplyer for scaling */
 if( (MX-mx)!=0 )
 {
  L->X_Scale=(double)(PW) / (MX - mx);
 }
 else
 {
  /* If a divide by zero would occur - set scale to 1 */
  L->X_Scale=1;
 }

 if( (MY -my) !=0)
 {
  L->Y_Scale=(double)(PH) / (MY - my);
 }
 else
 {
  /* If a divide by zero would occur - set scale to 1 */
  L->Y_Scale=1;
 }


 /* Calculate the Shift for X and Y */
 /* 0 minus lowest value give amout to Add to put at top/bottom */
 L->X_Left=0.0-mx;
 L->Y_Bottom=0.0-my;



 if(P_DEBUG > 1)
 {
  printf(" X_Scale=%lf\n Y_Scale=%lf\n",L->X_Scale,L->Y_Scale);
  printf(" X_Left=%lf\n Y_Bottom=%lf\n",L->X_Left,L->Y_Bottom);
 }

}





/************************************************************************
*                          load
*
* Loads Comma Separated Value (CSV) File from Disk
* Data MUST be arranged as X,Y
* Any NON Numerical data is Not added to the graph.
* Returns 1 on sucess 0 on failure
************************************************************************/
int Fl_PlotXY:: load(const char *FileName)
{
 Fl_PlotXY_Line* L;
 int line;
 unsigned int tmp;
 char str[255];
 double X,Y;
 FILE *fptr;

 if(P_DEBUG > 2) printf("Fl_PlotXY::load(int,char*)\n");


  /* Create New line */
 if((line=newline())==-1) return(-1);
 if((fptr=fopen(FileName,"r"))==NULL) return(-1);

 while(!feof(fptr))
 {
  /* Read a Line */
  fgets(str,255,fptr);

  /* Check if it is numbers or characters */
  if( (str[0] >= '0' && str[0] <='9') || str[0]=='-' )
  {
   /* Process the Line */
   X=atof(str);
   tmp=0;
   while(tmp < strlen(str))
   {
    switch(str[tmp])
    {
     case ',':
      Y=atof(&str[tmp+1]);
      add(line,X,Y);

     case 0:
      tmp=strlen(str)+1;
      break;

     default:
      tmp++;
      break;
    }
   }
  }
 }

 fclose(fptr);


 if ((L=getselected(line))==NULL) return (-1);
 minmax(L);

 redraw();
 return(line);
}

/************************************************************************
*                         save
************************************************************************/
int Fl_PlotXY:: save(int line,const char *FileName)
{
 FILE *fptr;
 unsigned long int cnt;
 Fl_PlotXY_Line* L;
 Fl_PlotXY_Data* D;

 if(P_DEBUG > 2) printf("Fl_PlotXY::save(int line,const char *FileName)\n");

 if ((L=getselected(line))==NULL) return (0); /* Get Data */


 D=L->data;                         /* Pointer to data */


 if( (fptr=fopen(FileName,"w"))==NULL) return (0);


 fprintf(fptr,"X,Y\n");

 /* Write data to the file */
 for(cnt=0;cnt<L->DataSize;cnt++)
 {
  fprintf(fptr,"%f,%f\n",D->X,D->Y);
  D++;
 }

 fclose(fptr);
 return(1);
}


/************************************************************************
*                           loadxyyy
* Load using first column as X and remaining columns as Y data
* This scans for the Maximum number of Y columns
* First Row is treated as titles for each Axis
* NOTE: This will Clear ALL data dbefore Loading.
************************************************************************/


char XLabel[41];
char YLabel[FL_PLOTXY_MAXLINES][41];
char GraphLineLabel[FL_PLOTXY_MAXLINES][41];


int Fl_PlotXY:: loadxyyy(const char *Filename)
{
 FILE *fptr;

 char str[255],*sptr,*sptr2,label[255];
 int count,H,M,S;
 int lines[FL_PLOTXY_MAXLINES];
 int XMode=FL_PLOTXY_VAL_NUMBER;  /* X Axis value mode */
 double xval,yval;



 if(P_DEBUG > 0) printf("Fl_PlotXY::loadxyyy(const char *Filename)\n");

 if(P_DEBUG >1)printf(" Opening file %s\n",Filename);
 if( (fptr=fopen(Filename,"r")) == NULL)
 {
  if(P_DEBUG >1)printf(" ERROR: Could not open %s\n",Filename);
  return(0);
 }

 if(P_DEBUG >1)printf(" File opened Sucessfully\n Removing all Lines\n");
 removeall();

 if(P_DEBUG >1)printf(" Reading and decoding Line Titles\n");

 for(count=0;count<FL_PLOTXY_MAXLINES;count++)lines[count]=-1; /* Clear all line values */




 /* First Row is line titles */
 fgets(str,254,fptr);

 /* First Column is X Axis - Skip it*/

 if( (sptr=strpbrk(str,",")) != NULL)
 {
  /* Remaining are Line Labels */
  for(count=0;count<FL_PLOTXY_MAXLINES;count++)
  {
   sptr++;
   sprintf(label,"%s",sptr);

   if(strlen(label) > 39)label[40]=0;   /* Limit Label size to 40 Characters */

   sprintf(GraphLineLabel[count],"%s",label);


   if( (sptr2=strpbrk(GraphLineLabel[count],",\n")) != NULL)*sptr2=0; /* Terminate the string */

   /* Create the Line */
   lines[count]=newline(0,0,0,0,FL_PLOTXY_AUTO,FL_BLACK,GraphLineLabel[count]);

   if(P_DEBUG >0)printf(" Line label %d = %s \n",lines[count],GraphLineLabel[count]);

   if( (sptr=strpbrk(sptr,",")) == NULL)
   {
    count=FL_PLOTXY_MAXLINES;
   }
  }
 }



 /* Second Row is Axis Labels */
 fgets(str,254,fptr);

 /* First Column is X Axis */
 sprintf(label,"%s",str);
 if(strlen(label) > 39)label[40]=0;   /* Limit Label size to 40 Characters */
 sprintf(XLabel,"%s",label);

 if(P_DEBUG >1)printf(" X Title = %s \n",XLabel);


 if( (sptr2=strpbrk(XLabel,",")) != NULL) *sptr2=0;

 if( (sptr=strpbrk(str,",")) != NULL)
 {
  /* Remaining Titles are Y Axis Columns */
  for(count=0;count<FL_PLOTXY_MAXLINES;count++)
  {
   sptr++;
   sprintf(label,"%s",sptr);
   if(strlen(label) > 39)label[40]=0;   /* Limit Label size to 40 Characters */
   sprintf(YLabel[count],"%s",label);

   if(P_DEBUG >1)printf(" Y Title = %s \n",YLabel[count]);

   if( (sptr2=strpbrk(YLabel[count],",\n")) != NULL)*sptr2=0; /* Terminate the string */

   //lines[count]=newline(0,0,0,0,FL_PLOTXY_AUTO,FL_BLACK,YLabel[count]);

   if(P_DEBUG >1)printf(" Adding Label to line\n");
   ymarklabel(lines[count],YLabel[count]);
   xmarklabel(lines[count],XLabel);

   if( (sptr=strpbrk(sptr,",")) == NULL)
   {
    count=FL_PLOTXY_MAXLINES;
   }
  }
 }


 if(P_DEBUG >1)printf(" Reading Line Data\n");
 while(!feof(fptr))
 {
  fgets(str,254,fptr);


  if(P_DEBUG >1)printf(" Checking if Time....\n");
  if(strpbrk(str,":")!=NULL)
  {
   /* Looks like the first column is time..... */
   if(P_DEBUG >1)printf(" Decoding time.");
   H=M=S=0;

   H=atoi(str);
   if( (sptr=strpbrk(str,":"))!=NULL)
   {
    sptr++;
    M=atoi(sptr);
    if( (sptr=strpbrk(str,":"))!=NULL)
    {
     sptr++;
     S=atoi(sptr);
    }
   }
   xval=((double)H*3600.0) + ((double)M*60.0) + (double)S;
   XMode=FL_PLOTXY_VAL_TIMEHMS;
  }
  else
  {
   if(P_DEBUG >1)printf(" Plain data found.");
   /* Get X Value  */
   xval=atof(str);
  }

  /* Go to first comma */
  if((sptr=strpbrk(str,","))!=NULL)
  {
   /* Decode data to get Y values */
   for(count=0;count<FL_PLOTXY_MAXLINES;count++)
   {
    sptr=strpbrk(sptr,"01234567890+-.");    /* Moving the numerical value on Line */
    if(sptr == NULL)
    {
     count=FL_PLOTXY_MAXLINES;              /* End of the line */
    }
    else
    {
     yval=atof(sptr);
     add(lines[count],xval,yval);  /* Add Data to graph */
    }
    if((sptr=strpbrk(sptr,","))==NULL)count=FL_PLOTXY_MAXLINES;
   }
  }
  else
  {
   count=FL_PLOTXY_MAXLINES;
  }
 }
 /* Set X Axis Value Mode */
 for(count=0;count<FL_PLOTXY_MAXLINES;count++)
 {
  xvaltype(lines[count],XMode);
  xmarkon(lines[count],FL_PLOTXY_MARK_ON|FL_PLOTXY_MARK_LABEL|FL_PLOTXY_MARK_NAME);
  ymarkon(lines[count],FL_PLOTXY_MARK_ON|FL_PLOTXY_MARK_LABEL);
 }

 fclose (fptr);

 redraw();

 return(1);
}



/************************************************************************
*                           savexyyy
* save using first column as X and remaining columns as Y data
* This scans for the Maximum number of Y columns
* First Row is titles for each Axis
************************************************************************/
int Fl_PlotXY:: savexyyy(const char *Filename)
{
 FILE *fptr;
 unsigned long int DataCount;
 int count,H,M,S,h;
 double X,Y;

 if(P_DEBUG > 2) printf("Fl_PlotXY::savexyyy(const char *Filename)\n");

 if(P_DEBUG >1)printf(" Opening file %s\n",Filename);
 if( (fptr=fopen(Filename,"w")) == NULL)
 {
  if(P_DEBUG >1)printf(" ERROR: Could not open %s\n",Filename);
  return(0);
 }


 /* Skip first column*/
 fprintf(fptr,"X");

 /* Write Y Line Names */
 for(count=0;count<FL_PLOTXY_MAXLINES;count++)
 {
  if(used(count)==1)
  {
   fprintf(fptr,",%s",linetitle(count));
  }
 }
 fprintf(fptr,"\n");



 /* Write X Axis titles */
 fprintf(fptr,"%s",xmarklabel(0));

 /* Write Y Axis titles */
 for(count=0;count<FL_PLOTXY_MAXLINES;count++)
 {
  if(used(count)==1)
  {
   fprintf(fptr,",%s",ymarklabel(count));
  }
 }
 fprintf(fptr,"\n");



 /* Write the X, Y values */
 for(DataCount=0;DataCount<datasize(0);DataCount++)
 {
  X=0;
  /* Write X Value */
  read(0,DataCount,&X,&Y);

  switch(xvaltype(0))
  {
   default:
   case FL_PLOTXY_VAL_NUMBER:
    fprintf(fptr,"%lf",X);
    break;

   case FL_PLOTXY_VAL_TIMEHM:
   case FL_PLOTXY_VAL_TIMEHMS:
   case FL_PLOTXY_VAL_TIMEHMSh:
    H=(int)(X/3600);       /* Calc Hours */
    X-=(double)(H*3600);
    M=(int)(X/60);         /* Calc Minutes */
    X-=(double)(M*60);
    S=(int)X;              /* Calc Seconds */
    X-=(double)S;
    h=(int)(X*100);        /* Calc hundreths */

    fprintf(fptr,"%02d:%02d:%02d.%02d",H,M,S,h);
    break;
  }

  /* Write Y Axis values */
  for(count=0;count<FL_PLOTXY_MAXLINES;count++)
  {
   if(used(count)==1)
   {
    Y=0;
    read(count,DataCount,&X,&Y);
    fprintf(fptr,",%lf",Y);
   }
  }
  fprintf(fptr,"\n");
 }

 fclose(fptr);
 return(1);
}








