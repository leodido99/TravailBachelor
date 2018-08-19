
CREATE TABLE race_tracker.country (
    country_code VARCHAR(2) UNIQUE NOT NULL,
    country_name VARCHAR(20) NOT NULL,
    country_icon BYTEA NOT NULL,
    PRIMARY KEY(country_code)
);

CREATE TABLE race_tracker.competition (
    competition_id SERIAL UNIQUE NOT NULL,
    name VARCHAR(50) NOT NULL,
    location GEOMETRY NOT NULL,
    event_date TIMESTAMP without time zone NOT NULL,
    active BOOLEAN NOT NULL DEFAULT FALSE,
    zoom INTEGER NOT NULL DEFAULT 12,
    PRIMARY KEY (competition_id)
);

CREATE TABLE race_tracker.track_point (
    track_point_id SERIAL UNIQUE NOT NULL,
    competition_id SERIAL NOT NULL REFERENCES race_tracker.competition,
    sequence INTEGER NOT NULL,
    position GEOMETRY NOT NULL,
    PRIMARY KEY (track_point_id)
);

CREATE TABLE race_tracker.competitor (
    competitor_id SERIAL UNIQUE NOT NULL,
    first_name VARCHAR(20) NOT NULL,
    last_name VARCHAR(20) NOT NULL,
    country_code VARCHAR(2) NOT NULL REFERENCES race_tracker.country,
    PRIMARY KEY (competitor_id)
);

CREATE TABLE race_tracker.competitor_registration (
    competitor_id SERIAL NOT NULL REFERENCES race_tracker.competitor,
    competition_id SERIAL NOT NULL REFERENCES race_tracker.competition,
    sensor_id INTEGER NOT NULL,
    bib_number SMALLINT NOT NULL,
    PRIMARY KEY (competitor_id, competition_id)
);

CREATE TABLE race_tracker.data_point (
    data_point_id SERIAL UNIQUE NOT NULL,
    competitor_id SERIAL NOT NULL REFERENCES race_tracker.competitor,
    competition_id SERIAL NOT NULL REFERENCES race_tracker.competition,
    sequence INTEGER NOT NULL,
    time_stamp TIMESTAMP without time zone NOT NULL,
    position GEOMETRY NOT NULL,
    heart_rate SMALLINT NOT NULL,
    cadence SMALLINT NOT NULL,
    nb_satellites SMALLINT NOT NULL,
    position_dop FLOAT8 NOT NULL,
    status SMALLINT NOT NULL,
    PRIMARY KEY (data_point_id, competitor_id, competition_id)
);

