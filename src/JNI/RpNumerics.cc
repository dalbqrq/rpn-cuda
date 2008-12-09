/**
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) RPNUMERICS.cc
 **/


//! Definition of RPNUMERICS.
/*!
 *
 * TODO:
 * NOTE :
 *
 * @ingroup rpnumerics
 */

#include "rpnumerics_RpNumerics.h"
#include "rpnumerics_RPNUMERICS.h"

#include "RpNumerics.h"
#include "Quad2.h"
#include "Quad2FluxParams.h"

#include "TriPhase.h"
#include "TriPhaseParams.h"
#include  "CapilParams.h"
#include "PermParams.h"
#include "ViscosityParams.h"

#include "ContinuationRarefactionMethod.h"
//#include "WaveFlowFactory.h"

#include "LSODEStopGenerator.h"
#include "LSODEProfile.h"
#include "LSODE.h"

#include "JNIDefs.h"
#include <string.h>
#include <iostream>

using namespace std;

Physics * RpNumerics::physics_=NULL;

const ODESolver * RpNumerics::odeSolver_=NULL;

double RpNumerics::sigma=0;


void RpNumerics::initODESolver(){
    
    int dimension=2;
    
    int itol=2;
    
    double rtol=1e-4;
    
    int mf =22;
    
    double deltaxi = 0.001;
    
    int nparam = 1 + dimension;
    
    double param [nparam];
    
    param[0] = 1;
    
    int ii;
    
    for (ii = 0; ii < dimension; ii++) param[1 + ii] = 0.1;
    
    int maxStepsNumber=100;
    
    
    LSODEProfile lsodeProfile(RpNumerics::getFlux(), RpNumerics::getPhysics().boundary(), maxStepsNumber, dimension, itol, rtol, mf, deltaxi, nparam, param);
    
    
    odeSolver_= new LSODE(lsodeProfile);
    
    
}

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setFamilyIndex
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setFamilyIndex
(JNIEnv *env , jobject obj, jint familyIndex){
    
    //WaveFlowFactory::setFamilyIndex(familyIndex);
    
    
}

/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    setTimeDirection
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_setTimeDirection
(JNIEnv * env , jobject obj , jint timeDirection){
    //WaveFlowFactory::setTimeDirection(timeDirection);
    
}





/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    clean
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_clean  (JNIEnv * env, jclass cls){RpNumerics::clean();}

void RpNumerics::clean(){
    
    delete physics_;
    delete odeSolver_;
}

/* Class:     rpnumerics_RPNUMERICS
 * Method:    physicsID
 * Signature: ()Ljava/lang/String;
 */


JNIEXPORT jstring JNICALL Java_rpnumerics_RPNUMERICS_physicsID(JNIEnv * env, jclass cls){
    return env->NewStringUTF(RpNumerics::getPhysics().ID());
}


/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    domain
 * Signature: ()Lwave/multid/Space;
 */
JNIEXPORT jobject JNICALL Java_rpnumerics_RPNUMERICS_domain  (JNIEnv * env, jclass cls){
    
    jclass spaceClass = env->FindClass("wave/multid/Space");
    
    jmethodID spaceConstructor = (env)->GetMethodID(spaceClass, "<init>", "(Ljava/lang/String;I)V");
    
    jstring spaceName = env->NewStringUTF(RpNumerics::getPhysics().domain().name());
    
    jobject space = env->NewObject(spaceClass, spaceConstructor, spaceName, RpNumerics::getPhysics().domain().dim());
    
    return space;
    
    
}

/* Class:     rpnumerics_RPNUMERICS
 * Method:    initNative
 * Signature: (Ljava/lang/String;)V
 */

