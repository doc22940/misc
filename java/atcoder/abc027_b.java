import java.util.*;

class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n = Integer.parseInt(scanner.nextLine());
        String[] line = scanner.nextLine().split(" ", n);

        for (int i = 0; i < n; i++) {
            int a = Integer.parseInt(line[i]);
        }
    }
}
