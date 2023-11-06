LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := jni
LOCAL_SRC_FILES := ../src/main/cpp/native-lib.cpp
LOCAL_LDFLAGS += -Wl,-Bsymbolic
#--unresolved-symbols=ignore-all
LOCAL_CFLAGS += -fPIC -g -ffunction-sections -fdata-sections -DUSE_LEAK_TRACER=$(USE_LEAK_TRACER)
LOCAL_CPPFLAGS += -g
LOCAL_LDFLAGS += -Wl,--gc-sections
ifeq ($(USE_LEAK_TRACER), 1)
LOCAL_SHARED_LIBRARIES += leaktracer
endif

LOCAL_LDLIBS := -landroid -llog
# include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,$(LOCAL_PATH))

ifeq ($(USE_LEAK_TRACER), 1)
$(call import-module,leak-tracer)
endif
