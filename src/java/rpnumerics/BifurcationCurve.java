/*
 *
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpnumerics;

import java.util.ArrayList;
import java.util.List;
import rpnumerics.methods.contour.ContourCurve;

import wave.multid.view.ViewingAttr;
import wave.util.RealSegment;

public class BifurcationCurve extends SegmentedCurve {
    //
    // Members
    //

    private int familyIndex_;
    private List leftSegments_;// TODO It is necessary ?
    private List rightSegments_;//TODO It is necessary ?
//    private List segments;

    //
    // Constructor
    public BifurcationCurve(List<HugoniotSegment> leftList, List<HugoniotSegment> rightList) {

        super(createSingleSegmentList(leftList, rightList));

        leftSegments_ = leftList;
        rightSegments_ = rightList;
    }


    public BifurcationCurve(List<HugoniotSegment> singleList) {

        super(singleList);

        leftSegments_ = singleList;
        rightSegments_ = singleList;
    }

    public BifurcationCurve(int familyIndex, ContourCurve curve, ViewingAttr viewingAttr) {

        super(new ArrayList());

        familyIndex_ = familyIndex;
        // segments = MultidAdapter.converseCoordsArrayToRealSegments(MultidAdapter.converseRPnCurveToCoordsArray(this));
//        segments = MultidAdapter.converseRPnCurveToRealSegments(this);
    }

    //
    // Accessors/Mutators
    //
    public int getFamilyIndex() {
        return familyIndex_;
    }

    public String toXML() {


        StringBuffer buffer = new StringBuffer();

        for (int i = 0; i < segments().size(); i++) {
            HugoniotSegment hugoniotSegment = (HugoniotSegment) segments().get(i);
            RealSegment realSegment = new RealSegment(hugoniotSegment.p1(), hugoniotSegment.p2());
            buffer.append(realSegment.toXML());
        }
        return buffer.toString();
    }

    public List<HugoniotSegment> leftSegments() {
        return leftSegments_;
    }

    public List<HugoniotSegment> rightSegments() {
        return rightSegments_;
    }

    private static List createSingleSegmentList(List<HugoniotSegment> leftSeg, List<HugoniotSegment> rightSeg) {
//        int i = 0;

         System.out.println("Listas do d contact: " + leftSeg.size() + " " + rightSeg.size());

        for (HugoniotSegment hugoniotSegment : leftSeg) {
            hugoniotSegment.setIntType(19);
//            i++;
        }
        for (HugoniotSegment hugoniotSegment : rightSeg) {
            hugoniotSegment.setIntType(18);
        }

//
////        List<HugoniotSegment> mergedList = new ArrayList<HugoniotSegment>();
//
//        for (int i = 0; i < rightSeg.size(); i++) {
//            HugoniotSegment hSegmentRight = rightSeg.get(i);
//            HugoniotSegment hSegmentLeft = leftSeg.get(i);
//
//            RealVector leftPoint = new RealVector(hSegmentLeft.p1().getSize() * 2);
//
//        for (HugoniotSegment hugoniotSegment : leftSeg) {
//            hugoniotSegment.setIntType(16);
//            i++;
//
//            for (int j = 0; j < hSegmentLeft.p1().getSize(); j++) {
//                leftPoint.setElement(j, hSegmentLeft.p1().getElement(j));
//
//            }
//
//            for (int j = 0; j < hSegmentRight.p1().getSize(); j++) {
//                leftPoint.setElement(j + hSegmentRight.p1().getSize(), hSegmentRight.p1().getElement(j));
//            }
//
//
//            RealVector rightPoint = new RealVector(hSegmentRight.p2().getSize() * 2);
//
//            for (int j = 0; j < hSegmentRight.p2().getSize(); j++) {
//                rightPoint.setElement(j, hSegmentLeft.p2().getElement(j));
//
//            }
//
//            for (int j = 0; j < hSegmentRight.p2().getSize(); j++) {
//                rightPoint.setElement(j + hSegmentRight.p2().getSize(), hSegmentRight.p2().getElement(j));
//            }
//            System.out.println(leftPoint + "  " + rightPoint);
////            RealVector rightPoint = new RealVector(hSegmentLeft.p2().toString() + hSegmentRight.p2().toString());
////            System.out.println(hSegmentLeft.p2().toString() + hSegmentRight.p2().toString());
////            System.out.println(rightPoint);
//
//        }
//
////            RealVector leftPoint = new RealVector(hSegmentLeft.p1().toString() + hSegmentRight.p1().toString());

        if (leftSeg.addAll(rightSeg)) {

            return leftSeg;
        } else {
            return null;
        }

    }
}
