package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class RaceTrackerDataPoints implements OnQueryResultReady, OnQueryExecuted {
    private Map<Integer, ArrayList<RaceTrackerDataPoint>> dataPoints;
    private int competitionId;
    private RaceTrackerDBConnection connection;
    private RaceTrackerDataPoints.OnDataPointsReady callback;
    private RaceTrackerQuery query;
    private Boolean lastPoint = false;

    /**
     * Create a new instance
     * @param connection Connection to the database
     * @param callback Called when data points are ready to be retrieved
     */
    public RaceTrackerDataPoints(RaceTrackerDBConnection connection, RaceTrackerDataPoints.OnDataPointsReady callback, int competitionId) {
        this.competitionId = competitionId;
        this.connection = connection;

        this.callback = callback;
        dataPoints = new HashMap<>();

        /* Prepare and execute query */
        query = new RaceTrackerQuery(connection);
        query.setQuery(MessageFormat.format("SELECT data_point_id, competitor_id, " +
                        "competition_id, sequence, time_stamp, ST_X(position) as lat, ST_Y(position) as lon, " +
                        "heart_rate, cadence, nb_satellites, position_dop, status " +
                        "FROM race_tracker.data_point WHERE data_point.competition_id = {0} ORDER BY sequence;"
                , competitionId));
        query.setCallbackResultReady(this);
        query.setCallbackExecuted(this);
        query.execute();
    }

    /**
     * Provides the last data point of each competitor
     */
    public void getLastPoints() {
        dataPoints.clear();

        /* Prepare and execute query */
        query = new RaceTrackerQuery(connection);
        query.setQuery(MessageFormat.format("SELECT data_point_id, competitor_id, " +
                "competition_id, sequence, time_stamp, ST_X(position) as lat, ST_Y(position) as lon" +
                ", heart_rate, cadence, nb_satellites, position_dop, status FROM (SELECT rank() " +
                "OVER (PARTITION BY data_point.competitor_id ORDER BY sequence DESC) AS therank, * " +
                "FROM race_tracker.data_point WHERE data_point.competition_id = {0}) t " +
                "WHERE therank = 1;", competitionId));
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
                RaceTrackerDataPoint dataPoint = new RaceTrackerDataPoint(results.getResult());
                /* Check if the map for the competitor already exists */
                if (!dataPoints.containsKey(dataPoint.getCompetitorId())) {
                    dataPoints.put(dataPoint.getCompetitorId(), new ArrayList<RaceTrackerDataPoint>());
                }
                dataPoints.get(dataPoint.getCompetitorId()).add(dataPoint);
            }
        }
    }

    /**
     * Callback executed when query is finished executing
     * @param query Query that was executed
     */
    public void onQueryExecuted(RaceTrackerQuery query) {
        this.callback.onDataPointsReady(dataPoints);
    }

    /**
     * Interface called when results are ready
     */
    public interface OnDataPointsReady {
        void onDataPointsReady(Map<Integer, ArrayList<RaceTrackerDataPoint>> dataPoints);
    }
}
