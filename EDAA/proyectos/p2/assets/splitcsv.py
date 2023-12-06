import pandas as pd

# Lee el archivo CSV original
df = pd.read_csv('1.csv')

# Crea el primer archivo CSV con las columnas n, t1kmp, v1kmp, t1sa, etc.
csv1 = df[['n', 't1kmp', 'v1kmp', 't1sa','v1sa','t1fmi','v1fmi']]  # Agrega las columnas que desees
csv1.to_csv('11.csv', index=False, float_format='%.5e')

# Crea el segundo archivo CSV con las columnas n, t2kmp, v2kmp, t2sa, etc.
csv2 = df[['n', 't2kmp', 'v2kmp', 't2sa', 'v2sa','t2fmi','v2fmi']]  # Agrega las columnas que desees
csv2.to_csv('12.csv', index=False, float_format='%.5e')

# Crea el tercer archivo CSV con las columnas n, skmp, ssa, t1fmi, v1fmi, etc.
csv3 = df[['n', 'skmp', 'ssa','sfmi']]  # Agrega las columnas que desees
csv3.to_csv('13.csv', index=False, float_format='%.5e')
