/*Calcolo del diametro diretto con la funzione SumSweepDirectedDiameterRadius di WebGraph.*/


package analyses;
import it.unimi.dsi.webgraph.ImmutableGraph;
import it.unimi.dsi.webgraph.algo.SumSweepDirectedDiameterRadius;
import it.unimi.dsi.logging.ProgressLogger;

public class CalculateDirectedDiameter {
    public static void main(String[] args) throws Exception {
        if (args.length < 1) {
            System.err.println("Usage: CalculateDirectedDiameter <graphBasename>");
            System.exit(1);
        }

        String basename = args[0];
        ImmutableGraph graph = ImmutableGraph.load(basename);

        // Definisce il livello di output (in questo caso vogliamo solo il diametro)
        SumSweepDirectedDiameterRadius.OutputLevel outputLevel = SumSweepDirectedDiameterRadius.OutputLevel.DIAMETER;

        // Creo un ProgressLogger
        ProgressLogger pl = new ProgressLogger();

        // Creo l'istanza di SumSweepDirectedDiameterRadius con i parametri corretti
        SumSweepDirectedDiameterRadius sweeper = new SumSweepDirectedDiameterRadius(graph, outputLevel, null, pl);

        // Eseguo il calcolo del diametro
        sweeper.compute();

        // Stampo il diametro calcolato
        System.out.println("Diametro del grafo: " + sweeper.getDiameter());
    }
}