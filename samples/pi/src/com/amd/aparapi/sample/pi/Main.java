/*
 * Pi calculation using 2 kernels: map and reduce.
 */

package com.amd.aparapi.sample.pi;

import com.amd.aparapi.Kernel;
import com.amd.aparapi.Kernel.KernelState;
import com.amd.aparapi.Range;
import com.amd.aparapi.device.Device;
import com.amd.aparapi.device.OpenCLDevice;
import com.amd.aparapi.internal.opencl.OpenCLContext;

public class Main{

   public static void main(String[] args) {

      final int size = 100000000;

      final int[] data = new int[size];

      Device device = Device.bestGPU();
      System.out.println(device);
      OpenCLContext.createSharedContext((OpenCLDevice) device); 
      System.out.println(OpenCLContext.getSharedContext());

      Kernel map = new Kernel() {

         public static final int m = 2147483647;

         public int randomInt(int seed) {
           int a = (int) ((seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1));
           if (a < 0) a = -a;
           return a;
         }
         
         @Override public void run() {
            int gid = getGlobalId();      
            int rnd = randomInt(gid);
            float x = ((float) rnd)/m * 2 - 1; 
            rnd = randomInt(rnd);
            float y = ((float) rnd)/m * 2 - 1;
            data[gid] = ((x * x + y * y) < 1) ? 1 : 0;
         } 
      };
      int numThreads = 1024;
      final int[] count = new int[numThreads];

      Kernel reduce = new Kernel(){
         @Override public void run() {
            int gid = getGlobalId();
            int localSum = count[gid];
            int globalSize = getGlobalSize();
            for (int idx = gid; idx < size; idx += globalSize) {
               localSum += data[idx];
            }
            count[gid] = localSum;
         }
      }; 

      long st = System.currentTimeMillis();
      map.setExplicit(true);
for (int loop=0; loop < 10; loop++) {
      map.execute(size);
      map.get(data); // no need to pull data from device mem

      // host mem is changed, but not effect to the device mem
      //for (int j = 0; j < size; j++) data[j] = 2; 

      reduce.setExplicit(true);
      reduce.put(data);  // already in device mem, no need to put
      reduce.execute(numThreads);
      reduce.get(count);

      int total = 0;
      for (int i = 0; i < numThreads; i++) {
         total += count[i];
      }
      System.out.println("Pi = " + 4*((float) total)/size);
}
      long et = System.currentTimeMillis();
      System.out.println("elapse time = " + (et-st));
//      map.dispose(); // dispose() not safe now
//      reduce.dispose();  // dispose() not safe now
   }

}
