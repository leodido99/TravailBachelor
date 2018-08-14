package ch.heigvd.bisel.racetracker;

import java.sql.ResultSet;
import java.sql.SQLException;

public interface RaceTrackerDBGetResult {
    void onQueryResultReady(ResultSet results) throws SQLException;
}
