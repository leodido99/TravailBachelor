package ch.heigvd.bisel.racetracker.activities;

import android.app.DatePickerDialog;

import java.text.DateFormat;
import java.util.Calendar;
import java.util.GregorianCalendar;

import android.app.TimePickerDialog;
import android.content.Intent;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.DatePicker;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;
import com.google.android.gms.location.places.Place;
import com.google.android.gms.location.places.ui.PlacePicker;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.fragments.DatePickerFragment;
import ch.heigvd.bisel.racetracker.fragments.TimePickerFragment;

public class CreateNewRaceActivity extends AppCompatActivity implements DatePickerDialog.OnDateSetListener, TimePickerDialog.OnTimeSetListener {
    int PLACE_PICKER_REQUEST = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_new_race);
    }

    public void pickRaceDate(View view) {
        DialogFragment datePicker = new DatePickerFragment();
        datePicker.show(getSupportFragmentManager(), "datePicker");
    }

    public void pickRaceTime(View view) {
        DialogFragment timePicker = new TimePickerFragment();
        timePicker.show(getSupportFragmentManager(), "timePicker");
    }

    public void onDateSet(DatePicker view, int year, int month, int day) {
        Calendar cal = new GregorianCalendar(year, month, day);
        setDate(cal);
    }

    private void setDate(final Calendar calendar) {
        final DateFormat dateFormat = DateFormat.getDateInstance(DateFormat.MEDIUM);
        ((TextView) findViewById(R.id.showRaceDate)).setText(dateFormat.format(calendar.getTime()));
    }

    private void setTime(final Calendar calendar) {
        final DateFormat timeFormat = DateFormat.getTimeInstance(DateFormat.MEDIUM);
        ((TextView) findViewById(R.id.showRaceTime)).setText(timeFormat.format(calendar.getTime()));
    }

    public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
        Calendar cal = new GregorianCalendar(0, 0,0, hourOfDay, minute);
        setTime(cal);
    }

    public void selectLocation(View view) throws GooglePlayServicesNotAvailableException, GooglePlayServicesRepairableException {
        Intent intent = new Intent(this, RaceLocationPickerActivity.class);
        startActivity(intent);
    }

    public void createRace(View view) {

    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == PLACE_PICKER_REQUEST) {
            if (resultCode == RESULT_OK) {
                Place place = PlacePicker.getPlace(this, data);
                String toastMsg = String.format("Place: %s", place.getName());
                Toast.makeText(this, toastMsg, Toast.LENGTH_LONG).show();
            }
        }
    }
}
