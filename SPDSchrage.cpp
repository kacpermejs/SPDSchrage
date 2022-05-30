// SPDSchrage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>

#define DEFAULT_HEAP_SIZE 20

#ifndef HEAP_EMPTY_EXCEPTION
#define HEAP_EMPTY_EXCEPTION -10
#endif


template <typename T>
class Heap
{
    std::vector<T> data;
    int capacity;
    int num_of_elem;
public:
    Heap();
    void resize();
    void insert(const T& newElement);
    void removeMax();
    std::size_t size();
    bool empty();
    T& operator[](int index);
};

template <typename T>
Heap<T>::Heap()
{
    data.resize(DEFAULT_HEAP_SIZE+1);
    capacity = DEFAULT_HEAP_SIZE;
    num_of_elem = 0;
}

template <typename T>
void Heap<T>::resize()
{
    int old_size = capacity;
    capacity *= 2;
    std::vector<T> temp_data;
    temp_data.resize(capacity);
    for (int i = 1; i < old_size; i++)
    {
        temp_data[i] = data[i];
    }

    data = temp_data;
}

template <typename T>
void Heap<T>::insert(const T& newElement)
{
    if (capacity - 1 == size())
        resize();


    int id = size() + 1;
    data[id] = newElement;

    while (data[id] > data[id / 2] && (id / 2) != 0)
    {
        //swap child - parent
        T temp = data[id / 2];
        data[id / 2] = data[id];
        data[id] = temp;
        id = id / 2;
    }
    num_of_elem++;

}

template <typename T>
void Heap<T>::removeMax()
{
    if (!empty())
    {
        int id = size();
        T temp = data[id];
        data[id] = data[1];
        data[1] = temp;
        num_of_elem--;

        id = 1;

        while (id * 2 < size()) //has left child
        {

            if (data[id * 2] > data[id])//if left child bigger
            {
                if ((id * 2) + 1 < size())//if has a right child
                {
                    if (data[id * 2] > data[(id * 2) + 1])
                    {
                        //swap parent - left child
                        T temp = data[id];
                        data[id] = data[id * 2];
                        id = id * 2;
                        data[id] = temp;
                    }
                    else
                    {
                        //swap parent - right child
                        T temp = data[id];
                        data[id] = data[(id * 2) + 1];
                        id = (id * 2) + 1;
                        data[id] = temp;
                    }
                }
                else
                {
                    //swap parent - right child
                    T temp = data[id];
                    data[id] = data[(id * 2) + 1];
                    id = (id * 2) + 1;
                    data[id] = temp;
                }
            }
            else if ((id * 2) + 1 < size())//if has a right child
            {
                if (data[(id * 2) + 1] > data[id])
                {
                    //swap parent - right child
                    T temp = data[id];
                    data[id] = data[(id * 2) + 1];
                    id = (id * 2) + 1;
                    data[id] = temp;
                }

            }
            else
                break;
        }
    }
    else
        throw HEAP_EMPTY_EXCEPTION;


}

template <typename T>
std::size_t Heap<T>::size()
{
    return num_of_elem;
}

template <typename T>
bool Heap<T>::empty()
{
    return num_of_elem == 0;
}

template <typename T>
T& Heap<T>::operator[](int index)
{
    static T t;

    t = data[index];

    return t;
}

struct Task
{
    int index;
    int R;
    int P;
    int Q;
    int C;

    bool finished;

    Task(int r, int p, int q) : R(r), P(p), Q(q), C(-1) {}
    Task(int r, int p, int q, int id) : R(r), P(p), Q(q), index(id), C(-1) {}
    Task(const Task& t, int c) : R(t.R), P(t.P), Q(t.Q), index(t.index), C(c) {}


};

int computeTotalLength(const int N, std::vector<int> &R, std::vector<int> &P, std::vector<int> &Q, std::vector<int> &X)
{
	int m = 0, c = 0;
	for (int n = 1; n <= N; n++)
	{
		int x = X[n];
		m = std::max(m, R[x]) + P[x];
		c = std::max(c, m + Q[x]);
	}
	return c;
}


struct compareR
{
    bool operator()(const Task &t1, const Task &t2)
    {
        if (t1.R == t2.R)
            return t1.index > t2.index;
        else
            return t1.R > t2.R;
    }
};

struct compareQ
{
    bool operator()(const Task& t1, const Task& t2)
    {
        if (t1.Q == t2.Q)
            return t1.index < t2.index;
        else
            return t1.Q < t2.Q;
    }
};

