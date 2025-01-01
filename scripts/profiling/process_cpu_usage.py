import csv
import argparse

def f1(args):

    with open(args.cpu_usage_data, 'r') as f:
        # CSV table
        # New Process,CPU Usage (in view) (s),Switch-In Time (s),New Switch-In Time (s),% CPU Usage
        
        # Add all columns to a table    
        reader = csv.DictReader(f)
    
        # Sort by Switch-In Time
        reader = sorted(reader, key=lambda row: float(row['Switch-In Time (s)']))

        # Traverse the table in the range [r0, r0 + 0.1s], aggregate all the cpu usage (in view) values from the process
        # with the name args.process_name and do the same for every other process, and then compute the cpu usage %.

        # The output should be a table with the following columns:
        # Process Name,Switch-In Time (s),% CPU Usage

        # Get the very first Switch-In Time (s) value
        r0 = 4.00#float(reader[0]['Switch-In Time (s)'])
        print(r0)

        for i in range(100):
            r1 = r0 + 0.2

            process_cpu_usage_s = 0.00
            total_cpu_usage_s = 0.00

            for row in reader:
                if float(row['Switch-In Time (s)']) >= r0 and float(row['Switch-In Time (s)']) < r1:
                    if args.process_name in row['New Process']:
                        process_cpu_usage_s += float(row['CPU Usage (in view) (s)'])
                    total_cpu_usage_s += float(row['CPU Usage (in view) (s)'])
                        #print(row)

            #print("")

            if total_cpu_usage_s == 0.00:
                continue

            # process name, timestamp (s) ,0000.00, % cpu usage
            
            print(f'{args.process_name}, {r0:.5f}, {r1:.5f}, {process_cpu_usage_s / total_cpu_usage_s * 100}')
            #print(f'{args.process_name}, {process_cpu_usage_s / total_cpu_usage_s * 100}')

            r0 = r1


def sum_range(table, process_name, t0, t1):
    process_cpu = 0.00
    total_cpu = 0.00

    for row in table:
        if float(row['Switch-In Time (s)']) >= t0 and float(row['Switch-In Time (s)']) <= t1:
            if process_name in row['New Process']:
                process_cpu += float(row['CPU Usage (in view) (s)'])
            total_cpu += float(row['CPU Usage (in view) (s)'])

    return f'{t0:.3f}', f'{t1:.3f}', f'{total_cpu:.2f}', f'{process_cpu:.2f}', f'{(process_cpu / total_cpu * 100):.2f}'

def f2(args):

    table = []
    with open(args.cpu_usage_data, 'r') as f:
        # Copy the csv file to a table
        reader = csv.DictReader(f)

        # Copy all rows to table
        
        for row in reader:
            table.append(row)

    result = []
    for row in table:
        if args.process_name in row['New Process']:
            #print(row)

            t0 = float(row['Switch-In Time (s)'])
            t1 = t0 + 1

            result.append(sum_range(table, args.process_name, t0-1, t1))
        
    result = sorted(result, key=lambda x: float(x[0]))

    print('t0, t1, total_cpu, process_cpu, % cpu usage')
    for r in result:
        # Export as csv
        print(','.join(r))

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('cpu_usage_data', type=str, help='Path to the CSV file containing the CPU usage data')
    parser.add_argument('process_name', type=str, help='Name of the process to analyze')

    args = parser.parse_args()

    f2(args)    

if __name__ == '__main__':
    main()

