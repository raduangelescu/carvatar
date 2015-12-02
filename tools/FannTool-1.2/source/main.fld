# data file for the Fltk User Interface Designer (fluid)
version 1.0300 
header_name {.h} 
code_name {.cpp}
comment {//
// FannTool, GUI tool for ANN by using FANN library
// Programmed by BlueKid
// http://derindelimavi.blogspot.com/
// Send me any suggestion, modification or bugs. 
// Don't hesitate to contact  me for any question, 
// I will be very grateful with your feedbacks.
// bluekid70@gmail.com
// Copyright (C) 2008  BlueKid
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or  any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
} {in_source in_header
} 

decl {\#include <string>} {public local
} 

decl {\#include <math.h>} {public local
} 

decl {\#include <deque>} {public local
} 

decl {\#include "Fl_PlotXY.H"} {public local
} 

decl {\#include "DataProcess.h"} {public local
} 

decl {\#include <FL/fl_ask.H>} {public local
} 

decl {\#include <FL/Fl_File_Chooser.H>} {public local
} 

decl {\#include <FL/Fl_Native_File_Chooser.H>} {public local
} 

decl {\#include <FL/Fl_Float_Input.H>} {public local
} 

decl {\#include <FL/Fl_Chart.H>} {public local
} 

decl {\#include <FL/filename.H>} {public local
} 

decl {\#include <stdio.h>} {public local
} 

decl {\#include <fann.h>} {public local
} 

declblock {\#ifdef WIN32} {after {\#endif}
} {
  decl {\#include "fanntoolrc.h"} {private local
  }
  decl {extern HINSTANCE fl_display;} {private local
  }
  decl {\#elif !defined(__APPLE__)} {private local
  }
  decl {\#include <X11/xpm.h>} {private local
  }
  decl {\#include "fanntool_icon.xpm"} {private local
  }
  decl {extern Display *fl_display;} {private local
  }
} 

decl {int Act[13]={
	  FANN_LINEAR,
	  FANN_SIGMOID,
	  FANN_SIGMOID_STEPWISE,
	  FANN_SIGMOID_SYMMETRIC,
	  FANN_SIGMOID_SYMMETRIC_STEPWISE,
	  FANN_GAUSSIAN,
	  FANN_GAUSSIAN_SYMMETRIC,
	  FANN_ELLIOT,
	  FANN_ELLIOT_SYMMETRIC,
	  FANN_LINEAR_PIECE,
	  FANN_LINEAR_PIECE_SYMMETRIC,
	  FANN_SIN_SYMMETRIC,
	  FANN_COS_SYMMETRIC 
    };} {private local
} 

decl {int Line;} {private local
} 

comment {MinMSE and MinANN
Min Training MSE
Min Testing MSE
Min OPS MSE
Latest MSE
both Training and Testing MSE
} {in_source not_in_header
} 

decl {struct fann* MinANN[4];} {private local
} 

decl {double MinTrainingMSE[4];} {private local
} 

decl {double MinTestingMSE[4];} {private local
} 

decl {FannTool ft;} {private local
} 

decl {bool cascadeFirst;} {private local
} 

decl {DataProcess *rdp;} {private local
} 

decl {TimeSeri *tdp;} {private local
} 

decl {DataItem *itmp;} {private local
} 

decl {std::deque<float> cX;} {private local
} 

decl {std::deque<float> cY;} {private local
} 

decl {char fNameBuf[2048];} {private local
} 

decl {enum SelectType {Open,Save};} {public global
} 

decl {enum FileType {Data,Log,RawData,Network};} {public global
} 

