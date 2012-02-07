/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */
package rpn;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Set;
import rpnumerics.Configuration;
import rpnumerics.RPNUMERICS;

public class RPnConfigurationDialog extends RPnDialog {

    private JTabbedPane extensionPanel_;

    public RPnConfigurationDialog() {
        super(false, false);
        System.out.println("Dentro do construtor de RPnConfigurationDialog");

        try {
            jbInit();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void jbInit() throws Exception {
        System.out.println("Dentro do método jbInit()");
        setTitle("Configuration");
        extensionPanel_ = new JTabbedPane();
        applyButton.setText("OK");
        extensionPanel_.setTabLayoutPolicy(JTabbedPane.SCROLL_TAB_LAYOUT);
        HashMap<String, Configuration> configMap = RPNUMERICS.getConfigurations();

        Set<Entry<String, Configuration>> configSet = configMap.entrySet();
        System.out.println("Tamanho de configSet.size() : " +configSet.size());

        System.out.println("Pronto pra entrar no loop");

        for (Entry<String, Configuration> entry : configSet) {
            System.out.println("Entrou no loop");

            String configurationType = entry.getValue().getType();

            if (!configurationType.equalsIgnoreCase("PHYSICS") && !configurationType.equalsIgnoreCase("VISUAL")) {

                RPnInputComponent inputComponent = new RPnInputComponent(entry.getValue());

                extensionPanel_.addTab(entry.getKey(), inputComponent.getContainer());

            }


        }

        setMinimumSize(new Dimension(getTitle().length() * 50, 40));

        getContentPane().add(extensionPanel_, BorderLayout.CENTER);

        extensionPanel_.getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT).put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "Apply");
        extensionPanel_.getActionMap().put("Apply", applyButton.getAction());


        System.out.println("Fim do método jbInit()");
        pack();
    }

    @Override
    protected void cancel() {
        dispose();
    }

    protected void apply() {

        dispose();

    }

    @Override
    protected void begin() {
        System.out.println("Chamando begin do extension config");
        dispose();
    }
}
