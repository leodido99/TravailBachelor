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
import android.view.WindowManager;

import com.google.android.gms.maps.CameraUpdate;
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

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitor;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitorAdapter;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitors;
import ch.heigvd.bisel.racetracker.RaceTrackerCountries;
import ch.heigvd.bisel.racetracker.RaceTrackerCountry;
import ch.heigvd.bisel.racetracker.RaceTrackerDBConnection;
import ch.heigvd.bisel.racetracker.RaceTrackerDataPoint;
import ch.heigvd.bisel.racetracker.RaceTrackerDataPoints;
import ch.heigvd.bisel.racetracker.RaceTrackerTrackPoint;
import ch.heigvd.bisel.racetracker.RaceTrackerTrackPoints;
import ch.heigvd.bisel.racetracker.RecyclerTouchListener;

public class ViewRaceActivity extends AppCompatActivity implements OnMapReadyCallback,
        RaceTrackerCompetitors.OnCompetitorsReady,
        RaceTrackerCountries.OnCountriesReady,
        RaceTrackerDataPoints.OnDataPointsReady,
        RaceTrackerTrackPoints.OnTrackPointsReady {
    protected GoogleMap mMap;

    private RaceTrackerDataPoints dataPointsDB;

    private RaceTrackerCompetition competition;
    protected Map<Integer, RaceTrackerCompetitor> competitors;
    protected ArrayList<RaceTrackerCompetitor> competitorsByIndex;
    static final float FOCUSED_ZOOM = 18.0f;
    protected RaceTrackerCompetitor focusedCompetitor;
    private Map<String, RaceTrackerCountry> countries;

    static final int HANDLER_INTERVAL = 5000;
    private Handler handler;
    private boolean handlerStarted = false;
    private boolean systemInit = false;

    private RecyclerView mRecyclerView;
    protected RecyclerView.Adapter mAdapter;

    /**
     * Called upon Activity creation
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_race);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        /* Obtain the SupportMapFragment and get notified when the map is ready to be used. */
        MapFragment mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.mapView);
        mapFragment.getMapAsync(this);

        /* Setup RecyclerView */
        mRecyclerView = findViewById(R.id.CompetitorsRecycler);

        RecyclerView.LayoutManager mLayoutManager = new LinearLayoutManager(this);
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

        /* Add on touch listener to detect competitor selection */
        mRecyclerView.addOnItemTouchListener(new RecyclerTouchListener(getApplicationContext(), mRecyclerView, new RecyclerTouchListener.ClickListener() {
            @Override
            public void onClick(View view, int position) {
                if (position< 0) {
                    return;
                }
                RaceTrackerCompetitor prev = focusedCompetitor;
                focusedCompetitor = competitorsByIndex.get(position);
                if (focusedCompetitor.getMapMarker() != null) {
                    mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(focusedCompetitor.getMapMarker().getPosition(), FOCUSED_ZOOM));
                } else {
                    focusedCompetitor = prev;
                }
            }

            @Override
            public void onLongClick(View view, int position) {

            }
        }));
    }

    /**
     * Handler of the Reset Camera button
     * @param view
     */
    public void onResetCameraOnClick(View view) {
        focusedCompetitor = null;
        resetCamera();
    }

    /**
     * Resets the camera to the DB position and zoom lvl
     */
    public void resetCamera() {
        /* Move camera and zoom to competition position*/
        CameraUpdate competitionLocation = CameraUpdateFactory.newLatLngZoom(
                competition.getLocation().getObject(), competition.getZoom());
        mMap.animateCamera(competitionLocation);
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

    /**
     * Runnable used to periodically query data points
     */
    Runnable dataPointChecker = new Runnable() {
        @Override
        public void run() {
            try {
                dataPointsDB.getLastPoints();
            } finally {
                /* Restart handler */
                handler.postDelayed(dataPointChecker, HANDLER_INTERVAL);
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
        competitorsByIndex = competitors;
        for (RaceTrackerCompetitor competitor : competitors) {
            /* Add competitors to hash map */
            this.competitors.put(competitor.getCompetitorId(), competitor);

            /* Set competitor country */
            competitor.setCountry(countries.get(competitor.getCountryCode()));
        }

        /* Initialize adapter and add it to view */
        mAdapter = new RaceTrackerCompetitorAdapter(competitors) {{
                setFocusEnable(false);
        }};
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

        Polyline track = mMap.addPolyline(new PolylineOptions()
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

        /* Dispatch data points to the competitors */
        for (Map.Entry<Integer, ArrayList<RaceTrackerDataPoint>> entry : dataPoints.entrySet()) {
            consumed = false;

            for (RaceTrackerDataPoint dataPoint : entry.getValue()) {
                consumed = competitors.get(dataPoint.getCompetitorId()).consumeDataPoint(dataPoint);
                if (consumed && leaveTrail) {
                    /* Notify update UI */
                    mAdapter.notifyItemRangeChanged(competitorsByIndex.indexOf(competitors.get(dataPoint.getCompetitorId())), 1);
                    if (leaveTrail) {
                        lastMarker = addMarker(dataPoint.getPosition(), R.drawable.competitor_marker_grey,
                                competitors.get(dataPoint.getCompetitorId()).getFirstName()
                                        + " " + competitors.get(dataPoint.getCompetitorId()).getLastName() + " #"
                                        + dataPoint.getSequence());
                    }
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

                /* If the competitor is focused then update camera */
                if (focusedCompetitor == competitors.get(entry.getKey())) {
                    CameraUpdate competitionLocation = CameraUpdateFactory.newLatLng(
                            competitors.get(entry.getKey()).getMapMarker().getPosition());
                    mMap.animateCamera(competitionLocation);
                }
            }
        }

        startDataPointChecker();
        systemInit = true;

        System.out.println("DBG: Processed " + cnt + " data points");
    }
}
