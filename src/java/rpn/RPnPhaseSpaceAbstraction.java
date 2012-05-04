/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn;

import rpn.controller.phasespace.*;
import wave.multid.model.AbstractScene;
import wave.multid.Space;
import rpnumerics.*;
import rpn.component.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Iterator;
import rpn.component.util.ClassifierAgent;
import rpn.component.util.GeometryGraphND;
import rpn.component.util.VelocityAgent;
import rpn.controller.ui.UIController;
import rpn.parser.RPnDataModule;
import wave.multid.model.MultiGeometry;
import wave.multid.model.MultiPolyLine;
import wave.util.RealVector;

public class RPnPhaseSpaceAbstraction extends AbstractScene {
    //
    // Constants
    //
    //
    // Members
    //

    private PhaseSpaceState state_;
    private RpGeometry selectedGeom_;
    private List<RPnCurvesList> curvesListFrames_;


    static public String namePhaseSpace = "";
    static public List listResolution = new ArrayList();
    static public int closestCurve;             //indice da curva mais proxima

    
    //
    // Constructors
    //
    public RPnPhaseSpaceAbstraction(String id, Space domain, PhaseSpaceState state) {
        super(id, domain);
        changeState(state);
        selectedGeom_ = null;
        curvesListFrames_ = new ArrayList<RPnCurvesList>();
    }

    //
    // Accessors/Mutators
    //
    public void changeState(PhaseSpaceState state) {
        state_ = state;
    }

    public void attach(RPnCurvesList curvesFrame) {
        curvesListFrames_.add(curvesFrame);
    }

    public void detach(RPnCurvesList curvesListFrame) {
        curvesListFrames_.remove(curvesListFrame);
    }

    public void showCurvesFrame(boolean show) {
        Iterator<RPnCurvesList> iterator = curvesListFrames_.iterator();

        while (iterator.hasNext()) {
            RPnCurvesList rPnCurvesListFrame = iterator.next();

            rPnCurvesListFrame.setVisible(show);

        }
    }

    public Iterator curvesListIterator() {
        return curvesListFrames_.iterator();
    }

    /**
     *@todo fix this... shouldn't have access to state_
     *
     */
    public PhaseSpaceState state() {
        return state_;
    }

    //
    // Methods
    //

    public void plot(RpGeometry geom) {
        state_.plot(this, geom);

    }

    
    public void delete(RpGeometry geom) {
        state_.delete(this, geom);
    }

    public RpGeometry getLastGeometry() {

        return (RpGeometry) super.geomList_.get(super.geomList_.size() - 1);
    }

    @Override
    public void join(MultiGeometry geom) {

        if (geom == null) {
            return;
        }

        super.join(geom);
        notifyState();


    }

    @Override
    public void remove(MultiGeometry geom) {

        super.remove(geom);

        notifyState();

    }

    @Override
    public String toString() {
        return getName() + " " + getSpace().toString() + " " + state_.toString();
    }

    public void clearGeometrySelection() {
        for (int i = 0; i < geomList_.size(); i++) {
            highlightGeometry(i);
        }
    }

    public void highlightGeometry(int index) {
        for (int i = 0; i < geomList_.size(); i++) {

            if (i == index) {
                MultiGeometry geometry = (MultiGeometry) geomList_.get(i);
                if (geometry instanceof SegmentedCurveGeom) {

                    SegmentedCurveGeom segGeom = (SegmentedCurveGeom) geometry;

                } else {

                    if (geometry instanceof MultiPolyLine) {

                        MultiPolyLine poly = (MultiPolyLine) geometry;
                        poly.highLight();

                    }
                }
            }
        }

        UIController.instance().panelsUpdate();
    }


