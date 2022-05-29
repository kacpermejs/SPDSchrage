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
    Task( const Task& t, int c) : R(t.R), P(t.P), Q(t.Q), index(t.index), C(c) {}

    
};



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
    t += MinHeapR.top().R; //czas rozpoczêcia pierwszego zadania

    while (!MinHeapR.empty() || !MaxHeapQ.empty())
    {
        while (!MinHeapR.empty() && MinHeapR.top().R <= t)
        {
            MaxHeapQ.push(MinHeapR.top());
            MinHeapR.pop();
        }

        if (!MaxHeapQ.empty())
        {
            t += MaxHeapQ.top().P; //czas trwania zadania dostêpnego

            C.push_back(Task(MaxHeapQ.top(), t + MaxHeapQ.top().Q));
            MaxHeapQ.pop();
        }
        else
        {
            t = MinHeapR.top().R;
        }
    }
}

void Schrage2(std::priority_queue<Task, std::vector<Task>, compareR >& MinHeapR,
    std::priority_queue<Task, std::vector<Task>, compareQ >& MaxHeapQ,
    std::vector<Task>& C)
{
    int tOld = 0;
    int t = MinHeapR.top().R; //czas rozpoczêcia pierwszego zadania

    while (!MinHeapR.empty() || !MaxHeapQ.empty())
    {
        tOld = t;
        while (!MinHeapR.empty() && MinHeapR.top().R <= t)
        {
            MaxHeapQ.push(MinHeapR.top());
            MinHeapR.pop();
        }
        
        if (!C.empty())//je¿eli jakieœ zadanie zaczê³o siê wykonywaæ
        {
            //wstaw pozosta³¹ czêœæ bierz¹cego zadania do zadañ dostêpnych
            int tFinished = t - tOld;
            if (tFinished < C.back().P)
            {
                Task temp = Task(C.back(), -1);
                temp.R = 0;
                temp.P -= tFinished;
                MaxHeapQ.push(temp);
                t += tFinished;
            }
        }
        
        if (!MaxHeapQ.empty())
        {

            //wstaw lepsze zadanie do uszeregowania
            C.push_back(Task(MaxHeapQ.top(), t + MaxHeapQ.top().Q));
            MaxHeapQ.pop();
            if (!MaxHeapQ.empty())
            {
                t = std::min(t + C.back().P, MaxHeapQ.top().R);
            }
            else if (!MinHeapR.empty())
            {
                t = std::min(t + C.back().P, MinHeapR.top().R);
            }
        }
        else if (!MinHeapR.empty())
        {
            t = MinHeapR.top().R;
        }
            
        
    }
}

/*
if (!MaxHeapQ.empty())
        {
            if (!C.empty())
            {
                int tOld = t;

                t += std::min(t + C.back().P, MinHeapR.top().R); // idziemy do momentu w którym pojawi siê nowe dostêpne

                if (C.back().P >= t - tOld)
                {
                    Task temp = Task(C.back());
                    C.back().C = -1;
                    temp.R = 0;
                    temp.P -= t - tOld;
                    MaxHeapQ.push(temp);
                }
            }
            else
            {
                t = MinHeapR.top().R;
            }

            C.push_back(Task(MaxHeapQ.top(), t + MaxHeapQ.top().Q));
            MaxHeapQ.pop();


        }
*/

int main()
{
    int N;

	//std::vector<Task> tasks;
    std::vector<Task> C;


	int totalC = 0;

    Heap<int> h1;

	std::ifstream data("C:/Users/kacpe/source/repos/SPDSchrage/schr.data.txt");

	std::string tmp;

    std::string dataSource = "data.001:";
	
	while (tmp != dataSource)
		data >> tmp;

	data >> N;
    
    std::priority_queue<Task, std::vector<Task>, compareR > MinHeapR; //niedostêpne
    std::priority_queue<Task, std::vector<Task>, compareQ > MaxHeapQ; //dostêpne
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


    

    
    


    /*
	int c = computeTotalLength(N, R, P, Q, X);
	totalC += c;
	//std::cout << dataSource[i] << std::endl;
	std::cout << "dlugosc: " << c << std::endl;
	for (int i = 1; i <= N; i++)
	{
		std::cout << X[i] << " ";
	}
	std::cout << std::endl;
	
	data.close();
	std::cout << "Total: " << totalC << std::endl;
    */
	return 0;
}


/*

int daneDoKopca[10] = { 2, 20, 4, 8, 9, 1 , 5, 5, 7, 16 };

    for (int i = 0; i < 10; i++)
    {
        //h1.insert(daneDoKopca[i]);
        MinHeapR.push(Task(daneDoKopca[i], 0 , 0) );
        //MaxHeapQ.push(Task(0, 0 , daneDoKopca[i]));
    }

    for (int i = 1; i <= 10; i++)
    {
        //std::cout << h1[1] << " ";
        //h1.removeMax();
        std::cout << MinHeapR.top().R << " ";
        MinHeapR.pop();
        //std::cout << MaxHeapQ.top() << " ";
        //MaxHeapQ.pop();
    }
    std::cout << std::endl;
    //===================================================================

    for (int i = 1; i <= N; i++)
    {
        //std::cout << h1[1] << " ";
        //h1.removeMax();
        std::cout << MinHeapR.top().R << " ";
        MinHeapR.pop();
        //std::cout << MaxHeapQ.top() << " ";
        //MaxHeapQ.pop();
    }
    std::cout << std::endl;

*/
