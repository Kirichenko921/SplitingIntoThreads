#include <iostream>
#include<ctime>
#include <vector>
#include<thread>
#include<algorithm>
void fillingArray(std::vector<int>& randArr, const int& rangeLen) // функция заполнения массива случайными числами
{
	std::srand(static_cast<unsigned int>(std::time(NULL))); //Устанавливаем начальное значение для rand, и преобразовывем его в unsigned int
	for (size_t i = 0; i < randArr.size(); ++i)//заполняем массив
	{
		randArr[i] = std::rand() % rangeLen;
	}
}

int partSum(int i,const unsigned int  size,const unsigned int  M, std::vector <int>& randArr) // фунция подсчёта суммы для каждого потока - части
{
	int partSum = 0;
	for (size_t q = (size - ((M - i) * (size / M))); q < (size - ((M - i - 1) * (size / M))); ++q)
	{
		partSum += randArr[q];
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // задерживаем поток (для небольших вычислений)
	return partSum;
}
int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	const unsigned int size = 10000000; //размер массива 
	const unsigned int M = 1;// количество потоков (кратное размеру массива)
	std::vector<int> randArr(size); // массив случайных чисел
	fillingArray(randArr, 100);  // заполнение массива случайными числами
	//for (const auto i : randArr)//вывод массива на экран (информативно - можно убрать)
		//std::cout << i << ' ';
	std::cout << std::endl;
	std::vector<int> divSum(M, 0); // вектор для хранения сумм каждого потока
		int sum = 0; //итоговая сумма 
	
	std::vector<std::thread> sumThreads; //вектор потоков

	for (int i = 0; i < M; ++i) // запускаем контрольное количество потоков
	{
		sumThreads.push_back(std::thread([i, size, M, &sum, &randArr, &divSum]() { // кажый поток считает сумму своеё части массива
			divSum[i] = partSum(i, size, M, randArr);
			}));
	}
		
	std::for_each(sumThreads.begin(), sumThreads.end(), [](std::thread& t) {t.join(); }); // ждём завершения потоков

	for (int i = 0; i < M; ++i)  // выводим сумму каждой части 
	{
		std::cout << "Sum "<<i+1<<"= "<< divSum[i] << std::endl;
		sum += divSum[i];
	}
	std::cout <<"\nSum = "<< sum << std::endl; // выводим общую суумму

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double>duration = end - start;
	std::cout << "Duration " << duration.count() << "s" << std::endl;
	return 0;
}
