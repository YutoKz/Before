package kobeU.cs.kadaiB.kadaiBLevel2.app;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Board extends JFrame {
    private static final long serialVersionUID = 1L;
    private JPanel jContentPane = null;
    private JPanel leftJContentPane = null;
    private JPanel rightJContentPane = null;
    private JPanel rightUnderJContentPane = null;
    private JPanel upperPanel = null;
    private JTextField jTextField = null;
    private JScrollPane jScrollPane = null;
    private GUIChat manager = null;  //  @jve:decl-index=0:
    private JTextArea jTextArea = null;
    private JLabel jLabel = null;

    // 追加
    private CustomView customView = null;
    private JButton btnReset;
    private JButton btnUndo;
    /**
     * This is the default constructor
     */
    public Board() {
        super();
        initialize();
    }

    /**
     * This method initializes this
     */
    private void initialize() {
        this.setSize(800, 400);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setPreferredSize(new Dimension(1000, 2000));
        this.setContentPane(getJContentPane());
        this.setTitle("ChatBoard");
        this.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                manager.startTermination();
            }
        });
    }

    /**
     * This method initializes jContentPane
     *
     * @return javax.swing.JPanel
     */
    private JPanel getJContentPane() {
        if (jContentPane == null) {
            jContentPane = new JPanel();
            jContentPane.setLayout(new BoxLayout(getJContentPane(), BoxLayout.X_AXIS));
            jContentPane.add(getLeftPane(), null);
            jContentPane.add(getRightPane(), null);
        }
        return jContentPane;
    }




    // 追加
    private JPanel getLeftPane() {
        if (leftJContentPane == null) {
            leftJContentPane = new JPanel();
            leftJContentPane.setLayout(new BoxLayout(getLeftPane(), BoxLayout.Y_AXIS));
            leftJContentPane.add(getUpperPanel(), null);
            leftJContentPane.add(getJScrollPane(), null);
        }
        return leftJContentPane;
    }




    /**
     * This method initializes upperPanel
     *
     * @return javax.swing.JPanel
     */
    private JPanel getUpperPanel() {
        if (upperPanel == null) {
            jLabel = new JLabel();
            jLabel.setText(" answer: ");
            jLabel.setDisplayedMnemonic(KeyEvent.VK_UNDEFINED);
            upperPanel = new JPanel();
            upperPanel.setLayout(new BoxLayout(getUpperPanel(), BoxLayout.X_AXIS));
            upperPanel.setPreferredSize(new Dimension(400, 80));
            upperPanel.add(jLabel, null);
            upperPanel.add(getJTextField(), null);
        }
        return upperPanel;
    }

    /**
     * This method initializes jTextField
     *
     * @return javax.swing.JTextField
     */
    private JTextField getJTextField() {
        if (jTextField == null) {
            jTextField = new JTextField();
            jTextField.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    manager.sendMsg(jTextField.getText());
                    jTextField.setText("");
                }
            });
        }
        return jTextField;
    }

    /**
     * This method initializes jScrollPane
     *
     * @return javax.swing.JScrollPane
     */
    private JScrollPane getJScrollPane() {
        if (jScrollPane == null) {
            jScrollPane = new JScrollPane();
            jScrollPane.setPreferredSize(new Dimension(400, 400));
            jScrollPane.setViewportView(getJTextArea());
        }
        return jScrollPane;
    }

    /**
     * This method initializes jTextArea
     *
     * @return javax.swing.JTextArea
     */
    private JTextArea getJTextArea() {
        if (jTextArea == null) {
            jTextArea = new JTextArea();
            jTextArea.setLineWrap(true);
            jTextArea.setEditable(false);
        }
        return jTextArea;
    }


    // 追加
    private JPanel getRightPane() {
        if (rightJContentPane == null) {
            rightJContentPane = new JPanel();
            rightJContentPane.setLayout(new BoxLayout(getRightPane(), BoxLayout.Y_AXIS));

            customView = new CustomView();
            customView.addMouseListener(new MouseAdapter() {
                @Override
                public void mousePressed(MouseEvent e) {
                    customView.mousePressed(e.getPoint());
                }
            });
            /*
            customView.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseReleased(MouseEvent e) {
                    customView.mouseReleased(e.getPoint());
                    manager.sendRect(customView.getLastShape());

                }
            });
            */
            customView.addMouseMotionListener(new MouseMotionAdapter() {
                @Override
                public void mouseDragged(MouseEvent e) {
                    customView.mouseDragged(e.getPoint());
                }
            });
            rightJContentPane.add(customView, null);

            rightJContentPane.add(getRightUnderPane(), null);

            // rightJContentPane.add(new JTextField(), null);
            rightJContentPane.setPreferredSize(new Dimension(400, 400));
        }
        return rightJContentPane;
    }

    private JPanel getRightUnderPane() {
        if (rightUnderJContentPane == null) {
            rightUnderJContentPane = new JPanel();
            rightUnderJContentPane.setLayout(new BoxLayout(getRightUnderPane(), BoxLayout.X_AXIS));
            btnUndo = new JButton("Undo");
            btnUndo.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    customView.undo();
                    manager.undoButton();
                }
            });
            rightUnderJContentPane.add(btnUndo, null);
            btnReset = new JButton("Reset");
            btnReset.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    customView.reset();
                    manager.resetButton();
                }
            });
            rightUnderJContentPane.add(btnReset, null);

            rightUnderJContentPane.setPreferredSize(new Dimension(400, 40));
        }
        return rightUnderJContentPane;
    }






    /**
     * public methods
     */
    public void setManager(GUIChat chatManager) {
        this.manager = chatManager;
    }
    public void addMessage(String text) {
        jTextArea.insert(text+"\n\n", 0);
        jTextArea.setCaretPosition(0);
    }


    // 追加
    public void recvRectangle(Rectangle r) {
        customView.recvRect(r);
    }
    public void undoPressed() {
        customView.undo();
    }
    public void resetPressed() {
        customView.reset();
    }


    @Override
    public void setVisible(boolean flag) {
        if((manager==null)&& flag) {
            System.err.println("Please set manager before setting GUIChatBoard visible.");
        } else {
            super.setVisible(flag);
        }
    }
}  //  @jve:decl-index=0:visual-constraint="69,9"
