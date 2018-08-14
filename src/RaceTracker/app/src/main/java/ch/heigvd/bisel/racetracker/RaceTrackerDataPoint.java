package ch.heigvd.bisel.racetracker;

import com.google.android.gms.maps.model.LatLng;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;

public class RaceTrackerDataPoint {
    private int dataPointId;
    private int competitorId;
    private int competitionId;
    private int sequence;
    private Timestamp timeStamp;
    private LatLng position;
    private int heartRate;
    private int cadence;
    private int nbSatellites;
    private double positionHdop;
    private int status;

    public RaceTrackerDataPoint(ResultSet fromDB) {
        try {
            dataPointId = fromDB.getInt("data_point_id");
            competitorId = fromDB.getInt("competitor_id");
            competitionId = fromDB.getInt("competition_id");
            sequence = fromDB.getInt("sequence");
            timeStamp = fromDB.getTimestamp("time_stamp");
            position = new LatLng(fromDB.getDouble("lat"), fromDB.getDouble("lon"));
            heartRate = fromDB.getInt("heart_rate");
            cadence = fromDB.getInt("cadence");
            nbSatellites = fromDB.getInt("nb_satellites");
            positionHdop = fromDB.getDouble("position_dop");
            status = fromDB.getInt("status");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public int getDataPointId() {
        return dataPointId;
    }

    public void setDataPointId(int dataPointId) {
        this.dataPointId = dataPointId;
    }

    public int getCompetitorId() {
        return competitorId;
    }

    public void setCompetitorId(int competitorId) {
        this.competitorId = competitorId;
    }

    public int getCompetitionId() {
        return competitionId;
    }

    public void setCompetitionId(int competitionId) {
        this.competitionId = competitionId;
    }

    public int getSequence() {
        return sequence;
    }

    public void setSequence(int sequence) {
        this.sequence = sequence;
    }

    public Timestamp getTimeStamp() {
        return timeStamp;
    }

    public void setTimeStamp(Timestamp timeStamp) {
        this.timeStamp = timeStamp;
    }

    public LatLng getPosition() {
        return position;
    }

    public void setPosition(LatLng position) {
        this.position = position;
    }

    public int getHeartRate() {
        return heartRate;
    }

    public void setHeartRate(int heartRate) {
        this.heartRate = heartRate;
    }

    public int getCadence() {
        return cadence;
    }

    public void setCadence(int cadence) {
        this.cadence = cadence;
    }

    public int getNbSatellites() {
        return nbSatellites;
    }

    public void setNbSatellites(int nbSatellites) {
        this.nbSatellites = nbSatellites;
    }

    public double getPositionHdop() {
        return positionHdop;
    }

    public void setPositionHdop(double positionHdop) {
        this.positionHdop = positionHdop;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }
}
