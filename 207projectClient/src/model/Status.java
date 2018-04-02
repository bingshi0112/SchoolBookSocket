package model;


/**
 * Created by lam on 4/19/17.
 */
public class Status extends Model {
    public FileStatus result;

    public static class FileStatus {
        public String status;
        public String filename;

        @Override
        public String toString() {
            return filename;
        }
    }
}
