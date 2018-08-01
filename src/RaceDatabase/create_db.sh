#!/bin/bash

db_name=race_tracker_db
postgis_scripts=/usr/share/postgresql/9.6/contrib/postgis-2.3

# Create the database
createdb $db_name
# Enable plsql
createlang plpgsql $db_name
# Load the PostGIS object and function definitions into the database
psql -d race_tracker_db -f ${postgis_scripts}/postgis.sql
# Create the schema
psql -d race_tracker_db -f schema_create.sql
# Create the tables
psql -d race_tracker_db -f data_model_main_create.sql
