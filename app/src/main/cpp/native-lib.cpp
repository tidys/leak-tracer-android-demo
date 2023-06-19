#include <jni.h>
#include <string>
#include "leak-tracer/include/MemoryTrace.hpp"
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    leaktracer::MemoryTrace::GetInstance().startMonitoringAllThreads();
    malloc(100);
    std::string hello = "Hello from C++";
    leaktracer::MemoryTrace::GetInstance().stopAllMonitoring();

    return env->NewStringUTF(hello.c_str());
}