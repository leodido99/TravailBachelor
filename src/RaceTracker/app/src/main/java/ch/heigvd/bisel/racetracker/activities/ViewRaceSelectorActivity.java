package ch.heigvd.bisel.racetracker.activities;

import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.Toast;

import java.sql.SQLException;
import java.util.ArrayList;

import ch.heigvd.bisel.racetracker.OnDataReady;
import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitionAdapter;
import ch.heigvd.bisel.racetracker.OnQueryResultReady;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitions;
import ch.heigvd.bisel.racetracker.RaceTrackerDB;
import ch.heigvd.bisel.racetracker.RaceTrackerDBConnection;
import ch.heigvd.bisel.racetracker.RaceTrackerQuery;
import ch.heigvd.bisel.racetracker.RecyclerTouchListener;

public class ViewRaceSelectorActivity extends AppCompatActivity implements OnDataReady {
    private RecyclerView mRecyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;
    private RaceTrackerCompetitions competitions;

    /**
     * Triggered when competitions are ready
     */
    public void onDataReady() {
        /* Initialize adapter and add it to view */
        mAdapter = new RaceTrackerCompetitionAdapter(competitions.getCompetitions());
        mRecyclerView.setAdapter(mAdapter);

        /* Updates UI */
        mAdapter.notifyDataSetChanged();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_race_selector);

        /* Retrieve Competitions list */
        RaceTrackerDBConnection connection = new RaceTrackerDBConnection(getApplicationContext(),
                getResources().getString(R.string.db_server_key),
                getResources().getString(R.string.db_user_key),
                getResources().getString(R.string.db_pwd_key));
        competitions = new RaceTrackerCompetitions(connection, this);

        /* Setup RecyclerView */
        mRecyclerView = findViewById(R.id.ViewRaceSelectorRecycler);

        /* use this setting to improve performance if you know that changes
         * in content do not change the layout size of the RecyclerView
         */
        mRecyclerView.setHasFixedSize(true);

        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        /* Add line between items */
        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, LinearLayoutManager.VERTICAL));

        /* Add on touch listener to detect race selection */
        mRecyclerView.addOnItemTouchListener(new RecyclerTouchListener(getApplicationContext(), mRecyclerView, new RecyclerTouchListener.ClickListener() {
            @Override
            public void onClick(View view, int position) {
                //Movie movie = movieList.get(position);
                RaceTrackerCompetition competition = competitions.getCompetitions().get(position);
                /* Add competition class to intent so next activity can retrieve it */
                Intent intent = new Intent(view.getContext(), ViewRaceActivity.class);
                intent.putExtra("competition", competition);
                startActivity(intent);
            }

            @Override
            public void onLongClick(View view, int position) {

            }
        }));
    }
}
