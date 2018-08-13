package ch.heigvd.bisel.racetracker.activities;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.Toast;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitionAdapter;
import ch.heigvd.bisel.racetracker.RaceTrackerDBGetResult;
import ch.heigvd.bisel.racetracker.RaceTrackerDB;
import ch.heigvd.bisel.racetracker.RecyclerTouchListener;

public class ViewRaceSelectorActivity extends AppCompatActivity implements RaceTrackerDBGetResult {
    private RecyclerView mRecyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;
    private ArrayList<RaceTrackerCompetition> resultList;

    public void getResult(ResultSet results) throws SQLException {
        while (results.next()) {
            RaceTrackerCompetition cp = new RaceTrackerCompetition(results);
            resultList.add(cp);
            System.out.println("DBG: Competition: " + cp.toString());
        }

        results.getStatement().close();
        results.close();

        mAdapter.notifyDataSetChanged();
    }

    public void goToViewRace() {

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_race_selector);

        /* Get list of competitions from the DB */
        RaceTrackerDB db = new RaceTrackerDB(this);
        db.getCompetitions(this);

        /* Setup RecyclerView */
        mRecyclerView = findViewById(R.id.ViewRaceSelectorRecycler);

        // use this setting to improve performance if you know that changes
        // in content do not change the layout size of the RecyclerView
        // mRecyclerView.setHasFixedSize(true);

        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        resultList = new ArrayList<>();

        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, LinearLayoutManager.VERTICAL));

        mRecyclerView.addOnItemTouchListener(new RecyclerTouchListener(getApplicationContext(), mRecyclerView, new RecyclerTouchListener.ClickListener() {
            @Override
            public void onClick(View view, int position) {
                //Movie movie = movieList.get(position);
                RaceTrackerCompetition competition = resultList.get(position);
                Toast.makeText(getApplicationContext(), competition.name + " is selected!", Toast.LENGTH_SHORT).show();
                /* Add competition class to intent so next activity can retrieve it */
                Intent intent = new Intent(view.getContext(), ViewRaceActivity.class);
                intent.putExtra("competition", competition);
                startActivity(intent);
            }

            @Override
            public void onLongClick(View view, int position) {

            }
        }));

        mAdapter = new RaceTrackerCompetitionAdapter(resultList);
        mRecyclerView.setAdapter(mAdapter);
    }
}
