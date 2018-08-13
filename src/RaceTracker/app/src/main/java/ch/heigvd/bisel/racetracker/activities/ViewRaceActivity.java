package ch.heigvd.bisel.racetracker.activities;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerDBGetResult;

import java.sql.*;

public class ViewRaceActivity extends AppCompatActivity implements RaceTrackerDBGetResult {
    private RaceTrackerCompetition competition;

    public void queryResult(ResultSet results) throws SQLException {
        while (results.next()) {
            System.out.println("DBG: " + results.getString(1) + " : " + results.getString(2));
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_race);
        /* Retrieve competition class from intent */
        competition = (RaceTrackerCompetition)getIntent().getSerializableExtra("competition");

        System.out.println("Hello ViewRaceActivity " + competition.toString());


        //RaceTrackerDB db = new RaceTrackerDB(this);
        //db.execute();
    }
}
