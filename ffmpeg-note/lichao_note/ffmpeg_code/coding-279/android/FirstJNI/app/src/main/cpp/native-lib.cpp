#include <jni.h>
#include <string>

#define JNI_CLASS_PATH "com/example/superlea/firstjni/MainActivity"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_superlea_firstjni_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_superlea_firstjni_MainActivity_stringFromJNI2(JNIEnv *env, jobject instance) {

    // TODO


    return env->NewStringUTF(",xxx");
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_superlea_firstjni_MainActivity_stringFromJNI3(
        JNIEnv *env,
        jobject instance,
        jstring str_) {

    const char *str = env->GetStringUTFChars(str_, 0);

    // TODO

    env->ReleaseStringUTFChars(str_, str);

    return env->NewStringUTF("aaa");
}

extern "C"
JNIEXPORT jstring JNICALL
my_test_register(JNIEnv *env,
                 jobject instance){
    return env->NewStringUTF("this is a test for register!");
}

static JNINativeMethod g_methods[] = {
        {"_test", "()Ljava/lang/String;", (void*)my_test_register},
};

jint JNI_OnLoad(JavaVM *vm, void *reserved){

    JNIEnv *env = NULL;
    vm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jclass clazz  = env->FindClass(JNI_CLASS_PATH);
    env->RegisterNatives(clazz, g_methods, sizeof(g_methods)/sizeof(g_methods[0]));

    return JNI_VERSION_1_6;
}
