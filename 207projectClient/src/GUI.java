import javax.swing.*;
import java.util.Stack;

public class GUI {
    public static final int WIDTH = 1000;
    public static final int HEIGHT = 800;
    public static final int NAV_HEIGHT = 80;
    public static JFrame smFrame;
    private static NavigationPanel mNavigationPanel;
    private static Stack<Panel> mStack = new Stack<>();

    static int getStackSize() {
        return mStack.size();
    }

    static void goBack() {
        if (mStack.size() > 1) {
            mStack.pop();
            Panel panel = mStack.pop();
            switchTo(panel, true);
        }
    }

    static void home() {
        while (mStack.size() > 1) {
            mStack.pop();
        }
        Panel panel = mStack.pop();
        switchTo(panel, true);
    }

    static void logOut() {
        if (mStack.size() > 0) {
            mStack.clear();
            initApp();
        }
    }

    static JPanel switchTo(Panel panel, boolean addToStack) {
        panel.setLayout(null);

        if (addToStack) {
            mNavigationPanel.updatePersonLogin();
            panel.add(mNavigationPanel);
            mStack.push(panel);
        }
        panel.setSize(WIDTH, HEIGHT);
        smFrame.setContentPane(panel);

        return panel;
    }

    public static void main(String argv[]) {
        new Thread(() -> Connection.getInstance().connectHost()).start();

        smFrame = new JFrame();
//        smFrame.setBackground(Color.blue);
        mNavigationPanel = new NavigationPanel();
        mNavigationPanel.setBounds(0, 0, WIDTH, NAV_HEIGHT);
        initApp();
    }

    private static void initApp() {
        smFrame.setSize(WIDTH, HEIGHT);
        smFrame.setLayout(null);
        smFrame.setVisible(true);
        switchTo(new LoginPanel(), false);
    }
}