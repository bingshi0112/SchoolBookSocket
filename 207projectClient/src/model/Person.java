package model;

/**
 * Created by lam on 4/16/17.
 */
public class Person extends Model {
    public int id;
    public String first_name;
    public String last_name;
    public String email;
    public int gender;
    public int role;
    String roleName;

    public String getRole() {
        if (role == 0) {
            roleName = "administrator";
        }
        if (role == 1) {
            roleName = "professor";
        }
        if (role == 2) {
            roleName = "student";
        }
        return roleName;
    }

    @Override
    public String toString() {
        return first_name + " " + last_name;
    }

    public String WelcomeString() {
        return "Welcome back, " + getRole() + " " + first_name + " " + last_name + "!";
    }

    public String naviString() {
        return  getRole() + " " + first_name + " " + last_name;
    }
}
