#ifndef ARRAYBUFFERMANAGER_H
#define ARRAYBUFFERMANAGER_H

#include "Common.h"
#include "ArrayBuffer.h"

ArrayBuffer* getSharedArrayBuffer(JNIEnv * jenv, cl_context context, jarray javaArray);
void setSharedArrayBuffer(cl_context context, jarray javaArray, ArrayBuffer *arrayBuffer);


#endif // ARRAYBUFFERMANAGER_H
