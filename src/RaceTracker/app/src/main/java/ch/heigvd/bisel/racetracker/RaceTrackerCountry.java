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
    private String countryFlagXml;

    public RaceTrackerCountry(ResultSet fromDB) throws SQLException {
        countryCode = fromDB.getString("country_code");
        countryName = fromDB.getString("country_name");
        countryFlag = fromDB.getBytes("country_icon");
        countryFlagXml = new String(countryFlag);
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

    public String getCountryFlagXml() {
        return countryFlagXml;
    }

    public void setCountryFlagXml(String countryFlagXml) {
        this.countryFlagXml = countryFlagXml;
    }
}
