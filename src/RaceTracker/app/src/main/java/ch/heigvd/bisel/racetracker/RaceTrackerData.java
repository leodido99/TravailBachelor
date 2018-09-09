package ch.heigvd.bisel.racetracker;

import java.util.ArrayList;

public class RaceTrackerData implements OnQueryResultReady {
    private int competitionId;
    private OnDataReady callback;

    private RaceTrackerDB raceTrackerDB;
    private ArrayList<RaceTrackerCompetitor> competitors;


    public RaceTrackerData(String serverAddress, int competitionId, OnDataReady callback) {
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
        if (results.getException() != null) {
            /* Exception during query */

        } else {
            /*while (results.getResult().next()) {

            }*/
        }
    }
}
