#include <jni.h>
#include <string>
#include "leak-tracer/include/MemoryTrace.hpp"

std::string leakReportFile;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    leaktracer::MemoryTrace::GetInstance().startMonitoringAllThreads();
    malloc(100);
    std::string hello = "Hello from C++";
    leaktracer::MemoryTrace::GetInstance().stopAllMonitoring();
    if (!leakReportFile.empty()) {
        leaktracer::MemoryTrace::GetInstance().writeLeaksToFile(leakReportFile.c_str());
    }
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_MainActivity_initLeakTracer(JNIEnv *env, jobject, jstring filePath) {
    const char *path = env->GetStringUTFChars(filePath, JNI_FALSE);
    leakReportFile = path;
    env->ReleaseStringUTFChars(filePath, path);
}