package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;

public interface OnQueryResultReady {
    void onQueryResultReady(RaceTrackerQuery results) throws SQLException;
}
