package ch.heigvd.bisel.racetracker;

import java.sql.ResultSet;
import java.sql.SQLException;

public interface RaceTrackerDBGetResult {
    public void getResult(ResultSet results) throws SQLException;
}
