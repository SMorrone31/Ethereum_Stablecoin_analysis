package builder;

public class EdgeData {
    private int count;
    private double value;

    public EdgeData(int count, double value) {
        this.count = count;
        this.value = value;
    }

    public void incrementCount() {
        count++;
    }

    public void addValue(double value) {
        this.value += value;
    }

    public int getCount() {
        return count;
    }

    public double getValue() {
        return value;
    }
}