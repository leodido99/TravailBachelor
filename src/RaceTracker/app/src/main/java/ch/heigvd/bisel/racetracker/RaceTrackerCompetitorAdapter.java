package ch.heigvd.bisel.racetracker;

import android.graphics.Color;
import android.support.v4.content.res.ResourcesCompat;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;

public class RaceTrackerCompetitorAdapter extends RecyclerView.Adapter<RaceTrackerCompetitorAdapter.ViewHolder>  {
    private ArrayList<RaceTrackerCompetitor> competitors;
    private boolean hideStats;
    private boolean focusEnable;
    private int focusedItem;

    /**
     * Provide a suitable constructor (depends on the kind of dataset)
     * @param competitors
     */
    public RaceTrackerCompetitorAdapter(ArrayList<RaceTrackerCompetitor> competitors) {
        this.competitors = competitors;
        hideStats = false;
        focusEnable = true;
        focusedItem = 0;
    }

    public boolean isFocusEnable() {
        return focusEnable;
    }

    public void setFocusEnable(boolean focusEnable) {
        this.focusEnable = focusEnable;
    }

    public boolean isHideStats() {
        return hideStats;
    }

    public void setHideStats(boolean hideStats) {
        this.hideStats = hideStats;
    }

    public ArrayList<RaceTrackerCompetitor> getCompetitors() {
        return competitors;
    }

    /**
     * Provide a reference to the views for each data item
     * Complex data items may need more than one view per item, and
     * you provide access to all the views for a data item in a view holder
     */
    public class ViewHolder extends RecyclerView.ViewHolder {
        public TextView firstName, lastName, bibNumber, distance, elapsedTime, speed;
        public TextView heartRate, cadence;
        public WebView countryFlag;
        public ImageView heartIcon, cadenceIcon;

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
            heartIcon = itemView.findViewById(R.id.HeartIcon);
            cadenceIcon = itemView.findViewById(R.id.CadenceIcon);

            /* Handle item click and set the selection */
            /*if (focusEnable) {
                itemView.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        System.out.println("1 ViewHolder Focused " + focusedItem);
                        notifyItemChanged(focusedItem);
                        focusedItem = getLayoutPosition();
                        System.out.println("2 ViewHolder Focused " + focusedItem);
                        notifyItemChanged(focusedItem);
                    }
                });
            }*/
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
        if (competitors.get(position).getCountry() != null) {
            holder.countryFlag.loadData(competitors.get(position).getCountry().getCountryFlagXml(), "text/xml" , null);
        }

        if (hideStats) {
            holder.bibNumber.setVisibility(View.GONE);
            holder.heartRate.setVisibility(View.GONE);
            holder.cadence.setVisibility(View.GONE);
            holder.distance.setVisibility(View.GONE);
            holder.speed.setVisibility(View.GONE);
            holder.elapsedTime.setVisibility(View.GONE);
            holder.bibNumber.setVisibility(View.GONE);
            holder.cadenceIcon.setVisibility(View.GONE);
            holder.heartIcon.setVisibility(View.GONE);
            holder.countryFlag.setVisibility(View.GONE);
        } else {
            holder.bibNumber.setText("#" + competitors.get(position).getBibNumber());
            holder.heartRate.setText(Integer.toString(competitors.get(position).getCurrHeartRate()));
            holder.cadence.setText(Integer.toString(competitors.get(position).getCurrCadence()));
            holder.distance.setText(String.format("%.2f", competitors.get(position).getDistance()) + " km");
            holder.speed.setText(String.format("%.1f", competitors.get(position).getSpeed()) + " min/km");
            holder.elapsedTime.setText(String.format("%02d:%02d:%02d",
                    competitors.get(position).getElapsedTimeH(),
                    competitors.get(position).getElapsedTimeM(),
                    competitors.get(position).getElapsedTimeS()));
        }

        if (focusEnable) {
            holder.itemView.setActivated(focusedItem == position);
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
