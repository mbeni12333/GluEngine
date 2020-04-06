#pragma once

#include <vector>

template<typename T>
class Heap{
public:
	Heap(){};
	~Heap(){};

	void push_back(T l_element){
		// insert at the end
		m_elements.push_back(l_element);
		// heapifyUp
		HeapifyUp();
	}
	T GetMin(){
		// get last element swap with front
		//if(m_elements.empty())return nullptr;
		T element = m_elements.back();
		m_elements[m_elements.size()-1] = m_elements.front();
		m_elements[0] = element;
		// pop_back
		element = m_elements.back();
		m_elements.pop_back();
		// heapifyDown
		HeapifyDown();
		// return element
		return element;
	}

	void HeapifyUp(){
		// initialise last element
		// initilise front
		int current = m_elements.size()-1;

		while(current>0){
			//pick min children, if smaller than current, swap
			int parent = Parent(current);

			if(*(m_elements[current])>=*(m_elements[parent])){
				// nothing to be done, go away	
				return;
			}

			//swap
			auto tmp = m_elements[parent];
			m_elements[parent] = m_elements[current];
			m_elements[current] = tmp;

			// set current to min
			current = parent;
		}


		// move up if element smaller than parent || root
	}
	void HeapifyDown(){
		// initilise front
		int current = 0;
		int end = m_elements.size()-1;

		while(current<end){
			//pick min children, if smaller than current, swap
			int r = RightChild(current);
			int l = LeftChild(current);

			int min = minElement(l, r);

			if(min==-1){
				// leaf node
				return;
			}
			if(*(m_elements[current])<=*(m_elements[min])){
				// nothing to be done, go away	
				return;
			}

			//swap
			auto tmp = m_elements[current];
			m_elements[current] = m_elements[min];
			m_elements[min] = tmp;

			// set current to min
			current = min;
		}
		
	}
	int minElement(int i, int j){
		if(i>m_elements.size()-1||j>m_elements.size()-1){
			if(j<m_elements.size())return j;
			if(i<m_elements.size())return i;
			return -1;
		}

		return (*m_elements[i])<=(*m_elements[j]) ? i : j;
	}
	void clear(){
		m_elements.clear();
	}
	int RightChild(int i){
		return 2*i+2;
	}
	int LeftChild(int i){
		return 2*i+1;
	}
	int Parent(int i){
		return i%2==0 ? (int)(i/2)-1 : (int)(i/2);
	}
	bool empty(){
		return m_elements.empty();
	}
	T front(){
		return m_elements.front();
	}

	typename std::vector<T>::iterator begin(){
		return m_elements.begin();
	}

	typename std::vector<T>::iterator end(){
		return m_elements.end();
	}

private:
	std::vector<T> m_elements;

};

