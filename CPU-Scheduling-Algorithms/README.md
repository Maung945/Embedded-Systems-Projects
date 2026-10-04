# Scheduling Algorithms Simulation

This project implements various CPU scheduling algorithms in C and provides test scripts for executing and logging the results of these simulations. The scheduling algorithms include First Come First Serve (FCFS), Shortest Job First (SJF), Round Robin (RR), and Multi-Level Feedback Queue (MLFQ).

## Directory Structure

- `FCFS.c`: Implements the First Come First Serve scheduling algorithm.
- `SJF.c`: Implements the Shortest Job First scheduling algorithm.
- `RR.c`: Implements the Round Robin scheduling algorithm.
- `MLFQ.c`: Implements the Multi-Level Feedback Queue scheduling algorithm.
- `TestAll.sh`: Script to test all scheduling algorithms and log outputs.
- `TestFirstComeFirstServe.sh`: Script to test and log the FCFS algorithm.
- `TestMultiLevelFeedbackQueue.sh`: Script to test and log the MLFQ algorithm.
- `TestRoundRobin.sh`: Script to test and log the RR algorithm.
- `TestShortestJobFirst.sh`: Script to test and log the SJF algorithm.


## Note
- The scheduling simulations are implemented in C and use system calls like `fork()` and `waitpid()` to manage processes.
- Ensure that the executables are compiled from the C source files before running the test scripts.
- Modify the workload sizes and quantum values in the C files to simulate different scheduling scenarios.

## Contributing
- Contributions to improve the scheduling simulations or test scripts are welcome. Please ensure to maintain the existing structure and standards for any pull requests.

## Running the Tests

To run the tests for all scheduling algorithms, use the following command:

`./TestAll.sh`
- This will execute each algorithm and save the outputs to their respective logs in the Logs directory.

## Individual Test Scripts
- Each scheduling algorithm can be tested individually by running its corresponding script. For example, to test the FCFS algorithm, you can use:

`./TestFirstComeFirstServe.sh`
- This command will execute the FCFS scheduling algorithm multiple times and save the output to FCFS.log.

## Logs
- Output logs for each test are stored in the Logs directory under their respective files:

- `FCFS.log` for First Come First Serve results.
- `MLFQ.log` for Multi-Level Feedback Queue results.
- `RR.log` for Round Robin results.
- `SJF.log` for Shortest Job First results.
- The logs include detailed metrics such as response time and context switches, which are essential for evaluating the performance of each scheduling algorithm.
