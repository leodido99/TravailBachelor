package ch.heigvd.bisel.racetracker;

import android.os.AsyncTask;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

public class RaceTrackerExecuteQuery extends AsyncTask<RaceTrackerQuery, Void, ArrayList<RaceTrackerQuery>> {
    private String connection;
    private String user;
    private String password;
    private ArrayList<RaceTrackerQuery> results;

    /**
     * Create a new instance to execute a query
     * @param connection Connection to the DB
     * @param user User of the DB
     * @param password Pwd of the DB
     */
    public RaceTrackerExecuteQuery(String connection, String user, String password) {
        this.connection = connection;
        this.user = user;
        this.password = password;
    }

    /**
     * Called at the end of the query execution
     */
    @Override
    protected void onPostExecute(ArrayList<RaceTrackerQuery> results) {
        super.onPostExecute(results);

        for(RaceTrackerQuery result : results) {
            if (result.getCallbackExecuted() != null) {
                result.getCallbackExecuted().onQueryExecuted(result);
            }
        }

    }

    /**
     * Executes the query
     */
    @Override
    public ArrayList<RaceTrackerQuery> doInBackground(RaceTrackerQuery... queries) {
        results = new ArrayList<>();

            for(int i = 0; i < queries.length; i++) {
                try (Connection conn = DriverManager.getConnection(connection, user, password);
                     Statement st = conn.createStatement();
                     ResultSet rs = st.executeQuery(queries[i].getQuery())) {
                    queries[i].setResult(rs);
                    if (queries[i].getCallbackResultReady() != null) {
                        queries[i].getCallbackResultReady().onQueryResultReady(queries[i]);
                    }
                    results.add(queries[i]);
                } catch (SQLException e) {
                    e.printStackTrace();
                    queries[i].setException(e);
                }
            }

        return results;
    }
}
