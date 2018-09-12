SELECT track_point_id, competition_id, sequence, ST_X(position) as latitude, ST_Y(position) as longitude FROM race_tracker.track_point WHERE competition_id = 1 ORDER BY sequence;