void Schrage1(std::priority_queue<Task, std::vector<Task>, compareR > &MinHeapR,
              std::priority_queue<Task, std::vector<Task>, compareQ > &MaxHeapQ,
              std::vector<Task> &C )
{
    int t = 0;
    t += MinHeapR.top().R; //czas rozpocz�cia pierwszego zadania

    while (!MinHeapR.empty() || !MaxHeapQ.empty())
    {
        while (!MinHeapR.empty() && MinHeapR.top().R <= t)
        {
            MaxHeapQ.push(MinHeapR.top());
            MinHeapR.pop();
        }

        if (!MaxHeapQ.empty())
        {
            t += MaxHeapQ.top().P; //dodaj czas trwania zadania dost�pnego

            C.push_back(Task(MaxHeapQ.top(), t + MaxHeapQ.top().Q));//doadaj do uszeregowania zadanie i jego czas zako�czenia
            MaxHeapQ.pop();
        }
        else
        {
            t = MinHeapR.top().R;//je�eli nie ma zada� dost�pnych, id� do momentu w kt�rym pojawi si� zadanie
        }
    }
}

void Schrage2(std::priority_queue<Task, std::vector<Task>, compareR >& MinHeapR,
    std::priority_queue<Task, std::vector<Task>, compareQ >& MaxHeapQ,
    std::vector<Task>& C)
{
    int tOld = 0;
    int t = MinHeapR.top().R; //czas rozpocz�cia pierwszego zadania

    while (!MinHeapR.empty() || !MaxHeapQ.empty())
    {
        //dodawanie nowych zada� ===================================================
        
        //dodaj zadania kt�re sta�y si� juz dost�pne
        while (!MinHeapR.empty() && MinHeapR.top().R <= t)
        {
            MaxHeapQ.push(MinHeapR.top());
            MinHeapR.pop();
        }

        //dodaj pozosta�� cz�� bierz�cego zadanoia je�eli jeszcze zota�a
        if (!C.empty()) //by�o ju� jakie� zadanie
        {
            if (!MaxHeapQ.empty() && MaxHeapQ.top().Q > C.back().Q)
            {
                
                if (t - tOld < C.back().P) // czas od pocz�tku zadania do jego przerwania < od jego czasu trwania
                {
                    Task temp = Task(C.back());
                    C.back().C = -1;
                    temp.R = 0;
                    temp.P -= t - tOld;
                    MaxHeapQ.push(temp);
                } 
            }      
        }
        //==========================================================================



        //ustwianie zada� w kolejno�ci =============================================
        if (!MaxHeapQ.empty())
        {
            tOld = t;
            if (!MinHeapR.empty())
                t = std::min(t + MaxHeapQ.top().P, MinHeapR.top().R); //dodaj czas trwania zadania dost�pnego lub moment w kt�rym mo�e zostac przewane
            else
                t += MaxHeapQ.top().P; //je�eli nie ma ju� zada� niedost�pnych ustaw czas na koniec bierz�cego
            
            

            C.push_back(Task(MaxHeapQ.top(), t + MaxHeapQ.top().Q));//doadaj do uszeregowania zadanie i jego czas zako�czenia
            MaxHeapQ.pop();
        }
        else
        {
            t = MinHeapR.top().R;//je�eli nie ma zada� dost�pnych, id� do momentu w kt�rym pojawi si� zadanie
        }
        //==========================================================================
    }
}

int main()
{
    int N;

	//std::vector<Task> tasks;
    std::vector<Task> C;


	int totalC = 0;

    Heap<int> h1;

	std::ifstream data("C:/Users/kacpe/source/repos/SPDSchrage/schr.data.txt");

	std::string tmp;

    std::string dataSource = "data.007:";
	
	while (tmp != dataSource)
		data >> tmp;

	data >> N;
    
    std::priority_queue<Task, std::vector<Task>, compareR > MinHeapR; //niedost�pne
    std::priority_queue<Task, std::vector<Task>, compareQ > MaxHeapQ; //dost�pne
    //! numerowanie od 1
	for (int i = 1; i < N+1; i++)
	{
        int r, p, q;
		data >> r >> p >> q;
		//data >> R[i] >> P[i] >> Q[i];
        
        MinHeapR.push(Task(r, p, q, i));
        //std::cout << "id: " << tasks[i-1].index << " R: " << tasks[i-1].R << " P: " << tasks[i - 1].P << " Q: " << tasks[i - 1].Q << std::endl;
	}
    
    Schrage2(MinHeapR, MaxHeapQ, C);

    int Cmax = 0;
    for (int i = 0; i < C.size(); i++)
    {
        Cmax = std::max(Cmax, C[i].C);
    }
    std::cout << "Cmax = " << Cmax << std::endl;

    for (int i = 0; i < C.size(); i++)
    {
        std::cout << C[i].index << " ";

    }
    std::cout << std::endl;
	return 0;
}

