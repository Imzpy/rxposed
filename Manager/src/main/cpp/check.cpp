#include <cinttypes>
#include <list>
#include <sys/mman.h>
#include <set>
#include <string_view>

#include <android/log.h>
#include <string>
#include <jni.h>
#include <dlfcn.h>
#include <fstream>
#include <unistd.h>
#include <vector>
#include "rxposed/artmethod_native_hook.h"
//#include "rxposed/tool.h"
#include "hideload/elf_symbol_resolver.h"

using namespace std;



extern "C"
JNIEXPORT jboolean JNICALL
Java_hepta_rxposed_manager_util_CheckTool_chekcPreGetenv(JNIEnv *env, jobject thiz) {
    void *Pre_GetEnv  =  linkerResolveElfInternalSymbol("libandroid_runtime.so", "_ZN7android14AndroidRuntime9getJNIEnvEv");

    JNIEnv *pEnv =((JNIEnv*(*)())Pre_GetEnv)();
    if(pEnv == env){
        return true;
    }
    return false;
}



int __attribute__((naked)) MyASMTest(int a, int b)
{
#ifdef __arm__

    asm(".thumb");
    asm(".syntax unified");

    asm("sub r0, r0, r1");
    asm("add r0, r0, #1");  // 为了区分当前用的是AArch32还是AArch64，这里对于AArch32情况下再加1
    asm("bx lr");

#else

    asm("sub w0, w0, w1");
    asm("ret");

#endif
}



void nakedFunction(void) __attribute__((naked)) {

}


jboolean hook_fun_addr(JNIEnv *env, jclass clazz) {

//    LOGE("hook_fun_addr");
    return true;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_hepta_rxposed_manager_util_CheckTool_jni_1hook_1test(JNIEnv *env, jobject thiz) {
//    LOGE("hook_test");
    return false;
}


extern "C"
JNIEXPORT void JNICALL
Java_hepta_rxposed_manager_util_CheckTool_jni_1hook(JNIEnv *env, jobject thiz) {


    // TODO: implement jni_hook()
    jclass  cls = env->GetObjectClass(thiz);
    jmethodID javamethod  =  env->GetMethodID(cls,"jni_hook_test", "()Z");
    HookJmethod_JniFunction(env,cls,javamethod,(uintptr_t)hook_fun_addr);

//    LOGE("Java_hepta_rxposed_manager_fragment_check_checkFragment_jni_1hook");
}

extern "C"
JNIEXPORT void JNICALL
Java_hepta_rxposed_manager_util_CheckTool_jni_1unhook(JNIEnv *env, jobject thiz) {
    // TODO: implement jni_unhook()
    jclass  cls = env->GetObjectClass(thiz);
    jmethodID javamethod  =  env->GetMethodID(cls,"jni_hook_test", "()Z");
    unHookJmethod_JniFunction(env,cls,javamethod);
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_hepta_rxposed_manager_util_CheckTool_chekc_1GetArtmethodNative_1init(JNIEnv *env,
                                                                          jobject thiz) {
    jclass  cls = env->GetObjectClass(thiz);
    jmethodID javamethod  =  env->GetMethodID(cls,"chekc_GetArtmethodNative_init", "()Z");
    uintptr_t native_fun_addr = reinterpret_cast<uintptr_t>(Java_hepta_rxposed_manager_util_CheckTool_chekc_1GetArtmethodNative_1init);
    INIT_HOOK_PlatformABI(env, cls, javamethod, (uintptr_t*)native_fun_addr, 109);
    uintptr_t jni_native_fun_addr = getJmethod_JniFunction(env,cls,javamethod);

    if(jni_native_fun_addr == native_fun_addr){
        return true;
    } else{
        return false;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_hepta_rxposed_manager_util_CheckTool_ELFresolveSymbol(JNIEnv *env, jobject thiz) {
    // TODO: implement ELFresolveSymbol()

    bool libc_resolve = false;
    bool linker_resolve = false;
    bool linker_resolve2 = false;
    uintptr_t __system_property_get_addr = reinterpret_cast<uintptr_t>(__system_property_get);
    uintptr_t __system_property_get_resolve_addr  = reinterpret_cast<uintptr_t>(linkerResolveElfInternalSymbol(
            "libc.so", "__system_property_get"));
    if(__system_property_get_addr == __system_property_get_resolve_addr){
        libc_resolve = true;
    }

    uintptr_t solist_get_somain_addr  = reinterpret_cast<uintptr_t>(linkerResolveElfInternalSymbol(
            get_android_linker_path(), "__dl__Z17solist_get_somainv"));
    if(solist_get_somain_addr != NULL) {
        linker_resolve = true;
    }

//    void * linker_handle = dlopen("linker",RTLD_GLOBAL);
//    if(linker_handle!= NULL){
//       void* __dl_rtld_db_dlactivity_addr = dlsym(linker_handle,"__dl_rtld_db_dlactivity");
//       if(__dl_rtld_db_dlactivity_addr != NULL){
//           linker_resolve2 = true;
//       }
//    }
    if(linker_resolve&&libc_resolve ){
        return true;
    }
    return false;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_hepta_rxposed_manager_util_CheckTool_check_1Process_1setArgV0(JNIEnv *env, jobject thiz) {
    // TODO: implement check_Process_setArgV0()

    jclass  Process_cls = env->FindClass("android/os/Process");
    jmethodID javamethod = env->GetStaticMethodID(Process_cls,"setArgV0Native", "(Ljava/lang/String;)V");
    if (javamethod == nullptr) {
        if(env->ExceptionCheck()) {
            env->ExceptionClear();
            javamethod = env->GetStaticMethodID(Process_cls, "setArgV0", "(Ljava/lang/String;)V");
            if(javamethod == nullptr){
                if(env->ExceptionCheck()) {
                    env->ExceptionClear();
                    return env->NewStringUTF("");
                }
            } else{
                return env->NewStringUTF("setArgV0");
            }
        }
    } else{
        return env->NewStringUTF("setArgV0Native");
    }
}