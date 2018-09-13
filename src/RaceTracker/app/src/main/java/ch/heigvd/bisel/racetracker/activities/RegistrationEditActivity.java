package ch.heigvd.bisel.racetracker.activities;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitionAdapter;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitor;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitorAdapter;
import ch.heigvd.bisel.racetracker.RaceTrackerRegistration;
import ch.heigvd.bisel.racetracker.RecyclerTouchListener;

public class RegistrationEditActivity extends AppCompatActivity {
    private RecyclerView mRecyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;

    private RaceTrackerRegistration registration;
    private ArrayList<RaceTrackerCompetitor> competitors;
    private RaceTrackerCompetitor selectedCompetitor;

    private boolean newRegistration;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registration_edit);

        newRegistration = getIntent().getBooleanExtra("new_registration", false);
        if (!newRegistration) {
            registration = new RaceTrackerRegistration();
        } else {
            registration = (RaceTrackerRegistration)getIntent().getSerializableExtra("registration");
            ((TextView)findViewById(R.id.RegistrationEditBib)).setText(String.valueOf(registration.getBibNumber()));
            ((TextView)findViewById(R.id.RegistrationEditSensor)).setText(String.valueOf(registration.getSensorId()));
            /* TODO Select in recycler view */
        }

        competitors = (ArrayList<RaceTrackerCompetitor>)getIntent().getSerializableExtra("competitors");

        /* Setup RecyclerView */
        mRecyclerView = findViewById(R.id.RegistrationEditCompetitorList);

        /* use this setting to improve performance if you know that changes
         * in content do not change the layout size of the RecyclerView
         */
        mRecyclerView.setHasFixedSize(true);

        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        /* Initialize adapter and add it to view */
        mAdapter = new RaceTrackerCompetitorAdapter(competitors) {{
            setHideStats(true);
        }};

        mRecyclerView.setAdapter(mAdapter);

        /* Add on touch listener to detect race selection */
        mRecyclerView.addOnItemTouchListener(new RecyclerTouchListener(getApplicationContext(), mRecyclerView, new RecyclerTouchListener.ClickListener() {
            @Override
            public void onClick(View view, int position) {
                selectedCompetitor = competitors.get(position);
            }

            @Override
            public void onLongClick(View view, int position) {

            }
        }));
    }

    public void validate(View view) {
        if (selectedCompetitor == null) {
            Toast.makeText(this, "Vous devez séléctionner un participant", Toast.LENGTH_SHORT).show();
            return;
        }

        if (Integer.valueOf(((EditText)findViewById(R.id.RegistrationEditSensor)).getText().toString())
                < 0 ||
                Integer.valueOf(((EditText)findViewById(R.id.RegistrationEditSensor)).getText().toString()) > 2147483647) {
            Toast.makeText(this, "Numéro de capteur invalide", Toast.LENGTH_SHORT).show();
            return;
        }

        if (Integer.valueOf(((EditText)findViewById(R.id.RegistrationEditBib)).getText().toString())
                < 0 ||
                Integer.valueOf(((EditText)findViewById(R.id.RegistrationEditBib)).getText().toString()) > 32767) {
            Toast.makeText(this, "Numéro de dossard invalide", Toast.LENGTH_SHORT).show();
            return;
        }

        /* Update fields */
        registration.setCompetitorId(selectedCompetitor.getCompetitorId());
        registration.setSensorId(Integer.valueOf(((EditText)findViewById(R.id.RegistrationEditSensor)).getText().toString()));
        registration.setBibNumber(Integer.valueOf(((EditText)findViewById(R.id.RegistrationEditBib)).getText().toString()));
        Intent data = new Intent();
        data.putExtra("registration", registration);
        setResult(RESULT_OK, data);
        finish();
    }

}
