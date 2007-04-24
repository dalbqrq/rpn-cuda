/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */

package rpn.usecase;


import rpn.controller.XZeroController;
import java.beans.*;
import java.util.List;
import java.util.ArrayList;
import rpn.controller.ui.*;


public abstract class RpModelConfigChangeAgent extends RpModelActionAgent {
    //
    // Members
    //
    private List listenersList_;

    public RpModelConfigChangeAgent(String shortDesc) {
        // no ICONS
        super(shortDesc, null);
        setEnabled(false);
        listenersList_ = new ArrayList();
    }

    public void applyChange(PropertyChangeEvent change) {
        firePropertyChange(change);
        logAction(change);
        rpn.parser.RPnDataModule.PHASESPACE.update();
        UIController.instance().panelsUpdate();
    }

    public void addPropertyChangeListener(PropertyChangeListener listener) {
        /*
         * MAKES SURE XZERO IS NOTIFIED FIRST
         */

        if (listener instanceof XZeroController) {
            listenersList_.add(0, listener);
        } else {
            listenersList_.add(listener);
        }
        super.addPropertyChangeListener(listener);
    }

    public void removePropertyChangeListener(PropertyChangeListener listener) {
        listenersList_.remove(listener);
    }

    public void firePropertyChange(PropertyChangeEvent event) {
        for (int i = 0; i < listenersList_.size(); i++) {
            ((PropertyChangeListener) listenersList_.get(i)).propertyChange(
                    event);
        }
    }
}
