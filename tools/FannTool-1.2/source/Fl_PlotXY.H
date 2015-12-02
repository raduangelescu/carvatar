/*************************************************************************
*                            Fl_PlotXY.H
*
* Simple X-Y Graph Display
*************************************************************************
*               Version information see Fl_PlotXY.cxx
*************************************************************************/
#ifndef _Fl_PlotXY_H_
#define _Fl_PlotXY_H_

/* Maximum Number of Lines per Plot */
#define FL_PLOTXY_MAXLINES 10


/* Drawing Modes */
#define FL_PLOTXY_DOT   0
#define FL_PLOTXY_LINE  1


/* Scale Modes */
#define FL_PLOTXY_AUTO  0
#define FL_PLOTXY_FIXED 1

/* Zoom Modes */
#define FL_PLOTXY_ZOOMALL 0
#define FL_PLOTXY_ZOOMSELECT 1


/* Marking Modes - OR'd together */
#define FL_PLOTXY_MARK_OFF   0x00           /* Graduations etc OFF */
#define FL_PLOTXY_MARK_ON    0x01           /* Show Y Graduations  */
#define FL_PLOTXY_MARK_LABEL 0x02           /* Show Axis Label     */
#define FL_PLOTXY_MARK_VAL   0x04           /* Show Axis Values    */
#define FL_PLOTXY_MARK_NAME  0x08           /* Show Lines NAME     */


/* Axis Value Types */
#define FL_PLOTXY_VAL_NUMBER   0x00           /* Axis Value is Number */
#define FL_PLOTXY_VAL_TIMES    0x01           /* Axis Value HH:MM:SS 0 ->3600x24 */
#define FL_PLOTXY_VAL_TIMEMS   0x02           /* Axis Value HH:MM:SS 0 ->3600x24 */
#define FL_PLOTXY_VAL_TIMEHMS  0x03           /* Axis Value HH:MM:SS 0 ->3600x24 */
#define FL_PLOTXY_VAL_TIMEHM   0x04           /* Axis Value HH:MM  0 ->3600x24 */
#define FL_PLOTXY_VAL_TIMEh    0x05           /* hh       */
#define FL_PLOTXY_VAL_TIMESh   0x06           /* SS.hh    */
#define FL_PLOTXY_VAL_TIMEMSh  0x07           /* MM:SS.hh */
#define FL_PLOTXY_VAL_TIMEHMSh 0x08           /* Axis Value HH:MM:SS.hh 0.00 ->3600x24 */


#define FL_PLOTXY_VAL_DATE     0x80           /* Date Format - Unused */



#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>



/* Data Points */
typedef struct
{
 double X;
 double Y;
}Fl_PlotXY_Data;



/* Line Data and Array */
typedef struct
{
 int LineNumber;        /* Line Number for this array        */
 int DrawMode;          /* Line Drawing Mode                 */
 int ScaleMode;         /* Scaling Mode                      */
 int Displayed;         /* Indicate if line shown or not     */

 int XMark;             /* X GraduYLabelation Marks mode           */
 int YMark;             /* Y Graduation Marks mode           */
 double XMarkSize;      /* X Graduation step size            */
 double YMarkSize;      /* Y Graduation step size            */

 int XValType;          /* Type of data for X Axis Value     */
 int YValType;          /* Type of data for Y Axis Value     */


 double X_Scale,Y_Scale;
 double X_Left,Y_Bottom;


 double Xmax,Ymax;      /* Maximum Axis Value for this line  */
 double Xmin,Ymin;      /* Minimum Axis Value for this line  */

 const char *XLabel;    /* Pointer to string for X label     */
 const char *YLabel;    /* Pointer to string for Y label     */

 const char *title;     /* Pointer to string array for line title */
 Fl_Color C;            /* Line and title Colour             */

 unsigned long int DataSize; /* Number of Points in array         */
 Fl_PlotXY_Data *data;  /* Pointer to data for this line     */

 char XValFormat[10];   /* Formatter for X Value display */
 char YValFormat[10];   /* Formatter for Y Value display */

}Fl_PlotXY_Line;





class FL_EXPORT Fl_PlotXY: public Fl_Widget
{

 Fl_PlotXY_Line LineData [FL_PLOTXY_MAXLINES]; /* Array of line data */

 double MinX,MaxX,MinY,MaxY;

 int    PX,PY,PW,PH;           /* Plot area location */


 protected:



  virtual void draw();

  /* Drawing functions for different modes */
  void calc_plot_area(void);

  void plot_line(Fl_PlotXY_Line*);
  void plot_dot(Fl_PlotXY_Line*);

  void draw_scales(Fl_PlotXY_Line* L);
  void draw_axislabel(Fl_PlotXY_Line* L);
  void draw_linelabel(Fl_PlotXY_Line* L);
  void draw_vline(Fl_PlotXY_Line* L);

  /* Set scale for drawing */
  void setscale(Fl_PlotXY_Line*);

