/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.usecase;

import java.awt.event.ActionEvent;
import javax.swing.JToggleButton;
import rpn.component.*;
import rpn.controller.ui.BIFURCATION_CONFIG;
import rpn.controller.ui.UIController;
import rpn.controller.ui.UI_ACTION_SELECTED;
import rpnumerics.*;
import wave.util.RealVector;

public class CoincidenceExtensionCurvePlotAgent extends RpModelPlotAgent {
    //
    // Constants
    //

    static public final String DESC_TEXT = "Coincidence Extension Curve";
    //
    // Members
    //
    static private CoincidenceExtensionCurvePlotAgent instance_ = null;

    //
    // Constructors/Initializers
    //
    protected CoincidenceExtensionCurvePlotAgent() {
        super(DESC_TEXT, rpn.RPnConfig.HUGONIOT, new JToggleButton());
    }

    @Override
    public void actionPerformed(ActionEvent event) {

        UI_ACTION_SELECTED action = new UI_ACTION_SELECTED(this);
        UIController.instance().setState(new BIFURCATION_CONFIG());

        action.userInputComplete(UIController.instance());// No input needed

    }

    public RpGeometry createRpGeometry(RealVector[] input) {

        CoincidenceExtensionCurveGeomFactory factory = new CoincidenceExtensionCurveGeomFactory(new CoincidenceExtensionCurveCalc());
        return factory.geom();

    }

    static public CoincidenceExtensionCurvePlotAgent instance() {
        if (instance_ == null) {
            instance_ = new CoincidenceExtensionCurvePlotAgent();
        }
        return instance_;
    }
}
