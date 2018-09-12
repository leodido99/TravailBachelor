package ch.heigvd.bisel.racetracker;

import java.io.Serializable;
import java.sql.ResultSet;
import java.sql.SQLException;

public class RaceTrackerRegistration implements Serializable {
    private RaceTrackerCompetitor competitor;
    private int competitorId;
    private int competitionId;
    private int sensorId;
    private int bibNumber;

    /**
     * Create new instance of registration
     */
    public RaceTrackerRegistration() {

    }

    /**
     * Create new instance of registration
     * @param fromDB Result set from DB
     */
    public RaceTrackerRegistration(ResultSet fromDB) {
        try {
            competitorId = fromDB.getInt("competitor_id");
            competitionId = fromDB.getInt("competition_id");
            sensorId = fromDB.getInt("sensor_id");
            bibNumber = fromDB.getInt("bib_number");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    /**
     * Returns competitor ID
     * @return competitor ID
     */
    public int getCompetitorId() {
        return competitorId;
    }

    /**
     * Set competitor ID
     * @param competitorId competitor ID
     */
    public void setCompetitorId(int competitorId) {
        this.competitorId = competitorId;
    }

    /**
     * Returns competition ID
     * @return Competition ID
     */
    public int getCompetitionId() {
        return competitionId;
    }

    /**
     * Set competition ID
     * @param competitionId Competition ID
     */
    public void setCompetitionId(int competitionId) {
        this.competitionId = competitionId;
    }

    /**
     * Returns sensor ID
     * @return Sensor ID
     */
    public int getSensorId() {
        return sensorId;
    }

    /**
     * Set sensor ID
     * @param sensorId Sensor ID
     */
    public void setSensorId(int sensorId) {
        this.sensorId = sensorId;
    }

    /**
     * Returns Bib Number
     * @return Bib Number
     */
    public int getBibNumber() {
        return bibNumber;
    }

    /**
     * Set Bib number
     * @param bibNumber Bib Number
     */
    public void setBibNumber(int bibNumber) {
        this.bibNumber = bibNumber;
    }

    /**
     * Returns the competitor
     * @return Competitor instance
     */
    public RaceTrackerCompetitor getCompetitor() {
        return competitor;
    }

    /**
     * Set competitor instance
     * @param competitor Competitor instance
     */
    public void setCompetitor(RaceTrackerCompetitor competitor) {
        this.competitor = competitor;
    }
}
