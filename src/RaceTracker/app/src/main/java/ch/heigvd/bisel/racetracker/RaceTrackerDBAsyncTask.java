package ch.heigvd.bisel.racetracker;

import android.os.AsyncTask;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

public class RaceTrackerDBAsyncTask extends AsyncTask<RaceTrackerQuery, Void, ArrayList<RaceTrackerQuery>> {
    private String connection;
    private String user;
    private String password;
    private ArrayList<RaceTrackerQuery> results;

    public RaceTrackerDBAsyncTask(String connection, String user, String password) {
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
        try {
            /* Execute all the callbacks */
            for(RaceTrackerQuery result : results) {
                result.getCallback().onQueryResultReady(result);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    /**
     * Executes the query
     */
    @Override
    public ArrayList<RaceTrackerQuery> doInBackground(RaceTrackerQuery... queries) {
        Statement st;
        results = new ArrayList<>();

        try {
            Connection conn = DriverManager.getConnection(connection,
                    user, password);

            for(int i = 0; i < queries.length; i++) {
                st = conn.createStatement();
                queries[i].setResult(st.executeQuery(queries[i].getQuery()));
                results.add(queries[i]);
            }
        } catch (SQLException e) {
            System.out.println("DBG: " + e.getMessage());
            for(int i = 0; i < queries.length; i++) {
                queries[i].setException(e);
                results.add(queries[i]);
            }
        }
        return results;
    }
}
