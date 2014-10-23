/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.command;

import java.awt.event.ActionEvent;
import javax.swing.JButton;
import rpn.RPnPhaseSpaceAbstraction;
import rpn.component.*;
import rpn.controller.ui.BIFURCATION_CONFIG;
import rpn.controller.ui.UIController;
import rpn.controller.ui.UI_ACTION_SELECTED;
import rpn.parser.RPnDataModule;
import rpnumerics.*;
import wave.util.RealVector;

public class SubInflectionExtensionCurveCommand extends RpModelPlotCommand {
    //
    // Constants
    //

    static public final String DESC_TEXT = "SubInflection Extension Curve";
    //
    // Members
    //
    static private SubInflectionExtensionCurveCommand instance_ = null;

    //
    // Constructors/Initializers
    //
    protected SubInflectionExtensionCurveCommand() {
        super(DESC_TEXT, rpn.RPnConfig.HUGONIOT, new JButton(DESC_TEXT));
    }

    @Override
    public void actionPerformed(ActionEvent event) {

        UI_ACTION_SELECTED action = new UI_ACTION_SELECTED(this);
//        UIController.instance().setState(new BIFURCATION_CONFIG());

        action.userInputComplete(UIController.instance());// No input needed

    }

    public RpGeometry createRpGeometry(RealVector[] input) {

        SubInflectionExtensionCurveGeomFactory factory = new SubInflectionExtensionCurveGeomFactory(RPNUMERICS.createSubInflectionExtensionCurveCalc());

        return factory.geom();

    }

    @Override
    public void execute() {

        SubInflectionExtensionCurveGeomFactory factory = new SubInflectionExtensionCurveGeomFactory(RPNUMERICS.createSubInflectionExtensionCurveCalc());
        RPnPhaseSpaceAbstraction auxPhaseSpace = RPnDataModule.PHASESPACE;
        RpGeometry geometry = factory.geom();
        auxPhaseSpace.plot(geometry);

    }

    static public SubInflectionExtensionCurveCommand instance() {
        if (instance_ == null) {
            instance_ = new SubInflectionExtensionCurveCommand();
        }
        return instance_;
    }
}
