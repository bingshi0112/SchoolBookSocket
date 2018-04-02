import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;

/**
 * Created by Bing on 4/18/17.
 */
public class TextPanel extends Panel {
    private String mPath;

    public TextPanel(String path) {
        mPath = path;
        init();
    }

    void init() {
        JPanel panel = new JPanel();
        panel.setBounds(0, GUI.NAV_HEIGHT, GUI.WIDTH, (GUI.HEIGHT - GUI.NAV_HEIGHT));
        panel.setLayout(null);
        panel.setBackground(Color.pink);

        JEditorPane editorPane = new JEditorPane();
        editorPane.setEditable(false);

        File file = new File("/media/lam/LAM/SJSU/Spring2017/207/ui_project/filestores/" + mPath);

        try {
            editorPane.setPage(file.toURI().toURL());
            System.out.print(file.toURI().toURL());
        } catch (IOException e) {
            e.printStackTrace();
        }


        editorPane.setSize(new Dimension(GUI.WIDTH, GUI.HEIGHT));
        add(panel);
        panel.add(editorPane);
    }
}