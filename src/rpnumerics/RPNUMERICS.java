/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpnumerics;

import java.util.logging.Level;
import java.util.logging.Logger;
import rpn.plugininterface.PluginProfile;
import rpn.plugininterface.PluginTableModel;
import rpnumerics.methods.ContourParams;
import rpnumerics.methods.HugoniotContinuationMethod;
import rpnumerics.methods.HugoniotContourMethod;
import rpnumerics.methods.contour.ContourHugoniot;
import rpnumerics.methods.contour.functionsobjects.InvalidHugoniotFunctionsRelation;
import rpnumerics.methods.contour.markedhypercubes.NullHyperCubeErrorTreatment;
import rpnumerics.methods.contour.samplefunctions.Hugoniot2DTest;
import wave.util.*;
import wave.ode.*;
import wave.multid.Space;

public class RPNUMERICS {
    //
    // Constants
    //
    static public int INCREASING_LAMBDA = 0;
    //
    // Members
    //
//    static private Physics physics_ = null;

    static private RpErrorControl errorControl_ = null;
    static private ODESolver odeSolver_ = null;
    static private ShockProfile shockProfile_ = ShockProfile.instance();
    static private RarefactionProfile rarefactionProfile_ = RarefactionProfile.instance();
    static private BifurcationProfile bifurcationProfile_ = BifurcationProfile.instance();
    static private ShockRarefactionProfile shockRarefactionProfile_ = null;
    static private int [] CONTOUR_RESOLUTION={100,100};

    //
    // Constructors/Initializers
    //
    static public void init(RPNumericsProfile profile) throws RpException {

        System.loadLibrary("wave");//TODO libwave is always loaded ?
        System.loadLibrary("rpnumerics");
        initNative(profile.getPhysicsID());
        errorControl_ = new RpErrorControl(boundary());
    }

    /**
     * 
     * @deprecated
     */
    public static native void initNative(String physicsName);

    public static HugoniotCurveCalc createHugoniotCalc() {

        HugoniotParams hparams = new HugoniotParams(shockProfile_.getXZero(), new FluxFunction());

        ShockFlow shockFlow = (ShockFlow) createShockFlow();
        //Not specific

        if (shockProfile_.getHugoniotMethodName().equals("Continuation")) {

            GenericHugoniotFunction hugoniotFunction = new GenericHugoniotFunction(hparams);

            HugoniotContinuationMethod method = new HugoniotContinuationMethod(hugoniotFunction, hparams, createODESolver(shockFlow));

            HugoniotCurveCalc hugoniotCurveCalc = new HugoniotCurveCalcND((HugoniotContinuationMethod) method);

            hugoniotCurveCalc.uMinusChangeNotify(shockProfile_.getUminus());

            return hugoniotCurveCalc;

        }

        if (shockProfile_.getHugoniotMethodName().equals("Contour")) {

            Hugoniot2DTest hugoniot2DTest = null;

            try {
                hugoniot2DTest = new Hugoniot2DTest(hparams.getFluxFunction(), hparams);
            } catch (InvalidHugoniotFunctionsRelation ex) {
                ex.printStackTrace();
                Logger.getLogger(RPNUMERICS.class.getName()).log(Level.SEVERE, null, ex);
            }

            ContourHugoniot contourHugoniot = new ContourHugoniot(hugoniot2DTest, new NullHyperCubeErrorTreatment());

            Boundary boundary = RPNUMERICS.boundary();
            
            double[] boundaryArray = null;
            
            if (boundary instanceof RectBoundary){

                boundaryArray = new double[4];

                RealVector minimums = boundary.getMinimums();
                RealVector maximums = boundary.getMaximums();

                double[] minimumsArray = minimums.toDouble();
                double[] maximumsArray = maximums.toDouble();

                boundaryArray[0] = minimumsArray[0];
                boundaryArray[1] = maximumsArray[0];
                boundaryArray[2] = minimumsArray[1];
                boundaryArray[3] = maximumsArray[1];
            }
            else {
                
                System.out.println("Implementar para dominio triangular");
                return null;
            }

            int[] resolution = getContourResolution();
            
//            System.out.println(resolution[0] + "  " + resolution[1]);

            ContourParams contourParams = new ContourParams(contourHugoniot, hugoniot2DTest, boundaryArray, resolution);

            HugoniotContourMethod contourMethod = new HugoniotContourMethod(contourParams, hparams);

            HugoniotCurveCalcND hugoniotCurveCalcND = new HugoniotCurveCalcND(contourMethod);

            hugoniotCurveCalcND.uMinusChangeNotify(shockProfile_.getUminus());

            return hugoniotCurveCalcND;


        }

        //        if (shockProfile_.isHugoniotSpecific()) { //TODO  Reactivate 
//
//            if (physicsID().equals("Quad2")) {
//
//                return new Quad2HugoniotCurveCalc(RPNUMERICS.fluxFunction().fluxParams(), hparams.getXZero());
//            }
//
//
//            if (physicsID().equals("Comb")) {
//
//                return new CombHugoniotCurveCalc((CombFluxParams) fluxFunction().fluxParams(), hparams.getXZero(), 1d);
//
//            }
//
//        }

        return null;
    }

