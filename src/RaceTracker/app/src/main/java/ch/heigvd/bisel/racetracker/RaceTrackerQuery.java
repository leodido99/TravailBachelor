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
    private RaceTrackerDBAsyncTask async;

    public RaceTrackerQuery() {

    }

    public RaceTrackerQuery(RaceTrackerDBConnection connection) {
        async = new RaceTrackerDBAsyncTask(connection.getConnection(), connection.getUser(), connection.getPwd());
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

    public void close() throws SQLException {
        result.getStatement().close();
        result.close();
        //connection.close();
    }

    public void execute() {
        async.execute(this);
    }
}
