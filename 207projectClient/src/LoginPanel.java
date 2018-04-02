import com.google.gson.Gson;
import model.Error;
import model.ListPerson;
import model.Person;

import javax.swing.*;
import java.awt.*;

/**
 * Created by lam on 4/9/17.
 */
public class LoginPanel extends Panel {
    public LoginPanel() {
        init();
    }

    void init() {
        Connection.setListener(message -> {
//            System.out.println(message);
            Gson gson = new Gson();
//            System.out.println("message " + message);

            try {
                ListPerson listPerson = gson.fromJson(message, ListPerson.class);
                Person person = listPerson.result.get(0);
                GlobalData.getInstance().setLoginPerson(person);

                if (person.role == 2 || person.role == 1) {
                    // if this is professor or student
                    GUI.switchTo(new MainPanel(MainPanel.LIST_TYPE.COURSE, person.email), true);
                } else {
                    // system admin
                    GUI.switchTo(new AdminPanel(), true);
                    System.out.println("go to system admin");
                }
            } catch (Exception e) {
                Error error = gson.fromJson(message, Error.class);
                JOptionPane.showMessageDialog(this, "Incorrect username or password!", "Error", JOptionPane.ERROR_MESSAGE);
                System.out.println(error);
            }
        });

        setLayout(new GridBagLayout());

        setVisible(true);

        JPanel panel = new JPanel();
        panel.setSize(GUI.WIDTH, GUI.HEIGHT);
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
//        panel.setBackground(Color.pink);

        JPanel panelHolder1 = new JPanel();
        panelHolder1.setMaximumSize(new Dimension(300, 40));

        JPanel loginMessagePanel = new JPanel();
        loginMessagePanel.setMaximumSize(new Dimension(260, 50));
        JLabel loginLabel = new JLabel("Welcome to School Book     ");
        loginLabel.setFont(new Font("Dialog", Font.BOLD, 16));

        JPanel panelHolder2 = new JPanel();
        panelHolder2.setMaximumSize(new Dimension(300, 40));

        JPanel userNamePanel = new JPanel();
        userNamePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        userNamePanel.setBackground(Color.yellow);
        userNamePanel.setMaximumSize(new Dimension(300, 40));

        JLabel userNameLabel = new JLabel("Username:");

        JTextField usernameTF = new JTextField();
        usernameTF.setColumns(15);

        JPanel passwordPanel = new JPanel();
        passwordPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        passwordPanel.setBackground(Color.yellow);
        passwordPanel.setMaximumSize(new Dimension(300, 40));

        JLabel passwordLabel = new JLabel("Password: ");

        JPasswordField passwordTF = new JPasswordField();
        passwordTF.setColumns(15);

        JButton b = new JButton("Login");
        b.setAlignmentX(java.awt.Component.CENTER_ALIGNMENT);
//        b.setBounds(130, 100, 100, 40);

        b.addActionListener(actionEvent -> {
            if (usernameTF.getText() == null || usernameTF.getText().isEmpty() || passwordTF.getText() == null || passwordTF.getText().isEmpty()) {
                System.out.println("Fail to login");
                JOptionPane.showMessageDialog(panel, "Please fill in all the fields to log in!", "Error", JOptionPane.ERROR_MESSAGE);
            } else {
                new Thread(() -> {
                    connection.sendMessage("check " + usernameTF.getText() + " " + passwordTF.getText());
//                    Connection.setListener(message -> {
//                        try {
//                            if(message.contains("error")){
//                                JOptionPane.showMessageDialog(panel, "Incorrect username or password!", "Error", JOptionPane.ERROR_MESSAGE);
//                            }
//                        } catch (Exception e) {
//                            //Eat comment
//                        }
//                    });
                }).start();
            }
        });

        add(panel, new GridBagConstraints());
        panel.add(panelHolder1);
        panel.add(loginMessagePanel);
        loginMessagePanel.add(loginLabel);
//        panel.add(panelHolder2);
        panel.add(userNamePanel);
        userNamePanel.add(userNameLabel);
        userNamePanel.add(usernameTF);
        panel.add(passwordPanel);
        passwordPanel.add(passwordLabel);
        passwordPanel.add(passwordTF);
        panel.add(b);
    }
}