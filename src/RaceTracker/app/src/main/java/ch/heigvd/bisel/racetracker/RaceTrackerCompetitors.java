package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.text.MessageFormat;
import java.util.ArrayList;

public class RaceTrackerCompetitors implements OnQueryResultReady, OnQueryExecuted {
    private int competitionId;
    private ArrayList<RaceTrackerCompetitor> competitors;
    private OnDataReady callback;
    private RaceTrackerQuery query;
    private int id;

    public RaceTrackerCompetitors(RaceTrackerDBConnection connection, OnDataReady callback, int competitionId) {
        this.competitionId = competitionId;
        this.callback = callback;
        competitors = new ArrayList<>();

        /* Prepare and execute query */
        query = new RaceTrackerQuery(connection);
        query.setQuery(MessageFormat.format("SELECT * FROM " +
                "race_tracker.competitor_registration INNER JOIN race_tracker.competitor ON " +
                "(competitor_registration.competitor_id = competitor.competitor_id) " +
                "WHERE competitor_registration.competition_id = {0};", this.competitionId));
        query.setCallbackResultReady(this);
        query.setCallbackExecuted(this);
        query.execute();
    }

    public RaceTrackerCompetitors(int id, RaceTrackerDBConnection connection, OnDataReady callback, int competitionId) {
        this(connection, callback, competitionId);
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
                RaceTrackerCompetitor competitor = new RaceTrackerCompetitor(results.getResult());
                competitors.add(competitor);
            }
        }
    }

    /**
     * Callback executed when query is finished executing
     * @param query Query that was executed
     */
    public void onQueryExecuted(RaceTrackerQuery query) {
        this.callback.onDataReady();
    }

    public ArrayList<RaceTrackerCompetitor> getCompetitors() {
        return competitors;
    }

    public void setCompetitors(ArrayList<RaceTrackerCompetitor> competitors) {
        this.competitors = competitors;
    }
}
