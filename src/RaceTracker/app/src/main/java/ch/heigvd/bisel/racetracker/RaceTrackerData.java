package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.util.ArrayList;

public class RaceTrackerData implements OnQueryResultReady {
    private int competitionId;
    private OnRaceTrackerDataReady callback;

    private RaceTrackerDB raceTrackerDB;
    private ArrayList<RaceTrackerCompetitor> competitors;


    public RaceTrackerData(String serverAddress, int competitionId, OnRaceTrackerDataReady callback) {
        this.competitionId = competitionId;
        this.callback = callback;

        /* Execute DB queries */
        raceTrackerDB = new RaceTrackerDB(serverAddress);


    }

    public ArrayList<RaceTrackerCompetitor> getCompetitors() {
        return competitors;
    }

    public void setCompetitors(ArrayList<RaceTrackerCompetitor> competitors) {
        this.competitors = competitors;
    }

    /**
     * Callback function 
     * @param results
     */
    public void onQueryResultReady(RaceTrackerQuery results) {

    }
}
