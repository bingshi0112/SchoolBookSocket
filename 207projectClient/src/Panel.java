import javax.swing.*;

/**
 * Created by lam on 4/9/17.
 */
public abstract class Panel extends JPanel {
    protected Connection connection = Connection.getInstance();

    public Panel() {

//        init();
    }
    abstract void init();
}