    public static RarefactionOrbitCalc createRarefactionCalc(OrbitPoint orbitPoint, int timeDirection) {

        RarefactionFlow flow = createRarefactionFlow();
        RealVector scales = new RealVector(rarefactionProfile_.getXZero().getSize());
        for (int i = 0; i < scales.getSize(); i++) {
            scales.setElement(i, 1);
        }

        errorControl_ = new RpErrorControl(scales);
        return new RarefactionOrbitCalc(orbitPoint, timeDirection, createODESolver(flow), rarefactionProfile_.geRarefactiontMethodName());
    }

    public static StationaryPointCalc createStationaryPointCalc(PhasePoint initial) {

        ShockFlow shockFlow = createShockFlow();
        createODESolver((WaveFlow) shockFlow);

        return new StationaryPointCalc(initial, shockFlow);

    }

    public static ManifoldOrbitCalc createManifoldCalc(StationaryPoint statPoint, PhasePoint initialPoint, int timeDirection) {
        ShockFlow shockFlow = (ShockFlow) createShockFlow();
        return new ManifoldOrbitCalc(statPoint, initialPoint, shockFlow, timeDirection);

    }

    public static ConnectionOrbitCalc createConnectionOrbitCalc(ManifoldOrbit manifoldA, ManifoldOrbit manifoldB) {

        ShockFlow shockFlow = (ShockFlow) createShockFlow();

        return new ConnectionOrbitCalc(manifoldA, manifoldB, shockFlow);


    }

    public static OrbitCalc createOrbitCalc(OrbitPoint orbitPoint, int timeDirection) {

        ShockFlow flow = (ShockFlow) createShockFlow();
        return new OrbitCalc(orbitPoint, timeDirection, createODESolver(flow));

    }

    public static BifurcationCurveCalc createBifurcationCalc() {
        return new BifurcationCurveCalc();
    }

    public static ShockFlow createShockFlow() {

        FluxFunction flux = new FluxFunction();

        PluginProfile profile = PluginTableModel.getPluginConfig(ShockProfile.SHOCKFLOW_NAME);

        Double sigmaValue = new Double(profile.getParamValue("sigma"));

        ShockFlowParams shockParams = new ShockFlowParams(shockProfile_.getXZero(), sigmaValue.doubleValue());

        ShockFlow flow = new ShockFlow(shockParams, flux);
        return flow;

    }

    public static ShockFlow createShockFlow(ShockFlowParams shockFlowParams) {


        ShockFlow flow = new ShockFlow(shockFlowParams, new FluxFunction());

        return flow;

    }

    private static RarefactionFlow createRarefactionFlow() {

        return new RarefactionFlow(rarefactionProfile_.getXZero(), rarefactionProfile_.getFamily(), new FluxFunction());

    }

    private static ODESolver createODESolver(WaveFlow flow) {


//        errorControl_ = new RpErrorControl(boundary());
        odeSolver_ = new Rk4BPMethod(
                new Rk4BPProfile(new FlowVectorField(flow),
                errorControl().eps(),
                errorControl().ode().maxStateStepLength(),
                boundary(), errorControl().ode().getScale(),
                errorControl().ode().maxNumberOfSteps()));
        return odeSolver_;
    }
    //
    // Accessors
    //
    public static void setCurrentProfile(ShockRarefactionProfile aShockRarefactionProfile_) {
        shockRarefactionProfile_ = aShockRarefactionProfile_;
    }

    public static ShockRarefactionProfile getCurrentProfile() {


        return shockRarefactionProfile_;
    }

    public static ShockProfile getShockProfile() {
        return shockProfile_;
    }

    public static RarefactionProfile getRarefactionProfile() {
        return rarefactionProfile_;
    }

    public static BifurcationProfile getBifurcationProfile() {
        return bifurcationProfile_;
    }

    //TODO 
    /**
     * 
     * @deprecated CHANGE TO RPFUNCTION REFERENCES !!
     */
//    static public final FluxFunction fluxFunction() {
////        return physics_.fluxFunction();
//        return fluxFunction_;
//    }

    //TODO KEEP TO JAVA CALCS USE  !!
    /**
     * 
     * 
     * @deprecated   ONLY TO JAVA CALCS USE  
     */
    static public final RpErrorControl errorControl() {
        return errorControl_;
    }

    /**
     * 
     * 
     * @deprecated   ONLY TO JAVA CALCS USE  !!
     */
    static public final SimplexPoincareSection pSection() {
        return ((Rk4BPProfile) odeSolver_.getProfile()).getPoincareSection();
    }

    /**
     * 
     * 
     * @deprecated  ONLY TO JAVA CALCS USE  !!
     */
    static public final ODESolver odeSolver() {
        return odeSolver_;
    }

    public static int[] getContourResolution() {
        return CONTOUR_RESOLUTION;
    }

    public static void setContourResolution(int[] aCONTOUR_RESOLUTION) {
        CONTOUR_RESOLUTION = aCONTOUR_RESOLUTION;
    }

    private native void setFamilyIndex(int familyIndex);

    private native void setTimeDirection(int timeDirection);

    /**
     * Clean up the native layer
     */
    public static native void clean();

    public static native String physicsID();

    public static native Boundary boundary();

    public static native int domainDim();

    public static native Space domain();

    
}
