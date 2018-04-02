import model.Person;

/**
 * Created by lam on 4/20/17.
 */
public class GlobalData {
    private static GlobalData ourInstance = new GlobalData();
    private Person mLoginPerson;

    private GlobalData() {
    }

    public static GlobalData getInstance() {
        return ourInstance;
    }

    public Person getLoginPerson() {
        return mLoginPerson;
    }

    public void setLoginPerson(Person person) {
        if (person != null)
            mLoginPerson = person;
    }
}