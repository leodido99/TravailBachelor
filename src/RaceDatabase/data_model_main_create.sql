CREATE TABLE race_tracker.competition (
    competition_id SERIAL NOT NULL,
    competition_track_id SERIAL,
    name varchar(20) NOT NULL,
    location GEOMETRY NOT NULL,
    event_date timestamp without time zone NOT NULL,
    PRIMARY KEY (competition_id)
);

CREATE INDEX ON race_tracker.competition
    (competition_track_id);


CREATE TABLE race_tracker.data_point (
    data_point_id integer NOT NULL,
    competitor_id SERIAL NOT NULL,
    competition_id SERIAL NOT NULL,
    time_stamp timestamp without time zone NOT NULL,
    position GEOMETRY NOT NULL,
    heart_rate smallint NOT NULL,
    cadence smallint NOT NULL,
    nb_satellites smallint NOT NULL,
    position_dop smallint NOT NULL,
    PRIMARY KEY (data_point_id)
);

CREATE INDEX ON race_tracker.data_point
    (competitor_id);
CREATE INDEX ON race_tracker.data_point
    (competition_id);


CREATE TABLE race_tracker.competitor_track (
    competitor_id SERIAL NOT NULL,
    competition_id SERIAL NOT NULL,
    captor_id integer NOT NULL,
    bib_number smallint NOT NULL,
    PRIMARY KEY (competitor_id, competition_id)
);


CREATE TABLE race_tracker.gps_point (
    gps_point_id integer NOT NULL,
    competition_track_id SERIAL NOT NULL,
    position GEOMETRY NOT NULL,
    PRIMARY KEY (gps_point_id, competition_track_id)
);


CREATE TABLE race_tracker.competitor (
    competitor_id SERIAL NOT NULL,
    first_name varchar(20) NOT NULL,
    last_name varchar(20) NOT NULL,
    country varchar(20) NOT NULL,
    PRIMARY KEY (competitor_id)
);


CREATE TABLE race_tracker.competition_track (
    competition_track_id SERIAL NOT NULL,
    gps_point_id integer NOT NULL,
    PRIMARY KEY (competition_track_id)
);

CREATE INDEX ON race_tracker.competition_track
    (gps_point_id);

