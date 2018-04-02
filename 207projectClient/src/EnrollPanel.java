import javax.swing.*;
import java.awt.*;

/**
 * Created by Bing on 4/18/17.
 */
public class EnrollPanel extends Panel {
    private int mCourseId;
    public EnrollPanel(int CourseId) {
        mCourseId = CourseId;
        init();
    }


    void init() {
        Connection.removeListener();
        setLayout(new GridBagLayout());
        setVisible(true);

        JPanel panel = new JPanel();
        panel.setBounds(0, GUI.NAV_HEIGHT, GUI.WIDTH, GUI.HEIGHT - GUI.NAV_HEIGHT);
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));


        JPanel titlePanel = new JPanel();
        titlePanel.setMaximumSize(new Dimension(300, 40));
        JLabel titleLabel = new JLabel("Enroll to Course");
        titleLabel.setFont(new Font("Dialog", Font.BOLD, 18));


        JPanel emailPanel = new JPanel();
        emailPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
        emailPanel.setMaximumSize(new Dimension(300, 40));

        JLabel emailLabel = new JLabel("Email:  ");

        JTextField emailTF = new JTextField();
        emailTF.setColumns(15);


        JPanel insertButtonPanel = new JPanel();
        insertButtonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        insertButtonPanel.setMaximumSize(new Dimension(300, 40));

        JButton clearButton = new JButton("Clear");
        JButton enrollButton = new JButton("Enroll");

        enrollButton.addActionListener(actionEvent -> {
            if (emailTF.getText().isEmpty() || emailTF.getText().length() == 0 || emailTF.getText().equals("") || emailTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter email!", "Error", JOptionPane.ERROR_MESSAGE);
            } else {
                connection.sendMessage("enroll person email " + emailTF.getText() + " course id " + mCourseId);
                Connection.setListener(message -> {
                    try {
                        if(message.contains("success")){
                            JOptionPane.showMessageDialog(panel, "Successfully enroll a student to a new course!", "Success", JOptionPane.INFORMATION_MESSAGE);
                            emailTF.setText("");
                        }
                        if(message.contains("error")){
                            JOptionPane.showMessageDialog(panel, "Error encountered, please enter a valid email!", "Error", JOptionPane.ERROR_MESSAGE);
                        }
                    } catch (Exception e) {
                        //Eat comment
                    }
                });
            }
        });

        clearButton.addActionListener(actionEvent -> {
            emailTF.setText("");
        });

        add(panel, new GridBagConstraints());
        panel.add(titlePanel);
        titlePanel.add(titleLabel);
        panel.add(emailPanel);
        emailPanel.add(emailLabel);
        emailPanel.add(emailTF);
        panel.add(insertButtonPanel);
        insertButtonPanel.add(clearButton);
        insertButtonPanel.add(enrollButton);

//        Connection.setListener(tp::setText);
    }
}