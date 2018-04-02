import javax.swing.*;
import java.awt.*;
import javax.swing.ButtonGroup;

/**
 * Created by Bing on 4/18/17.
 */
public class InsertPersonPanel extends Panel {
    public InsertPersonPanel() {
        init();
    }

    void init() {
        Connection.removeListener();
        setLayout(new GridBagLayout());
//        setLayout(new MigLayout());
        setVisible(true);

        JPanel panel = new JPanel();
        panel.setBounds(0, GUI.NAV_HEIGHT, GUI.WIDTH, GUI.HEIGHT - GUI.NAV_HEIGHT);
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
//        panel.setBackground(Color.pink);


        JPanel titlePanel = new JPanel();
        titlePanel.setMaximumSize(new Dimension(300, 40));
        JLabel titleLabel = new JLabel("Insert Person");
        titleLabel.setFont(new Font("Dialog", Font.BOLD, 18));


        JPanel firstNamePanel = new JPanel();
        firstNamePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        firstNamePanel.setBackground(Color.yellow);
        firstNamePanel.setMaximumSize(new Dimension(300, 40));

        JLabel firstNameLabel = new JLabel("First Name:");
//        firstNameLabel.setFont(new Font("Dialog", Font.PLAIN, 24));

        JTextField firstNameTF = new JTextField();
        firstNameTF.setColumns(15);
//        firstNameTF.setFont(new Font( "Dialog", Font.PLAIN, 24));


        JPanel lastNamePanel = new JPanel();
        lastNamePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        lastNamePanel.setBackground(Color.yellow);
        lastNamePanel.setMaximumSize(new Dimension(300, 40));

        JLabel lastNameLabel = new JLabel("Last Name: ");

        JTextField lastNameTF = new JTextField();
        lastNameTF.setColumns(15);


        JPanel emailPanel = new JPanel();
        emailPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        emailPanel.setBackground(Color.yellow);
        emailPanel.setMaximumSize(new Dimension(300, 40));

        JLabel emailLabel = new JLabel("Email:         ");

        JTextField emailTF = new JTextField();
        emailTF.setColumns(15);


        JPanel passwordPanel = new JPanel();
        passwordPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        passwordPanel.setBackground(Color.yellow);
        passwordPanel.setMaximumSize(new Dimension(300, 40));

        JLabel passwordLabel = new JLabel("Password:   ");

        JPasswordField passwordTF = new JPasswordField();
        passwordTF.setColumns(15);


        JPanel genderPanel = new JPanel();
        genderPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
//        genderPanel.setBackground(Color.yellow);
        genderPanel.setMaximumSize(new Dimension(300, 40));

        ButtonGroup genderGroup = new ButtonGroup( );
        JRadioButton maleRadio = new JRadioButton("Male");
        JRadioButton femaleRadio = new JRadioButton("Female");
        genderGroup.add(maleRadio);
        genderGroup.add(femaleRadio);


        JPanel rolePanel = new JPanel();
        rolePanel.setLayout(new FlowLayout(FlowLayout.CENTER));
//        rolePanel.setBackground(Color.yellow);
        rolePanel.setMaximumSize(new Dimension(600, 40));

        ButtonGroup roleGroup = new ButtonGroup( );
        JRadioButton adminRadio = new JRadioButton("Administrator");
        JRadioButton studentRadio = new JRadioButton("Student");
        JRadioButton professorRadio = new JRadioButton("Professor");
        roleGroup.add(adminRadio);
        roleGroup.add(studentRadio);
        roleGroup.add(professorRadio);


        JPanel insertButtonPanel = new JPanel();
        insertButtonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
//        insertButtonPanel.setBackground(Color.yellow);
        insertButtonPanel.setMaximumSize(new Dimension(300, 40));

        JButton clearButton = new JButton("Clear");
        JButton insertButton = new JButton("Insert");

        insertButton.addActionListener(actionEvent -> {
            String genderText = "2";
            if(maleRadio.isSelected()){
                genderText = "0";
            }
            if(femaleRadio.isSelected()){
                genderText = "1";
            }
            String roleText = "3";
            if(adminRadio.isSelected()){
                roleText = "0";
            }
            if(professorRadio.isSelected()){
                roleText = "1";
            }
            if(studentRadio.isSelected()){
                roleText = "2";
            }
            if (firstNameTF.getText().isEmpty() || firstNameTF.getText().length() == 0 || firstNameTF.getText().equals("") || firstNameTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter first name!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (lastNameTF.getText().isEmpty() || lastNameTF.getText().length() == 0 || lastNameTF.getText().equals("") || lastNameTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter last number!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (emailTF.getText().isEmpty() || emailTF.getText().length() == 0 || emailTF.getText().equals("") || emailTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter email!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (passwordTF.getText().isEmpty() || passwordTF.getText().length() == 0 || passwordTF.getText().equals("") || passwordTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter password!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (genderText.equals("2")) {
                JOptionPane.showMessageDialog(panel, "Please choose a gender!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (roleText.equals("3")) {
                JOptionPane.showMessageDialog(panel, "Please choose a role!", "Error", JOptionPane.ERROR_MESSAGE);
            } else {
                connection.sendMessage("insert person " + firstNameTF.getText() + " " + lastNameTF.getText() + " " + emailTF.getText() + " " + passwordTF.getText() + " " + genderText + " " + roleText);
                Connection.setListener(message -> {
                    try {
                        if(message.contains("success")){
                            JOptionPane.showMessageDialog(panel, "Successfully insert a new person!", "Success", JOptionPane.INFORMATION_MESSAGE);
                            firstNameTF.setText("");
                            lastNameTF.setText("");
                            emailTF.setText("");
                            passwordTF.setText("");
                            roleGroup.clearSelection();
                            genderGroup.clearSelection();
                        }
                        if(message.contains("error")){
                            JOptionPane.showMessageDialog(panel, "Error encountered, please double check your entry for duplication!", "Error", JOptionPane.ERROR_MESSAGE);
                        }
                    } catch (Exception e) {
                        //Eat comment
                    }
                });
            }
        });

        clearButton.addActionListener(actionEvent -> {
            firstNameTF.setText("");
            lastNameTF.setText("");
            emailTF.setText("");
            passwordTF.setText("");
            roleGroup.clearSelection();
            genderGroup.clearSelection();
        });


        add(panel, new GridBagConstraints());
        panel.add(titlePanel);
        titlePanel.add(titleLabel);
        panel.add(firstNamePanel);
        firstNamePanel.add(firstNameLabel);
        firstNamePanel.add(firstNameTF);
        panel.add(lastNamePanel);
        lastNamePanel.add(lastNameLabel);
        lastNamePanel.add(lastNameTF);
        panel.add(emailPanel);
        emailPanel.add(emailLabel);
        emailPanel.add(emailTF);
        panel.add(passwordPanel);
        passwordPanel.add(passwordLabel);
        passwordPanel.add(passwordTF);
        panel.add(genderPanel);
        genderPanel.add(maleRadio);
        genderPanel.add(femaleRadio);
        panel.add(rolePanel);
        rolePanel.add(adminRadio);
        rolePanel.add(professorRadio);
        rolePanel.add(studentRadio);
        panel.add(insertButtonPanel);
        insertButtonPanel.add(clearButton);
        insertButtonPanel.add(insertButton);

//        Connection.setListener(tp::setText);
    }
}