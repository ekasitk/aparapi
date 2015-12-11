java \
 -Djava.library.path=../../com.amd.aparapi.jni/dist \
 -Dcom.amd.aparapi.executionMode=$1 \
 -Dcom.amd.aparapi.logLevel=WARNING \
 -Dcom.amd.aparapi.enableVerboseJNI=false \
 -Dcom.amd.aparapi.enableExecutionModeReporting=true \
 -classpath ../../com.amd.aparapi/dist/aparapi.jar:pi.jar \
 com.amd.aparapi.sample.pi.Main
