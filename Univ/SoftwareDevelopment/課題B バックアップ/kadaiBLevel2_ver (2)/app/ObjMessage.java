package kobeU.cs.kadaiB.kadaiBLevel2.app;

import java.io.PrintWriter;
import java.io.Serializable;
import java.io.StringWriter;
import java.util.Date;

public class ObjMessage implements Serializable {

    private static final long serialVersionUID = 3249940705431916304L;
    public final Object body;
    public final Date  date;
    public ObjMessage(Object body) {
        this.body = body;
        this.date = new Date();
    }
    public String toString() {
        StringWriter out0 = new StringWriter();
        PrintWriter out = new PrintWriter(out0);
        out.println("[ObjMsg:" + body + ",");
        out.println(" Date:" + date);
        out.close();
        return out0.toString();
    }
}