    //**************************************************************************
    public static RpGeometry findClosestGeometry(RealVector targetPoint) {

        RpGeometry closestGeometry_ = null;      //a curva mais proxima

        listResolution.clear();

        double distminCurve = 1000000.;
        double distancia = 0.;
        int k = 0;
        Iterator<RpGeometry> geomList = null ;

        //--------------------------
        if (namePhaseSpace.equals("Phase Space"))      geomList = RPnDataModule.PHASESPACE.getGeomObjIterator();
        if (namePhaseSpace.equals("RightPhase Space")) geomList = RPnDataModule.RIGHTPHASESPACE.getGeomObjIterator();
        if (namePhaseSpace.equals("LeftPhase Space"))  geomList = RPnDataModule.LEFTPHASESPACE.getGeomObjIterator();

            while (geomList.hasNext()) {

            RpGeometry geom = (RpGeometry) geomList.next();

                if (GeometryGraphND.onCurve == 1) {

                    if ((namePhaseSpace.equals("Phase Space")  &&  geom != RPnDataModule.PHASESPACE.getLastGeometry())
                            || (namePhaseSpace.equals("RightPhase Space")  &&  geom != RPnDataModule.RIGHTPHASESPACE.getLastGeometry())
                            || (namePhaseSpace.equals("LeftPhase Space")  &&  geom != RPnDataModule.LEFTPHASESPACE.getLastGeometry())) {

                        if (geom.viewingAttr().isVisible()) {

                            RpGeomFactory factory = geom.geomFactory();
                            RPnCurve curve = (RPnCurve) factory.geomSource();

                            curve.findClosestSegment(targetPoint);   //***

                            distancia = curve.distancia;

                            if (distminCurve >= distancia) {
                                distminCurve = distancia;
                                closestCurve = k;
                                closestGeometry_ = geom;
                            }

                        }
                    }

                }

                if (GeometryGraphND.onCurve == 0) {

                    if (geom.viewingAttr().isVisible()) {

                        RpGeomFactory factory = geom.geomFactory();
                        RPnCurve curve = (RPnCurve) factory.geomSource();

                        if (curve instanceof SegmentedCurve) {
                            RpCalcBasedGeomFactory geomFactory = (RpCalcBasedGeomFactory) factory;
                            RpCalculation calc = geomFactory.rpCalc();
                            ContourCurveCalc curveCalc = (ContourCurveCalc) calc;
                            listResolution.add(curveCalc.getParams().getResolution());

                        } else {
                            int[] resolution = {1, 1};
                            listResolution.add(resolution);
                        }

                        curve.findClosestSegment(targetPoint);   //***

                        distancia = curve.distancia;

                        if (distminCurve >= distancia) {
                            distminCurve = distancia;
                            closestCurve = k;
                            closestGeometry_ = geom;
                        }

                    }

                }

            k++;

        }
        //--------------------------

        return closestGeometry_;
    }


    //**************************************************************************

    //******************************************************* Leandro, em 28/Set
    //*** ClassifierAgent.hideClassifiers - chamado aqui dentro - deverá ser revisto para permitir a simples ocultação dos classificadores
    //*** Por enquanto, é feita a remoção, através de ClassifierAgent.clearClassifiers
    public static void ocultaStringsCla(int geometryIndex, String name) {

        int index = 0;
        if (name.equals("Phase Space")) index = 1;
        if (name.equals("RightPhase Space")) index = 2;
        if (name.equals("LeftPhase Space")) index = 3;

        for (int i = 0; i < ClassifierAgent.indCurvaCla.size(); i++) {
            if ((Integer) ClassifierAgent.indCurvaCla.get(i) == geometryIndex && index == (Integer)ClassifierAgent.strView.get(i)) {    //************************* Flexibilizar em funcao do painel
                ClassifierAgent.paraOcultarIndCla.add(i);
            }
        }

        if (ClassifierAgent.paraOcultarIndCla.size() > 0) {
            //ClassifierAgent.hideClassifiers(ClassifierAgent.paraOcultarIndCla);
            ClassifierAgent.clearClassifiers(ClassifierAgent.paraOcultarIndCla);
        }

        ClassifierAgent.paraOcultarIndCla.clear();

    }

