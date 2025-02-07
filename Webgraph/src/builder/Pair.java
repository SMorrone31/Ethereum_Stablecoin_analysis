package builder;

import java.util.Objects;

public class Pair implements Comparable<Pair> {
    private final int from;
    private final int to;

    public Pair(int from, int to) {
        this.from = from;
        this.to = to;
    }

    public int getFrom() {
        return from;
    }

    public int getTo() {
        return to;
    }

    @Override
    public int compareTo(Pair other) {
        int compareFrom = Integer.compare(this.from, other.from);
        if (compareFrom != 0) return compareFrom;
        return Integer.compare(this.to, other.to);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Pair pair = (Pair) o;
        return from == pair.from && to == pair.to;
    }

    @Override
    public int hashCode() {
        return Objects.hash(from, to);
    }
}
