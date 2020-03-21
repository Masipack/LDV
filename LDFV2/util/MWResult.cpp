#include "MWResult.h"
#include  <stdlib.h>
#include <string.h>


PointF::PointF(void) :
    x(0), y(0)
{

}

MWLocation::MWLocation()
{

}
MWLocation::MWLocation(float * _points)
{

        points = new PointF[4];

        for (int i = 0; i < 4; i++)
        {
            points[i].setX(_points[i * 2]);
            points[i].setY(_points[i * 2 + 1]);
        }

        p1 = new PointF();
        p2 = new PointF();
        p3 = new PointF();
        p4 = new PointF();

        p1->setX( _points[0]);
        p1->setY( _points[1]);
        p2->setX(_points[2]);
        p2->setY(_points[3]);
        p3->setX(_points[4]);
        p3->setY( _points[5]);
        p4->setX( _points[6]);
        p4->setY(_points[7]);
}

MWLocation::~MWLocation()
{
    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete[] points;
}

MWResult::MWResult() :
    text(""), bytes(NULL), bytesLength(0), type(0), subtype(0), imageWidth(0), imageHeight(0), isGS1(false), locationPoints()
{
       // cout << "MWResult initialized " << endl;
    locationPoints = NULL;
    bytes = NULL;
    parserInput = NULL;
    locationPoints = NULL;
}

MWResult::~MWResult(){
    if (locationPoints != NULL){
        delete locationPoints;
    }
    if (bytes != NULL){
        free(bytes);
    }
    if (parserInput != NULL){
        free(parserInput);
    }
    
}


//MWResult::MWResult(uint8_t *) :
//    text(""), bytes(NULL), bytesLength(0), type(0), subtype(0), imageWidth(0), imageHeight(0), isGS1(false)
//{
//        cout << "MWResult initialized " << endl;
//}

MWResults::~MWResults()
{
   // for (int i = 0; i < count; i++){
        //results.clear();
   // }
    
    for (std::list<MWResult*>::iterator it=results.begin(); it!=results.end(); ++it){
        delete *it;
    }
        
}

MWResults::MWResults() :
    version(0), count(0), results()
{

}



string getTypeName (int typeID) {
    string typeName = "Unknown";
    switch (typeID) {
        case FOUND_25_INTERLEAVED: typeName = "Code 25 Interleaved";break;
        case FOUND_25_STANDARD: typeName = "Code 25 Standard";break;
        case FOUND_128: typeName = "Code 128";break;
        case FOUND_128_GS1: typeName = "Code 128 GS1";break;
        case FOUND_39: typeName = "Code 39";break;
        case FOUND_93: typeName = "Code 93";break;
        case FOUND_AZTEC: typeName = "AZTEC";break;
        case FOUND_DM: typeName = "Datamatrix";break;
        case FOUND_QR: typeName = "QR";break;
        case FOUND_EAN_13: typeName = "EAN 13";break;
        case FOUND_EAN_8: typeName = "EAN 8";break;
        case FOUND_NONE: typeName = "None";break;
        case FOUND_RSS_14: typeName = "Databar 14";break;
        case FOUND_RSS_14_STACK: typeName = "Databar 14 Stacked";break;
        case FOUND_RSS_EXP: typeName = "Databar Expanded";break;
        case FOUND_RSS_LIM: typeName = "Databar Limited";break;
        case FOUND_UPC_A: typeName = "UPC A";break;
        case FOUND_UPC_E: typeName = "UPC E";break;
        case FOUND_PDF: typeName = "PDF417";break;
        case FOUND_MICRO_PDF: typeName = "Micro PDF417";break;
        case FOUND_CODABAR: typeName = "Codabar";break;
        case FOUND_DOTCODE: typeName = "Dotcode";break;
        case FOUND_11: typeName = "Code 11";break;
        case FOUND_MSI: typeName = "MSI Plessey";break;
        case FOUND_25_IATA: typeName = "IATA Code 25";break;
        case FOUND_ITF14: typeName = "ITF 14";break;
        case FOUND_25_MATRIX: typeName = "Code 2/5 Matrix";break;
        case FOUND_25_COOP: typeName = "Code 2/5 COOP";break;
        case FOUND_25_INVERTED: typeName = "Code 2/5 Inverted";break;
        case FOUND_MAXICODE: typeName = "Maxicode";break;
        case FOUND_QR_MICRO: typeName = "QR Micro";break;
        case FOUND_POSTNET: typeName = "Postnet"; break;
        case FOUND_PLANET: typeName = "Planet"; break;
        case FOUND_IMB: typeName = "Intelligent mail"; break;
        case FOUND_ROYALMAIL: typeName = "Royal mail"; break;
        case FOUND_32: typeName = "Code 32"; break;
    }
    
    return typeName;
}


