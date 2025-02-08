
# Questo script utilizza il programma esterno (libreria) plfit per analizzare la distribuzione delle connessioni nei grafi corrispondenti alle transazioni delle 3 stablecoin + WETH. 
# L'obiettivo è stimare la legge di potenza della distribuzione dei gradi in ingresso e in uscita per ogni stablecoin, memorizzando i risultati in un file CSV separato per ciascuna di essa
# estrae valori come alpha, xmin, log-Likelihood, KS Distance e  p-value
import os
import subprocess

# Percorso completo di plfit
plfit_path = "/data/morroneT/plfit/build/src/plfit"

# Percorso base dei file di input e output
# attenzione questi percorsi vengono cambiati in base ai gradi da analizzare
# infatti questa analisi viene fatta sia sui gradi del grafo globale delle transazioni delle monete che su ogni grafo temporale costruito
# ovviamente se parliamo del grafo globale avremo solo una riga di risultati: quindi logicamente per n chukn avremo n righe (n risultat per ogni statistica calcolata)
base_input_dir = "/data/morroneT/igraph_results/results/chunks_stats/InOutDegreeChunk/{stablecoin}/{chunk}/"
output_file_template = "/data/morroneT/igraph_results/results/chunks_stats/power_law/{stablecoin}_results_plfit.csv"

# Lista delle stablecoin
stablecoins = ["dai","tether", "wrapped_ether", "usdc"]

# Funzione per eseguire plfit e estrarre i risultati
def run_plfit(file_path):
    try:
        # Esegue plfit con il comando desiderato e cattura l'output
        result = subprocess.run(
            [plfit_path, file_path, "-p", "exact", "-b"],
            capture_output=True,
            text=True,
            check=True
        )
        output = result.stdout.strip().split()

        # controllo se l'output sia nel formato atteso
        if len(output) >= 7 and output[1] == "D":
            # Estrazione dei valori da plfit
            alpha = output[2]
            xmin = output[3]
            log_likelihood = output[4]
            ks_distance = output[5]
            pvalue = output[6]
        else:
            print(f"Output inatteso da plfit su {file_path}: {output}")
            return None, None, None, None, None

        return alpha, xmin, log_likelihood, ks_distance, pvalue
    except Exception as e:
        print(f"Errore durante l'esecuzione di plfit su {file_path}: {e}")
        return None, None, None, None, None

# Iterazione su ogni stablecoin e ogni chunk
for stablecoin in stablecoins:
    print(f"Inizio elaborazione per {stablecoin}...")
    
    # Percorso di output per il file dei risultati
    output_file = output_file_template.format(stablecoin=stablecoin)
    
    # intestazione 
    with open(output_file, "w") as f_out:
        f_out.write("chunk,InAlpha,OutAlpha,InXmin,OutXmin,InL,OutL,InD,OutD,InPvalue,OutPvalue\n")
        
        # Directory di input
        input_dir = base_input_dir.format(stablecoin=stablecoin, chunk="")
        
        # Elenca i chunk dinamicamente
        for chunk in os.listdir(input_dir):
            chunk_dir = os.path.join(input_dir, chunk)
            if not os.path.isdir(chunk_dir):
                continue

            print(f" Inizio elaborazione chunk {chunk} per {stablecoin}...")

            # Percorsi dei file di input
            indegree_file = os.path.join(chunk_dir, "indegree.csv")
            outdegree_file = os.path.join(chunk_dir, "outdegree.csv")
            
            # Esegue plfit per indegree e outdegree
            in_alpha, in_xmin, in_l, in_d, in_pvalue = run_plfit(indegree_file)
            out_alpha, out_xmin, out_l, out_d, out_pvalue = run_plfit(outdegree_file)
            
            # Stampa di debug per verificare i valori estratti
            print(f"  Risultati per {chunk} - indegree: alpha={in_alpha}, xmin={in_xmin}, log_likelihood={in_l}, ks_distance={in_d}, pvalue={in_pvalue}")
            print(f"  Risultati per {chunk} - outdegree: alpha={out_alpha}, xmin={out_xmin}, log_likelihood={out_l}, ks_distance={out_d}, pvalue={out_pvalue}")
            
            # Scrive i risultati nel file di output gestendo eventuali valori None
            f_out.write(f"{chunk},{in_alpha or ''},{out_alpha or ''},{in_xmin or ''},{out_xmin or ''},{in_l or ''},{out_l or ''},{in_d or ''},{out_d or ''},{in_pvalue or ''},{out_pvalue or ''}\n")

            print(f" Fine elaborazione chunk {chunk} per {stablecoin}.")
    
    print(f"Fine elaborazione per {stablecoin}.")

print("Processo completato e risultati salvati.")
