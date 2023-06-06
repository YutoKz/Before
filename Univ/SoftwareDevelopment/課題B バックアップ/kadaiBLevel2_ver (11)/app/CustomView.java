package kobeU.cs.kadaiB.kadaiBLevel2.app;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

public class CustomView extends JPanel {

    ArrayList<Integer> points = new ArrayList<Integer>();
    Point before, next;
    Color currentColor = Color.WHITE;
    float currentWidth = 1;

    public void setPenColor(String color) {
        switch (color) {
            case "ERASER":
                currentColor = Color.WHITE;
                break;
            case "Black":
                currentColor = Color.BLACK;
                break;
            case "Red":
                currentColor = Color.RED;
                break;
            case "Green":
                currentColor = Color.GREEN;
                break;
            case "Yellow":
                currentColor = Color.YELLOW;
                break;
            case "Blue":
                currentColor = Color.BLUE;
                break;
            case "Orange":
                currentColor = Color.ORANGE;
                break;
            case "Pink":
                currentColor = Color.PINK;
                break;
        }
    }

    public void setPenWidth(String width){
        currentWidth = Float.parseFloat(width);
    }

    /**
     * Create the panel.
     */
    public CustomView() {
        for(int i = 0; i < 4; i++) {
            points.add(0);
        }
        before = new Point();
        next   = new Point();
        before.x = 0;
        before.y = 0;
        next.x = 0;
        next.y = 0;
    }

    @Override
    public void paintComponent(Graphics g0) {
        Graphics2D g = (Graphics2D)g0;
        g.setBackground(Color.WHITE);
        g.clearRect(0, 0, getWidth(), getHeight());
        //g.setColor(Color.BLACK);
        //for(Rectangle rect: shapes) {
        //    g.fill(rect);
        //}
        //g.setColor(Color.RED);
        //if(dragCurrent!=null)
        //    g.fill(genRect(dragBegin, dragCurrent));
    }


    public void drawLine(int x1, int y1, int x2, int y2){
        Graphics2D g = (Graphics2D) this.getGraphics();
        g.setColor(currentColor);
        g.setStroke(new BasicStroke(currentWidth ,BasicStroke.CAP_ROUND,BasicStroke.JOIN_MITER));
        g.drawLine(x1, y1, x2, y2);
    }

    /*
    public static Rectangle genRect(Point a, Point b) {
        return new Rectangle(Math.min(a.x, b.x), Math.min(a.y, b.y), Math.abs(a.x-b.x), Math.abs(a.y-b.y));
    }


    public void addShape(Point a, Point b) {
        shapes.add(genRect(a,b));
    }
    */
    // public Rectangle getLastShape() { return shapes.get(shapes.size() - 1); }
    public ArrayList<Integer> getPoints() { return points; }

    /*
    public void recvRect(Rectangle r) {
        shapes.add(r);
        repaint();
    } */

    public void recvPointsList(ArrayList<Integer> list) {
        drawLine(list.get(0), list.get(1), list.get(2), list.get(3));
    }

    /*
    public void undo() {
        if(shapes.isEmpty()) return;
        shapes.remove(shapes.size()-1);
        repaint();
    }
*/

    public void reset() {
        //shapes.clear();
        repaint();
    }



    public void mouseDragged(Point point) {
        next.x = point.x;
        next.y = point.y;
        drawLine(before.x,before.y,next.x,next.y);

        points.set(0, before.x);
        points.set(1, before.y);
        points.set(2, next.x);
        points.set(3, next.y);

        before.x=next.x;
        before.y=next.y;
    }
    /*
    public void mouseReleased(Point point) {

        if(!point.equals(dragBegin)) {
            addShape(dragBegin, point)
        }
        dragBegin = dragCurrent = null
        repaint();

    }
*/
    public void mousePressed(Point point) {
        // dragBegin = point;
        before.x = point.x;
        before.y = point.y;
    }




}
