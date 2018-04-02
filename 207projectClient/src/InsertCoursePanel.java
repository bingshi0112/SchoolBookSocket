import javax.swing.*;
import java.awt.*;

/**
 * Created by Bing on 4/18/17.
 */
public class InsertCoursePanel extends Panel {
    public InsertCoursePanel() {
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
        JLabel titleLabel = new JLabel("Insert Course");
        titleLabel.setFont(new Font("Dialog", Font.BOLD, 18));


        JPanel departmentPanel = new JPanel();
        departmentPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        departmentPanel.setBackground(Color.yellow);
        departmentPanel.setMaximumSize(new Dimension(300, 40));

        JLabel departmentLabel = new JLabel("Department:  ");

        JTextField departmentTF = new JTextField();
        departmentTF.setColumns(15);


        JPanel courseNumberPanel = new JPanel();
        courseNumberPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        courseNumberPanel.setBackground(Color.yellow);
        courseNumberPanel.setMaximumSize(new Dimension(300, 40));

        JLabel courseNumberLabel = new JLabel("Course No.:   ");

        JTextField courseNumberTF = new JTextField();
        courseNumberTF.setColumns(15);


        JPanel semesterPanel = new JPanel();
        semesterPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        semesterPanel.setBackground(Color.yellow);
        semesterPanel.setMaximumSize(new Dimension(300, 40));

        JLabel semesterLabel = new JLabel("Semester:      ");

        JTextField semesterTF = new JTextField();
        semesterTF.setColumns(15);


        JPanel yearPanel = new JPanel();
        yearPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        yearPanel.setBackground(Color.yellow);
        yearPanel.setMaximumSize(new Dimension(300, 40));

        JLabel yearLabel = new JLabel("Year:              ");

        JTextField yearTF = new JTextField();
        yearTF.setColumns(15);


        JPanel courseNamePanel = new JPanel();
        courseNamePanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        courseNamePanel.setBackground(Color.yellow);
        courseNamePanel.setMaximumSize(new Dimension(300, 40));

        JLabel courseNameLabel = new JLabel("Course Name:");

        JTextField courseNameTF = new JTextField();
        courseNameTF.setColumns(15);


        JPanel sectionNumberPanel = new JPanel();
        sectionNumberPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
//        sectionNumberPanel.setBackground(Color.yellow);
        sectionNumberPanel.setMaximumSize(new Dimension(300, 40));

        JLabel sectionNumberLabel = new JLabel("Section No.:   ");

        JTextField sectionNumberTF = new JTextField();
        sectionNumberTF.setColumns(15);


        JPanel insertButtonPanel = new JPanel();
        insertButtonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
//        insertButtonPanel.setBackground(Color.yellow);
        insertButtonPanel.setMaximumSize(new Dimension(300, 40));

        JButton clearButton = new JButton("Clear");
        JButton insertButton = new JButton("Insert");

        insertButton.addActionListener(actionEvent -> {
            if (departmentTF.getText().isEmpty() || departmentTF.getText().length() == 0 || departmentTF.getText().equals("") || departmentTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter department name!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (courseNumberTF.getText().isEmpty() || courseNumberTF.getText().length() == 0 || courseNumberTF.getText().equals("") || courseNumberTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter course number!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (semesterTF.getText().isEmpty() || semesterTF.getText().length() == 0 || semesterTF.getText().equals("") || semesterTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter semester!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (yearTF.getText().isEmpty() || yearTF.getText().length() == 0 || yearTF.getText().equals("") || yearTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter year!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (courseNameTF.getText().isEmpty() || courseNameTF.getText().length() == 0 || courseNameTF.getText().equals("") || courseNameTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter course name!", "Error", JOptionPane.ERROR_MESSAGE);
            } else if (sectionNumberTF.getText().isEmpty() || sectionNumberTF.getText().length() == 0 || sectionNumberTF.getText().equals("") || sectionNumberTF.getText() == null) {
                JOptionPane.showMessageDialog(panel, "Please enter section number!", "Error", JOptionPane.ERROR_MESSAGE);
            } else {
                connection.sendMessage("insert course " + departmentTF.getText() + " " + courseNumberTF.getText() + " " + semesterTF.getText() + " " + yearTF.getText() + " " + courseNameTF.getText() + " " + sectionNumberTF.getText());
                Connection.setListener(message -> {
                    try {
                        if(message.contains("success")){
                            JOptionPane.showMessageDialog(panel, "Successfully insert a new course!", "Success", JOptionPane.INFORMATION_MESSAGE);
                            departmentTF.setText("");
                            courseNumberTF.setText("");
                            semesterTF.setText("");
                            yearTF.setText("");
                            courseNameTF.setText("");
                            sectionNumberTF.setText("");
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
            departmentTF.setText("");
            courseNumberTF.setText("");
            semesterTF.setText("");
            yearTF.setText("");
            courseNameTF.setText("");
            sectionNumberTF.setText("");
        });

        add(panel, new GridBagConstraints());
        panel.add(titlePanel);
        titlePanel.add(titleLabel);
        panel.add(departmentPanel);
        departmentPanel.add(departmentLabel);
        departmentPanel.add(departmentTF);
        panel.add(courseNumberPanel);
        courseNumberPanel.add(courseNumberLabel);
        courseNumberPanel.add(courseNumberTF);
        panel.add(semesterPanel);
        semesterPanel.add(semesterLabel);
        semesterPanel.add(semesterTF);
        panel.add(yearPanel);
        yearPanel.add(yearLabel);
        yearPanel.add(yearTF);
        panel.add(courseNamePanel);
        courseNamePanel.add(courseNameLabel);
        courseNamePanel.add(courseNameTF);
        panel.add(sectionNumberPanel);
        sectionNumberPanel.add(sectionNumberLabel);
        sectionNumberPanel.add(sectionNumberTF);
        panel.add(insertButtonPanel);
        insertButtonPanel.add(clearButton);
        insertButtonPanel.add(insertButton);

//        Connection.setListener(tp::setText);
    }
}