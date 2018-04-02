import com.google.gson.Gson;
import model.ListFiles;
import model.Status;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.List;

/**
 * Created by Bing on 4/18/17.
 */

//TODO: when double click text , it didnt display message
//TODO: after uploading the file, it didnt refresh (I thought you finished it)
public class FileSharingPanel extends Panel {
    public JPanel mPanel;
    private JList mList;
    private DefaultListModel mModel;
    private int mCourseId = -1;
    private List<String> mDataList;

    private String profName = "1";
    private String mCourseName;

    public FileSharingPanel(int id, String courseName) {
        mPanel = new JPanel();
        mCourseId = id;
        mCourseName = courseName;
        init();

        retrieveDataList();
    }

    private void updateDataSetChange() {
        if (mDataList == null) return;
        mModel.removeAllElements();
        if (mDataList != null) {
            for (String m : mDataList) {
                mModel.addElement(m);
            }
        }
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
                        String filename = (String) theList.getModel().getElementAt(index);
                        new Thread(() ->
                                connection.sendMessage("download " + filename + " " + mCourseId)).start();
                    }
                }
            }
        });

        return list;
    }

    void init() {
        if (mPanel == null) return;
        remove(mPanel);
        mPanel.removeAll();

        JPanel titlePanel = new JPanel();

//        JLabel titleL = new JLabel();
        titlePanel.setMaximumSize(new Dimension(400, 40));
//        titleL.setText("Course: " + mCourseName);

        JPanel professorPanel = new JPanel();

        JLabel professorL = new JLabel();
        professorL.setText("Professor: " + profName);
        professorPanel.setMaximumSize(new Dimension(400, 40));

        mPanel.setBounds(0, GUI.NAV_HEIGHT + 50, GUI.WIDTH, (GUI.HEIGHT - GUI.NAV_HEIGHT));
        mPanel.setLayout(new BoxLayout(mPanel, BoxLayout.Y_AXIS));

        mList = buildList();
        System.out.println(mList.getModel());

        JScrollPane listScrollPane = new JScrollPane(mList);
        listScrollPane.setMaximumSize(new Dimension(400, 300));

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        buttonPanel.setMaximumSize(new Dimension(300, 40));

        JButton uploadBtn = new JButton("Upload");

        uploadBtn.addActionListener(actionEvent -> {
                    JFileChooser chooser = new JFileChooser("../filestores");
                    FileNameExtensionFilter filter = new FileNameExtensionFilter(
                            "Text file", "txt");
                    chooser.setFileFilter(filter);
                    int returnVal = chooser.showOpenDialog(mPanel);
                    if (returnVal == JFileChooser.APPROVE_OPTION) {
                        System.out.println("You chose to open this file: " +
                                chooser.getSelectedFile().getName());
                        new Thread(() ->
                                connection.sendMessage("upload " + chooser.getSelectedFile().getName() + " " + mCourseId)
                        ).start();
                    }
                }
        );


        add(mPanel);
        mPanel.add(titlePanel);
//        titlePanel.add(titleL);
        mPanel.add(listScrollPane);
        mPanel.add(buttonPanel);
        buttonPanel.add(uploadBtn);

        switch (GlobalData.getInstance().getLoginPerson().getRole()) {
            case "student":
                buttonPanel.setVisible(false);
                break;
            default:
        }

    }

    public void retrieveDataList() {
        Gson gson = new Gson();

        Connection.setListener(message -> {
            try {
                ListFiles model = gson.fromJson(message, ListFiles.class);
                mDataList = model.result;
                updateDataSetChange();
            } catch (Exception e) {
                //Eat comment
                Status result = gson.fromJson(message, Status.class);

                if (result.result.status.equals("success_download")) {
                    if (result.result.filename.contains("txt")) {
                        GUI.switchTo(new TextPanel(result.result.filename), true);
                    }
                } else if (result.result.status.equals("success_upload")) {
                    new Thread(() -> {
                        connection.sendMessage("select filestore courseId is " + mCourseId);
                    }).start();
                }
            }
        });

        // get list of courses belong to a person
        new Thread(() -> {
            connection.sendMessage("select filestore courseId is " + mCourseId);
        }).start();
    }

}