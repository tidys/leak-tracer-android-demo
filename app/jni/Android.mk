LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := jni
LOCAL_SRC_FILES := ../src/main/cpp/native-lib.cpp
LOCAL_LDFLAGS += -Wl,-Bsymbolic
#--unresolved-symbols=ignore-all
LOCAL_CFLAGS += -fPIC -g
LOCAL_CPPFLAGS += -g

LOCAL_SHARED_LIBRARIES += leaktracer

LOCAL_LDLIBS := -landroid -llog
# include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,$(LOCAL_PATH))

$(call import-module,leak-tracer)

