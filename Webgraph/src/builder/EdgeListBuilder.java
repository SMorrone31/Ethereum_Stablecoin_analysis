/*Il codice ha lo scopo di elaborare un file di input contenente le transazioni e costruire una lista di archi per rappresentare il grafo con WebGraph.
 Per ogni transazione assegna ID numerici ai nodi, aggrega gli archi tra coppie di nodi.
 */

package builder;

import java.io.*;
import java.nio.file.*;
import java.util.*;

public class EdgeListBuilder {

    // Mappa per tenere traccia degli ID dei nodi
    private static final Map<Integer, Integer> nodeIdMap = new HashMap<>();
    private static int nextId = 0;  // Contatore per assegnare nuovi ID ai nodi

    // Mappa per gestire gli archi aggregati con ordinamento naturale
    private static final Map<Pair, EdgeData> aggregatedEdges = new TreeMap<>();

    public static void main(String[] args) {
        if (args.length < 4) {
            System.err.println("Usage: EdgeListBuilder <inputFile> <outputFileNumber> <outputFileValue> <mappingDir>");
            System.exit(1);
        }

        String inputFile = args[0];
        String outputFileNumber = args[1];  // Output per numero di transazioni
        String outputFileValue = args[2];   // Output per valore delle transazioni
        String mappingDir = args[3];        // Directory dove salvare il file di mapping

        try {
            processFile(inputFile);
            writeEdgeList(outputFileNumber, outputFileValue);
            writeMappingFile(mappingDir, inputFile);
        } catch (IOException e) {
            System.err.println("Error processing file: " + e.getMessage());
            e.printStackTrace();
        }
    }

    // Legge il file di input riga per riga
    private static void processFile(String inputFile) throws IOException {
        try (BufferedReader reader = Files.newBufferedReader(Paths.get(inputFile))) {
            String line = reader.readLine();

            // Salta l'intestazione, se presente
            if (line != null && line.toLowerCase().contains("blockid")) {
                line = reader.readLine();  // Salta l'intestazione
            }

            // Elabora ogni riga del file di input
            while (line != null) {
                processLine(line);
                line = reader.readLine();
            }
        }
    }

    // Processa una riga e aggiorna la mappa degli archi
    private static void processLine(String line) {
        String[] parts = line.split(",");
        if (parts.length != 6) {
            System.err.println("Malformed line: " + line);
            return;
        }

        try {
            int from = Integer.parseInt(parts[2].trim());  // Nodo mittente
            int to = Integer.parseInt(parts[3].trim());    // Nodo destinatario
            double value = Double.parseDouble(parts[4].trim());

            // Ottiene o crea gli ID per i nodi
            int u = getOrCreateId(from);
            int v = getOrCreateId(to);

            // Crea una chiave di arco ordinata e aggrega i dati
            Pair edgeKey = new Pair(u, v);
            aggregatedEdges.compute(edgeKey, (key, data) -> {
                if (data == null) {
                    return new EdgeData(1, value);
                }
                data.incrementCount();
                data.addValue(value);
                return data;
            });

        } catch (NumberFormatException e) {
            System.err.println("Number parsing error in line: " + line + " -> " + e.getMessage());
        }
    }

    private static int getOrCreateId(int node) {
        return nodeIdMap.computeIfAbsent(node, k -> nextId++);
    }

    // Scrive la lista degli archi nei file di output
    private static void writeEdgeList(String outputFileNumber, String outputFileValue) throws IOException {
        createDirectories(outputFileNumber, outputFileValue);

        try (BufferedWriter outNumber = Files.newBufferedWriter(Paths.get(outputFileNumber));
             BufferedWriter outValue = Files.newBufferedWriter(Paths.get(outputFileValue))) {

            for (Map.Entry<Pair, EdgeData> entry : aggregatedEdges.entrySet()) {
                int u = entry.getKey().getFrom();
                int v = entry.getKey().getTo();
                EdgeData data = entry.getValue();

                outNumber.write(String.format("%d\t%d\t%d%n", u, v, data.getCount()));  // Numero di transazioni
                outValue.write(String.format("%d\t%d\t%.18f%n", u, v, data.getValue()));   // Valore delle transazioni come double
            }

            System.out.println("Transaction count and value files created successfully.");
        }
    }

    // Scrive il file di mappatura (associa l'indirizzo del nodo all'ID creato)
    private static void writeMappingFile(String mappingDir, String inputFile) throws IOException {
        String chunkName = extractChunkName(inputFile);

        Path mappingDirPath = Paths.get(mappingDir);
        if (!Files.exists(mappingDirPath)) {
            Files.createDirectories(mappingDirPath);
        }

        // Scrittura del file di mapping
        String mappingFile = mappingDir + "/" + chunkName + "_mapping.csv";
        try (BufferedWriter writer = Files.newBufferedWriter(Paths.get(mappingFile))) {
            writer.write("address,node_id\n");
            for (Map.Entry<Integer, Integer> entry : nodeIdMap.entrySet()) {
                writer.write(entry.getKey() + "," + entry.getValue() + "\n");
            }
            System.out.println("Mapping file created for chunk: " + chunkName);
        }
    }

    // Metodo per estrarre il nome del chunk dal file di input
    private static String extractChunkName(String filePath) {
        Path path = Paths.get(filePath);
        String fileName = path.getFileName().toString();
        return fileName.replace(".csv", "");  // Rimuove l'estensione .csv
    }

    private static void createDirectories(String outputFileNumber, String outputFileValue) throws IOException {
        Path outputPathNumber = Paths.get(outputFileNumber).getParent();
        Path outputPathValue = Paths.get(outputFileValue).getParent();

        if (outputPathNumber != null && !Files.exists(outputPathNumber)) {
            Files.createDirectories(outputPathNumber);
        }

        if (outputPathValue != null && !Files.exists(outputPathValue)) {
            Files.createDirectories(outputPathValue);
        }
    }
}
