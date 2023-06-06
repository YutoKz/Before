package kobeU.cs.kadaiB.kadaiBLevel1.comTools;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.ArrayList;
import javax.swing.JOptionPane;

public class CommunicationHub {
    ArrayList<kobeU.cs.kadaiB.kadaiBLevel1.comTools.ClientProxy> clients = new ArrayList<>();
    private ServerSocket ss;
    private final CommunicationHubGUI board;
    CommunicationHub() {
        board = new CommunicationHubGUI();
        board.setManager(this);
        board.setVisible(true);
    }

    /**************************************************************
     *  基本機能群
     **************************************************************/

    synchronized void broadcastMsg(Object msg, kobeU.cs.kadaiB.kadaiBLevel1.comTools.ClientProxy sender) {
        assert(sender!=null);
        for(kobeU.cs.kadaiB.kadaiBLevel1.comTools.ClientProxy client: clients) {
            if(sender != client)
                client.sendMsg(msg);
        }
    }
    public synchronized void serverTermination() {
        board.addLog("server termination");
        for(kobeU.cs.kadaiB.kadaiBLevel1.comTools.ClientProxy client: clients) {
            client.startTermination();
        }
        clients.clear();
        closeServerSocket();
    }
    synchronized void join(kobeU.cs.kadaiB.kadaiBLevel1.comTools.ClientProxy proxy) {
        board.addLog("join: "+ proxy.getName());
        clients.add(proxy);
    }
    synchronized void remove(kobeU.cs.kadaiB.kadaiBLevel1.comTools.ClientProxy proxy) {
        board.addLog("remove: "+ proxy.getName());
        clients.remove(proxy);
    }
    synchronized void process(Object msg, kobeU.cs.kadaiB.kadaiBLevel1.comTools.ClientProxy sender) {
        board.addLog("process msg from " + sender.getName() + ":" +  msg);
        broadcastMsg(msg, sender);
    }

    public void waitClientLoop(int port) throws IOException {
        if(port < 0) {
            board.addLog("Wrong input for Port Number: " + port);
            return;
        }
        ss = new ServerSocket(port);
        try {
            int counter = 0;
            while(true) {
                Socket socket = ss.accept();
                String name ="client" + counter;
                kobeU.cs.kadaiB.kadaiBLevel1.comTools.ClientProxy client;
                client = new ClientProxy(name, socket, this);
                new Thread(client).start();
                counter++;
            }
        } catch (SocketException e) {
            synchronized (this) {
                if(ss!=null) {
                    e.printStackTrace();
                } else {
                    /* This is the case where other thread closed the server socket,
                     * and ignore the exception.  */
                }
            }

        } finally {
            closeServerSocket();
        }
    }

    /*********************************************************
     *  connection 関係
     *********************************************************/

    public synchronized void closeServerSocket() {
        if(ss==null) return;
        try { if(!ss.isClosed()) ss.close();} catch (IOException e) {/* ignore closing exception */}
        ss = null;
    }

    public static int getPortNum() {
        String result = JOptionPane.showInputDialog("Input port number:", kobeU.cs.kadaiB.kadaiBLevel1.comTools.Communicator.getDefaultPort());
        return Communicator.checkPortNum(result);
    }

    public static void main(String[] args) throws IOException {
        kobeU.cs.kadaiB.kadaiBLevel1.comTools.CommunicationHub multiManager = new kobeU.cs.kadaiB.kadaiBLevel1.comTools.CommunicationHub();
        int port = getPortNum();
        multiManager.waitClientLoop(port);
    }
}
