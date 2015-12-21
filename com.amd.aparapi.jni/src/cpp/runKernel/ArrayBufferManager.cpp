
#include "ArrayBufferManager.h"
#include "Config.h"
#include "Common.h"
#include "List.h"

// Initialize arrayBufferMap 
std::map<cl_context, std::set<ArrayBuffer*> > ArrayBufferManager::arrayBufferMap;

// Initialized refCountMap
std::map<ArrayBuffer*, int> ArrayBufferManager::refCountMap;

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
// deprecated
ArrayBuffer* ArrayBufferManager::getSharedArrayBuffer(JNIEnv *jenv, cl_context context, jarray javaArray) {
   ArrayBuffer *arrayBuffer = NULL;
   if ((context != NULL) && (arrayBufferMap.count(context) > 0)) { 
      std::set<ArrayBuffer *>& arrayBufferSet = arrayBufferMap[context]; // return existing or create an empty arrayBufferSet
      std::set<ArrayBuffer *>::iterator it;
      for (it = arrayBufferSet.begin(); it != arrayBufferSet.end(); it++) {
         // simply checking (*it)->javaArray == javaArray won't work
         //  as javaArray may be changed from different GetObjectField call
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

// deprecated
void ArrayBufferManager::setSharedArrayBuffer(cl_context context, jarray javaArray, ArrayBuffer *arrayBuffer) {
   if (context != NULL) {
      std::set<ArrayBuffer *>& arrayBufferSet = arrayBufferMap[context]; // return existing or create an empty arrayBufferSet. Notice the &arrayBufferSet
      arrayBufferSet.insert(arrayBuffer);
      if (config->isVerbose()){
         fprintf(stderr,"set shared arrayBuffer at %p for context %p\n", arrayBuffer,context);
      }
   } 
}


ArrayBuffer* ArrayBufferManager::getOrCreateArrayBuffer(JNIEnv *jenv, cl_context context, jarray javaArray) {
   if (context == NULL) {
      fprintf(stderr,"cl_context must not be NULL!\n");
      return NULL;
   }

   ArrayBuffer *arrayBuffer = NULL;
   std::set<ArrayBuffer*>& arrayBufferSet = arrayBufferMap[context]; // return existing or create an empty arrayBufferSet. Notice the &arrayBufferSet
   std::set<ArrayBuffer*>::iterator it;
   //fprintf(stderr,"looking for shared arrayBuffer for javaArray %p\n", javaArray);
   for (it = arrayBufferSet.begin(); it != arrayBufferSet.end(); it++) {
      // simply checking (*it)->javaArray == javaArray won't work
      //  as javaArray may be changed from different GetObjectField call
      if (jenv->IsSameObject(javaArray, (*it)->javaArray)) {
         if (config->isVerbose()){
            fprintf(stderr,"reuse arrayBuffer at %p in context %p\n", (*it),context);
         }
         arrayBuffer = (*it);
         int count = refCountMap[arrayBuffer];
         refCountMap[arrayBuffer] = count + 1;
         return arrayBuffer;
      }
   }

   arrayBuffer = new ArrayBuffer();
   arrayBufferSet.insert(arrayBuffer);
   refCountMap[arrayBuffer] = 1;
   if (config->isVerbose()){
      fprintf(stderr,"create arrayBuffer at %p in context %p\n", arrayBuffer,context);
   }

   return arrayBuffer;
}

void ArrayBufferManager::dispose(JNIEnv *jenv, cl_context context, ArrayBuffer *arrayBuffer) {
   if (context == NULL) {
      fprintf(stderr,"cl_context must not be NULL!\n");
      return;
   }
   int count = refCountMap[arrayBuffer];
   count = count - 1;
   if (count == 0) {
      if (arrayBuffer->mem != 0){
         if (config->isTrackingOpenCLResources()){
            memList.remove((cl_mem)arrayBuffer->mem, __LINE__, __FILE__);
         }
         cl_int status = clReleaseMemObject((cl_mem)arrayBuffer->mem);
         //fprintf(stderr, "clReleaseMemObject %0lx\n", arrayBuffer->mem);
         CLException::checkCLError(status, "clReleaseMemObject()");
         arrayBuffer->mem = (cl_mem)0;
      }
      if (arrayBuffer->javaArray != NULL)  {
         jenv->DeleteWeakGlobalRef((jweak) arrayBuffer->javaArray);
      }
      if (config->isVerbose()){
         fprintf(stderr, "destroy arrayBuffer at %p in context %p\n", arrayBuffer, context);
      }

      std::set<ArrayBuffer*>& arrayBufferSet = arrayBufferMap[context]; // return existing or create an empty arrayBufferSet. Notice the &arrayBufferSet
      arrayBufferSet.erase(arrayBuffer);

      refCountMap.erase(arrayBuffer);
      delete arrayBuffer;

   } else {
      refCountMap[arrayBuffer] = count;
      if (config->isVerbose()){
         fprintf(stderr, "detach arrayBuffer at %p in context %p\n", arrayBuffer, context);
      }
   }

}
