/*Calcolo della centralità armonica con HyperBall*/

package analyses;

import it.unimi.dsi.webgraph.ImmutableGraph;
import it.unimi.dsi.webgraph.algo.HyperBall;
import it.unimi.dsi.logging.ProgressLogger;
import it.unimi.dsi.webgraph.Transform;

import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.concurrent.*;

public class Centrality {

    public static void main(String[] args) throws Exception {
        if (args.length < 2) {
            System.err.println("Usage: Centrality <chunkDirectory> <outputDirectory>");
            System.exit(1);
        }

        String chunkDirectory = args[0];
        String outputDirectory = args[1];

        Path graphPrefix = Paths.get(chunkDirectory, "graph_output");
        if (!Files.exists(graphPrefix.resolveSibling("graph_output.graph"))) {
            System.err.println("Graph file does not exist in: " + chunkDirectory);
            return;
        }

        Path outputDirPath = Paths.get(outputDirectory);
        if (!Files.exists(outputDirPath)) {
            Files.createDirectories(outputDirPath);
        }

        int numThreads = Runtime.getRuntime().availableProcessors();
        System.out.println("Using " + numThreads + " threads.");

        ExecutorService executor = Executors.newWorkStealingPool(numThreads);

        String chunkName = Paths.get(chunkDirectory).getFileName().toString();
        String outputFile = Paths.get(outputDirectory, chunkName + "_centrality.csv").toString();

        Callable<Void> task = () -> {
            try {
                System.out.println("Processing chunk: " + chunkName);

                ImmutableGraph graph = ImmutableGraph.load(graphPrefix.toString());

                ProgressLogger pl = new ProgressLogger();
                pl.logInterval = 100000;

                int log2m = 8;

                HyperBall hyperBall = new HyperBall(graph, graph, log2m, pl, numThreads, 0, 0, false, false, true, null, 0);

                hyperBall.init();

                while (true) {
                    hyperBall.iterate();
                    if (hyperBall.modified() == 0) {
                        break;
                    }
                }

                float[] harmonic = hyperBall.sumOfInverseDistances;
                if (harmonic == null) {
                    System.err.println("Error: Harmonic centrality array is null for chunk: " + chunkName);
                    return null;
                }

                writeResults(outputFile, harmonic, graph.numNodes());

                System.out.println("Finished processing chunk: " + chunkName);
            } catch (Exception e) {
                System.err.println("Error processing chunk: " + chunkName + " -> " + e.getMessage());
                e.printStackTrace();
            }
            return null;
        };

        executor.submit(task).get();
        executor.shutdown();
        executor.awaitTermination(1, TimeUnit.HOURS);
    }

    private static void writeResults(String outputFile, float[] harmonic, int numNodes) throws Exception {
        List<String> lines = new ArrayList<>();
        lines.add("IdNode,HarmonicCentrality");

        for (int i = 0; i < numNodes; i++) {
            lines.add(String.format(Locale.US, "%d,%.6f", i, harmonic[i]));
        }

        Path path = Paths.get(outputFile);
        Files.write(path, lines, StandardCharsets.UTF_8, StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);
    }
}
