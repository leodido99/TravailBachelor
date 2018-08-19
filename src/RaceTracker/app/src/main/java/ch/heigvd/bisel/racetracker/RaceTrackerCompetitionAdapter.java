package ch.heigvd.bisel.racetracker;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.ArrayList;

public class RaceTrackerCompetitionAdapter extends RecyclerView.Adapter<RaceTrackerCompetitionAdapter.ViewHolder> {
    private ArrayList<RaceTrackerCompetition> competitions;

    // Provide a suitable constructor (depends on the kind of dataset)
    public RaceTrackerCompetitionAdapter(ArrayList<RaceTrackerCompetition> competitions) {
        this.competitions = competitions;
    }

    // Provide a reference to the views for each data item
    // Complex data items may need more than one view per item, and
    // you provide access to all the views for a data item in a view holder
    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView name, date, active;

        public ViewHolder(View v) {
            super(v);
            name = itemView.findViewById(R.id.RaceName);
            date = itemView.findViewById(R.id.RaceDate);
            active = itemView.findViewById(R.id.RaceStatus);
        }
    }

    // Create new views (invoked by the layout manager)
    @Override
    public RaceTrackerCompetitionAdapter.ViewHolder onCreateViewHolder(ViewGroup parent,
                                                   int viewType) {
        // create a new view
        View itemView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.race_selector_row, parent, false);
        ViewHolder vh = new ViewHolder(itemView);
        return vh;
    }

    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        String activeText;

        /* - get element from your dataset at this position
           - replace the contents of the view with that element */
        holder.name.setText(competitions.get(position).getName());
        /* TODO Clean-up way of showing that app cannot connect to DB */
        if (competitions.get(position).getEventDate() != null) {
            holder.date.setText(competitions.get(position).getEventDate().toString());
        }
        if (competitions.get(position).isActive()) {
            activeText = "LIVE";
        } else {
            activeText = "Revoir";
        }
        holder.active.setText(activeText);
    }

    // Return the size of your dataset (invoked by the layout manager)
    @Override
    public int getItemCount() {
        return competitions.size();
    }

}
