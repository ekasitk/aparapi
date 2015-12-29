#ifndef APARAPIBUFFERMANAGER_H
#define APARAPIBUFFERMANAGER_H

#include <map>
#include <set>

#include "Common.h"
#include "AparapiBuffer.h"

class AparapiBufferManager {

private: 

   // A map to find shared aparapiBuffer for each opencl context
   static std::map<cl_context, std::set<AparapiBuffer*> > aparapiBufferMap;

   // Keep track of reference count
   static std::map<AparapiBuffer*, int> refCountMap;

public: 
   static AparapiBuffer* getOrCreateAparapiBuffer(JNIEnv *jenv, cl_context context, jobject javaBuffer);
   static void detach(JNIEnv *jenv, cl_context context, AparapiBuffer *aparapiBuffer); 

};

#endif // APARAPIBUFFERMANAGER_H
