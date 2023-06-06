package kobeU.cs.kadaiB.kadaiBLevel2.comTools;

import java.io.IOException;
import java.net.Socket;

public class ClientProxy implements Runnable{
    protected CommunicationHub hub;
    private final String name;
    protected Communicator comm;

    public ClientProxy(String name, Socket socket, CommunicationHub hub) throws IOException {
        this.name = name;
        this.hub= hub;
        this.comm = new Communicator(socket);
        hub.join(this);
    }

    public String getName() { return name; }

    /*************************************************************
     *  message 送信 (-> Client)
     *************************************************************/

    public void sendMsg(Object msg) {
        try {
            comm.send(msg);
        } catch (IOException e) {
            hub.remove(this);
        }
    }

    /*************************************************************
     *  message 受信ループ (from Client)
     *************************************************************/

    public void run() {
        try {
            /**
             * 受信用ルーチン
             */
            while(true) {
                Object msg =  comm.recv();
                if(msg==null) {
                    hub.remove(this);
                    return;
                }
                hub.process(msg, this);
            }
        } catch (ClassNotFoundException | IOException e) {
            e.printStackTrace();
            hub.remove(this);
        }
    }

    /**
     * 終了作業依頼
     */
    public synchronized void startTermination() {
        comm.startClosing();
    }
}
