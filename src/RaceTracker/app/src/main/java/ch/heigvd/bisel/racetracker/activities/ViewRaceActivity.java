package ch.heigvd.bisel.racetracker.activities;

import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Handler;
import android.preference.PreferenceManager;
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
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitors;
import ch.heigvd.bisel.racetracker.RaceTrackerCountries;
import ch.heigvd.bisel.racetracker.RaceTrackerCountry;
import ch.heigvd.bisel.racetracker.RaceTrackerDBConnection;
import ch.heigvd.bisel.racetracker.RaceTrackerDataPoint;
import ch.heigvd.bisel.racetracker.RaceTrackerDataPoints;
import ch.heigvd.bisel.racetracker.RaceTrackerQuery;
import ch.heigvd.bisel.racetracker.RaceTrackerTrackPoint;
import ch.heigvd.bisel.racetracker.RaceTrackerTrackPoints;

public class ViewRaceActivity extends AppCompatActivity implements OnMapReadyCallback,
        RaceTrackerCompetitors.OnCompetitorsReady,
        RaceTrackerCountries.OnCountriesReady,
        RaceTrackerDataPoints.OnDataPointsReady,
        RaceTrackerTrackPoints.OnTrackPointsReady {
    private GoogleMap mMap;
    private Polyline track;

    private RaceTrackerDataPoints dataPointsDB;

    private RaceTrackerCompetition competition;
    private Map<Integer, RaceTrackerCompetitor> competitors;
    private Map<String, RaceTrackerCountry> countries;
    private ArrayList<RaceTrackerDataPoint> dataPoints;

    private int handlerInterval = 5000;
    private Handler handler;
    private boolean handlerStarted = false;
    private boolean systemInit = false;

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

        /* Create DB connection */
        RaceTrackerDBConnection connection = new RaceTrackerDBConnection(getApplicationContext(),
                getResources().getString(R.string.db_server_key),
                getResources().getString(R.string.db_user_key),
                getResources().getString(R.string.db_pwd_key));

        /* Retrieve data from DB */
        RaceTrackerCountries countriesDB = new RaceTrackerCountries(connection, this);
        RaceTrackerCompetitors competitorsDB = new RaceTrackerCompetitors(connection,
                this,
                competition.getCompetitionId());
        dataPointsDB = new RaceTrackerDataPoints(connection,
                this,
                competition.getCompetitionId());
        RaceTrackerTrackPoints trackPointsDB = new RaceTrackerTrackPoints(connection,
                this,
                competition.getCompetitionId());

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

    /**
     * Handler of the Reset Camera button
     * @param view
     */
    public void onResetCameraOnClick(View view) {
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
        if (!handlerStarted) {
            handler.postDelayed(dataPointChecker, 0);
            handlerStarted = true;
        }
    }

    /**
     * Stop new Data Point Checker
     */
    public void stopDataPointChecker() {
        handler.removeCallbacks(dataPointChecker);
        handlerStarted = false;
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
                dataPointsDB.getLastPoints();
            } finally {
                /* Restart handler */
                handler.postDelayed(dataPointChecker, handlerInterval);
            }
        }
    };

    /**
     * Triggered when the activity resumes
     */
    @Override
    public void onResume() {
        super.onResume();

        if (systemInit) {
            startDataPointChecker();
        }
    }

    /**
     * Triggered when the activity is put on pause
     */
    @Override
    public void onPause() {
        super.onPause();
        stopDataPointChecker();
    }

    /**
     * Triggered when the back button is pressed
     */
    @Override
    public void onBackPressed() {
        stopDataPointChecker();
        super.onBackPressed();
    }

    /**
     * Triggered when competitor list is ready
     * @param competitors List of competitors
     */
    public void onCompetitorsReady(ArrayList<RaceTrackerCompetitor> competitors) {
        for (RaceTrackerCompetitor competitor : competitors) {
            /* Add competitors to hash map */
            this.competitors.put(competitor.getCompetitorId(), competitor);

            /* Set competitor country */
            competitor.setCountry(countries.get(competitor.getCountryCode()));
        }

        /* Initialize adapter and add it to view */
        mAdapter = new RaceTrackerCompetitorAdapter(competitors);
        mRecyclerView.setAdapter(mAdapter);

        /* Updates UI */
        mAdapter.notifyDataSetChanged();
    }

    /**
     * Triggered when the list of countries is ready
     * @param countries List of countries
     */
    public void onCountriesReady(ArrayList<RaceTrackerCountry> countries) {
        /* Create hash table */
        for (RaceTrackerCountry country : countries) {
            this.countries.put(country.getCountryCode(), country);
        }
    }

    /**
     * Adds a marker to the map
     * @param position Marker position
     * @param icon Marker icon
     * @param label Marker label
     * @return New marker
     */
    public Marker addMarker(LatLng position, int icon, String label) {
        Marker oldPos = mMap.addMarker(new MarkerOptions().position(position));
        oldPos.setIcon(BitmapDescriptorFactory.fromResource(icon));
        oldPos.setAnchor(0.5f, 0.5f);
        oldPos.setTitle(label);
        return oldPos;
    }

    /**
     * Triggered when track points are ready
     * @param trackPoints Track point list
     */
    public void onTrackPointsReady(ArrayList<RaceTrackerTrackPoint> trackPoints) {
        ArrayList<LatLng> trackPositions = new ArrayList<>();

        for (RaceTrackerTrackPoint trackPoint : trackPoints) {
            trackPositions.add(trackPoint.getPosition());
        }

        track = mMap.addPolyline(new PolylineOptions()
                .width(5)
                .color(Color.RED));
        track.setPoints(trackPositions);
    }

    /**
     * Triggered when data points are ready
     * @param dataPoints List of new data points
     */
    public void onDataPointsReady(Map<Integer, ArrayList<RaceTrackerDataPoint>> dataPoints) {
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        Boolean leaveTrail = sharedPref.getBoolean(getResources().getString(R.string.leave_trail_key), false);
        Marker lastMarker = null;
        int cnt = 0;
        boolean consumed;

        /* TODO Replay mode */

        /* Dispatch data points to the competitors */
        for (Map.Entry<Integer, ArrayList<RaceTrackerDataPoint>> entry : dataPoints.entrySet()) {
            consumed = false;

            for (RaceTrackerDataPoint dataPoint : entry.getValue()) {
                consumed = competitors.get(dataPoint.getCompetitorId()).consumeDataPoint(dataPoint);
                if (consumed && leaveTrail) {
                    lastMarker = addMarker(dataPoint.getPosition(), R.drawable.competitor_marker_grey,
                            competitors.get(dataPoint.getCompetitorId()).getFirstName()
                                    + " " + competitors.get(dataPoint.getCompetitorId()).getLastName() + " #"
                                    + dataPoint.getSequence());
                }

                cnt++;
            }

            /* If the competitor consumed a data point that means the last position is going to change */
            if (consumed) {
                /* Update of previous marker if it exists */
                if (competitors.get(entry.getKey()).getMapMarker() != null) {
                    competitors.get(entry.getKey()).getMapMarker().setIcon(BitmapDescriptorFactory.fromResource(R.drawable.competitor_marker_grey));
                    competitors.get(entry.getKey()).getMapMarker().setTitle(competitors.get(entry.getKey()).getMapMarker().getTitle()
                            + " #" + competitors.get(entry.getKey()).getLastDataPoint().getSequence());
                }

                if (lastMarker != null) {
                    /* Last marker becomes the current position */
                    lastMarker.setIcon(BitmapDescriptorFactory.fromResource(R.drawable.competitor_marker_blue));
                    lastMarker.setTitle(competitors.get(entry.getKey()).getFirstName() + " "
                            + competitors.get(entry.getKey()).getLastName());
                    competitors.get(entry.getKey()).setMapMarker(lastMarker);
                } else {
                    /* Create marker for last position */
                    competitors.get(entry.getKey()).setMapMarker(addMarker(competitors.get(entry.getKey()).getLastDataPoint().getPosition(), R.drawable.competitor_marker_blue,
                            competitors.get(entry.getKey()).getFirstName()
                                    + " " + competitors.get(entry.getKey()).getLastName()));
                }
            }
        }

        startDataPointChecker();
        systemInit = true;

        /* Updates UI */
        mAdapter.notifyDataSetChanged();

        System.out.println("DBG: Processed " + cnt + " data points");
    }
}
