/*
 *
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn.ui;

import java.awt.GridLayout;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.JSpinner.DefaultEditor;
import javax.swing.SpinnerNumberModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import rpn.configuration.Parameter;
import rpn.configuration.ParameterLeaf;
import rpn.configuration.RangedParameter;
import rpnumerics.RPNUMERICS;

public class RangedParameterView extends ParameterView {

    private JSpinner spinner_;


    public RangedParameterView(Parameter parameter) {
        super(parameter);
        
        RangedParameter rangedParameter = (RangedParameter)parameter;

        Integer family = new Integer(parameter.getValue());

        SpinnerNumberModel spinerModel = new SpinnerNumberModel(family.intValue(), Integer.parseInt(rangedParameter.getRange()[0]),
                Integer.parseInt(rangedParameter.getRange()[1]), Integer.parseInt(rangedParameter.getIncrement()));

        spinner_ = new JSpinner(spinerModel);

        ((DefaultEditor) spinner_.getEditor()).getTextField().setEditable(false);

        spinner_.addChangeListener(new FamilyChangeHandler());

        spinner_.setValue(new Integer(parameter.getValue()));


    }
    


    @Override
    public void associatedChanged(Parameter associatedParameter, Object arg) {
       
    }

    @Override
    public JComponent getComponent() {
       return spinner_;
    }

    @Override
    protected void decorate(JComponent component) {
      
        JPanel panel = (JPanel)component;
        panel.add(new JLabel(getParameter().getName()));
        
    }

    private class FamilyChangeHandler implements ChangeListener {

        @Override
        public void stateChanged(ChangeEvent e) {
            getParameter().setValue(String.valueOf(spinner_.getValue()));
           
        }
    }

}
