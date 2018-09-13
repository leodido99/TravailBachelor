package ch.heigvd.bisel.racetracker.activities;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.sql.SQLException;

import ch.heigvd.bisel.racetracker.OnUpdateDone;
import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitions;
import ch.heigvd.bisel.racetracker.RaceTrackerDBConnection;

public class StartEndRaceActivity extends AppCompatActivity implements OnUpdateDone {
    private RaceTrackerCompetition competition;
    private boolean start;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_end_race);

        /* Retrieve competition class from intent */
        competition = (RaceTrackerCompetition)getIntent().getSerializableExtra("competition");
        ((TextView)findViewById(R.id.StartEndRaceName)).setText(competition.getName());

        Button button = ((Button)findViewById(R.id.StartEndButton));
        if (competition.isActive()) {
            button.setText(getResources().getString(R.string.managemenu_race_start_button_end));
        } else {
            button.setText(getResources().getString(R.string.managemenu_race_start_button_start));
        }
    }

    public void buttonOnClick(View view) {
        /* Create DB connection */
        RaceTrackerDBConnection connection = new RaceTrackerDBConnection(getApplicationContext(),
                getResources().getString(R.string.db_server_key),
                getResources().getString(R.string.db_user_key),
                getResources().getString(R.string.db_pwd_key));

        RaceTrackerCompetitions competitions = new RaceTrackerCompetitions(connection);
        competition.setActive(!competition.isActive());
        competitions.updateRace(competition, this);
    }

    @Override
    public void onInsertDone(int nbItems, SQLException exception) {
        if (exception != null) {
            Toast.makeText(getApplicationContext(), "Impossible de changer l'état de la course "
                    + exception.getMessage(), Toast.LENGTH_LONG).show();
        } else {
            Toast.makeText(getApplicationContext(), "Course mise à jour", Toast.LENGTH_SHORT).show();
        }
    }
}
