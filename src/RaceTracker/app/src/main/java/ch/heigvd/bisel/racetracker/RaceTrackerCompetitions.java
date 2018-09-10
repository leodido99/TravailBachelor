package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.util.ArrayList;

public class RaceTrackerCompetitions implements OnQueryResultReady, OnQueryExecuted {
    private RaceTrackerCompetitions.OnCompetitionsReady callback;
    private ArrayList<RaceTrackerCompetition> competitions;
    private RaceTrackerQuery query;

    /**
     * Create a new instance to retrieve the list of competitions
     *
     * @param connection Connection to the database
     * @param callback Called when competitions are ready to be retrieved
     */
    public RaceTrackerCompetitions(RaceTrackerDBConnection connection, RaceTrackerCompetitions.OnCompetitionsReady callback) {
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
     * Callback executed when the results of the query are ready
     * @param results Results of the query
     * @throws SQLException
     */
    public void onQueryResultReady(RaceTrackerQuery results) throws SQLException, RuntimeException {
        if (results.getException() != null) {
            /* Exception during query */
            throw new RuntimeException(results.getException().getMessage());
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
        this.callback.onCompetitionsReady(competitions);
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

    /**
     * Interface used for callbacks when data is ready
     */
    public interface OnCompetitionsReady {
        void onCompetitionsReady(ArrayList<RaceTrackerCompetition> competitions);
    }
}
