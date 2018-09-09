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
import java.text.MessageFormat;

public class RaceTrackerDB {
    static final String jdbc = "jdbc:postgresql://";
    static final String databaseName = "race_tracker_db";
    static final String databasePort = "5432";
    static final String dbUser = "pi";
    static final String dbPwd = "heig";
    String connectionString;

    public RaceTrackerDB(Context context) {
        super();
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(context);
        connectionString = jdbc + sharedPref.getString("server_address", "") + ":"
                + databasePort + "/" + databaseName;
        System.out.println("DBG: DB Connection string: " + connectionString);
    }

    /**
     * Get all the competitions
     * @param callback Callback called when results are ready
     */
    public void getCompetitions(OnQueryResultReady callback) {
        RaceTrackerQuery query = new RaceTrackerQuery();
        query.setCallback(callback);
        query.setQuery("SELECT competition_id, name, ST_X(location) as lat, ST_Y(location) as lon, event_date, active, zoom " +
                "FROM race_tracker.competition;");
        RaceTrackerDBAsyncTask task = new RaceTrackerDBAsyncTask(connectionString, dbUser, dbPwd);
        task.execute(query);
    }

    /**
     * Get the last data point for each competitor
     * @param callback Callback called when results are ready
     * @param competition_id Competition
     */
    public void getLastDataPoint(OnQueryResultReady callback, int competition_id) {
        RaceTrackerQuery query = new RaceTrackerQuery();
        query.setCallback(callback);
        query.setQuery(MessageFormat.format("SELECT data_point_id, competitor_id, " +
                "competition_id, sequence, time_stamp, ST_X(position) as lat, ST_Y(position) as lon" +
                ", heart_rate, cadence, nb_satellites, position_dop, status FROM (SELECT rank() " +
                "OVER (PARTITION BY data_point.competitor_id ORDER BY sequence DESC) AS therank, * " +
                "FROM race_tracker.data_point WHERE data_point.competition_id = {0}) t " +
                "WHERE therank = 1;", competition_id));
        RaceTrackerDBAsyncTask task = new RaceTrackerDBAsyncTask(connectionString, dbUser, dbPwd);
        task.execute(query);
    }

    /**
     * Returns all the data points for the given competition
     * @param callback Callback called when results are ready
     * @param competition_id Competition
     */
    public void getDataPoints(OnQueryResultReady callback, int competition_id) {
        RaceTrackerQuery query = new RaceTrackerQuery();
        query.setCallback(callback);
        query.setQuery(MessageFormat.format("SELECT data_point_id, competitor_id, " +
                "competition_id, sequence, time_stamp, ST_X(position) as lat, ST_Y(position) as lon, " +
                "heart_rate, cadence, nb_satellites, position_dop, status " +
                "FROM race_tracker.data_point WHERE data_point.competition_id = {0};"
                , competition_id));
        RaceTrackerDBAsyncTask task = new RaceTrackerDBAsyncTask(connectionString, dbUser, dbPwd);
        task.execute(query);
    }

    /**
     * Get all the competitors registered for a race
     * @param callback Callback called when results are ready
     * @param competition_id Competition
     */
    public void getCompetitors(OnQueryResultReady callback, int competition_id) {
        RaceTrackerQuery query = new RaceTrackerQuery();
        query.setCallback(callback);
        query.setQuery(MessageFormat.format("SELECT * FROM " +
                "race_tracker.competitor_registration INNER JOIN race_tracker.competitor ON " +
                "(competitor_registration.competitor_id = competitor.competitor_id) " +
                "WHERE competitor_registration.competition_id = {0};", competition_id));
        RaceTrackerDBAsyncTask task = new RaceTrackerDBAsyncTask(connectionString, dbUser, dbPwd);
        task.execute(query);
    }

    /**
     * Get all the countries
     * @param callback Callback called when results are ready
     */
    public void getCountries(OnQueryResultReady callback) {
        RaceTrackerQuery query = new RaceTrackerQuery();
        query.setCallback(callback);
        query.setQuery("SELECT * FROM race_tracker.country;");
        RaceTrackerDBAsyncTask task = new RaceTrackerDBAsyncTask(connectionString, dbUser, dbPwd);
        task.execute(query);
    }
}
