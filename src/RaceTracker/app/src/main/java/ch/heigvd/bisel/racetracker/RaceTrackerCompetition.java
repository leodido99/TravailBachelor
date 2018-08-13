package ch.heigvd.bisel.racetracker;

import com.google.android.gms.maps.model.LatLng;

import java.io.Serializable;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;

public class RaceTrackerCompetition implements Serializable {
    public int competitionId;
    public String name;
    public LatLngWrapper location;
    public Timestamp eventDate;
    public boolean active;

    public RaceTrackerCompetition(ResultSet fromDB) {
        try {
            competitionId = fromDB.getInt("competition_id");
            name = fromDB.getString("name");
            location = new LatLngWrapper(fromDB.getDouble("lat"), fromDB.getDouble("lon"));
            eventDate = fromDB.getTimestamp("event_date");
            active = fromDB.getBoolean("active");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public String toString() {
        return "id=" + competitionId + " name=" + name + " lat=" + location.getObject().latitude + " lon=" +
                location.getObject().longitude + " date=" + eventDate.toString() + " active=" + active;
    }
}
