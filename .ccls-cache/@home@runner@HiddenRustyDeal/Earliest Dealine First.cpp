#include <iostream>
#include <numeric>
#include <math.h>
using namespace std;
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

int trisha(int argc, char* argv[]) {
  
  get_process_info();
  int observation_time = get_observation_time();
  earliest_deadline_first(observation_time);
}

