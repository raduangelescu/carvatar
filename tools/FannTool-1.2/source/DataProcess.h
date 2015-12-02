#ifndef DATAPROCESS_H
#define DATAPROCESS_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

int GetnItem(char *FileName);

class DataItem
{
    public:
        int nhist;
        bool scale;
        DataItem();
        double GetData(unsigned long i);
        void   SetData(unsigned long i,double value);
        void   Add(double value);
        // void DeleteData(unsigned long i);
        double *Histogram(int n=10);
        void Scale( double iminv,double imaxv );
        void Scale();
        void Swap(unsigned long i1,unsigned long i2);
        double GetMin();
        void SetMin(double value);
        double GetMinV();
        void SetMinV(double value);
        double GetMax();
        void SetMax(double value);
        double GetMaxV();
        void SetMaxV(double value);
        double GetAvg();
        virtual ~DataItem();
    protected:
      double min;
      double max;
      double minv;
      double maxv;
      double sum;
      double hist[20];
      double avg;
      double stddev;
      vector<double> data;
    private:


};

// typedef pDataItem DataItem*;

class DataProcess
{
    public:
        DataProcess();
        bool LoadRawData(char * filename);
        bool ParseData();
        bool WriteData(char *FileName,int nOut, float pTrain);
        bool WriteScaleParameters(char *FileName);
        DataItem *GetItem(int i);
        unsigned long GetNData(){
           return nData;
        }
        unsigned int GetNItem(){
          return nItem;
        }
        void ScaleAll();
        void Shuffle();
        virtual ~DataProcess();
    protected:
        vector<double> line_data;
        int ReadLine(string line);
        vector<DataItem*> items;
        unsigned int nItem;
        unsigned long nData;

    private:
};


// Time Series Reader

class TimeSeri : public DataItem
{
    public:
        TimeSeri();
        bool shuffle;
        bool LoadRawData(char *file);
        bool WriteData(char *file,float pTrain);
        bool WriteScaleParameters(char *FileName);
        unsigned long GetNData(){
           return (count-nInput-nOutput);
        }
        virtual ~TimeSeri();

//    protected:
        long count;
        int nInput;
        int nOutput;
    private:
};


#endif // DATAPROCESS_H
