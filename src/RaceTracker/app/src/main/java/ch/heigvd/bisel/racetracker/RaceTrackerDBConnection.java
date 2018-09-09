package ch.heigvd.bisel.racetracker;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

public class RaceTrackerDBConnection {
    static final String port = "5432";
    static final String databaseName = "race_tracker_db";
    private String connection;
    private String user;
    private String pwd;

    /**
     * Create a new connection string to the DB
     * @param server Server address
     * @param user User to the DB
     * @param pwd Password to the DB
     */
    public RaceTrackerDBConnection(String server, String user, String pwd) {
        initConnection(server, user, pwd);
    }

    /**
     * Create a new connection using the preferences
     * @param context
     * @param serverKey
     * @param userKey
     * @param pwdKey
     */
    public RaceTrackerDBConnection(Context context, String serverKey, String userKey, String pwdKey) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        initConnection(prefs.getString(serverKey, ""), prefs.getString(userKey, ""), prefs.getString(pwdKey, ""));
    }

    /**
     * Initialize the connection string
     * @param server Server address
     * @param user User to the DB
     * @param pwd Password to the DB
     */
    public void initConnection(String server, String user, String pwd) {
        connection = "jdbc:postgresql://" + server + ":" + port + "/" + databaseName;
        this.user = user;
        this.pwd = pwd;
    }

    /**
     * Returns the connection string
     * @return Connection string
     */
    public String getConnection() {
        return connection;
    }

    /**
     * Returns the user name
      * @return User to the DB
     */
    public String getUser() {
        return user;
    }

    /**
     * Set sthe user name
     * @param user User to the DB
     */
    public void setUser(String user) {
        this.user = user;
    }

    /**
     * Returns the password
     * @return Password to the DB
     */
    public String getPwd() {
        return pwd;
    }

    /**
     * Sets the password
     * @param pwd Password to the DB
     */
    public void setPwd(String pwd) {
        this.pwd = pwd;
    }
}
