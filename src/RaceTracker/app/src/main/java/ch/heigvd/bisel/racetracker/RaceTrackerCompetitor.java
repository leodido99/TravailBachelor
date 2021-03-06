package ch.heigvd.bisel.racetracker;

import android.location.Location;
import android.os.Parcelable;

import com.google.android.gms.maps.model.Marker;

import java.io.Serializable;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.GregorianCalendar;

public class RaceTrackerCompetitor implements Serializable {
    private int competitorId;
    private String countryCode;
    private String firstName;
    private String lastName;
    private int bibNumber;
    private int sensorId;

    private Marker mapMarker;
    private float distance;
    private float speed;
    private int elapsedTimeH;
    private int elapsedTimeM;
    private int elapsedTimeS;
    private int currHeartRate;
    private int currCadence;
    private Timestamp startTime;
    private RaceTrackerCountry country;

    private ArrayList<RaceTrackerDataPoint> dataPoints;
    private RaceTrackerDataPoint prevLastDataPoint;
    private RaceTrackerDataPoint lastDataPoint;

    /**
     * Creates a new empty instance of RaceTrackerCompetitor
     */
    public RaceTrackerCompetitor() {
        dataPoints = new ArrayList<>();
    }

    /**
     * Construct a RaceTrackerCompetitor from the result of a query
     * @param fromDB Result from DB that contains all the expected columns
     */
    public RaceTrackerCompetitor(ResultSet fromDB) {
        this();
        try {
            competitorId = fromDB.getInt("competitor_id");
            countryCode = fromDB.getString("country_code");
            firstName = fromDB.getString("first_name");
            lastName = fromDB.getString("last_name");
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            bibNumber = fromDB.getInt("bib_number");
            sensorId = fromDB.getInt("sensor_id");
        } catch (SQLException e) {
            /* Assume that Bib and Sensor IDs are not part of resultSet */
            bibNumber = 0;
            sensorId = 0;
        }
            distance = 0.0f;
            speed = 0.0f;
            elapsedTimeH = 0;
            elapsedTimeM = 0;
            elapsedTimeS = 0;
            currHeartRate = 0;
            currCadence = 0;
    }

    /**
     * Update the competitor using the last data point
     */
    public void update() {
        if (lastDataPoint == null) {
            return;
        }

        //mapMarker.setPosition(lastDataPoint.getPosition());
        //mapMarker.setVisible(true);
        currHeartRate = lastDataPoint.getHeartRate();
        currCadence = lastDataPoint.getCadence();

        updateElapsedTime();
        updateDistance();
        updateAvgSpeed();
    }

    /**
     * Update the Elapsed time
     */
    public void updateElapsedTime() {
        if (startTime != null && hasLastDataPoint()) {
            /* Calculate Elapsed time */
            long elapsedMs = lastDataPoint.getTimeStamp().getTime() - startTime.getTime();
            int elapsedSeconds = (int)elapsedMs / 1000;
            elapsedTimeH = elapsedSeconds / 3600;
            elapsedTimeM = (elapsedSeconds % 3600) / 60;
            elapsedTimeS = (elapsedSeconds % 3600) % 60;
        }
    }

    public void updateDistance() {
        if (prevLastDataPoint != null && lastDataPoint != null) {
            float[] dist = new float[1];
            Location.distanceBetween(prevLastDataPoint.getPosition().latitude,
                                     prevLastDataPoint.getPosition().longitude,
                                     lastDataPoint.getPosition().latitude,
                                     lastDataPoint.getPosition().longitude,
                                     dist);
            distance += dist[0] / 1000;
        }
    }

    public void updateAvgSpeed() {
        /* Speed in min per km */
        float totalMin = ((elapsedTimeH * 60) + elapsedTimeM + (elapsedTimeS / 60));
        if (totalMin > 0) {
            speed = totalMin / distance;
        } else {
            speed = 0;
        }
    }

    public String toString() {
        return "id=" + competitorId + " " + firstName + " " + lastName + " from " + countryCode;
    }

    public RaceTrackerDataPoint getLastDataPoint() {
        return lastDataPoint;
    }

    public boolean hasLastDataPoint() {
        if (lastDataPoint == null) {
            return false;
        } else {
            return true;
        }
    }

    public boolean hasMarker() {
        if (mapMarker == null) {
            return false;
        } else {
            return true;
        }
    }

    public void setLastDataPoint(RaceTrackerDataPoint lastDataPoint) {
        this.prevLastDataPoint = this.lastDataPoint;
        this.lastDataPoint = lastDataPoint;
        update();
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

    public Marker getMapMarker() {
        return mapMarker;
    }

    public void setMapMarker(Marker mapMarker) {
        this.mapMarker = mapMarker;
    }

    public float getDistance() {
        return distance;
    }

    public void setDistance(float distance) {
        this.distance = distance;
    }

    public float getSpeed() {
        return speed;
    }

    public void setSpeed(float speed) {
        this.speed = speed;
    }

    public int getCurrHeartRate() {
        return currHeartRate;
    }

    public void setCurrHeartRate(int currHeartRate) {
        this.currHeartRate = currHeartRate;
    }

    public int getElapsedTimeH() {
        return elapsedTimeH;
    }

    public void setElapsedTimeH(int elapsedTimeH) {
        this.elapsedTimeH = elapsedTimeH;
    }

    public int getElapsedTimeM() {
        return elapsedTimeM;
    }

    public void setElapsedTimeM(int elapsedTimeM) {
        this.elapsedTimeM = elapsedTimeM;
    }

    public int getElapsedTimeS() {
        return elapsedTimeS;
    }

    public void setElapsedTimeS(int elapsedTimeS) {
        this.elapsedTimeS = elapsedTimeS;
    }

    public Timestamp getStartTime() {
        return startTime;
    }

    public void setStartTime(Timestamp startTime) {
        this.startTime = startTime;
    }

    public RaceTrackerCountry getCountry() {
        return country;
    }

    public void setCountry(RaceTrackerCountry country) {
        this.country = country;
    }

    public int getCurrCadence() {
        return currCadence;
    }

    public void setCurrCadence(int currCadence) {
        this.currCadence = currCadence;
    }

    public boolean consumeDataPoint(RaceTrackerDataPoint dataPoint) {
        if (lastDataPoint != null) {
            /* If the data point is the same than the last one, silently skip */
            if (lastDataPoint.getSequence() == dataPoint.getSequence()) {
                return false;
            }

            /* Check sequence */
            if (dataPoint.getSequence() < lastDataPoint.getSequence()) {
                /* New data point is out of sequence */
                throw new RuntimeException("Out of sequence data point!");
            }
        }

        if (startTime == null) {
            startTime = dataPoint.getTimeStamp();
        }

        if (lastDataPoint != null) {
            prevLastDataPoint = lastDataPoint;
        }

        lastDataPoint = dataPoint;
        update();

        return true;
    }
}
