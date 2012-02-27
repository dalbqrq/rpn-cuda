/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.controller;

import rpn.component.RpGeomFactory;
import rpn.usecase.*;
import java.beans.PropertyChangeEvent;
import rpn.component.HugoniotCurveGeomFactory;
import rpn.component.RarefactionExtensionGeomFactory;
import rpnumerics.HugoniotCurveCalcND;
import rpnumerics.HugoniotParams;
import rpnumerics.PhasePoint;
import rpnumerics.RarefactionExtensionCalc;
import wave.util.RealVector;

public class RarefactionExtensionController extends RpCalcController {
    //
    // Members
    //

    private RarefactionExtensionGeomFactory geomFactory_;
    //
    // Constructors
    //
    //
    // Accessors/Mutators
    //
    //
    // Methods
    //

    @Override
    protected void register() {
        DragPlotAgent.instance().addPropertyChangeListener(this);
        ChangeFluxParamsAgent.instance().addPropertyChangeListener(this);


    }

    @Override
    protected void unregister() {
        DragPlotAgent.instance().removePropertyChangeListener(this);
        ChangeFluxParamsAgent.instance().removePropertyChangeListener(this);


    }

    @Override
    public void install(RpGeomFactory geom) {
        super.install(geom);
        geomFactory_ = (RarefactionExtensionGeomFactory) geom;
    }

    @Override
    public void uninstall(RpGeomFactory geom) {
        super.uninstall(geom);
        geomFactory_ = null;
    }

    @Override
    public void propertyChange(PropertyChangeEvent change) {

        if (change.getSource() instanceof DragPlotAgent) {
            ((RarefactionExtensionCalc) geomFactory_.rpCalc()).setStart(new PhasePoint((RealVector) change.getNewValue()));
            geomFactory_.updateGeom();
            return;
        }
        super.propertyChange(change);
    }
}
