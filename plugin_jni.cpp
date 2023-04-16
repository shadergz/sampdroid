#include <jni.h>

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    const jint useVersion{JNI_VERSION_1_6};
    return useVersion;
}