package model;

/**
 * Created by lam on 4/19/17.
 */
public class Course extends Model {
    public int id;
    public String department;
    public String number;
    public String semester;
    public String year;
    public String name;
    public String section;

    @Override
    public String toString() {
        return department + " " + number + " " + name;
    }
}
