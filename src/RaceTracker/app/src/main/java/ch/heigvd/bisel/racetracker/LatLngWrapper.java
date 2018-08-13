package ch.heigvd.bisel.racetracker;

import com.google.android.gms.maps.model.LatLng;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class LatLngWrapper implements Serializable {
    // mark it transient so defaultReadObject()/defaultWriteObject() ignore it
    private transient LatLng mLocation;

    // ...


    public LatLngWrapper(Double lat, Double lon) {
        this.mLocation = new LatLng(lat, lon);
    }

    public LatLng getObject() {
        return mLocation;
    }

    private void writeObject(ObjectOutputStream out) throws IOException {
        out.defaultWriteObject();
        out.writeDouble(mLocation.latitude);
        out.writeDouble(mLocation.longitude);
    }

    private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException {
        in.defaultReadObject();
        mLocation = new LatLng(in.readDouble(), in.readDouble());
    }
}
