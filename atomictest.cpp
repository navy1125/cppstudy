// atomic::compare_exchange_weak example:
#include <iostream>  // std::cout
#include <atomic>    // std::atomic
#include <thread>    // std::thread
#include <vector>    // std::vector

// a simple global linked list:
struct Node
{
	int value;
	Node* next;
};
std::atomic< Node* > list_head(nullptr);

void append(int val)
{  // append an element to the list
	while(true)
	{
		Node* oldHead = list_head;
		Node* newNode = new Node{val, oldHead};

		// what follows is equivalent to: list_head = newNode, but in a thread-safe way:
		while (!list_head.compare_exchange_weak(oldHead, newNode))
		{
			newNode->next = oldHead;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			std::this_thread::yield();
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}
void pop(int val)
{  // append an element to the list
	while(true)
	{
		Node* result = list_head.load();
		while (result != nullptr)
		{
			/*
			while (!list_head.compare_exchange_weak(result, result->next))
			{
				std::cout << "pop wait" << result->value << std::endl;
			}
			// */
			//std::cout << "aaaa" << result->value << std::endl;
			delete(result);
			result = list_head.load();
		}
			std::this_thread::yield();
	}
}

int main()
{
	// spawn 10 threads to fill the linked list:
	std::vector< std::thread > threads;
	for (int i = 0; i < 100; ++i)
		threads.push_back(std::thread(append, i));
	auto thread = std::thread(pop, 0);
	threads.push_back(thread);
	for (auto& th : threads)
		th.join();

	// print contents:
	for (Node* it = list_head; it != nullptr; it = it->next)
		std::cout << ' ' << it->value;
	std::cout << '\n';

	// cleanup:
	/*
	   Node* it;
	   while (it = list_head)
	   {
	   list_head = it->next;
	   delete it;
	   }
	// */

	return 0;
}
