package ch.heigvd.bisel.racetracker;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.ArrayList;

public class RaceTrackerCompetitorAdapter extends RecyclerView.Adapter<RaceTrackerCompetitorAdapter.ViewHolder>  {
    ArrayList<RaceTrackerCompetitor> competitors;

    // Provide a suitable constructor (depends on the kind of dataset)
    public RaceTrackerCompetitorAdapter(ArrayList<RaceTrackerCompetitor> competitors) {
        this.competitors = competitors;
    }

    // Provide a reference to the views for each data item
    // Complex data items may need more than one view per item, and
    // you provide access to all the views for a data item in a view holder
    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView firstName, lastName;

        public ViewHolder(View v) {
            super(v);
            firstName = itemView.findViewById(R.id.CompetitorFirstName);
            lastName = itemView.findViewById(R.id.CompetitorLastName);
        }
    }

    // Create new views (invoked by the layout manager)
    @Override
    public RaceTrackerCompetitorAdapter.ViewHolder onCreateViewHolder(ViewGroup parent,
                                                                       int viewType) {
        // create a new view
        View itemView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.race_competitor, parent, false);
        RaceTrackerCompetitorAdapter.ViewHolder vh = new RaceTrackerCompetitorAdapter.ViewHolder(itemView);

        return vh;
    }

    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(RaceTrackerCompetitorAdapter.ViewHolder holder, int position) {
        holder.firstName.setText(competitors.get(position).getFirstName());
        holder.lastName.setText(competitors.get(position).getLastName());
    }

    // Return the size of your dataset (invoked by the layout manager)
    @Override
    public int getItemCount() {
        return competitors.size();
    }
}
