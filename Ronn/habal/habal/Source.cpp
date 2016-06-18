#include <iostream>
#include<functional>

using namespace std;


template<typename T>
class Matrix {
	
private:
	T**mat;
	
public:
	int n;
	function<T(T, T)> addF;
	function<T(T, T)> subF;
	function<T(T, T)> mulF;
	Matrix(int n, function <T(T, T)> addFunc, function <T(T, T)> sub, function <T(T, T)> mul) {
		addF = addFunc;
		subF = sub;
		mulF = mul;
		mat = new T*[n];
		for (int i = 0; i < n; i++)
			mat[i] = new T[n];


	}
	int N()
	{
		return n;
	}
	T get(const int&x, const int& y)
	{
		return mat[x][y];
	}
	T set(const T& t, const int& x, const int& y)
	{
		data[x][y] = t;
	}
	T& operator[](int index)//override operator []
	{
		for (int i = 0; i < n;i++)// for every col return a row
		return mat[i];
	}

	void operator=(T temp)//override operator =
	{
		for (int i = 0; i < n; i++)
			for (j = 0; j < n;j++)
				mat[i][j] = temp;
	}
	friend Matrix operator +(Matrix<T> m1, Matrix <T> m2)
	{
		int i,j;
		Matrix <T>temp=new Matrix<T>(m1.n,m1.addF,m1.subF,m1.mulF);
		for (i = 0; i < m1.n;i++)
			for (j = 0; j< m2.n; j++)
				temp[i][j] = m1.addF(m1[i][j], m2[i][j]);

		return temp;
	}
	friend Matrix operator -(Matrix<T> m1, Matrix <T> m2)
	{
		int i, j;
		Matrix<T> temp = new Matrix<T>(m1.n, m1.addF, m1.subF, m1.mulF);
		for (i = 0; i < m1.n; i++)
			for (j = 0; j < m2.n; j++)
				temp[i][j] = m1.subF(m1[i][ j], m2[i][ j]);
		
			return temp;
	}
	friend Matrix operator *(Matrix<T> m1, Matrix<T> m2)
	{
		Matrix<T> temp=new Matrix<T>(m1.n,m1.addF,m1.subF,m1.mulF);
		
		for (int i = 0; i < m1.n; i++)
		{
			for (int j = 0; j < m2.n; j++)
			{

				for (int k = 0; k < temp.n; k++) // OR k<b.GetLength(0)
					temp[i][j] = m1.addF(temp[i][j], m1.mulF(m1[i][k], m2[k][j]));
			}
		}

		return temp;
	}
		void  print() // print a matrix
		{
			int i, j;
			for (i = 0; i < n; i++)
			{
				for (j = 0; j < n; j++)
				{
					cout << mat[i][ j] << " ";
				}
				cout << " " << endl;

			}
		}



	/*T add(T t1, T t2) {
		return addF(t1, t2);
	}
	T sub(T t1, T t2)
	{
		return subF(t1, t2);
	}
	T mul(T t1, T t2) {
		return mulF(t1,t2);
	}*/

};

//class mainClass {
	int int_add(int x, int y)
	{
		int ix, iy, iz;

		ix = (int)x;
		iy = (int)y;
		iz = ix + iy;
		return ((int)iz);
	} // int_add


	int int_sub(int x, int y)
	{
		int ix, iy, iz;

		ix = (int)x;
		iy = (int)y;
		iz = ix - iy;
		return ((int)iz);
	} // int_sub




	int int_mul(int x, int y)
	{
		int ix, iy, iz;

		ix = (int)x;
		iy = (int)y;
		iz = ix * iy;
		return ((int)iz);
	} // int_mul

	double double_add(double x, double y)
	{
		double dx, dy, dz;

		dx = (double)x;
		dy = (double)y;
		dz = dx + dy;
		return ((double)dz);
	} // double_add


	double double_sub(double x, double y)
	{
		double dx, dy, dz;

		dx = (double)x;
		dy = (double)y;
		dz = dx - dy;
		return ((double)dz);
	} // double_sub


	double double_mul(double x, double y)
	{
		double dx, dy, dz;

		dx = (double)x;
		dy = (double)y;
		dz = dx * dy;
		return ((double)dz);
	} // double_mul
//};
int main() {

	function <int(int, int)> A = [](int x,int y) { return x + y; };
	int n = 5, i, j;
	Matrix<double> M1d =  Matrix<double>(n, double_add, double_sub, double_mul),
		M2d =  Matrix<double>(n, double_add, double_sub, double_mul),
		M3d =  Matrix<double>(n, double_add, double_sub, double_mul);
	double d;
	int k;

	for (i = 0; i < M1d.n; i++)
		for (j = 0; j < M1d.n; j++)
		{
			M1d[i, j] = 1.1 * i + j;
			M2d[i, j] = 10.1 * i + 1.1 * j;
		} // for

	cout << ("M1d:") << endl;
	M1d.print();
	cout << ("M2d:") << endl;
	M2d.print();

	cout << ("M1d + M2d:") << endl;
	M3d = M1d + M2d;
	M3d.print();
	M3d = M1d * M2d;
	cout<<("M1d * M2d:")<<endl;
	M3d.print();




	Matrix<int> M1i =  Matrix<int>(n, int_add, int_sub, int_mul),
		M2i =  Matrix<int>(n, int_add, int_sub, int_mul),
		M3i =  Matrix<int>(n, int_add, int_sub, int_mul);


	for (i = 0; i < M1i.n; i++)
		for (j = 0; j < M1i.n; j++)
		{
			M1i[i, j] = i + 10 * j;
			M2i[i, j] = 100 * i + 10 * j;
		} // for

	cout << ("\n\nM1i:") << endl;

	M1i.print();
	cout << ("M2i:") << endl;
	M2i.print();

	cout << ("M1i + M2i:") << endl;
	M3i = M1i + M2i;
	M3i.print();
	M3i = M1i * M2i;
	cout<<("M1i * M2i:")<<endl;
	M3i.print();
	return 0;


	//Matrix<int> m(10,A,A,A);

	//int x = m.add(10,10);
	//cout << x << endl;
	//cin >> x;
}
