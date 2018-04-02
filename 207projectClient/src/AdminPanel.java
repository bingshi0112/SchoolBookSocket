import javax.swing.*;
import java.awt.*;

/**
 * Created by Bing on 4/18/17.
 */
public class AdminPanel extends Panel {
    public AdminPanel() {
        init();
    }

    void init() {
        Connection.removeListener();
        setLayout(new GridBagLayout());
        setVisible(true);

        JPanel panel = new JPanel();
        panel.setBounds(0, GUI.NAV_HEIGHT, GUI.WIDTH, 200);
        panel.setLayout(new GridLayout(5, 1, 0, 4));

        JPanel allStudentsPanel = new JPanel();
        JButton allStudentsButton = new JButton(" All Students  ");

        JPanel allProfessorsPanel = new JPanel();
        JButton allProfessorsButton = new JButton("All Professors");

        JPanel allCoursesPanel = new JPanel();
        JButton allCoursesButton = new JButton("  All Courses  ");

        JPanel addProfessorPanel = new JPanel();
        JButton addProfessorButton = new JButton("  Add Person  ");

        JPanel addCoursesPanel = new JPanel();
        JButton addCourseButton = new JButton("  Add Course  ");

        allStudentsButton.addActionListener(actionEvent -> {
            GUI.switchTo(new MainPanel(MainPanel.LIST_TYPE.PERSON, 2, -1), true);
        });
        allProfessorsButton.addActionListener(actionEvent -> {
            GUI.switchTo(new MainPanel(MainPanel.LIST_TYPE.PERSON, 1, -1), true);
        });

        allCoursesButton.addActionListener(actionEvent -> {
            GUI.switchTo(new MainPanel(MainPanel.LIST_TYPE.COURSE, -1), true);
        });

        addProfessorButton.addActionListener(actionEvent -> {
            GUI.switchTo(new InsertPersonPanel(), true);
        });

        addCourseButton.addActionListener(actionEvent -> {
            GUI.switchTo(new InsertCoursePanel(), true);
        });

        add(panel, new GridBagConstraints());
        panel.add(allStudentsPanel);
        allStudentsPanel.add(allStudentsButton);
        panel.add(allProfessorsPanel);
        allProfessorsPanel.add(allProfessorsButton);
        panel.add(allCoursesPanel);
        allCoursesPanel.add(allCoursesButton);
        panel.add(addProfessorPanel);
        addProfessorPanel.add(addProfessorButton);
        panel.add(addCoursesPanel);
        addCoursesPanel.add(addCourseButton);
    }
}