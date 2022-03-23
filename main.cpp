#include <iostream>
#include <cstdlib>
#include <cmath>
#include <numeric>
#include <math.h>
#include <vector>
#include <bits/stdc++.h>
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

int shrey()
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


vector<int> encoding(string s1)
{
	cout << "Encoding\n";
	unordered_map<string, int> table;
	for (int i = 0; i <= 255; i++) {
		string ch = "";
		ch += char(i);
		table[ch] = i;
	}

	string p = "", c = "";
	p += s1[0];
	int code = 256;
	vector<int> output_code;
	cout << "String\tOutput_Code\tAddition\n";
	for (int i = 0; i < s1.length(); i++) {
		if (i != s1.length() - 1)
			c += s1[i + 1];
		if (table.find(p + c) != table.end()) {
			p = p + c;
		}
		else {
			cout << p << "\t" << table[p] << "\t\t"
				<< p + c << "\t" << code << endl;
			output_code.push_back(table[p]);
			table[p + c] = code;
			code++;
			p = c;
		}
		c = "";
	}
	cout << p << "\t" << table[p] << endl;
	output_code.push_back(table[p]);
	return output_code;
}

void decoding(vector<int> op)
{
	cout << "\nDecoding\n";
	unordered_map<int, string> table;
	for (int i = 0; i <= 255; i++) {
		string ch = "";
		ch += char(i);
		table[i] = ch;
	}
	int old = op[0], n;
	string s = table[old];
	string c = "";
	c += s[0];
	cout << s;
	int count = 256;
	for (int i = 0; i < op.size() - 1; i++) {
		n = op[i + 1];
		if (table.find(n) == table.end()) {
			s = table[old];
			s = s + c;
		}
		else {
			s = table[n];
		}
		cout << s;
		c = "";
		c += s[0];
		table[count] = table[old] + c;
		count++;
		old = n;
	}
}
int garv()
{

	string s = "WYS*WYGWYS*WYSWYSG";
	vector<int> output_code = encoding(s);
	cout << "Output Codes are: ";
	for (int i = 0; i < output_code.size(); i++) {
		cout << output_code[i] << " ";
	}
	cout << endl;
	decoding(output_code);
  return 0;
}



int saachi()
{
	int p[20],bt[20], su[20],     
      wt[20],tat[20],i, k, n, temp;
  
	float wtavg, tatavg;
	cout<<"Enter the number of processes: ";
	cin>>n;
	for(i=0;i<n;i++)
	{
		p[i] = i;
		cout<<"Enter the Burst Time of Process: ";
		cin>>bt[i];
		cout<<"System/User Process (0/1) ? ";
		cin>>su[i];
	}
	for(i=0;i<n;i++)
		for(k=i+1;k<n;k++)
			if(su[i] > su[k])
			{
			temp=p[i];
			p[i]=p[k];
			p[k]=temp;
			temp=bt[i];
			bt[i]=bt[k];
			bt[k]=temp;
			temp=su[i];
			su[i]=su[k];
			su[k]=temp;
			}
	wtavg = wt[0] = 0;
	tatavg = tat[0] = bt[0];
	for(i=1;i<n;i++)
	{
		wt[i] = wt[i-1] + bt[i-1];
		tat[i] = tat[i-1] + bt[i];
		wtavg = wtavg + wt[i];
		tatavg = tatavg + tat[i];
	}
  for (int i = 0; i < n; i++) {
  	cout<<"\nPROCESS: "<<p[i]<<endl;
    cout<<"SYSTEM/USER PROCESS: "<<su[i]<<endl;
    cout<<"BURST TIME: "<<bt[i]<<endl;
    cout<<"WAITING TIME: "<<wt[i]<<endl;
    cout<<"TURNAROUND TIME: "<<tat[i]<<endl;
  }
	
  cout<<"\nAverage Waiting Time is --- "<<(wtavg/n)<<endl;
	cout<<"\nAverage Turnaround Time is --- "<<(tatavg/n)<<endl;
	return 0;
}

#define MAX_PROCESS 10
int num_of_process=3, count, remain, time_quantum;
int execution_time[MAX_PROCESS], period[MAX_PROCESS], remain_time[MAX_PROCESS], deadline[MAX_PROCESS], remain_deadline[MAX_PROCESS]; 


