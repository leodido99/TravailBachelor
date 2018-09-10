package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;
import java.util.ArrayList;

public class RaceTrackerCountries implements OnQueryResultReady, OnQueryExecuted {
    private ArrayList<RaceTrackerCountry> countries;
    private RaceTrackerCountries.OnCountriesReady callback;
    private RaceTrackerQuery query;

    /**
     * Create a new instance
     * @param connection Connection to the database
     * @param callback Called when countries are ready to be retrieved
     */
    public RaceTrackerCountries(RaceTrackerDBConnection connection, RaceTrackerCountries.OnCountriesReady callback) {
        this.callback = callback;
        countries = new ArrayList<>();

        /* Prepare and execute query */
        query = new RaceTrackerQuery(connection);
        query.setQuery("SELECT * FROM race_tracker.country;");
        query.setCallbackResultReady(this);
        query.setCallbackExecuted(this);
        query.execute();
    }

    /**
     * Callback executed when the results of the query are ready
     * @param results Results of the query
     * @throws SQLException
     * @throws RuntimeException
     */
    public void onQueryResultReady(RaceTrackerQuery results) throws SQLException, RuntimeException {
        if (results.getException() != null) {
            /* Exception during query */
            throw new RuntimeException(results.getException().getMessage());
        } else {
            while (results.getResult().next()) {
                RaceTrackerCountry country = new RaceTrackerCountry(results.getResult());
                countries.add(country);
            }
        }
    }

    /**
     * Callback executed when query is finished executing
     * @param query Query that was executed
     */
    public void onQueryExecuted(RaceTrackerQuery query) {
        this.callback.onCountriesReady(countries);
    }

    /**
     * Returns the list of countries
     * @return Countries
     */
    public ArrayList<RaceTrackerCountry> getCountries() {
        return countries;
    }

    /**
     * Sets the list of countries
     * @param countries Countries
     */
    public void setCountries(ArrayList<RaceTrackerCountry> countries) {
        this.countries = countries;
    }

    /**
     * Interface called when results are ready
     */
    public interface OnCountriesReady {
        void onCountriesReady(ArrayList<RaceTrackerCountry> countries);
    }
}
