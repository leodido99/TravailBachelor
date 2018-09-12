package ch.heigvd.bisel.racetracker;

import com.google.android.gms.maps.model.LatLng;

import java.sql.ResultSet;
import java.sql.SQLException;

public class RaceTrackerTrackPoint {
    int sequence;
    private LatLng position;

    /**
     * Create a new instance of Track point
     * @param fromDB Result set from a DB query
     */
    public RaceTrackerTrackPoint(ResultSet fromDB) {
        try {
            sequence = fromDB.getInt("sequence");
            position = new LatLng(fromDB.getDouble("latitude"), fromDB.getDouble("longitude"));
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    /**
     * Return sequence number
     * @return Sequence number
     */
    public int getSequence() {
        return sequence;
    }

    /**
     * Set the sequence number
     * @param sequence Sequence number
     */
    public void setSequence(int sequence) {
        this.sequence = sequence;
    }

    /**
     * Return track point position
     * @return Position of track point
     */
    public LatLng getPosition() {
        return position;
    }

    /**
     * Set track point position
     * @param position Position of track point
     */
    public void setPosition(LatLng position) {
        this.position = position;
    }
}
