package ch.heigvd.bisel.racetracker.activities;

import android.os.Bundle;

import java.util.ArrayList;
import java.util.Map;

import ch.heigvd.bisel.racetracker.RaceTrackerDataPoint;

public class ReplayRaceActivity extends ViewRaceActivity {
    private Map<Integer, ArrayList<RaceTrackerDataPoint>> dataPoints;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onDataPointsReady(Map<Integer, ArrayList<RaceTrackerDataPoint>> dataPoints) {
        this.dataPoints = dataPoints;




    }
}
