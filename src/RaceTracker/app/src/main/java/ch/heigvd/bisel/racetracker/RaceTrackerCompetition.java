package ch.heigvd.bisel.racetracker;

import com.google.android.gms.maps.model.LatLng;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;

public class RaceTrackerCompetition {
    public int competitionId;
    public String name;
    public LatLng location;
    public Timestamp eventDate;
    public boolean active;

    public RaceTrackerCompetition(ResultSet fromDB) {
        try {
            competitionId = fromDB.getInt("competition_id");
            name = fromDB.getString("name");
            location = new LatLng(fromDB.getDouble("lat"), fromDB.getDouble("lon"));
            eventDate = fromDB.getTimestamp("event_date");
            active = fromDB.getBoolean("active");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public String toString() {
        return "id=" + competitionId + " name=" + name + " lat=" + location.latitude + " lon=" +
                location.longitude + " date=" + eventDate.toString() + " active=" + active;
    }
}
