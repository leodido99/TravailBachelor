import argparse
import re

sql_query = 'INSERT INTO race_tracker.track_point (competition_id, sequence, position) VALUES ({}, {}, ST_MakePoint({}, {}));\n'

parser = argparse.ArgumentParser(description='Extract GPS positions')

parser.add_argument('-o', metavar='file_name', nargs='?', default='gps_points.sql',
                    help='Output file name')
parser.add_argument('id', metavar='competition_id', nargs=1,
                    help='Competition ID to specify in the SQL query')
parser.add_argument('file', metavar='file_name', nargs='+',
                   help='File to extract GPS positions from')

args = parser.parse_args()

seq = 0

for file_name in args.file:
    print('Extracting positions from ' + file_name)
    with open(file_name, 'r') as f:
        lines = f.readlines()
    with open(args.o, 'w') as f_out:
        for line in lines:
	    line_data = re.split(' |	', line)
            f_out.write(str.format(sql_query, args.id[0], seq, line_data[3], line_data[4]))
	    seq = seq + 1
