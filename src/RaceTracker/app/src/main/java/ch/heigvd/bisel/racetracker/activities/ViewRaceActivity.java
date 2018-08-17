package ch.heigvd.bisel.racetracker.activities;

import android.os.Handler;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.MarkerOptions;

import java.sql.ResultSet;
import java.sql.SQLException;
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
    private RaceTrackerDB db;
    private int handlerInterval = 5000; /* In seconds */
    private Handler handler;

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

        /* Get Competitors */
        db = new RaceTrackerDB(this);
        db.getCompetitors(onCompetitorsResults, competition.getCompetitionId());

        /* Create handler */
        handler = new Handler();
    }

    public void handleDataPointLive(RaceTrackerDataPoint dataPoint) {
        /* First data point */
        if (!competitors.get(dataPoint.getCompetitorId()).hasLastDataPoint()) {
            competitors.get(dataPoint.getCompetitorId()).setLastDataPoint(dataPoint);
            competitors.get(dataPoint.getCompetitorId()).updatePositionMarker();
        } else {
            /* Check if data point is new */
            /* TODO Check for jump in sequence */
            if (dataPoint.getSequence() > competitors.get(dataPoint.getCompetitorId()).getLastDataPoint().getSequence()) {
                competitors.get(dataPoint.getCompetitorId()).setLastDataPoint(dataPoint);
                competitors.get(dataPoint.getCompetitorId()).updatePositionMarker();
            }
        }
    }

    public void handleDataPointReplay(RaceTrackerDataPoint dataPoint) {

    }

    public void handleDataPoint(RaceTrackerDataPoint dataPoint) {
        if (competition.isActive()) {
            handleDataPointLive(dataPoint);
        } else {
            handleDataPointReplay(dataPoint);
        }
    }

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
            } else {
                while (results.getResult().next()) {
                    RaceTrackerDataPoint dataPoint = new RaceTrackerDataPoint(results.getResult());
                    handleDataPoint(dataPoint);
                /*RaceTrackerCompetition cp = new RaceTrackerCompetition(results);
                resultList.add(cp);
                System.out.println("DBG: Competition: " + cp.toString());*/
                }

                results.getResult().getStatement().close();
                results.getResult().close();
            }


            /* Updates UI */
            //mAdapter.notifyDataSetChanged();
        }
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
            } else {
                while (results.getResult().next()) {
                    RaceTrackerCompetitor competitor = new RaceTrackerCompetitor(results.getResult());
                    competitors.put(competitor.getCompetitorId(), competitor);
                    System.out.println("DBG: Competitor: " + competitor.toString());
                }

                results.getResult().getStatement().close();
                results.getResult().close();
            }

            /* Updates UI */
            //mAdapter.notifyDataSetChanged();
            /* Once the competitor list is ready, we can start checking for data points */
            startDataPointChecker();
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


        //mMap.addMarker(new MarkerOptions().position(competition.getLocation().getObject()).title("Race"));
        mMap.moveCamera(CameraUpdateFactory.newLatLng(competition.getLocation().getObject()));
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
