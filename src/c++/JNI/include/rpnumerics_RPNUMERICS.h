/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class rpnumerics_RPNUMERICS */

#ifndef _Included_rpnumerics_RPNUMERICS
#define _Included_rpnumerics_RPNUMERICS
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    initNative
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_initNative
  (JNIEnv *, jclass, jstring);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setFamilyIndex
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setFamilyIndex
  (JNIEnv *, jobject, jint);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setTimeDirection
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setTimeDirection
  (JNIEnv *, jobject, jint);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    clean
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_clean
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setParams
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setParams
  (JNIEnv *, jclass, jobjectArray);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    getFluxParams
 * Signature: ()Lrpnumerics/FluxParams;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_RPNUMERICS_getFluxParams
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setResolution
 * Signature: (Lwave/util/RealVector;Lwave/util/RealVector;Ljava/lang/String;[I)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setResolution
  (JNIEnv *, jclass, jobject, jobject, jstring, jintArray);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    getAccumulationParams
 * Signature: ()Lwave/util/RealVector;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_RPNUMERICS_getAccumulationParams
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    physicsID
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_rpnumerics_RPNUMERICS_physicsID
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setRPnHome
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setRPnHome
  (JNIEnv *, jclass, jstring);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    boundary
 * Signature: ()Lwave/util/Boundary;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_RPNUMERICS_boundary
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setBoundary
 * Signature: (Lwave/util/Boundary;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setBoundary
  (JNIEnv *, jclass, jobject);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setMethod
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setMethod
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    domainDim
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_rpnumerics_RPNUMERICS_domainDim
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    domain
 * Signature: ()Lwave/multid/Space;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_RPNUMERICS_domain
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    clearCurvesCache
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_clearCurvesCache
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    removeCurve
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_removeCurve
  (JNIEnv *, jclass, jint);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setFluxParams
 * Signature: (Lrpnumerics/FluxParams;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setFluxParams
  (JNIEnv *, jclass, jobject);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setAccumulationParams
 * Signature: (Lwave/util/RealVector;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setAccumulationParams
  (JNIEnv *, jclass, jobject);

#ifdef __cplusplus
}
#endif
#endif
