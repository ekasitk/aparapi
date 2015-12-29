
#include "AparapiBufferManager.h"
#include "Config.h"
#include "Common.h"
#include "List.h"

// Initialize aparapiBufferMap 
std::map<cl_context, std::set<AparapiBuffer*> > AparapiBufferManager::aparapiBufferMap;

// Initialized refCountMap
std::map<AparapiBuffer*, int> AparapiBufferManager::refCountMap;

/**
 * find or create the arguement that has shared aparapiBuffer with other Kernels
 *
 * @param context The opencl context we're working in
 * @param javaBuffer The javaBuffer 
 *
 * @return the shared aparapiBuffer if existed, or create a new one
 *
 */
AparapiBuffer* AparapiBufferManager::getOrCreateAparapiBuffer(JNIEnv *jenv, cl_context context, jobject javaBuffer) {
   if (context == NULL) {
      fprintf(stderr,"cl_context must not be NULL!\n");
      return NULL;
   }

   AparapiBuffer *aparapiBuffer = NULL;
   std::set<AparapiBuffer*>& aparapiBufferSet = aparapiBufferMap[context]; // return existing or create an empty aparapiBufferSet. Notice the &aparapiBufferSet
   std::set<AparapiBuffer*>::iterator it;
   //fprintf(stderr,"looking for shared aparapiBuffer for javaBuffer %p\n", javaBuffer);
   for (it = aparapiBufferSet.begin(); it != aparapiBufferSet.end(); it++) {
      if (jenv->IsSameObject(javaBuffer, (*it)->javaObject)) {
         if (config->isVerbose()){
            fprintf(stderr,"reuse aparapiBuffer at %p in context %p\n", (*it),context);
         }
         aparapiBuffer = (*it);
         int count = refCountMap[aparapiBuffer];
         refCountMap[aparapiBuffer] = count + 1;
         return aparapiBuffer;
      }
   }

   aparapiBuffer = new AparapiBuffer();
   aparapiBufferSet.insert(aparapiBuffer);
   refCountMap[aparapiBuffer] = 1;
   if (config->isVerbose()){
      fprintf(stderr,"create aparapiBuffer at %p in context %p\n", aparapiBuffer,context);
   }

   return aparapiBuffer;
}

void AparapiBufferManager::detach(JNIEnv *jenv, cl_context context, AparapiBuffer *aparapiBuffer) {
   if (context == NULL) {
      fprintf(stderr,"cl_context must not be NULL!\n");
      return;
   }
   int count = refCountMap[aparapiBuffer];
   count = count - 1;
   if (count == 0) {
      if (aparapiBuffer->mem != 0){
         if (config->isTrackingOpenCLResources()){
            memList.remove((cl_mem)aparapiBuffer->mem, __LINE__, __FILE__);
         }
         cl_int status = clReleaseMemObject((cl_mem)aparapiBuffer->mem);
         //fprintf(stderr, "clReleaseMemObject %0lx\n", aparapiBuffer->mem);
         CLException::checkCLError(status, "clReleaseMemObject()");
         aparapiBuffer->mem = (cl_mem)0;
      }
      if (aparapiBuffer->javaObject != NULL)  {
         jenv->DeleteWeakGlobalRef((jweak) aparapiBuffer->javaObject);
      }

      if (config->isVerbose()){
         fprintf(stderr, "dispose aparapiBuffer at %p in context %p\n", aparapiBuffer, context);
      }

      std::set<AparapiBuffer*>& aparapiBufferSet = aparapiBufferMap[context]; // return existing or create an empty aparapiBufferSet. Notice the &aparapiBufferSet
      aparapiBufferSet.erase(aparapiBuffer);

      refCountMap.erase(aparapiBuffer);
      delete aparapiBuffer;

   } else {
      refCountMap[aparapiBuffer] = count;
      if (config->isVerbose()){
         fprintf(stderr, "detach aparapiBuffer at %p in context %p\n", aparapiBuffer, context);
      }
   }

}