MWResults::MWResults(uint8_t *buffer) :
    version(0), count(0), results()
{
    setCount(0);
    if (buffer[0] != 'M' || buffer[1] != 'W' || buffer[2] != 'R')
            {
                    return;
            }

            setVersion(buffer[3]);
            setCount(buffer[4]);
            int currentPos = 5;
    

            for (int i = 0; i < count; i++)
            {
                           MWResult *result = new MWResult();

                           int fieldsCount = buffer[currentPos];
                           currentPos++;
                           for (int f = 0; f < fieldsCount; f++){
                               int fieldType = buffer[currentPos];
                               int fieldNameLength = buffer[currentPos + 1];
                               int fieldContentLength = 256 * (buffer[currentPos + 3 + fieldNameLength] & 0xFF) + (buffer[currentPos + 2 + fieldNameLength]& 0xFF);
                               string *fieldName = NULL;

                               if (fieldNameLength > 0){
                                   char *ptr = (char *)malloc(fieldNameLength+1);
                                   if(!ptr)
                                   {
                                       cout << "Can not allocate memory!!" << endl;
                                       return;
                                   }
                                   strncpy(ptr, (char*)buffer+currentPos+2,fieldNameLength );
                                   ptr[fieldNameLength] = 0;
                                   fieldName = new string(ptr);
                                   free(ptr);
                               }

                               int contentPos = currentPos + fieldNameLength + 4;
                               
                               switch (fieldType)
                               {
                                   case MWB_RESULT_FT_TYPE:
                                              result->setType(*(int*)&buffer[contentPos]);
                                              result->setTypeName(getTypeName(result->type));
                                       break;
                                   case MWB_RESULT_FT_SUBTYPE:
                                            result->setSybType(*(int*)&buffer[contentPos]);
                                       break;
                                   case MWB_RESULT_FT_ISGS1:
                                            result->setGs1((*(int*)&buffer[contentPos])  == 1);
                                       break;
                                   case MWB_RESULT_FT_IMAGE_WIDTH:
                                    {
                                            result->setImageWidth(*(int*)&buffer[contentPos]);
                                            break;
                                    }
                                    case MWB_RESULT_FT_IMAGE_HEIGHT:
                                    {
                                     result->setImageHeight(*(int*)&buffer[contentPos]);
                                        break;
                                    }

                                   case MWB_RESULT_FT_LOCATION:
                                    {
                                       
                                        float *locations = (float *) malloc(8 * sizeof(float));
                                        for (int l = 0; l < 8; l++)
                                       {
                                           locations[l] =  *(float *)&buffer[contentPos+l*4];
                                        }

                                       MWLocation *l = new MWLocation(locations);
                                        free(locations);
                                       result->setLocationPoints(l);

                                       break;
                                    }
                                   case MWB_RESULT_FT_TEXT:
                                   {
                                            char *ptr = (char *)malloc(fieldContentLength+1);
                                            if(!ptr)
                                            {
                                                cout << "Can not allocate memory!!" << endl;
                                                return;
                                            }
                                            strncpy(ptr, (char*)buffer+contentPos,fieldContentLength );
                                            ptr[fieldContentLength] = 0;

                                            string tmp(ptr);
                                            result->setText(tmp);
                                            free(ptr);
                                            break;
                                    }

                                   case MWB_RESULT_FT_BYTES:
                                    {
                                            result->setBytesLength(fieldContentLength);
                                            result->setBytes((char*)buffer+contentPos);

                                       break;
                                    }
                                   case MWB_RESULT_FT_PARSER_BYTES:
                               {
//                                                           result.parserInput = new String(buffer, contentPos, fieldContentLength);
                                            result->setParserInput((char *)buffer+contentPos, fieldContentLength);
                                           break;
                               }
                                       
                                   case MWB_RESULT_FT_MODULES_COUNT_X:
                                       result->setModulesCountX(*(int*)&buffer[contentPos]);
                                       break;
                                   case MWB_RESULT_FT_MODULES_COUNT_Y:
                                        result->setModulesCountY(*(int*)&buffer[contentPos]);
                                        break;
                                   case MWB_RESULT_FT_MODULE_SIZE_X:
                                       result->setModuleSizeX(*(float*)&buffer[contentPos]);
                                       break;
                                   case MWB_RESULT_FT_MODULE_SIZE_Y:
                                       result->setModuleSizeY(*(float*)&buffer[contentPos]);
                                       break;
                                   case MWB_RESULT_FT_SKEW:
                                       result->setSkew(*(float*)&buffer[contentPos]);
                                       break;
                                       
                                       
                               default:
                                   break;
                               }
                               currentPos += (fieldNameLength + fieldContentLength + 4);
                               delete fieldName;
                           }
                            results.push_back(result);
           }
}



void MWResult::setBytes(char * array)
{
    int i;
    int bytesLen = getBytesLength();
    locationPoints = NULL;

    bytes = (char *) malloc(bytesLen);
    if(!bytes)
    {
        cout << "Can not allocate memory!!!" << endl;
        return;
    }
    memcpy(bytes, array, bytesLen);
}
void MWResult::setParserInput(char *array, int len)
{
    parserInput = (char *) malloc(len+1);
    if(!parserInput)
    {
        cout << "Not enought memory!!" << endl;
        return;
    }
    memcpy(parserInput, array, len);
    parserInput[len] = '\0';
}

