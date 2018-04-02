import com.google.gson.Gson;
import model.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by lam on 4/8/17.
 */

//TODO: didnt display all students (i checked in my data and schema)
public class MainPanel extends Panel {
    private final JPanel mPanel;
    private JList mList;
    private DefaultListModel mModel;

    private LIST_TYPE mType;
    //    private String mTitle;
    private List<Model> mDataList;
    // user id or course id  or -1 if all
    private String mEmail;
    private int mCourseId = -1;
    private int mRole = -1;

    private String profName = "1";
    private String mCourseName = "1";
    private int professorCount = 0;
    private Person professor;

    public MainPanel(LIST_TYPE type, String email) {
        mPanel = new JPanel();
        mType = type;
        mEmail = email;
        init();

        retrieveDataList();
    }

    public MainPanel(LIST_TYPE type, int id) {
        mPanel = new JPanel();
        mType = type;
        mCourseId = id;
        init();

        retrieveDataList();
    }

    public MainPanel(LIST_TYPE type, int role, int id) {
        mPanel = new JPanel();
        mType = type;
        mRole = role;
        init();

        retrieveDataList();
    }

    private List<Model> convertPersonToModel(List<Person> people) {
        List<Model> list = new ArrayList<>();
        for (Person p : people) {
            if (p.role == 2) {
                list.add(p);
            }
            professorCount =0;
            if (p.role == 1) {
                professorCount++;
                profName = p.first_name + p.last_name;
                professor = p;
                list.add(p);
            }
        }
        if (professorCount > 1) {
            profName = "0";
            list.remove(professor);
        }
        return list;
    }

    private List<Model> convertCourseToModel(List<Course> courses) {
        List<Model> list = new ArrayList<>();
        for (Course course : courses) {
            list.add(course);
        }
        return list;
    }

    public void retrieveDataList() {
        if (mType == null) return;
        Gson gson = new Gson();

        switch (mType) {
            case PERSON:
// get list of student base on course id
                Connection.setListener(message -> {
                    try {
                        ListPerson model1 = gson.fromJson(message, ListPerson.class);
                        mDataList = convertPersonToModel(model1.result);
                        updateDataSetChange();
                    } catch (Exception e) {
                    }
                });

                new Thread(() -> {
                    System.out.println(mCourseId);
                    if (mCourseId != -1) {
                        connection.sendMessage("get person courseId is " + mCourseId);
                    } else if (mRole != -1) {
                        connection.sendMessage("select person role is " + mRole);
                        System.out.println("select person role is " + mRole);
                    }
                }).start();
                break;
            case COURSE:
                Connection.setListener(message -> {
                    try {
                        ListCourse model = gson.fromJson(message, ListCourse.class);
                        mDataList = convertCourseToModel(model.result);

                        updateDataSetChange();
                    } catch (Exception e) {
                        //Eat comment
                    }
                });

                // get list of courses belong to a person
                new Thread(() -> {
                    if (mEmail != null)
                        connection.sendMessage("get course email is " + mEmail + " current");
                    else
                        connection.sendMessage("select course current");
                }).start();
                break;
            default:
        }
    }

    public List<Model> getDataList() {
        return mDataList;
    }

