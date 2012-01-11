/*
 *
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpnumerics;

import wave.util.RealVector;


import wave.multid.view.ViewingAttr;
import java.awt.Color;
import java.util.ArrayList;
import rpn.component.MultidAdapter;
import rpn.component.OrbitGeom;
import wave.util.RealSegment;

public class Orbit extends RPnCurve implements RpSolution {
    //
    // Members
    //

    private OrbitPoint[] points_;
    private int intFlag_;

    public double distancia = 0;      //** declarei isso (Leandro)
    
    //
    // Constructor
    //

    public Orbit(RealVector[] coords, double[] times, int flag) {
        super(MultidAdapter.converseRealVectorsToCoordsArray(coords), new ViewingAttr(Color.white));

        intFlag_ = flag;
        points_ = orbitPointsFromRealVectors(coords, times);
    }

    public Orbit(OrbitPoint[] points, int flag) {
        super(MultidAdapter.converseOrbitPointsToCoordsArray(points), new ViewingAttr(Color.white));
        intFlag_ = flag;
        points_ = points;
    }

    public Orbit(Orbit orbit) {

        super(MultidAdapter.converseOrbitPointsToCoordsArray(orbit.getPoints()), new ViewingAttr(Color.white));

        intFlag_ = orbit.getIntegrationFlag();
        points_ = orbit.getPoints();
    }

    private static OrbitPoint[] orbitPointsFromRealVectors(RealVector[] coords,
            double[] times) {
        OrbitPoint[] result = new OrbitPoint[times.length];
        for (int i = 0; i < times.length; i++) {
            result[i] = new OrbitPoint(coords[i], times[i]);
        }
        return result;
    }

    //
    // Methods
    //
    // there is a possibility that the concatenation of
    // Orbits not exist...
    static public Orbit cat(Orbit curve1, Orbit curve2) {//TODO Reimplements . Bugged !
        Orbit swap = new Orbit(curve1.getPoints(), RpSolution.DEFAULT_NULL_FLAG);
        swap.cat(curve2);
        return swap;
    }

    public void cat(Orbit curve) {//TODO Reimplements . Bugged !
        // opposite time directions assumed...
        OrbitPoint[] swap = new OrbitPoint[points_.length
                + curve.getPoints().length - 1];
        double deltat = lastPoint().getLambda() - curve.lastPoint().getLambda();
        for (int i = 0, j = curve.getPoints().length - 2; i < swap.length; i++) {
            if (i >= points_.length) {
                swap[i] = curve.getPoints()[j--];
                swap[i].setLambda(swap[i].getLambda() + deltat);
            } else {
                swap[i] = (OrbitPoint) points_[i];
            }
        }
        System.arraycopy(swap, 0, points_, 0, swap.length);
    }

    public static Orbit concat(Orbit backward, Orbit forward) {
        // opposite time directions assumed...
        OrbitPoint[] swap = new OrbitPoint[backward.getPoints().length
                + forward.getPoints().length - 1];

        double timeAdjust = -backward.getPoints()[0].getLambda();

        for (int i = 0, j = backward.getPoints().length - 1; i < swap.length; i++) {
            if (i >= backward.getPoints().length) {
                swap[i] = (OrbitPoint) forward.getPoints()[i - backward.getPoints().length + 1];
            } else {
                swap[i] = backward.getPoints()[j--];
                swap[i].setLambda(swap[i].getLambda() + timeAdjust);

            }
        }

        return new Orbit(swap, OrbitGeom.BOTH_DIR);

    }





    //** inseri este método (Leandro)
    @Override
    public int findClosestSegment(RealVector targetPoint, double alpha) {

        ArrayList segments = MultidAdapter.converseCoordsArrayToRealSegments(MultidAdapter.converseRPnCurveToCoordsArray(this));

        RealVector target = new RealVector(targetPoint);
        RealVector closest = null;
        RealVector segmentVector = null;
        alpha = 0;
        int closestSegment = 0;
        double closestDistance = -1;

        double[] dist = new double[segments.size()];
        double distmin = 0, distprox;

        for (int i = 0; i < segments.size(); i++) {

            RealSegment segment = (RealSegment) segments.get(i);
            segmentVector = new RealVector(segment.p1());
            segmentVector.sub(segment.p2());

            for (int k = 0; k < target.getSize(); k++) {
                if (target.getElement(k) == 0.) {
                    segmentVector.setElement(k, 0.);
                }
            }

            closest = new RealVector(target);

            closest.sub(segment.p2());

            alpha = closest.dot(segmentVector)
                    / segmentVector.dot(segmentVector);

            if (alpha < 0) {
                alpha = 0;
            }
            if (alpha > 1) {
                alpha = 1;
            }
            segmentVector.scale(alpha);

            closest.sub(segmentVector);

            for (int k = 0; k < target.getSize(); k++) {
                if (target.getElement(k) == 0.) {
                    closest.setElement(k, 0.);
                }
            }

            dist[i] = closest.norm();
        }

        distmin = dist[0];

        for (int i = 1; i < dist.length; i++) {
            distprox = dist[i];
            if (distprox <= distmin) {
                distmin = distprox;
                closestSegment = i;
            }
        }

        distancia = distmin;

        return closestSegment;
    }
    //**************************************************************************

    @Override
    public String toString() {
        StringBuffer buf = new StringBuffer();
        buf.append("\n points = ");
        for (int i = 0; i < points_.length; i++) {
            buf.append("[" + i + "] = " + points_[i] + "  ");
            buf.append("\n");
        }
        return buf.toString();
    }

    public String toXML() {
        StringBuffer buffer = new StringBuffer();

        String timedir = "pos";
        if (getIntegrationFlag() == OrbitGeom.BACKWARD_DIR) {
            timedir = "neg";
        }

        buffer.append("<ORBIT timedirection=\"" + timedir + "\">\n");

        for (int i = 0; i < points_.length; i++) {

            buffer.append("<ORBITPOINT time=\""
                    + ((OrbitPoint) points_[i]).getLambda() + "\">");
            buffer.append(points_[i].toXML());
            buffer.append("</ORBITPOINT>\n");
        }
        buffer.append("</ORBIT>\n");

        return buffer.toString();
    }

    public String toXML(boolean calcReady) {
        StringBuffer buffer = new StringBuffer();
        if (calcReady) {
            buffer.append("<ORBIT timedirection=\"" + intFlag_ + "\"" + ">\n");
            for (int i = 0; i < points_.length; i++) {


                buffer.append("<ORBITPOINT time=\""
                        + ((OrbitPoint) points_[i]).getLambda() + "\">");
                buffer.append(points_[i].toXML());
                buffer.append("</ORBITPOINT>\n");
            }
            buffer.append("</ORBIT>\n");
        }
        return buffer.toString();
    }

    public String toMatlabData(int curveIndex) {

        StringBuffer buffer = new StringBuffer();
        buffer.append("data" + curveIndex + "=[");

        for (int i = 0; i < points_.length; i++) {
            OrbitPoint orbitPoint = points_[i];

            buffer.append(orbitPoint.toString());
            buffer.append(";\n");

        }

        buffer.append("];");

        return buffer.toString();


    }

    //
    // Accessors/Mutators
    //
    public OrbitPoint[] getPoints() {
        return points_;
    }

    public OrbitPoint lastPoint() {
        return (OrbitPoint) points_[points_.length - 1];
    }

    public OrbitPoint firstPoint(){
        return (OrbitPoint) points_[0];
    }

    public int getIntegrationFlag() {
        return intFlag_;
    }

    public void setIntegrationFlag(int flag) {
        intFlag_ = flag;
    }

    public String createPoint3DMatlabPlot(int identifier) {

        StringBuffer buffer = new StringBuffer();

        String color = null;
        if (this instanceof RarefactionOrbit) {
            RarefactionOrbit rarefactionOrbit = (RarefactionOrbit) this;
            int family = rarefactionOrbit.getFamilyIndex();
            if (family == 1) {
                color = "[1 0 0]";
            } else {
                color = "[0 0 1]";
            }

        }

        buffer.append("plot3(data" + identifier + "(:,1),data" + identifier + "(:,2),data" + identifier + "(:,3),'Color'," + color + ")\n");

        RealVector xMin = RPNUMERICS.boundary().getMinimums();
        RealVector xMax = RPNUMERICS.boundary().getMaximums();

        buffer.append("axis([" + xMin.getElement(0) + " " + xMax.getElement(0) + " " + xMin.getElement(1) + " " + xMax.getElement(1) + " " + xMin.getElement(2) + " " + xMax.getElement(2) + "]);\n");

        buffer.append("xlabel('s')\nylabel('T')\nzlabel('u')\n");

        return buffer.toString();

    }

    public String create2DPointMatlabPlot(int x, int y, int identifier) {

        StringBuffer buffer = new StringBuffer();

        String color = null;

        if (this instanceof RarefactionOrbit) {
            RarefactionOrbit rOrbit = (RarefactionOrbit) this;

            int family = rOrbit.getFamilyIndex();
            if (family == 1) {
                color = "[1 0 0]";
            } else {
                color = "[0 0 1]";
            }
        }


        x++;
        y++;

        buffer.append("plot(data" + identifier + "(:,");
        buffer.append(x);
        buffer.append("),");
        buffer.append("data" + identifier + "(:,");
        buffer.append(y);

        buffer.append("),'Color'" + "," + color + ")\n");

        RealVector xMin = RPNUMERICS.boundary().getMinimums();
        RealVector xMax = RPNUMERICS.boundary().getMaximums();

        buffer.append("axis([" + xMin.getElement(x - 1) + " " + xMax.getElement(x - 1) + " " + xMin.getElement(y - 1) + " " + xMax.getElement(y - 1) + "]);\n");

        buffer.append(createAxisLabel2D(x - 1, y - 1));
        return buffer.toString();

    }

    private static String createAxisLabel2D(int x, int y) {

        String axisName[] = new String[3];

        axisName[0] = "s";
        axisName[1] = "T";
        axisName[2] = "u";


        StringBuffer buffer = new StringBuffer();
        buffer.append("xlabel('");
        buffer.append(axisName[x] + "')\n");
        buffer.append("ylabel('" + axisName[y] + "')\n");

        return buffer.toString();

    }
}
