import javax.swing.*;
import java.awt.*;

/**
 * Created by Bing on 4/18/17.
 */
public class NavigationPanel extends Panel {
    private JLabel mTitleLabel;
    private JPanel mTextPanel;

    public NavigationPanel() {
        init();
    }

    void updatePersonLogin() {
        if (GUI.getStackSize() == 0) {
            mTitleLabel.setText(GlobalData.getInstance().getLoginPerson() != null ?
                    GlobalData.getInstance().getLoginPerson().WelcomeString() : "1");
        } else {
            mTitleLabel.setText(GlobalData.getInstance().getLoginPerson() != null ?
                    GlobalData.getInstance().getLoginPerson().naviString() : "2");
        }
    }

    void updatePersonLoginLayout() {

        if (GUI.getStackSize() == 0) {
            mTextPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        } else {
            mTextPanel.setLayout(new FlowLayout(FlowLayout.TRAILING));

        }
    }

    void init() {
        setVisible(true);
//        setBackground(Color.MAGENTA);
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(2, 1, 0, 4));

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new GridLayout(1, 3, 0, 4));

        JButton back = new JButton("                        Back                         ");
        back.addActionListener(actionEvent -> GUI.goBack());
        back.setMinimumSize(new Dimension(100, 100));

        JButton home = new JButton("Home");
        home.addActionListener(actionEvent -> GUI.home());

        JButton logOut = new JButton("Log Out");
        logOut.addActionListener(actionEvent -> GUI.logOut());

        mTextPanel = new JPanel();
        updatePersonLoginLayout();

        mTitleLabel = new JLabel();
        mTitleLabel.setFont(new Font("Dialog", Font.PLAIN, 16));
        updatePersonLogin();

        add(panel);
        panel.add(buttonPanel);
        buttonPanel.add(back);
        buttonPanel.add(home);
        buttonPanel.add(logOut);
        panel.add(mTextPanel);
        mTextPanel.add(mTitleLabel);
    }
}