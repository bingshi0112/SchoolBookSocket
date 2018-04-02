import model.Person;

import javax.swing.*;
import java.awt.*;

/**
 * Created by lam on 4/16/17.
 */
public class CustomCellenderer extends DefaultListCellRenderer {
    @Override
    public Component getListCellRendererComponent(JList<?> jList, Object o, int i, boolean b, boolean b1) {
        super.getListCellRendererComponent(jList, o, i, b, b1);
        if (o instanceof Person) {
            setText(o.toString());
        }

        return this;
    }
}
