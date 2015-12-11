
#include "ArrayBufferManager.h"
#include "Config.h"

#include <map>
#include <set>

// A map to find shared arrayBuffer for each opencl context
std::map<cl_context, std::set<ArrayBuffer*> > arrayBufferMap;


/**
 * find or create the arguement that has shared arrayBuffer with other Kernels
 *
 * @param context The opencl context we're working in
 * @param javaArray The java array 
 *
 * @return the shared arrayBuffer if existed, or NULL
 *
 * TEST WITH 1D primitive array, not 1D object array
 * NOT SUPPORT KERNEL.DISPOSE() GRACEFULLY
 */
ArrayBuffer* getSharedArrayBuffer(JNIEnv *jenv, cl_context context, jarray javaArray) {
   ArrayBuffer *arrayBuffer = NULL;
   if ((context != NULL) && (arrayBufferMap.count(context) > 0)) { 
      std::set<ArrayBuffer *> arrayBufferSet = arrayBufferMap[context]; // return existing or create an empty arrayBufferSet
      std::set<ArrayBuffer *>::iterator it;
      for (it = arrayBufferSet.begin(); it != arrayBufferSet.end(); it++) {
         if (jenv->IsSameObject(javaArray, (*it)->javaArray)) {
            if (config->isVerbose()){
               fprintf(stderr,"get shared arrayBuffer at %p from context %p\n", (*it),context);
            }
            return (*it);
         }
      }
   } 
   return NULL;
}

void setSharedArrayBuffer(cl_context context, jarray javaArray, ArrayBuffer *arrayBuffer) {
   if (context != NULL) {
      std::set<ArrayBuffer *>& arrayBufferSet = arrayBufferMap[context]; // return existing or create an empty arrayBufferSet
      arrayBufferSet.insert(arrayBuffer);
      if (config->isVerbose()){
         fprintf(stderr,"set shared arrayBuffer at %p for context %p\n", arrayBuffer,context);
      }
   } 
}

