
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//Process object represent cpu process per unit
struct process
{
    int id;
    int priority;
    int burst_time;
    int arrival_time;
    int waiting_time;
    int finishing_time;
    int turn_around_time;
    int completion_time;
    int response_time;
    int start_time;
};

//Function to display main page menu
void main_page() 
{
    cout << "+----- CPU scheduling algorithms -----+" << endl;
    cout << "|                                     |" << endl;
    cout << "| Select Algorithm:                   |" << endl;
    cout << "| 1. Round Robin with Quantum 3       |" << endl;
    cout << "| 2. Preemptive                       |" << endl;
    cout << "| 3. Non Preemptive                   |" << endl;
    cout << "| 4. Show All                         |" << endl;
    cout << "| 5. Exit                             |" << endl;
    cout << "|                                     |" << endl;
    cout << "+-------------------------------------+" << endl;
    cout << "Select: ";
}

//Funtion to get user option between 1-5 from main page menu
void get_option(int& inp) {
    main_page(); // display main page
    cin >> inp; // get user input (pass by reference)

    while (inp<=0 && inp>=5)
    {
        // // Linux / Mac OS
        // system("clear"); 

        // Windows OS
        system("cls");

        // Display the main page again (for the wrong input)
        main_page();
        cout << "Sorry! Wrong Input!" << endl;
        cout << "Please select between 1 to 3" << endl;
        cin >> inp; // re-input
    }
}

//Funtion to get number of process limit to 3-10 [pass input by reference]
void get_num_process(int& inp) {
    cout << "Enter Number of processes [ 3 - 10 only ] => ";
    cin >> inp; // get user input 

    while (inp<=3 && inp>=10)
    {
        cout << "Sorry! Wrong Input!" << endl;
        cout << "Please select between 3 to 10" << endl;
        cin >> inp;
    }
}

// Function to get all process object 
// processes vector and number of processes [1D vector: pass by reference]
void get_processes(vector<process>& processes, const int& num_process)
{
    for (int i=0; i<num_process; i++)
    {
        // Get process object input
        int arrival_time, burst_time, priority;
        cout << "\nPlease input space seperated data (based on number of processes)" << endl;
        cout << "--> Arrival time --> burst time --> priority (if available, else 0)\n" << endl;
        cout << "Example: if the process = 4\nPlease input:0 10 0" << endl;
        cout << "----------------------------------\n";
        cout << "Input: ";
        cin >> arrival_time >> burst_time >> priority;

        // Create process object
        process temp_process;
        temp_process.id = i;
        temp_process.arrival_time   = arrival_time;
        temp_process.burst_time     = burst_time;
        temp_process.priority       = priority;

        // Push process object to vector
        processes.push_back(temp_process);
    }
}

//display process ganttchart
void display_ganttchart(vector<process> processes) {

    // Upper line
    cout << "+";
    for (int i=0; i<processes.size(); i++)
        cout << "------+";
    cout << endl;

    // Content
    cout << "|";
    for (int i=0; i<processes.size(); i++)
        cout << "  P" << processes.at(i).id << "  |";
    cout << endl;

    // Bottom line
    cout << "+";
    for (int i=0; i<processes.size(); i++)
        cout << "------+";
    cout << endl;

    // Timestamp
    cout << "0";
    for (int i=0; i<processes.size(); i++)
        printf("%7d", processes.at(i).completion_time);
    cout << endl << endl;
}

//Function to display process table
void display_table(vector<process> processes) {
    printf(
        "%-10s%-13s%-15s%-18s%-19s%-15s", 
        "Process", "Burst Time", "Arrival Time", 
        "Completion Time", "Turn Around Time","Waiting Time"
    );
    cout << endl;

    for (int i=0; i<processes.size(); i++) {
       printf(
            "%-14d%-14d%-16d%-18d%-18d%-15d",
            processes.at(i).id,
            processes.at(i).burst_time,
            processes.at(i).arrival_time,
            processes.at(i).completion_time,
            processes.at(i).turn_around_time,
            processes.at(i).waiting_time
       );
        cout << endl;
    }
    cout << endl;    
}

