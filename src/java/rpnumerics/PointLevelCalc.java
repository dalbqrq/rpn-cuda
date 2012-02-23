/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpnumerics;

import wave.util.RealVector;

public class PointLevelCalc extends LevelCurveCalc {

    private RealVector startPoint_;

    public PointLevelCalc(RealVector point, int family, ContourParams params) {
        super(family, params);
        startPoint_ = point;
    }

    @Override
    public RpSolution calc() throws RpException {
        LevelCurve result = (LevelCurve) calcNative(getFamily(), startPoint_, getParams().getResolution());

        if (result == null) {
            throw new RpException("Error in native layer");
        }

        return result;

    }

    public RealVector getStartPoint() {
        return startPoint_;


    }

    private native RpSolution calcNative(int family, RealVector point, int[] resolution);
}
