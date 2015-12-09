
#ifndef OPEN_CL_CONTEXT_H
#define OPEN_CL_CONTEXT_H

#include "JNIHelper.h"

class OpenCLContext{
   public:
      static jobject create(JNIEnv *jenv, cl_context context,jobject deviceInstance) {
         return(JNIHelper::createInstance(jenv, OpenCLContextClass, 
                     ArgsVoidReturn( LongArg OpenCLDeviceClassArg) , 
                     (jlong)context, deviceInstance));
      }

      static cl_context getContextId(JNIEnv *jenv, jobject openCLContext){
         return((cl_context) JNIHelper::getInstanceField<jlong>(jenv, openCLContext, "contextId")); 
      }

      static jobject getDevice(JNIEnv *jenv, jobject openCLContext){
         return((jobject) JNIHelper::getInstanceField<jobject>(jenv, openCLContext, "device", ObjectClassArg)); 
      }
};


#endif //OPEN_CL_CONTEXT_H
