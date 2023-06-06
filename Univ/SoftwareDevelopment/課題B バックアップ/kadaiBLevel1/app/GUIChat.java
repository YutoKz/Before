package kobeU.cs.kadaiB.kadaiBLevel1.app;

import java.io.IOException;
import javax.swing.SwingUtilities;
import kobeU.cs.kadaiB.kadaiBLevel1.comTools.Communicator;

public class GUIChat {

    kobeU.cs.kadaiB.kadaiBLevel1.app.Board board;
    SetupDialog setupDialog;
    Communicator comm;

    /***************************************************
     *  GUI, 標準出力表示
     ***************************************************/
    /**
     * msg の内容を、GUI および標準出力に表示
     * @param sender  表示の際のtag
     * @param msg
     */
    void displayMsgOnBoard(String sender, ObjMessage msg) {
        displayMsgInner(sender + ": " + msg.body + "[" + msg.date + "]");
    }
    /**
     * System からのメッセージを、GUI および標準出力に表示
     */
    void displaySysMsg(String text) {
        displayMsgInner("Sys: " + text);
    }

    void displayMsgInner(final String text) {
        System.out.println(text);
        if(SwingUtilities.isEventDispatchThread()) {
            board.addMessage(text);
        } else {
            SwingUtilities.invokeLater(()->{
                board.addMessage(text);
            });
        }
    }


    /******************************************
     *  GUI routines
     ******************************************/
    /**
     * GUI 処理を行う。
     * User 入力は、GUI event としてシステムのGUI 用スレッドが処理。
     */
    void GUIsetup() {
        assert(SwingUtilities.isEventDispatchThread());
        board = new Board();
        board.setManager(this);
        board.addMessage("Waitting for Connection....");
        board.setVisible(true);
        setupDialog = new SetupDialog(board);
        setupDialog.setModal(true);
    }

    /**
     *  接続先入力用の dialog を出し、接続を行う
     */
    Communicator showDialogAndStartConnection() {
        setupDialog.setEchoText("Please input connection target.");
        setupDialog.setVisible(true);
        if(setupDialog.shouldTerminate()) return null;
        try {
            Communicator c;
            if(setupDialog.asServer()) {
                c = Communicator.startConnectionAsServer(setupDialog.getPort());
            } else {
                c = Communicator.startConnectionAsClient(setupDialog.getHostName(), setupDialog.getPort());
            }
            displaySysMsg("Connection Established");/* 接続成功 */
            return c;
        } catch (IOException e) {
            board.addMessage("Connection failed. Please stop this program."); /* 接続失敗 */
            return null;
        }
    }


    /******************************************
     *  main routines
     ******************************************/
    /**
     * GUI 付きChat プログラムの起動を行う。引数は利用しない。
     * （接続相手はGUIを介して行う）
     * @param args
     */
    public static void main(String[] args) {
        kobeU.cs.kadaiB.kadaiBLevel1.app.GUIChat manager = new kobeU.cs.kadaiB.kadaiBLevel1.app.GUIChat();
        manager.exec();
    }

    /**
     *  manager の全処理を順に行う。
     */
    void exec() {
        try {
            SwingUtilities.invokeAndWait(()-> { /* 依頼を待つ */
                /* GUI Thread に実行依頼 */
                GUIsetup();
                comm = showDialogAndStartConnection();
            });
        } catch (Exception e) {
            System.err.println("Init Error");
            e.printStackTrace();
            return;
        }
        if(comm != null) {
            loopOfMsgRecv();
        }
    }

    /******************************************
     *  Message 受信
     ******************************************/
    /**
     * ネットワーク入力を処理するループ
     */
    void loopOfMsgRecv() {
        try {
            /*
             *  メッセージ受信用ルーチン
             */
            while(true) {
                ObjMessage msg = (ObjMessage)comm.recv();
                if(msg==null) {
                    displaySysMsg("Connection closed.");
                    return;
                }
                displayMsgOnBoard("MSG", msg);
            }
        } catch (IOException e) {
            e.printStackTrace();
            board.addMessage("System: IO Exception! check stderr please!");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            board.addMessage("System: ClassNotFound Exception!");
        }
    }
    /*************************************************
     *   Message 送信
     *************************************************/
    /**
     * 通信メッセージを送信。GUI 側から呼ばれる前提
     */
    public void sendMsg(String text)  {
        assert(SwingUtilities.isEventDispatchThread());
        if(comm==null) {
            displaySysMsg("not connected.");
        } else if(text.equals("exit")) {
            comm.startClosing();
            displaySysMsg("Start closing of connections...");
        } else {
            try {
                ObjMessage msg =new ObjMessage(text);
                comm.send(msg);
                displayMsgOnBoard("SELF", msg);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    /**
     * GUI 側から、終了開始するためのメソッド
     */
    public synchronized void startTermination() {
        assert(SwingUtilities.isEventDispatchThread());
        try {
            if(comm!=null) comm.startClosing();
        } catch(Exception e) {
            e.printStackTrace();
        }
        board.setVisible(false);
    }
}
