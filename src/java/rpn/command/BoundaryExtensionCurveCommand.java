/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.command;

import java.awt.event.ActionEvent;
import javax.swing.JButton;
import rpn.component.*;
import rpn.controller.ui.UIController;
import rpn.controller.ui.UI_ACTION_SELECTED;
import rpnumerics.*;
import wave.util.RealVector;

public class BoundaryExtensionCurveCommand extends BifurcationPlotCommand {
    //
    // Constants
    //

    static public final String DESC_TEXT = "Boundary Extension Curve";
    //
    // Members
    //
    static private BoundaryExtensionCurveCommand instance_ = null;

    //
    // Constructors/Initializers
    //
    protected BoundaryExtensionCurveCommand() {
        super(DESC_TEXT, rpn.configuration.RPnConfig.HUGONIOT, new JButton(DESC_TEXT));
    }

    @Override
    public void actionPerformed(ActionEvent event) {

        UI_ACTION_SELECTED action = new UI_ACTION_SELECTED(this);
//        UIController.instance().setState(new BIFURCATION_CONFIG());

        action.userInputComplete(UIController.instance());// No input needed

    }

    public RpGeometry createRpGeometry(RealVector[] input) {

        BoundaryExtensionCurveGeomFactory factory = new BoundaryExtensionCurveGeomFactory(RPNUMERICS.createBoundaryExtensionCurveCalc());

        return factory.geom();

    }

    @Override
    public void execute() {

        BoundaryExtensionCurveGeomFactory factory = new BoundaryExtensionCurveGeomFactory(RPNUMERICS.createBoundaryExtensionCurveCalc());
        execute(factory);

    }

    static public BoundaryExtensionCurveCommand instance() {
        if (instance_ == null) {
            instance_ = new BoundaryExtensionCurveCommand();
        }
        return instance_;
    }
}
