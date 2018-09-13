package ch.heigvd.bisel.racetracker.activities;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;

public class ManageRaceSelectorActivity extends AppCompatActivity {
    final static int SELECT_RACE_REGISTRATION = 0;
    final static int SELECT_RACE_START_END = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manage_race_selector);
    }

    public void goToCreateRace(View view) {
        Intent intent = new Intent(this, CreateNewRaceActivity.class);
        startActivity(intent);
    }

    public void goToStartEndRace(View view) {
        Intent intent = new Intent(this, ViewRaceSelectorActivity.class);
        startActivityForResult(intent, SELECT_RACE_START_END);
    }

    public void goToCreateCompetitor(View view) {
        Intent intent = new Intent(this, CreateNewCompetitorActivity.class);
        startActivity(intent);
    }

    public void goToRegistrations(View view) {
        Intent intent = new Intent(this, ViewRaceSelectorActivity.class);
        startActivityForResult(intent, SELECT_RACE_REGISTRATION);
    }

    /**
     * Triggered when user has selected a race
     * @param requestCode Request code
     * @param resultCode Result code
     * @param data Data returned
     */
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Intent intent = null;

        if (resultCode == RESULT_OK) {
            RaceTrackerCompetition competition = (RaceTrackerCompetition)data.getSerializableExtra("competition");
            if (requestCode == SELECT_RACE_REGISTRATION) {
                intent = new Intent(this, RegistrationsActivity.class);
            } else if (requestCode == SELECT_RACE_START_END) {
                intent = new Intent(this, StartEndRaceActivity.class);
            } else {
                throw new RuntimeException("Unknown selection");
            }

            intent.putExtra("competition", competition);
            startActivity(intent);
        }
    }

}