Function {} {} {
  code {splashScreen(600);} {}
  Fl_Window window_main {
    label {FANN Tool 1.2} open
    xywh {310 33 775 684} type Double box PLASTIC_DOWN_BOX color 21 labelcolor 1 align 80 resizable
    code0 {Fl::visual(FL_DOUBLE|FL_INDEX);}
    code1 {if((window_main->w()> Fl::w())||(window_main->h()> Fl::h())){ window_main->resizable(window_main); window_main->resize(window_main->x(),window_main->y(),Fl::w(),Fl::h());}}
    code2 {window_main->callback(Exit_CB);} visible
  } {
    Fl_Menu_Bar {} {
      xywh {0 0 777 35} box GTK_UP_BOX color 29
    } {
      Submenu {} {
        label File
        xywh {0 0 62 20}
      } {
        MenuItem {} {
          label {Open Training Data}
          callback {ft.LoadTrainData();}
          xywh {0 0 30 20}
        }
        MenuItem {} {
          label {Open Test Data}
          callback {ft.LoadTestData();}
          xywh {10 10 30 20}
        }
        MenuItem {} {
          label {Open Log}
          callback {ft.LoadLog();}
          xywh {0 0 30 20}
        }
        MenuItem {} {
          label {Save Log}
          callback {ft.SaveLog();}
          xywh {10 10 30 20}
        }
        MenuItem {} {
          label {Clear Log}
          callback {ft.ClearLog();}
          xywh {20 20 30 20}
        }
        MenuItem {} {
          label Exit
          callback {Exit_CB(0,0);
//exit(0);}
          xywh {30 30 30 20}
        }
      }
      Submenu {} {
        label {Neural Network}
        xywh {0 0 62 20}
      } {
        Submenu {} {
          label Detect
          xywh {0 0 62 20}
        } {
          MenuItem {} {
            label {Optimum Training Algorithm}
            callback {ft.OptimumAlgorithm();}
            xywh {10 10 30 20}
          }
          MenuItem {} {
            label {Optimum Activation Functions}
            callback {ft.OptimumActivations();}
            xywh {20 20 30 20}
          }
        }
        Submenu {} {
          label Train
          xywh {0 0 62 20}
        } {
          MenuItem {} {
            label Normal
            callback {ft.TrainNormal();}
            xywh {0 0 30 20}
          }
          MenuItem {} {
            label Cascade
            callback {ft.TrainCascade();}
            xywh {10 10 30 20}
          }
        }
        MenuItem {} {
          label Test
          callback {ft.Test();}
          xywh {10 10 30 20}
        }
        Submenu {} {
          label Run
          xywh {10 10 62 20}
        } {
          MenuItem {} {
            label Normal
            callback {ft.RunNormal();}
            xywh {10 10 30 20}
          }
          MenuItem {} {
            label {with File}
            callback {ft.RunwithFile();}
            xywh {20 20 30 20}
          }
          MenuItem {} {
            label {as a Time Series}
            callback {ft.RunAsTS();}
            xywh {30 30 30 20}
          }
          MenuItem {} {
            label {as a Classifier}
            callback {ft.RunAsClassifier();}
            xywh {40 40 30 20}
          }
        }
        MenuItem {} {
          label Info
          callback {ft.NeuralNetworkInfo();}
          xywh {20 20 30 20}
        }
      }
      MenuItem {} {
        label {Data Processing}
        callback {ft.DataProcessing();}
        xywh {0 0 30 20}
      }
      Submenu {} {
        label Help
        xywh {5 5 62 20}
      } {
        MenuItem {} {
          label {About FannTool}
          callback {ft.About();}
          xywh {0 0 30 20}
        }
      }
    }
    Fl_Group {} {open
      xywh {5 91 765 229}
    } {
      Fl_Group {} {
        xywh {145 106 595 60}
      } {
        Fl_Input DataFile {
          label {Training Data File }
          xywh {145 108 420 23}
        }
        Fl_Input TestFile {
          label {Test Data File }
          xywh {145 143 420 23}
        }
        Fl_Repeat_Button {} {
          label {...}
          callback {// GetTrainData
ft.LoadTrainData();}
          xywh {575 106 20 25} box PLASTIC_UP_BOX color 4 labeltype ENGRAVED_LABEL
        }
        Fl_Repeat_Button {} {
          label {...}
          callback {// GetTestData
ft.LoadTestData();}
          xywh {575 141 20 25} box PLASTIC_UP_BOX color 4 labeltype ENGRAVED_LABEL
        }
        Fl_Value_Output Input {
          label {Input dim.:}
          xywh {690 107 50 24}
        }
        Fl_Value_Output Output {
          label {Output dim.:}
          xywh {690 142 50 24}
        }
      }
      Fl_Group {} {open
        xywh {293 181 477 129}
      } {
        Fl_Spinner Layer {
          label {\# of Layers}
          callback {if(Layer->value()==3){
 Hid2->deactivate();
 Hid3->deactivate(); 
}
else if(Layer->value()==4){
 Hid2->activate();
 Hid3->deactivate(); 
}
else if(Layer->value()==5){
 Hid2->activate();
 Hid3->activate();
} 
SetHiddens();}
          xywh {293 182 37 24} color 7 when 1
          code0 {o->value(3);}
          code1 {o->maximum(5);}
          code2 {o->minimum(3);}
        }
        Fl_Spinner Hid1 {
          label {Hid. Layer 1 :}
          xywh {432 182 45 24} maximum 999
          code0 {o->value(0);}
        }
        Fl_Spinner Hid2 {
          label {Hid. Layer 2 :}
          xywh {575 182 45 24} maximum 999 deactivate
          code0 {o->value(0);}
        }
        Fl_Spinner Hid3 {
          label {Hid. Layer 3 :}
          xywh {715 182 45 24} maximum 999 deactivate
          code0 {o->value(0);}
        }
        Fl_Choice Method {
          label {Training Method :} open
          xywh {430 215 320 25} down_box BORDER_BOX
          code0 {FillMethods();}
          code1 {o->value(2);}
        } {}
        Fl_Choice HiddenActivationF {
          label {Activation Function (Hidden) :} open
          xywh {430 250 320 25} down_box BORDER_BOX
        } {}
        Fl_Choice OutputActivationF {
          label {Activation Function (Output) :} open
          xywh {430 285 320 25} down_box BORDER_BOX
          code0 {FillActivationF();}
          code1 {o->value(3);}
          code2 {HiddenActivationF->value(3);}
        } {}
      }
      Fl_Group {} {
        xywh {10 176 205 144} box EMBOSSED_FRAME align 0
      } {
        Fl_Choice StopFunction {
          label {Stop Function} open
          xywh {20 196 190 22} down_box BORDER_BOX align 5
          code0 {StopFunction->add("FANN_STOPFUNC_MSE");}
          code1 {StopFunction->add("FANN_STOPFUNC_BIT");}
          code2 {StopFunction->value(0);}
        } {}
        Fl_Value_Input EReports {
          label {Epochs Between Reports}
          xywh {20 242 55 23} align 5 minimum 1 maximum 50000 step 10 value 1000
        }
        Fl_Value_Input MaxEpoch {
          label {Max \# of Epochs : }
          callback {/*
if(MaxEpoch->value()>=1000)
  MaxEpoch->step(100);  
if(MaxEpoch->value()>=10000)
  MaxEpoch->step(1000);  
if(MaxEpoch->value()>=100000)
  MaxEpoch->step(10000);
  
*/}
          xywh {20 286 60 24} align 5 minimum 1000 maximum 1e+006 step 1000 value 500000
        }
      }
    }
    Fl_Tabs Tabs {
      xywh {0 324 776 381} box GTK_UP_FRAME color 21
      code0 {o->value(LogG);}
    } {
      Fl_Group LogG {
        label Log open
        xywh {5 367 760 338}
      } {
        Fl_Browser Out {
          xywh {10 372 755 301} type Hold box GTK_DOWN_BOX color 31 labelcolor 2
        }
      }
      Fl_Group GraphG {
        label Graphic open
        xywh {1 365 775 340} hide
      } {
        Fl_Box Graph {
          tooltip {MSE vs Epoch} xywh {11 371 746 273} box GTK_DOWN_BOX color 16
          code0 {Line=Graph->newline();}
          code1 {Graph->scalemode(Line,FL_PLOTXY_AUTO);}
          code2 {Graph->linecolor(Line,FL_BLUE);}
          class Fl_PlotXY
        }
        Fl_Value_Output Ep {
          label {Epoch :}
          xywh {63 651 60 24} labelcolor 4
        }
        Fl_Value_Output Mse {
          label {MSE :}
          xywh {174 651 110 24} labelcolor 4
        }
        Fl_Spinner nVData {
          label {\# of Visible Data :}
          xywh {712 651 48 24} labelcolor 4 minimum 20 maximum 500 value 500
        }
        Fl_Value_Output BitFailOut {
          label {Bit Fail  :}
          xywh {549 651 44 24} labelcolor 4
        }
        Fl_Value_Output TestMse {
          label {Test MSE :}
          tooltip {Test Data MSE} xywh {375 651 110 24} labelcolor 1 deactivate
        }
      }
      Fl_Group {} {
        label {Fine Tuning} open
        xywh {10 370 755 335} hide
      } {
        Fl_Value_Input DesiredError {
          label {Desired Error (MSE) :}
          xywh {169 401 72 24} maximum 0.1 step 1e-005 value 0.0001
        }
        Fl_Value_Input BitFail {
          label {Bit Fail Limit :}
          xywh {170 439 72 24} maximum 0.1 step 1e-005 value 0.035
        }
        Fl_Choice ErrorFunction {
          label {Error Function} open
          xywh {331 398 225 24} down_box BORDER_BOX align 5
          code0 {ErrorFunction->add("FANN_ERRORFUNC_LINEAR");}
          code1 {ErrorFunction->add("FANN_ERRORFUNC_TANH");}
          code2 {ErrorFunction->value(0);}
        } {}
        Fl_Value_Input HiddenStepness {
          label {Hidden Activation Steepness  }
          xywh {335 443 67 22} align 5 maximum 0 value 0.5
        }
        Fl_Value_Input OutputStepness {
          label {Output Activation Steepness  }
          xywh {335 491 67 24} align 5 maximum 0 value 0.5
        }
        Fl_Value_Input DecayFactor {
          label {Quickprop Decay Factor}
          xywh {584 398 67 24} align 5 minimum -0.2 maximum 0 value -0.0001
        }
        Fl_Value_Input MuFactor {
          label {Quickprop Mu Factor}
          xywh {584 441 70 24} align 5 minimum 1 maximum 3 value 1.75
        }
        Fl_Value_Input IncreaseFactor {
          label {RPROP Increase Factor }
          xywh {584 491 70 24} align 5 minimum 1 maximum 3 value 1.2
        }
        Fl_Value_Input DecreaseFactor {
          label {RPROP Decrease Factor }
          xywh {584 541 70 24} align 5 value 0.5
        }
        Fl_Value_Input DeltaMin {
          label {RPROP Minimum Step-Size}
          xywh {584 596 70 24} align 5 maximum 3
        }
        Fl_Value_Input DeltaMax {
          label {RPROP Maximum Step-Size}
          xywh {584 646 70 24} align 5 minimum 1 maximum 100 value 50
        }
        Fl_Counter ConnectionRate {
          label {Connection Rate}
          xywh {135 489 142 24} align 4 minimum 0.1 maximum 1 value 1
        }
        Fl_Counter Momentum {
          label {Momentum :}
          xywh {415 526 140 25} align 4 minimum 0 maximum 1
        }
        Fl_Check_Button Shuffle {
          label {Shuffle Train Data}
          xywh {453 556 26 25} down_box DOWN_BOX align 4
        }
        Fl_Check_Button InitWghts {
          label {Initialize the weights ( Widrow + Nguyen Alg.)}
          xywh {300 557 19 24} down_box DOWN_BOX align 4
        }
        Fl_Check_Button OverTraining {
          label {Overtraining Caution System}
          callback {if(OverTraining->value()){
  ft.overtraining=true;
  TestMse->activate();
}
else{
  ft.overtraining=false;
  TestMse->value(0);
  TestMse->deactivate();
  
}}
          xywh {300 591 19 24} down_box DOWN_BOX align 4 deactivate
        }
        Fl_Spinner LearningRate {
          label {Learning Rate}
          xywh {140 526 80 24} type Float minimum 0.001 maximum 1 step 0.1 value 0.7
        }
      }
      Fl_Group {} {
        label {Cascade Tuning}
        xywh {7 370 750 335} hide
      } {
        Fl_Value_Input OutputChange {
          label {Output Change Fraction:}
          xywh {51 476 93 24} align 5 value 0.01
        }
        Fl_Value_Input OutputStag {
          label {Output Stagnation Epochs}
          xywh {51 526 93 24} align 5 maximum 0 value 12
        }
        Fl_Value_Input CandidateChange {
          label {Candidate Change Fraction:}
          xywh {51 576 93 24} align 5 value 0.01
        }
        Fl_Value_Input CandidateStag {
          label {Candidate Stagnation Epochs}
          xywh {51 626 93 24} align 5 maximum 0 value 12
        }
        Fl_Value_Input WeighMultiplier {
          label {Weight Multiplier}
          xywh {301 421 93 24} align 5 value 0.4
        }
        Fl_Value_Input CandidateLimit {
          label {Candidate Limit}
          xywh {301 471 93 24} align 5 maximum 2000 value 1000
        }
        Fl_Value_Input MaxOutEpoch {
          label {Maximum Out Epochs}
          xywh {301 521 93 24} align 5 maximum 500 value 150
        }
        Fl_Value_Input MaxCasndidatetEpoch {
          label {Maximum Candidate Epochs}
          xywh {301 571 93 24} align 5 maximum 500 value 150
        }
        Fl_Value_Input NumCandidateGroups {
          label {Number of Candidate Groups}
          xywh {301 616 93 24} align 5 maximum 0 value 2
        }
        Fl_Value_Input MaxCascade {
          label {Maximum Number of Neurons}
          xywh {51 426 93 24} align 5 minimum 1 maximum 0 value 10
        }
      }
    }
    Fl_Group {} {open
      xywh {10 42 494 56} box EMBOSSED_FRAME
    } {
      Fl_Button But_Alg {
        callback {ft.OptimumAlgorithm();}
        tooltip {Detect Optimum Training Algorithm} image {img/alg.png} xywh {20 56 32 32} box PLASTIC_UP_BOX down_box FLAT_BOX color 4 align 128 deactivate
        code0 {InactImg(o);}
      }
      Fl_Button But_Act {
        callback {ft.OptimumActivations();}
        tooltip {Detect Optimum Activation Functions} image {img/act.png} xywh {60 56 32 32} box PLASTIC_UP_BOX color 4 align 128 deactivate
        code0 {InactImg(o);}
      }
      Fl_Menu_Button But_Trn {
        callback {if(But_Trn->value()==0)
  ft.TrainNormal();
else if(But_Trn->value()==1)
  ft.TrainCascade();} open
        tooltip {Train ANN} image {img/train.png} xywh {100 56 32 32} box PLASTIC_UP_BOX color 4 deactivate
        code0 {o->add("Normal");}
        code1 {o->add("Cascade");}
        code2 {InactImg(o);}
      } {}
      Fl_Button But_Test {
        callback {ft.Test();}
        tooltip {Test ANN} image {img/test.png} xywh {140 56 32 32} box PLASTIC_UP_BOX color 4 align 128 deactivate
        code0 {InactImg(o);}
      }
      Fl_Menu_Button But_Run {
        callback {if(But_Run->value()==0)
  ft.RunNormal();
else if(But_Run->value()==1)
  ft.RunwithFile();  
else if(But_Run->value()==2)
  ft.RunAsTS();
else 
  ft.RunAsClassifier();}
        tooltip {Run ANN} image {img/Run.png} xywh {180 56 32 32} box PLASTIC_UP_BOX color 4
        code0 {o->add("Normal");}
        code1 {o->add("with File");o->add("As a Time Series");o->add("As a Classifier");}
        code2 {InactImg(o);}
      } {}
      Fl_Button {} {
        callback {ft.ClearLog();}
        tooltip {Clear Log} image {img/clear.png} xywh {220 56 32 32} box PLASTIC_UP_BOX color 4 align 128
        code0 {InactImg(o);}
      }
      Fl_Button {} {
        callback {ft.SaveLog();}
        tooltip {Save Log} image {img/save-2.png} xywh {260 56 32 32} box PLASTIC_UP_BOX color 4 align 128
        code0 {InactImg(o);}
      }
      Fl_Button Stop {
        callback {ft.StopProcess();}
        tooltip {Stop Process} image {img/stop.png} xywh {300 56 32 32} box PLASTIC_UP_BOX color 4 labelcolor 1 align 128 deactivate
        code0 {InactImg(o);}
      }
      Fl_Button {} {
        callback {ft.LoadLog();}
        tooltip {Load  Log} image {img/load.png} xywh {340 56 32 32} box PLASTIC_UP_BOX color 4 align 128
        code0 {InactImg(o);}
      }
      Fl_Button {} {
        callback {ft.About();}
        tooltip Help image {img/Help.png} xywh {380 56 32 32} box PLASTIC_UP_BOX color 4 align 128
        code0 {InactImg(o);}
      }
      Fl_Button {} {
        callback {ft.DataProcessing();}
        tooltip {Data Processing} image {img/Data.png} xywh {420 56 32 32} box PLASTIC_UP_BOX color 4 align 128
        code0 {InactImg(o);}
      }
      Fl_Button {} {
        callback {ft.NeuralNetworkInfo();}
        tooltip {Neural Network Information} image {img/Information.png} xywh {462 56 32 32} box PLASTIC_UP_BOX color 4 align 128
        code0 {InactImg(o);}
      }
    }
  }
  code {\#ifdef WIN32
  window_main->icon((char *)LoadIcon(fl_display, MAKEINTRESOURCE(IDI_ICON)));
\#elif !defined(__APPLE__)
  Pixmap p, mask;
  XpmCreatePixmapFromData(fl_display, DefaultRootWindow(fl_display),fanntool_icon_xpm, &p, &mask, NULL);
  window_main->icon((char *)p);  

\#endif // WIN32} {}
} 

Function {cb_Ok(Fl_Return_Button*o, void*w)} {} {
  code {((Fl_Window *)(o->parent()))->hide();} {}
} 

Function {Refresh()} {return_type {inline void}
} {
  code {Out->bottomline(Out->size());
Out->redraw();
Graph->redraw();
Ep->redraw();
Mse->redraw();
Fl::wait(0);} {}
} 

Function {FillMethods()} {} {
  code {/*
FANN_TRAIN_INCREMENTAL = 0,
FANN_TRAIN_BATCH,
FANN_TRAIN_RPROP,
FANN_TRAIN_QUICKPROP
FANN_TRAIN_SARPROP
*/	
	
Method->add("FANN_TRAIN_INCREMENTAL");
Method->add("FANN_TRAIN_BATCH");
Method->add("FANN_TRAIN_RPROP");
Method->add("FANN_TRAIN_QUICKPROP");
Method->add("FANN_TRAIN_SARPROP");} {}
} 

Function {FillActivationF()} {} {
  code {char *ActF[13]={
	  "FANN_LINEAR",
	  "FANN_SIGMOID",
	  "FANN_SIGMOID_STEPWISE",
	  "FANN_SIGMOID_SYMMETRIC",
	  "FANN_SIGMOID_SYMMETRIC_STEPWISE",
	  "FANN_GAUSSIAN",
	  "FANN_GAUSSIAN_SYMMETRIC",
	  "FANN_ELLIOT",
	  "FANN_ELLIOT_SYMMETRIC",
	  "FANN_LINEAR_PIECE",
	  "FANN_LINEAR_PIECE_SYMMETRIC",
	  "FANN_SIN_SYMMETRIC",
	  "FANN_COS_SYMMETRIC" 

    };
    
    
    for(int i=0;i<13;i++){
      HiddenActivationF->add(ActF[i]);
      OutputActivationF->add(ActF[i]);      
    }} {}
} 

Function {GetWeigths(struct fann *ann)} {return_type {fann_type *}
} {
  code {fann_type *w;
w=(fann_type *)malloc(ann->total_connections*sizeof(fann_type));
for (unsigned int i=0; i < ann->total_connections; i++)
   w[i] = ann->weights[i];
return w;} {}
} 

Function {SetWeights(struct fann *ann,fann_type *w)} {return_type void
} {
  code {for (unsigned int i=0; i < ann->total_connections; i++)
      ann->weights[i]=w[i];} {}
} 

Function {ExamineTrain(struct fann *ann,fann_train_enum tal,fann_activationfunc_enum hact,fann_activationfunc_enum oact,fann_train_data *TrainData)} {return_type fann_type
} {
  code {fann_set_training_algorithm(ann,tal);
fann_set_activation_function_hidden(ann, hact);
fann_set_activation_function_output(ann, oact);
fann_set_callback(ann, LogOut );
// inith Graph
cX.clear();
cY.clear();
//
fann_train_on_data(ann, TrainData, 2000, 250, 0.0);

  double trainMSE=fann_get_MSE(ann);
  double testMSE=-1;
  if(ft.TestData && ft.overtraining){
    fann_reset_MSE(ann);
    fann_test_data(ann,ft.TestData);
    testMSE=fann_get_MSE(ann);
    return (trainMSE+testMSE)/2;
    
  }
  else
    return trainMSE;} {}
} 

Function {LogOut(struct fann *ann, struct fann_train_data *train,unsigned int max_epochs, unsigned int epochs_between_reports,float desired_error, unsigned int epochs)} {return_type {int FANN_API}
} {
  code {char Buf[512];
  double trainMSE=fann_get_MSE(ann);
  double testMSE=-1;
  unsigned int newBitFail=fann_get_bit_fail(ann);
  if(ft.TestData && ft.overtraining){
    fann_reset_MSE(ann);
    fann_test_data(ann,ft.TestData);
    testMSE=fann_get_MSE(ann);
    fann_test_data(ann,ft.TrainData);
    sprintf(Buf,"%08d : %f       : %f       : %d ", epochs,trainMSE ,testMSE,newBitFail);
  }
  else
    sprintf(Buf,"%08d : %f       : %d ", epochs,trainMSE , newBitFail);
  Out->add(Buf);  
  // Memorizing Begin
     if(epochs==1){
       for(int i=0;i<3;i++) {
         MinTrainingMSE[i]=trainMSE;
         MinANN[i]=fann_copy(ann);
         if(ft.TestData && ft.overtraining)
            MinTestingMSE[i]=testMSE;
       }
       MinANN[3]=ann;
       
     }

     // Latest 
     MinTrainingMSE[3]= trainMSE;
     MinTestingMSE[3]= testMSE; 
//     if( MinANN[3]) fann_destroy( MinANN[3]);
//       MinANN[3]=fann_copy(ann);
  
     // Minimum Training MSE
     if(MinTrainingMSE[0]> trainMSE ){
       if( MinANN[0]) fann_destroy( MinANN[0]);
       MinANN[0]=fann_copy(ann);
       MinTrainingMSE[0]=trainMSE;
       if(ft.TestData && ft.overtraining)
           MinTestingMSE[0]=testMSE;
       
     }
     if(ft.TestData && ft.overtraining){
       // Minimum Testing MSE
       if(MinTestingMSE[1]> testMSE ){
         if( MinANN[1]) fann_destroy( MinANN[1]);
         MinANN[1]=fann_copy(ann);
         MinTrainingMSE[1]=trainMSE;
         MinTestingMSE[1]=testMSE;

       }
       // Minimum (Training MSE + Testing MSE )/2 
       if((MinTestingMSE[2]+ MinTrainingMSE[2])> (trainMSE + testMSE) ){
         if( MinANN[2]) fann_destroy( MinANN[2]);
         MinANN[2]=fann_copy(ann);
         MinTrainingMSE[2]=trainMSE;
         MinTestingMSE[2]=testMSE;

       }       
       
         
     }     
  
    // Memorizing End

  
  DrawGraph(epochs,trainMSE,testMSE,newBitFail);  
  Refresh();
  if(ft.stop)
    return -1;
  return 1;} {}
} 

Function {CascadeLogOut(struct fann *ann, struct fann_train_data *train,unsigned int max_epochs, unsigned int epochs_between_reports,float desired_error, unsigned int epochs)} {return_type {int FANN_API}
} {
  code {char Buf[512];

double trainMSE=fann_get_MSE(ann);
double testMSE=-1;
unsigned int newBitFail=fann_get_bit_fail(ann);

if(ft.TestData && ft.overtraining){
    fann_reset_MSE(ann);
    fann_test_data(ann,ft.TestData);
    testMSE=fann_get_MSE(ann);
    fann_test_data(ann,ft.TrainData);
    sprintf(Buf,"%08d : %f       : %f       : %d ", epochs,trainMSE ,testMSE ,newBitFail);   
}
else
  sprintf(Buf,"%08d : %f       : %d  ", epochs,trainMSE , newBitFail);
/*
sprintf(Buf,"Current error: %.6f. Total error:%8.4f. Epochs %5d. Bit fail %3d",
					 newMSE, ann->MSE_value, epochs, ann->num_bit_fail);
					 
	*/
					 
Out->add(Buf);					 
					 
if((ann->last_layer-2) != ann->first_layer)
{
  sprintf(Buf,"@C4Candidate steepness %.2f. function %s", 
     (ann->last_layer-2)->first_neuron->activation_steepness,
     FANN_ACTIVATIONFUNC_NAMES[(ann->last_layer-2)->first_neuron->activation_function]);
  Out->add(Buf);     
}


  // Memorizing Begin
     if(cascadeFirst){
       for(int i=0;i<3;i++) {
         MinTrainingMSE[i]=trainMSE;
         MinANN[i]=fann_copy(ann);
         if(ft.TestData && ft.overtraining)
            MinTestingMSE[i]=testMSE;
       }
       MinANN[3]=ann;
       cascadeFirst=false;
       
     }

     // Latest 
     MinTrainingMSE[3]= trainMSE;
     MinTestingMSE[3]= testMSE; 
//     if( MinANN[3]) fann_destroy( MinANN[3]);
//       MinANN[3]=fann_copy(ann);
  
     // Minimum Training MSE
     if(MinTrainingMSE[0]> trainMSE ){
       if( MinANN[0]) fann_destroy( MinANN[0]);
       MinANN[0]=fann_copy(ann);
       MinTrainingMSE[0]=trainMSE;
       if(ft.TestData && ft.overtraining)
           MinTestingMSE[0]=testMSE;
       
     }
     if(ft.TestData && ft.overtraining){
       // Minimum Testing MSE
       if(MinTestingMSE[1]> testMSE ){
         if( MinANN[1]) fann_destroy( MinANN[1]);
         MinANN[1]=fann_copy(ann);
         MinTrainingMSE[1]=trainMSE;
         MinTestingMSE[1]=testMSE;

       }
       // Minimum (Training MSE + Testing MSE )/2 
       if((MinTestingMSE[2]+ MinTrainingMSE[2])> (trainMSE + testMSE) ){       
         if( MinANN[2]) fann_destroy( MinANN[2]);
         MinANN[2]=fann_copy(ann);
         MinTrainingMSE[2]=trainMSE;
         MinTestingMSE[2]=testMSE;

       }       
       
         
     }     
  
    // Memorizing End



DrawGraph(epochs,trainMSE,testMSE,ann->num_bit_fail);

Refresh();
if(ft.stop) 
 return -1;

return 1;} {}
} 

Function {DrawGraph(float epochs, float TrainMSE, float TestMSE,unsigned int newBitFail)} {return_type {int FANN_API}
} {
  code {if(epochs < 2){
    cX.clear();
    cY.clear();  
  }

  Graph->clear(Line);

  cX.push_back(epochs);

  if(TestMSE==-1)
    cY.push_back(TrainMSE);
  else
    cY.push_back((TrainMSE+TestMSE)/2);
  

  while(cX.size()>=(int)nVData->value()){
    cX.pop_front();
    cY.pop_front();
  }

  for (int i=1; i<cX.size(); ++i) {
    Graph->add(Line,cX[i],cY[i]);
  }

  Graph->redraw();

  Ep->value(epochs);
  Mse->value(TrainMSE);
  if(TestMSE!=-1)
    TestMse->value(TestMSE);
  BitFailOut->value(newBitFail);} {}
} 

Function {SetHiddens()} {return_type void
} {
  code {int tmp,fark;

fark=(int )(Input->value()-Output->value());
if(fark > 0 ) {
  if(Layer->value()==3){
    tmp=fark/2;
    Hid1->value(tmp);
    Hid2->value(0);
    Hid3->value(0);
  }
  else if(Layer->value()==4){
    tmp=fark/3;
    Hid1->value(Input->value()-tmp);
    Hid2->value(Hid1->value()-tmp);
    Hid3->value(0);
  }
  else if(Layer->value()==5){
    tmp=fark/4;
    Hid1->value(Input->value()-tmp);
    Hid2->value(Hid1->value()-tmp);
    Hid3->value(Hid2->value()-tmp);

  }
  if(Hid1->value()==0) Hid1->value(1);
  if(Hid2->value()==0) Hid2->value(1);
  if(Hid3->value()==0) Hid3->value(1);

}
else{
 Hid1->value(1);
 Hid2->value(1);
 Hid3->value(1);
}} {}
} 

Function {InactImg(Fl_Widget* o)} {} {
  code {Fl_Image *r=o->image();
r=r->copy(r->w(),r->h());
r->inactive();
o->deimage(r);} {}
} 

class FannTool {open
} {
  decl {struct fann *ysa;} {public local
  }
  decl {fann_type *result;} {public local
  }
  decl {fann_type *data;} {public local
  }
  decl {bool working;} {public local
  }
  decl {bool stop;} {public local
  }
  decl {bool overtraining;} {public local
  }
  decl {fann_train_data *TrainData} {public local
  }
  decl {fann_train_data *TestData} {public local
  }
  Function {FannTool()} {} {
    code {stop=false;
overtraining=false;
working=false;
TrainData=NULL;
TestData=NULL;
for(int i=0;i<4;i++){
  MinANN[i]=NULL; 
}} {}
  }
  Function {OptimumAlgorithm()} {} {
    code {if(TrainData==NULL){
  fl_alert("Firstly Load Train Data !");
  return;
}
if(working) return;
char Buf[512];
Out->clear();
ActivateStop();
if(Layer->value()==5)
  ysa = fann_create_sparse(ConnectionRate->value(),5,(int)Input->value(),(int)Hid1->value(),(int)Hid2->value(),(int)Hid3->value(),(int)Output->value());
if(Layer->value()==4)
  ysa = fann_create_sparse(ConnectionRate->value(),4,(int)Input->value(),(int)Hid1->value(),(int)Hid2->value(),(int)Output->value());  
else
  ysa = fann_create_sparse(ConnectionRate->value(),3,(int)Input->value(),(int)Hid1->value(),(int)Output->value());
fann_type *w=GetWeigths(ysa);
int best_ta;
fann_type min=1,mse;
for(int  ta=FANN_TRAIN_INCREMENTAL; ta<=FANN_TRAIN_SARPROP ;ta++) {
    SetWeights(ysa,w);
    sprintf(Buf,"@C4Train Method : %s ",FANN_TRAIN_NAMES[ta]);
    Out->add(Buf);
    Out->redraw();
    mse=ExamineTrain(ysa,(fann_train_enum)ta,(fann_activationfunc_enum)HiddenActivationF->value(),(fann_activationfunc_enum)OutputActivationF->value(),TrainData);
    if(mse<min){
       min=mse;
       best_ta=ta;
    }
    if(stop) break; 
}
if(stop==false){
  sprintf(Buf,"@C1Best Train Method : %s ",FANN_TRAIN_NAMES[best_ta]);
  Method->value(best_ta);
  Out->add(Buf);
}
else
  Out->add("@C1Process Stopped !");
Out->bottomline(Out->size());
Out->redraw();
fann_destroy(ysa);
DeactivateStop();} {}
  }
  Function {OptimumActivations()} {open
  } {
    code {if(TrainData==NULL){
  fl_alert("Firstly Load Train Data !");
  return;
}
if(working) return;
int best_ha,best_oa;
Out->clear();
ActivateStop();
if(Layer->value()==5)
  ysa = fann_create_sparse(ConnectionRate->value(),5,(int)Input->value(),(int)Hid1->value(),(int)Hid2->value(),(int)Hid3->value(),(int)Output->value());
if(Layer->value()==4)
  ysa = fann_create_sparse(ConnectionRate->value(),4,(int)Input->value(),(int)Hid1->value(),(int)Hid2->value(),(int)Output->value());  
else
  ysa = fann_create_sparse(ConnectionRate->value(),3,(int)Input->value(),(int)Hid1->value(),(int)Output->value());
fann_type *w=GetWeigths(ysa);

int best_ta;
fann_type min=1,mse;
char Buf[512];
for(int  i=0; i<13 && !stop;i++) {
  for(int  j=0; j<13&& !stop;j++) {
  SetWeights(ysa,w);
    sprintf(Buf,"@C4Hid Activation Func. : %s --- Out Activation Func. : %s ",FANN_ACTIVATIONFUNC_NAMES[Act[i]],FANN_ACTIVATIONFUNC_NAMES[Act[j]]);
    Out->add(Buf);
    mse=ExamineTrain(ysa,(fann_train_enum)Method->value(),(fann_activationfunc_enum) Act[i],(fann_activationfunc_enum) Act[j],TrainData);
    if(mse<min){
       min=mse;
       best_ha=i;
       best_oa=j;

    }
  }
}

  if(stop)
    Out->add("@C1Process Stopped !");

  sprintf(Buf,"@C1Best  Hid. Activation Func. : %s --- Out. Activation Func. : %s ",FANN_ACTIVATIONFUNC_NAMES[Act[best_ha]],FANN_ACTIVATIONFUNC_NAMES[Act[best_oa]]);
  Out->add(Buf);
  HiddenActivationF->value(best_ha);
  OutputActivationF->value(best_oa);

Out->bottomline(Out->size());
Out->redraw();
fann_destroy(ysa);
DeactivateStop();} {selected
    }
  }
  Function {TrainNormal()} {} {
    code {if(TrainData==NULL){
  fl_alert("Firstly Load Train Data !");
  return;
}
if(working) return;
char Buf[512];
Out->clear();
Fl::wait(0);    
ActivateStop();
Out->add("@C4Normal Training...");
Tabs->value(LogG);
Tabs->redraw();

Refresh();
 ClearTrainingMemory();
if(Layer->value()==5)
  ysa = fann_create_sparse(ConnectionRate->value(),5,(int)Input->value(),(int)Hid1->value(),(int)Hid2->value(),(int)Hid3->value(),(int)Output->value());
else if(Layer->value()==4)
  ysa = fann_create_sparse(ConnectionRate->value(),4,(int)Input->value(),(int)Hid1->value(),(int)Hid2->value(),(int)Output->value());  
else
  ysa = fann_create_sparse(ConnectionRate->value(),3,(int)Input->value(),(int)Hid1->value(),(int)Output->value());


fann_set_training_algorithm(ysa,(fann_train_enum)Method->value());
fann_set_activation_function_hidden(ysa,(fann_activationfunc_enum)Act[HiddenActivationF->value()]);
fann_set_activation_function_output(ysa,(fann_activationfunc_enum)Act[OutputActivationF->value()]);
fann_set_learning_rate(ysa,LearningRate->value());
fann_set_learning_momentum(ysa,Momentum->value());
fann_set_callback(ysa, LogOut );

//

if(Shuffle->value()){
  Out->add("\# Shuffling...");
  fann_shuffle_train_data(TrainData);
}

if(InitWghts->value()){
 
  Out->add("\# Initializing the weights using Widrow + Nguyen?s algorithm...");
  fann_init_weights(ysa,TrainData);
}

//
float err;
fann_set_train_stop_function(ysa,(fann_stopfunc_enum)StopFunction->value() ); 

sprintf(Buf,"\# Stop Function setted as  %s ",FANN_STOPFUNC_NAMES[StopFunction->value()]);
Out->add(Buf);
if(StopFunction->value()==0)
 err=DesiredError->value();
else
 err=BitFail->value();

SetFineTuning();
// inith Graph
cX.clear();
cY.clear();
//

if(ft.TestData && ft.overtraining)
  Out->add("Epoch       : Training MSE : Testing MSE  :  Bit Fail ");
else
  Out->add("Epoch       : Training MSE : Bit Fail ");


fann_train_on_data(ysa, TrainData, (int)MaxEpoch->value(),(int)EReports->value(),err );


if(stop){
   Out->add("@C1Process Stopped !");
   if(fl_choice("Training stopped !\\n  Do you want to save ANN file ?", "No","Yes",NULL))
      SaveANN(); 
}
else 
   SaveANN(); 

Refresh();
// fann_destroy(ysa);
DeactivateStop();} {}
  }
  Function {TrainCascade()} {} {
    code {if(TrainData==NULL){
  fl_alert("Firstly Load Train Data !");
  return;
}
if(working) return;
Out->clear();

ActivateStop();

Tabs->value(LogG);
Tabs->redraw();

Out->add("@C4Cascade Training...");
Refresh();
  ClearTrainingMemory();
double desired_error=DesiredError->value();
// unsigned int max_neurons = Input->value()+Hid1->value()+Hid2->value()+Hid3->value()+Output->value();
unsigned int max_neurons = (unsigned int )MaxCascade->value();
unsigned int neurons_between_reports = 1;

ysa = fann_create_shortcut(2, (int)Input->value(), (int)Output->value());
fann_set_training_algorithm(ysa,(fann_train_enum)Method->value());
fann_set_activation_function_hidden(ysa,(fann_activationfunc_enum)Act[HiddenActivationF->value()]);
fann_set_activation_function_output(ysa,(fann_activationfunc_enum)Act[OutputActivationF->value()]);
fann_set_callback(ysa, CascadeLogOut);
//
// fann_set_train_error_function(ysa, FANN_ERRORFUNC_LINEAR);
SetFineTuning();
SetCascadeTuning();
// init graph
cX.clear();
cY.clear(); 
//

if(ft.TestData && ft.overtraining)
  Out->add("Epoch       : Training MSE : Testing MSE  :  Bit Fail ");
else
  Out->add("Epoch       : Training MSE : Bit Fail ");
  

  
cascadeFirst=true;
fann_cascadetrain_on_data(ysa, TrainData, max_neurons, neurons_between_reports, desired_error);

//
if(stop){
   Out->add("@C1Process Stopped !");
   if(fl_choice("Training stopped !\\n  Do you want to save ANN file ?", "No","Yes",NULL))
      SaveANN(); 
}
else 
   SaveANN(); 

 // fann_destroy(ysa);
Refresh();
DeactivateStop();} {}
  }
  Function {Test()} {} {
    code {if(TestData==NULL){
  fl_alert("Firstly Load Test Data !");
  return;
}
if(working) return;
char Buf[512];
char* file=NULL;
Tabs->value(LogG);
Tabs->redraw();
Out->clear();
file = FileSelect("Load Trained ANN", Network);
if(file!=NULL){
   Out->add("Trained ANN Loaded...");   
   ysa=fann_create_from_file(fNameBuf);

   Out->add("@C4ANN Testing...");
   fann_reset_MSE(ysa);
   fann_test_data(ysa,TestData);

   sprintf(Buf,"Test Result MSE : %f ", fann_get_MSE(ysa) );
   Out->add(Buf);
 

}} {}
  }
  Function {Run()} {} {
    code {char Buf[512];
result=fann_run(ysa, data);

for(int i=0;i<ysa->num_input;i++){
  sprintf(Buf,"Input %03d : %f",i,data[i]);
  Out->add(Buf);

}

for(int i=0;i<ysa->num_output;i++){
  sprintf(Buf,"Output %03d : %f",i,result[i]);
  Out->add(Buf);

}} {}
  }
  Function {RunNormal()} {} {
    code {if(working) return;
char Buf[512];
char* file=NULL;
Out->clear();
file = FileSelect("Load Trained ANN", Network);
// file = fl_file_chooser("Load Trained ANN", "*.net", file);
if(file!=NULL){
   Out->add("Trained ANN Loaded...");   
   ysa=fann_create_from_file(fNameBuf);

   Out->add("@C4Give Inputs...");
//

  


//
   GetInputs *op=new GetInputs();
   op->show(ysa);


 

}} {}
  }
  Function {RunwithFile()} {} {
    code {if(working) return;
char Buf[512];
char* file=NULL;
Out->clear();

if(TestData==NULL)
   LoadTestData();
file = FileSelect("Load Trained ANN", Network);   
//file = fl_file_chooser("Load Trained ANN", "*.net", file);
if(file!=NULL){
   Out->add("Trained ANN Loaded...");
   ysa=fann_create_from_file(fNameBuf);
   Out->add("@C4Running with Test Data File...");
   Out->add("Data No : Calculated Outputs : Desired Outputs ");   


char Buf[512];
std::string ln1,ln2;

Tabs->value(LogG);
Tabs->redraw();

for(int i=0;i<TestData->num_data;i++){
  result=fann_run(ysa, TestData->input[i]);
//  sprintf(Buf,"%04d : ",i+1);
  sprintf(Buf,"%08d ",i+1);
//  Out->add(Buf);
  ln1=Buf;
  ln2="";
  for(int j=0;j<ysa->num_output;j++){
    sprintf(Buf,"%0.04f ",result[j]);
//    sprintf(Buf,"%+e ",result[j]);
    ln1=ln1+Buf;
    sprintf(Buf,"%0.04f ",TestData->output[i][j]);
//    sprintf(Buf,"%+e ",TestData->output[i][j]);
    ln2=ln2+Buf;

  }
  ln1=ln1+" : "+ln2;
//  ln1=ln1+ln2;
  Out->add(ln1.c_str());

}

}} {}
  }
  Function {RunAsTS()} {} {
    code {if(working) return;
char Buf[512];
char* file=NULL;
Out->clear();
file = FileSelect("Load Trained ANN", Network);
// file = fl_file_chooser("Load Trained ANN", "*.net", file);
if(file!=NULL){

   ysa=fann_create_from_file(fNameBuf);
   int noutputs=fann_get_num_output(ysa);
   if(noutputs!=1){
       fl_alert("Error Time Series Runner work with 1 output ");   
       fann_destroy(ysa);
       return;
   
   }
   


   Tabs->value(LogG);
   Tabs->redraw();

   Out->add("Trained ANN Loaded...");   
   Out->add("@C4Give Inputs...");
//

  


//
   GetInputs *op=new GetInputs(true);
   op->show(ysa);


 

}} {}
  }
  Function {RunAsClassifier()} {} {
    code {if(working) return;
          char Buf[512];
          char* file=NULL;
          Out->clear();
  
          if(TestData==NULL)
              LoadTestData();
          file = FileSelect("Load Trained ANN", Network);
          if(file!=NULL)
          {
              Out->add("Trained ANN Loaded...");
              ysa=fann_create_from_file(fNameBuf);
              Out->add("@C4Running with Test Data File...");
              Out->add("Data No : Predicted Class : Actual Class :  Result");
  
          }
  
          Tabs->value(LogG);
          Tabs->redraw();
  
      // If only 1 class
          if(ysa->num_output==1)
          {
              int resultReal=-1;
              int resultCalc=-1;
              bool rCorrect=false;
              float nCorrect=0;
              for(int i=0; i<TestData->num_data; i++)
              {
                  result=fann_run(ysa, TestData->input[i]);
                  if(*result>=0.5) resultCalc=1;
                  else
                      resultCalc=0;
                  if(TestData->output[i][0]>=0.5) resultReal=1;
                  else
                      resultReal=0;
                  if(resultReal==resultCalc)
                      rCorrect=true;
                  else
                      rCorrect=false;
                  if(rCorrect) nCorrect+=1;
                  sprintf(Buf,"%04d : %d : %d : %s",i+1,resultCalc,resultReal,(rCorrect)? "Correct":"Wrong");
                  Out->add(Buf);
              }
              sprintf(Buf,"@C4Classification Succes %% : %.2f ",nCorrect*100/TestData->num_data);
              Out->add(Buf);
              Out->bottomline(Out->size());
              return;
          }
  
      //  If Multi Class
  
          bool rCorrect=false;
          float nCorrect=0;
          int resultReal;
          int resultPrediction;
  
  
  
          for(int i=0; i<TestData->num_data; i++)
          {
              
              result=fann_run(ysa, TestData->input[i]);
              resultReal=GetClass(i);
              resultPrediction=GetPredictionClass();
  
              if(resultReal==resultPrediction)
                      rCorrect=true;
                  else
                      rCorrect=false;
  
              if(rCorrect) nCorrect+=1;
              if(resultPrediction==-1)
                sprintf(Buf,"%04d : Unknonw : %d : %s",i+1,resultReal,(resultReal==resultPrediction)? "Correct":"Wrong");
              else
                sprintf(Buf,"%04d : %d : %d : %s",i+1,resultPrediction,resultReal,(resultReal==resultPrediction)? "Correct":"Wrong");
              Out->add(Buf);
          }
          sprintf(Buf,"@C4Classification Succes %% : %.2f ",nCorrect*100/TestData->num_data);
          Out->add(Buf);
          Out->bottomline(Out->size());
          return;} {}
  }
  Function {ClearLog()} {} {
    code {if(working) return;
Out->clear();} {}
  }
  Function {SaveLog()} {} {
    code {if(working) return;
if(Out->size()<1)return;
char Buf[512];
char* file=NULL;
file = FileSelect("Save Log File", Log,Save);
//file = fl_file_chooser("Save Log File", "*.log", file);
if(file!=NULL){
  FILE *fp;
  fp=fopen(fNameBuf,"wt");
  if(fp){
   for(int i=1;i<=Out->size();i++){
     sprintf(Buf,"%s\\n",Out->text(i));
     fputs(Buf,fp);
   }
   fclose(fp);   

   Out->add("Log file Saved...");   
   
     
  }

}} {}
  }
  Function {LoadLog()} {} {
    code {if(working) return;
char* file=NULL;
Out->add("Select Log File...");
file = FileSelect("Select Log File...", Log);
// file = fl_file_chooser("Save Log File", "*.log", file);
if(file!=NULL)
   Out->load(fNameBuf);} {}
  }
  Function {StopProcess()} {} {
    code {stop=true;
Out->add("@C1Stop pressed please wait ...");} {}
  }
  Function {About()} {} {
    code {AboutDlg *ab=new AboutDlg();
ab->show();} {}
  }
  Function {Help()} {} {
    code {} {}
  }
  Function {DataProcessing()} {} {
    code {char *fn;
	char Buf [512];
	int num;

	if(working) return;

	fn = FileSelect("Select Raw Data File ", RawData);
//	fn=fl_file_chooser("Select Raw Data File ","*.txt", NULL, 0);


	if(fn){	
           int num=GetnItem(fNameBuf);
           if(num==1){
             tdp=new TimeSeri(); 
//             fl_alert("Time Series Raw Data File ");
             if(tdp->LoadRawData(fNameBuf)==false){
               fl_alert("Error readingg data File :(");
               return;           
             }               
	     TDPDlg *dp=new TDPDlg(fn);
	     dp->show();

	     return;           
           }
           else if (num>1){
             rdp=new DataProcess();
//             rdp->LoadRawData(fn);
             if(rdp->LoadRawData(fNameBuf)==false){
               fl_alert("Error readingg data File :(");
               return;           
             }               
	     NDPDlg *dp=new NDPDlg(fn);
	     dp->show();
// Ne u?a?t?d?beni ...
//	     delete rdp;
	     return;                    

           }
           else{
             fl_alert("Error in Raw Data File !");
	     return;                      
           
           }            
 
   
	}
	else{

	  
	}} {}
  }
  Function {LoadTrainData()} {} {
    code {char *fn;
char Buf [512];
int num,in,out;

if(working) return;

fn = FileSelect("Select Train Data File ", Data);
// fn=fl_file_chooser("Select Train Data File ","*.dat", NULL, 0);


if(fn){
  if(TrainData)fann_destroy_train(TrainData);
  TrainData = fann_read_train_from_file(fNameBuf);
  int err=fann_get_errno( (struct fann_error *) ft.TrainData);
  if(err == FANN_E_NO_ERROR){
      Out->clear();
      Out->add("Training Data File Selected");           
      sprintf(Buf,"Number of Data : %d ",TrainData->num_data);
      Out->add(Buf);
      sprintf(Buf,"\# of Input Nodes  : %d ",TrainData->num_input);
      Out->add(Buf);
      sprintf(Buf,"\# of Output Nodes : %d ",TrainData->num_output);
      Out->add(Buf);    
      Input->value(TrainData->num_input);
      Output->value(TrainData->num_output);   
      SetHiddens();       
      DataFile->value(fn);
      But_Alg->activate();
      But_Act->activate();
      But_Trn->activate();            
   
  }
  else{
   Out->add(fann_get_errstr((struct fann_error *)TrainData));
  
  }
  


}} {}
  }
  Function {LoadTestData()} {} {
    code {char *fn;
  char Buf [512];
  int num,in,out;

  if(working) return;
  
  fn = FileSelect("Select Test Data File ", Data);  
  

  if(fn){

    if(TestData)fann_destroy_train(TestData);
    TestData = fann_read_train_from_file(fNameBuf);
    int err=fann_get_errno( (struct fann_error *) ft.TestData);
    if(err == FANN_E_NO_ERROR){
      Out->clear();
      Out->add("Test Data File Selected");           
      sprintf(Buf,"Number of Data : %d ",TestData->num_data);
      Out->add(Buf);
      sprintf(Buf,"Number of Input : %d ",TestData->num_input);
      Out->add(Buf);
      sprintf(Buf,"Number of Output : %d ",TestData->num_output);
      Out->add(Buf);    
      TestFile->value(fn);
      But_Test->activate();   
      OverTraining->activate();     
    }
    else{
      Out->add(fann_get_errstr((struct fann_error *)TestData));
  
    }

  }} {}
  }
  Function {SetFineTuning()} {} {
    code {fann_set_train_error_function(ysa,(fann_errorfunc_enum)ErrorFunction->value());
    fann_set_activation_steepness_hidden(ysa,(fann_type) HiddenStepness->value());
    fann_set_activation_steepness_output(ysa,(fann_type) OutputStepness->value());
    fann_set_quickprop_decay(ysa,DecayFactor->value());
    fann_set_quickprop_mu(ysa,MuFactor->value());
    fann_set_rprop_increase_factor(ysa,IncreaseFactor->value());
    fann_set_rprop_decrease_factor(ysa,DecreaseFactor->value());
    fann_set_rprop_delta_min(ysa,DeltaMin->value());
    fann_set_rprop_delta_max(ysa,DeltaMax->value());} {}
  }
  Function {SetCascadeTuning()} {} {
    code {fann_set_cascade_output_change_fraction(ysa,OutputChange->value());
    fann_set_cascade_output_stagnation_epochs(ysa,(unsigned int )OutputStag->value());
    fann_set_cascade_candidate_change_fraction(ysa,CandidateChange->value());
    fann_set_cascade_candidate_stagnation_epochs(ysa,  (unsigned  int)CandidateStag->value());
    fann_set_cascade_weight_multiplier(ysa,WeighMultiplier->value());
    fann_set_cascade_candidate_limit(ysa,(fann_type)CandidateLimit->value());
    fann_set_cascade_max_out_epochs(ysa,(unsigned  int)MaxOutEpoch->value());
    fann_set_cascade_max_cand_epochs(ysa,(unsigned  int)MaxCasndidatetEpoch->value());
    fann_set_cascade_num_candidate_groups(ysa,(unsigned  int)NumCandidateGroups->value());} {}
  }
  Function {NeuralNetworkInfo()} {open
  } {
    code {if(working) return;
  char Buf[512];
  char* file=NULL;
  Out->clear();
  file = FileSelect("Load Trained ANN", Network);
//  file = fl_file_chooser("Load Trained ANN", "*.net", file);
  if(file!=NULL){
     ysa=fann_create_from_file(fNameBuf);
     
     Tabs->value(LogG);
     Tabs->redraw();     
     
     Out->add("@C4Neural Network ");
     Out->add(file);
     Out->add("");
 //
     NetInfo *op=new NetInfo();
      op->show(ysa);


 

  }} {}
  }
  Function {CreateDesiredANN()} {private
  } {
    code {if(Layer->value()==5)
  ysa = fann_create_sparse(ConnectionRate->value(),5,(int)Input->value(),(int)Hid1->value(),(int)Hid2->value(),(int)Hid3->value(),(int)Output->value());
else if(Layer->value()==4)
  ysa = fann_create_sparse(ConnectionRate->value(),4,(int)Input->value(),(int)Hid1->value(),(int)Hid2->value(),(int)Output->value());  
else
  ysa = fann_create_sparse(ConnectionRate->value(),3,(int)Input->value(),(int)Hid1->value(),(int)Output->value());

// Set Fine Tuning Parameters} {}
  }
  Function {ActivateStop()} {private
  } {
    code {stop=false;
Stop->activate();
working=true;} {}
  }
  Function {DeactivateStop()} {private
  } {
    code {stop=false;
Stop->deactivate();
working=false;} {}
  }
  Function {SaveANN()} {open private
  } {
    code {SaveDlg *save=new SaveDlg();
save->show();} {}
  }
  Function {ClearTrainingMemory()} {private
  } {
    code {for(int i=0; i<4; i++) {
    MinTrainingMSE[i]=MinTestingMSE[i]=1;
  }} {}
  }
  Function {GetClass(int i)} {return_type int
  } {
    code {int resultReal=-1;
    fann_type maxReal=-1.0;


    for(int j=0; j<ysa->num_output; j++)
    {

        if(TestData->output[i][j]>maxReal)
        {
            maxReal=TestData->output[i][j];
            resultReal=j;
        }
    }

    return resultReal;} {}
  }
  Function {GetPredictionClass()} {return_type int
  } {
    code {fann_type maxCalc0, maxCalc1;
    int resultCalc0,resultCalc1;
    maxCalc0=maxCalc1=-2;

    for(int j=0; j<ysa->num_output; j++)
    {

        if(result[j]>=maxCalc0)
        {
            maxCalc1=maxCalc0;
            maxCalc0=result[j];
            resultCalc1=resultCalc0;
            resultCalc0=j;
        }
    }

    if(maxCalc0==maxCalc1)
        return -1;
    if(maxCalc0<0.5)
        return -1;
    return resultCalc0;} {}
  }
} 

class GetInputs {} {
  decl {struct fann *ann;} {public local
  }
  decl {int ninputs, noutputs;} {public local
  }
  decl {bool tseries;} {public local
  }
  decl {fann_type *data,*result;} {private local
  }
  Function {GetInputs(bool ts=false)} {} {
    code {tseries=ts;
// data=new fann_type[ninputs];} {}
    Fl_Window window {
      label {Run ANN} open
      xywh {430 204 610 328} type Double box PLASTIC_UP_BOX color 21 align 0 hide
      code0 {o->set_modal();}
    } {
      Fl_Scroll Grid {
        label Inputs open
        xywh {10 25 235 245} box DOWN_BOX when 1
      } {}
      Fl_Return_Button Ok {
        label Ok
        user_data {"GetInputs"}
        callback cb_Ok
        xywh {355 285 235 30} box PLASTIC_UP_BOX color 15
      }
      Fl_Scroll GridOut {
        label Outputs open
        xywh {355 25 235 245} box DOWN_BOX color 19 when 1
      } {}
      Fl_Button {} {
        label {Run @->}
        callback {if(tseries)
  RunTS();
else
  Run();}
        xywh {255 135 90 30} box PLASTIC_UP_BOX color 1
      }
      Fl_Spinner kere {
        label Times
        xywh {260 86 75 24} align 2 step 10 value 10
        code0 {if(tseries==false) kere->hide();}
      }
    }
  }
  Function {show(struct fann *iann)} {open
  } {
    code {ann=iann;
  ninputs=fann_get_num_input(ann);
  noutputs=fann_get_num_output(ann);
  data=new fann_type[ninputs];

  char *Num;
  Fl_Float_Input *f;

//  Grid->position(0,0);
  Grid->begin();
  for(int i=0;i<ninputs;i++){
    Num= new char[10];
    sprintf(Num," %03d : ",i+1);
  // SCROLL ?? EK YAPARKEN SCROLLUN POZ?YONU ?ER?DEN EKLE  !!!
    f=new Fl_Float_Input(Grid->x()+70,Grid->y()+20*i+3,70,20,Num);
    if(tseries && ft.TestData){
       char buf[256];
       sprintf(buf,"%lf",ft.TestData->input[0][i]);
       f->value(buf);
     Grid->add(f);

    }

  }
  Grid->end();

//  GridOut->position(0,0);
  GridOut->begin();
  for(int i=0;i<noutputs;i++){
    Num= new char[10];
    sprintf(Num," %03d : ",i+1);
  // SCROLL ?? EK YAPARKEN SCROLLUN POZ?YONU ?ER?DEN EKLE  !!!
    f=new Fl_Float_Input(GridOut->x()+70,GridOut->y()+20*i+3,70,20,Num);
    GridOut->add(f);
  }
  Fl::focus(GridOut->child(0));
  GridOut->end();
  GridOut->redraw();

  Fl::focus(Grid->child(0));
  window->show();} {}
  }
  Function {hide()} {} {
    code {window->hide();
delete data;} {}
  }
  Function {Run()} {} {
    code {char buf[10];

Fl_Float_Input *f;

for(int i=0;i<ninputs;i++){
   f=(Fl_Float_Input *)Grid->child(i);
   data[i]=atof(f->value());
}

result=fann_run(ann, data);

for(int i=0;i<noutputs;i++){
   sprintf(buf,"%lf",result[i]);
   f=(Fl_Float_Input *)GridOut->child(i);
   f->value(buf);
}

Fl::focus(GridOut->child(0));
GridOut->end();
GridOut->redraw();} {}
  }
  Function {RunTS()} {} {
    code {char buf[512];
Fl_Float_Input *f;


int max=kere->value();
Out->add("Inputs...");
for(int i=0;i<ninputs;i++){
   f=(Fl_Float_Input *)Grid->child(i);
   Out->add(f->value());
   data[i]=atof(f->value());
}

Out->add("Forecasts...");


for(int i=0;i<max;i++){
   result=fann_run(ann, data);
   sprintf(buf,"%f",result[0]);
   Out->add(buf);

   if(i<max) {
     for(int j=0;j<ninputs-1;j++)
        data[j]=data[j+1];
     data[ninputs-1]=result[0];
   }
}

for(int i=0;i<ninputs;i++){
   f=(Fl_Float_Input *)Grid->child(i);
   sprintf(buf,"%lf",data[i]);
   f->value(buf);
}

sprintf(buf,"%lf",result[0]);
f=(Fl_Float_Input *)GridOut->child(0);
f->value(buf);} {}
  }
} 

class NetInfo {open
} {
  decl {vector<double> weights;} {private local
  }
  decl {struct fann *ann;} {public local
  }
  Function {NetInfo()} {} {
    Fl_Window window {
      label {Neural Network Information} open
      xywh {429 144 533 374} type Double box PLASTIC_UP_BOX color 21 align 0 hide
      code0 {o->set_modal();}
    } {
      Fl_Return_Button Ok {
        label Ok
        user_data {"GetInputs"}
        callback cb_Ok
        xywh {362 330 161 30} box PLASTIC_UP_BOX color 15
      }
      Fl_Value_Output Input {
        label {Input Layer  + 1 Bias}
        xywh {50 56 50 24} box PLASTIC_UP_BOX color 140 align 1
      }
      Fl_Browser Hidden {
        label {Hidden Layers}
        xywh {160 31 242 71} box PLASTIC_UP_BOX color 140 align 1
      }
      Fl_Box {} {
        label {@+4->}
        xywh {115 53 35 27} labelcolor 15
      }
      Fl_Box {} {
        label {@+4->}
        xywh {415 53 30 27} labelcolor 15
      }
      Fl_Value_Output Output {
        label {Output Layer}
        xywh {460 51 50 24} box PLASTIC_UP_BOX color 140 align 1
      }
      Fl_Output NType {
        label {Network Type :}
        xywh {150 131 290 24}
      }
      Fl_Output Alg {
        label {Training Algorithm :}
        xywh {150 165 290 24}
      }
      Fl_Output Err {
        label {Error Function :}
        xywh {150 206 290 24} hotspot
      }
      Fl_Output Stop {
        label {Stop Function :}
        xywh {150 240 290 24} hotspot
      }
      Fl_Choice WLabel {
        label {Weights : }
        callback {WValue->value(weights[WLabel->value()]);} open
        xywh {77 285 243 25} down_box BORDER_BOX
      } {}
      Fl_Value_Output WValue {
        label {@+4->}
        xywh {350 286 115 24} color 7 labelcolor 15
      }
    }
  }
  Function {show(struct fann *iann)} {open
  } {
    code {ann=iann;

  char buf[512];
  int j=1;
  struct fann_layer *layer_it;
  
  Input->value( ann->num_input );
  Output->value( ann->num_output );
  
  sprintf(buf, "Input layer                          :%4d neurons, 1 bias\\n", ann->num_input);
  Out->add(buf);
  for(layer_it = ann->first_layer + 1; layer_it != ann->last_layer - 1; layer_it++)
  {
    if(ann->network_type == FANN_NETTYPE_SHORTCUT){
      sprintf(buf,"  Hidden layer  %d                    :%4d neurons, 0 bias\\n",j,
				   layer_it->last_neuron - layer_it->first_neuron);
      Out->add(buf);	
      sprintf(buf," Layer %d : %4d neurons, 0 bias\\n",j,
				   layer_it->last_neuron - layer_it->first_neuron);      
      Hidden->add(buf);			   
    }
    else{
      sprintf(buf,"  Hidden layer  %d                    :%4d neurons, +1 bias\\n",j,
				   layer_it->last_neuron - layer_it->first_neuron - 1);
      Out->add(buf);
      sprintf(buf," Layer %d : %4d neurons, +1 bias\\n",j,
				   layer_it->last_neuron - layer_it->first_neuron -1);            
      Hidden->add(buf);			         
    }
    j++;
  }
  sprintf(buf,"Output layer                         :%4d neurons\\n", ann->num_output);
  Out->add(buf);
  sprintf(buf,"Total neurons and biases             :%4d\\n", fann_get_total_neurons(ann));
  Out->add(buf);  
  sprintf(buf,"Total connections                    :%4d\\n", ann->total_connections);
  Out->add(buf);  
  sprintf(buf,"Connection rate                      :%8.3f\\n", ann->connection_rate);
  Out->add(buf);  
  sprintf(buf,"Network type                         :   %s\\n", FANN_NETTYPE_NAMES[ann->network_type]);  
  Out->add(buf);  
  NType->value(FANN_NETTYPE_NAMES[ann->network_type]);
  sprintf(buf,"Training algorithm                   :   %s\\n", FANN_TRAIN_NAMES[ann->training_algorithm]);
  Out->add(buf);  
  Alg->value(FANN_TRAIN_NAMES[ann->training_algorithm]);
  sprintf(buf,"Training error function              :   %s\\n", FANN_ERRORFUNC_NAMES[ann->train_error_function]);
  Out->add(buf);  
  Err->value(FANN_ERRORFUNC_NAMES[ann->train_error_function]);
  sprintf(buf,"Training stop function               :   %s\\n", FANN_STOPFUNC_NAMES[ann->train_stop_function]);
  Stop->value(FANN_STOPFUNC_NAMES[ann->train_stop_function]);
  Out->add(buf);  
  sprintf(buf,"Bit fail limit                       :%8.3f\\n", ann->bit_fail_limit);
  Out->add(buf);  
  sprintf(buf,"Learning rate                        :%8.3f\\n", ann->learning_rate);
  Out->add(buf);  
  sprintf(buf,"Learning momentum                    :%8.3f\\n", ann->learning_momentum);
  Out->add(buf);  
  sprintf(buf,"Quickprop decay                      :%11.6f\\n", ann->quickprop_decay);
  Out->add(buf);  
  sprintf(buf,"Quickprop mu                         :%8.3f\\n", ann->quickprop_mu);
  Out->add(buf);  
  sprintf(buf,"RPROP increase factor                :%8.3f\\n", ann->rprop_increase_factor);
  Out->add(buf);  
  sprintf(buf,"RPROP decrease factor                :%8.3f\\n", ann->rprop_decrease_factor);
  Out->add(buf);  
  sprintf(buf,"RPROP delta min                      :%8.3f\\n", ann->rprop_delta_min);
  Out->add(buf);  
  sprintf(buf,"RPROP delta max                      :%8.3f\\n", ann->rprop_delta_max);
  Out->add(buf);  
  sprintf(buf,"Cascade output change fraction       :%11.6f\\n", ann->cascade_output_change_fraction);
  Out->add(buf);  
  sprintf(buf,"Cascade candidate change fraction    :%11.6f\\n", ann->cascade_candidate_change_fraction);
  Out->add(buf);  
  sprintf(buf,"Cascade output stagnation epochs     :%4d\\n", ann->cascade_output_stagnation_epochs);
  Out->add(buf);  
  sprintf(buf,"Cascade candidate stagnation epochs  :%4d\\n", ann->cascade_candidate_stagnation_epochs);
  Out->add(buf);  
  sprintf(buf,"Cascade max output epochs            :%4d\\n", ann->cascade_max_out_epochs);
  Out->add(buf);  
  sprintf(buf,"Cascade max candidate epochs         :%4d\\n", ann->cascade_max_cand_epochs);
  Out->add(buf);  
  sprintf(buf,"Cascade weight multiplier            :%8.3f\\n", ann->cascade_weight_multiplier);
  Out->add(buf);  
  sprintf(buf,"Cascade candidate limit              :%8.3f\\n", ann->cascade_candidate_limit);
  Out->add(buf);  
  for(unsigned int i = 0; i < ann->cascade_activation_functions_count; i++){
    sprintf(buf,"Cascade activation functions[%d]      :   %s\\n", i,
			FANN_ACTIVATIONFUNC_NAMES[ann->cascade_activation_functions[i]]);
    Out->add(buf);			
   }
   for(unsigned int i = 0; i < ann->cascade_activation_steepnesses_count; i++){
     sprintf(buf,"Cascade activation steepnesses[%d]    :%8.3f\\n", i,
			ann->cascade_activation_steepnesses[i]);
     Out->add(buf);			
   }
   
   sprintf(buf,"Cascade candidate groups             :%4d\\n", ann->cascade_num_candidate_groups);
   Out->add(buf);   
   sprintf(buf,"Cascade no. of candidates            :%4d\\n", fann_get_cascade_num_candidates(ann));
   Out->add(buf);   
   window->show();
 //
    struct fann_neuron *first_neuron;
    struct fann_neuron *neuron_it;
    unsigned int index;
    unsigned int source_index;
    unsigned int destination_index;
    int lyr=0;

    first_neuron = ann->first_layer->first_neuron;

    source_index = 0;
    destination_index = 0;
    

    /* The following assumes that the last unused bias has no connections */

    /* for each layer */
    for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++,lyr++){
      sprintf(buf,"Layer %d ",lyr);
      Out->add(buf);
        /* for each neuron */
        for(neuron_it = layer_it->first_neuron; neuron_it != layer_it->last_neuron; neuron_it++){
            /* for each connection */

            int linx=0;
            for (index = neuron_it->first_con; index < neuron_it->last_con; index++){

                
                sprintf(buf," From Neuron To Neuron  ( %d ---> %d ) :  Weight : %lf",linx ,destination_index,ann->weights[source_index]);
/*
                sprintf(buf," From Neuron To Neuron  ( %d ---> %d ) :  Weight : %lf"
                    ,(ann->connections[source_index] - first_neuron),destination_index,ann->weights[source_index]);
*/
                Out->add(buf);
//                sprintf(buf,"From Neuron  %d To Neuron %d ",(ann->connections[source_index] - first_neuron),destination_index);
                sprintf(buf,"From Neuron  %d To Neuron %d ",linx++,destination_index);                             
                WLabel->add(buf);
                weights.push_back(ann->weights[source_index]);
                source_index++;
            }
            destination_index++;
        }
    }
    WLabel->value(0);
    WValue->value(weights[0]);

   //   
   
   
   
   window->show();} {}
  }
  Function {hide()} {} {
    code {window->hide();} {}
  }
} 

class AboutDlg {} {
  Function {AboutDlg()} {open
  } {
    Fl_Window window {
      label About open
      xywh {577 248 532 472} type Double box PLASTIC_UP_BOX color 22 align 0 hide
      code0 {o->set_modal();} modal
    } {
      Fl_Return_Button Ok {
        label Ok
        callback cb_Ok
        xywh {395 430 130 30} box PLASTIC_UP_BOX color 15
      }
      Fl_Group {} {open
        xywh {10 7 515 131} box EMBOSSED_FRAME color 1
      } {
        Fl_Box {} {
          image {img/Logo6.jpg} xywh {15 14 505 121} box SHADOW_FRAME color 1
        }
      }
      Fl_Help_View Help {
        xywh {10 155 515 260} box GTK_DOWN_BOX color 19
        code0 {Help->load("About.html");}
        code1 {Help->link(OpenLink);}
        code2 {Help->textsize(15);}
      }
      Fl_Button {} {
        callback {mDonation();}
        image {img/btn_donate_LG.png} xywh {10 425 105 30} box NO_BOX
      }
    }
  }
  Function {show()} {open
  } {
    code {/*
Info->add("* FannTool is a GUI tool for ANN by using FANN library");
Info->add("  FANN is a free open source neural network library");
Info->add("  FANN Web Page : leenissen.dk/fann/");
Info->add("  FannTool page : http://code.google.com/p/fanntool/");
Info->add("* Programmed by BlueKid");
Info->add("  http://derindelimavi.blogspot.com/");
Info->add("* Please Send me any suggestion, modification or bugs.");
Info->add(" Don't hesitate to contact me for any question");
Info->add(" I will be very grateful with your feedbacks.");
Info->add(" bluekid70@gmail.com");
*/
window->show();} {}
  }
  Function {hide()} {} {
    code {window->hide();} {}
  }
} 

class SaveDlg {} {
  Function {SaveDlg()} {open
  } {
    Fl_Window window {
      label {Save ANN} open
      xywh {554 30 573 366} type Double box PLASTIC_UP_BOX color 22 align 0 hide resizable
      code0 {o->set_modal();} modal
    } {
      Fl_Return_Button Ok {
        label Exit
        callback cb_Ok
        xywh {424 315 130 30} box PLASTIC_UP_BOX color 15
      }
      Fl_Group {} {open
        xywh {20 27 535 238} box EMBOSSED_FRAME
      } {
        Fl_Group {} {open
          xywh {45 82 170 140}
        } {
          Fl_Round_Button Latest {
            label {Latest  ANN}
            xywh {45 82 170 20} type Radio down_box ROUND_DOWN_BOX value 1
          }
          Fl_Round_Button ANN0 {
            label {Minimum Training MSE}
            xywh {45 115 170 24} type Radio down_box ROUND_DOWN_BOX
          }
          Fl_Round_Button ANN1 {
            label {Minimum Testing MSE}
            xywh {45 163 170 19} type Radio down_box ROUND_DOWN_BOX deactivate
          }
          Fl_Round_Button ANN2 {
            label {Minimum  OCS MSE}
            xywh {45 202 170 20} type Radio down_box ROUND_DOWN_BOX deactivate
          }
        }
        Fl_Value_Output {} {
          label {Training MSE }
          xywh {295 78 100 24} align 1
          code0 {o->value(MinTrainingMSE[3]);}
        }
        Fl_Group {} {open
          xywh {295 78 230 145}
        } {
          Fl_Group OCS1 {open
            xywh {420 78 105 61} deactivate
          } {
            Fl_Value_Output {} {
              label {Testing MSE }
              xywh {420 78 105 24} align 1
              code0 {if(ft.TestData && ft.overtraining) o->value(MinTestingMSE[3]);}
            }
            Fl_Value_Output {} {
              xywh {420 115 105 24} align 1
              code0 {if(ft.TestData && ft.overtraining)o->value(MinTestingMSE[0]);}
            }
          }
          Fl_Group OCS2 {open
            xywh {295 159 230 64} deactivate
          } {
            Fl_Value_Output {} {
              xywh {295 159 100 24} align 1
              code0 {if(ft.TestData && ft.overtraining) o->value(MinTrainingMSE[1]);}
            }
            Fl_Value_Output {} {
              xywh {420 159 105 24} align 1
              code0 {if(ft.TestData && ft.overtraining)o->value(MinTestingMSE[1]);}
            }
            Fl_Value_Output {} {
              xywh {295 198 100 24} align 1
              code0 {if(ft.TestData && ft.overtraining) o->value(MinTrainingMSE[2]);}
            }
            Fl_Value_Output {} {
              xywh {420 198 105 24} align 1
              code0 {if(ft.TestData && ft.overtraining)o->value(MinTestingMSE[2]);}
            }
          }
        }
        Fl_Value_Output {} {
          xywh {295 115 100 24} align 1
          code0 {o->value(MinTrainingMSE[0]);}
        }
      }
      Fl_Button {} {
        label {&Save ANN}
        callback {char* file=NULL;
int i=-1;
if(Latest->value()){
  i=3;
}
else if(ANN0->value())
  i=0;
else if(ANN1->value())
  i=1;
else if(ANN2->value())
  i=2;
  
file = FileSelect("Save Selected ANN", Network,Save);
//file = fl_file_chooser("Save Selected ANN", "*.net", file);
if(file!=NULL){
   fann_save(MinANN[i], fNameBuf);
   fl_message("ANN Saved...");   
}}
        xywh {250 315 130 30} box PLASTIC_UP_BOX shortcut 0x40073 color 1
      }
    }
  }
  Function {show()} {open
  } {
    code {if(ft.TestData && ft.overtraining){
  ANN1->activate();
  ANN2->activate();
  OCS1->activate();
  OCS2->activate();
  
}
window->show();} {}
  }
  Function {hide()} {open
  } {
    code {window->hide();} {}
  }
  Function {~SaveDlg()} {} {
    code {fl_alert("Save dialog Closing");} {}
  }
} 

class NDPDlg {} {
  Function {NDPDlg(char * FileName)} {} {
    Fl_Window window {
      label {Raw Data Processing } open
      xywh {263 120 755 533} type Double box PLASTIC_UP_BOX color 19 align 0 hide resizable
      code0 {o->set_modal();} modal
    } {
      Fl_Return_Button Ok {
        label Exit
        callback {// cb_Ok
delete rdp;
((Fl_Window *)(o->parent()))->hide();}
        xywh {619 497 130 30} box PLASTIC_UP_BOX color 15
      }
      Fl_Output RawFile {
        label {Raw Data File:}
        xywh {110 14 367 24}
        code0 {RawFile->value(FileName);}
      }
      Fl_Value_Output nDataPoint {
        label {Number of Data:}
        xywh {599 15 80 24}
      }
      Fl_Value_Output nInput {
        label {Number of Input dim.:}
        xywh {158 50 44 24}
      }
      Fl_Counter nOutput {
        label {Number of Output dim.:}
        callback {nInput->value(rdp->GetNItem()-nOutput->value());}
        xywh {360 48 75 26} type Simple box GTK_UP_BOX color 33 align 4 minimum 1 step 1 value 1
      }
      Fl_Button Wbt {
        label {Write Data Files}
        callback {bool result;
if(shuffle->value())
        rdp->Shuffle();
  if(Scale_D->value()){
     result=rdp->WriteScaleParameters((char*)fName->value());    
     rdp->ScaleAll();
     if(result)
       fl_message("Scaling Parameters saved successfully ");
     else
       fl_alert("Error occured during Scaling Parameters saving ");     
     
     
   }  
  
  result=rdp->WriteData((char*)fName->value(),nOutput->value(),rTrain->value());
  
  if(result)
    fl_message("Data Files saved successfully ");
  else
    fl_alert("Error occured during Data Files saving ");
    
delete rdp;
((Fl_Window *)(o->parent()))->hide();}
        xywh {472 499 130 30} box PLASTIC_UP_BOX color 10
      }
      Fl_Value_Slider rTrain {
        label {Fraction of Data for Training}
        callback {rTest->value(1.0-rTrain->value());}
        xywh {20 100 175 20} type Horizontal box EMBOSSED_BOX color 22 selection_color 1 align 5 minimum 0.5 step 0.1 value 0.6 textsize 14
      }
      Fl_Value_Slider rTest {
        label {Fraction of Data for Testing}
        callback {rTrain->value(1.0-rTest->value());}
        xywh {210 100 175 20} type Horizontal box EMBOSSED_BOX color 22 selection_color 1 align 5 maximum 0.5 step 0.1 value 0.4 textsize 14
      }
      Fl_Check_Button shuffle {
        label {Shuffle Data}
        xywh {440 54 85 20} down_box DOWN_BOX
      }
      Fl_Input fName {
        label {Data File Name Stem:}
        xywh {560 96 136 24}
        code0 {fName->value(fl_filename_setext(FileName,"_out"));}
      }
      Fl_Check_Button Scale_D {
        label {Scale Data}
        xywh {570 50 120 24} down_box DOWN_BOX value 1
      }
      Fl_Group {} {open
        xywh {15 130 735 365} box PLASTIC_UP_BOX color 19
      } {
        Fl_Counter Item {
          label {Item No}
          callback {GetItemData();}
          xywh {100 154 56 20} type Simple labelcolor 4 align 4 minimum 0 step 1
        }
        Fl_Box Chart {
          tooltip Histogram xywh {40 179 695 268} box EMBOSSED_BOX color 34
          code0 {Chart->type(FL_BAR_CHART);}
          code1 {Chart->bounds(-0.05,1);}
          class Fl_Chart
        }
        Fl_Value_Slider MinV {
          label {Minimum Output Value}
          callback {itmp=rdp->GetItem(Item->value());
  itmp->SetMinV(MinV->value());}
          xywh {174 154 142 20} type Horizontal box EMBOSSED_BOX color 22 selection_color 1 align 1 minimum -1 maximum 0.3 step 0.1 textsize 14
        }
        Fl_Value_Slider MaxV {
          label {Maximum Output Value}
          callback {itmp=rdp->GetItem(Item->value());
  itmp->SetMaxV(MaxV->value());}
          xywh {329 154 146 20} type Horizontal box EMBOSSED_BOX color 22 selection_color 1 align 1 minimum 0.5 step 0.1 value 1 textsize 14
        }
        Fl_Check_Button Scale_I {
          label {Scale Item }
          callback {itmp=rdp->GetItem(Item->value());
  itmp->scale=Scale_I->value();}
          xywh {514 154 64 15} down_box DOWN_BOX value 1
        }
        Fl_Value_Input Min {
          callback {itmp=rdp->GetItem(Item->value());
if(itmp->GetMin() > Min->value()){
  itmp->SetMin(Min->value());
}
else{
   fl_alert("Value must be less than %f",itmp->GetMin());
   Min->value(itmp->GetMin());
}}
          xywh {40 453 80 25} when 4
        }
        Fl_Value_Input Max {
          callback {itmp=rdp->GetItem(Item->value());
if(itmp->GetMax() < Max->value()){
  itmp->SetMax(Max->value());
}
else{
   fl_alert("Value must be greater than %f",itmp->GetMax());
   Max->value(itmp->GetMax());
}}
          xywh {655 453 80 24} when 4
        }
      }
    }
  }
  Function {show()} {} {
    code {nDataPoint->value(rdp->GetNData());
  nInput->value(rdp->GetNItem()-1);
  nOutput->maximum(rdp->GetNItem()-1);
  Item->maximum(rdp->GetNItem()-1);  
  GetItemData();  
  window->show();} {}
  }
  Function {hide()} {} {
    code {window->hide();} {}
  }
  Function {GetItemData()} {} {
    code {//  Item k?m?
  itmp=rdp->GetItem(Item->value());
  Min->value(itmp->GetMin());
  MinV->value(itmp->GetMinV());  
  Max->value(itmp->GetMax());  
  MaxV->value(itmp->GetMaxV());    
  Chart->clear();
  double *hst=itmp->Histogram(10);
  for(int i=0; i<10;i++){
    char buf[50];
    sprintf(buf,"%.1f",100*hst[i]);   
    Chart->add(hst[i],buf,19);
  }
  Scale_I->value(itmp->scale);} {}
  }
} 

class TDPDlg {} {
  Function {TDPDlg(char * FileName)} {} {
    Fl_Window window {
      label {Time Series  Data Processing } open
      xywh {297 118 784 530} type Double box PLASTIC_UP_BOX color 19 align 0 hide resizable
      code0 {o->set_modal();} modal
    } {
      Fl_Return_Button Ok {
        label Exit
        callback {// cb_Ok
delete rdp;
((Fl_Window *)(o->parent()))->hide();}
        xywh {657 495 130 30} box PLASTIC_UP_BOX color 15
      }
      Fl_Output RawFile {
        label {Raw Data File :}
        xywh {115 16 395 24}
        code0 {RawFile->value(FileName);}
      }
      Fl_Value_Output nDataPoint {
        label {Number of Data:}
        xywh {665 16 80 24}
      }
      Fl_Counter nInput {
        label {Number of Input dim.:}
        callback {tdp->nInput=nInput->value();}
        xywh {155 48 75 26} type Simple box GTK_UP_BOX color 33 align 4 minimum 1 step 1 value 1
      }
      Fl_Button Wbt {
        label {Write Data Files}
        callback {bool result;
// if(shuffle->value()) tdp->Shuffle();
  if(Scale_D->value()){
     result=tdp->WriteScaleParameters((char*)fName->value());    
     if(tdp->scale) tdp->Scale();
     if(result)
       fl_message("Scaling Parameters saved successfully ");
     else
       fl_alert("Error occured during Scaling Parameters saving ");     
     
     
   }  
  
  result=tdp->WriteData((char*)fName->value(),rTrain->value());
  
  if(result)
    fl_message("Data Files saved successfully ");
  else
    fl_alert("Error occured during Data Files saving ");
   

((Fl_Window *)(o->parent()))->hide();}
        xywh {505 495 130 30} box PLASTIC_UP_BOX color 10
      }
      Fl_Value_Slider rTrain {
        label {Fraction of Data for Training}
        callback {rTest->value(1.0-rTrain->value());}
        xywh {20 100 175 20} type Horizontal box EMBOSSED_BOX color 22 selection_color 1 align 5 minimum 0.5 step 0.1 value 0.6 textsize 14
      }
      Fl_Value_Slider rTest {
        label {Fraction of Data for Testing}
        callback {rTrain->value(1.0-rTest->value());}
        xywh {230 100 175 20} type Horizontal box EMBOSSED_BOX color 22 selection_color 1 align 5 maximum 0.5 step 0.1 value 0.4 textsize 14
      }
      Fl_Check_Button Shuffle {
        label {Shuffle Data}
        callback {tdp->shuffle=Shuffle->value();}
        xywh {460 54 85 20} down_box DOWN_BOX
      }
      Fl_Input fName {
        label {Data File Name Stem:}
        xywh {560 96 136 24}
        code0 {fName->value(fl_filename_setext(FileName,"_out"));}
      }
      Fl_Check_Button Scale_D {
        label {Scale Data}
        callback {tdp->scale=Scale_D->value();}
        xywh {590 50 120 24} down_box DOWN_BOX value 1
      }
      Fl_Group Gr {open
        xywh {15 130 767 355} box PLASTIC_UP_BOX color 19
      } {
        Fl_Value_Input Min {
          callback {if(tdp->GetMin() > Min->value()){
  tdp->SetMin(Min->value());
}
else{
   fl_alert("Value must be less than %f",tdp->GetMin());
   Min->value(tdp->GetMin());
}}
          xywh {40 451 75 24} when 4
        }
        Fl_Value_Input Max {
          callback {if(tdp->GetMax() < Max->value()){
  tdp->SetMax(Max->value());
}
else{
   fl_alert("Value must be greater than %f",tdp->GetMax());
   Max->value(tdp->GetMax());
}}
          xywh {692 451 80 24} when 4
        }
        Fl_Box Chart {
          xywh {40 179 710 268} box EMBOSSED_BOX color 34
          code0 {Chart->type(FL_BAR_CHART);}
          code1 {Chart->bounds(-0.05,1);}
          class Fl_Chart
        }
        Fl_Value_Slider MinV {
          label {Minimum Output Value}
          callback {tdp->SetMinV(MinV->value());}
          xywh {55 149 142 20} type Horizontal box EMBOSSED_BOX color 22 selection_color 1 align 1 minimum -1 maximum 0.3 step 0.1 textsize 14
        }
        Fl_Value_Slider MaxV {
          label {Maximum Output Value}
          callback {tdp->SetMaxV(MaxV->value());}
          xywh {225 151 146 18} type Horizontal box EMBOSSED_BOX color 22 selection_color 1 align 1 minimum 0.5 step 0.1 value 1 textsize 14
        }
        Fl_Group {} {open
          xywh {395 140 355 29} color 23
        } {
          Fl_Light_Button Hist {
            label Histogram
            callback {DrawGraph();}
            xywh {425 145 115 20} type Radio box NO_BOX value 1 selection_color 1
          }
          Fl_Light_Button {} {
            label {Time Series}
            callback {DrawGraph();}
            xywh {590 145 115 20} type Radio box NO_BOX selection_color 1
          }
        }
      }
      Fl_Value_Output {} {
        label {Number of Output dim.:}
        xywh {395 50 25 24} value 1
      }
    }
  }
  Function {show()} {open
  } {
    code {nDataPoint->value(tdp->GetNData());
  nInput->value(tdp->nInput);
//  nOutput->maximum(tdp->nOutput);

//  itmp=rdp->GetItem(Item->value());
  Min->value(tdp->GetMin());
  MinV->value(tdp->GetMinV());  
  Max->value(tdp->GetMax());  
  MaxV->value(tdp->GetMaxV());    
  DrawGraph();
  Scale_D->value(tdp->scale);


  window->show();} {}
  }
  Function {hide()} {} {
    code {window->hide();} {}
  }
  Function {DrawGraph()} {open
  } {
    code {if(Hist->value()){
    Chart->clear();
    Chart->type(FL_BAR_CHART);
    Chart->bounds(-0.05,1);    
    Chart->resize(40,179,710,268);    
    Max->position(692,451);    
    double *hst=tdp->Histogram(10);
    for(int i=0; i<10;i++){
      char buf[50];
      sprintf(buf,"%.1f",100*hst[i]);   
      Chart->add(hst[i],buf,19);
    }
  }
  else{
    Chart->clear();
    Chart->type(FL_LINE_CHART);
    Chart->bounds(Min->value(),Max->value());  
    Chart->resize(115,179,635,268);
    Max->position(30,183);        
    for(long i=0; i<tdp->count;i++){
       Chart->add(tdp->GetData(i));
    }
  
  }
  
Gr->redraw();} {}
  }
} 

Function {OpenLink(Fl_Widget *w, const char *uri)} {return_type {const char *}
} {
  code {fl_open_uri(uri, "Deneme");
return NULL;} {}
} 

Function {Exit_CB(Fl_Widget*o, void*w)} {} {
  code {if(ft.working ){
 if(fl_choice("Are you sure you want to exit? \\n Training Continue!","Return to Program" ,"Exit" ,NULL)){  
  
    ft.StopProcess();
    Fl::wait(10);
    exit(0);
  }
  
}


if(fl_choice("Do you want to exit","Return to Program" ,"Exit" ,NULL)){  
  
  exit(0);
}} {}
} 

Function {splashScreen(int sec)} {return_type void
} {
  Fl_Window splash {
    xywh {650 146 522 378} type Double box UP_BOX color 31 hide hotspot modal
  } {
    Fl_Slider wait {
      xywh {15 140 500 10} type {Horz Fill} box ENGRAVED_BOX color 23 selection_color 1
      code0 {wait->maximum(sec);}
      code1 {wait->clear_visible_focus();}
    }
    Fl_Box {} {
      image {img/Logo6.jpg} xywh {15 12 500 114}
    }
    Fl_Button {} {
      callback {mDonation();}
      image {img/btn_donate_LG.png} xywh {15 325 100 35} box NO_BOX
    }
    Fl_Return_Button Ok {
      label Ok
      callback {nowait->value(1);}
      xywh {400 335 115 30}
    }
    Fl_Check_Button nowait {
      label button
      xywh {185 430 64 15} down_box DOWN_BOX hide deactivate
    }
    Fl_Browser DMesg {
      xywh {15 160 500 155} box GTK_UP_FRAME textsize 16
    }
  }
  code {int x,y,w,h;
fl_center(splash);
splash->clear_border();
 char *Text[]={"",
          "FANNTool is an Open Source project ",
  	      "And so the users and the community are important factor.",
  	      "@bWe appreciate every feedback, question, bug report, patches",
  	      "@bAnd of course donations.",
  	      "@bBy donating money to FANNTool,",
  	      "@bYou help us to continue to development of new versions ",
  	      "",
  	      };
  for(int i=0;i<8;i++)
     DMesg->add(Text[i]);splash->show();

do {
       Fl::check();

} while (!splash->visible());


//  time_t start,end;
  
  clock_t start,end;

  double dif;
//  time (&start);
start= clock();


  do{ 
//    time (&end);
    end = clock();
//    dif = difftime (end,start);
    dif = 100*((double)(end-start))/CLK_TCK;
    wait->value(dif);
    wait->redraw();
    Fl::check();    
  }while(dif<sec && nowait->value()==0);  

// Ok->activate();
 splash->hide();


// return splash;} {}
} 

Function {fl_center(Fl_Double_Window*win)} {return_type void
} {
  code {int x,y,w,h;
    Fl::screen_xywh(x,y,w,h);
    int sx,sy;

    sx=(w-win->w())/2;
    sy=(h-win->h())/2;
    win->position(sx,sy);} {}
} 

decl {bool FSIn=false;} {private local
} 

decl {Fl_Native_File_Chooser fnfc;} {
  comment {// gloabal olmayınca tekrar tekrar çalışıyor} private local
} 

Function {FileSelect(char *Msg,FileType f,SelectType s=Open)} {return_type {char *}
} {
  code {if(FSIn) return NULL;
      FSIn=true;
    char *filterTexts[]={
        "Data File \\t*.dat",
        "Log File \\t*.log",
        "Raw Data File \\t*.txt",
        "Neural Network File \\t*.net"
    };
   //     Fl_Native_File_Chooser *fnfc=new Fl_Native_File_Chooser();;
        fnfc.title(Msg);
        if(s== Open)
          fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        else
          fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);

        fnfc.filter(filterTexts[f]);
        switch ( fnfc.show() ) {
          case -1:
              fl_alert("ERROR: %s\\n", fnfc.errmsg());
              FSIn=false;
              return NULL;
          break; // ERROR
          case 1:
               FSIn=false;
               return NULL;
          break; // CANCEL
          default:
              FSIn=false;
              fl_utf8to_mb (fnfc.filename(),strlen(fnfc.filename()),fNameBuf,2048 );
              return (char *) fnfc.filename();
          break; // FILE CHOSEN
        }} {}
} 

Function {mDonation()} {} {
  code {char errmsg[512];
char url[]="https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=Z4LJTTPXCLYN6l";
if ( !fl_open_uri(url, errmsg, sizeof(errmsg)) ) {
char warnmsg[768];
sprintf(warnmsg, "Error: %s", errmsg);
fl_alert(warnmsg);
}} {}
} 
