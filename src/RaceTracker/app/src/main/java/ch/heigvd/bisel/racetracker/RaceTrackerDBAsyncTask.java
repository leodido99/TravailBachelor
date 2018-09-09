package ch.heigvd.bisel.racetracker;

import android.os.AsyncTask;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

/* TODO Clean-up */
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

        for(RaceTrackerQuery result : results) {
            result.getCallbackExecuted().onQueryExecuted(result);
        }


        /*try {*/
            /* Execu*te all the callbacks */
            /*for(RaceTrackerQuery result : results) {
                result.getCallbackResultReady().onQueryResultReady(result);
                result.close();
            }*/
        /*} catch (SQLException e) {
            e.printStackTrace();
        }*/
    }

    /**
     * Executes the query
     */
    @Override
    public ArrayList<RaceTrackerQuery> doInBackground(RaceTrackerQuery... queries) {
        //Statement st;
        results = new ArrayList<>();

        //try {
            for(int i = 0; i < queries.length; i++) {
                try (Connection conn = DriverManager.getConnection(connection, user, password);
                     Statement st = conn.createStatement();
                     ResultSet rs = st.executeQuery(queries[i].getQuery())) {
                    queries[i].setResult(rs);
                    queries[i].getCallbackResultReady().onQueryResultReady(queries[i]);
                    results.add(queries[i]);
                } catch (SQLException e) {
                    e.printStackTrace();
                }

                /*queries[i].setConnection(conn);
                st = queries[i].getConnection().createStatement();
                queries[i].setResult(st.executeQuery(queries[i].getQuery()));
                results.add(queries[i]);*/
            }
        /*} catch (SQLException e) {
            System.out.println("DBG: " + e.getMessage());
            for(int i = 0; i < queries.length; i++) {
                queries[i].setException(e);
                results.add(queries[i]);
            }
        }*/
        return results;
    }
}
