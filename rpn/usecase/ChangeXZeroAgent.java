/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */

package rpn.usecase;

import wave.util.RealVector;
import rpn.RPnUIFrame;
import rpn.component.RpGeomFactory;
import rpn.component.XZeroGeomFactory;
import rpnumerics.PhasePoint;
import rpnumerics.RpSolution;
import rpnumerics.RPNUMERICS;
import rpnumerics.ConservationShockFlow;
import rpnumerics.StationaryPoint;
import wave.util.RealVector;
import wave.util.RealMatrix2;
import javax.swing.AbstractAction;
import java.awt.event.ActionEvent;
import java.beans.PropertyChangeEvent;
import rpn.controller.ui.*;
import rpn.controller.PhaseSpacePanel2DController;
import rpnumerics.NativeShockFlow;


public class ChangeXZeroAgent extends RpModelConfigChangeAgent {
    //
    // Constants
    //
    static public final String DESC_TEXT = "Change X-Zero";
    //
    // Members
    //
    static private ChangeXZeroAgent instance_ = null;

    //
    // Constructors
    //
    protected ChangeXZeroAgent() {
        super(DESC_TEXT);
    }

    public void execute() {
      RealVector[] userInputList = UIController.instance().userInputList();
      RealVector lastPointAdded = userInputList[userInputList.length - 1];
      PhasePoint newXZero = new PhasePoint(lastPointAdded);

      PhasePoint oldXZero =null;

      System.out.println("New XZero " + newXZero.getCoords());

      RPNUMERICS.flow().setXZero(newXZero);

      applyChange(new PropertyChangeEvent(this, DESC_TEXT, oldXZero, newXZero));
    }

    public void unexecute() {
        PhasePoint oldValue = (PhasePoint)log().getOldValue();
        PhasePoint newValue = (PhasePoint)log().getNewValue();
        // removes the new changed XZero
        applyChange(new PropertyChangeEvent(this, DESC_TEXT, newValue, oldValue));
    }

    static public ChangeXZeroAgent instance() {
        if (instance_ == null)
            instance_ = new ChangeXZeroAgent();
        return instance_;
    }
}
