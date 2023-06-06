package kobeU.cs.kadaiB.kadaiBLevel2.app;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class CustomView extends JPanel {

    List<Rectangle> shapes = new ArrayList<>();
    Point dragBegin, dragCurrent;

    /**
     * Create the panel.
     */
    public CustomView() {

    }

    @Override
    public void paintComponent(Graphics g0) {
        Graphics2D g = (Graphics2D)g0;
        g.setBackground(Color.WHITE);
        g.clearRect(0, 0, getWidth(), getHeight());
        g.setColor(Color.BLUE);
        for(Rectangle rect: shapes) {
            g.fill(rect);
        }
        g.setColor(Color.RED);
        if(dragCurrent!=null)
            g.fill(genRect(dragBegin, dragCurrent));
    }
    public static Rectangle genRect(Point a, Point b) {
        return new Rectangle(Math.min(a.x, b.x), Math.min(a.y, b.y), Math.abs(a.x-b.x), Math.abs(a.y-b.y));
    }

    public void addShape(Point a, Point b) {
        shapes.add(genRect(a,b));
    }
    public Rectangle getLastShape() { return shapes.get(shapes.size() - 1); }
    public void recvRect(Rectangle r) {
        shapes.add(r);
        repaint();
    }
    public void undo() {
        if(shapes.isEmpty()) return;
        shapes.remove(shapes.size()-1);
        repaint();
    }
    public void reset() {
        shapes.clear();
        repaint();
    }

    public void mouseDragged(Point point) {
        dragCurrent=point;
        repaint();
    }

    public void mouseReleased(Point point) {
        if(!point.equals(dragBegin)) {
            addShape(dragBegin, point);

            // ここで他の人に送信
        }




        dragBegin = dragCurrent = null;
        repaint();
    }

    public void mousePressed(Point point) {
        dragBegin = point;
    }


}
