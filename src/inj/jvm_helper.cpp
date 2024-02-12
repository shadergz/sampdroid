#include <client/log_client.h>
#include <inj/jvm_helper.h>
extern JNIEnv* g_gameEnv;
extern jobject g_gtaSA;

uint getPackageIdentifier(std::span<char> packageNameId)
{
    auto gtasaClass = g_gameEnv->FindClass("core/app/Application")};
    jclass threadClass = g_gameEnv->FindClass("core/app/ActivityThread")};

    jmethodID getNameMid = g_gameEnv->GetMethodID(gtasaClass, "getPackageName", "()Ljava/lang/String;")};
    jmethodID threadMid = g_gameEnv->GetStaticMethodID(threadClass,
        "currentActivityThread", "()Landroid/app/ActivityThread;");

    jobject activityThreadObj = g_gameEnv->CallStaticObjectMethod(threadClass, threadMid);
    jmethodID getGTASAMid = g_gameEnv->GetMethodID(threadClass, "getApplication", "()Landroid/app/Application;")};
    jobject GTASAobj = g_gameEnv->CallObjectMethod(activityThreadObj, getGTASAMid);

    auto GTASAid = static_cast<jstring>(g_gameEnv->CallObjectMethod(GTASAobj, getNameMid));

    const char* redable = g_gameEnv->GetStringUTFChars(GTASAid, nullptr);
    auto packStrLen = strlen(redable);
    std::strncpy(packageNameId.data(), redable, packageNameId.size());

    g_gameEnv->ReleaseStringUTFChars(GTASAid, redable);
    g_gameEnv->DeleteLocalRef(GTASAid);
    g_gameEnv->DeleteLocalRef(GTASAobj);
    g_gameEnv->DeleteLocalRef(activityThreadObj);
    g_gameEnv->DeleteLocalRef(threadClass);
    g_gameEnv->DeleteLocalRef(gtasaClass);

    return packStrLen;
}

ImVec2 getScreenSize()
{
    useriDsp("Java: Geeting screen size from JVM");

    jclass activityClass = g_gameEnv->GetObjectClass(g_gtaSA)};
    jclass displayClass = g_gameEnv->FindClass("core/view/Display")};
    jmethodID getDefaultDisplayMethod = g_gameEnv->GetMethodID(activityClass, "getWindowManager", "()Landroid/view/WindowManager;")};
    jmethodID getDefaultDisplayMethodID = g_gameEnv->GetMethodID(displayClass, "getDefaultDisplay", "()Landroid/view/Display;")};

    jobject windowManager = g_gameEnv->CallObjectMethod(g_gtaSA, getDefaultDisplayMethod);
    jobject display = g_gameEnv->CallObjectMethod(windowManager, getDefaultDisplayMethodID);

    jmethodID getMetricsMethod = g_gameEnv->GetMethodID(displayClass, "getMetrics", "(Landroid/util/DisplayMetrics;)V")};

    jclass metricsClass = g_gameEnv->FindClass("core/util/DisplayMetrics");
    jmethodID constructor = g_gameEnv->GetMethodID(metricsClass, "<init>", "()V");
    jobject metrics = g_gameEnv->NewObject(metricsClass, constructor);

    g_gameEnv->CallVoidMethod(display, getMetricsMethod, metrics);

    jfieldID widthPixelsField = g_gameEnv->GetFieldID(metricsClass, "widthPixels", "I");
    jfieldID heightPixelsFiel = {g_gameEnv->GetFieldID(metricsClass, "heightPixels", "I");

    jint width = g_gameEnv->GetIntField(metrics, widthPixelsField);
    jint height = g_gameEnv->GetIntField(metrics, heightPixelsField);

    // Clean up the local references
    g_gameEnv->DeleteLocalRef(activityClass);
    g_gameEnv->DeleteLocalRef(displayClass);
    g_gameEnv->DeleteLocalRef(windowManager);
    g_gameEnv->DeleteLocalRef(display);
    g_gameEnv->DeleteLocalRef(metricsClass);
    g_gameEnv->DeleteLocalRef(metrics);

    ImVec2 screenDimensions;
    screenDimensions.x = static_cast<float>(width);
    screenDimensions.y = static_cast<float>(height);

    return screenDimensions;
}