    private JList buildList() {
        mModel = new DefaultListModel();
        JList list = new JList(mModel);
        list.setCellRenderer(new CustomCellenderer());

        list.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent mouseEvent) {
                JList theList = (JList) mouseEvent.getSource();
                if (mouseEvent.getClickCount() == 2) {
                    int index = theList.locationToIndex(mouseEvent.getPoint());
                    if (index >= 0) {
                        if (mType.equals(LIST_TYPE.COURSE)) {
                            Course course = (Course) theList.getModel().getElementAt(index);
                            mCourseName = course.department + " " + course.number + "-" + course.name;
                            GUI.switchTo(new MainPanel(LIST_TYPE.PERSON, course.id), true);
                        }
                    }
                }
            }
        });

        updateDataSetChange();
        return list;
    }

    private void updateDataSetChange() {
        if (mDataList == null) return;
        mModel.removeAllElements();
        if (mDataList != null) {
            for (Model m : mDataList) {
                mModel.addElement(m);
            }
        }
    }

    void init() {
        if (mPanel == null) return;
        remove(mPanel);
        mPanel.removeAll();

        JPanel titlePanel = new JPanel();

        JLabel titleL = new JLabel();
        titlePanel.setMaximumSize(new Dimension(400, 40));

        JPanel professorPanel = new JPanel();

        JLabel professorL = new JLabel();
        professorL.setText("Professor: " + profName);
        professorPanel.setMaximumSize(new Dimension(400, 40));

        JPanel refreshPanel = new JPanel();

        JButton refreshBtn = new JButton("Refresh");
        refreshPanel.setMaximumSize(new Dimension(400, 40));
        refreshBtn.addActionListener(actionEvent -> {
            retrieveDataList();
        });

        switch (mType) {
            case PERSON:
                titleL.setText("Course: " + mCourseName);
                break;
            case COURSE:
                if (GUI.getStackSize() == 0) {
                    titleL.setText("Current Semester Courses:");
                }
                if (GUI.getStackSize() == 1) {
                    titleL.setText("All courses:");
                }
                professorPanel.setVisible(false);
                break;
            default:
                throw new RuntimeException("Not correct type");
        }

        if (profName.equals("1") || profName.equals("0")) {
            professorPanel.setVisible(false);
        }
        if (mCourseName.equals("1")) {
            titlePanel.setVisible(false);
        }
        mPanel.setBounds(0, GUI.NAV_HEIGHT + 50, GUI.WIDTH, (GUI.HEIGHT - GUI.NAV_HEIGHT));
        mPanel.setLayout(new BoxLayout(mPanel, BoxLayout.Y_AXIS));

        mList = buildList();
        System.out.println(mList.getModel());

        JScrollPane listScrollPane = new JScrollPane(mList);
        listScrollPane.setMaximumSize(new Dimension(400, 300));

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        buttonPanel.setMaximumSize(new Dimension(300, 40));

        JButton addBtn = new JButton("         Add          ");
        JButton removeBtn = new JButton("      Remove       ");

        JPanel enrollButtonPanel = new JPanel();
        enrollButtonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        enrollButtonPanel.setMaximumSize(new Dimension(300, 40));

        JButton enrollBtn = new JButton("        Enroll        ");
        JButton dropBtn = new JButton("        Drop         ");

        JPanel fileButtonPanel = new JPanel();
        fileButtonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        fileButtonPanel.setMaximumSize(new Dimension(300, 40));

        JButton anouncementBtn = new JButton("Announcements");
        JButton fileBtn = new JButton("          File         ");

        addBtn.addActionListener(actionEvent -> {
            switch (mType) {
                case PERSON:
                    GUI.switchTo(new InsertPersonPanel(), true);
                    break;
                case COURSE:
                    GUI.switchTo(new InsertCoursePanel(), true);
                    break;
                default:
                    throw new RuntimeException("Not correct type");
            }
        });

        removeBtn.addActionListener(actionEvent -> {
            //TODO
            int[] indices = mList.getSelectedIndices();
            for (int i : indices) {
                System.out.println("Remove " + i);

                switch (mType) {
                    case PERSON:
                        connection.sendMessage("delete person id is " + i);
                        break;
                    case COURSE:
                        connection.sendMessage("delete course id is " + i);
                        break;
                    default:
                        throw new RuntimeException("Not correct type");
                }
            }
        });

        enrollBtn.addActionListener(actionEvent -> {
            GUI.switchTo(new EnrollPanel(mCourseId), true);
        });

        dropBtn.addActionListener(actionEvent -> {
            //TODO
            int[] indices = mList.getSelectedIndices();
            for (int i : indices) {
                System.out.println("Remove " + i);

                switch (mType) {
                    case PERSON:
                        connection.sendMessage("delete person id is " + i);
                        break;
                    case COURSE:
                        connection.sendMessage("delete course id is " + i);
                        break;
                    default:
                        throw new RuntimeException("Not correct type");
                }
            }
        });

        add(mPanel);
        mPanel.add(titlePanel);
        titlePanel.add(titleL);
        mPanel.add(professorPanel);
        professorPanel.add(professorL);
        mPanel.add(refreshPanel);
        refreshPanel.add(refreshBtn);
        mPanel.add(listScrollPane);
        mPanel.add(buttonPanel);
        buttonPanel.add(addBtn);
        buttonPanel.add(removeBtn);
        mPanel.add(enrollButtonPanel);
        enrollButtonPanel.add(enrollBtn);
        enrollButtonPanel.add(dropBtn);
        enrollButtonPanel.setVisible(false);
        mPanel.add(fileButtonPanel);
//        fileButtonPanel.add(anouncementBtn);
        fileButtonPanel.add(fileBtn);
        fileButtonPanel.setVisible(false);
        fileBtn.addActionListener(actionEvent -> {
            GUI.switchTo(new FileSharingPanel(mCourseId, mCourseName), true);
        });

        if (LIST_TYPE.PERSON.equals(mType) && mCourseId > 0) {
            fileButtonPanel.setVisible(true);
            buttonPanel.setVisible(false);
            enrollButtonPanel.setVisible(true);
        }

        switch (GlobalData.getInstance().getLoginPerson().getRole()) {
            case "professor":
                enrollBtn.setVisible(false);
                buttonPanel.setVisible(false);
                break;
            case "student":
                enrollButtonPanel.setVisible(false);
                buttonPanel.setVisible(false);
                break;
            default:
        }
    }

    public enum LIST_TYPE {PERSON, COURSE}
}