JNIEXPORT void JNICALL Java_rpnumerics_RPNUMERICS_initNative(JNIEnv * env, jclass cls, jstring physicsName) {
    
    const char *physicsID;
    
    physicsID = env->GetStringUTFChars(physicsName, NULL);
    
    
    if (physicsID == NULL) {
        return; /* OutOfMemoryError already thrown */
    }
    
    if (!strcmp(physicsID, "QuadraticR2")){
        cout <<"Criando quad2"<<endl;
        
        RpNumerics::setPhysics(Quad2(Quad2FluxParams()));
        
    }
    
    if(!strcmp(physicsID, "TriPhase")){
        
        RpNumerics::setPhysics(TriPhase( TriPhaseParams(), PermParams(), CapilParams(0.4, 3.0, 44.0, 8.0), ViscosityParams(0.5)));
    }
    
    
    
}



JNIEXPORT void JNICALL Java_rpnumerics_RpNumerics_init(JNIEnv * env, jclass cls, jobject numericsProfile){
    
    jclass numericsProfileClass = env->FindClass(NUMERICSPROFILE_LOCATION);
    jmethodID getPhysIDMethod = env->GetMethodID(numericsProfileClass, "getPhysicsID", "()Ljava/lang/String;");
    jstring ID=  (jstring)env->CallObjectMethod(numericsProfile, getPhysIDMethod);
    
    const char *physicsID;
    
    physicsID = env->GetStringUTFChars(ID, NULL);
    
    if (physicsID == NULL) {
        return; /* OutOfMemoryError already thrown */
    }
    
    //Physics instantiation
    
    if (!strcmp(physicsID, "QuadraticR2")){
        
        RpNumerics::setPhysics(Quad2(Quad2FluxParams()));
        
    }
    
    if(!strcmp(physicsID, "TriPhase")){
        
        RpNumerics::setPhysics(TriPhase( TriPhaseParams(), PermParams(), CapilParams(0.4, 3.0, 44.0, 8.0), ViscosityParams(0.5)));
    }
    
    //ODE solver instantiation
    
    RpNumerics::initODESolver();
    
    cout <<"Physics: "<< physicsID <<endl;
    
    env->ReleaseStringUTFChars(ID, physicsID);
    
}

JNIEXPORT jobject JNICALL Java_rpnumerics_RpNumerics_getXZero(JNIEnv * env, jclass cls ){
    
    double teste[2];
    
    teste[0]=0.1;
    teste[1]=0.1;
    
    int coordsSize=2;
    
    jclass realVectorClass_ = env->FindClass("wave/util/RealVector");
    
    jclass phasePointClass_ = env->FindClass("rpnumerics/PhasePoint");
    
    jmethodID phasePointConstructor_ = (env)->GetMethodID(phasePointClass_, "<init>", "(Lwave/util/RealVector;)V");
    
    jmethodID  realVectorConstructorDoubleArray_= env->GetMethodID(realVectorClass_, "<init>", "([D)V");
    
    jdoubleArray tempArray = env->NewDoubleArray(coordsSize);
    
    env->SetDoubleArrayRegion(tempArray, 0, coordsSize, teste);
    
    jobject realVector=  env->NewObject(realVectorClass_, realVectorConstructorDoubleArray_, tempArray);
    
    jobject phasePoint = env->NewObject(phasePointClass_, phasePointConstructor_, realVector);
    
    env->DeleteLocalRef(tempArray);
    
    return phasePoint;
}




/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    createNativeRarefactionFlow
 * Signature: (Ljava/lang/String;)V
 */


/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    domainDim
 * Signature: ()I
 */

JNIEXPORT jint JNICALL Java_rpnumerics_RPNUMERICS_domainDim(JNIEnv * env, jclass cls) {
    return  RpNumerics::getPhysics().domain().dim();
}


/*
 * Class:     rpnumerics_RPNUMERICS
 * Method:    boundary
 * Signature: ()Lwave/util/Boundary;
 */



