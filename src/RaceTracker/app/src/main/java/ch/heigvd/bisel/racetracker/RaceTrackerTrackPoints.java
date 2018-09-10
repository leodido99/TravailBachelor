package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.text.MessageFormat;
import java.util.ArrayList;

public class RaceTrackerTrackPoints implements OnQueryResultReady, OnQueryExecuted {
    private ArrayList<RaceTrackerTrackPoint> trackPoints;
    private RaceTrackerTrackPoints.OnTrackPointsReady callback;
    private RaceTrackerQuery query;

    /**
     * Create a new instance
     * @param connection Connection to the database
     * @param callback Called when track points are ready to be retrieved
     */
    public RaceTrackerTrackPoints(RaceTrackerDBConnection connection, RaceTrackerTrackPoints.OnTrackPointsReady callback, int competitionId) {
        this.callback = callback;
        trackPoints = new ArrayList<>();

        /* Prepare and execute query */
        query = new RaceTrackerQuery(connection);
        query.setQuery(MessageFormat.format("SELECT track_point_id, competition_id, sequence, " +
                        "ST_X(position) as latitude, ST_Y(position) as longitude FROM " +
                        "race_tracker.track_point WHERE competition_id = {0} ORDER BY sequence;"
                , competitionId));
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
                RaceTrackerTrackPoint trackPoint = new RaceTrackerTrackPoint(results.getResult());
                trackPoints.add(trackPoint);
            }
        }
    }

    /**
     * Callback executed when query is finished executing
     * @param query Query that was executed
     */
    public void onQueryExecuted(RaceTrackerQuery query) {
        this.callback.onTrackPointsReady(trackPoints);
    }

    /**
     * Interface called when results are ready
     */
    public interface OnTrackPointsReady {
        void onTrackPointsReady(ArrayList<RaceTrackerTrackPoint> trackPoints);
    }
}
