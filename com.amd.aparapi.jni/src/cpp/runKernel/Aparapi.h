/*
   Copyright (c) 2010-2011, Advanced Micro Devices, Inc.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
   following conditions are met:

   Redistributions of source code must retain the above copyright notice, this list of conditions and the following
   disclaimer. 

   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
   disclaimer in the documentation and/or other materials provided with the distribution. 

   Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission. 

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   If you use the software (in whole or in part), you shall adhere to all applicable U.S., European, and other export
   laws, including but not limited to the U.S. Export Administration Regulations ("EAR"), (15 C.F.R. Sections 730 
   through 774), and E.U. Council Regulation (EC) No 1334/2000 of 22 June 2000.  Further, pursuant to Section 740.6 of
   the EAR, you hereby certify that, except pursuant to a license granted by the United States Department of Commerce
   Bureau of Industry and Security or as otherwise permitted pursuant to a License Exception under the U.S. Export 
   Administration Regulations ("EAR"), you will not (1) export, re-export or release to a national of a country in 
   Country Groups D:1, E:1 or E:2 any restricted technology, software, or source code you receive hereunder, or (2) 
   export to Country Groups D:1, E:1 or E:2 the direct product of such technology or software, if such foreign produced
   direct product is subject to national security controls as identified on the Commerce Control List (currently 
   found in Supplement 1 to Part 774 of EAR).  For the most current Country Group listings, or for additional 
   information about the EAR or your obligations under those regulations, please refer to the U.S. Bureau of Industry
   and Security�s website at http://www.bis.doc.gov/. 
   */

#ifndef APARAPI_H
#define APARAPI_H


#include "Common.h"
#include "com_amd_aparapi_internal_jni_KernelRunnerJNI.h"
#include "CLException.h"
#include "Range.h"
#include "KernelArg.h"
#include "JNIContext.h"

//compiler dependant code
int enqueueMarker(cl_command_queue commandQueue, cl_event* firstEvent);
jint getProcess();

/*
void idump(const char *str, void *ptr, int size);

void fdump(const char *str, void *ptr, int size);
*/

jint writeProfileInfo(JNIContext* jniContext);

cl_int profile(ProfileInfo *profileInfo, cl_event *event, jint type, char* name, cl_ulong profileBaseTime);

jint updateNonPrimitiveReferences(JNIEnv *jenv, jobject jobj, JNIContext* jniContext);

void profileFirstRun(JNIContext* jniContext);

void updateArray(JNIEnv* jenv, JNIContext* jniContext, KernelArg* arg, int& argPos, int argIdx);
void updateBuffer(JNIEnv* jenv, JNIContext* jniContext, KernelArg* arg, int& argPos, int argIdx);

void processObject(JNIEnv* jenv, JNIContext* jniContext, KernelArg* arg, int& argPos, int argIdx);
void processArray(JNIEnv* jenv, JNIContext* jniContext, KernelArg* arg, int& argPos, int argIdx);
void processBuffer(JNIEnv* jenv, JNIContext* jniContext, KernelArg* arg, int& argPos, int argIdx);

void updateWriteEvents(JNIEnv* jenv, JNIContext* jniContext, KernelArg* arg, int argIdx, int& writeEventCount);

void processLocal(JNIEnv* jenv, JNIContext* jniContext, KernelArg* arg, int& argPos, int argIdx);
void processLocalArray(JNIEnv* jenv, JNIContext* jniContext, KernelArg* arg, int& argPos, int argIdx);
void processLocalBuffer(JNIEnv* jenv, JNIContext* jniContext, KernelArg* arg, int& argPos, int argIdx);

int processArgs(JNIEnv* jenv, JNIContext* jniContext, int& argPos, int& writeEventCount);

void enqueueKernel(JNIContext* jniContext, Range& range, int passes, int argPos, int writeEventCount);

int getReadEvents(JNIContext* jniContext);

void waitForReadEvents(JNIContext* jniContext, int readEventCount, int passes);

void checkEvents(JNIEnv* jenv, JNIContext* jniContext, int writeEventCount);

void writeProfile(JNIEnv* jenv, JNIContext* jniContext);

KernelArg* getArgForBuffer(JNIEnv* jenv, JNIContext* jniContext, jobject buffer);

ArrayBuffer* getSharedArrayBuffer(JNIEnv * jenv, cl_context context, jarray javaArray);
void setSharedArrayBuffer(cl_context context, jarray javaArray, ArrayBuffer *arrayBuffer);


#endif // APARAPI_H
