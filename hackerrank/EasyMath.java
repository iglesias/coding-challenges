import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;

public class EasyMath {

    private static boolean isAllFours(BigInteger n) {
        String str = n.toString();
        for (int i = 0; i < str.length(); i++)
            if (str.charAt(i) == '0') return false;
        return true;
    }
    
    private static BigInteger next(BigInteger n) {        
        if (isAllFours(n)) {
            int numDigits = n.toString().length();
            n = BigInteger.valueOf(4);
            for (int i = 0; i < numDigits; i++)
                n = n.multiply(BigInteger.TEN);
            return n;
        } else {
            String str = n.toString();
            int i = 0;
            while (i < str.length() && str.charAt(i) == '4') i++;
            i = str.length()-1-i;
            BigInteger summand = BigInteger.valueOf(4);
            while (i > 0)
            {
                i--;
                summand = summand.multiply(BigInteger.TEN);
            }
            n = n.add(summand);
            return n;
        }
    }
    
    private static int solve(BigInteger n) {
        String str = n.toString();
        
        int i = 0;
        while (i < str.length() && str.charAt(i) == '4') i++;
        int a = i;
        int b = str.length()-a;
        return 2*a + b;
    }
    
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int T = sc.nextInt();
        for (int t = 0; t < T; t++) {
            BigInteger X = BigInteger.valueOf(sc.nextInt());
            BigInteger n = BigInteger.valueOf(4);
            while (n.compareTo(X) < 0) {
                n = next(n);
                System.out.println(n);
            }
            
            while (n.remainder(X).compareTo(BigInteger.ZERO) != 0) {
                n = next(n);
                System.out.println(n);
            }
            
            System.out.println(solve(n));
        }
    }
}
