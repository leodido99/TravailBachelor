package ch.heigvd.bisel.racetracker;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * Class passed which contains all data necessary to execute
 * query
 */
public class RaceTrackerQuery {
    private Connection connection;
    private String query;
    private SQLException exception;
    private ResultSet result;
    private OnQueryResultReady callback;

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

    public OnQueryResultReady getCallback() {
        return callback;
    }

    public void setCallback(OnQueryResultReady callback) {
        this.callback = callback;
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

    public void close() throws SQLException {
        result.getStatement().close();
        result.close();
        //connection.close();
    }
}
