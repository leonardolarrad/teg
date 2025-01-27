import random
import csv

def generate_number():
    # Genera un número aleatorio entre 23 y 24.96
    num = random.uniform(23, 24.96)
    # Hacemos que los números menores a 24 sean extremadamente raros
    if num < 24:
        if random.random() < 0.01:  # 1% de probabilidad de que el número sea menor a 24
            return num
        else:
            return generate_number()
    return num

def generate_table():
    rows = []
    for _ in range(85):
        row = []
        for _ in range(6):
            while True:
                num_a = generate_number()
                num_b = generate_number()
                if num_a < num_b:
                    break
            row.append(f"{num_a:.2f}")
            row.append(f"{num_b:.2f}")
        rows.append(row)
    return rows

def save_to_csv(rows, filename="output.csv"):
    header = []
    for i in range(6):
        header.extend([f"Columna A{i+1}", f"Columna B{i+1}"])
    
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(header)
        writer.writerows(rows)

# Genera la tabla y guarda en un archivo CSV
rows = generate_table()
save_to_csv(rows)

print("Archivo CSV generado exitosamente.")
