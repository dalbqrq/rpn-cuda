/*
 * Instituto de Matematica Pura e Aplicada - IMPA
 * Departamento de Dinamica dos Fluidos
 *
 */

package rpn.controller;

import rpn.RPnPhaseSpacePanel;
import java.util.List;
import java.awt.Point;
import java.awt.geom.Rectangle2D;

public interface PhaseSpacePanelController {
    void install(RPnPhaseSpacePanel panel);

    void uninstall(RPnPhaseSpacePanel panel);

    public void evaluateCursorCoords(RPnPhaseSpacePanel clickedPanel, Point point);

    public void resetCursorCoords();

    List  pointMarkBuffer();
    List <Rectangle2D.Double> getSelectionAreas();

    Point get_dc_CompletePoint();

    void set_dc_CompletePoint(Point point);

    int getAbsIndex();

    int getOrdIndex();

    boolean isAbsComplete();

    boolean isOrdComplete();

}
