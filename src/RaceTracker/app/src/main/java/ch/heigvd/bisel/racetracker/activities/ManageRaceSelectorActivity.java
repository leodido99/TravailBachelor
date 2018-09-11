package ch.heigvd.bisel.racetracker.activities;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import ch.heigvd.bisel.racetracker.R;

public class ManageRaceSelectorActivity extends AppCompatActivity {

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

    public void goToCreateCompetition(View view) {

    }

    public void goToCreateCompetitor(View view) {
        Intent intent = new Intent(this, CreateNewCompetitorActivity.class);
        startActivity(intent);
    }

    public void goToAssignCompetitor(View view) {

    }
}