JNIEXPORT jobject JNICALL Java_rpnumerics_RPNUMERICS_boundary(JNIEnv * env, jclass cls){
    
    jclass realVectorClass = env->FindClass("wave/util/RealVector");
    
    jmethodID realVectorConstructor = (env)->GetMethodID(realVectorClass, "<init>", "([D)V");
    
    const Boundary & boundary= RpNumerics::getPhysics().boundary();
    
    if (!strcmp(RpNumerics::getPhysics().ID(), "QuadraticR2")){
        
        jclass boundaryClass = env->FindClass("wave/util/RectBoundary");
        
        jmethodID boundaryConstructor = (env)->GetMethodID(boundaryClass, "<init>", "(Lwave/util/RealVector;Lwave/util/RealVector;)V");
        
        double minimum [2];
        double maximum [2];
        
        minimum[0]=boundary.minimums().component(0);
        
        minimum[1]=boundary.minimums().component(1);
        
        maximum[0]=boundary.maximums().component(0);
        maximum[1]=boundary.maximums().component(1);
        
        
        //---------------------------------
        
        jdoubleArray min = (env)->NewDoubleArray(2);
        jdoubleArray max = (env)->NewDoubleArray(2);
        
        (env)->SetDoubleArrayRegion(min, 0, 2, minimum);
        (env)->SetDoubleArrayRegion(max, 0, 2, maximum);
        
        jobject minRealVector = (env)->NewObject(realVectorClass, realVectorConstructor, min);
        jobject maxRealVector = (env)->NewObject(realVectorClass, realVectorConstructor, max);
        
        jobject boundary = (env)->NewObject(boundaryClass, boundaryConstructor, minRealVector, maxRealVector);
        
        (env)->DeleteLocalRef(min);
        (env)->DeleteLocalRef(max);
        (env)->DeleteLocalRef(minRealVector);
        (env)->DeleteLocalRef(maxRealVector);
        
        
        return boundary;
        
    }
    
    if (!strcmp(RpNumerics::getPhysics().ID(), "TriPhase")){
        
        const IsoTriang2DBoundary & boundary = (const IsoTriang2DBoundary &) RpNumerics::getPhysics().boundary();
        
        jclass isoRect2DBboundaryClass = env->FindClass("wave/util/IsoTriang2DBoundary");
        
        jmethodID isoTriang2DBoundaryConstructor = (env)->GetMethodID(isoRect2DBboundaryClass, "<init>", "(Lwave/util/RealVector;Lwave/util/RealVector;Lwave/util/RealVector;)V");
        
        jdoubleArray A = (env)->NewDoubleArray(2);
        jdoubleArray B = (env)->NewDoubleArray(2);
        jdoubleArray C = (env)->NewDoubleArray(2);
        
        // Getting A,B and C
        double Anative [2];
        
        Anative[0]=boundary.getA().component(0);
        Anative[1]=boundary.getA().component(1);
        
        double  Bnative [2];
        
        Bnative[0]=boundary.getB().component(0);
        Bnative[1]=boundary.getB().component(1);
        
        double  Cnative [2];
        
        Cnative[0]=boundary.getC().component(0);
        Cnative[1]=boundary.getC().component(1);
        
        //---------------------------
        
        (env)->SetDoubleArrayRegion(A, 0, 2, Anative);
        (env)->SetDoubleArrayRegion(B, 0, 2, Bnative);
        (env)->SetDoubleArrayRegion(C, 0, 2, Cnative);
        
        jobject ArealVector = (env)->NewObject(realVectorClass, realVectorConstructor, A);
        jobject BrealVector = (env)->NewObject(realVectorClass, realVectorConstructor, B);
        jobject CrealVector = (env)->NewObject(realVectorClass, realVectorConstructor, C);
        
        jobject isoTriang2DBoundary = (env)->NewObject(isoRect2DBboundaryClass, isoTriang2DBoundaryConstructor, ArealVector, BrealVector, CrealVector);
        
        (env)->DeleteLocalRef(A);
        (env)->DeleteLocalRef(B);
        (env)->DeleteLocalRef(C);
        
        (env)->DeleteLocalRef(ArealVector);
        (env)->DeleteLocalRef(BrealVector);
        (env)->DeleteLocalRef(CrealVector);
        
        return isoTriang2DBoundary;
        
    }
    
}






