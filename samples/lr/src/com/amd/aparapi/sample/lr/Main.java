/*
 * Logistic regression using 2 kernels: map and reduce.
 * For 10 iterations,
 * Final weights = 237.54578, 278.3174, 372.1334, 363.333, 280.15613, 214.93636, 330.18307, 382.141, 319.41797, 258.59735,
 */

package com.amd.aparapi.sample.lr;

import java.util.*;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;

import com.amd.aparapi.Kernel;
import com.amd.aparapi.Kernel.KernelState;
import com.amd.aparapi.Range;

public class Main{
   final static int dimension = 10;
   final static Random rand = new Random(42); 

   public static void main(String[] args) throws Exception {
      System.setProperty("com.amd.aparapi.enableBufferSharing","true");

      final float[] w = new float[dimension];
      for (int i = 0; i < dimension; i++) {
         w[i] = (float) (2*rand.nextDouble()-1);
      }
      System.out.print("Initial weights = ");
      for (int i = 0; i < dimension; i++) {
         System.out.print(w[i] + ", ");
      }
      System.out.println("\n");

      List<String> lines = Files.readAllLines(Paths.get("lr_data.txt"),
                                   Charset.defaultCharset());
      final int size = lines.size();
      final float[][] data = new float[size][]; 

      int count = 0;
      for (String line: lines) {
         data[count] = parseLine(line);
         count++;
      }
  
      final float[][] gradient = new float[size][dimension];

      Kernel map = new Kernel() {

         public float dot(float[] a, float[] b) { 
            float x = 0;
            for (int i = 0; i < dimension; i++) {
               x += a[i]*b[i+1];  // b[0] is y
            }
            return x; 
         }

         @Override public void run() {
            int gid = getGlobalId();      
            float y = data[gid][0];
            float dot = dot(w,data[gid]);
            for (int i = 0; i < dimension; i++) {
               gradient[gid][i] = (1 / (1 + (float) Math.exp(-y * dot)) - 1) * y * data[gid][i+1];
            }
         } 
      };

      int numThreads = 1024;
      final float[][] sum = new float[numThreads][dimension];

      Kernel reduce = new Kernel(){
         @Override public void run() {
            int gid = getGlobalId();
            float[] localSum = sum[gid];
            for (int i = 0; i < dimension; i++) {
               localSum[i] = 0;
            }
            int globalSize = getGlobalSize();
            for (int idx = gid; idx < size; idx += globalSize) {
               for (int i = 0; i < dimension; i++) {
                  localSum[i] += gradient[idx][i];
               }
            }
         }
      }; 

      long st = System.currentTimeMillis();
      map.setExplicit(true);   // cannot use false because it doesn't know if w is changed. 
      map.put(data);
for (int loop=0; loop < 10; loop++) {
      map.put(w);
      map.execute(size);
//      map.get(gradient); 

      // host mem is changed, but not effect to the device mem
/*
      for (int i = 0; i < size; i++) {
         for (int j = 0; j < dimension; j++) {
            gradient[i][j] = 1.0f;
         }
      } 
*/
      
      reduce.setExplicit(true);
//      reduce.put(gradient);  
      reduce.execute(numThreads);
      reduce.get(sum);

      float[] total = new float[dimension];
      for (int i = 0; i < numThreads; i++) {
         for (int j = 0; j < dimension; j++) {
            total[j] += sum[i][j];
         }
      }

      for (int i = 0; i < dimension; i++) {
         w[i] -= total[i];
      }
      System.out.println("Finish iteration = " + loop);
      System.out.print("weights = ");
      for (int i = 0; i < dimension; i++) {
         System.out.print(w[i] + ", ");
      }
      System.out.println("\n");

}

      System.out.print("Final weights = ");
      for (int i = 0; i < dimension; i++) {
         System.out.print(w[i] + ", ");
      }
      System.out.println("\n");

      long et = System.currentTimeMillis();
      System.out.println("elapse time = " + (et-st));
      map.dispose(); 
      reduce.dispose();  
   }

   public static float[] parseLine(String line) {
      String[] tok = line.split(" ");
      float[] point = new float[dimension+1]; // first is y
      point[0] = Float.parseFloat(tok[0]);
      for (int i = 1; i < dimension+1; i++) {
        point[i] = Float.parseFloat(tok[i]);
      }
      return point;
   }

}
