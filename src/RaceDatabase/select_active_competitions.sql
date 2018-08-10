-- List currently active competitions
SELECT * FROM race_tracker.competition WHERE active = True;

-- List active competition for a given competitor through its sensor ID
select race_tracker.competitor_registration.competition_id, race_tracker.competitor_registration.competitor_id  from race_tracker.competitor_registration INNER JOIN race_tracker.competitor ON (race_tracker.competitor_registration.competitor_id = race_tracker.competitor.competitor_id) INNER JOIN race_tracker.competition ON (race_tracker.competition.competition_id = race_tracker.competitor_registration.competition_id) WHERE race_tracker.competitor_registration.sensor_id = CAST(x'BEEF' AS int) AND race_tracker.competition.active = True;






