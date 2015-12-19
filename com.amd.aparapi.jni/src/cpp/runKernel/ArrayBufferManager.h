#ifndef ARRAYBUFFERMANAGER_H
#define ARRAYBUFFERMANAGER_H

#include <map>
#include <set>

#include "Common.h"
#include "ArrayBuffer.h"

class ArrayBufferManager {

private: 

   // A map to find shared arrayBuffer for each opencl context
   static std::map<cl_context, std::set<ArrayBuffer*> > arrayBufferMap;


public: 
   static ArrayBuffer* getSharedArrayBuffer(JNIEnv * jenv, cl_context context, jarray javaArray);
   static void setSharedArrayBuffer(cl_context context, jarray javaArray, ArrayBuffer *arrayBuffer);

};

#endif // ARRAYBUFFERMANAGER_H
