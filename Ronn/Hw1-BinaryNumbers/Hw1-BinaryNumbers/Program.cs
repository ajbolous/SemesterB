using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hw1_BinaryNumbers
{
    class Number
    {
        int n;
        int[] digits;// every index has or 0 or 1
        private string hexString;
        public Number()
        {
            
            
        }
        public Number(String value,int n)
        {
            this.n = powerOfTwo(n);
            this.digits = new int[n];
            for (int i = value.Length-1; i >= 0; i--)
            {
                if (value[i] == 0)
                    this.digits[i] = 0;
                else
                    if (value[i] == '1')
                    this.digits[i] = 1;

            }
        }
        static public int powerOfTwo(int num)
        {
            int powerOfTwo = 1;

            while (powerOfTwo < num && powerOfTwo < int.MaxValue)
                powerOfTwo *= 2;
            return powerOfTwo;
        }
        public String HexString
        {
            get { return hexString; }
            set {
                
                string binaryval = "";
                this.hexString = value;

                 binaryval = Convert.ToString(Convert.ToInt32(hexString, 16), 2); ;
                this.n = powerOfTwo(binaryval.Length);
                this.digits = new int[this.n];
                int i, j;
                for ( i = n - 1, j = binaryval.Length-1 ;  i >=n- binaryval.Length; i--,j--)
                {
                    this.digits[i] = (int)binaryval[j] - '0';
                    
                    //Console.WriteLine("{0}", binaryval[i]);
                }
                for ( i = n-binaryval.Length - 1; i >=0 ; i--)
                    this.digits[i] = 0;
               
               
                
            }
        }
        public int this[int i]
        {
            get
            {
                return digits[i];
            }
        }
      public  string HexConverted(string strBinary)
        {
            string strHex = Convert.ToInt32(strBinary, 2).ToString("X");
            return strHex;
        }
        public int N { get { return n; } }
        public override string ToString()
        {
            string binaryval = "";
            return hexString;

        }
        public static String operator +(Number b1,Number b2)
        {
            int i, f, j, k, carry = 0;
            int n;
            string str;
            n = (b1.n > b2.n ? b1.n : b2.n);
           
            n = powerOfTwo(n);

            Number num = new Number();
            num.n = n;
            num.digits = new int[n];
            num.hexString = b1.hexString + b2.hexString;
            k = n - 1;

            for (i = b1.n - 1, j = b2.n - 1; i > 0 && j > 0; i--, j--)
            {
                int sum = b1.digits[i] + b2.digits[j] + carry;
                if (sum == 2)
                {
                    carry = 1;
                    sum = 0;
                }
                if (sum == 1)
                {
                    carry = 0;
                    sum = 1;
                }
                if (sum == 3)
                {
                    sum = 1;
                    carry = 1;
                }

                num.digits[k] = sum;
               
                k--;
            }
           

            return string.Join("", num.digits);// have to return a string 

        }

        public static implicit operator Number(string s)
        {
            int i, j;
            Number num = new Number(s,s.Length);
           
            num.hexString = s;
           
            return num;
        }

    }

    class Program
    {
       
        static void Main(string[] args)
        {
            Number n1, n2, n3;
            int i;
           
            n1 = new Number();
            n2 = new Number();
            n1.HexString = "F4215";
            n2.HexString = "A352B";
            n3 = n1 + n2;

            Console.WriteLine("n1.N = " + n1.N);

            Console.WriteLine("n2.N = " + n2.N);
          
            Console.WriteLine("n3.N = " + n3.N);
           
            
            Console.WriteLine("{0} + {1} ={2} ",
                     n1.ToString(), n2.ToString(),n3.ToString());
            


            Console.WriteLine("{0} + {1} = {2}",
                       n1.HexString, n2.HexString, n3.HexString);
            Console.ReadLine();
            /*Console.WriteLine("digits n1:");
            for (i = 0; i < n1.N; i++)
                Console.Write(" {0} ", n1[i]);
            Console.WriteLine();
            n3 = n1 * n2;

            Console.WriteLine("n1.N = " + n1.N);

            Console.WriteLine("n2.N = " + n2.N);

            Console.WriteLine("n3.N = " + n3.N);

            Console.WriteLine("{0} * {1} = {2}",
                     n1.ToString(), n2.ToString(), n3.ToString());

            Console.WriteLine("{0} * {1} = {2}",
                     n1.HexString, n2.HexString, n3.HexString);

            Console.WriteLine("n1.N = " + n1.N);

            n1 = "1011110";
            n2 = "1100011";
            n3 = n1 + n2;

            Console.WriteLine("n1.N = " + n1.N);

            Console.WriteLine("n2.N = " + n2.N);

            Console.WriteLine("n3.N = " + n3.N);

            Console.WriteLine("{0} + {1} = {2}",
                     n1.ToString(), n2.ToString(), n3.ToString());

            Console.WriteLine("{0} + {1} = {2}",
                     n1.HexString, n2.HexString, n3.HexString);

            n3 = n1 * n2;

            Console.WriteLine("n1.N = " + n1.N);

            Console.WriteLine("n2.N = " + n2.N);

            Console.WriteLine("n3.N = " + n3.N);

            Console.WriteLine("{0} * {1} = {2}",
                     n1.ToString(), n2.ToString(), n3.ToString());

            Console.WriteLine("{0} * {1} = {2}",
                     n1.HexString, n2.HexString, n3.HexString);

            Console.WriteLine("n1.N = " + n1.N);



            n1 =
            new
           Number("1100011010101110011111", 128);
            n2 =
           new
           Number("1011101001111000001111", 128);

            Console.WriteLine("n1.N = " + n1.N);

            Console.WriteLine("n2.N = " + n2.N);

            Console.WriteLine("n3.N = " + n3.N);

            n3 = n1 + n2;

            Console.WriteLine("{0} + {1} = {2}",
                     n1.ToString(), n2.ToString(), n3.ToString());

            Console.WriteLine("{0} + {1} = {2}",
                     n1.HexString, n2.HexString, n3.HexString);

            n3 = n1 * n2;
            Console.WriteLine("n1.N = " + n1.N);

            Console.WriteLine("n2.N = " + n2.N);

            Console.WriteLine("n3.N = " + n3.N);

            Console.WriteLine("{0} * {1} = {2}",
                     n1.ToString(), n2.ToString(), n3.ToString());

            Console.WriteLine("{0} * {1} = {2}",
                     n1.HexString, n2.HexString, n3.HexString);



            n1 =
            new
           Number("11000110101011100111110001111111001111001111100111100110011", 128);
            n2 =
           new
           Number("10111010011110000011111111111100000111000111000011100011111", 128);

            n3 = n1 + n2;

            Console.WriteLine("n1.N = " + n1.N);

            Console.WriteLine("n2.N = " + n2.N);

            Console.WriteLine("n3.N = " + n3.N);

            Console.WriteLine("{0} + {1} = {2}",
                     n1.ToString(), n2.ToString(), n3.ToString());

            Console.WriteLine("{0} + {1} = {2}",
                     n1.HexString, n2.HexString, n3.HexString);

            n3 = n1 * n2;

            Console.WriteLine("n1.N = " + n1.N);

            Console.WriteLine("n2.N = " + n2.N);

            Console.WriteLine("n3.N = " + n3.N);

            Console.WriteLine("{0} * {1} = {2}",
                     n1.ToString(), n2.ToString(), n3.ToString());

            Console.WriteLine("{0} * {1} = {2}",
                     n1.HexString, n2.HexString, n3.HexString);*/


        }
    }
}
