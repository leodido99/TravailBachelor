package ch.heigvd.bisel.racetracker.activities;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import java.util.ArrayList;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitor;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitors;
import ch.heigvd.bisel.racetracker.RaceTrackerCountries;
import ch.heigvd.bisel.racetracker.RaceTrackerCountry;
import ch.heigvd.bisel.racetracker.RaceTrackerCountryAdapter;
import ch.heigvd.bisel.racetracker.RaceTrackerDBConnection;
import ch.heigvd.bisel.racetracker.RecyclerTouchListener;

public class CreateNewCompetitorActivity extends AppCompatActivity implements RaceTrackerCountries.OnCountriesReady {
    private RecyclerView mRecyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;
    private ArrayList<RaceTrackerCountry> countries;
    private RaceTrackerCountry competitorCountry;
    private RaceTrackerDBConnection connection;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_new_competitor);

        /* Setup RecyclerView */
        mRecyclerView = findViewById(R.id.CompetitorCountries);

        /* use this setting to improve performance if you know that changes
         * in content do not change the layout size of the RecyclerView
         */
        mRecyclerView.setHasFixedSize(true);

        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        /* Add on touch listener to detect race selection */
        mRecyclerView.addOnItemTouchListener(new RecyclerTouchListener(getApplicationContext(), mRecyclerView, new RecyclerTouchListener.ClickListener() {
            @Override
            public void onClick(View view, int position) {
                competitorCountry = countries.get(position);
            }

            @Override
            public void onLongClick(View view, int position) {

            }
        }));

        /* Get list of countries */
        connection = new RaceTrackerDBConnection(getApplicationContext(),
                getResources().getString(R.string.db_server_key),
                getResources().getString(R.string.db_user_key),
                getResources().getString(R.string.db_pwd_key));
        RaceTrackerCountries countries = new RaceTrackerCountries(connection, this);
    }

    @Override
    public void onCountriesReady(ArrayList<RaceTrackerCountry> countries) {
        this.countries = countries;
        /* Initialize adapter and add it to view */
        mAdapter = new RaceTrackerCountryAdapter(countries);
        mRecyclerView.setAdapter(mAdapter);

        /* Updates UI */
        mAdapter.notifyDataSetChanged();
    }

    public void createNewCompetitor(View view) {
        if (competitorCountry == null) {
            Toast.makeText(this, "Le pays du participant n'a pas été choisi", Toast.LENGTH_SHORT).show();
            return;
        }

        RaceTrackerCompetitor competitor = new RaceTrackerCompetitor();
        competitor.setFirstName(((EditText)findViewById(R.id.competitorFirstName)).getText().toString());
        competitor.setLastName(((EditText)findViewById(R.id.competitorLastName)).getText().toString());
        competitor.setCountryCode(competitorCountry.getCountryCode());
        RaceTrackerCompetitors competitors = new RaceTrackerCompetitors(connection);
        competitors.insertNewCompetitor(competitor);
        /* TODO Check insert is OK */
        finish();
    }
}
