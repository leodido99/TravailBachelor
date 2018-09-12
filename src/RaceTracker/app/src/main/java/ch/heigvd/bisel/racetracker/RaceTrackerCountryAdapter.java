package ch.heigvd.bisel.racetracker;

import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.TextView;

import java.util.ArrayList;

public class RaceTrackerCountryAdapter extends RecyclerView.Adapter<RaceTrackerCountryAdapter.ViewHolder> {
    private ArrayList<RaceTrackerCountry> countries;

    // Provide a suitable constructor (depends on the kind of dataset)
    public RaceTrackerCountryAdapter(ArrayList<RaceTrackerCountry> countries) {
        this.countries = countries;
    }

    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(RaceTrackerCountryAdapter.ViewHolder holder, int position) {
        /* - get element from your dataset at this position
           - replace the contents of the view with that element */
        holder.countryCode.setText(countries.get(position).getCountryCode());
        holder.countryName.setText(countries.get(position).getCountryName());
        holder.countryIcon.loadData(countries.get(position).getCountryFlagXml(), "text/xml" , null);
    }

    @Override
    public int getItemCount() {
        return countries.size();
    }


    // Provide a reference to the views for each data item
    // Complex data items may need more than one view per item, and
    // you provide access to all the views for a data item in a view holder
    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView countryCode, countryName;
        public WebView countryIcon;

        public ViewHolder(View v) {
            super(v);
            countryCode = itemView.findViewById(R.id.countryCode);
            countryName = itemView.findViewById(R.id.countryName);
            countryIcon = itemView.findViewById(R.id.countryIcon);
        }
    }

    // Create new views (invoked by the layout manager)
    @Override
    public RaceTrackerCountryAdapter.ViewHolder onCreateViewHolder(ViewGroup parent,
                                                                       int viewType) {
        // create a new view
        View itemView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.country_selector, parent, false);
        RaceTrackerCountryAdapter.ViewHolder vh = new RaceTrackerCountryAdapter.ViewHolder(itemView);
        return vh;
    }
}
