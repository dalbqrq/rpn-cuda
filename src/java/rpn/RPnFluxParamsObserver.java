/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package rpn;

import java.awt.event.TextListener;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;
import java.util.Set;
import javax.swing.JOptionPane;
import javax.swing.event.DocumentListener;
import rpnumerics.Configuration;
import rpnumerics.FluxParams;
import rpnumerics.RPNUMERICS;
import wave.util.RealVector;

/**
 *
 * @author moreira
 */
public class RPnFluxParamsObserver implements RPnObserver {

    private RPnFluxParamsSubject fluxParamsSubject_;
    private Configuration configuration_;
    private RPnInputComponent inputComponent_;

    
    public RPnFluxParamsObserver(RPnFluxParamsSubject fluxParamsSubject) {
        System.out.println("Entrei em RPnFluxParamsObserver(RPnFluxParamsSubject fluxParamsSubject)");
        fluxParamsSubject_ = fluxParamsSubject;
    }

    public RPnFluxParamsObserver(Configuration fluxConfiguration) {
        System.out.println("Entrei em RPnFluxParamsObserver(Configuration fluxConfiguration)");
        configuration_ = fluxConfiguration;
    }

    public void setView(RPnInputComponent inputComponent) {
        inputComponent_ = inputComponent;
    }

    public void update() {

        Configuration physicsConfiguration = RPNUMERICS.getConfiguration(RPNUMERICS.physicsID());

        Configuration fluxConfiguration = physicsConfiguration.getConfiguration("fluxfunction");

        
        //*** Leandro teste
        for (int i = 0; i < fluxParamsSubject_.getState().length; i++) {
            String paramKey = fluxConfiguration.getParamName(i);
            fluxConfiguration.setParamValue(paramKey, String.valueOf(fluxParamsSubject_.getState()[i]));
        }


    }

    public Configuration getConfiguration() {
        return configuration_;
    }

    public void setSubject(RPnFluxParamsSubject fluxParamsSubject) {
        fluxParamsSubject_ = fluxParamsSubject;
    }

    public void update(RPnSubject subject) {

        //System.out.println("RPnFluxParamsObserver : Estou na aba ... " +subject.getName());

        //*** Leandro teste
        for (int i = 0; i < subject.getState().length; i++) {
            String paramKey = configuration_.getParamName(i);
            configuration_.setParamValue(paramKey, String.valueOf(subject.getState()[i]));      // parece que nao esta sendo usado

            //*** futuramente, getTextField será trocado por algum método que vai atualizar a visao daquele inputComponent
            inputComponent_.getTextField()[i].setText(String.valueOf(subject.getState()[i]));   // se desativado, vai certo para camada numerica, mas nao atualiza visao

        }


    }
}