    //*** Este método será usado para recuperar a visao das strings de classificacao
    //*** NÃO USE AINDA !!!
    public void mostraStringsCla(int geometryIndex) {

        for (int i = 0; i < ClassifierAgent.indCurvaCla.size(); i++) {
            if ((Integer) ClassifierAgent.indCurvaCla.get(i) == geometryIndex) {
                ClassifierAgent.paraOcultarIndCla.add(i);
            }
        }

        if (ClassifierAgent.paraOcultarIndCla.size() > 0) {
            ClassifierAgent.viewClassifiers(ClassifierAgent.paraOcultarIndCla);
        }

        ClassifierAgent.paraOcultarIndCla.clear();

    }

    //*** ClassifierAgent.hideVelocities - chamado aqui dentro - deverá ser revisto para permitir a simples ocultação das strings de velocidade
    //*** Por enquanto, é feita a remoção, através de ClassifierAgent.clearVelocities
    public static void ocultaStringsVel(int geometryIndex, String name) {

        int index = 0;
        if (name.equals("Phase Space")) index = 1;
        if (name.equals("RightPhase Space")) index = 2;
        if (name.equals("LeftPhase Space")) index = 3;

        for (int i = 0; i < VelocityAgent.indCurvaVel.size(); i++) {
            if ((Integer) VelocityAgent.indCurvaVel.get(i) == geometryIndex && index == (Integer)VelocityAgent.velView.get(i)) {
                VelocityAgent.paraOcultarIndVel.add(i);
            }
        }

        if (VelocityAgent.paraOcultarIndVel.size() > 0) {
            //VelocityAgent.hideVelocities(VelocityAgent.paraOcultarIndVel);
            VelocityAgent.clearVelocities(VelocityAgent.paraOcultarIndVel);
        }

        VelocityAgent.paraOcultarIndVel.clear();

    }

    //*** Este método será usado para recuperar a visao das strings de velocidade
    //*** NÃO USE AINDA !!!
    public void mostraStringsVel(int geometryIndex) {

        for (int i = 0; i < VelocityAgent.indCurvaVel.size(); i++) {
            if ((Integer) VelocityAgent.indCurvaVel.get(i) == geometryIndex) {
                VelocityAgent.paraOcultarIndVel.add(i);
            }
        }

        if (VelocityAgent.paraOcultarIndVel.size() > 0) {
            VelocityAgent.viewVelocities(VelocityAgent.paraOcultarIndVel);
        }

        VelocityAgent.paraOcultarIndVel.clear();

    }
    //**************************************************************************

//    public void hideGeometry(int index) {
//
//        for (int i = 0; i < geomList_.size(); i++) {
//
//            if (i == index) {
//                MultiGeometry geometry = (MultiGeometry) geomList_.get(i);
//
//                geometry.viewingAttr().setVisible(false);
//                //ocultaStringsCla(index);    // ******
//                ocultaStringsCla(index, this.getName());
//                ocultaStringsVel(index);    // ******
//            }
//        }
//
//        UIController.instance().panelsUpdate();
//
//    }
//
//    public void displayGeometry(int index) {
//        for (int i = 0; i < geomList_.size(); i++) {
//
//            if (i == index) {
//                MultiGeometry geometry = (MultiGeometry) geomList_.get(i);
//                geometry.viewingAttr().setVisible(true);
//                //mostraStringsCla(index);    // ******
//                //mostraStringsVel(index);    // ******
//
//            }
//
//        }
//
//        UIController.instance().panelsUpdate();
//
//
//    }

    public void lowlightGeometry(int index) {


        for (int i = 0; i < geomList_.size(); i++) {

            if (i == index) {
                MultiGeometry geometry = (MultiGeometry) geomList_.get(i);
                if (geometry instanceof SegmentedCurveGeom) {

                    SegmentedCurveGeom segGeom = (SegmentedCurveGeom) geometry;
                    segGeom.lowLight();


                } else {

                    if (geometry instanceof MultiPolyLine) {

                        MultiPolyLine poly = (MultiPolyLine) geometry;
                        poly.lowLight();

                    }
                }
            }
        }
        UIController.instance().panelsUpdate();
    }

