/*Il codice suddivide i dati delle transazioni in blocchi temporali in base ai timestamp dei blocchi Ethereum. Le finestre temporali, o chunk, sono su base mensile.
* Il programma legge i timestamp dei blocchi da un file e li memorizza in una struttura dati thread-safe. Poi converte ogni timestamp Unix in un periodo "MMMYY" e utilizza questa informazione per suddividere i dati di input in file corrispondenti ai vari periodi.*/

package builder;

import java.io.*;
import java.nio.file.*;
import java.time.*;
import java.time.format.*;
import java.util.*;
import java.util.concurrent.*;

public class TemporalChunkDivider {

    // limite massimo di periodo (Giugno 2022)
    private static final YearMonth maxDate = YearMonth.of(2022, Month.JUNE);

    // Converte un timestamp Unix in un periodo "MMMYY" (es. Feb20), limitato a maxDate
    private static String timestampToPeriod(long timestamp) {
        try {
            LocalDateTime dateTime = LocalDateTime.ofInstant(Instant.ofEpochSecond(timestamp), ZoneOffset.UTC);
            YearMonth periodDate = YearMonth.from(dateTime);

            // Controlla che il periodo non sia oltre il limite
            if (periodDate.isAfter(maxDate)) {
                return "ExceedsMaxDate";  // Usa un nome speciale per evitare la creazione di chunk
            }
            return dateTime.format(DateTimeFormatter.ofPattern("MMMYY", Locale.ENGLISH));
        } catch (DateTimeException e) {
            System.err.println("Error formatting timestamp: " + timestamp + " -> " + e.getMessage());
            return "Unknown";
        }
    }

    // Legge i timestamp dei blocchi e li mappa in un ConcurrentHashMap (thread-safe)
    private static Map<Integer, Long> readBlockTimestamps(String filename) throws IOException {
        Map<Integer, Long> blockTimestamps = new ConcurrentHashMap<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(filename), 8192)) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");
                if (parts.length == 2) {
                    int blockId = Integer.parseInt(parts[0]);
                    long timestamp = Long.parseLong(parts[1]);

                    // Verifica che il timestamp rientri nel limite di Giugno 2022
                    if (YearMonth.from(LocalDateTime.ofInstant(Instant.ofEpochSecond(timestamp), ZoneOffset.UTC)).isAfter(maxDate)) {
                        continue;
                    }
                    blockTimestamps.put(blockId, timestamp);
                }
            }
        }
        return blockTimestamps;
    }

    // Suddivide i trasferimenti in chunk temporali e li salva su file (ottimizzato)
    private static void processTransfersByTimeChunks(String transfersFile, String valuesFile, Map<Integer, Long> blockTimestamps, String outputDir) throws IOException, InterruptedException {
        Path chunkDir = Paths.get(outputDir);  // Usa il percorso di output specificato
        Files.createDirectories(chunkDir);

        // Usa ExecutorService per parallelizzare la scrittura su file
        ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());

        try (BufferedReader transfersReader = new BufferedReader(new FileReader(transfersFile), 8192);
             BufferedReader valuesReader = new BufferedReader(new FileReader(valuesFile), 8192)) {

            String transferLine;
            String valueLine;
            Map<String, List<String>> batchMap = new HashMap<>();
            int batchSize = 10000;

            while ((transferLine = transfersReader.readLine()) != null && (valueLine = valuesReader.readLine()) != null) {
                String[] parts = transferLine.split(",");
                if (parts.length != 4) {
                    System.err.println("Error in transfer line format: " + transferLine);
                    continue;
                }

                try {
                    int blockId = Integer.parseInt(parts[0]);
                    int contractId = Integer.parseInt(parts[1]);
                    int from = Integer.parseInt(parts[2]);
                    int to = Integer.parseInt(parts[3]);
                    double value = Double.parseDouble(valueLine);

                    if (blockTimestamps.containsKey(blockId)) {
                        long timestamp = blockTimestamps.get(blockId);
                        String period = timestampToPeriod(timestamp);

                        // Salta i trasferimenti con periodi dopo il limite
                        if (period.equals("ExceedsMaxDate")) {
                            continue;
                        }

                        String formattedTransfer = String.join(",", String.valueOf(blockId), String.valueOf(contractId),
                                String.valueOf(from), String.valueOf(to), String.valueOf(value), String.valueOf(timestamp));

                        batchMap.computeIfAbsent(period, k -> new ArrayList<>()).add(formattedTransfer);
                    }
                } catch (NumberFormatException e) {
                    System.err.println("Error parsing line data: " + transferLine + " -> " + e.getMessage());
                }

                if (batchMap.size() >= batchSize) {
                    Map<String, List<String>> batchToWrite = new HashMap<>(batchMap);
                    executor.submit(() -> writeBatchesToFile(batchToWrite, chunkDir));  // Parallelizza la scrittura
                    batchMap.clear();
                }
            }

            // Scrive i batch rimanenti
            if (!batchMap.isEmpty()) {
                executor.submit(() -> writeBatchesToFile(batchMap, chunkDir));
            }

        } finally {
            // Attende il completamento di tutti i thread
            executor.shutdown();
            executor.awaitTermination(1, TimeUnit.HOURS);
        }
    }

    // Scrive i batch su file in modo thread-safe e parallelo
    private static void writeBatchesToFile(Map<String, List<String>> batchMap, Path chunkDir) {
        batchMap.forEach((period, transfers) -> {
            if (period.equals("ExceedsMaxDate") || period.equals("Unknown")) return;  // Salta i periodi fuori limite
            Path filePath = chunkDir.resolve(period + ".csv");
            try (BufferedWriter writer = Files.newBufferedWriter(filePath, StandardOpenOption.CREATE, StandardOpenOption.APPEND)) {
                if (Files.size(filePath) == 0) {
                    writer.write("BlockID,ContractID,From,To,Value,Timestamp\n");
                }
                for (String transfer : transfers) {
                    writer.write(transfer + "\n");
                }
            } catch (IOException e) {
                System.err.println("Error writing to file: " + filePath + " -> " + e.getMessage());
            }
        });
    }

    public static void main(String[] args) {
        if (args.length < 4) {
            System.err.println("Usage: java TemporalChunkDivider <block_timestamps_file> <transfers_file> <values_file> <output_directory>");
            return;
        }

        long startTime = System.nanoTime();

        try {
            Map<Integer, Long> blockTimestamps = readBlockTimestamps(args[0]);
            processTransfersByTimeChunks(args[1], args[2], blockTimestamps, args[3]);
        } catch (IOException | InterruptedException e) {
            System.err.println("Error processing transfers: " + e.getMessage());
        }

        long endTime = System.nanoTime();
        long duration = TimeUnit.NANOSECONDS.toSeconds(endTime - startTime);
        System.out.println("Process completed in " + duration + " seconds.");
    }
}
