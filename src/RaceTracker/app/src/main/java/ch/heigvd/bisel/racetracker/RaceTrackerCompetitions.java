package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.util.ArrayList;

public class RaceTrackerCompetitions implements OnQueryResultReady, OnQueryExecuted {
    private OnDataReady callback;
    private ArrayList<RaceTrackerCompetition> competitions;
    private RaceTrackerDB raceTrackerDB;
    private RaceTrackerQuery query;
    private int id;

    /**
     * Create a new instance to retrieve the list of competitions
     *
     * @param connection Connection to the database
     * @param callback Called when competitions are ready to be retrieved
     */
    public RaceTrackerCompetitions(RaceTrackerDBConnection connection, OnDataReady callback) {
        this.callback = callback;
        competitions = new ArrayList<>();

        /* Prepare and execute query */
        query = new RaceTrackerQuery(connection);
        query.setQuery("SELECT competition_id, name, ST_X(location) as lat, ST_Y(location) as lon, " +
                "event_date, active, zoom " +
                "FROM race_tracker.competition;");
        query.setCallbackResultReady(this);
        query.setCallbackExecuted(this);
        query.execute();
    }

    /**
     * Creates a new instance to retrieve the list of competitions
     * @param id ID of the transaction (provided back in callback)
     * @param connection Connection to the database
     * @param callback Called when competitions are ready to be retrieved
     */
    public RaceTrackerCompetitions(int id, RaceTrackerDBConnection connection, OnDataReady callback) {
        this(connection, callback);
        this.id = id;
    }

    /**
     * Callback executed when the results of the query are ready
     * @param results Results of the query
     * @throws SQLException
     */
    public void onQueryResultReady(RaceTrackerQuery results) throws SQLException {
        if (results.getException() != null) {
            /* Exception during query */

        } else {
            while (results.getResult().next()) {
                RaceTrackerCompetition competition = new RaceTrackerCompetition(results.getResult());
                competitions.add(competition);
            }
        }
    }

    /**
     * Callback executed when query is finished executing
     * @param query Query that was executed
     */
    public void onQueryExecuted(RaceTrackerQuery query) {
        this.callback.onDataReady(this.id);
    }

    /**
     * Return list of competitions
     * @return List of competitions
     */
    public ArrayList<RaceTrackerCompetition> getCompetitions() {
        return competitions;
    }

    /**
     * Sets the list of competitions
     * @param competitions List of competitions
     */
    public void setCompetitions(ArrayList<RaceTrackerCompetition> competitions) {
        this.competitions = competitions;
    }
}
