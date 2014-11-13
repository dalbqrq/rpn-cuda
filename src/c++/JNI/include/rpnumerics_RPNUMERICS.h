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
 * Method:    clean
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_clean
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setPhysicsParams
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setPhysicsParams
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    getPhysicsParam
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_rpnumerics_RPNUMERICS_getPhysicsParam
  (JNIEnv *, jclass, jint);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    getPhysicsParamsNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_rpnumerics_RPNUMERICS_getPhysicsParamsNames
  (JNIEnv *, jclass);

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
 * Method:    getEigenSortFunctionNames
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_RPNUMERICS_getEigenSortFunctionNames
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setEigenSortFunction
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setEigenSortFunction
  (JNIEnv *, jclass, jstring);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    getXLabel
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_rpnumerics_RPNUMERICS_getXLabel
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    getYLabel
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_rpnumerics_RPNUMERICS_getYLabel
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    getTransisionalLinesNames
 * Signature: ()Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_RPNUMERICS_getTransisionalLinesNames
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    readNativePhysicsConfig
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_readNativePhysicsConfig
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    getAuxFunctionNames
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_RPNUMERICS_getAuxFunctionNames
  (JNIEnv *, jclass);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setAuxFuntionParam
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setAuxFuntionParam
  (JNIEnv *, jclass, jstring, jstring, jstring);

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    getHugoniotCaseNames
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_rpnumerics_RPNUMERICS_getHugoniotCaseNames
  (JNIEnv *, jclass, jstring);

#ifdef __cplusplus
}
#endif
#endif