//Function to display process of calculation
void display_calculation(const vector<process>& processes) {
    // ttat = total time of arrival time
    // atat = average time of arrival time
    // twt = total time of waiting time
    // awt = average time of waiting time
    double ttat, atat, twt, awt;

    // get the total of waiting and arrival time
    for (auto &&process_data : processes) {
        ttat += process_data.turn_around_time; // total arrival time
        twt += process_data.waiting_time; // total waiting time
    }

    // calculate the average of waiting and arrival time
    atat = ttat/processes.size();
    awt = twt/processes.size();

    cout << "Total Turn Around Time = " << ttat << endl;
    cout << "Average Turn Around Time = " << atat << endl;
    cout << "Total Waiting Time = " << twt << endl;
    cout << "Average Waiting Time = " << awt << endl;
}

//comparison to sort by arrival time
bool compare_arrival(const process& lhs, const process& rhs) {
    if (lhs.arrival_time>rhs.arrival_time) return 0; // first process data
    if (lhs.arrival_time<rhs.arrival_time) return 1; // second process data
    return 0;
}

//comparison to sort by arrival time then priority
bool compare_arrival_priority(const process& lhs, const process& rhs) {
    if (lhs.arrival_time>rhs.arrival_time) return 0;
    if (lhs.arrival_time<rhs.arrival_time) return 1;
    if (lhs.priority>rhs.priority) return 0;
    if (lhs.priority<rhs.priority) return 1;
    return 0;
}

//comparison to sort by burst time
bool compare_burst(const process& lhs, const process& rhs) {
    if (lhs.burst_time>rhs.burst_time) return 0;
    if (lhs.burst_time<rhs.burst_time) return 1;
    return 0;
}

//Round Robin Algorithm
void run_RR(vector<process>& processes, vector<process>& ganttchart, const int quantum=3) {
    vector<process> ready_queue;
    vector<process> temp = processes;

    int timer=0;
    process check_process, null_process;

    // Indicate null
    null_process.id = -1;
    check_process = null_process;

    // sort the data
    sort(temp.begin(), temp.end(), compare_arrival_priority);

    while(!(temp.empty() && ready_queue.empty())) {

        // Pop the latest data arrives
        while (!temp.empty() && temp.at(0).arrival_time <= timer) {
            ready_queue.push_back(temp.at(0));
            temp.erase(temp.begin());
        }
        // if the process is not done yet
        if (check_process.id != -1 && check_process.burst_time>0)
            ready_queue.push_back(check_process);

        // If there is a process in queue
        if (!ready_queue.empty()) {
            // pop the first queue
            check_process = ready_queue.at(0);
            ready_queue.erase(ready_queue.begin());

            // if time more than quantum, take quantum
            if (check_process.burst_time > quantum) {
                timer += quantum;
                check_process.burst_time = check_process.burst_time-quantum;

                // update gantt chart to be display
                process temp_process = check_process;
                temp_process.completion_time = timer;
                ganttchart.push_back(temp_process);
            }
            // if time less than quantum, take quantum whole process
            else {
                timer += check_process.burst_time;
                // UPDATE DATA
                for (auto &&process_data : processes) {
                    if (process_data.id == check_process.id) {
                        process_data.completion_time = timer;
                        process_data.turn_around_time = timer-process_data.arrival_time;
                        process_data.waiting_time = process_data.turn_around_time - process_data.burst_time;
                    }
                }

                // update gantt chart to be display
                process temp_process = check_process;
                temp_process.completion_time = timer;
                ganttchart.push_back(temp_process);
                check_process.burst_time = 0;
            }
        }
        // no process in queue
        else {
            timer += 1;
            // update gantt chart to be display
            process temp_process = check_process;
            temp_process.completion_time = timer;
            // push id -1
            ganttchart.push_back(temp_process);
            check_process = null_process;
        }
    }
}

