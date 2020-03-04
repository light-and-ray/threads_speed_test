//print как в питоне
#include <iostream>
#include <functional>
#include <thread>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>

typedef unsigned long long ull;

/////////////////////////////////////////////////
inline double expression(const double& i)
{
	return (i == 0) ? 0 : 1. / i;
}
////////////////////////////////////////////////

template <typename Arg>
void print(Arg arg)
{
	std::cout << arg << '\n';
}

template <typename Arg, typename... Args>
void print(Arg arg, Args... args)
{
	std::cout << arg << ' ';
	print(args...);
}

double Loop(double& x, ull b, ull a)
{
	for (ull i = b; i < a; i++)
	{
		x += expression(i);
	}
	return x;
}

double test(const int& THREADS, const ull& a)
{
	double x = 0;
	ull b = 0; //start

	if (THREADS != 1)
	{
		ull DIST = a / THREADS;

		std::vector <double> X(THREADS);
		std::vector <std::thread> loop(THREADS);
		for (int i = 0; i < THREADS; i++)
		{
			X[i] = 0;
			loop[i] = std::thread(Loop, std::ref(X[i]), b + DIST * i, b + DIST * (i + 1));
		}

		for (int i = 0; i < THREADS; i++)
		{
			loop[i].join();
			x += X[i];
		}
	}
	else
	{
		Loop(x, b, a);
	}
	return x;
}

int main()
{
	ull COUNT = 1000000000;
	int NUMBER = 3;
	int AVGNUM = 2;
	int START = 5;
	std::vector<float> results(NUMBER);

	std::cout << "Teasting " << COUNT << " terms:\n\n";
	std::ofstream fout;
	std::stringstream ss;
	ss << "log_" << time(NULL) << ".txt";
	fout.open(ss.str());
	fout << "COUNT of terms is " << COUNT << "\n"
		<< "Averange value AVGNUM = " << AVGNUM << "\n\n";

	for (int i = 0; i < NUMBER; i++)
	{
		print("Treads:", i + START);
		std::clock_t c_start = std::clock();

		for (int i_ = 0; i_ < AVGNUM; i_++)
			test(i + START, COUNT);

		std::clock_t c_end = std::clock();
		results[i] = (c_end - c_start) / AVGNUM;
		print("avg duration is", 0.001 * results[i], "s\n");
		fout << "Threads " << i + START << "\navg duration is " << 0.001 * results[i] << "\n\n";
		c_start = std::clock();
		while (std::clock() - c_start < 500)
		{

		}
	}

	auto min = results[0];
	int minI = 0;
	for (int i = 1; i < NUMBER; i++)
	{
		if (results[i] < min)
		{
			min = results[i];
			minI = i;
		}
	}
	print("The best avg result is", min * 0.001 , "s on", minI + START, "threads");
	fout << "The best avg result is " << min * 0.001 << " s on " << minI + START << " threads\n";
	fout.close();
}
