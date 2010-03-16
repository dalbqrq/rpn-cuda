/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.component;

import rpnumerics.Orbit;
import rpnumerics.OrbitPoint;
import wave.multid.CoordsArray;

import wave.util.RealVector;

import rpnumerics.RPnCurve;
import wave.util.PointNDimension;
import java.util.ArrayList;
import wave.multid.Space;
import wave.util.RealSegment;

public class MultidAdapter {

    static public int RENDERING_PRECISION = 2;

    static public CoordsArray[] converseOrbitToCoordsArray(Orbit orbit) {
        int renderingFactor = RENDERING_PRECISION;
        OrbitPoint[] intPoints = orbit.getPoints();
        if (intPoints.length <= RENDERING_PRECISION) {
            renderingFactor = 1;
        }
        int newSize = new Double(Math.floor(new Double(intPoints.length / renderingFactor).doubleValue())).intValue();
        CoordsArray[] wc_pCoords = new CoordsArray[newSize + 1];
        // TODO this is the plotter definition (Const)
        int i = 0;
        for (; i < newSize; i++) {
            wc_pCoords[i] = new CoordsArray(intPoints[i * renderingFactor].getCoords());
        // appends the last point
        }
        wc_pCoords[i] = new CoordsArray(intPoints[intPoints.length - 1].getCoords());
        return wc_pCoords;
    }

    static public CoordsArray[] converseRealVectorsToCoordsArray(RealVector[] input) {
        CoordsArray[] output = new CoordsArray[input.length];
        for (int i = 0; i < input.length; i++) {
            output[i] = new CoordsArray(input[i]);
        }
        return output;
    }

    public static CoordsArray[] converseOrbitPointsToCoordsArray(OrbitPoint[] coords) {

        CoordsArray[] vertices = new CoordsArray[coords.length];

        for (int i = 0; i < coords.length; i++) {
            vertices[i] = new CoordsArray(coords[i].getCoords());
        }
        return vertices;
    }

    static public CoordsArray[] converseRPnCurveToCoordsArray(RPnCurve rpncurve) {

        PointNDimension[][] polyline = rpncurve.getPolylines();

        ArrayList tempCoords = new ArrayList();

        for (int polyLineIndex = 0; polyLineIndex < polyline.length; polyLineIndex++) {

            int size = polyline[polyLineIndex].length;

            for (int polyPoint = 0; polyPoint < size; polyPoint++) {
                tempCoords.add(polyline[polyLineIndex][polyPoint].toCoordsArray());
            }
        }
        CoordsArray[] coords = new CoordsArray[tempCoords.size()];
        for (int i = 0; i < tempCoords.size(); i++) {
            coords[i] = (CoordsArray) tempCoords.get(i);
        }
        tempCoords.clear();

        return coords;

    }

    static public ArrayList converseCoordsArrayToRealSegments(CoordsArray[] coords) {

        ArrayList realSegments = new ArrayList();

        RealVector p1 = null;
        RealVector p2 = null;

        for (int i = 0; i < coords.length - 2; i++) {

            p1 = new RealVector(coords[i].getCoords());
            p2 = new RealVector(coords[i + 1].getCoords());

            realSegments.add(new RealSegment(p1, p2));
        }

        p1 = new RealVector(coords[coords.length - 2].getCoords());
        p2 = new RealVector(coords[coords.length - 1].getCoords());
        realSegments.add(new RealSegment(p1, p2));

        return realSegments;

    }

    static public CoordsArray createCoords() {
        return new CoordsArray(new Space("Espaco de teste em MultidAdapter",rpnumerics.RPNUMERICS.domainDim()*2));
//        return new CoordsArray(rpnumerics.RPNUMERICS.domain()*2);
    }
}
