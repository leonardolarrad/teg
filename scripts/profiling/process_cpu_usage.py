import csv
import argparse

def sum_range(table, process_name, t0, t1, index=0):
    total_cpu = 0.00
    idle_cpu = 0.00
    other_cpu = 0.00
    process_cpu = 0.00

    for row in table[max(index-1000, 0):]:
        if float(row['Switch-In Time (s)']) >= t0 and float(row['Switch-In Time (s)']) <= t1:
            if process_name in row['New Process']:
                process_cpu += float(row['CPU Usage (in view) (s)'])
            elif 'Idle (0)' in row['New Process']:
                idle_cpu += float(row['CPU Usage (in view) (s)'])
            else:
                other_cpu += float(row['CPU Usage (in view) (s)'])

            total_cpu += float(row['CPU Usage (in view) (s)'])

    return (
        f'{t0:.3f}',
        f'{t1:.3f}', 
        f'{total_cpu:.4f}', 
        f'{idle_cpu:.4f}',
        f'{other_cpu:.4f}',
        f'{process_cpu:.4f}',
        f'{(idle_cpu / total_cpu * 100):.2f}',
        f'{(other_cpu / total_cpu * 100):.2f}',
        f'{(process_cpu / total_cpu * 100):.2f}')


def treduce(data):
    result = []

    t0 = data[0][0]

    for d in data:
        d = list(d)
        t = float(d[0])
        d[0] = f'{t - float(t0):.3f}'
        
        if float(d[0]) >= 10.0:
            return result
        
        result.append(d)
    
    return result

def tmerge(data):
    # Merge data
    size = len(data) // 80
    result = []

    print(size)

    for d in data:
        print(d)

    for i in range(0, len(data), size):
        t0 = data[i][0]
        t1 = data[i+size][1]
        total_cpu = 0.00
        idle_cpu = 0.00
        other_cpu = 0.00
        process_cpu = 0.00

        for d in data[i:i+size]:
            total_cpu += float(d[2])
            idle_cpu += float(d[3])
            other_cpu += float(d[4])
            process_cpu += float(d[5])

        result.append((
            f'{t0}',
            f'{t1}',
            f'{total_cpu:.4f}',
            f'{idle_cpu:.4f}',
            f'{other_cpu:.4f}',
            f'{process_cpu:.4f}',
            f'{(process_cpu / idle_cpu * 100):.2f}',
            f'{(process_cpu / other_cpu * 100):.2f}',
            f'{(process_cpu / total_cpu * 100):.2f}'))

    return result

def process(args):

    table = []
    with open(args.cpu_usage_data, 'r') as f:
        # Copy the csv file to a table
        reader = csv.DictReader(f)

        # Copy all rows to table        
        for row in reader:
            table.append(row)

    result = []
    index=0
    for row in table:
        if args.process_name in row['New Process']:
            t0 = float(row['Switch-In Time (s)'])
            t1 = t0 + 0.2

            result.append(sum_range(table, args.process_name, t0-0.2, t1, index))

        index += 1
        
    result = sorted(result, key=lambda x: float(x[0]))

    data = tmerge(treduce(result))

    print('t0 (s), t1 (s), total_cpu (s), idle_cpu (s), other_cpu (s), process_cpu (s), % idle, % other, % cpu usage')
    for r in data:
        # Export as csv
        print(','.join(r))

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('cpu_usage_data', type=str, help='Path to the CSV file containing the CPU usage data')
    parser.add_argument('process_name', type=str, help='Name of the process to analyze')

    args = parser.parse_args()
    process(args)    

if __name__ == '__main__':
    main()

