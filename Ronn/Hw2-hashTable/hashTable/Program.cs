using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace hashTable
{
    interface hash_table_interface
    {

        bool is_member(int number);
        void insert(int number);
        int hash_func(int x, int n);
    }


    class array_hash_table : hash_table_interface
    {

        public int n;
        int[] arr;
        public array_hash_table(int n)
        {

            this.n = 2 * n;
            this.arr = new int[this.n];
            for (int i = 0; i < this.n; i++)
                arr[i] = -1;//initialiaze the array with -1 

        }

        public array_hash_table()
        {
        }

        public int hash_func(int x, int n)
        {
            return (x % n);
        }


        public bool is_member(int number)
        {
            int i;
            for (i = 0; i < this.n; i++)
            {
                if (number == this.arr[hash_func(i, n)])
                    return true;
            }
            return false;

        }
        public void insert(int num)
        {

            if (is_member(num))
                return;

            if (this.arr[hash_func(num, n)] == -1)
                this.arr[hash_func(num, n)] = num;
            else
                if (this.arr[hash_func(num, n)] == num)
                return;

            else
            {
                for (int i = num + 1; i < this.n; i++)
                    if (this.arr[hash_func(i, n)] == -1)
                        this.arr[hash_func(i, n)] = num;

            }
        }
    }
    public delegate int func(int number);
    class ifunc
    {
        private Func<int, int, int> alt_hash_fun;

        public ifunc(Func<int, int, int> alt_hash_fun)
        {
            this.alt_hash_fun = alt_hash_fun;
        }

        ifunc()
        {

        }
    }
    class array_hash_table2 : array_hash_table
    {
        private ifunc new_hash_fun;

       
        public array_hash_table2(int n, ifunc new_hash_fun) : base(n) //have to call the constructor of the array_hash_table 2 that's the problem
        {
            this.new_hash_fun = new_hash_fun;
        }
    }
    class new_hash
    {
        public static int alt_hash_fun(int x, int n)
        {
            return (x * x) % n;

        } // alt_hash_fun

    } //  new_hash


    class MainClass
    {
        public static int Main()
        {
            array_hash_table My_table = new array_hash_table(7);

            My_table.insert(39);
            My_table.insert(5);
            My_table.insert(23);
            My_table.insert(9);
            My_table.insert(3);
            My_table.insert(2);


            Console.WriteLine("\nIs member {0} = {1}", 23L, My_table.is_member(23));
            Console.WriteLine("\nIs member {0} = {1}", 39L, My_table.is_member(39));
            Console.WriteLine("\nIs member {0} = {1}", 40L, My_table.is_member(40));

            ifunc new_hash_fun = new ifunc(new_hash.alt_hash_fun);

            array_hash_table2 My_table2 = new array_hash_table2(9, new_hash_fun);

            My_table2.insert(39);
            My_table2.insert(5);
            My_table2.insert(23);
            My_table2.insert(9);
            My_table2.insert(3);
            My_table2.insert(2);


            Console.WriteLine("\nIs member {0} = {1}", 23L, My_table2.is_member(23));
            Console.WriteLine("\nIs member {0} = {1}", 39L, My_table2.is_member(39));
            Console.WriteLine("\nIs member {0} = {1}", 40L, My_table2.is_member(40));
            Console.ReadLine();

            return 0;

        } // main
    } // MainClass

}

     


   