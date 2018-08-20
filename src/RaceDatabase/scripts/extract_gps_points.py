#!/usr/bin/python
import argparse
import re

track_point_sql_query = 'INSERT INTO race_tracker.track_point (competition_id, sequence, position) VALUES ({}, {}, ST_MakePoint({}, {}));\n'
data_point_sql_query = 'INSERT INTO race_tracker.data_point (competitor_id, competition_id, sequence, time_stamp, position, heart_rate, cadence, nb_satellites, position_dop, status) VALUES ({}, {}, {}, \'2018-05-26 08:30:00-00\', ST_MakePoint({}, {}), 130, 160, 5, 9.9, 0);\n'

parser = argparse.ArgumentParser(description='Extract GPS positions')

parser.add_argument('-o', metavar='file_name', nargs='?', default='gps_points.sql',
                    help='Output file name')
parser.add_argument('query', metavar='query_type', nargs=1,
                    help='Type of query to generate (0 = track point, 1 = data point)')
parser.add_argument('competition', metavar='competition_id', nargs=1,
                    help='Competition ID to specify in the SQL query')
parser.add_argument('competitor', metavar='competitor_id', nargs=1,
                    help='Competitor ID to specify in the SQL query')
parser.add_argument('file', metavar='file_name', nargs='+',
                   help='File to extract GPS positions from')

args = parser.parse_args()

seq = 0

print(args)

for file_name in args.file:
    print('Extracting positions from ' + file_name)
    with open(file_name, 'r') as f:
        lines = f.readlines()
    with open(args.o, 'w') as f_out:
        for line in lines:
	    line_data = re.split(' |	', line)
	    if args.query[0] == '0':
	            f_out.write(str.format(track_point_sql_query, args.competition[0], seq, line_data[3], line_data[4]))
	    else:
	            f_out.write(str.format(data_point_sql_query, args.competitor[0], args.competition[0], seq, line_data[3], line_data[4]))
	    seq = seq + 1
