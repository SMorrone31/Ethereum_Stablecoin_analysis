# questo codice estrare tutte le transazioni effettuate dall'indirizzo 0x, e i corrispettivi valori nell'altro file alla stessa altezza

import pandas as pd

# Specifico i nomi dei file di input e output: vengono cambiati poi con usdc, wrapped_ether e dai.
transfer_file = '/data/morroneT/data/transfer_extract/transfers/tether.csv'
values_file = '/data/morroneT/data/transfer_extract/values/tether.csv'
transfer_output_file = '/data/morroneT/data/transfer_extract/transfers/tether_filtered.csv'
values_output_file = '/data/morroneT/data/transfer_extract/values/tether_filtered.csv'

df_transfers = pd.read_csv(transfer_file,header=None)
df_values = pd.read_csv(values_file,header=None)

if len(df_transfers) != len(df_values):
    print(f"Le dimensioni dei file non corrispondono:")
    print(f"Numero di righe nel file delle transazioni: {len(df_transfers)}")
    print(f"Numero di righe nel file dei valori: {len(df_values)}")

# Filtra le righe dove né la terza né la quarta colonna nel file delle transazioni contengono "0"
mask = (df_transfers.iloc[:, 2] != 0) & (df_transfers.iloc[:, 3] != 0)
df_transfers_filtered = df_transfers[mask].reset_index(drop=True)
df_values_filtered = df_values[mask].reset_index(drop=True)

df_transfers_filtered.to_csv(transfer_output_file, index=False,header=False)
df_values_filtered.to_csv(values_output_file, index=False, header=False)

print(f"Filtraggio completato. I nuovi file '{transfer_output_file}' e '{values_output_file}' sono stati creati.")
