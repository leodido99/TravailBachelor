package ch.heigvd.bisel.racetracker;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.drawable.VectorDrawable;
import android.renderscript.ScriptGroup;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.sql.ResultSet;
import java.sql.SQLException;

public class RaceTrackerCountry {
    private String countryCode;
    private String countryName;
    private byte[] countryFlag;
    private Bitmap countryFlagIcon;
    private String countryFlagXml;

    public RaceTrackerCountry(ResultSet fromDB) throws SQLException {
        countryCode = fromDB.getString("country_code");
        countryName = fromDB.getString("country_name");
        countryFlag = fromDB.getBytes("country_icon");


        countryFlagXml = new String(countryFlag);
        //VectorDrawable vectorDrawable = new VectorDrawable();
        //String xmlData = new String(countryFlag);
        //vectorDrawable.createFromXml(xmlData)

        //InputStream arrayInputStream = new ByteArrayInputStream(countryFlag);
        //vectorDrawable.createFromStream(arrayInputStream, "test");
        //countryFlagIcon = Bitmap.createBitmap(vectorDrawable.getIntrinsicWidth(),
        //        vectorDrawable.getIntrinsicHeight(), Bitmap.Config.ARGB_8888);
        //Canvas canvas = new Canvas(countryFlagIcon);
        //vectorDrawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
        //vectorDrawable.draw(canvas);

        //
        InputStream arrayInputStream = new ByteArrayInputStream(countryFlag);
        countryFlagIcon = BitmapFactory.decodeStream(arrayInputStream);
    }

    public String getCountryCode() {
        return countryCode;
    }

    public void setCountryCode(String countryCode) {
        this.countryCode = countryCode;
    }

    public String getCountryName() {
        return countryName;
    }

    public void setCountryName(String countryName) {
        this.countryName = countryName;
    }

    public byte[] getCountryFlag() {
        return countryFlag;
    }

    public void setCountryFlag(byte[] countryFlag) {
        this.countryFlag = countryFlag;
    }

    public Bitmap getCountryFlagIcon() {
        return countryFlagIcon;
    }

    public void setCountryFlagIcon(Bitmap countryFlagIcon) {
        this.countryFlagIcon = countryFlagIcon;
    }

    public String getCountryFlagXml() {
        return countryFlagXml;
    }

    public void setCountryFlagXml(String countryFlagXml) {
        this.countryFlagXml = countryFlagXml;
    }
}
