package ch.heigvd.bisel.racetracker;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.TextView;

import java.util.ArrayList;

public class RaceTrackerCompetitorAdapter extends RecyclerView.Adapter<RaceTrackerCompetitorAdapter.ViewHolder>  {
    ArrayList<RaceTrackerCompetitor> competitors;

    /**
     * Provide a suitable constructor (depends on the kind of dataset)
     * @param competitors
     */
    public RaceTrackerCompetitorAdapter(ArrayList<RaceTrackerCompetitor> competitors) {
        this.competitors = competitors;
    }

    /**
     * Provide a reference to the views for each data item
     * Complex data items may need more than one view per item, and
     * you provide access to all the views for a data item in a view holder
     */
    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView firstName, lastName, bibNumber, distance, elapsedTime, speed;
        public TextView heartRate, cadence;
        public WebView countryFlag;

        public ViewHolder(View v) {
            super(v);
            firstName = itemView.findViewById(R.id.CompetitorFirstName);
            lastName = itemView.findViewById(R.id.CompetitorLastName);
            bibNumber = itemView.findViewById(R.id.CompetitorBib);
            heartRate = itemView.findViewById(R.id.CompetitorHR);
            distance = itemView.findViewById(R.id.CompetitorDistance);
            speed = itemView.findViewById(R.id.CompetitorSpeed);
            elapsedTime = itemView.findViewById(R.id.CompetitorTimeElapsed);
            countryFlag = itemView.findViewById(R.id.CompetitorCountry);
            cadence = itemView.findViewById(R.id.CompetitorCadence);
        }
    }

    /**
     * Create new views (invoked by the layout manager)
     * @param parent
     * @param viewType
     * @return
     */
    @Override
    public RaceTrackerCompetitorAdapter.ViewHolder onCreateViewHolder(ViewGroup parent,
                                                                       int viewType) {
        View itemView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.race_competitor, parent, false);
        RaceTrackerCompetitorAdapter.ViewHolder vh = new RaceTrackerCompetitorAdapter.ViewHolder(itemView);

        return vh;
    }

    /**
     * Replace the contents of a view (invoked by the layout manager)
     * @param holder
     * @param position
     */
    @Override
    public void onBindViewHolder(RaceTrackerCompetitorAdapter.ViewHolder holder, int position) {
        holder.firstName.setText(competitors.get(position).getFirstName());
        holder.lastName.setText(competitors.get(position).getLastName());
        holder.bibNumber.setText("#" + competitors.get(position).getBibNumber());
        holder.heartRate.setText(Integer.toString(competitors.get(position).getCurrHeartRate()));
        holder.cadence.setText(Integer.toString(competitors.get(position).getCurrCadence()));
        holder.distance.setText(String.format("%.2f", competitors.get(position).getDistance()) + " km");
        holder.speed.setText(String.format("%.1f", competitors.get(position).getSpeed()) + " min/km");
        holder.elapsedTime.setText(String.format("%02d:%02d:%02d",
                competitors.get(position).getElapsedTimeH(),
                competitors.get(position).getElapsedTimeM(),
                competitors.get(position).getElapsedTimeS()));
        if (competitors.get(position).getCountry() != null) {
            holder.countryFlag.loadData(competitors.get(position).getCountry().getCountryFlagXml(), "text/xml" , null);
        }
    }

    /**
     * Return the size of your dataset (invoked by the layout manager)
     * @return Count item in RecyclerView
     */
    @Override
    public int getItemCount() {
        return competitors.size();
    }
}
