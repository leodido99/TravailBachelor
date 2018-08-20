package ch.heigvd.bisel.racetracker.activities;

import android.os.Handler;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.Toast;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import java.sql.Array;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitor;
import ch.heigvd.bisel.racetracker.OnQueryResultReady;
import ch.heigvd.bisel.racetracker.RaceTrackerDB;
import ch.heigvd.bisel.racetracker.RaceTrackerDataPoint;

public class ViewRaceActivity extends FragmentActivity implements OnMapReadyCallback {
    private GoogleMap mMap;
    private RaceTrackerCompetition competition;
    private Map<Integer, RaceTrackerCompetitor> competitors;
    private OnCompetitorsResults onCompetitorsResults;
    private OnLastDataPointResults onLastDataPointResults;
    private OnDataPointsResults onDataPointsResults;
    private RaceTrackerDB db;
    private int handlerInterval = 5000; /* In seconds */
    private Handler handler;
    private boolean leavePositionTrail;
    private ArrayList<RaceTrackerDataPoint> dataPoints;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_race_google);
        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);

        /* Retrieve competition class from intent */
        competition = (RaceTrackerCompetition)getIntent().getSerializableExtra("competition");

        /* Initialize the competitors map */
        competitors = new HashMap<>();

        /* Create query results handler */
        onLastDataPointResults = new OnLastDataPointResults();
        onCompetitorsResults = new OnCompetitorsResults();
        onDataPointsResults = new OnDataPointsResults();

        /* Get Competitors */
        db = new RaceTrackerDB(this);
        db.getCompetitors(onCompetitorsResults, competition.getCompetitionId());

        /* Create handler */
        handler = new Handler();

        /* Create list of all data points used by the replay mode */
        dataPoints = new ArrayList<>();

        leavePositionTrail = true;
    }

    public void handleDataPointLive(RaceTrackerDataPoint dataPoint) {
        /* Check if competitor of the data point is known */
        if (!competitors.containsKey(dataPoint.getCompetitorId())) {
            Toast.makeText(getApplicationContext(), "Competitor of the data point is unknown", Toast.LENGTH_LONG).show();
            return;
        }

        /* Check if the marker position needs updating */
        /* TODO Check for jump in sequence i.e lost packet */
        if (!competitors.get(dataPoint.getCompetitorId()).hasLastDataPoint() ||
            dataPoint.getSequence() > competitors.get(dataPoint.getCompetitorId()).getLastDataPoint().getSequence()) {
            /* In leave position trail mode, we create a new marker to save the last position */
            if (competitors.get(dataPoint.getCompetitorId()).hasLastDataPoint() && leavePositionTrail) {
                /* Create new marker at old position */
                Marker oldpos = mMap.addMarker(new MarkerOptions().position(competitors.get(dataPoint.getCompetitorId()).getMapMarker().getPosition()));
                oldpos.setIcon(BitmapDescriptorFactory.fromResource(R.drawable.competitor_marker_grey));
                oldpos.setTitle(competitors.get(dataPoint.getCompetitorId()).getMapMarker().getTitle()
                        + " #" + competitors.get(dataPoint.getCompetitorId()).getLastDataPoint().getSequence());
            }
            /* Add data point to competitor and update the marker position */
            competitors.get(dataPoint.getCompetitorId()).setLastDataPoint(dataPoint);
            competitors.get(dataPoint.getCompetitorId()).updatePositionMarker();
        }
    }

    public void handleDataPointReplay(RaceTrackerDataPoint dataPoint) {

    }

    /**
     * Class handling the result of the last data point query
     * This query retrieves all the last data points (highest sequence number)
     */
    public class OnLastDataPointResults implements OnQueryResultReady {
        private RaceTrackerDB.RaceTrackerQuery results;

        public RaceTrackerDB.RaceTrackerQuery getResults() {
            return results;
        }

        /* Callback when competitions are ready */
        public void onQueryResultReady(RaceTrackerDB.RaceTrackerQuery results) throws SQLException {
            this.results = results;

            if (results.getException() != null) {
                /* Exception during query */
                Toast.makeText(getApplicationContext(), "Exception during query: " + results.getException().getMessage(), Toast.LENGTH_LONG).show();
            } else {
                while (results.getResult().next()) {
                    RaceTrackerDataPoint dataPoint = new RaceTrackerDataPoint(results.getResult());
                    handleDataPointLive(dataPoint);
                }

                results.getResult().getStatement().close();
                results.getResult().close();
            }

            /* Updates UI */
            //mAdapter.notifyDataSetChanged();
        }
    }

    public void addMarker(RaceTrackerDataPoint dataPoint, int markerResource) {
        /* Check if competitor of the data point is known */
        if (!competitors.containsKey(dataPoint.getCompetitorId())) {
            Toast.makeText(getApplicationContext(), "Competitor of the data point is unknown", Toast.LENGTH_LONG).show();
            return;
        }

        Marker marker = mMap.addMarker(new MarkerOptions().position(dataPoint.getPosition()));
        marker.setIcon(BitmapDescriptorFactory.fromResource(markerResource));
        marker.setTitle(competitors.get(dataPoint.getCompetitorId()).getFirstName() + " " + competitors.get(dataPoint.getCompetitorId()).getLastName()
                + " #" + dataPoint.getSequence());

        /* Update the last data point of the competitor if needed */
        if (!competitors.get(dataPoint.getCompetitorId()).hasLastDataPoint() || dataPoint.getSequence() > competitors.get(dataPoint.getCompetitorId()).getLastDataPoint().getSequence()) {
            competitors.get(dataPoint.getCompetitorId()).setLastDataPoint(dataPoint);
        }
    }

    public class OnDataPointsResults implements OnQueryResultReady {
        private RaceTrackerDB.RaceTrackerQuery results;

        public RaceTrackerDB.RaceTrackerQuery getResults() {
            return results;
        }

        /* Callback when competitions are ready */
        public void onQueryResultReady(RaceTrackerDB.RaceTrackerQuery results) throws SQLException {
            this.results = results;

            if (results.getException() != null) {
                /* Exception during query */
                Toast.makeText(getApplicationContext(), "Exception during query: " + results.getException().getMessage(), Toast.LENGTH_LONG).show();
            } else {
                while (results.getResult().next()) {
                    RaceTrackerDataPoint dataPoint = new RaceTrackerDataPoint(results.getResult());
                    if (competition.isActive()) {
                        handleDataPointLive(dataPoint);
                    } else {
                        /* In replay mode we simply fill the list */
                        dataPoints.add(dataPoint);
                    }
                }

                results.getResult().getStatement().close();
                results.getResult().close();
            }

            /* Updates UI */
            //mAdapter.notifyDataSetChanged();

            /* In replay mode start the handler */
            if (!competition.isActive()) {
                /* TODO Start handler for replay mode (set interval with difference from timestamps */

            } else {
                /* Once the marker up until now are set, we can check for new points */
                startDataPointChecker();
            }
        }
    }

    public void setupMarkers() {
        /* Send query to get all data points until now */
        db.getDataPoints(onDataPointsResults, competition.getCompetitionId());
    }

    public class OnCompetitorsResults implements OnQueryResultReady {
        private RaceTrackerDB.RaceTrackerQuery results;

        public RaceTrackerDB.RaceTrackerQuery getResults() {
            return results;
        }

        /* Callback when competitions are ready */
        public void onQueryResultReady(RaceTrackerDB.RaceTrackerQuery results) throws SQLException {
            this.results = results;

            if (results.getException() != null) {
                /* Exception during query */
                Toast.makeText(getApplicationContext(), "Exception during query: " + results.getException().getMessage(), Toast.LENGTH_LONG).show();
            } else {
                while (results.getResult().next()) {
                    RaceTrackerCompetitor competitor = new RaceTrackerCompetitor(results.getResult());
                    competitors.put(competitor.getCompetitorId(), competitor);
                    /* Setup map marker */
                    competitor.setMapMarker(mMap.addMarker(new MarkerOptions()
                            .position(new LatLng(0,0))
                            .title(competitor.getFirstName() + " " + competitor.getLastName())
                            .visible(false)
                            .icon(BitmapDescriptorFactory.fromResource(R.drawable.competitor_marker_blue))));
                    System.out.println("DBG: Competitor: " + competitor.toString());
                }

                results.getResult().getStatement().close();
                results.getResult().close();
            }

            /* Updates UI */
            //mAdapter.notifyDataSetChanged();

            /* Setup markers of data points */
            setupMarkers();
        }
    }

    public void startDataPointChecker() {
        handler.postDelayed(dataPointChecker, 0);
    }

    public void stopDataPointChecker() {
        handler.removeCallbacks(dataPointChecker);
    }

    /**
     * Manipulates the map once available.
     * This callback is triggered when the map is ready to be used.
     * This is where we can add markers or lines, add listeners or move the camera. In this case,
     * we just add a marker near Sydney, Australia.
     * If Google Play services is not installed on the device, the user will be prompted to install
     * it inside the SupportMapFragment. This method will only be triggered once the user has
     * installed Google Play services and returned to the app.
     */
    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;

        /* Change map type */
        mMap.setMapType(GoogleMap.MAP_TYPE_SATELLITE);

        /* Move camera and zoom to competition position*/
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(competition.getLocation().getObject(), competition.getZoom()));
    }

    Runnable dataPointChecker = new Runnable() {
        @Override
        public void run() {
            try {
                db.getLastDataPoint(onLastDataPointResults, competition.getCompetitionId());
            } finally {
                /* Restart handler */
                handler.postDelayed(dataPointChecker, handlerInterval);
            }
        }
    };
}
