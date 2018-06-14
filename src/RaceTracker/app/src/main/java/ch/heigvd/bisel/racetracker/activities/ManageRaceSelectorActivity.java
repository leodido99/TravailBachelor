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

    public void onClickCreateRace(View view) {
        Intent intent = new Intent(this, CreateNewRaceActivity.class);
        /*EditText editText = (EditText) findViewById(R.id.editText);
        String message = editText.getText().toString();
        intent.putExtra(EXTRA_MESSAGE, message);*/
        startActivity(intent);
    }

    public void onClickModifyRace(View view) {

    }

    public void onClickCreateCompetitor(View view) {

    }

}