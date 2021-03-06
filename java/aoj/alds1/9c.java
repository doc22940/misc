import java.util.*;

class Main {
    class PriorityQueue {
        private List<Long> heap;

        public PriorityQueue() {
            this.heap = new ArrayList<>();
        }

        public void insert(long key) {
            this.heap.add(key);
            int i = this.heap.size() - 1;

            while (i > 0 && this.heap.get(this.parentIndexOf(i)) < this.heap.get(i)) {
                long tmp = this.heap.get(this.parentIndexOf(i));
                this.heap.set(this.parentIndexOf(i), this.heap.get(i));
                this.heap.set(i, tmp);
                i = this.parentIndexOf(i);
            }
        }

        public long extractMax() {
            long result = this.heap.get(0);
            if (this.heap.size() > 1) {
                this.heap.set(0, this.heap.remove(this.heap.size() - 1));
                this.maxHeapify(0);
            }
            return result;
        }

        private void maxHeapify(int i) {
            int largest = i;

            int l = this.leftIndexOf(i);
            if (l < this.heap.size() && this.heap.get(l) > this.heap.get(largest)) {
                largest = l;
            }

            int r = this.rightIndexOf(i);
            if (r < this.heap.size() && this.heap.get(r) > this.heap.get(largest)) {
                largest = r;
            }

            if (largest != i) {
                long tmp = this.heap.get(i);
                this.heap.set(i, this.heap.get(largest));
                this.heap.set(largest, tmp);
                this.maxHeapify(largest);
            }
        }

        private int parentIndexOf(int i) {
            return ((i + 1) / 2) - 1;
        }

        private int leftIndexOf(int i) {
            return ((i + 1) * 2) - 1;
        }

        private int rightIndexOf(int i) {
            return (i + 1) * 2;
        }
    }

    public static void main(String[] args) {
        new Main().main();
    }

    public void main() {
        PriorityQueue queue = new PriorityQueue();

        Scanner scanner = new Scanner(System.in);
        while (true) {
            String[] line = scanner.nextLine().split(" ", 2);
            if (line[0].equals("insert")) {
                queue.insert(Long.parseLong(line[1]));
            } else if (line[0].equals("extract")) {
                System.out.println(queue.extractMax());
            } else if (line[0].equals("end")) {
                break;
            } else {
                throw new RuntimeException("unexpected operation: " + line[0]);
            }
        }
    }
}
