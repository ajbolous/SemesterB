using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Matrix_Type
{
    public class Matrix<Type>
    {
        int n;// the size of the matrix
        private object[][] mat;//matrix n*n
        
        public Matrix(int n, bin_op add, bin_op sub, bin_op mul)
        {
            this.n = n;
            

        }
        public int N { get { return n; } }
        public object indexer(int i,int j)//indexer 
        {
            return this.mat[i][j];
        }

        public static Matrix<Type> operator +(Matrix<Type> x, Matrix<Type> y)
        {
            bin_op add;
            int[,] m0 = x.indexer(i, i);
            int[,] m1 = y._values;
            int[,] newMatrix = add(m0, m1)/* add m0 and m1 */;
            return new Matrix(newMatrix);
        }
    }
    class bin_op
    {
        public static object[,] add(this object[,] matrix1, object[,] matrix2)
        {
            object[,] retorno = new object[n, n];
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    retorno[i, j] = matrix1[i, j] + matrix2[i, j];
                }
            }
            return retorno;
        }
    }
    class Program
    {
            public static object int_add(object x, object y)
            {
                int ix, iy, iz;

                ix = (int)x;
                iy = (int)y;
                iz = ix + iy;
                return ((object)iz);
            } // int_add


            public static object int_sub(object x, object y)
            {
                int ix, iy, iz;

                ix = (int)x;
                iy = (int)y;
                iz = ix - iy;
                return ((object)iz);
            } // int_sub




            public static object int_mul(object x, object y)
            {
                int ix, iy, iz;

                ix = (int)x;
                iy = (int)y;
                iz = ix * iy;
                return ((object)iz);
            } // int_mul

            public static object double_add(object x, object y)
            {
                double dx, dy, dz;

                dx = (double)x;
                dy = (double)y;
                dz = dx + dy;
                return ((double)dz);
            } // double_add


            public static object double_sub(object x, object y)
            {
                double dx, dy, dz;

                dx = (double)x;
                dy = (double)y;
                dz = dx - dy;
                return ((double)dz);
            } // double_sub


            public static object double_mul(object x, object y)
            {
                double dx, dy, dz;

                dx = (double)x;
                dy = (double)y;
                dz = dx * dy;
                return ((double)dz);
            } // double_mul


        }
        static void Main(string[] args)
        {
            int n = 5, i, j;
            Matrix<double> M1d = new Matrix<double>(n, double_add, double_sub, double_mul),
               M2d = new Matrix<double>(n, double_add, double_sub, double_mul),
                M3d = new Matrix<double>(n, double_add, double_sub, double_mul);
            double d;
            int k;

            for (i = 0; i < M1d.N; i++)
                for (j = 0; j < M1d.N; j++)
                {
                    M1d[i, j] = 1.1 * i + j;
                    M2d[i, j] = 10.1 * i + 1.1 * j;
                } // for

            Console.WriteLine("M1d:");
            M1d.print();
            Console.WriteLine("M2d:");
            M2d.print();

            Console.WriteLine("M1d + M2d:");
            M3d = M1d + M2d;
            M3d.print();
            M3d = M1d * M2d;
            Console.WriteLine("M1d * M2d:");
            M3d.print();

            d = (double)M1d;
            Console.WriteLine("\n(double)M1d = " + d);


            Matrix<int> M1i = new Matrix<int>(n, int_add, int_sub, int_mul),
               M2i = new Matrix<int>(n, int_add, int_sub, int_mul),
                M3i = new Matrix<int>(n, int_add, int_sub, int_mul);


            for (i = 0; i < M1i.N; i++)
                for (j = 0; j < M1i.N; j++)
                {
                    M1i[i, j] = i + 10 * j;
                    M2i[i, j] = 100 * i + 10 * j;
                } // for

            Console.WriteLine("\n\nM1i:");
            M1i.print();
            Console.WriteLine("M2i:");
            M2i.print();

            Console.WriteLine("M1i + M2i:");
            M3i = M1i + M2i;
            M3i.print();
            M3i = M1i * M2i;
            Console.WriteLine("M1i * M2i:");
            M3i.print();

            k = (int)M1i;
            Console.WriteLine("\n(int)M1i = " + k);

        } // Main
    


    }
}
