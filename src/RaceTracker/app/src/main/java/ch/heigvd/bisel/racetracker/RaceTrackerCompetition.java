package ch.heigvd.bisel.racetracker;

import java.io.Serializable;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;

public class RaceTrackerCompetition implements Serializable {
    private int competitionId;
    private String name;
    private LatLngWrapper location;
    private Timestamp eventDate;
    private boolean active;

    /**
     * Construct a RaceTrackerCompetition from the result of a query
     * @param fromDB Result from DB that contains all the expected columns
     */
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

    public int getCompetitionId() {
        return competitionId;
    }

    public void setCompetitionId(int competitionId) {
        this.competitionId = competitionId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public LatLngWrapper getLocation() {
        return location;
    }

    public void setLocation(LatLngWrapper location) {
        this.location = location;
    }

    public Timestamp getEventDate() {
        return eventDate;
    }

    public void setEventDate(Timestamp eventDate) {
        this.eventDate = eventDate;
    }

    public boolean isActive() {
        return active;
    }

    public void setActive(boolean active) {
        this.active = active;
    }
}
