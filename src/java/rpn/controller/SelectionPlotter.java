/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */

package rpn.controller;

import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Path2D;
import rpn.RPnPhaseSpacePanel;
import wave.multid.Coords2D;
import wave.multid.CoordsArray;
import wave.multid.Space;
import wave.multid.view.ViewingTransform;

public abstract class SelectionPlotter implements  MouseMotionListener, MouseListener{

    protected Path2D.Double plotWCPath(Point cursorPos_, MouseEvent me, RPnPhaseSpacePanel panel) {
        ViewingTransform viewingTransform = panel.scene().getViewingTransform();

        double[] cursorPosArray = {cursorPos_.x, cursorPos_.y};
        double[] mePosArray = {me.getX(), me.getY()};

        CoordsArray cursorPosWC = new CoordsArray(new Space(" ", 2));
        CoordsArray mePosWC = new CoordsArray(new Space(" ", 2));

        Coords2D cursorPosDC = new Coords2D(cursorPosArray);
        Coords2D mePosDC = new Coords2D(mePosArray);

        viewingTransform.dcInverseTransform(cursorPosDC, cursorPosWC);
        viewingTransform.dcInverseTransform(mePosDC, mePosWC);

        Path2D.Double selectionPath = new Path2D.Double();

        selectionPath.moveTo(cursorPosWC.getElement(0), cursorPosWC.getElement(1));

        selectionPath.lineTo(mePosWC.getElement(0), cursorPosWC.getElement(1));

        selectionPath.lineTo(mePosWC.getElement(0), mePosWC.getElement(1));

        selectionPath.lineTo(cursorPosWC.getElement(0), mePosWC.getElement(1));

        selectionPath.closePath();
        return selectionPath;


    }
}
