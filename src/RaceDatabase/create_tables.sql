
CREATE TABLE race_tracker.country (
    country_code VARCHAR(2) UNIQUE NOT NULL,
    country_name VARCHAR(20) NOT NULL,
    country_icon BYTEA NOT NULL,
    PRIMARY_KEY(country_code)
);

CREATE TABLE race_tracker.competition (
    competition_id SERIAL UNIQUE NOT NULL,
    name varchar(20) NOT NULL,
    location GEOMETRY NOT NULL,
    event_date timestamp without time zone NOT NULL,
    PRIMARY KEY (competition_id)
);

CREATE TABLE race_tracker.track_point (
    track_point_id SERIAL UNIQUE NOT NULL,
    competition_id SERIAL NOT NULL REFERENCES race_tracker.competition,
    sequence INTEGER NOT NULL,
    position GEOMETRY NOT NULL,
    PRIMARY KEY (track_point_id, competition_track_id)
);

CREATE TABLE race_tracker.competitor (
    competitor_id SERIAL UNIQUE NOT NULL,
    first_name varchar(20) NOT NULL,
    last_name varchar(20) NOT NULL,
    country_code varchar(2) NOT NULL REFERENCES race_tracker.country,
    PRIMARY KEY (competitor_id)
);

CREATE TABLE race_tracker.competitor_registration (
    competitor_id SERIAL NOT NULL REFERENCES race_tracker.competitor,
    competition_id SERIAL NOT NULL REFERENCES race_tracker.competition,
    sensor_id integer NOT NULL,
    bib_number smallint NOT NULL,
    PRIMARY KEY (competitor_id, competition_id)
);

CREATE TABLE race_tracker.data_point (
    data_point_id SERIAL UNIQUE NOT NULL,
    competitor_id SERIAL NOT NULL REFERENCES race_tracker.competitor,
    competition_id SERIAL NOT NULL REFERENCES race_tracker.competition,
    sequence INTEGER NOT NULL,
    time_stamp timestamp without time zone NOT NULL,
    position GEOMETRY NOT NULL,
    heart_rate smallint NOT NULL,
    cadence smallint NOT NULL,
    nb_satellites smallint NOT NULL,
    position_dop smallint NOT NULL,
    status smallint NOT NULL,
    PRIMARY KEY (data_point_id, competitor_id, competition_id)
);

