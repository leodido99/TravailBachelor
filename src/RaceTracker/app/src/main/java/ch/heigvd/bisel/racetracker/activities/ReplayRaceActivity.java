package ch.heigvd.bisel.racetracker.activities;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.preference.PreferenceManager;

import com.google.android.gms.maps.CameraUpdate;
import com.google.android.gms.maps.CameraUpdateFactory;

import java.util.ArrayList;
import java.util.Map;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerDataPoint;

public class ReplayRaceActivity extends ViewRaceActivity {
    private Map<Integer, ArrayList<RaceTrackerDataPoint>> dataPoints;

    /**
     * Triggered on creation of the activity
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    /**
     * Triggered when data points are ready
     * @param dataPoints List of new data points
     */
    @Override
    public void onDataPointsReady(Map<Integer, ArrayList<RaceTrackerDataPoint>> dataPoints) {
        DataPointHandler handler;

        this.dataPoints = dataPoints;

        /* Create data point handlers for each competitor */
        for (Map.Entry<Integer, ArrayList<RaceTrackerDataPoint>> entry : this.dataPoints.entrySet()) {
            handler = new DataPointHandler(entry.getValue());

            /* Create competitor marker */
            competitors.get(entry.getKey()).setMapMarker(addMarker(handler.getLastDataPoint().getPosition(),
                    R.drawable.competitor_marker_green,
                    competitors.get(handler.getLastDataPoint().getCompetitorId()).getFirstName()
                            + " " + competitors.get(handler.getLastDataPoint().getCompetitorId()).getLastName()));
        }
    }

    /**
     * Handler for the data points
     * Sets up the next handler call to provide the next data point to the competitor
     */
    public class DataPointHandler {
        private ArrayList<RaceTrackerDataPoint> dataPoints;
        private RaceTrackerDataPoint lastDataPoint;
        private Handler handler;

        /**
         * Create a new instance of DataPointHandler
         * @param dataPoints dataPoints to handle
         */
        public DataPointHandler(ArrayList<RaceTrackerDataPoint> dataPoints) {
            this.dataPoints = dataPoints;
            handler = new Handler();
            /* Get first data point and remove it from list */
            lastDataPoint = getNextDataPoint();

            /* Trigger directly */
            handler.postDelayed(dataPointProvider, 0);
        }

        /**
         * Returns the last data point handled
         * @return Last data point
         */
        public RaceTrackerDataPoint getLastDataPoint() {
            return lastDataPoint;
        }

        /**
         * Returns the next data point
         * DataPoint in arraylist are sorted by seq nb
         * @return Next data point
         */
        private RaceTrackerDataPoint getNextDataPoint() {
            RaceTrackerDataPoint next;
            next = this.dataPoints.get(0);
            this.dataPoints.remove(next);
            return next;
        }

        /**
         * Runnable used to provide next data point to the competitors
         */
        Runnable dataPointProvider = new Runnable() {
            @Override
            public void run() {
                RaceTrackerDataPoint next;
                long elapsedMs = 0;
                SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
                Boolean leaveTrail = sharedPref.getBoolean(getResources().getString(R.string.leave_trail_key), false);
                float timeScale = Float.parseFloat(sharedPref.getString(getResources().getString(R.string.replay_scale_key), ""));

                try {
                    /* Give data point to be consumed to competitor */
                    competitors.get(lastDataPoint.getCompetitorId()).consumeDataPoint(lastDataPoint);
                    next = getNextDataPoint();
                    System.out.println("DBG: Prevseq: " + lastDataPoint.getSequence() +
                    " Nextseq: " + next.getSequence());
                    /* Calculate time between data point */
                    elapsedMs = next.getTimeStamp().getTime() - lastDataPoint.getTimeStamp().getTime();
                    elapsedMs *= timeScale;
                    System.out.println("DBG: Next data point in " + elapsedMs/1000 + " s");
                    /* Create new marker if leave trail mode is on */
                    if (leaveTrail) {
                        addMarker(lastDataPoint.getPosition(), R.drawable.competitor_marker_grey,
                                competitors.get(lastDataPoint.getCompetitorId()).getFirstName()
                                        + " " + competitors.get(lastDataPoint.getCompetitorId()).getLastName() + " #"
                                        + lastDataPoint.getSequence());
                    }

                    /* Update competitor marker */
                    competitors.get(lastDataPoint.getCompetitorId()).getMapMarker()
                            .setPosition(next.getPosition());

                    /* If the competitor is focused then update camera */
                    if (focusedCompetitor == competitors.get(lastDataPoint.getCompetitorId())) {
                        CameraUpdate competitionLocation = CameraUpdateFactory.newLatLng(
                                next.getPosition());
                        mMap.animateCamera(competitionLocation);
                    }

                    lastDataPoint = next;
                    /* Updates UI */
                    mAdapter.notifyDataSetChanged();
                } finally {
                    /* Restart handler */
                    handler.postDelayed(dataPointProvider, elapsedMs);
                }
            }
        };

    }
}
