# DELHI-METRO-ROUTE

## Overview
**DELHI-METRO-ROUTE** is a C++ project that simulates the Delhi Metro system. It provides tools to calculate the shortest distance between any two stations, along with the path taken by the metro, including all intermediate stations. The program uses graph theory to model the metro network, making it a useful tool for navigating the Delhi Metro efficiently.

## Key Features
- **Graph Representation**: Models the Delhi Metro network as a graph, with stations as nodes and metro lines as edges.
- **Shortest Path Calculation**: Utilizes Dijkstra’s Algorithm to find the shortest route between two stations.
- **Path Tracing**: Outputs the full path the metro will travel, including all stations between the start and end points.
- **Cost Estimation**: Provides estimates for the fare based on the selected route.

## Installation & Usage
1. Clone this repository to your local machine:
    ```bash
    git clone https://github.com/your-username/DELHI-METRO-ROUTE.git
    ```
2. Navigate to the project directory:
    ```bash
    cd DELHI-METRO-ROUTE
    ```
3. Compile the C++ code using a C++ compiler:
    ```bash
    g++ main.cpp -o delhi_metro
    ```
4. Run the executable:
    ```bash
    ./delhi_metro
    ```
5. Follow the prompts to input the starting and ending stations to receive the shortest path and related travel details.

## Contributions
Contributions are welcome! If you'd like to contribute, please fork the repository, make your changes, and submit a pull request.
