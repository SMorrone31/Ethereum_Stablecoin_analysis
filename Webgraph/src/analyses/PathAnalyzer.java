/*COMMENTATO PERCHE' NON PERFETTAMENTE FUNZIONANTE E NON INCLUSO NELLA TESI*/

/*package analyses;

import it.unimi.dsi.webgraph.ImmutableGraph;
import it.unimi.dsi.webgraph.Transform;
import it.unimi.dsi.webgraph.algo.ConnectedComponents;
import it.unimi.dsi.webgraph.algo.HyperBall;
import it.unimi.dsi.logging.ProgressLogger;

import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;

public class PathAnalyzer {

    public static void main(String[] args) throws Exception {
        if (args.length < 2) {
            System.err.println("Usage: AvgSPL <graphDirectory> <outputFile>");
            System.exit(1);
        }

        String graphDirectory = args[0];
        String outputFile = args[1];

        Path outputFilePath = Paths.get(outputFile);
        if (!Files.exists(outputFilePath)) {
            Files.createFile(outputFilePath);
            Files.write(outputFilePath, Collections.singletonList("ChunkName,AverageSPL"), StandardCharsets.UTF_8, StandardOpenOption.CREATE);
        }

        int numThreads = Runtime.getRuntime().availableProcessors();
        System.out.println("Using " + numThreads + " threads.");

        ExecutorService executor = Executors.newWorkStealingPool(numThreads);

        List<Path> graphDirs = Files.list(Paths.get(graphDirectory))
                .filter(Files::isDirectory)
                .collect(Collectors.toList());

        if (graphDirs.isEmpty()) {
            System.err.println("No graph directories found in: " + graphDirectory);
            System.exit(1);
        }

        System.out.println("Found " + graphDirs.size() + " graph directories. Starting calculations...");

        List<Callable<Void>> tasks = new ArrayList<>();
        for (Path graphDir : graphDirs) {
            String chunkName = graphDir.getFileName().toString();
            Path graphFile = graphDir.resolve("graph_output");

            if (!Files.exists(graphFile.resolveSibling("graph_output.graph"))) {
                System.err.println("Graph file does not exist for chunk: " + chunkName);
                continue;
            }

            tasks.add(() -> {
                try {
                    System.out.println("Processing chunk: " + chunkName);
                    ImmutableGraph graph = ImmutableGraph.load(graphFile.toString());

                    ProgressLogger pl = new ProgressLogger();
                    pl.itemsName = "nodes";
                    pl.expectedUpdates = graph.numNodes();
                    pl.displayFreeMemory = true;
                    pl.logInterval = 5000;

                    // Trasformiamo e semplifichiamo il grafo (rimuove auto-loop e lo rende non orientato)
                    ImmutableGraph simplifiedGraph = Transform.simplify(graph, Transform.transpose(graph));

                    // Calcoliamo la componente connessa più grande
                    ImmutableGraph largestComponent = ConnectedComponents.getLargestComponent(simplifiedGraph, numThreads, pl);

                    // Configuriamo HyperBall per il calcolo della SPL
                    int log2m = 9;  // Precisione limitata
                    HyperBall hyperBall = new HyperBall(
                            largestComponent,
                            null,
                            log2m,
                            pl,
                            numThreads,
                            HyperBall.DEFAULT_BUFFER_SIZE,
                            HyperBall.DEFAULT_GRANULARITY,
                            false,
                            true,
                            false,
                            null,
                            null,
                            12345L
                    );

                    hyperBall.init();
                    hyperBall.run();

                    // Verifica che le distanze non siano null
                    float[] distances = hyperBall.sumOfDistances;
                    if (distances == null) {
                        throw new NullPointerException("sumOfDistances array is null.");
                    }

                    double totalDistance = 0;
                    int numNodes = largestComponent.numNodes();
                    for (int i = 0; i < numNodes; i++) {
                        totalDistance += distances[i];
                    }

                    double avgSPL = totalDistance / (numNodes * (numNodes - 1));

                    String resultLine = String.format(Locale.US, "%s,%.6f", chunkName, avgSPL);
                    Files.write(outputFilePath, Collections.singletonList(resultLine), StandardCharsets.UTF_8, StandardOpenOption.APPEND);

                    System.out.println("Finished processing chunk: " + chunkName);
                } catch (Exception e) {
                    System.err.println("Error processing chunk: " + chunkName + " -> " + e.getMessage());
                    e.printStackTrace();
                }
                return null;
            });
        }

        List<Future<Void>> futures = executor.invokeAll(tasks);
        for (Future<Void> future : futures) {
            future.get();
        }

        executor.shutdown();
        executor.awaitTermination(1, TimeUnit.HOURS);
    }
}
*/

