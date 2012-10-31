/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.usecase;

import java.awt.event.ActionEvent;
import java.util.Iterator;
import rpn.RPnPhaseSpacePanel;
import rpn.controller.ui.*;
import rpn.message.RPnActionMediator;


public class ClearPhaseSpaceAgent extends javax.swing.AbstractAction {
    //
    // Constants
    //
    static public final String DESC_TEXT = "Clears the Phase Space";
    //
    // Members
    //
    static private ClearPhaseSpaceAgent instance_ = null;

    //
    // Constructors
    //
    protected ClearPhaseSpaceAgent() {
        super(DESC_TEXT, null);
    }

    public void clear() {
        
        // rpn.RPnUIFrame.instance().setTitle(" completing ...  " + DESC_TEXT);
        UIController.instance().setWaitCursor();
        UIController.instance().panelsBufferClear();
        rpn.parser.RPnDataModule.PHASESPACE.clear();


        // --- Está correto??? Algum modo de determinar o "panel" certo sem usar o nome???
        Iterator<RPnPhaseSpacePanel> phaseSpacePanelIterator = UIController.instance().getInstalledPanelsIterator();
        while (phaseSpacePanelIterator.hasNext()) {
            RPnPhaseSpacePanel panel = phaseSpacePanelIterator.next();
            if (panel.getName().equals("Phase Space")) {
                panel.clearAllStrings();
                panel.repaint();    
            }

        }
        // ---
        

        // ClearScene is not undoable
        UndoActionController.instance().setEnabled(false);
        System.gc();
        //rpn.RPnUIFrame.instance().setTitle("");
        UIController.instance().resetCursor();

    }
   

    public void actionPerformed(ActionEvent event) {

        if (UIController.instance().getNetStatusHandler().isOnline() && UIController.instance().getNetStatusHandler().isMaster()) {
            RPnActionMediator.instance().setState(DESC_TEXT);
        }

        clear();

    }

    static public ClearPhaseSpaceAgent instance() {
        if (instance_ == null) {
            instance_ = new ClearPhaseSpaceAgent();
        }
        return instance_;
    }
}
