package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;

public interface OnUpdateDone {
    void onInsertDone(int nbItems, SQLException exception);
}