//Preemptive SJF Algorithm
void run_PSJF(vector<process>& processes, vector<process>& ganttchart) {
    vector<process> ready_queue;
    vector<process> temp = processes;

    int timer=0;
    process check_process, null_process;

    // Indicate null
    null_process.id = -1;
    check_process = null_process;

    while (!(temp.empty() && ready_queue.empty())) {
        // Pop the latest data arrives
        while (!temp.empty() && temp.at(0).arrival_time <= timer) {
            ready_queue.push_back(temp.at(0));
            temp.erase(temp.begin());
        }

        if (!ready_queue.empty()) {
            // To get non preemptive priority
            sort(ready_queue.begin(), ready_queue.end(), compare_burst);

            check_process = ready_queue.at(0);
            ready_queue.erase(ready_queue.begin());

            // Compare current burst with next arrival
            if (!temp.empty() && check_process.burst_time > temp.at(0).arrival_time) {
                timer += temp.at(0).arrival_time;
                check_process.burst_time -= temp.at(0).arrival_time;

                // update gantt chart to be display
                process temp_process = check_process;
                temp_process.completion_time = timer;
                ganttchart.push_back(temp_process);
                ready_queue.push_back(check_process);
            }
            // take all current burst time
            else {
                timer += check_process.burst_time;
                check_process.burst_time = 0;

                // Update data
                for (auto &&process_data : processes) {
                    if (process_data.id == check_process.id) {
                        process_data.completion_time = timer;
                        process_data.turn_around_time = timer-process_data.arrival_time;
                        process_data.waiting_time = process_data.turn_around_time - process_data.burst_time;
                    }
                }

                // // update gantt chart to be display
                process temp_process = check_process;
                temp_process.completion_time = timer;
                ganttchart.push_back(temp_process);
            }

        }
        else {
            timer += 1;

            // update gantt chart to be display
            process temp_process = check_process;
            temp_process.completion_time = timer;
            //push id -1
            ganttchart.push_back(temp_process);
            check_process = null_process;
        }
    }
}

//Non Preemptive SJF Algorithm
void run_NPSJF(vector<process>& processes, vector<process>& ganttchart) {
    vector<process> ready_queue;
    vector<process> temp = processes;

    int timer=0;
    process check_process, null_process;

    // indicate null
    null_process.id = -1;
    check_process = null_process;

    while (!(temp.empty() && ready_queue.empty())) {
        // Pop the latest data arrives
        while (!temp.empty() && temp.at(0).arrival_time <= timer) {
            ready_queue.push_back(temp.at(0));
            temp.erase(temp.begin());
        }

        if (!ready_queue.empty()) {
            // to get non preemptive priority
            sort(ready_queue.begin(), ready_queue.end(), compare_burst);

            check_process = ready_queue.at(0);
            ready_queue.erase(ready_queue.begin());
            timer += check_process.burst_time;

            // Update data
            for (auto &&process_data : processes) {
                if (process_data.id == check_process.id) {
                    process_data.completion_time = timer;
                    process_data.turn_around_time = timer-process_data.arrival_time;
                    process_data.waiting_time = process_data.turn_around_time - process_data.burst_time;
                }
            }

            // update gantt chart to be display
            process temp_process = check_process;
            temp_process.completion_time = timer;
            ganttchart.push_back(temp_process);
        }
        else {
            timer += 1;

            // update gantt chart to be display
            process temp_process = check_process;
            temp_process.completion_time = timer;
            // push id -1
            ganttchart.push_back(temp_process);
            check_process = null_process;
        }
    }
}

int main() 
{
    vector<process> processes, ganttchart;

    // display main page
    int opt;
    get_option(opt);

    // Get input
    int num_process; //num of process to get
    get_num_process(num_process);
    get_processes(processes, num_process);
    
    // sort by arrival time
    sort(processes.begin(), processes.end(), compare_arrival);

    // start
    switch (opt)
    {
    case 1:
        // Round robin with quantum 3
        run_RR(processes, ganttchart);
        display_ganttchart(ganttchart);
        display_table(processes);
        display_calculation(processes);
        break;
    case 2:
        // Preemptive SJF
        run_PSJF(processes, ganttchart);
        display_ganttchart(ganttchart);
        display_table(processes);
        display_calculation(processes);
        break;
    case 3:
        // Non-preemptive SJF
        run_NPSJF(processes, ganttchart);
        display_ganttchart(ganttchart);
        display_table(processes);
        display_calculation(processes);
        break;
    case 4:
        // display all scheduling
        run_RR(processes, ganttchart);
        display_ganttchart(ganttchart);
        display_table(processes);
        display_calculation(processes);
        run_PSJF(processes, ganttchart);
        display_ganttchart(ganttchart);
        display_table(processes);
        display_calculation(processes);
        run_NPSJF(processes, ganttchart);
        display_ganttchart(ganttchart);
        display_table(processes);
        display_calculation(processes);
        break;
    case 5:
        // Exit
        cout << "Thank you" << endl;
        break;
    default:
        cout << "Something's wrong" << endl;
        break;
    }

    return 0;
}