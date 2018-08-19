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
import java.sql.Timestamp;
import java.util.ArrayList;

import ch.heigvd.bisel.racetracker.R;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetition;
import ch.heigvd.bisel.racetracker.RaceTrackerCompetitionAdapter;
import ch.heigvd.bisel.racetracker.OnQueryResultReady;
import ch.heigvd.bisel.racetracker.RaceTrackerDB;
import ch.heigvd.bisel.racetracker.RecyclerTouchListener;

public class ViewRaceSelectorActivity extends AppCompatActivity {
    private RecyclerView mRecyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;
    private ArrayList<RaceTrackerCompetition> resultList;
    private CompetitionsResult competitionsResult;

    public class CompetitionsResult implements OnQueryResultReady {
        private RaceTrackerDB.RaceTrackerQuery results;

        public RaceTrackerDB.RaceTrackerQuery getResults() {
            return results;
        }

        /* Callback when competitions are ready */
        public void onQueryResultReady(RaceTrackerDB.RaceTrackerQuery results) throws SQLException {
            this.results = results;

            if (results.getException() != null) {
                /* Exception during query */
                Toast.makeText(getApplicationContext(), "Exception during query: " + results.getException().getMessage(), Toast.LENGTH_LONG).show();
            } else {
                while (results.getResult().next()) {
                    RaceTrackerCompetition cp = new RaceTrackerCompetition(results.getResult());
                    resultList.add(cp);
                    System.out.println("DBG: Competition: " + cp.toString());
                }

                results.getResult().getStatement().close();
                results.getResult().close();
            }

            /* Updates UI */
            mAdapter.notifyDataSetChanged();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_race_selector);

        /* Get list of competitions from the DB */
        RaceTrackerDB db = new RaceTrackerDB(this);
        competitionsResult = new CompetitionsResult();
        db.getCompetitions(competitionsResult);

        /* Setup RecyclerView */
        mRecyclerView = findViewById(R.id.ViewRaceSelectorRecycler);

        /* use this setting to improve performance if you know that changes
         * in content do not change the layout size of the RecyclerView
         */
        mRecyclerView.setHasFixedSize(true);

        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        /* Initialize list to be filled later in callback */
        resultList = new ArrayList<>();

        /* Add line between items */
        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, LinearLayoutManager.VERTICAL));

        /* Add on touch listener to detect race selection */
        mRecyclerView.addOnItemTouchListener(new RecyclerTouchListener(getApplicationContext(), mRecyclerView, new RecyclerTouchListener.ClickListener() {
            @Override
            public void onClick(View view, int position) {
                //Movie movie = movieList.get(position);
                RaceTrackerCompetition competition = resultList.get(position);
                Toast.makeText(getApplicationContext(), competition.getName() + " is selected!", Toast.LENGTH_SHORT).show();
                /* Add competition class to intent so next activity can retrieve it */
                Intent intent = new Intent(view.getContext(), ViewRaceActivity.class);
                intent.putExtra("competition", competition);
                startActivity(intent);
            }

            @Override
            public void onLongClick(View view, int position) {

            }
        }));

        /* Initialize adapter and add it to view */
        mAdapter = new RaceTrackerCompetitionAdapter(resultList);
        mRecyclerView.setAdapter(mAdapter);
    }
}
