/*
 * HelloWorld Service driver.
 * Copyright (C) 2009 Michael Richardson <mcr@credil.org>
 *
 * Released under the terms of the file ../NOTICE
 */
#define LOG_TAG "HelloWorld/Service"

#include <jni.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>

#include "IHelloWorld.h"

int hello_main()
{
	LOGI("Hello client is now starting");

        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::IBinder> binder;
        android::sp<android::IHelloWorld> shw;

        do {
                binder = sm->getService(android::String16(HELLOWORLD_NAME));
                if (binder != 0)
                        break;
                LOGW("HelloWorld not published, waiting...");
                usleep(500000); // 0.5 s
        } while(true);

        //LOGI("sm: %u binder: %u", sm, binder);

        //if (sDeathNotifier == NULL) {
        //    sDeathNotifier = new DeathNotifier();
        //}
        //binder->linkToDeath(sDeathNotifier);
        //sMediaPlayerService = interface_cast<IMediaPlayerService>(binder);

	LOGI("Hello client is now trying");

        shw = android::interface_cast<android::IHelloWorld>(binder);
        shw->hellothere("fun");
        int filesize = 0;

        filesize = shw->getfilesize("/data/data/com.wssyncmldm/databases/wssdmdatabase.db");

        LOGI("Hello Filesize: %d\n", filesize);
	
	LOGI("Hello client is now exiting");

	return(filesize);
}

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

//jstring to char*
char* jstringTostring(JNIEnv* env, jstring jstr)
{
       char* rtn = NULL;
       jclass clsstring = env->FindClass("java/lang/String");
       jstring strencode = env->NewStringUTF("utf-8");
       jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
       jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
       jsize alen = env->GetArrayLength(barr);
       jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
       if (alen > 0)
       {
                 rtn = (char*)malloc(alen + 1);
                 memcpy(rtn, ba, alen);
                 rtn[alen] = 0;
       }
       env->ReleaseByteArrayElements(barr, ba, 0);
       return rtn;
}
//char* to jstring
jstring stoJstring(JNIEnv* env, const char* pat)
{
       //jclass strClass = env->FindClass("Ljava/lang/String;");
       jclass strClass = env->FindClass("java/lang/String");
       jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
       jbyteArray bytes = env->NewByteArray(strlen(pat));
       env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
       jstring encoding = env->NewStringUTF("utf-8");
       return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}


/* Native interface, it will be call in java code */
JNIEXPORT jstring JNICALL  Java_org_credil_helloworldservice_HelloWorldActivity_printJNI(JNIEnv *env,
		jobject obj) {
	LOGI("Hello World From libhelloworldservice.so!");
    print();
    setuid(0);
    print();
    setuid(10004);
    print();
    int filesize = hello_main();
    LOGI("Hello World From libhelloworldservice.so  filesize: %d\n", filesize);
    char *str = (char *)malloc(1024);
    sprintf(str, "libhelloworldservice Hello World, filesize: %d\n",  filesize);
	return stoJstring(env, str);
}

#ifdef __cplusplus
}
#endif
