package ch.heigvd.bisel.racetracker.activities;

import android.app.DatePickerDialog;

import java.util.Date;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.GregorianCalendar;

import android.app.TimePickerDialog;
import android.content.Intent;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import ch.heigvd.bisel.racetracker.LatLngWrapper;
import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitions;
import ch.heigvd.bisel.racetracker.RaceTrackerDBConnection;
import ch.heigvd.bisel.racetracker.fragments.DatePickerFragment;
import ch.heigvd.bisel.racetracker.fragments.TimePickerFragment;

public class CreateNewRaceActivity extends AppCompatActivity implements
        DatePickerDialog.OnDateSetListener,
        TimePickerDialog.OnTimeSetListener {
    int PLACE_PICKER_REQUEST = 1;
    private Calendar dateTime;
    private LatLngWrapper raceLocation;
    private float zoom;

    /**
     * On creation of the activity
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_new_race);
        dateTime = new GregorianCalendar();
    }

    /**
     * Called on button pick date clicked
     * @param view
     */
    public void pickRaceDate(View view) {
        DialogFragment datePicker = new DatePickerFragment();
        datePicker.show(getSupportFragmentManager(), "datePicker");
    }

    /**
     * Called on button pick race time clicked
     * @param view
     */
    public void pickRaceTime(View view) {
        DialogFragment timePicker = new TimePickerFragment();
        timePicker.show(getSupportFragmentManager(), "timePicker");
    }

    /**
     * Called when date is validated
     * @param view
     * @param year
     * @param month
     * @param day
     */
    public void onDateSet(DatePicker view, int year, int month, int day) {
        //Calendar cal = new GregorianCalendar(year, month, day);
        dateTime.set(year, month, day);
        setDate(dateTime);
    }

    /**
     * Set date in text view
     * @param calendar
     */
    private void setDate(final Calendar calendar) {
        final DateFormat dateFormat = DateFormat.getDateInstance(DateFormat.MEDIUM);
        ((TextView) findViewById(R.id.showRaceDate)).setText(dateFormat.format(calendar.getTime()));
    }

    /**
     * Set time in text view
     * @param calendar
     */
    private void setTime(final Calendar calendar) {
        final DateFormat timeFormat = DateFormat.getTimeInstance(DateFormat.MEDIUM);
        ((TextView) findViewById(R.id.showRaceTime)).setText(timeFormat.format(calendar.getTime()));
    }

    /**
     * Called when time is validated
     * @param view
     * @param hourOfDay Hour of the day
     * @param minute Minute
     */
    public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
        //Calendar cal = new GregorianCalendar(0, 0,0, hourOfDay, minute);
        dateTime.set(Calendar.HOUR, hourOfDay);
        dateTime.set(Calendar.MINUTE, minute);
        setTime(dateTime);
    }

    /**
     * Called when selectLocation button is clicked
     * @param view
     */
    public void selectLocation(View view) {
        Intent intent = new Intent(this, RaceLocationPickerActivity.class);
        startActivityForResult(intent, PLACE_PICKER_REQUEST);
    }

    /**
     * Create a new race
     * @param view
     */
    public void createRace(View view) {
        if (raceLocation == null) {
            Toast.makeText(this, "L'emplacement de la course manque", Toast.LENGTH_SHORT).show();
            return;
        }

        if (((EditText)findViewById(R.id.setRaceName)).getText().equals("")) {
            Toast.makeText(this, "Le nom de la course manque", Toast.LENGTH_SHORT).show();
            return;
        }

        if (((TextView) findViewById(R.id.showRaceDate)).getText().equals("")) {
            Toast.makeText(this, "La date de la course manque", Toast.LENGTH_SHORT).show();
            return;
        }

        if (((TextView) findViewById(R.id.showRaceTime)).getText().equals("")) {
            Toast.makeText(this, "L'heure de la course manque", Toast.LENGTH_SHORT).show();
            return;
        }

        RaceTrackerDBConnection connection = new RaceTrackerDBConnection(getApplicationContext(),
                getResources().getString(R.string.db_server_key),
                getResources().getString(R.string.db_user_key),
                getResources().getString(R.string.db_pwd_key));

        RaceTrackerCompetitions competitions = new RaceTrackerCompetitions(connection);

        RaceTrackerCompetition competition = new RaceTrackerCompetition();
        competition.setName(((EditText)findViewById(R.id.setRaceName)).getText().toString());
        /* Create timestamp (format: 2018-08-01 10:00:00-00) */
        Timestamp ts = new Timestamp(dateTime.getTimeInMillis());
        competition.setEventDate(ts);
        competition.setLocation(raceLocation);
        competition.setZoom(zoom);

        competitions.insertNewRace(competition);

        /* TODO Track Points */

    }

    /**
     * Called when the Race Location Activity is finished
     * @param requestCode Request Code
     * @param resultCode Result of the selection
     * @param data Associated data
     */
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == PLACE_PICKER_REQUEST) {
            if (resultCode == RESULT_OK) {
                raceLocation = new LatLngWrapper(data.getDoubleExtra("latitude", 0),
                        data.getDoubleExtra("longitude", 0));
                zoom = data.getFloatExtra("zoom", 0);
                ((TextView) findViewById(R.id.showLocation)).setText(
                        "Lat: " + data.getDoubleExtra("latitude", 0) +
                        " Lon: " + data.getDoubleExtra("longitude", 0) +
                        " Zoom: " + data.getFloatExtra("zoom", 0));
            }
        }
    }
}
