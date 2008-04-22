/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JComboBox;
import javax.swing.JSeparator;
import javax.swing.SwingConstants;
import rpn.usecase.CurvePlotAgent;
import rpnumerics.RpNumerics;

public class RPnCurveConfigPanel extends javax.swing.JPanel {

    public RPnCurveConfigPanel() {
        initComponents();

        //Adding names

        addMethodNames();
        addFlowNames();
        addFamilyIndex();
        directionNames();

        curvePanel.add(CurvePlotAgent.instance().getContainer(), BorderLayout.SOUTH);
        
        JSeparator separator1 = new JSeparator();
        JSeparator separator2 = new JSeparator();
        
        separator1.setOrientation(SwingConstants.VERTICAL);
        separator2.setOrientation(SwingConstants.VERTICAL);
        
        panelSeparator1.add(separator1,BorderLayout.CENTER);
        panelSeparator2.add(separator2,BorderLayout.CENTER);
        
        CurvePlotAgent.instance().setEnabled(true);
        

        //Initial configuration of plot agent

        if (((String) flowComboBox.getSelectedItem()).matches(".*Shock.*")) {//TODO HardCoded !!Only to show a possible behaviour
            familyIndexComboBox.setEnabled(false);
            directionComboBox.setEnabled(false);
            CurvePlotAgent.instance().setType("shock");
        } else {
            directionComboBox.setEnabled(true);
            familyIndexComboBox.setEnabled(true);
            CurvePlotAgent.instance().setType("rarefaction");
        }


        if (((String) directionComboBox.getSelectedItem()).matches("Forward")) {
            CurvePlotAgent.instance().setTimeDirection(1);
        } else {
            CurvePlotAgent.instance().setTimeDirection(-1);
        }


        CurvePlotAgent.instance().setMethodName((String) methodComboBox.getSelectedItem());
        CurvePlotAgent.instance().setFlowName((String) flowComboBox.getSelectedItem());
        CurvePlotAgent.instance().setFamilyIndex(((Integer) familyIndexComboBox.getSelectedItem()).intValue());

    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {
        java.awt.GridBagConstraints gridBagConstraints;

        methodPanel = new javax.swing.JPanel();
        methodComboBox = new javax.swing.JComboBox();
        methodLabel = new javax.swing.JLabel();
        curvePanel = new javax.swing.JPanel();
        flowPanel = new javax.swing.JPanel();
        familyIndexComboBox = new javax.swing.JComboBox();
        familyIndexLabel = new javax.swing.JLabel();
        directionComboBox = new javax.swing.JComboBox();
        directionLabel = new javax.swing.JLabel();
        flowComboBox = new javax.swing.JComboBox();
        flowLabel = new javax.swing.JLabel();
        panelSeparator1 = new javax.swing.JPanel();
        panelSeparator2 = new javax.swing.JPanel();

        setLayout(new java.awt.GridBagLayout());

        methodPanel.setLayout(new java.awt.GridBagLayout());

        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 1;
        gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
        gridBagConstraints.insets = new java.awt.Insets(7, 0, 2, 0);
        methodPanel.add(methodComboBox, gridBagConstraints);

        methodLabel.setText("Method");
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
        gridBagConstraints.insets = new java.awt.Insets(6, 0, 0, 0);
        methodPanel.add(methodLabel, gridBagConstraints);

        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.fill = java.awt.GridBagConstraints.BOTH;
        add(methodPanel, gridBagConstraints);

        curvePanel.setLayout(new java.awt.BorderLayout());
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 4;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.gridwidth = 3;
        gridBagConstraints.fill = java.awt.GridBagConstraints.BOTH;
        gridBagConstraints.anchor = java.awt.GridBagConstraints.SOUTH;
        add(curvePanel, gridBagConstraints);

        flowPanel.setLayout(new java.awt.GridBagLayout());

        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 5;
        gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
        gridBagConstraints.ipadx = 13;
        gridBagConstraints.insets = new java.awt.Insets(16, 17, 0, 56);
        flowPanel.add(familyIndexComboBox, gridBagConstraints);

        familyIndexLabel.setText("Family Index");
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 4;
        gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
        gridBagConstraints.insets = new java.awt.Insets(13, 16, 0, 8);
        flowPanel.add(familyIndexLabel, gridBagConstraints);

        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 3;
        gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
        gridBagConstraints.ipadx = 7;
        gridBagConstraints.insets = new java.awt.Insets(9, 16, 0, 55);
        flowPanel.add(directionComboBox, gridBagConstraints);

        directionLabel.setText("Direction");
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 2;
        gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
        gridBagConstraints.insets = new java.awt.Insets(1, 13, 0, 17);
        flowPanel.add(directionLabel, gridBagConstraints);

        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 1;
        gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
        gridBagConstraints.ipadx = 51;
        gridBagConstraints.insets = new java.awt.Insets(12, 13, 10, 9);
        flowPanel.add(flowComboBox, gridBagConstraints);

        flowLabel.setText("Flow");
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
        gridBagConstraints.ipadx = 47;
        gridBagConstraints.insets = new java.awt.Insets(5, 13, 0, 13);
        flowPanel.add(flowLabel, gridBagConstraints);

        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 2;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.fill = java.awt.GridBagConstraints.BOTH;
        gridBagConstraints.insets = new java.awt.Insets(0, 20, 0, 20);
        add(flowPanel, gridBagConstraints);

        panelSeparator1.setLayout(new java.awt.BorderLayout());
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 1;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.fill = java.awt.GridBagConstraints.BOTH;
        gridBagConstraints.insets = new java.awt.Insets(0, 7, 0, 0);
        add(panelSeparator1, gridBagConstraints);

        panelSeparator2.setLayout(new java.awt.BorderLayout());
        gridBagConstraints = new java.awt.GridBagConstraints();
        gridBagConstraints.gridx = 3;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.fill = java.awt.GridBagConstraints.BOTH;
        gridBagConstraints.insets = new java.awt.Insets(0, 0, 0, 7);
        add(panelSeparator2, gridBagConstraints);
    }// </editor-fold>//GEN-END:initComponents
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JPanel curvePanel;
    private javax.swing.JComboBox directionComboBox;
    private javax.swing.JLabel directionLabel;
    private javax.swing.JComboBox familyIndexComboBox;
    private javax.swing.JLabel familyIndexLabel;
    private javax.swing.JComboBox flowComboBox;
    private javax.swing.JLabel flowLabel;
    private javax.swing.JPanel flowPanel;
    private javax.swing.JComboBox methodComboBox;
    private javax.swing.JLabel methodLabel;
    private javax.swing.JPanel methodPanel;
    private javax.swing.JPanel panelSeparator1;
    private javax.swing.JPanel panelSeparator2;
    // End of variables declaration//GEN-END:variables
    private void addMethodNames() {

        methodComboBox.addItem("ContinuationRarefactionMethod");
        methodComboBox.addItem("RarefactionMethod");
        methodComboBox.addActionListener(new MethodAction());
    }

    private void addFlowNames() {

        flowComboBox.addItem("RarefactionFlow");
        flowComboBox.addItem("ShockFlow");
        flowComboBox.addActionListener(new FlowAction());
    }

    private void directionNames() {

        directionComboBox.addItem("Forward");
        directionComboBox.addItem("Backward");
    }

    private void addFamilyIndex() {

        for (int i = 0; i < RpNumerics.domainDim(); i++) {
            familyIndexComboBox.addItem(new Integer(i));
        }
        familyIndexComboBox.addActionListener(new FamilyIndexAction());

    }

    private class MethodAction implements ActionListener {

        public void actionPerformed(ActionEvent e) {
            JComboBox combo = (JComboBox) e.getSource();
            CurvePlotAgent.instance().setMethodName((String) methodComboBox.getSelectedItem());

        }
    }

    private class FlowAction implements ActionListener {

        public void actionPerformed(ActionEvent e) {
            JComboBox combo = (JComboBox) e.getSource();
            CurvePlotAgent.instance().setFlowName((String) flowComboBox.getSelectedItem());
            if (((String) flowComboBox.getSelectedItem()).matches(".*Shock.*")) {//TODO HardCoded !!Only to show a possible behaviour
                familyIndexComboBox.setEnabled(false);
                directionComboBox.setEnabled(false);
                CurvePlotAgent.instance().setType("shock");
            } else {
                directionComboBox.setEnabled(true);
                familyIndexComboBox.setEnabled(true);
                CurvePlotAgent.instance().setType("rarefaction");
            }
        }
    }

    private class FamilyIndexAction implements ActionListener {

        public void actionPerformed(ActionEvent e) {
            JComboBox combo = (JComboBox) e.getSource();
            CurvePlotAgent.instance().setFamilyIndex(((Integer) familyIndexComboBox.getSelectedItem()).intValue());

        }
    }

    private class DirectionAction implements ActionListener {

        public void actionPerformed(ActionEvent e) {
            JComboBox combo = (JComboBox) e.getSource();

            if (((String) combo.getSelectedItem()).matches("Forward")) {
                CurvePlotAgent.instance().setTimeDirection(1);
            } else {
                CurvePlotAgent.instance().setTimeDirection(-1);
            }
        }
    }
}
