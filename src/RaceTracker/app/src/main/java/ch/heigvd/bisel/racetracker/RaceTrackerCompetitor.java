package ch.heigvd.bisel.racetracker;

import com.google.android.gms.maps.model.MarkerOptions;

import java.sql.ResultSet;
import java.sql.SQLException;

public class RaceTrackerCompetitor {
    private int competitorId;
    private String countryCode;
    private String firstName;
    private String lastName;
    private int bibNumber;
    private int sensorId;
    private RaceTrackerDataPoint lastDataPoint;
    private MarkerOptions mapMarker;

    /**
     * Construct a RaceTrackerCompetitor from the result of a query
     * @param fromDB Result from DB that contains all the expected columns
     */
    public RaceTrackerCompetitor(ResultSet fromDB) {
        try {
            competitorId = fromDB.getInt("competitor_id");
            countryCode = fromDB.getString("country_code");
            firstName = fromDB.getString("first_name");
            lastName = fromDB.getString("last_name");
            bibNumber = fromDB.getInt("bib_number");
            sensorId = fromDB.getInt("sensor_id");
        } catch (SQLException e) {
            e.printStackTrace();
        }
        /* Initialize marker */
        mapMarker = new MarkerOptions();
        mapMarker.title(firstName + " " + lastName);
        mapMarker.visible(false);
        mapMarker.draggable(false);
    }

    /**
     * Update the competitor position marker using the last data point
     */
    public void updatePositionMarker() {
        mapMarker.position(lastDataPoint.getPosition());
    }

    public String toString() {
        return "id=" + competitorId + " " + firstName + " " + lastName + " from " + countryCode;
    }

    public RaceTrackerDataPoint getLastDataPoint() {
        return lastDataPoint;
    }

    public MarkerOptions getMapMarker() {
        return mapMarker;
    }

    public void setMapMarker(MarkerOptions mapMarker) {
        this.mapMarker = mapMarker;
    }

    public boolean hasLastDataPoint() {
        if (lastDataPoint == null) {
            return false;
        } else {
            return true;
        }
    }

    public void setLastDataPoint(RaceTrackerDataPoint lastDataPoint) {
        this.lastDataPoint = lastDataPoint;
    }

    public int getCompetitorId() {
        return competitorId;
    }

    public void setCompetitorId(int competitorId) {
        this.competitorId = competitorId;
    }

    public String getCountryCode() {
        return countryCode;
    }

    public void setCountryCode(String countryCode) {
        this.countryCode = countryCode;
    }

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public int getBibNumber() {
        return bibNumber;
    }

    public void setBibNumber(int bibNumber) {
        this.bibNumber = bibNumber;
    }

    public int getSensorId() {
        return sensorId;
    }

    public void setSensorId(int sensorId) {
        this.sensorId = sensorId;
    }
}
