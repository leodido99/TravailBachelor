package ch.heigvd.bisel.racetracker;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

/* TODO Clean-up */
/**
 * Class passed which contains all data necessary to execute
 * query
 */
public class RaceTrackerQuery {
    private int id;
    private Connection connection;
    private String query;
    private SQLException exception;
    private ResultSet result;
    private OnQueryResultReady callbackResultReady;
    private OnQueryExecuted callbackExecuted;
    private RaceTrackerExecuteQuery asyncQuery;
    private RaceTrackerExecuteUpdate asynchUpdate;
    private boolean isUpdateQuery;
    private int nbUpdated;

    public RaceTrackerQuery() {
        isUpdateQuery = false;
    }

    public RaceTrackerQuery(RaceTrackerDBConnection connection) {
        this();
        asyncQuery = new RaceTrackerExecuteQuery(connection.getConnection(), connection.getUser(), connection.getPwd());
        asynchUpdate =  new RaceTrackerExecuteUpdate(connection.getConnection(), connection.getUser(), connection.getPwd());
    }

    public ResultSet getResult() {
        return result;
    }

    public void setResult(ResultSet result) {
        this.result = result;
    }

    public String getQuery() {
        return query;
    }

    public void setQuery(String query) {
        this.query = query;
    }

    public OnQueryResultReady getCallbackResultReady() {
        return callbackResultReady;
    }

    public void setCallbackResultReady(OnQueryResultReady callbackResultReady) {
        this.callbackResultReady = callbackResultReady;
    }

    public SQLException getException() {
        return exception;
    }

    public void setException(SQLException exception) {
        this.exception = exception;
    }

    public Connection getConnection() {
        return connection;
    }

    public void setConnection(Connection connection) {
        this.connection = connection;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public OnQueryExecuted getCallbackExecuted() {
        return callbackExecuted;
    }

    public void setCallbackExecuted(OnQueryExecuted callbackExecuted) {
        this.callbackExecuted = callbackExecuted;
    }

    public void execute() {
        if (isUpdateQuery) {
            asynchUpdate.execute(this);
        } else {
            asyncQuery.execute(this);
        }
    }

    public boolean isUpdateQuery() {
        return isUpdateQuery;
    }

    public void setUpdateQuery(boolean updateQuery) {
        isUpdateQuery = updateQuery;
    }

    public int getNbUpdated() {
        return nbUpdated;
    }

    public void setNbUpdated(int nbUpdated) {
        this.nbUpdated = nbUpdated;
    }
}
