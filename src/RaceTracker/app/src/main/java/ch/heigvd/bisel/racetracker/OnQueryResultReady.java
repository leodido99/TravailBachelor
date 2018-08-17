package ch.heigvd.bisel.racetracker;

import java.sql.ResultSet;
import java.sql.SQLException;

public interface OnQueryResultReady {
    void onQueryResultReady(RaceTrackerDB.RaceTrackerQuery results) throws SQLException;
}
