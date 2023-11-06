#include <jni.h>
#include <string>
#include <unwind.h>
#include <android/log.h>
#include <dlfcn.h>

#if USE_LEAK_TRACER
#include "leak-tracer/include/MemoryTrace.hpp"
#endif
std::string leakReportFile;

// 测试基地址的一些代码
struct TraceHandle {
    void **backtrace;
    int pos;
};

Dl_info info;

_Unwind_Reason_Code UnwindTraceFn(_Unwind_Context *context, void *hnd) {
    _Unwind_Word ip = _Unwind_GetIP(context);
    void *addr = (void *) (ip - (_Unwind_Word) info.dli_fbase);
    __android_log_print(ANDROID_LOG_INFO, "leak-tracer", "ip: %lu, hex:%lx, addr:%p", ip, ip, addr);
    return _URC_NO_REASON;
}

void test_address() {
    // 获取基地址
    if (dladdr((void *) test_address, &info)) {
        __android_log_print(ANDROID_LOG_INFO, "leak-tracer",
                            "dli_fbase: %p, dli_fname:%s, dli_saddr:%p, dli_sname:%s",
                            info.dli_fbase, info.dli_fname, info.dli_saddr, info.dli_sname);
    }

    TraceHandle handle;
    _Unwind_Backtrace(UnwindTraceFn, &handle);

    void *pFrame = __builtin_frame_address(0);
    if (pFrame != NULL) {
        pFrame = __builtin_return_address(0);
    } else {
        pFrame = nullptr;
    }
    __android_log_print(ANDROID_LOG_INFO, "leak-tracer", "pFrame: %p", pFrame);
    // 这是一个Android ndk的c++代码，使用llvm-addr2line.exe无法识别pFrame，pFrame的可能值有0x791e0c7978
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
#if USE_LEAK_TRACER
    leaktracer::MemoryTrace::GetInstance().startMonitoringAllThreads();
    malloc(100);
    leaktracer::MemoryTrace::GetInstance().stopAllMonitoring();
    if (!leakReportFile.empty()) {
        leaktracer::MemoryTrace::GetInstance().writeLeaksToFile(leakReportFile.c_str());
    }
#endif
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_MainActivity_initLeakTracer(JNIEnv *env, jobject, jstring filePath) {
    const char *path = env->GetStringUTFChars(filePath, JNI_FALSE);
    leakReportFile = path;
//    test_address();
    env->ReleaseStringUTFChars(filePath, path);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_jni_MainActivity_callTest(JNIEnv *env, jobject thiz) {
    // test crash
#if USE_LEAK_TRACER
    __android_log_print(ANDROID_LOG_INFO, "call-test", "yes");
#else
    __android_log_print(ANDROID_LOG_INFO, "call-test", "no");
#endif
    int* ptr= nullptr;
//    *ptr;
}