  /* Min/Max calc for auto scaling */
  void minmax(Fl_PlotXY_Line*);

  /* Font Size to use on the Axis Markers */
  int AxisFontSize;
  int AxisFontFace;

  int AxisTextSpaceX;  /* Text space for X Axis Display */
  int AxisTextSpaceY;  /* Text Spaceing for Y Axis Display */


  /* Gets selected graph details */
  Fl_PlotXY_Line* getselected(int);


  /* Vertical Line Options */
  int VLineEnable;                 /* Enable or Disable Line        */
  int VLineSel;                    /* Display for this line         */
  unsigned long int VLinePos;      /* Position in data to plot line and highlight */
  
  
  
 public:

  Fl_PlotXY(int ,int ,int ,int ,const char * = 0);

  ~Fl_PlotXY(void);

  int newline(double, double, double, double,int, Fl_Color, const char *);
  int newline(void){return(newline(0.0,0.0,0.0,0.0,FL_PLOTXY_AUTO,FL_BLACK,""));};

  int add(int,double ,double);    /* Add data to selected line       */

  int setrange(int,double,double,double,double);  /* Set lines range */
  int getrange(int,double*,double*,double*,double*); /* Gets range from current mode*/

  int linecolor(int,Fl_Color);    /* Set Line Colour                 */
  Fl_Color linecolor(int);        /* Return Line color */

  int scalemode(int, int);        /* Set scale type - Auto or manual */
  int drawmode(int, int);         /* Set Dot/Line drawing mode       */

  void clear(int);                 /* Clear the lines data           */
  void clearall(void);             /* Clear all lines data           */

  void remove(int);                /* Remove individual line         */
  void removeall(void);            /* Remove All Lines               */


  int change(int,unsigned long int,double,double);  /* Change value of a point */
  int read (int,unsigned long int,double*,double*); /* Read value of a point   */

  unsigned long int datasize(int);    /* Return current Data size of line */

  int load(const char *);             /* Load a File into the graph     */
  int loadxyyy(const char *Filename); /* Load Graph data X,Y,Y,Y...  */

  int save(int, const char *);        /* Save graph to CSV format file  */
  int savexyyy(const char *Filename); /* Save Graph data as X,Y,Y,Y..*/



  int used(int);                  /* Indicates if line number used */

  int lineshow(int);               /* Turn a line on */
  int linehide(int);               /* turn Line off  */
  int linevisible(int);            /* Check if line is currently visible */

  int linetitle(int, const char*); /* Set Lines title */
  const char* linetitle(int);      /* Get Lines title */


  int xmarkon(int);                /* check if X marks are On or Off */
  int xmarkon(int,int);            /* Turn X marks on or Off         */
  int xmarkstep(int,double);       /* Set the X mark step size       */
  int xmarklabel(int,const char*); /* X Axis Label                   */
  const char* xmarklabel(int);     /* Returns X Axis Label           */

  int ymarkon(int);                /* check if Y marks are On or Off */
  int ymarkon(int,int);            /* Turn Y marks on or Off         */
  int ymarkstep(int,double);       /* Set the Y mark step size       */
  int ymarklabel(int,const char*); /* Y Axis Label                   */
  const char* ymarklabel(int);     /* Returns Y Axis Label           */



  int xvaltype(int,int);           /* Type of data for X Axis Value     */
  int xvaltype(int);               /* Return X value Type               */

  int yvaltype(int,int);           /* Type of data for Y Axis Value     */
  int yvaltype(int);               /* Return type for Y Axis Value      */


  int xvalformat(int,int,int);     /* X Axis value number format       */
  int yvalformat(int,int,int);     /* Y Axis value number format       */



  void axisfontsize(int a){AxisFontSize=a;};/* Font Size to use on the Axis      */
  int axisfontsize(void){return(AxisFontSize);};

  void axisfontface(int f){AxisFontFace=f;};/* Font Size to use on the Axis      */
  int axisfontface(void){return(AxisFontFace);};


  void axistextspacex(int s){AxisTextSpaceX=s;};  /* Text space for X Axis Display */
  int axistextspacex(void){return(AxisTextSpaceX);};

  void axistextspacey(int s){AxisTextSpaceY=s;};  /* Text space for Y Axis Display */
  int axistextspacey(void){return(AxisTextSpaceY);};


  const char* version(void);            /* returns widgets current Version */
  
  
  
  void vlineenable(int e){VLineEnable=e;};        /* Enable or Disable Line        */
  int  vlineenable(void){return (VLineEnable);};
  
  void vlinesel(int l){VLineSel=l;};              /* Display for this line         */  
  int  vlinesel(void){return(VLineSel);};                    
  
  void vlinepos(unsigned long int p){VLinePos=p;};  /* Position in data to plot line and highlight */
  unsigned long int vlinepos(void){return(VLinePos);};
  
  
  
  
  
  

};



/******* END OF FILE ********/
#endif



