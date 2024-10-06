import java.io.*;

public class ExponentialOddonacci {

    // Method to calculate Oddonacci number using multiple recursion
    public static long oddonacciExponential(int n) {
        // Base cases: return 1 for the first four Oddonacci numbers
        if (n == 0 || n == 1 || n == 2 || n == 3) {
            return 1;
        }

        // Recursive calls: calculate Oddonacci(n) by adding the results of Oddonacci(n-1), Oddonacci(n-2), and Oddonacci(n-3)
        return oddonacciExponential(n - 1) + oddonacciExponential(n - 2) + oddonacciExponential(n - 3);
    }

    public static void main(String[] args) {
        // Use a BufferedWriter to write the output to the file named "ExponentialOddoOut.txt"
        try (BufferedWriter bw = new BufferedWriter(new FileWriter("ExponentialOddoOut.txt", true))) {

            long startTime;
            long endTime;
            long elapsed;

            // Loop through values of n from 5 to 200, incrementing by 5
            for (int i = 5; i <= 200; i += 5) {
                startTime = System.currentTimeMillis();
                long result = oddonacciExponential(i);  // Call linearOdd to compute the Oddonacci number for n
                endTime = System.currentTimeMillis();

                elapsed = endTime - startTime;

                bw.write("ExponentialOdd(" + i + "): " + result + "\t time taken to run the algorithm: " + elapsed
                        + "ms\n");
            }
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}