/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.component;

import rpnumerics.*;

public class BuckleyLeverettinCurveGeomFactory extends BifurcationCurveGeomFactory {

    public BuckleyLeverettinCurveGeomFactory(BuckleyLeverettinInflectionCurveCalc calc) {
        super(calc);

    }

    //
    // Methods
    //
    protected RpGeometry createGeomFromSource() {

        BuckleyLeverettInflectionCurve curve = (BuckleyLeverettInflectionCurve) geomSource();

        // assuming a container with HugoniotSegment elements
        int resultSize = curve.segments().size();

        BifurcationSegGeom[] hugoniotArray = new BifurcationSegGeom[resultSize];
        for (int i = 0; i < resultSize; i++) {
            hugoniotArray[i] = new BifurcationSegGeom((HugoniotSegment) curve.segments().get(i));
        }
        return new BuckleyLeverettinInflectionGeom(hugoniotArray, this);

    }

    public String toMatlab(int curveIndex) {

        StringBuffer buffer = new StringBuffer();
        BuckleyLeverettInflectionCurve curve = (BuckleyLeverettInflectionCurve) geomSource();
        buffer.append("%%\nclose all;clear all;\n");
        buffer.append(RpCalcBasedGeomFactory.createMatlabColorTable());
        buffer.append((curve.toMatlabData(0)));

        buffer.append(curve.createMatlabPlotLoop(0, 1, 0));

//        buffer.append("%%\n% begin plot x y\n");
//        buffer.append("figure; set(gca, 'Color',[0 0 0]); hold on\n");
//        buffer.append(curve.createMatlabPlotLoop(0, 2, 1));



        return buffer.toString();

    }

    public String toXML() {


        StringBuffer buffer = new StringBuffer();

        BifurcationCurve curve = (BifurcationCurve) geomSource();

        buffer.append("<COMMAND name=\"buckleylevertti\"/>\n");

        buffer.append(curve.toXML());

        buffer.append("</COMMAND>\n");

        return buffer.toString();


    }
}