void get_process_info(){
	cout << "Enter total number of processes(maximum "<< MAX_PROCESS << "): ";
   	cin >> num_of_process;
   	if (num_of_process < 1){
   		cout << "Do you really want to schedule "<< num_of_process << " processes?" << endl;
   		exit(0);
   	}

  for (int i = 0; i < num_of_process; i++){
		cout << endl << "Process "<< i+1 << ":-" << endl;
    cout << "Execution time: ";
    cin >> execution_time[i];
    remain_time[i] = execution_time[i];
    cout << "Deadline: ";
	  cin >> deadline[i];
    cout << "Period: ";
	  cin >> deadline[i];
  }
}


int max(int a,int b, int c){
  long max,lcom, count, flag=0;
  if( a>=b && a>=c){
    return max=a;
  } else if (b >=a && b>=c) {
    return max=b;
  } else if (c>=a && c>=b) {
    return max=c;
  } 
  return 0;
}

int get_observation_time(){
  return max(deadline[0], deadline[1], deadline[2]);
}

void print_schedule(int process_list[], int cycles){
	cout << endl << "Scheduling:-" << endl << endl;
	
	for (int i=0; i<num_of_process; i++){
    cout << endl;
		cout << "P[" << i+1 << "]: ";
		for (int j=0; j<cycles; j++){
			if (process_list[j] == i+1)
				cout << j << " ";
			else
				cout << "";
		}
	}
}

void earliest_deadline_first(int time){
	float utilization = 0;
	for (int i = 0; i < num_of_process; i++){
		utilization += (1.0*execution_time[i])/deadline[i];
	}

  cout << endl << "CPU Utilization: " << utilization << endl;
	int n = num_of_process;
	if (utilization > 1){
		cout << endl << "Given problem is not schedulable under said scheduling algorithm." << endl;
		exit(0);
	}

	int process[num_of_process];
	int max_deadline, current_process=0, min_deadline,process_list[time];;
	bool is_ready[num_of_process];

	for(int i=0; i<num_of_process; i++){
		is_ready[i] = true;
		process[i] = i+1; 
	}

	max_deadline=deadline[0];
	for(int i=1; i<num_of_process; i++){
		if(deadline[i] > max_deadline)
			max_deadline = deadline[i];
	}

	for(int i=0; i<num_of_process; i++){
		for(int j=i+1; j<num_of_process; j++){	
			if(deadline[j] < deadline[i]){
				int temp = execution_time[j];
				execution_time[j] = execution_time[i];
				execution_time[i] = temp;
				temp = deadline[j];
				deadline[j] = deadline[i];
				deadline[i] = temp;
				temp = process[j];
				process[j] = process[i];
				process[i] = temp;
			}
		}
	}

	for(int i=0; i<num_of_process; i++){
		remain_time[i] = execution_time[i];
		remain_deadline[i] = deadline[i];
	}

	for (int t = 0; t < time; t++){
		if(current_process != -1){		
			--execution_time[current_process];
			process_list[t] = process[current_process];
		}	
		else
			process_list[t] = 0;
	
		for(int i=0;i<num_of_process;i++){
			--deadline[i];
			if((execution_time[i] == 0) && is_ready[i]){
				deadline[i] += remain_deadline[i];
				is_ready[i] = false;
			}
			if((deadline[i] <= remain_deadline[i]) && (is_ready[i] == false)){
				execution_time[i] = remain_time[i];
				is_ready[i] = true;
			}
		}
	
		min_deadline = max_deadline;		
		current_process = -1;
		for(int i=0;i<num_of_process;i++){
			if((deadline[i] <= min_deadline) && (execution_time[i] > 0)){				
				current_process = i;
				min_deadline = deadline[i];
			}
		}
	}	
	print_schedule(process_list, time);
}

int trisha() {
  
  get_process_info();
  int observation_time = get_observation_time();
  earliest_deadline_first(observation_time);
  return 0;
}

int main()
{    int ch=0;
  cout<<"1.Shrey 2.Garv 3.Saachi 4.Trisha"<<endl;
  cin>>ch;
  switch(ch){
    case 1: shrey();
    break;
    case 2: garv();
    break;
    case 3: saachi();
    break;
    case 4: trisha();
    break;
    default: cout<<"Not found"<<endl;
  }
  return 0;
}