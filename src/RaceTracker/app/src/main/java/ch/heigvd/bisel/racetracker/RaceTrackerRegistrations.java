package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.text.MessageFormat;
import java.util.ArrayList;

public class RaceTrackerRegistrations implements OnQueryResultReady, OnQueryExecuted {
    private ArrayList<RaceTrackerRegistration> registrations;
    private RaceTrackerRegistrations.OnRegistrationsReady callback;
    private RaceTrackerQuery query;
    private RaceTrackerDBConnection connection;

    /**
     * Create new instance
     * @param connection Connection to the database
     */
    public RaceTrackerRegistrations(RaceTrackerDBConnection connection) {
        this.connection = connection;
    }

    /**
     * Create a new instance
     * @param connection Connection to the database
     * @param callback Called when data points are ready to be retrieved
     * @param competitionId Competition ID
     */
    public RaceTrackerRegistrations(RaceTrackerDBConnection connection,
                                    RaceTrackerRegistrations.OnRegistrationsReady callback,
                                    int competitionId) {
        this(connection);
        registrations = new ArrayList<>();
        this.callback = callback;

        /* Prepare and execute query */
        query = new RaceTrackerQuery(connection);
        query.setQuery(MessageFormat.format("SELECT * FROM race_tracker.competitor_registration " +
                        "WHERE competition_id = {0};"
                , competitionId));
        query.setCallbackResultReady(this);
        query.setCallbackExecuted(this);
        query.execute();
    }

    /**
     * Inserts a new registration
     * @param registration New registration
     */
    public void insertNewRegistration(RaceTrackerRegistration registration) {
        RaceTrackerQuery query = new RaceTrackerQuery(connection);
        query.setQuery(MessageFormat.format("INSERT INTO race_tracker.competitor_registration " +
                        "(competitor_id, competition_id, sensor_id, bib_number) " +
                        "VALUES ({1}, {2}, {3}, {4});",
                registration.getCompetitorId(),
                registration.getCompetitionId(),
                registration.getSensorId(),
                registration.getBibNumber()));
        query.setUpdateQuery(true);
        query.execute();
    }

    /**
     * Callback executed when query is finished executing
     * @param query Query that was executed
     */
    @Override
    public void onQueryExecuted(RaceTrackerQuery query) {
        this.callback.onRegistrationsReady(registrations);
    }

    /**
     * Callback executed when the results of the query are ready
     * @param results Results of the query
     * @throws SQLException
     * @throws RuntimeException
     */
    @Override
    public void onQueryResultReady(RaceTrackerQuery results) throws SQLException {
        if (results.getException() != null) {
            /* Exception during query */
            throw new RuntimeException(results.getException().getMessage());
        } else {
            while (results.getResult().next()) {
                RaceTrackerRegistration registration = new RaceTrackerRegistration(results.getResult());
                registrations.add(registration);
            }
        }
    }

    /**
     * Interface called when results are ready
     */
    public interface OnRegistrationsReady {
        void onRegistrationsReady(ArrayList<RaceTrackerRegistration> registrations);
    }
}
