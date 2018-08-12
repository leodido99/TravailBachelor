package ch.heigvd.bisel.racetracker;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.preference.PreferenceManager;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class RaceTrackerDB extends AsyncTask<String, Void, ResultSet> {
    static final String jdbc = "jdbc:postgresql://";
    static final String databaseName = "race_tracker_db";
    static final String databasePort = "5432";
    static final String dbUser = "pi";
    static final String dbPwd = "heig";
    String connectionString;
    RaceTrackerDBGetResult callback;

    public RaceTrackerDB(Context context) {
        super();
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(context);
        connectionString = jdbc + sharedPref.getString("server_address", "") + ":"
                + databasePort + "/" + databaseName;
        System.out.println("DBG: DB Connection string: " + connectionString);
    }

    public void getCompetitions(RaceTrackerDBGetResult callback) {
        execute("SELECT * FROM race_tracker.competition");
        this.callback = callback;
    }

    @Override
    protected void onPostExecute(ResultSet result) {
        super.onPostExecute(result);
        System.out.println("Query executed: ");
        try {
            callback.getResult(result);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    @Override
    public ResultSet doInBackground(String... queries) {
        Statement st;

        try {
            Connection conn = DriverManager.getConnection(connectionString, dbUser, dbPwd);

            System.out.println("DBG: Execute: " + queries[0]);
            st = conn.createStatement();
            ResultSet results = st.executeQuery(queries[0]);
            return results;
        } catch (SQLException e) {
            System.out.println("DBG: " + e.getMessage());
            return null;
        }
    }
}
