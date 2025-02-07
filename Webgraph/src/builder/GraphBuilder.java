package builder;

import it.unimi.dsi.webgraph.BVGraph;

import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;

public class GraphBuilder {

    public static void main(String[] args) throws Exception {
        if (args.length < 3) {
            System.err.println("Usage: GraphBuilder <inputFile> <outputDirectory> <outputPrefix>");
            System.exit(1);
        }

        long startTime = System.nanoTime();

        String inputFile = args[0];
        String outputDirectory = args[1];
        String outputPrefix = args[2];

        // Crea il file di output per WebGraph con il prefisso specificato
        String outputFilePrefix = Paths.get(outputDirectory, outputPrefix).toString();

        // Esegue il comando per generare il grafo con WebGraph
        try {
            BVGraph.main(("-g ArcListASCIIGraph " + inputFile + " " + outputFilePrefix).split(" "));
        } catch (Exception e) {
            System.err.println("Error processing graph: " + inputFile + " -> " + e.getMessage());
            e.printStackTrace();
        }

        long endTime = System.nanoTime();
        long duration = TimeUnit.NANOSECONDS.toSeconds(endTime - startTime);

        System.out.println("Graph built successfully in " + duration + " seconds.");
    }
}
