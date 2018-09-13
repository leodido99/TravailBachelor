package ch.heigvd.bisel.racetracker.activities;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import ch.heigvd.bisel.racetracker.OnUpdateDone;
import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitor;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitors;
import ch.heigvd.bisel.racetracker.RaceTrackerDBConnection;
import ch.heigvd.bisel.racetracker.RaceTrackerRegistration;
import ch.heigvd.bisel.racetracker.RaceTrackerRegistrationAdapter;
import ch.heigvd.bisel.racetracker.RaceTrackerRegistrations;

public class RegistrationsActivity extends AppCompatActivity implements
        RaceTrackerRegistrations.OnRegistrationsReady,
        RaceTrackerCompetitors.OnCompetitorsReady,
        OnUpdateDone {
    final static int NEW_REGISTRATION = 0;
    final static int EDIT_REGISTRATION = 0;
    private RaceTrackerCompetition competition;
    private Map<Integer, RaceTrackerCompetitor> competitorsMap;
    private ArrayList<RaceTrackerCompetitor> competitors;
    private ArrayList<RaceTrackerRegistration> registrations;

    private RecyclerView mRecyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;

    private RaceTrackerDBConnection connection;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registrations);

        /* Retrieve competition class from intent */
        competition = (RaceTrackerCompetition)getIntent().getSerializableExtra("competition");
        ((TextView)findViewById(R.id.RaceName)).setText("Inscriptions pour " + competition.getName());

        competitorsMap = new HashMap<>();

        /* Setup RecyclerView */
        mRecyclerView = findViewById(R.id.RaceRegistrations);

        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        /* Add line between items */
        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, LinearLayoutManager.VERTICAL));

        /* Create DB connection */
        connection = new RaceTrackerDBConnection(getApplicationContext(),
                getResources().getString(R.string.db_server_key),
                getResources().getString(R.string.db_user_key),
                getResources().getString(R.string.db_pwd_key));
        /* Retrieve data from DB */
        /* TODO We need ALL competitors, not only the ones from a certain competition */
        RaceTrackerCompetitors competitors = new RaceTrackerCompetitors(connection,
                this);
        RaceTrackerRegistrations registration = new RaceTrackerRegistrations(connection,
                this, competition.getCompetitionId());
    }

    /**
     * Triggered when button to add a new registration is pressed
     * @param view
     */
    public void createNewRegistration(View view) {
        Intent intent = new Intent(this, RegistrationEditActivity.class);
        intent.putExtra("competitors", competitors);
        intent.putExtra("new_registration", false);
        startActivityForResult(intent, NEW_REGISTRATION);
    }

    /**
     * Triggered when competitor list is ready
     * @param competitors List of competitorsMap
     */
    public void onCompetitorsReady(ArrayList<RaceTrackerCompetitor> competitors) {
        this.competitors = competitors;
        for (RaceTrackerCompetitor competitor : competitors) {
            /* Add competitorsMap to hash map */
            this.competitorsMap.put(competitor.getCompetitorId(), competitor);
        }
    }

    /**
     * Triggered when registrations list is ready
     * @param registrations List of registrations
     */
    @Override
    public void onRegistrationsReady(ArrayList<RaceTrackerRegistration> registrations) {
        for (RaceTrackerRegistration registration : registrations) {
            registration.setCompetitor(competitorsMap.get(registration.getCompetitorId()));
        }

        this.registrations = registrations;

        /* Initialize adapter and add it to view */
        mAdapter = new RaceTrackerRegistrationAdapter(this.registrations);
        mRecyclerView.setAdapter(mAdapter);

        mAdapter.notifyDataSetChanged();
    }

    /**
     * Triggered when user has selected a race
     * @param requestCode Request code
     * @param resultCode Result code
     * @param data Data returned
     */
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == NEW_REGISTRATION) {
            if (resultCode == RESULT_OK) {
                RaceTrackerRegistration registration = (RaceTrackerRegistration)data.getSerializableExtra("registration");
                registration.setCompetitionId(competition.getCompetitionId());
                /* Write to DB */
                RaceTrackerRegistrations registrations = new RaceTrackerRegistrations(connection);
                registrations.insertNewRegistration(registration, this);
                /* Add the new registration to the list and update GUI */
                registration.setCompetitor(competitorsMap.get(registration.getCompetitorId()));
                this.registrations.add(registration);
                mAdapter.notifyDataSetChanged();
            }
        }
    }

    /**
     * Triggered on insertion finished
     * @param nbItems Number of items updated
     * @param exception Exception if one occurred, otherwise null
     */
    @Override
    public void onInsertDone(int nbItems, SQLException exception) {
        if (exception != null) {
            Toast.makeText(getApplicationContext(), "Impossible d'ajouter le participant "
                    + exception.getMessage(), Toast.LENGTH_LONG).show();
        } else {
            Toast.makeText(getApplicationContext(), "Ajouté " + nbItems + " participant", Toast.LENGTH_SHORT).show();
        }
    }
}
