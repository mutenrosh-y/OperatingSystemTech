#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;

typedef size_t var;

var MEMORY = 0;
struct node {
	var space, value; bool allocated;
	var startAddress, endAddress;
	node(var space): space(space), value(0), allocated(false) {}
	node(var space, var value): space(space), value(value), allocated(true) {}
};
vector<node*> m;

var give_fitter(var value) {
	if(value == 1) { return 2; }
	return pow(2, ceil( log(value) / log(2) ));
}

vector<node*> give_slots(var space, var fitter) {
	var number_of_slots = 2;
	if(space != fitter) { 
		number_of_slots = 1 + ( log(space / fitter) / log(2) );
	}
	vector<node*> v(number_of_slots, NULL);
	v[0] = new node(fitter);
	for(var i = 1, s = fitter; i < number_of_slots; ++i, s *= 2) { v[i] = new node(s); }
	return v;
}

bool allocate(var value) {
	if(m.size() == 0) { return false; }
	var fitter = give_fitter(value);
	for(var i = 0; i < m.size(); ++i) { if(m[i]->space == fitter && !m[i]->allocated) {
		m[i]->value = value; 
		m[i]->allocated = true;
		return true;
	} }
	for(auto itr = m.begin(); itr != m.end(); ++itr) {
		node *temp = *itr;
		if(!temp->allocated && temp->space > fitter) {
			auto slots = give_slots(temp->space, fitter);
			slots[0]->value = value;
			slots[0]->allocated = true;
			slots[0]->startAddress = temp->startAddress;
			slots[0]->endAddress = temp->startAddress + slots[0]->space;
			for(var i = 1; i < slots.size(); ++i) {
				slots[i]->startAddress = slots[i-1]->endAddress;
				slots[i]->endAddress = slots[i]->startAddress + slots[i]->space;
			}
			auto prev_itr = itr; --prev_itr;
			m.erase(itr);
			for(var i = 0; i < slots.size(); ++i) {
				prev_itr = m.insert(++prev_itr, slots[i]); 
			}
			slots.clear();
			return true;
		}
	}
	return false;
}

bool buddies(node *a, node *b) {
	var add = MEMORY*2;
	if(floor( (add + a->startAddress)/(2*a->space) ) == floor( (add + b->startAddress)/(2*b->space) )) {
		return true;
	}
	return false;
}

void check_merge() {
	auto itr = m.begin(), prev_itr = itr; ++itr;
	while(itr != m.end()) {
		auto prevNode = *prev_itr, currNode = *itr;
		if(prevNode->allocated || currNode->allocated || (prevNode->space != currNode->space) || !buddies(prevNode, currNode)) { 
			prev_itr = itr; ++itr; continue;
		}
		prevNode->space *= 2;
		prevNode->endAddress = prevNode->startAddress + prevNode->space;
		m.erase(itr);
		prev_itr = m.begin();
		itr = prev_itr; ++itr;
	}
}

bool deallocate(var address) {
	if(address >= MEMORY) { return false; }
	for(auto i: m) {
		if(address == i->startAddress && !i->allocated) { return false; }
		if(address == i->startAddress) {
			i->allocated = false;
			i->value = 0;
			check_merge();
			return true;
		}
	}
	return false;
}

void display() {
	for(auto i: m) {
		var if_ = 0; if(i->allocated) { if_ = i->space - i->value; }
		printf("> Start: %4zu  | SIZE: %4zu |  Value: %4zu  IF: %4zu  End: %4zu\n", 
			i->startAddress, i->space, i->value, if_, i->endAddress);
	}
}

int main(int argc, char const *argv[])
{
	cout <<"\n Amount of memory 2^x, if x = 10 then memory is 2^10 = 1024\n\n Enter the value of x = ";
	cin >> MEMORY;
	if(MEMORY >= 32) { cout <<"\n Memory value is huge (x >= 32), program terminated\n"<< endl; return 0; }
	
	MEMORY = (var)pow(2, MEMORY);
	m.emplace_back(new node(MEMORY));
	m[0]->startAddress = 0;
	m[0]->endAddress = m[0]->startAddress + m[0]->space;

	while(true) {
		int c; cout <<"\n Press: [1] Allocate  [2] Deallocate  [3] Exit -> "; cin >> c;	
		var value = 0;
		switch (c) {	
			case 1: cout <<"\n Enter the amount of memory to allocate (= Block Value): "; cin >> value;
					if(value == 0) { cout <<"\n Error: Zero(0) cannot be allocated"<< endl; break; }
					if(!allocate(value)) {
						cout <<"\n Error: Memory insufficient or space is not availabel"<< endl;
						break;
					}
					cout <<"\n THE BLOCKS IN MEMORY\n" << endl; display(); break;
			case 2: cout <<"\n Enter the start address of block to deallocate (Start): "; cin >> value;
					if(!deallocate(value)) {
						cout <<"\n Error: Block is not allocated at given address"<< endl;
						break;
					}
					cout <<"\n THE BLOCKS IN MEMORY\n" << endl; display(); break;
			case 3: cout <<"\n Program terminated\n"<< endl;
					return 0;
			default: cout <<"\n Error: Invalid input"<< endl; 
		}
	}
	return 0;
}