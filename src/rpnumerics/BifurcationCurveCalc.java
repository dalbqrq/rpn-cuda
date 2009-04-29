/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpnumerics;

import rpnumerics.methods.*;

public class BifurcationCurveCalc implements RpCalculation{
    
    private BifurcationMethod bifurcationMethod_;
    private BifurcationParams params_; 

    //
    // Constructors
    //
    
    public BifurcationCurveCalc (){
        
    }
    
    public BifurcationCurveCalc(BifurcationContourMethod bifurcationMethod) {
        bifurcationMethod_ = bifurcationMethod;
        params_ = bifurcationMethod.getParams();
    }
    
    public BifurcationCurveCalc(BifurcationContinuationMethod bifurcationMethod) {
        bifurcationMethod_ = bifurcationMethod;
        params_ = bifurcationMethod.getParams();
    }
    
    //
    // Accessors/Mutators
    //

    public int getFamilyIndex() {
        return params_.getFamilyIndex();
    }
    
    public RpSolution calc() {    	
        return bifurcationMethod_.curve();
    }

    public RpSolution recalc() {
        return calc();
    }

    public String getCalcMethodName() {
        throw new UnsupportedOperationException("Bifurcation");
    }
}
