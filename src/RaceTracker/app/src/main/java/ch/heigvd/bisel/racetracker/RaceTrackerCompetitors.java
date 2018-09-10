package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.text.MessageFormat;
import java.util.ArrayList;

public class RaceTrackerCompetitors implements OnQueryResultReady, OnQueryExecuted {
    private int competitionId;
    private ArrayList<RaceTrackerCompetitor> competitors;
    private RaceTrackerCompetitors.OnCompetitorsReady callback;
    private RaceTrackerQuery query;

    /**
     * Create a new instance to retrieve the list of competitors
     * @param connection Connection to the database
     * @param callback Called when competitions are ready to be retrieved
     * @param competitionId Competition ID to retrieve the competitors
     */
    public RaceTrackerCompetitors(RaceTrackerDBConnection connection, RaceTrackerCompetitors.OnCompetitorsReady callback, int competitionId) {
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

    /**
     * Callback executed when the results of the query are ready
     * @param results Results of the query
     * @throws SQLException
     * @throws RuntimeException
     */
    public void onQueryResultReady(RaceTrackerQuery results) throws SQLException, RuntimeException {
        if (results.getException() != null) {
            /* Exception during query */
            throw new RuntimeException(results.getException().getMessage());
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
        this.callback.onCompetitorsReady(competitors);
    }

    /**
     * Returns the list of competitors
     * @return Competitors
     */
    public ArrayList<RaceTrackerCompetitor> getCompetitors() {
        return competitors;
    }

    /**
     * Sets the list of competitors
     * @param competitors Competitors
     */
    public void setCompetitors(ArrayList<RaceTrackerCompetitor> competitors) {
        this.competitors = competitors;
    }

    /**
     * Interface called when results are ready
     */
    public interface OnCompetitorsReady {
        void onCompetitorsReady(ArrayList<RaceTrackerCompetitor> competitors);
    }
}