    @Override
    public void update() {

        Iterator geomList = getGeomObjIterator();
        List removeList =
                new ArrayList();
        List joinList = new ArrayList();
        while (geomList.hasNext()) {
            RpGeometry geom = (RpGeometry) geomList.next();
            RpGeomFactory factory = geom.geomFactory();

            if (factory.isGeomOutOfDate()) {

                removeList.add(geom);
                joinList.add(factory.geom());
                factory.setGeomOutOfDate(false);
            }
        }
        for (int i = 0; i < removeList.size(); i++) {

            RpGeometry geometryToRemove = (RpGeometry)removeList.get(i);

            super.remove(geometryToRemove);
        }
        for (int i = 0; i < joinList.size(); i++) {
            super.join((RpGeometry) joinList.get(i));

        }
        notifyState();
    }

    // overwriting so we don't remove the last Hugoniot
    @Override
    public void clear() {

        ArrayList deleteList = new ArrayList();

        Iterator geomList = getGeomObjIterator();
        while (geomList.hasNext()) {
            RpGeometry geom = (RpGeometry) geomList.next();

            deleteList.add(geom);

        }
        for (int i = 0; i < deleteList.size(); i++) {
            delete((RpGeometry) deleteList.get(i));
        }

        notifyState();

    }

    public RpGeometry getSelectedGeom() {
        return selectedGeom_;
    }

    public void setSelectedGeom(RpGeometry selected) {
        selectedGeom_ = selected;
    }

    //
    // Methods
    //
    public StationaryPointGeom find(RealVector coords) {
        // only searches for StatPointGeom...
        Iterator geomList = getGeomObjIterator();
        StationaryPointGeom closest = null;
        while (geomList.hasNext()) {
            RpGeometry geom = (RpGeometry) geomList.next();
            if (geom instanceof StationaryPointGeom) {
                StationaryPointGeom evaluate = (StationaryPointGeom) geom;
                if (closest == null) {
                    closest = evaluate;
                } else {
                    double closestDeltaSum = 0d;
                    double evaluateDeltaSum = 0d;
                    for (int i = 0; i < coords.getSize(); i++) {
                        double closestDelta = Math.pow(coords.getElement(i)
                                - ((StationaryPoint) closest.geomFactory().geomSource()).getPoint().getCoords().getElement(i), 2);
                        double evaluateDelta = Math.pow(coords.getElement(i)
                                - ((StationaryPoint) evaluate.geomFactory().geomSource()).getPoint().getCoords().getElement(i), 2);
                        closestDeltaSum += closestDelta;
                        evaluateDeltaSum += evaluateDelta;
                    }
                    if (evaluateDeltaSum < closestDeltaSum) {
                        closest = evaluate;
                    }
                }
            }

        }
        return closest;
    }

    public PhasePoint findSelectionMidPoint() {

        PhasePoint selectionPoint = ((StationaryPoint) selectedGeom_.geomFactory().geomSource()).getPoint();
        PhasePoint xzeroPoint = ((StationaryPoint) ((NUMCONFIG_READY) state_).xzeroGeom().geomFactory().geomSource()).getPoint();
        PhasePoint midPoint = new PhasePoint(xzeroPoint);
        midPoint.getCoords().add(selectionPoint.getCoords(), xzeroPoint.getCoords());
        midPoint.getCoords().scale(.5);
        return midPoint;
    }

    public void select(RealVector coords) {
        state_.select(this, coords);
    }

    public void unselectAll() {
        if (selectedGeom_ != null) {
            selectedGeom_.viewingAttr().setSelected(false);
        }
    }

    private void notifyState() {
        for (RPnCurvesList curvesFrame : curvesListFrames_) {
            curvesFrame.update();
        }

    }
}
