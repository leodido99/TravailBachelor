package ch.heigvd.bisel.racetracker;

import java.sql.SQLException;

public interface OnUpdateDone {
    void onUpdateDone(int nbItems, SQLException exception);
}
