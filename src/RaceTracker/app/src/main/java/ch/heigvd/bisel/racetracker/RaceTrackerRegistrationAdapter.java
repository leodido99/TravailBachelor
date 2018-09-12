package ch.heigvd.bisel.racetracker;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.TextView;

import java.util.ArrayList;

public class RaceTrackerRegistrationAdapter extends RecyclerView.Adapter<RaceTrackerRegistrationAdapter.ViewHolder> {
    private ArrayList<RaceTrackerRegistration> registrations;

    // Provide a suitable constructor (depends on the kind of dataset)
    public RaceTrackerRegistrationAdapter(ArrayList<RaceTrackerRegistration> registrations) {
        this.registrations = registrations;
    }

    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(RaceTrackerRegistrationAdapter.ViewHolder holder, int position) {
        /* - get element from your dataset at this position
           - replace the contents of the view with that element */
        holder.competitorFirstName.setText(registrations.get(position).getCompetitor().getFirstName());
        holder.competitorLastName.setText(registrations.get(position).getCompetitor().getLastName());
        holder.competitorBib.setText("#" + registrations.get(position).getBibNumber());
        holder.competitorSensor.setText(String.valueOf(registrations.get(position).getSensorId()));
    }

    @Override
    public int getItemCount() {
        return registrations.size();
    }

    // Provide a reference to the views for each data item
    // Complex data items may need more than one view per item, and
    // you provide access to all the views for a data item in a view holder
    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView competitorFirstName, competitorLastName, competitorBib,
        competitorSensor;

        public ViewHolder(View v) {
            super(v);
            competitorFirstName = itemView.findViewById(R.id.CompetitorFirstName);
            competitorLastName = itemView.findViewById(R.id.CompetitorLastName);
            competitorBib = itemView.findViewById(R.id.CompetitorBib);
            competitorSensor = itemView.findViewById(R.id.CompetitorSensor);
        }
    }

    // Create new views (invoked by the layout manager)
    @Override
    public RaceTrackerRegistrationAdapter.ViewHolder onCreateViewHolder(ViewGroup parent,
                                                                   int viewType) {
        // create a new view
        View itemView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.registration_row, parent, false);
        RaceTrackerRegistrationAdapter.ViewHolder vh = new RaceTrackerRegistrationAdapter.ViewHolder(itemView);
        return vh;
    }
}
