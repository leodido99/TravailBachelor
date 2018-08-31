/** 
 * @file race_mode_record.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 30, 2018
 */
#ifndef SRC_RACE_APP_SERVER_RACE_MODE_RECORD_H_
#define SRC_RACE_APP_SERVER_RACE_MODE_RECORD_H_

#include <cstdint>
#include <string>

#define RACE_PKT_NO_ID 0xFFFF

class race_pkt_timestamp {
private:
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
public:
	uint8_t get_day() const
	{
		return day;
	}

	void set_day(uint8_t day)
	{
		this->day = day;
	}

	uint8_t get_hour() const
	{
		return hour;
	}

	void set_hour(uint8_t hour)
	{
		this->hour = hour;
	}

	uint8_t get_min() const
	{
		return min;
	}

	void set_min(uint8_t min)
	{
		this->min = min;
	}

	uint8_t get_month() const
	{
		return month;
	}

	void set_month(uint8_t month)
	{
		this->month = month;
	}

	uint8_t get_sec() const
	{
		return sec;
	}

	void set_sec(uint8_t sec)
	{
		this->sec = sec;
	}

	uint16_t get_year() const
	{
		return year;
	}

	void set_year(uint16_t year)
	{
		this->year = year;
	}
};

class race_mode_record {
private:
	uint16_t id;
	uint8_t status;
	uint16_t seq;
	double lat;
	double lon;
	uint8_t nb_sv;
	double hdop;
	uint8_t heart_rate;
	uint8_t cadence;
	race_pkt_timestamp timestamp;
public:
	race_mode_record();
	virtual ~race_mode_record();
	std::string to_string();

	uint8_t get_cadence() const
	{
		return cadence;
	}

	void set_cadence(uint8_t cadence)
	{
		this->cadence = cadence;
	}

	double get_hdop() const
	{
		return hdop;
	}

	void set_hdop(double hdop)
	{
		this->hdop = hdop;
	}

	uint8_t get_heart_rate() const
	{
		return heart_rate;
	}

	void set_heart_rate(uint8_t heartRate)
	{
		heart_rate = heartRate;
	}

	uint16_t get_id() const
	{
		return id;
	}

	void set_id(uint16_t id)
	{
		this->id = id;
	}

	double get_lat() const
	{
		return lat;
	}

	void set_lat(double lat)
	{
		this->lat = lat;
	}

	double get_lon() const
	{
		return lon;
	}

	void set_lon(double lon)
	{
		this->lon = lon;
	}

	uint8_t get_nb_sv() const
	{
		return nb_sv;
	}

	void set_nb_sv(uint8_t nbSv)
	{
		nb_sv = nbSv;
	}

	uint16_t get_seq() const
	{
		return seq;
	}

	void set_seq(uint16_t seq)
	{
		this->seq = seq;
	}

	uint8_t get_status() const
	{
		return status;
	}

	void set_status(uint8_t status)
	{
		this->status = status;
	}

	race_pkt_timestamp& get_timestamp()
	{
		return timestamp;
	}

	void set_timestamp(const race_pkt_timestamp& timestamp)
	{
		this->timestamp = timestamp;
	}
};

#endif /* SRC_RACE_APP_SERVER_RACE_MODE_RECORD_H_ */
