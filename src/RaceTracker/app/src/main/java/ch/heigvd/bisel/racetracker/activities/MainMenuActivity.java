package ch.heigvd.bisel.racetracker.activities;

import android.content.Intent;
import android.preference.PreferenceActivity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;

public class MainMenuActivity extends AppCompatActivity {
    final static int SELECT_RACE = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_menu);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.configure:
                goToSettings();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    /**
     * Select a race
     * @param view
     */
    public void selectRace(View view) {
        Intent intent = new Intent(this, ViewRaceSelectorActivity.class);
        startActivityForResult(intent, SELECT_RACE);
    }

    /**
     * Triggered when user has selected a race
     * @param requestCode Request code
     * @param resultCode Result code
     * @param data Data returned
     */
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == SELECT_RACE) {
            if (resultCode == RESULT_OK) {
                RaceTrackerCompetition competition = (RaceTrackerCompetition)data.getSerializableExtra("competition");
                Intent intent = new Intent(this, ViewRaceActivity.class);
                intent.putExtra("competition", competition);
                startActivity(intent);
            }
        }
    }

    /**
     * go to manage race activity
     * @param view
     */
    public void goToManageRaces(View view) {
        Intent intent = new Intent(this, ManageRaceSelectorActivity.class);
        startActivity(intent);
    }

    /**
     * go to settings
     */
    public void goToSettings() {
        Intent intent = new Intent(this, SettingsActivity.class);
        intent.putExtra(PreferenceActivity.EXTRA_SHOW_FRAGMENT, SettingsActivity.GeneralPreferenceFragment.class.getName());
        intent.putExtra(PreferenceActivity.EXTRA_NO_HEADERS,true);
        startActivity(intent);
    }
}
