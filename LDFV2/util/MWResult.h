#ifndef MWRESULT_H
#define MWRESULT_H

#include <list>
#include <string>
#include <iostream>
using namespace std;

#include <stdint.h>
#include "BarcodeScanner.h"

class PointF
{
public:
    PointF();
    float getX(void ) { return x;}
    void setX(float x) { this->x = x; }
    float getY(void) { return y; }
    void setY(float y) { this->y = y; }

//private:
    float x;
    float y;
};

class MWLocation
{

public:
    MWLocation(void);
    ~MWLocation(void);

    MWLocation(float *points);

    PointF * getPoint1(void) {return p1;}
    PointF * getPoint2(void){return p2;}
    PointF * getPoint3(void) {return p3;}
    PointF * getPoint4(void) {return p4;}

    PointF * getPoints(void) { return points; }
//private:
    PointF *p1;
    PointF *p2;
     PointF *p3;
     PointF *p4;

    PointF *points;
};


class MWResult
{
public:
    MWResult();
    ~MWResult();
//    MWResult(uint8_t *);
    string getText(void) { return text; }
    void setText(string txt) { text = txt; }

    char * getBytes(void) { return bytes;}
    void setBytes(char * array);

    int getBytesLength(void) { return bytesLength; }
    void setBytesLength(int len) { bytesLength = len; }

    int getType(void){ return type; }
    void setType(int type) { this->type = type; }
    
    string getTypeName(void){ return typeName; }
    void setTypeName(string typeName) { this->typeName = typeName; }

    int getSubType(void){ return subtype; }
    void setSybType(int sybtype) { this->subtype = subtype; }

    int getImageWidth(void) { return imageWidth; }
    void setImageWidth(int width) { imageWidth = width; }

    int getImageHeight(void) { return imageHeight; }
    void setImageHeight(int height) { imageHeight = height; }

    bool isGs1(void) { return isGS1; }
    void setGs1(bool t) { isGS1 = t; }

    void setLocationPoints(MWLocation *points) { locationPoints = points; }
    MWLocation *getLocationPoints(void ) { return locationPoints; }

    char * getParserInput(void) { return parserInput; }
    void setParserInput(char *array, int len);
    
    int getModulesCountX(void) { return modulesCountX; }
    void setModulesCountX(int _modulesCountX) { modulesCountX = _modulesCountX; }
    
    int getModulesCountY(void) { return modulesCountY; }
    void setModulesCountY(int _modulesCountY) { modulesCountY = _modulesCountY; }

    
    float getModuleSizeX(void) { return moduleSizeX; }
    void setModuleSizeX(float _moduleSizeX) { moduleSizeX = _moduleSizeX; }
    
    float getModuleSizeY(void) { return moduleSizeY; }
    void setModuleSizeY(float _moduleSizeY) { moduleSizeY = _moduleSizeY; }
    
    float getSkew(void) { return skew; }
    void setSkew(float _skew) { skew = _skew; }


//private:
            string text;
            string typeName;
            char *bytes;
            int bytesLength;
            int type;
            int subtype;
            int imageWidth;
            int imageHeight;
            bool isGS1;
            MWLocation *locationPoints;
            char *parserInput;
    
            int modulesCountX;
            int modulesCountY;
            float moduleSizeX;
            float moduleSizeY;
            float skew;
    
   };

class MWResults
{
public:
    MWResults();
    ~MWResults();
    MWResults(uint8_t *buffer);
    int getVersion(void) { return version; }
    void setVersion(int version) { this->version = version; }

    int getCount(void) { return count; }
    void setCount(int count) { this->count = count; }

    list<MWResult *> getResults(void) { return results; }

private:
    int version;
    int count;
    list<MWResult *> results;
};




#endif // MWRESULT_H
