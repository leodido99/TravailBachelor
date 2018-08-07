-- Test race #1
-- Inactive
INSERT INTO race_tracker.competitor_registration (competitor_id, competition_id, sensor_id, bib_number) VALUES (1, 1, x'BEEF'::int, 65);
-- Active
INSERT INTO race_tracker.competitor_registration (competitor_id, competition_id, sensor_id, bib_number) VALUES (2, 4, x'BEEF'::int, 99);
