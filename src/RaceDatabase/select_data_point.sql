-- Get last sequence for each competitor for a given competition
SELECT data_point.competitor_id, MAX(sequence) FROM race_tracker.data_point WHERE data_point.competition_id = X GROUP BY data_point.competitor_id;
-- Get data point
SELECT * FROM race_tracker.data_point WHERE data_point.competitor_id = X AND data_point.sequence = Y AND data_point.competition_id = Z;

-- All in one
SELECT data_point_id, competitor_id, competition_id, sequence, time_stamp, ST_X(position) as lat, ST_Y(position) as lon, heart_rate, cadence, nb_satellites, position_dop, status FROM (SELECT rank() OVER (PARTITION BY data_point.competitor_id ORDER BY sequence DESC) AS therank, * FROM race_tracker.data_point WHERE data_point.competition_id = 1) t WHERE therank = 1;

SELECT data_point_id, competitor_id, competition_id, sequence, time_stamp, ST_X(position) as lat, ST_Y(position) as lon, heart_rate, cadence, nb_satellites, position_dop, status FROM (SELECT rank() OVER (PARTITION BY data_point.competitor_id ORDER BY sequence DESC) AS therank, * FROM race_tracker.data_point WHERE data_point.competition_id = 4) t WHERE therank = 1;

-- Get all data points for a given competition
SELECT data_point_id, competitor_id, competition_id, sequence, time_stamp, ST_X(position) as lat, ST_Y(position) as lon, heart_rate, cadence, nb_satellites, position_dop, status FROM race_tracker.data_point WHERE data_point.competition_id = 1;
