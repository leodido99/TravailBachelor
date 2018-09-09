package ch.heigvd.bisel.racetracker.activities;

import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.Toast;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.maps.model.Polyline;
import com.google.android.gms.maps.model.PolylineOptions;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitor;
import ch.heigvd.bisel.racetracker.OnQueryResultReady;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitorAdapter;
import ch.heigvd.bisel.racetracker.RaceTrackerCountry;
import ch.heigvd.bisel.racetracker.RaceTrackerDB;
import ch.heigvd.bisel.racetracker.RaceTrackerDataPoint;
import ch.heigvd.bisel.racetracker.RaceTrackerQuery;

public class ViewRaceActivity extends AppCompatActivity implements OnMapReadyCallback {
    private GoogleMap mMap;
    private Polyline track;

    private RaceTrackerDB db;
    private RaceTrackerCompetition competition;
    private Map<Integer, RaceTrackerCompetitor> competitors;
    private Map<String, RaceTrackerCountry> countries;
    private ArrayList<RaceTrackerDataPoint> dataPoints;

    private OnCountryResults onCountryResults;
    private OnCompetitorsResults onCompetitorsResults;
    private OnLastDataPointResults onLastDataPointResults;
    private OnDataPointsResults onDataPointsResults;
    private OnTrackPointResults onTrackPointResults;

    private int handlerInterval = 5000;
    private Handler handler;

    private RecyclerView mRecyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;

