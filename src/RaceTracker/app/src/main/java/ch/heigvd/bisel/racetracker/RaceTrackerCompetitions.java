package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.text.MessageFormat;
import java.util.ArrayList;

public class RaceTrackerCompetitions implements OnQueryResultReady, OnQueryExecuted {
    private RaceTrackerCompetitions.OnCompetitionsReady callback;
    private OnUpdateDone callbackUpdate;
    private ArrayList<RaceTrackerCompetition> competitions;
    private RaceTrackerDBConnection connection;

    public RaceTrackerCompetitions(RaceTrackerDBConnection connection) {
        this.connection = connection;
    }

    /**
     * Create a new instance to retrieve the list of competitions
     *
     * @param connection Connection to the database
     * @param callback Called when competitions are ready to be retrieved
     */
    public RaceTrackerCompetitions(RaceTrackerDBConnection connection, RaceTrackerCompetitions.OnCompetitionsReady callback) {
        this(connection);
        this.callback = callback;
        competitions = new ArrayList<>();

        /* Prepare and execute query */
        RaceTrackerQuery query = new RaceTrackerQuery(connection);
        query.setQuery("SELECT competition_id, name, ST_X(location) as lat, ST_Y(location) as lon, " +
                "event_date, active, zoom " +
                "FROM race_tracker.competition;");
        query.setCallbackResultReady(this);
        query.setCallbackExecuted(this);
        query.execute();
    }

    /**
     * Insert a new race in the database
     * @param competition New race
     */
    public void insertNewRace(RaceTrackerCompetition competition, OnUpdateDone callback) {
        RaceTrackerQuery query = new RaceTrackerQuery(connection);
        callbackUpdate = callback;
        query.setQuery(MessageFormat.format("INSERT INTO race_tracker.competition (name, location, event_date, active, zoom)" +
                " VALUES (''{0}'', ST_MakePoint({1}, {2}), " +
                "''{3}'', {4}, {5});",
                competition.getName(),
                competition.getLocation().getObject().latitude,
                competition.getLocation().getObject().longitude,
                competition.getEventDate().toString(),
                competition.isActive(),
                competition.getZoom()));
        query.setUpdateQuery(true);
        query.setCallbackExecuted(this);
        query.execute();
    }

    public void updateRace(RaceTrackerCompetition competition, OnUpdateDone callback) {
        RaceTrackerQuery query = new RaceTrackerQuery(connection);
        callbackUpdate = callback;
        query.setQuery(MessageFormat.format("UPDATE race_tracker.competition " +
                "SET active = {0} WHERE competition_id = {1};",
                competition.isActive(),
                competition.getCompetitionId()));
        query.setUpdateQuery(true);
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
        if (query.isUpdateQuery()) {
            this.callbackUpdate.onUpdateDone(query.getNbUpdated(), query.getException());
        } else {
            this.callback.onCompetitionsReady(competitions);
        }
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
