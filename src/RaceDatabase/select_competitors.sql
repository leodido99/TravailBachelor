-- Select competitors to a certain competition
SELECT * FROM race_tracker.competitor_registration INNER JOIN race_tracker.competitor ON (competitor_registration.competitor_id = competitor.competitor_id) WHERE competitor_registration.competition_id = 1;
