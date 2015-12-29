java \
 -Djava.library.path=../../com.amd.aparapi.jni/dist \
 -Dcom.amd.aparapi.executionMode=$1 \
 -Dcom.amd.aparapi.logLevel=WARNING \
 -Dcom.amd.aparapi.enableVerboseJNI=true \
 -Dcom.amd.aparapi.enableExecutionModeReporting=true \
 -classpath ../../com.amd.aparapi/dist/aparapi.jar:lr.jar \
 com.amd.aparapi.sample.lr.Main