    /**
     * Called upon Activity creation
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_race);

        /* Obtain the SupportMapFragment and get notified when the map is ready to be used. */
        MapFragment mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.mapView);
        mapFragment.getMapAsync(this);

        /* Setup RecyclerView */
        mRecyclerView = findViewById(R.id.CompetitorsRecycler);

        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        /* Add line between items */
        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, LinearLayoutManager.VERTICAL));

        /* Retrieve competition class from intent */
        competition = (RaceTrackerCompetition)getIntent().getSerializableExtra("competition");

        /* Initialize the maps */
        competitors = new HashMap<>();
        countries = new HashMap<>();

        /* Create query results handler */
        onLastDataPointResults = new OnLastDataPointResults();
        onCompetitorsResults = new OnCompetitorsResults();
        onDataPointsResults = new OnDataPointsResults();
        onCountryResults = new OnCountryResults();
        onTrackPointResults = new OnTrackPointResults();

        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        db = new RaceTrackerDB(sharedPref.getString("server_address", ""));
        /* Get Countries */
        try {
            db.getCountries(onCountryResults);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        /* Get Competitors */
        try {
            db.getCompetitors(onCompetitorsResults, competition.getCompetitionId());
        } catch (SQLException e) {
            e.printStackTrace();
        }

        /* Get Track points */
        db.getTrackPoints(onTrackPointResults, competition.getCompetitionId());

        /* Create handler */
        handler = new Handler();

        /* Create list of all data points used by the replay mode */
        dataPoints = new ArrayList<>();
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
            /* Retrieve preferences manager */
            SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());

            /* In leave position trail mode, we create a new marker to save the last position */
            if (competitors.get(dataPoint.getCompetitorId()).hasLastDataPoint() && sharedPref.getBoolean("leave_point_trail", false)) {
                /* Create new marker at old position */
                Marker oldpos = mMap.addMarker(new MarkerOptions().position(competitors.get(dataPoint.getCompetitorId()).getMapMarker().getPosition()));
                oldpos.setIcon(BitmapDescriptorFactory.fromResource(R.drawable.competitor_marker_grey));
                oldpos.setAnchor(0.5f, 0.5f);
                oldpos.setTitle(competitors.get(dataPoint.getCompetitorId()).getMapMarker().getTitle()
                        + " #" + competitors.get(dataPoint.getCompetitorId()).getLastDataPoint().getSequence());
            }

            /* Add data point to competitor and update the marker position */
            competitors.get(dataPoint.getCompetitorId()).setLastDataPoint(dataPoint);
        }
    }

    public void handleDataPointReplay(RaceTrackerDataPoint dataPoint) {

    }

    /**
     * Class handling the result of the last data point query
     * This query retrieves all the last data points (highest sequence number)
     */
    public class OnLastDataPointResults implements OnQueryResultReady {
        private RaceTrackerQuery results;

        public RaceTrackerQuery getResults() {
            return results;
        }

        /* Callback when competitions are ready */
        public void onQueryResultReady(RaceTrackerQuery results) throws SQLException {
            this.results = results;

            if (results.getException() != null) {
                /* Exception during query */
                Toast.makeText(getApplicationContext(), "Exception during query: " + results.getException().getMessage(), Toast.LENGTH_LONG).show();
            } else {
                while (results.getResult().next()) {
                    RaceTrackerDataPoint dataPoint = new RaceTrackerDataPoint(results.getResult());
                    handleDataPointLive(dataPoint);
                }
            }

            /* Updates UI */
            //mAdapter.notifyDataSetChanged();
        }
    }

    public class OnCountryResults implements OnQueryResultReady {
        /* Callback when countries are ready */
        public void onQueryResultReady(RaceTrackerQuery results) throws SQLException {
            if (results.getException() != null) {
                /* Exception during query */
                Toast.makeText(getApplicationContext(), "Exception during query: " + results.getException().getMessage(), Toast.LENGTH_LONG).show();
            } else {
                while (results.getResult().next()) {
                    RaceTrackerCountry country = new RaceTrackerCountry(results.getResult());
                    countries.put(country.getCountryCode(), country);
                }
            }
        }
    }

    public class OnTrackPointResults implements OnQueryResultReady {
        /* Callback when countries are ready */
        public void onQueryResultReady(RaceTrackerQuery results) throws SQLException {
            if (results.getException() != null) {
                /* Exception during query */
                Toast.makeText(getApplicationContext(), "Exception during query: " + results.getException().getMessage(), Toast.LENGTH_LONG).show();
            } else {
                ArrayList<LatLng> points = new ArrayList<>();
                while (results.getResult().next()) {
                    LatLng point = new LatLng(results.getResult().getDouble("latitude"), results.getResult().getDouble("longitude"));
                    points.add(point);
                }
                track = mMap.addPolyline(new PolylineOptions()
                        .width(5)
                        .color(Color.RED));
                track.setPoints(points);
            }
        }
    }

    public class OnDataPointsResults implements OnQueryResultReady {
        private RaceTrackerQuery results;

        public RaceTrackerQuery getResults() {
            return results;
        }

        /* Callback when competitions are ready */
        public void onQueryResultReady(RaceTrackerQuery results) throws SQLException {
            this.results = results;

            if (results.getException() != null) {
                /* Exception during query */
                Toast.makeText(getApplicationContext(), "Exception during query: " + results.getException().getMessage(), Toast.LENGTH_LONG).show();
            } else {
                while (results.getResult().next()) {
                    RaceTrackerDataPoint dataPoint = new RaceTrackerDataPoint(results.getResult());

                    if (competitors.get(dataPoint.getCompetitorId()).getStartTime() == null) {
                        competitors.get(dataPoint.getCompetitorId()).setStartTime(dataPoint.getTimeStamp());
                    }

                    if (competition.isActive()) {
                        handleDataPointLive(dataPoint);
                    } else {
                        /* In replay mode we simply fill the list */
                        dataPoints.add(dataPoint);
                    }
                }
            }

            /* Updates UI */
            mAdapter.notifyDataSetChanged();

            /* In replay mode start the handler */
            if (!competition.isActive()) {
                /* TODO Start handler for replay mode (set interval with difference from timestamps */

            } else {
                /* Once the marker up until now are set, we can check for new points */
                startDataPointChecker();
            }
        }
    }

    public void setupMarkers() throws SQLException {
        /* Send query to get all data points until now */
        db.getDataPoints(onDataPointsResults, competition.getCompetitionId());
    }

    public class OnCompetitorsResults implements OnQueryResultReady {
        private RaceTrackerQuery results;

        public RaceTrackerQuery getResults() {
            return results;
        }

        /* Callback when competitions are ready */
        public void onQueryResultReady(RaceTrackerQuery results) throws SQLException {
            this.results = results;
            ArrayList<RaceTrackerCompetitor> competitorsList = new ArrayList<>();

            if (results.getException() != null) {
                /* Exception during query */
                Toast.makeText(getApplicationContext(), "Exception during query: " + results.getException().getMessage(), Toast.LENGTH_LONG).show();
            } else {
                while (results.getResult().next()) {
                    RaceTrackerCompetitor competitor = new RaceTrackerCompetitor(results.getResult());
                    competitors.put(competitor.getCompetitorId(), competitor);
                    competitorsList.add(competitor);
                    /* Setup map marker */
                    competitor.setMapMarker(mMap.addMarker(new MarkerOptions()
                            .position(new LatLng(0,0))
                            .title(competitor.getFirstName() + " " + competitor.getLastName())
                            .visible(false)
                            .anchor(0.5f, 0.5f)
                            .icon(BitmapDescriptorFactory.fromResource(R.drawable.competitor_marker_blue))));
                    /* Setup country */
                    competitor.setCountry(countries.get(competitor.getCountryCode()));
                    System.out.println("DBG: Competitor: " + competitor.toString());
                }
            }

            /* Initialize adapter and add it to view */
            mAdapter = new RaceTrackerCompetitorAdapter(competitorsList);
            mRecyclerView.setAdapter(mAdapter);

            /* Updates UI */
            mAdapter.notifyDataSetChanged();

            /* Setup markers of data points */
            setupMarkers();
        }
    }

    /**
     * Handler of the Reset Camera button
     * @param view
     */
    public void onResetCamerabOnClick(View view) {
        resetCamera();
    }

    /**
     * Resets the camera to the DB position and zoom lvl
     */
    public void resetCamera() {
        /* Move camera and zoom to competition position*/
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(competition.getLocation().getObject(), competition.getZoom()));
    }

    /**
     * Start handler which periodically makes queries to the DB
     * to see if a new point is available
     */
    public void startDataPointChecker() {
        handler.postDelayed(dataPointChecker, 0);
    }

    /**
     * Stop new Data Point Checker
     */
    public void stopDataPointChecker() {
        handler.removeCallbacks(dataPointChecker);
    }

    /**
     * Manipulates the map once available.
     * This callback is triggered when the map is ready to be used.
     */
    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;

        /* Change map type */
        mMap.setMapType(GoogleMap.MAP_TYPE_SATELLITE);

        resetCamera();
    }

    Runnable dataPointChecker = new Runnable() {
        @Override
        public void run() {
            try {
                db.getLastDataPoint(onLastDataPointResults, competition.getCompetitionId());
            } catch (SQLException e) {
                e.printStackTrace();
            } finally {
                /* Restart handler */
                handler.postDelayed(dataPointChecker, handlerInterval);
            }
        }
    };
}
