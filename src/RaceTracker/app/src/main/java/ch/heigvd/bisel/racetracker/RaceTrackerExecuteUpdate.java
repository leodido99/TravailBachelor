package ch.heigvd.bisel.racetracker;

import android.os.AsyncTask;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.ArrayList;

public class RaceTrackerExecuteUpdate extends AsyncTask<RaceTrackerQuery, Void, ArrayList<RaceTrackerQuery>> {
    private String connection;
    private String user;
    private String password;
    private ArrayList<RaceTrackerQuery> results;

    /**
     * Create a new instance
     * @param connection Connection to the DB
     * @param user User of the DB
     * @param password Pwd of the DB
     */
    public RaceTrackerExecuteUpdate(String connection, String user, String password) {
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
                 PreparedStatement st = conn.prepareStatement(queries[i].getQuery())) {
                queries[i].setNbUpdated(st.executeUpdate());
            } catch (SQLException e) {
                e.printStackTrace();
                queries[i].setException(e);
            }

            results.add(queries[i]);
        }

        return results;
    }
}
