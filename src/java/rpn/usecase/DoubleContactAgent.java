/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.usecase;

import java.awt.event.ActionEvent;
import javax.swing.JButton;
import rpn.RPnPhaseSpaceAbstraction;
import rpn.component.*;
import rpn.controller.ui.UIController;
import rpn.controller.ui.UI_ACTION_SELECTED;
import rpn.parser.RPnDataModule;
import rpnumerics.*;
import wave.util.RealVector;

public class DoubleContactAgent extends RpModelPlotAgent {
    //
    // Constants
    //

    static public final String DESC_TEXT = "DoubleContact Curve";
    //
    // Members
    //
    static private DoubleContactAgent instance_ = null;

    //
    // Constructors/Initializers
    //
    protected DoubleContactAgent() {
        super(DESC_TEXT, rpn.RPnConfig.HUGONIOT, new JButton());
    }

    @Override
    public void actionPerformed(ActionEvent event) {

        UI_ACTION_SELECTED action = new UI_ACTION_SELECTED(this);
        action.userInputComplete(UIController.instance());// No input needed

    }

    public RpGeometry createRpGeometry(RealVector[] input) {

        DoubleContactGeomFactory factory = new DoubleContactGeomFactory(RPNUMERICS.createDoubleContactCurveCalc());
        return factory.geom();

    }

    @Override
    public void execute() {


        DoubleContactGeomFactory factory = new DoubleContactGeomFactory(RPNUMERICS.createDoubleContactCurveCalc());
        if (UIController.instance().isAuxPanelsEnabled()) {
            RPnPhaseSpaceAbstraction leftPhaseSpace = RPnDataModule.LEFTPHASESPACE;

            RPnPhaseSpaceAbstraction rightPhaseSpace = RPnDataModule.RIGHTPHASESPACE;

            RpGeometry leftGeometry = factory.leftGeom();
            RpGeometry rightGeometry = factory.rightGeom();

            leftPhaseSpace.plot(leftGeometry);
            rightPhaseSpace.plot(rightGeometry);
        } else {
            RPnDataModule.PHASESPACE.plot(factory.geom());
        }







    }

    static public DoubleContactAgent instance() {
        if (instance_ == null) {
            instance_ = new DoubleContactAgent();
        }
        return instance_;
    }
}
