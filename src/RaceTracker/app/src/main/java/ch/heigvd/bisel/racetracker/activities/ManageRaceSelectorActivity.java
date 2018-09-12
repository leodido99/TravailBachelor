package ch.heigvd.bisel.racetracker.activities;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;

public class ManageRaceSelectorActivity extends AppCompatActivity {
    final static int SELECT_RACE = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manage_race_selector);
    }

    public void goToCreateRace(View view) {
        Intent intent = new Intent(this, CreateNewRaceActivity.class);
        startActivity(intent);
    }

    public void goToModifyRace(View view) {

    }

    public void goToCreateCompetitor(View view) {
        Intent intent = new Intent(this, CreateNewCompetitorActivity.class);
        startActivity(intent);
    }

    public void goToRegistrations(View view) {
        Intent intent = new Intent(this, ViewRaceSelectorActivity.class);
        startActivityForResult(intent, SELECT_RACE);
    }

    /**
     * Triggered when user has selected a race
     * @param requestCode Request code
     * @param resultCode Result code
     * @param data Data returned
     */
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == SELECT_RACE) {
            if (resultCode == RESULT_OK) {
                RaceTrackerCompetition competition = (RaceTrackerCompetition)data.getSerializableExtra("competition");
                Intent intent = new Intent(this, RegistrationsActivity.class);
                intent.putExtra("competition", competition);
                startActivity(intent);
            }
        }
    }
}
