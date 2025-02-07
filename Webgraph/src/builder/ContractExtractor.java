/*Questo codice ha lo scopo di estrarre transazioni associate a specifici contratti di stablecoin da due file di input:
* uno che contiene informazioni sulle transazioni e uno che contiene i valori delle transazioni. Il programma identifica le transazioni
*  che appartengono a uno specifico contratto e scrive queste transazioni in due file di output separati, uno per le transazioni stesse e uno per i valori associati a ciascuna transazione.
* Quindi ogni transazione ha il suo corrispettivo valore nell'altro file alla stessa altezza. */

package builder;

import java.io.*;
import java.util.HashMap;
import java.util.Map;

public class ContractExtractor {

    // Mappa per collegare l'hash della stablecoin all'ID numerico
    private static final Map<String, Integer> STABLECOIN_CONTRACTS = new HashMap<>();

    static {
        // Popoliamo la mappa con gli indirizzi delle stablecoin e il loro ID numerico
        STABLECOIN_CONTRACTS.put("dac17f958d2ee523a2206206994597c13d831ec7", 12071); // tether
        STABLECOIN_CONTRACTS.put("c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2", 13532); // wrapped_ether
        STABLECOIN_CONTRACTS.put("a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48", 65331); // usdc
        STABLECOIN_CONTRACTS.put("6b175474e89094c44da98b954eedeac495271d0f", 124573); // dai
    }

    public static void main(String[] args) {
        final String transferFile = args[0];
        final String valuesFile = args[1];
        final String stablecoin = args[2].toLowerCase();  // Stablecoin hash
        final String outputTransferFile = args[3];
        final String outputValuesFile = args[4];

        // Trova il contractId associato allo stablecoin hash
        Integer targetContractId = STABLECOIN_CONTRACTS.get(stablecoin);
        if (targetContractId == null) {
            System.err.println("Errore: Stablecoin non trovato per l'hash fornito.");
            System.exit(1);
        }

        try (
                BufferedReader tin = new BufferedReader(new InputStreamReader(new FileInputStream(transferFile), "UTF-8"), 8192 * 4); // Buffer più grande per lettura
                BufferedReader vin = new BufferedReader(new InputStreamReader(new FileInputStream(valuesFile), "UTF-8"), 8192 * 4);
                PrintWriter tout = new PrintWriter(new BufferedWriter(new FileWriter(outputTransferFile, true), 8192 * 4)); // BufferedWriter per output efficiente
                PrintWriter vout = new PrintWriter(new BufferedWriter(new FileWriter(outputValuesFile, true), 8192 * 4));
        ) {
            String transferLine;
            String valueLine;

            // Legge i file riga per riga
            while (((transferLine = tin.readLine()) != null) && ((valueLine = vin.readLine()) != null)) {
                int firstComma = transferLine.indexOf(',');
                int secondComma = transferLine.indexOf(',', firstComma + 1);
                String contractIdStr = transferLine.substring(firstComma + 1, secondComma);
                int contractId = Integer.parseInt(contractIdStr);

                // Se il contractId corrisponde al targetContractId, scrive nei file di output
                if (contractId == targetContractId) {
                    tout.println(transferLine);
                    vout.println(valueLine);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }
}
