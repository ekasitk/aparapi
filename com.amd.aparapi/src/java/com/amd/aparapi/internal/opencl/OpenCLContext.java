package com.amd.aparapi.internal.opencl;

import com.amd.aparapi.*;

import java.io.*;
import java.lang.annotation.*;
import java.lang.reflect.*;
import java.util.*;
import java.util.logging.*;

import com.amd.aparapi.device.OpenCLDevice;
import com.amd.aparapi.internal.jni.OpenCLJNI;

public class OpenCLContext extends OpenCLJNI {

   private final long contextId;
   private final OpenCLDevice device;

   private static Logger logger = Logger.getLogger(Config.getLoggerName());

   /**
    * Minimal constructor
    * 
    * @param _contextId
    * @param _device
    */
   public OpenCLContext(long _contextId, OpenCLDevice _device) {
      contextId = _contextId;
      device = _device;
   }

   public long getContextId() {
      return (contextId);
   }

   public OpenCLDevice getDevice() {
      return device;
   }

   @Override
   public String toString() {
      return "OpenCLContext cl_context " + contextId + " on device " +  device.getShortDescription();
   }

   public static OpenCLContext createContext(OpenCLDevice _device) {
      return createContext(_device);
   }

}
