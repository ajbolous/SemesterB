using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Matrix_Type
{
    public delegate object bin_op(object x,object y);
    public class Matrix<T>
    {
        int n;// the size of the matrix
        private T[,] mat;//matrix n*n
        public bin_op add;
        public bin_op sub;
        public bin_op mul;
        public Matrix(int n, bin_op add, bin_op sub, bin_op mul)
        {
            this.n = n;
            this.mat = new T[n, n];
            this.add = add;
            this.sub = sub;
            this.mul = mul;
            
        }
        public int N { get { return n; } }
        public T this[int i,int j]
        {
            get
            {
                
                return mat[i,j];
            }
            set
            {
                mat[i,j] = (T)value;
            }
        }

        public static Matrix<T> operator +(Matrix<T> x, Matrix<T> y)
        {
            Matrix<T> z = new Matrix<T>(x.n,x.add,x.sub,x.mul);
            
            int i, j;
            for (i = 0; i < x.n; i++)
                for (j = 0; j < y.n; j++)
                {
                    z[i, j]= x.add((dynamic)x[i,j],(dynamic)y[i,j]);
                   
                }
            return z;        

        }
       public static Matrix<T> operator *(Matrix<T> x ,Matrix<T> y)
        {
            Matrix<T> z = new Matrix<T>(x.n, x.add, x.sub, x.mul);

            for (int i = 0; i <z.n; i++)
            {
                for (int j = 0; j < x.n; j++)
                {
                  
                    for (int k = 0; k < y.n; k++) // OR k<b.GetLength(0)
                        z[i, j] = x.add((dynamic)z[i, j] , x.mul((dynamic)x[i, k] , (dynamic)y[k, j]));
                }
            }

            return z;
        }
        
        public void print() // print a matrix
        {
            int i, j;
            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n; j++)
                {
                    Console.Write("{0}  ", mat[i,j]);
                }
                Console.WriteLine();
            }
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
            Console.ReadLine();
            

        } // Main


    }
    }


