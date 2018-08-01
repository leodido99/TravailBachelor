-- Test race #1
-- Léonard
INSERT INTO race_tracker.data_point (competitor_id, competition_id, sequence, time_stamp, position, heart_rate, cadence, nb_satellites, position_dop, status) 
	VALUES (1, 1, 0, '2018-05-26 08:30:00-00', ST_MakePoint(46.9933, 6.91612), 130, 160, 5, 9.9, 0);
