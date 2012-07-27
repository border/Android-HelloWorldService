#include <jni.h>
#define LOG_TAG "HelloWorld"
#include <utils/Log.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void print(void) {
	LOGI(
            "         UID           GID  \n"
            "Real      %d  Real      %d  \n"
            "Effective %d  Effective %d  \n",
            getuid (),     getgid (),
            geteuid(),     getegid()
          );
}

/* Native interface, it will be call in java code */
JNIEXPORT jstring JNICALL  Java_org_credil_helloworldservice_HelloWorldActivity_printJNI(JNIEnv *env,
		jobject obj) {
	LOGI("Hello World From libhello.so!");
    print();
    setuid(0);
    print();
    setuid(10004);
    print();
	return env->NewStringUTF("Hello World!");
}

#ifdef __cplusplus
}
#endif
