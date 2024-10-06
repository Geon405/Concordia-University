import java.io.*;

public class LinearOddonacci {

    // Method to calculate the Oddonacci number recursively
    public static long[] linearOdd(int n) {
        // Base case: for the first three Oddonacci numbers, return an array with all values set to 1
        if (n <= 3) {
            long[] answer = {1, 1, 1};
            return answer;
        } else {
            // Recursive call to calculate Oddonacci(n - 1)
            long[] temp = linearOdd(n - 1);
            // Calculate Oddonacci(n) by summing the last three computed values
            long[] answer = {temp[0] + temp[1] + temp[2], temp[0], temp[1]};
            return answer;
        }
    }

    public static void main(String[] args) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter("LinearOddoOut.txt", true))) {

            long startTime;
            long endTime;
            long elapsed;

            // Loop through values of n from 5 to 200, incrementing by 5
            for (int i = 5; i <= 200; i += 5) {
                startTime = System.currentTimeMillis();
                long result = linearOdd(i)[0];  // Call linearOdd to compute the Oddonacci number for n
                endTime = System.currentTimeMillis();

                elapsed = endTime - startTime;

                bw.write("LinearOdd(" + i + "): " + result + "\t time taken to run the algorithm: " + elapsed + "ms\n");
            }
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}