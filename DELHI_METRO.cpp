#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>
#include <cmath> // for sqrt and pow
#include <set>   // for set_intersection

using namespace std;

// Station details including latitude, longitude, and metro lines
struct Station {
    string name;
    double latitude;
    double longitude;
    vector<string> metroLines;
    Station(string n, double lat, double lon) : name(n), latitude(lat), longitude(lon) {}
    Station() : name(""), latitude(0), longitude(0) {} // Default constructor
};

// Edge structure
struct Edge {
    string to;
    int distance;
    vector<string> metroLines;  // Metro lines between stations
    Edge(string t, int d, vector<string> lines) : to(t), distance(d), metroLines(lines) {}
};

// Graph class using adjacency list representation
class MetroGraph {
private:
    unordered_map<string, vector<Edge>> adjacencyList;
    unordered_map<string, Station> stations;

public:
    // Function to add an undirected edge between two stations
    void addEdge(string station1, string station2, int distance, vector<string> lines) {
        adjacencyList[station1].emplace_back(station2, distance, lines);
        adjacencyList[station2].emplace_back(station1, distance, lines);
    }

    // Dijkstra's algorithm to find shortest path from source to destination
    pair<vector<string>, int> dijkstra(string source, string destination) {
        // Priority queue for Dijkstra's algorithm (min-heap)
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

        // Distance map
        unordered_map<string, int> distance;
        for (auto& pair : adjacencyList) {
            distance[pair.first] = numeric_limits<int>::max();
        }
        distance[source] = 0;

        // Previous station map for path reconstruction
        unordered_map<string, string> previous;

        // Insert source into priority queue
        pq.push({0, source});

        while (!pq.empty()) {
            string u = pq.top().second;
            int dist = pq.top().first;
            pq.pop();

            // Stop early if we reach the destination
            if (u == destination) {
                break;
            }

            // If current distance is greater than known distance, skip
            if (dist > distance[u]) {
                continue;
            }

            // Explore neighbors
            for (const auto& edge : adjacencyList[u]) {
                string v = edge.to;
                int weight = edge.distance;
                int new_dist = dist + weight;

                // Update shortest path to neighbor if found a shorter path
                if (new_dist < distance[v]) {
                    distance[v] = new_dist;
                    previous[v] = u;
                    pq.push({new_dist, v});
                }
            }
        }

        // Reconstruct path
        vector<string> path;
        int totalDistance = distance[destination];
        if (totalDistance == numeric_limits<int>::max()) {
            return {path, totalDistance}; // No path found
        }
        for (string at = destination; !at.empty(); at = previous[at]) {
            path.push_back(at);
            if (at == source) break;
        }
        reverse(path.begin(), path.end());

        return {path, totalDistance};
    }

    // Function to calculate Euclidean distance between two stations (based on latitude and longitude)
    double toRadians(double degree) {
        return degree * M_PI / 180.0;
    }

    double calculateDistance(string station1, string station2) {
        double lat1 = stations[station1].latitude;
        double lon1 = stations[station1].longitude;
        double lat2 = stations[station2].latitude;
        double lon2 = stations[station2].longitude;

        double lat1Rad = toRadians(lat1);
        double lon1Rad = toRadians(lon1);
        double lat2Rad = toRadians(lat2);
        double lon2Rad = toRadians(lon2);

        // Using Haversine formula for simplicity (considering Earth as a perfect sphere)
        double dlat = lat2Rad - lat1Rad;
        double dlon = lon2Rad - lon1Rad;

        double a = sin(dlat / 2) * sin(dlat / 2) +
                   cos(lat1Rad) * cos(lat2Rad) *
                   sin(dlon / 2) * sin(dlon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));

        // Radius of Earth in kilometers (approximate)
        const double radius = 6371.0;

        // Distance in kilometers
        double distance = radius * c;

        return distance;
    }

    // Function to calculate fare based on distance
    double calculateFare(int distance) {
        // Example fare calculation based on Delhi Metro fare structure
        if (distance <= 2) {
            return 10.0;
        } else if (distance > 2 && distance <= 5) {
            return 20.0;
        } else if (distance > 5 && distance <= 12) {
            return 30.0;
        } else if (distance > 12 && distance <= 21) {
            return 40.0;
        } else if (distance > 21 && distance <= 32) {
            return 50.0;
        } else {
            return 60.0;
        }
    }

    // Function to add a station with details including latitude and longitude
    void addStation(string name, double latitude, double longitude, vector<string> metroLines) {
        stations[name] = Station(name, latitude, longitude);
        stations[name].metroLines = metroLines;
    }

    // Function to get the metro lines common to two stations
    vector<string> getCommonLines(const string& station1, const string& station2) {
        vector<string> commonLines;
        const auto& lines1 = stations[station1].metroLines;
        const auto& lines2 = stations[station2].metroLines;

        set_intersection(lines1.begin(), lines1.end(),
                         lines2.begin(), lines2.end(),
                         back_inserter(commonLines));
        return commonLines;
    }

    // Function to get metro lines for a station
    vector<string> getMetroLines(const string& station) const {
        return stations.at(station).metroLines;
    }
};

int main() {
    MetroGraph delhiMetro;

    // Adding metro stations and connections
delhiMetro.addStation("Samaypur Badli", 28.748035, 77.134733, {"Yellow Line"});
delhiMetro.addStation("Rohini Sector 18, 19", 28.736278, 77.124249, {"Yellow Line"});
delhiMetro.addStation("Haiderpur Badli Mor", 28.721776, 77.154535, {"Yellow Line"});
delhiMetro.addStation("Jahangirpuri", 28.716939, 77.170724, {"Yellow Line"});
delhiMetro.addStation("Adarsh Nagar", 28.714401, 77.167288, {"Yellow Line"});
delhiMetro.addStation("Azadpur", 28.704235, 77.170330, {"Yellow Line"});
delhiMetro.addStation("Model Town", 28.704746, 77.185037, {"Yellow Line"});
delhiMetro.addStation("GTB Nagar", 28.699882, 77.189732, {"Yellow Line"});
delhiMetro.addStation("Vishwavidyalaya", 28.697151, 77.210441, {"Yellow Line"});
delhiMetro.addStation("Vidhan Sabha", 28.684715, 77.217919, {"Yellow Line"});
delhiMetro.addStation("Civil Lines", 28.675192, 77.225668, {"Yellow Line"});
delhiMetro.addStation("Kashmere Gate", 28.667856, 77.228885, {"Yellow Line"});
delhiMetro.addStation("Chandni Chowk", 28.657420, 77.231109, {"Yellow Line"});
delhiMetro.addStation("Chawri Bazar", 28.650121, 77.229249, {"Yellow Line"});
delhiMetro.addStation("New Delhi", 28.640382, 77.224842, {"Yellow Line"});
delhiMetro.addStation("Rajiv Chowk", 28.632782, 77.219675, {"Yellow Line"});
delhiMetro.addStation("Patel Chowk", 28.627567, 77.212830, {"Yellow Line"});
delhiMetro.addStation("Central Secretariat", 28.618566, 77.208220, {"Yellow Line"});
delhiMetro.addStation("Udyog Bhawan", 28.609851, 77.205502, {"Yellow Line"});
delhiMetro.addStation("Lok Kalyan Marg", 28.599467, 77.204842, {"Yellow Line"});
delhiMetro.addStation("Jor Bagh", 28.589578, 77.206206, {"Yellow Line"});
delhiMetro.addStation("INA", 28.578802, 77.206432, {"Yellow Line"});
delhiMetro.addStation("AIIMS", 28.567786, 77.209062, {"Yellow Line"});
delhiMetro.addStation("Green Park", 28.558330, 77.207450, {"Yellow Line"});
delhiMetro.addStation("Hauz Khas", 28.545656, 77.206173, {"Yellow Line"});
delhiMetro.addStation("Malviya Nagar", 28.528545, 77.204276, {"Yellow Line"});
delhiMetro.addStation("Saket", 28.518198, 77.206028, {"Yellow Line"});
delhiMetro.addStation("Qutab Minar", 28.508559, 77.201447, {"Yellow Line"});
delhiMetro.addStation("Chhatarpur", 28.500326, 77.175646, {"Yellow Line"});
delhiMetro.addStation("Sultanpur", 28.485960, 77.156209, {"Yellow Line"});
delhiMetro.addStation("Ghitorni", 28.474529, 77.146862, {"Yellow Line"});
delhiMetro.addStation("Arjan Garh", 28.461856, 77.137522, {"Yellow Line"});
delhiMetro.addStation("Guru Dronacharya", 28.456780, 77.121756, {"Yellow Line"});
delhiMetro.addStation("Sikanderpur", 28.481986, 77.083438, {"Yellow Line"});
delhiMetro.addStation("MG Road", 28.470822, 77.072855, {"Yellow Line"});
delhiMetro.addStation("IFFCO Chowk", 28.467601, 77.064324, {"Yellow Line"});
delhiMetro.addStation("HUDA City Centre", 28.459940, 77.050694, {"Yellow Line"});

delhiMetro.addStation("Shaheed Sthal (New Bus Adda)", 28.682328, 77.453100, {"Red Line"});
delhiMetro.addStation("Hindon River", 28.676695, 77.434548, {"Red Line"});
delhiMetro.addStation("Arthala", 28.674124, 77.420869, {"Red Line"});
delhiMetro.addStation("Mohan Nagar", 28.668681, 77.402053, {"Red Line"});
delhiMetro.addStation("Shyam Park", 28.671632, 77.386873, {"Red Line"});
delhiMetro.addStation("Major Mohit Sharma Rajendra Nagar", 28.674178, 77.374406, {"Red Line"});
delhiMetro.addStation("Raj Bagh", 28.676860, 77.361217, {"Red Line"});
delhiMetro.addStation("Shaheed Nagar", 28.678568, 77.347793, {"Red Line"});
delhiMetro.addStation("Dilshad Garden", 28.682053, 77.327570, {"Red Line"});
delhiMetro.addStation("Jhilmil", 28.675620, 77.314731, {"Red Line"});
delhiMetro.addStation("Mansarovar Park", 28.672283, 77.305071, {"Red Line"});
delhiMetro.addStation("Shahdara", 28.670072, 77.291874, {"Red Line"});
delhiMetro.addStation("Welcome", 28.672592, 77.279591, {"Red Line"});
delhiMetro.addStation("Seelampur", 28.671353, 77.266329, {"Red Line"});
delhiMetro.addStation("Shastri Park", 28.668951, 77.250956, {"Red Line"});
delhiMetro.addStation("Kashmere Gate", 28.667856, 77.228885, {"Red Line"});
delhiMetro.addStation("Tis Hazari", 28.664364, 77.216701, {"Red Line"});
delhiMetro.addStation("Pulbangash", 28.664255, 77.206060, {"Red Line"});
delhiMetro.addStation("Pratap Nagar", 28.664196, 77.193961, {"Red Line"});
delhiMetro.addStation("Shastri Nagar", 28.666105, 77.179902, {"Red Line"});
delhiMetro.addStation("Inderlok", 28.667297, 77.168057, {"Red Line"});
delhiMetro.addStation("Kanhaiya Nagar", 28.682240, 77.157448, {"Red Line"});
delhiMetro.addStation("Keshav Puram", 28.696642, 77.153415, {"Red Line"});
delhiMetro.addStation("Netaji Subhash Place", 28.698713, 77.149625, {"Red Line"});
delhiMetro.addStation("Shakurpur", 28.701120, 77.141533, {"Red Line"});
delhiMetro.addStation("Punjabi Bagh West", 28.705022, 77.131308, {"Red Line"});
delhiMetro.addStation("Ashok Park Main", 28.698374, 77.125218, {"Red Line"});
delhiMetro.addStation("Satguru Ram Singh Marg", 28.685384, 77.117357, {"Red Line"});
delhiMetro.addStation("Kirti Nagar", 28.678474, 77.120043, {"Red Line"});

delhiMetro.addStation("Kashmere Gate", 28.6672231, 77.2307327, {"Violet Line"});
delhiMetro.addStation("Lal Qila", 28.6564738, 77.2410157, {"Violet Line"});
delhiMetro.addStation("Jama Masjid", 28.6505282, 77.2360851, {"Violet Line"});
delhiMetro.addStation("Delhi Gate", 28.6429863, 77.2433636, {"Violet Line"});
delhiMetro.addStation("ITO", 28.6285392, 77.2447288, {"Violet Line"});
delhiMetro.addStation("Mandi House", 28.625755, 77.241033, {"Violet Line"});
delhiMetro.addStation("Janpath", 28.625802, 77.218707, {"Violet Line"});
delhiMetro.addStation("Central Secretariat", 28.614707, 77.209045, {"Violet Line"});
delhiMetro.addStation("Khan Market", 28.6007813, 77.2272815, {"Violet Line"});
delhiMetro.addStation("Jawaharlal Nehru Stadium", 28.5855817, 77.2402333, {"Violet Line"});
delhiMetro.addStation("Jangpura", 28.5744807, 77.2440081, {"Violet Line"});
delhiMetro.addStation("Lajpat Nagar", 28.5686478, 77.2439311, {"Violet Line"});
delhiMetro.addStation("Moolchand", 28.5583036, 77.237691, {"Violet Line"});
delhiMetro.addStation("Kailash Colony", 28.5487422, 77.240526, {"Violet Line"});
delhiMetro.addStation("Nehru Place", 28.5411366, 77.2463884, {"Violet Line"});
delhiMetro.addStation("Kalkaji Mandir", 28.5316742, 77.2588722, {"Violet Line"});
delhiMetro.addStation("Govind Puri", 28.5219614, 77.2670347, {"Violet Line"});
delhiMetro.addStation("Okhla NSIC", 28.512188, 77.269095, {"Violet Line"});
delhiMetro.addStation("Harkesh Nagar Okhla", 28.502588, 77.269853, {"Violet Line"});
delhiMetro.addStation("Jasola Apollo", 28.484569, 77.269929, {"Violet Line"});
delhiMetro.addStation("Sarita Vihar", 28.474222, 77.275438, {"Violet Line"});
delhiMetro.addStation("Mohan Estate", 28.459574, 77.282737, {"Violet Line"});
delhiMetro.addStation("Tughlakabad", 28.441823, 77.284299, {"Violet Line"});
delhiMetro.addStation("Badarpur Border", 28.424145, 77.286287, {"Violet Line"});
delhiMetro.addStation("Sarai", 28.408067, 77.291271, {"Violet Line"});
delhiMetro.addStation("NHPC Chowk", 28.393115, 77.301728, {"Violet Line"});
delhiMetro.addStation("Mewala Maharajpur", 28.380705, 77.308161, {"Violet Line"});
delhiMetro.addStation("Sector 28", 28.368541, 77.314703, {"Violet Line"});
delhiMetro.addStation("Badkhal Mor", 28.353668, 77.323024, {"Violet Line"});
delhiMetro.addStation("Old Faridabad", 28.340175, 77.327356, {"Violet Line"});
delhiMetro.addStation("Neelam Chowk Ajronda", 28.319508, 77.326519, {"Violet Line"});
delhiMetro.addStation("Bata Chowk", 28.306912, 77.321365, {"Violet Line"});
delhiMetro.addStation("Escorts Mujesar", 28.289001, 77.313548, {"Violet Line"});


delhiMetro.addStation("Majlis Park", 28.7106, 77.1386, {"Pink Line"});
delhiMetro.addStation("Keshav Puram", 28.7091, 77.1357, {"Pink Line"});
delhiMetro.addStation("Kanhaiya Nagar", 28.7069, 77.1321, {"Pink Line"});
delhiMetro.addStation("Shastri Nagar", 28.7055, 77.1305, {"Pink Line"});
delhiMetro.addStation("Tis Hazari", 28.7023, 77.1328, {"Pink Line"});
delhiMetro.addStation("Karam Pura", 28.6921, 77.1334, {"Pink Line"});
delhiMetro.addStation("Maya Puri", 28.6863, 77.1294, {"Pink Line"});
delhiMetro.addStation("Patel Nagar", 28.6920, 77.1370, {"Pink Line"});
delhiMetro.addStation("Kirti Nagar", 28.6903, 77.1513, {"Pink Line"});
delhiMetro.addStation("Rajouri Garden", 28.6614, 77.1398, {"Pink Line"});
delhiMetro.addStation("Maharani Bagh", 28.5858, 77.2501, {"Pink Line"});
delhiMetro.addStation("Hazrat Nizamuddin", 28.5863, 77.2453, {"Pink Line"});
delhiMetro.addStation("Jangpura", 28.5948, 77.2498, {"Pink Line"});
delhiMetro.addStation("Sarai Kale Khan", 28.5806, 77.2596, {"Pink Line"});
delhiMetro.addStation("New Ashok Nagar", 28.6102, 77.2728, {"Pink Line"});
delhiMetro.addStation("Noida City Centre", 28.5866, 77.3265, {"Pink Line"});
delhiMetro.addStation("Sector 15", 28.5966, 77.3318, {"Pink Line"});
delhiMetro.addStation("Sector 18", 28.5883, 77.3298, {"Pink Line"});
delhiMetro.addStation("Botanical Garden", 28.5852, 77.3364, {"Pink Line"});
delhiMetro.addStation("Sector 52", 28.5834, 77.3425, {"Pink Line"});
delhiMetro.addStation("Sector 61", 28.5795, 77.3401, {"Pink Line"});
delhiMetro.addStation("Sector 62", 28.5722, 77.3355, {"Pink Line"});
delhiMetro.addStation("Sector 63", 28.5726, 77.3349, {"Pink Line"});
delhiMetro.addStation("IIT Delhi", 28.5531, 77.1915, {"Pink Line"});
delhiMetro.addStation("Hauz Khas", 28.5505, 77.2075, {"Pink Line"});
delhiMetro.addStation("Green Park", 28.5476, 77.2046, {"Pink Line"});
delhiMetro.addStation("Aurobindo Place", 28.5396, 77.2201, {"Pink Line"});
delhiMetro.addStation("Sarai Jullena", 28.5616, 77.2514, {"Pink Line"});
delhiMetro.addStation("Ashram", 28.5664, 77.2610, {"Pink Line"});
delhiMetro.addStation("Bhikaji Cama Place", 28.5843, 77.1990, {"Pink Line"});
delhiMetro.addStation("Durgabai Deshmukh South Campus", 28.5783, 77.2073, {"Pink Line"});
delhiMetro.addStation("East Azad Nagar", 28.6422, 77.2810, {"Pink Line"});
delhiMetro.addStation("East Vinod Nagar – Mayur Vihar-II", 28.6347, 77.2886, {"Pink Line"});
delhiMetro.addStation("ESI Hospital", 28.6152, 77.2748, {"Pink Line"});
delhiMetro.addStation("Gokulpuri", 28.6952, 77.2653, {"Pink Line"});
delhiMetro.addStation("IP Extension", 28.6368, 77.2971, {"Pink Line"});
delhiMetro.addStation("Jaffrabad", 28.6945, 77.2610, {"Pink Line"});
delhiMetro.addStation("Kalindi Kunj", 28.5324, 77.2826, {"Pink Line"});


delhiMetro.addStation("Janakpuri West", 28.586826, 77.057601, {"Magenta Line"});
delhiMetro.addStation("Dabri Mor", 28.581026, 77.075396, {"Magenta Line"});
delhiMetro.addStation("Dashrathpuri", 28.580927, 77.084861, {"Magenta Line"});
delhiMetro.addStation("Palam", 28.573446, 77.099407, {"Magenta Line"});
delhiMetro.addStation("Sadar Bazar Cantonment", 28.568754, 77.108571, {"Magenta Line"});
delhiMetro.addStation("Terminal 1 IGI Airport", 28.558262, 77.095366, {"Magenta Line"});
delhiMetro.addStation("Shankar Vihar", 28.552682, 77.093794, {"Magenta Line"});
delhiMetro.addStation("Vasant Vihar", 28.558855, 77.112676, {"Magenta Line"});
delhiMetro.addStation("Munirka", 28.561509, 77.104609, {"Magenta Line"});
delhiMetro.addStation("RK Puram", 28.565783, 77.112351, {"Magenta Line"});
delhiMetro.addStation("IIT", 28.553322, 77.164053, {"Magenta Line"});
delhiMetro.addStation("Hauz Khas", 28.549788, 77.203232, {"Magenta Line"});
delhiMetro.addStation("Panchsheel Park", 28.543213, 77.213477, {"Magenta Line"});
delhiMetro.addStation("Chirag Delhi", 28.543213, 77.213477, {"Magenta Line"});
delhiMetro.addStation("Greater Kailash", 28.533080, 77.240260, {"Magenta Line"});
delhiMetro.addStation("Nehru Enclave", 28.533124, 77.251130, {"Magenta Line"});
delhiMetro.addStation("Kalkaji Mandir", 28.531680, 77.259387, {"Magenta Line"});
delhiMetro.addStation("Okhla NSIC", 28.531077, 77.279527, {"Magenta Line"});
delhiMetro.addStation("Sukhdev Vihar", 28.529560, 77.291082, {"Magenta Line"});
delhiMetro.addStation("Jamia Milia Islamiya", 28.530743, 77.306484, {"Magenta Line"});
delhiMetro.addStation("Okhla Vihar", 28.529877, 77.319420, {"Magenta Line"});
delhiMetro.addStation("Jasola Vihar Shaheen Bagh", 28.523699, 77.332795, {"Magenta Line"});
delhiMetro.addStation("Kalindi Kunj", 28.510310, 77.336334, {"Magenta Line"});
delhiMetro.addStation("Okhla Bird Sanctuary", 28.502383, 77.332879, {"Magenta Line"});
delhiMetro.addStation("Botanical Garden", 28.506285, 77.334550, {"Magenta Line"});


delhiMetro.addStation("New Delhi", 28.640196, 77.219638, {"Airport Express Line"});
delhiMetro.addStation("Shivaji Stadium", 28.631508, 77.216059, {"Airport Express Line"});
delhiMetro.addStation("Dhaula Kuan", 28.603580, 77.189060, {"Airport Express Line"});
delhiMetro.addStation("Delhi Aerocity", 28.572211, 77.195070, {"Airport Express Line"});
delhiMetro.addStation("Airport T3", 28.570166, 77.109497, {"Airport Express Line"});
delhiMetro.addStation("Dwarka Sector 21", 28.561731, 77.023850, {"Airport Express Line"});
delhiMetro.addStation("Yashobhoomi Dwarka Sector 25", 28.559616, 77.016670, {"Airport Express Line"});


delhiMetro.addStation("Noida Electronic City", 28.5602, 77.3192, {"Blue Line"});
delhiMetro.addStation("Noida Sector 62", 28.5933, 77.3324, {"Blue Line"});
delhiMetro.addStation("Noida Sector 59", 28.5956, 77.3383, {"Blue Line"});
delhiMetro.addStation("Noida Sector 61", 28.5976, 77.3398, {"Blue Line"});
delhiMetro.addStation("Noida Sector 52", 28.5927, 77.3378, {"Blue Line"});
delhiMetro.addStation("Noida Sector 34", 28.5941, 77.3284, {"Blue Line"});
delhiMetro.addStation("Noida City Centre", 28.5964, 77.3265, {"Blue Line"});
delhiMetro.addStation("Golf Course", 28.5940, 77.3585, {"Blue Line"});
delhiMetro.addStation("Botanical Garden", 28.5727, 77.3290, {"Blue Line"});
delhiMetro.addStation("Noida Sector 18", 28.5855, 77.3378, {"Blue Line"});
delhiMetro.addStation("Noida Sector 16", 28.5960, 77.3307, {"Blue Line"});
delhiMetro.addStation("Noida Sector 15", 28.5965, 77.3302, {"Blue Line"});
delhiMetro.addStation("New Ashok Nagar", 28.5914, 77.3166, {"Blue Line"});
delhiMetro.addStation("Mayur Vihar Extension", 28.5933, 77.3042, {"Blue Line"});
delhiMetro.addStation("Mayur Vihar I", 28.5941, 77.3064, {"Blue Line"});
delhiMetro.addStation("Akshardham", 28.6139, 77.2757, {"Blue Line"});
delhiMetro.addStation("Yamuna Bank", 28.6133, 77.2928, {"Blue Line"});
delhiMetro.addStation("Indraprastha", 28.6128, 77.2908, {"Blue Line"});
delhiMetro.addStation("Supreme Court (Pragati Maidan)", 28.6115, 77.2616, {"Blue Line"});
delhiMetro.addStation("Mandi House", 28.6215, 77.2321, {"Blue Line"});
delhiMetro.addStation("Barakhambha Road", 28.6288, 77.2248, {"Blue Line"});
delhiMetro.addStation("Rajiv Chowk", 28.6286, 77.2161, {"Blue Line"});
delhiMetro.addStation("RK Ashram Marg", 28.6297, 77.2094, {"Blue Line"});
delhiMetro.addStation("Jhandewalan", 28.6307, 77.2070, {"Blue Line"});
delhiMetro.addStation("Karol Bagh", 28.6312, 77.1984, {"Blue Line"});
delhiMetro.addStation("Rajendra Place", 28.6324, 77.1837, {"Blue Line"});
delhiMetro.addStation("Patel Nagar", 28.6342, 77.1698, {"Blue Line"});
delhiMetro.addStation("Shadipur", 28.6353, 77.1591, {"Blue Line"});
delhiMetro.addStation("Kirti Nagar", 28.6358, 77.1466, {"Blue Line"});
delhiMetro.addStation("Moti Nagar", 28.6356, 77.1366, {"Blue Line"});
delhiMetro.addStation("Ramesh Nagar", 28.6352, 77.1252, {"Blue Line"});
delhiMetro.addStation("Rajouri Garden", 28.6357, 77.1137, {"Blue Line"});
delhiMetro.addStation("Tagore Garden", 28.6356, 77.1015, {"Blue Line"});
delhiMetro.addStation("Subhash Nagar", 28.6341, 77.0890, {"Blue Line"});
delhiMetro.addStation("Tilak Nagar", 28.6340, 77.0768, {"Blue Line"});
delhiMetro.addStation("Janakpuri East", 28.6168, 77.0841, {"Blue Line"});
delhiMetro.addStation("Janakpuri West", 28.5868, 77.0576, {"Blue Line"});
delhiMetro.addStation("Uttam Nagar East", 28.5938, 77.0598, {"Blue Line"});
delhiMetro.addStation("Uttam Nagar West", 28.5824, 77.0464, {"Blue Line"});
delhiMetro.addStation("Nawada", 28.5702, 77.0347, {"Blue Line"});
delhiMetro.addStation("Dwarka Mor", 28.5584, 77.0277, {"Blue Line"});
delhiMetro.addStation("Dwarka", 28.5555, 77.0236, {"Blue Line"});
delhiMetro.addStation("Dwarka Sector 14", 28.5583, 77.0166, {"Blue Line"});
delhiMetro.addStation("Dwarka Sector 13", 28.5624, 77.0113, {"Blue Line"});
delhiMetro.addStation("Dwarka Sector 12", 28.5664, 77.0072, {"Blue Line"});
delhiMetro.addStation("Dwarka Sector 11", 28.5705, 77.0032, {"Blue Line"});
delhiMetro.addStation("Dwarka Sector 10", 28.5755, 76.9986, {"Blue Line"});
delhiMetro.addStation("Dwarka Sector 9", 28.5790, 76.9927, {"Blue Line"});
delhiMetro.addStation("Dwarka Sector 8", 28.5831, 76.9869, {"Blue Line"});
delhiMetro.addStation("Dwarka Sector 21", 28.5606, 77.0217, {"Blue Line"});

    // Adding connections with calculated distances
   double a1 = delhiMetro.calculateDistance("Samaypur Badli", "Rohini Sector 18, 19");
double a2 = delhiMetro.calculateDistance("Rohini Sector 18, 19", "Haiderpur Badli Mor");
double a3 = delhiMetro.calculateDistance("Haiderpur Badli Mor", "Jahangirpuri");
double a4 = delhiMetro.calculateDistance("Jahangirpuri", "Adarsh Nagar");
double a5 = delhiMetro.calculateDistance("Adarsh Nagar", "Azadpur");
double a6 = delhiMetro.calculateDistance("Azadpur","Model Town");
double a7 = delhiMetro.calculateDistance("Model Town", "GTB Nagar");
double a8 = delhiMetro.calculateDistance("GTB Nagar", "Vishwavidyalaya");
double a9 = delhiMetro.calculateDistance("Vishwavidyalaya", "Vidhan Sabha");
double a10 = delhiMetro.calculateDistance("Vidhan Sabha", "Civil Lines");
double a11 = delhiMetro.calculateDistance("Civil Lines", "Kashmere Gate");
double a12 = delhiMetro.calculateDistance("Kashmere Gate", "Chandni Chowk");
double a13 = delhiMetro.calculateDistance("Chandni Chowk", "Chawri Bazar");
double a14 = delhiMetro.calculateDistance("Chawri Bazar", "New Delhi");
double a15 = delhiMetro.calculateDistance("New Delhi", "Rajiv Chowk");
double a16 = delhiMetro.calculateDistance("Rajiv Chowk", "Patel Chowk");
double a17 = delhiMetro.calculateDistance("Patel Chowk", "Central Secretariat");
double a18 = delhiMetro.calculateDistance("Central Secretariat", "Udyog Bhawan");
double a19 = delhiMetro.calculateDistance("Udyog Bhawan", "Lok Kalyan Marg");
double a20 = delhiMetro.calculateDistance("Lok Kalyan Marg", "Jor Bagh");
double a21 = delhiMetro.calculateDistance("Jor Bagh", "INA");
double a22 = delhiMetro.calculateDistance("INA", "AIIMS");
double a23 = delhiMetro.calculateDistance("AIIMS", "Green Park");
double a24 = delhiMetro.calculateDistance("Green Park", "Hauz Khas");
double a25 = delhiMetro.calculateDistance("Hauz Khas", "Malviya Nagar");
double a26 = delhiMetro.calculateDistance("Malviya Nagar", "Saket");
double a27 = delhiMetro.calculateDistance("Saket", "Qutab Minar");
double a28 = delhiMetro.calculateDistance("Qutab Minar", "Chhatarpur");
double a29 = delhiMetro.calculateDistance("Chhatarpur", "Sultanpur");
double a30 = delhiMetro.calculateDistance("Sultanpur", "Ghitorni");
double a31 = delhiMetro.calculateDistance("Ghitorni", "Arjan Garh");
double a32 = delhiMetro.calculateDistance("Arjan Garh", "Guru Dronacharya");
double a33 = delhiMetro.calculateDistance("Guru Dronacharya", "Sikanderpur");
double a34 = delhiMetro.calculateDistance("Sikanderpur", "MG Road");
double a35 = delhiMetro.calculateDistance("MG Road", "IFFCO Chowk");
double a36 = delhiMetro.calculateDistance("IFFCO Chowk", "HUDA City Centre");

double b1 = delhiMetro.calculateDistance("Shaheed Sthal (New Bus Adda)", "Hindon River");
double b2 = delhiMetro.calculateDistance("Hindon River", "Arthala");
double b3 = delhiMetro.calculateDistance("Arthala", "Mohan Nagar");
double b4 = delhiMetro.calculateDistance("Mohan Nagar", "Shyam Park");
double b5 = delhiMetro.calculateDistance("Shyam Park", "Major Mohit Sharma Rajendra Nagar");
double b6 = delhiMetro.calculateDistance("Major Mohit Sharma Rajendra Nagar", "Raj Bagh");
double b7 = delhiMetro.calculateDistance("Raj Bagh", "Shaheed Nagar");
double b8 = delhiMetro.calculateDistance("Shaheed Nagar", "Dilshad Garden");
double b9 = delhiMetro.calculateDistance("Dilshad Garden", "Jhilmil");
double b10 = delhiMetro.calculateDistance("Jhilmil", "Mansarovar Park");
double b11 = delhiMetro.calculateDistance("Mansarovar Park", "Shahdara");
double b12 = delhiMetro.calculateDistance("Shahdara", "Welcome");
double b13 = delhiMetro.calculateDistance("Welcome", "Seelampur");
double b14 = delhiMetro.calculateDistance("Seelampur", "Shastri Park");
double b15 = delhiMetro.calculateDistance("Shastri Park", "Kashmere Gate");
double b16 = delhiMetro.calculateDistance("Kashmere Gate", "Tis Hazari");
double b17 = delhiMetro.calculateDistance("Tis Hazari", "Pulbangash");
double b18 = delhiMetro.calculateDistance("Pulbangash", "Pratap Nagar");
double b19 = delhiMetro.calculateDistance("Pratap Nagar", "Shastri Nagar");
double b20 = delhiMetro.calculateDistance("Shastri Nagar", "Inderlok");
double b21 = delhiMetro.calculateDistance("Inderlok", "Kanhaiya Nagar");
double b22 = delhiMetro.calculateDistance("Kanhaiya Nagar", "Keshav Puram");
double b23 = delhiMetro.calculateDistance("Keshav Puram", "Netaji Subhash Place");
double b24 = delhiMetro.calculateDistance("Netaji Subhash Place", "Shakurpur");
double b25 = delhiMetro.calculateDistance("Shakurpur", "Punjabi Bagh West");
double b26 = delhiMetro.calculateDistance("Punjabi Bagh West", "Ashok Park Main");
double b27 = delhiMetro.calculateDistance("Ashok Park Main", "Satguru Ram Singh Marg");
double b28 = delhiMetro.calculateDistance("Satguru Ram Singh Marg", "Kirti Nagar");


double d1 = delhiMetro.calculateDistance("Kashmere Gate", "Lal Qila");
double d2 = delhiMetro.calculateDistance("Lal Qila", "Jama Masjid");
double d3 = delhiMetro.calculateDistance("Jama Masjid", "Delhi Gate");
double d4 = delhiMetro.calculateDistance("Delhi Gate", "ITO");
double d5 = delhiMetro.calculateDistance("ITO", "Mandi House");
double d6 = delhiMetro.calculateDistance("Mandi House", "Janpath");
double d7 = delhiMetro.calculateDistance("Janpath", "Central Secretariat");
double d8 = delhiMetro.calculateDistance("Central Secretariat", "Khan Market");
double d9 = delhiMetro.calculateDistance("Khan Market", "Jawaharlal Nehru Stadium");
double d10 = delhiMetro.calculateDistance("Jawaharlal Nehru Stadium", "Jangpura");
double d11 = delhiMetro.calculateDistance("Jangpura", "Lajpat Nagar");
double d12 = delhiMetro.calculateDistance("Lajpat Nagar", "Moolchand");
double d13 = delhiMetro.calculateDistance("Moolchand", "Kailash Colony");
double d14 = delhiMetro.calculateDistance("Kailash Colony", "Nehru Place");
double d15 = delhiMetro.calculateDistance("Nehru Place", "Kalkaji Mandir");
double d16 = delhiMetro.calculateDistance("Kalkaji Mandir", "Govind Puri");
double d17 = delhiMetro.calculateDistance("Govind Puri", "Okhla NSIC");
double d18 = delhiMetro.calculateDistance("Okhla NSIC", "Harkesh Nagar Okhla");
double d19 = delhiMetro.calculateDistance("Harkesh Nagar Okhla", "Jasola Apollo");
double d20 = delhiMetro.calculateDistance("Jasola Apollo", "Sarita Vihar");
double d21 = delhiMetro.calculateDistance("Sarita Vihar", "Mohan Estate");
double d22 = delhiMetro.calculateDistance("Mohan Estate", "Tughlakabad");
double d23 = delhiMetro.calculateDistance("Tughlakabad", "Badarpur Border");
double d24 = delhiMetro.calculateDistance("Badarpur Border", "Sarai");
double d25 = delhiMetro.calculateDistance("Sarai", "NHPC Chowk");
double d26 = delhiMetro.calculateDistance("NHPC Chowk", "Mewala Maharajpur");
double d27 = delhiMetro.calculateDistance("Mewala Maharajpur", "Sector 28");
double d28 = delhiMetro.calculateDistance("Sector 28", "Badkhal Mor");
double d29 = delhiMetro.calculateDistance("Badkhal Mor", "Old Faridabad");
double d30 = delhiMetro.calculateDistance("Old Faridabad", "Neelam Chowk Ajronda");
double d31 = delhiMetro.calculateDistance("Neelam Chowk Ajronda", "Bata Chowk");
double d32 = delhiMetro.calculateDistance("Bata Chowk", "Escorts Mujesar");


double e1 = delhiMetro.calculateDistance("Majlis Park", "Keshav Puram");
double e2 = delhiMetro.calculateDistance("Keshav Puram", "Kanhaiya Nagar");
double e3 = delhiMetro.calculateDistance("Kanhaiya Nagar", "Shastri Nagar");
double e4 = delhiMetro.calculateDistance("Shastri Nagar", "Tis Hazari");
double e5 = delhiMetro.calculateDistance("Tis Hazari", "Karam Pura");
double e6 = delhiMetro.calculateDistance("Karam Pura", "Maya Puri");
double e7 = delhiMetro.calculateDistance("Maya Puri", "Patel Nagar");
double e8 = delhiMetro.calculateDistance("Patel Nagar", "Kirti Nagar");
double e9 = delhiMetro.calculateDistance("Kirti Nagar", "Rajouri Garden");
double e10 = delhiMetro.calculateDistance("Rajouri Garden", "Maharani Bagh");
double e11 = delhiMetro.calculateDistance("Maharani Bagh", "Hazrat Nizamuddin");
double e12 = delhiMetro.calculateDistance("Hazrat Nizamuddin", "Jangpura");
double e13 = delhiMetro.calculateDistance("Jangpura", "Sarai Kale Khan");
double e14 = delhiMetro.calculateDistance("Sarai Kale Khan", "New Ashok Nagar");
double e15 = delhiMetro.calculateDistance("New Ashok Nagar", "Noida City Centre");
double e16 = delhiMetro.calculateDistance("Noida City Centre", "Sector 15");
double e17 = delhiMetro.calculateDistance("Sector 15", "Sector 18");
double e18 = delhiMetro.calculateDistance("Sector 18", "Botanical Garden");
double e19 = delhiMetro.calculateDistance("Botanical Garden", "Sector 52");
double e20 = delhiMetro.calculateDistance("Sector 52", "Sector 61");
double e21 = delhiMetro.calculateDistance("Sector 61", "Sector 62");
double e22 = delhiMetro.calculateDistance("Sector 62", "Sector 63");
double e23 = delhiMetro.calculateDistance("Sector 63", "IIT Delhi");
double e24 = delhiMetro.calculateDistance("IIT Delhi", "Hauz Khas");
double e25 = delhiMetro.calculateDistance("Hauz Khas", "Green Park");
double e26 = delhiMetro.calculateDistance("Green Park", "Aurobindo Place");
double e27 = delhiMetro.calculateDistance("Aurobindo Place", "Sarai Jullena");
double e28 = delhiMetro.calculateDistance("Sarai Jullena", "Ashram");
double e29 = delhiMetro.calculateDistance("Ashram", "Bhikaji Cama Place");
double e30 = delhiMetro.calculateDistance("Bhikaji Cama Place", "Durgabai Deshmukh South Campus");
double e31 = delhiMetro.calculateDistance("Durgabai Deshmukh South Campus", "East Azad Nagar");
double e32 = delhiMetro.calculateDistance("East Azad Nagar", "East Vinod Nagar – Mayur Vihar-II");
double e33 = delhiMetro.calculateDistance("East Vinod Nagar – Mayur Vihar-II", "ESI Hospital");
double e34 = delhiMetro.calculateDistance("ESI Hospital", "Gokulpuri");
double e35 = delhiMetro.calculateDistance("Gokulpuri", "IP Extension");
double e36 = delhiMetro.calculateDistance("IP Extension", "Jaffrabad");
double e37 = delhiMetro.calculateDistance("Jaffrabad", "Kalindi Kunj");


double f1 = delhiMetro.calculateDistance("Janakpuri West", "Dabri Mor");
double f2 = delhiMetro.calculateDistance("Dabri Mor", "Dashrathpuri");
double f3 = delhiMetro.calculateDistance("Dashrathpuri", "Palam");
double f4 = delhiMetro.calculateDistance("Palam", "Sadar Bazar Cantonment");
double f5 = delhiMetro.calculateDistance("Sadar Bazar Cantonment", "Terminal 1 IGI Airport");
double f6 = delhiMetro.calculateDistance("Terminal 1 IGI Airport", "Shankar Vihar");
double f7 = delhiMetro.calculateDistance("Shankar Vihar", "Vasant Vihar");
double f8 = delhiMetro.calculateDistance("Vasant Vihar", "Munirka");
double f9 = delhiMetro.calculateDistance("Munirka", "RK Puram");
double f10 = delhiMetro.calculateDistance("RK Puram", "IIT");
double f11 = delhiMetro.calculateDistance("IIT", "Hauz Khas");
double f12 = delhiMetro.calculateDistance("Hauz Khas", "Panchsheel Park");
double f13 = delhiMetro.calculateDistance("Panchsheel Park", "Chirag Delhi");
double f14 = delhiMetro.calculateDistance("Chirag Delhi", "Greater Kailash");
double f15 = delhiMetro.calculateDistance("Greater Kailash", "Nehru Enclave");
double f16 = delhiMetro.calculateDistance("Nehru Enclave", "Kalkaji Mandir");
double f17 = delhiMetro.calculateDistance("Kalkaji Mandir", "Okhla NSIC");
double f18 = delhiMetro.calculateDistance("Okhla NSIC", "Sukhdev Vihar");
double f19 = delhiMetro.calculateDistance("Sukhdev Vihar", "Jamia Milia Islamiya");
double f20 = delhiMetro.calculateDistance("Jamia Milia Islamiya", "Okhla Vihar");
double f21 = delhiMetro.calculateDistance("Okhla Vihar", "Jasola Vihar Shaheen Bagh");
double f22 = delhiMetro.calculateDistance("Jasola Vihar Shaheen Bagh", "Kalindi Kunj");
double f23 = delhiMetro.calculateDistance("Kalindi Kunj", "Okhla Bird Sanctuary");
double f24 = delhiMetro.calculateDistance("Okhla Bird Sanctuary", "Botanical Garden");



double g1 = delhiMetro.calculateDistance("New Delhi", "Shivaji Stadium");
double g2 = delhiMetro.calculateDistance("Shivaji Stadium", "Dhaula Kuan");
double g3 = delhiMetro.calculateDistance("Dhaula Kuan", "Delhi Aerocity");
double g4 = delhiMetro.calculateDistance("Delhi Aerocity", "Airport T3");
double g5 = delhiMetro.calculateDistance("Airport T3", "Dwarka Sector 21");
double g6 = delhiMetro.calculateDistance("Dwarka Sector 21", "Yashobhoomi Dwarka Sector 25");


// Distances between the stations on the Blue Line
double i1 = delhiMetro.calculateDistance("Noida Electronic City", "Noida Sector 62");
double i2 = delhiMetro.calculateDistance("Noida Sector 62", "Noida Sector 59");
double i3 = delhiMetro.calculateDistance("Noida Sector 59", "Noida Sector 61");
double i4 = delhiMetro.calculateDistance("Noida Sector 61", "Noida Sector 52");
double i5 = delhiMetro.calculateDistance("Noida Sector 52", "Noida Sector 34");
double i6 = delhiMetro.calculateDistance("Noida Sector 34", "Noida City Centre");
double i7 = delhiMetro.calculateDistance("Noida City Centre", "Golf Course");
double i8 = delhiMetro.calculateDistance("Golf Course", "Botanical Garden");
double i9 = delhiMetro.calculateDistance("Botanical Garden", "Noida Sector 18");
double i10 = delhiMetro.calculateDistance("Noida Sector 18", "Noida Sector 16");
double i11 = delhiMetro.calculateDistance("Noida Sector 16", "Noida Sector 15");
double i12 = delhiMetro.calculateDistance("Noida Sector 15", "New Ashok Nagar");
double i13 = delhiMetro.calculateDistance("New Ashok Nagar", "Mayur Vihar Extension");
double i14 = delhiMetro.calculateDistance("Mayur Vihar Extension", "Mayur Vihar I");
double i15 = delhiMetro.calculateDistance("Mayur Vihar I", "Akshardham");
double i16 = delhiMetro.calculateDistance("Akshardham", "Yamuna Bank");
double i17 = delhiMetro.calculateDistance("Yamuna Bank", "Indraprastha");
double i18 = delhiMetro.calculateDistance("Indraprastha", "Supreme Court (Pragati Maidan)");
double i19 = delhiMetro.calculateDistance("Supreme Court (Pragati Maidan)", "Mandi House");
double i20 = delhiMetro.calculateDistance("Mandi House", "Barakhambha Road");
double i21 = delhiMetro.calculateDistance("Barakhambha Road", "Rajiv Chowk");
double i22 = delhiMetro.calculateDistance("Rajiv Chowk", "RK Ashram Marg");
double i23 = delhiMetro.calculateDistance("RK Ashram Marg", "Jhandewalan");
double i24 = delhiMetro.calculateDistance("Jhandewalan", "Karol Bagh");
double i25 = delhiMetro.calculateDistance("Karol Bagh", "Rajendra Place");
double i26 = delhiMetro.calculateDistance("Rajendra Place", "Patel Nagar");
double i27 = delhiMetro.calculateDistance("Patel Nagar", "Shadipur");
double i28 = delhiMetro.calculateDistance("Shadipur", "Kirti Nagar");
double i29 = delhiMetro.calculateDistance("Kirti Nagar", "Moti Nagar");
double i30 = delhiMetro.calculateDistance("Moti Nagar", "Ramesh Nagar");
double i31 = delhiMetro.calculateDistance("Ramesh Nagar", "Rajouri Garden");
double i32 = delhiMetro.calculateDistance("Rajouri Garden", "Tagore Garden");
double i33 = delhiMetro.calculateDistance("Tagore Garden", "Subhash Nagar");
double i34 = delhiMetro.calculateDistance("Subhash Nagar", "Tilak Nagar");
double i35 = delhiMetro.calculateDistance("Tilak Nagar", "Janakpuri East");
double i36 = delhiMetro.calculateDistance("Janakpuri East", "Janakpuri West");
double i37 = delhiMetro.calculateDistance("Janakpuri West", "Uttam Nagar East");
double i38 = delhiMetro.calculateDistance("Uttam Nagar East", "Uttam Nagar West");
double i39 = delhiMetro.calculateDistance("Uttam Nagar West", "Nawada");
double i40 = delhiMetro.calculateDistance("Nawada", "Dwarka Mor");
double i41 = delhiMetro.calculateDistance("Dwarka Mor", "Dwarka");
double i42 = delhiMetro.calculateDistance("Dwarka", "Dwarka Sector 14");
double i43 = delhiMetro.calculateDistance("Dwarka Sector 14", "Dwarka Sector 13");
double i44 = delhiMetro.calculateDistance("Dwarka Sector 13", "Dwarka Sector 12");
double i45 = delhiMetro.calculateDistance("Dwarka Sector 12", "Dwarka Sector 11");
double i46 = delhiMetro.calculateDistance("Dwarka Sector 11", "Dwarka Sector 10");
double i47 = delhiMetro.calculateDistance("Dwarka Sector 10", "Dwarka Sector 9");
double i48 = delhiMetro.calculateDistance("Dwarka Sector 9", "Dwarka Sector 8");
double i49 = delhiMetro.calculateDistance("Dwarka Sector 8", "Dwarka Sector 21");

delhiMetro.addEdge("Samaypur Badli", "Rohini Sector 18, 19", static_cast<int>(a1), {"Yellow Line"});
delhiMetro.addEdge("Rohini Sector 18, 19", "Samaypur Badli", static_cast<int>(a1), {"Yellow Line"});

delhiMetro.addEdge("Rohini Sector 18, 19", "Haiderpur Badli Mor", static_cast<int>(a2), {"Yellow Line"});
delhiMetro.addEdge("Haiderpur Badli Mor", "Rohini Sector 18, 19", static_cast<int>(a2), {"Yellow Line"});

delhiMetro.addEdge("Haiderpur Badli Mor", "Jahangirpuri", static_cast<int>(a3), {"Yellow Line"});
delhiMetro.addEdge("Jahangirpuri", "Haiderpur Badli Mor", static_cast<int>(a3), {"Yellow Line"});

delhiMetro.addEdge("Jahangirpuri", "Adarsh Nagar", static_cast<int>(a4), {"Yellow Line"});
delhiMetro.addEdge("Adarsh Nagar", "Jahangirpuri", static_cast<int>(a4), {"Yellow Line"});

delhiMetro.addEdge("Adarsh Nagar", "Azadpur", static_cast<int>(a5), {"Yellow Line"});
delhiMetro.addEdge("Azadpur", "Adarsh Nagar", static_cast<int>(a5), {"Yellow Line"});

delhiMetro.addEdge("Azadpur", "Model Town", static_cast<int>(a6), {"Yellow Line"});
delhiMetro.addEdge("Model Town", "Azadpur", static_cast<int>(a6), {"Yellow Line"});

delhiMetro.addEdge("Model Town", "GTB Nagar", static_cast<int>(a7), {"Yellow Line"});
delhiMetro.addEdge("GTB Nagar", "Model Town", static_cast<int>(a7), {"Yellow Line"});

delhiMetro.addEdge("GTB Nagar", "Vishwavidyalaya", static_cast<int>(a8), {"Yellow Line"});
delhiMetro.addEdge("Vishwavidyalaya", "GTB Nagar", static_cast<int>(a8), {"Yellow Line"});

delhiMetro.addEdge("Vishwavidyalaya", "Vidhan Sabha", static_cast<int>(a9), {"Yellow Line"});
delhiMetro.addEdge("Vidhan Sabha", "Vishwavidyalaya", static_cast<int>(a9), {"Yellow Line"});

delhiMetro.addEdge("Vidhan Sabha", "Civil Lines", static_cast<int>(a10), {"Yellow Line"});
delhiMetro.addEdge("Civil Lines", "Vidhan Sabha", static_cast<int>(a10), {"Yellow Line"});

delhiMetro.addEdge("Civil Lines", "Kashmere Gate", static_cast<int>(a11), {"Yellow Line"});
delhiMetro.addEdge("Kashmere Gate", "Civil Lines", static_cast<int>(a11), {"Yellow Line"});

delhiMetro.addEdge("Kashmere Gate", "Chandni Chowk", static_cast<int>(a12), {"Yellow Line"});
delhiMetro.addEdge("Chandni Chowk", "Kashmere Gate", static_cast<int>(a12), {"Yellow Line"});

delhiMetro.addEdge("Chandni Chowk", "Chawri Bazar", static_cast<int>(a13), {"Yellow Line"});
delhiMetro.addEdge("Chawri Bazar", "Chandni Chowk", static_cast<int>(a13), {"Yellow Line"});

delhiMetro.addEdge("Chawri Bazar", "New Delhi", static_cast<int>(a14), {"Yellow Line"});
delhiMetro.addEdge("New Delhi", "Chawri Bazar", static_cast<int>(a14), {"Yellow Line"});

delhiMetro.addEdge("New Delhi", "Rajiv Chowk", static_cast<int>(a15), {"Yellow Line"});
delhiMetro.addEdge("Rajiv Chowk", "New Delhi", static_cast<int>(a15), {"Yellow Line"});

delhiMetro.addEdge("Rajiv Chowk", "Patel Chowk", static_cast<int>(a16), {"Yellow Line"});
delhiMetro.addEdge("Patel Chowk", "Rajiv Chowk", static_cast<int>(a16), {"Yellow Line"});

delhiMetro.addEdge("Patel Chowk", "Central Secretariat", static_cast<int>(a17), {"Yellow Line"});
delhiMetro.addEdge("Central Secretariat", "Patel Chowk", static_cast<int>(a17), {"Yellow Line"});

delhiMetro.addEdge("Central Secretariat", "Udyog Bhawan", static_cast<int>(a18), {"Yellow Line"});
delhiMetro.addEdge("Udyog Bhawan", "Central Secretariat", static_cast<int>(a18), {"Yellow Line"});

delhiMetro.addEdge("Udyog Bhawan", "Lok Kalyan Marg", static_cast<int>(a19), {"Yellow Line"});
delhiMetro.addEdge("Lok Kalyan Marg", "Udyog Bhawan", static_cast<int>(a19), {"Yellow Line"});

delhiMetro.addEdge("Lok Kalyan Marg", "Jor Bagh", static_cast<int>(a20), {"Yellow Line"});
delhiMetro.addEdge("Jor Bagh", "Lok Kalyan Marg", static_cast<int>(a20), {"Yellow Line"});

delhiMetro.addEdge("Jor Bagh", "INA", static_cast<int>(a21), {"Yellow Line"});
delhiMetro.addEdge("INA", "Jor Bagh", static_cast<int>(a21), {"Yellow Line"});

delhiMetro.addEdge("INA", "AIIMS", static_cast<int>(a22), {"Yellow Line"});
delhiMetro.addEdge("AIIMS", "INA", static_cast<int>(a22), {"Yellow Line"});

delhiMetro.addEdge("AIIMS", "Green Park", static_cast<int>(a23), {"Yellow Line"});
delhiMetro.addEdge("Green Park", "AIIMS", static_cast<int>(a23), {"Yellow Line"});

delhiMetro.addEdge("Green Park", "Hauz Khas", static_cast<int>(a24), {"Yellow Line"});
delhiMetro.addEdge("Hauz Khas", "Green Park", static_cast<int>(a24), {"Yellow Line"});

delhiMetro.addEdge("Hauz Khas", "Malviya Nagar", static_cast<int>(a25), {"Yellow Line"});
delhiMetro.addEdge("Malviya Nagar", "Hauz Khas", static_cast<int>(a25), {"Yellow Line"});

delhiMetro.addEdge("Malviya Nagar", "Saket", static_cast<int>(a26), {"Yellow Line"});
delhiMetro.addEdge("Saket", "Malviya Nagar", static_cast<int>(a26), {"Yellow Line"});

delhiMetro.addEdge("Saket", "Qutab Minar", static_cast<int>(a27), {"Yellow Line"});
delhiMetro.addEdge("Qutab Minar", "Saket", static_cast<int>(a27), {"Yellow Line"});

delhiMetro.addEdge("Qutab Minar", "Chhatarpur", static_cast<int>(a28), {"Yellow Line"});
delhiMetro.addEdge("Chhatarpur", "Qutab Minar", static_cast<int>(a28), {"Yellow Line"});

delhiMetro.addEdge("Chhatarpur", "Sultanpur", static_cast<int>(a29), {"Yellow Line"});
delhiMetro.addEdge("Sultanpur", "Chhatarpur", static_cast<int>(a29), {"Yellow Line"});

delhiMetro.addEdge("Sultanpur", "Ghitorni", static_cast<int>(a30), {"Yellow Line"});
delhiMetro.addEdge("Ghitorni", "Sultanpur", static_cast<int>(a30), {"Yellow Line"});

delhiMetro.addEdge("Ghitorni", "Arjan Garh", static_cast<int>(a31), {"Yellow Line"});
delhiMetro.addEdge("Arjan Garh", "Ghitorni", static_cast<int>(a31), {"Yellow Line"});

delhiMetro.addEdge("Arjan Garh", "Guru Dronacharya", static_cast<int>(a32), {"Yellow Line"});
delhiMetro.addEdge("Guru Dronacharya", "Arjan Garh", static_cast<int>(a32), {"Yellow Line"});

delhiMetro.addEdge("Guru Dronacharya", "Sikanderpur", static_cast<int>(a33), {"Yellow Line"});
delhiMetro.addEdge("Sikanderpur", "Guru Dronacharya", static_cast<int>(a33), {"Yellow Line"});

delhiMetro.addEdge("Sikanderpur", "MG Road", static_cast<int>(a34), {"Yellow Line"});
delhiMetro.addEdge("MG Road", "Sikanderpur", static_cast<int>(a34), {"Yellow Line"});

delhiMetro.addEdge("MG Road", "IFFCO Chowk", static_cast<int>(a35), {"Yellow Line"});
delhiMetro.addEdge("IFFCO Chowk", "MG Road", static_cast<int>(a35), {"Yellow Line"});

delhiMetro.addEdge("IFFCO Chowk", "HUDA City Centre", static_cast<int>(a36), {"Yellow Line"});
delhiMetro.addEdge("HUDA City Centre", "IFFCO Chowk", static_cast<int>(a36), {"Yellow Line"});


delhiMetro.addEdge("Shaheed Sthal (New Bus Adda)", "Hindon River", static_cast<int>(b1), {"Red Line"});
delhiMetro.addEdge("Hindon River", "Shaheed Sthal (New Bus Adda)", static_cast<int>(b1), {"Red Line"});

delhiMetro.addEdge("Hindon River", "Arthala", static_cast<int>(b2), {"Red Line"});
delhiMetro.addEdge("Arthala", "Hindon River", static_cast<int>(b2), {"Red Line"});

delhiMetro.addEdge("Arthala", "Mohan Nagar", static_cast<int>(b3), {"Red Line"});
delhiMetro.addEdge("Mohan Nagar", "Arthala", static_cast<int>(b3), {"Red Line"});

delhiMetro.addEdge("Mohan Nagar", "Shyam Park", static_cast<int>(b4), {"Red Line"});
delhiMetro.addEdge("Shyam Park", "Mohan Nagar", static_cast<int>(b4), {"Red Line"});

delhiMetro.addEdge("Shyam Park", "Major Mohit Sharma Rajendra Nagar", static_cast<int>(b5), {"Red Line"});
delhiMetro.addEdge("Major Mohit Sharma Rajendra Nagar", "Shyam Park", static_cast<int>(b5), {"Red Line"});

delhiMetro.addEdge("Major Mohit Sharma Rajendra Nagar", "Raj Bagh", static_cast<int>(b6), {"Red Line"});
delhiMetro.addEdge("Raj Bagh", "Major Mohit Sharma Rajendra Nagar", static_cast<int>(b6), {"Red Line"});

delhiMetro.addEdge("Raj Bagh", "Shaheed Nagar", static_cast<int>(b7), {"Red Line"});
delhiMetro.addEdge("Shaheed Nagar", "Raj Bagh", static_cast<int>(b7), {"Red Line"});

delhiMetro.addEdge("Shaheed Nagar", "Dilshad Garden", static_cast<int>(b8), {"Red Line"});
delhiMetro.addEdge("Dilshad Garden", "Shaheed Nagar", static_cast<int>(b8), {"Red Line"});

delhiMetro.addEdge("Dilshad Garden", "Jhilmil", static_cast<int>(b9), {"Red Line"});
delhiMetro.addEdge("Jhilmil", "Dilshad Garden", static_cast<int>(b9), {"Red Line"});

delhiMetro.addEdge("Jhilmil", "Mansarovar Park", static_cast<int>(b10), {"Red Line"});
delhiMetro.addEdge("Mansarovar Park", "Jhilmil", static_cast<int>(b10), {"Red Line"});

delhiMetro.addEdge("Mansarovar Park", "Shahdara", static_cast<int>(b11), {"Red Line"});
delhiMetro.addEdge("Shahdara", "Mansarovar Park", static_cast<int>(b11), {"Red Line"});

delhiMetro.addEdge("Shahdara", "Welcome", static_cast<int>(b12), {"Red Line"});
delhiMetro.addEdge("Welcome", "Shahdara", static_cast<int>(b12), {"Red Line"});

delhiMetro.addEdge("Welcome", "Seelampur", static_cast<int>(b13), {"Red Line"});
delhiMetro.addEdge("Seelampur", "Welcome", static_cast<int>(b13), {"Red Line"});

delhiMetro.addEdge("Seelampur", "Shastri Park", static_cast<int>(b14), {"Red Line"});
delhiMetro.addEdge("Shastri Park", "Seelampur", static_cast<int>(b14), {"Red Line"});

delhiMetro.addEdge("Shastri Park", "Kashmere Gate", static_cast<int>(b15), {"Red Line"});
delhiMetro.addEdge("Kashmere Gate", "Shastri Park", static_cast<int>(b15), {"Red Line"});

delhiMetro.addEdge("Kashmere Gate", "Tis Hazari", static_cast<int>(b16), {"Red Line"});
delhiMetro.addEdge("Tis Hazari", "Kashmere Gate", static_cast<int>(b16), {"Red Line"});

delhiMetro.addEdge("Tis Hazari", "Pulbangash", static_cast<int>(b17), {"Red Line"});
delhiMetro.addEdge("Pulbangash", "Tis Hazari", static_cast<int>(b17), {"Red Line"});

delhiMetro.addEdge("Pulbangash", "Pratap Nagar", static_cast<int>(b18), {"Red Line"});
delhiMetro.addEdge("Pratap Nagar", "Pulbangash", static_cast<int>(b18), {"Red Line"});

delhiMetro.addEdge("Pratap Nagar", "Shastri Nagar", static_cast<int>(b19), {"Red Line"});
delhiMetro.addEdge("Shastri Nagar", "Pratap Nagar", static_cast<int>(b19), {"Red Line"});

delhiMetro.addEdge("Shastri Nagar", "Inderlok", static_cast<int>(b20), {"Red Line"});
delhiMetro.addEdge("Inderlok", "Shastri Nagar", static_cast<int>(b20), {"Red Line"});

delhiMetro.addEdge("Inderlok", "Kanhaiya Nagar", static_cast<int>(b21), {"Red Line"});
delhiMetro.addEdge("Kanhaiya Nagar", "Inderlok", static_cast<int>(b21), {"Red Line"});

delhiMetro.addEdge("Kanhaiya Nagar", "Keshav Puram", static_cast<int>(b22), {"Red Line"});
delhiMetro.addEdge("Keshav Puram", "Kanhaiya Nagar", static_cast<int>(b22), {"Red Line"});

delhiMetro.addEdge("Keshav Puram", "Netaji Subhash Place", static_cast<int>(b23), {"Red Line"});
delhiMetro.addEdge("Netaji Subhash Place", "Keshav Puram", static_cast<int>(b23), {"Red Line"});

delhiMetro.addEdge("Netaji Subhash Place", "Shakurpur", static_cast<int>(b24), {"Red Line"});
delhiMetro.addEdge("Shakurpur", "Netaji Subhash Place", static_cast<int>(b24), {"Red Line"});

delhiMetro.addEdge("Shakurpur", "Punjabi Bagh West", static_cast<int>(b25), {"Red Line"});
delhiMetro.addEdge("Punjabi Bagh West", "Shakurpur", static_cast<int>(b25), {"Red Line"});

delhiMetro.addEdge("Punjabi Bagh West", "Ashok Park Main", static_cast<int>(b26), {"Red Line"});
delhiMetro.addEdge("Ashok Park Main", "Punjabi Bagh West", static_cast<int>(b26), {"Red Line"});

delhiMetro.addEdge("Ashok Park Main", "Satguru Ram Singh Marg", static_cast<int>(b27), {"Red Line"});
delhiMetro.addEdge("Satguru Ram Singh Marg", "Ashok Park Main", static_cast<int>(b27), {"Red Line"});

delhiMetro.addEdge("Satguru Ram Singh Marg", "Kirti Nagar", static_cast<int>(b28), {"Red Line"});
delhiMetro.addEdge("Kirti Nagar", "Satguru Ram Singh Marg", static_cast<int>(b28), {"Red Line"});


delhiMetro.addEdge("Kashmere Gate", "Lal Qila", static_cast<int>(d1), {"Violet Line"});
delhiMetro.addEdge("Lal Qila", "Kashmere Gate", static_cast<int>(d1), {"Violet Line"});

delhiMetro.addEdge("Lal Qila", "Jama Masjid", static_cast<int>(d2), {"Violet Line"});
delhiMetro.addEdge("Jama Masjid", "Lal Qila", static_cast<int>(d2), {"Violet Line"});

delhiMetro.addEdge("Jama Masjid", "Delhi Gate", static_cast<int>(d3), {"Violet Line"});
delhiMetro.addEdge("Delhi Gate", "Jama Masjid", static_cast<int>(d3), {"Violet Line"});

delhiMetro.addEdge("Delhi Gate", "ITO", static_cast<int>(d4), {"Violet Line"});
delhiMetro.addEdge("ITO", "Delhi Gate", static_cast<int>(d4), {"Violet Line"});

delhiMetro.addEdge("ITO", "Mandi House", static_cast<int>(d5), {"Violet Line"});
delhiMetro.addEdge("Mandi House", "ITO", static_cast<int>(d5), {"Violet Line"});

delhiMetro.addEdge("Mandi House", "Janpath", static_cast<int>(d6), {"Violet Line"});
delhiMetro.addEdge("Janpath", "Mandi House", static_cast<int>(d6), {"Violet Line"});

delhiMetro.addEdge("Janpath", "Central Secretariat", static_cast<int>(d7), {"Violet Line"});
delhiMetro.addEdge("Central Secretariat", "Janpath", static_cast<int>(d7), {"Violet Line"});

delhiMetro.addEdge("Central Secretariat", "Khan Market", static_cast<int>(d8), {"Violet Line"});
delhiMetro.addEdge("Khan Market", "Central Secretariat", static_cast<int>(d8), {"Violet Line"});

delhiMetro.addEdge("Khan Market", "Jawaharlal Nehru Stadium", static_cast<int>(d9), {"Violet Line"});
delhiMetro.addEdge("Jawaharlal Nehru Stadium", "Khan Market", static_cast<int>(d9), {"Violet Line"});

delhiMetro.addEdge("Jawaharlal Nehru Stadium", "Jangpura", static_cast<int>(d10), {"Violet Line"});
delhiMetro.addEdge("Jangpura", "Jawaharlal Nehru Stadium", static_cast<int>(d10), {"Violet Line"});

delhiMetro.addEdge("Jangpura", "Lajpat Nagar", static_cast<int>(d11), {"Violet Line"});
delhiMetro.addEdge("Lajpat Nagar", "Jangpura", static_cast<int>(d11), {"Violet Line"});

delhiMetro.addEdge("Lajpat Nagar", "Moolchand", static_cast<int>(d12), {"Violet Line"});
delhiMetro.addEdge("Moolchand", "Lajpat Nagar", static_cast<int>(d12), {"Violet Line"});

delhiMetro.addEdge("Moolchand", "Kailash Colony", static_cast<int>(d13), {"Violet Line"});
delhiMetro.addEdge("Kailash Colony", "Moolchand", static_cast<int>(d13), {"Violet Line"});

delhiMetro.addEdge("Kailash Colony", "Nehru Place", static_cast<int>(d14), {"Violet Line"});
delhiMetro.addEdge("Nehru Place", "Kailash Colony", static_cast<int>(d14), {"Violet Line"});

delhiMetro.addEdge("Nehru Place", "Kalkaji Mandir", static_cast<int>(d15), {"Violet Line"});
delhiMetro.addEdge("Kalkaji Mandir", "Nehru Place", static_cast<int>(d15), {"Violet Line"});

delhiMetro.addEdge("Kalkaji Mandir", "Govind Puri", static_cast<int>(d16), {"Violet Line"});
delhiMetro.addEdge("Govind Puri", "Kalkaji Mandir", static_cast<int>(d16), {"Violet Line"});

delhiMetro.addEdge("Govind Puri", "Okhla NSIC", static_cast<int>(d17), {"Violet Line"});
delhiMetro.addEdge("Okhla NSIC", "Govind Puri", static_cast<int>(d17), {"Violet Line"});

delhiMetro.addEdge("Okhla NSIC", "Harkesh Nagar Okhla", static_cast<int>(d18), {"Violet Line"});
delhiMetro.addEdge("Harkesh Nagar Okhla", "Okhla NSIC", static_cast<int>(d18), {"Violet Line"});

delhiMetro.addEdge("Harkesh Nagar Okhla", "Jasola Apollo", static_cast<int>(d19), {"Violet Line"});
delhiMetro.addEdge("Jasola Apollo", "Harkesh Nagar Okhla", static_cast<int>(d19), {"Violet Line"});

delhiMetro.addEdge("Jasola Apollo", "Sarita Vihar", static_cast<int>(d20), {"Violet Line"});
delhiMetro.addEdge("Sarita Vihar", "Jasola Apollo", static_cast<int>(d20), {"Violet Line"});

delhiMetro.addEdge("Sarita Vihar", "Mohan Estate", static_cast<int>(d21), {"Violet Line"});
delhiMetro.addEdge("Mohan Estate", "Sarita Vihar", static_cast<int>(d21), {"Violet Line"});

delhiMetro.addEdge("Mohan Estate", "Tughlakabad", static_cast<int>(d22), {"Violet Line"});
delhiMetro.addEdge("Tughlakabad", "Mohan Estate", static_cast<int>(d22), {"Violet Line"});

delhiMetro.addEdge("Tughlakabad", "Badarpur Border", static_cast<int>(d23), {"Violet Line"});
delhiMetro.addEdge("Badarpur Border", "Tughlakabad", static_cast<int>(d23), {"Violet Line"});

delhiMetro.addEdge("Badarpur Border", "Sarai", static_cast<int>(d24), {"Violet Line"});
delhiMetro.addEdge("Sarai", "Badarpur Border", static_cast<int>(d24), {"Violet Line"});

delhiMetro.addEdge("Sarai", "NHPC Chowk", static_cast<int>(d25), {"Violet Line"});
delhiMetro.addEdge("NHPC Chowk", "Sarai", static_cast<int>(d25), {"Violet Line"});

delhiMetro.addEdge("NHPC Chowk", "Mewala Maharajpur", static_cast<int>(d26), {"Violet Line"});
delhiMetro.addEdge("Mewala Maharajpur", "NHPC Chowk", static_cast<int>(d26), {"Violet Line"});

delhiMetro.addEdge("Mewala Maharajpur", "Sector 28", static_cast<int>(d27), {"Violet Line"});
delhiMetro.addEdge("Sector 28", "Mewala Maharajpur", static_cast<int>(d27), {"Violet Line"});

delhiMetro.addEdge("Sector 28", "Badkhal Mor", static_cast<int>(d28), {"Violet Line"});
delhiMetro.addEdge("Badkhal Mor", "Sector 28", static_cast<int>(d28), {"Violet Line"});

delhiMetro.addEdge("Badkhal Mor", "Old Faridabad", static_cast<int>(d29), {"Violet Line"});
delhiMetro.addEdge("Old Faridabad", "Badkhal Mor", static_cast<int>(d29), {"Violet Line"});

delhiMetro.addEdge("Old Faridabad", "Neelam Chowk Ajronda", static_cast<int>(d30), {"Violet Line"});
delhiMetro.addEdge("Neelam Chowk Ajronda", "Old Faridabad", static_cast<int>(d30), {"Violet Line"});

delhiMetro.addEdge("Neelam Chowk Ajronda", "Bata Chowk", static_cast<int>(d31), {"Violet Line"});
delhiMetro.addEdge("Bata Chowk", "Neelam Chowk Ajronda", static_cast<int>(d31), {"Violet Line"});

delhiMetro.addEdge("Bata Chowk", "Escorts Mujesar", static_cast<int>(d32), {"Violet Line"});
delhiMetro.addEdge("Escorts Mujesar", "Bata Chowk", static_cast<int>(d32), {"Violet Line"});



delhiMetro.addEdge("Majlis Park", "Keshav Puram", static_cast<int>(e1), {"Pink Line"});
delhiMetro.addEdge("Keshav Puram", "Majlis Park", static_cast<int>(e1), {"Pink Line"});

delhiMetro.addEdge("Keshav Puram", "Kanhaiya Nagar", static_cast<int>(e2), {"Pink Line"});
delhiMetro.addEdge("Kanhaiya Nagar", "Keshav Puram", static_cast<int>(e2), {"Pink Line"});

delhiMetro.addEdge("Kanhaiya Nagar", "Shastri Nagar", static_cast<int>(e3), {"Pink Line"});
delhiMetro.addEdge("Shastri Nagar", "Kanhaiya Nagar", static_cast<int>(e3), {"Pink Line"});

delhiMetro.addEdge("Shastri Nagar", "Tis Hazari", static_cast<int>(e4), {"Pink Line"});
delhiMetro.addEdge("Tis Hazari", "Shastri Nagar", static_cast<int>(e4), {"Pink Line"});

delhiMetro.addEdge("Tis Hazari", "Karam Pura", static_cast<int>(e5), {"Pink Line"});
delhiMetro.addEdge("Karam Pura", "Tis Hazari", static_cast<int>(e5), {"Pink Line"});

delhiMetro.addEdge("Karam Pura", "Maya Puri", static_cast<int>(e6), {"Pink Line"});
delhiMetro.addEdge("Maya Puri", "Karam Pura", static_cast<int>(e6), {"Pink Line"});

delhiMetro.addEdge("Maya Puri", "Patel Nagar", static_cast<int>(e7), {"Pink Line"});
delhiMetro.addEdge("Patel Nagar", "Maya Puri", static_cast<int>(e7), {"Pink Line"});

delhiMetro.addEdge("Patel Nagar", "Kirti Nagar", static_cast<int>(e8), {"Pink Line"});
delhiMetro.addEdge("Kirti Nagar", "Patel Nagar", static_cast<int>(e8), {"Pink Line"});

delhiMetro.addEdge("Kirti Nagar", "Rajouri Garden", static_cast<int>(e9), {"Pink Line"});
delhiMetro.addEdge("Rajouri Garden", "Kirti Nagar", static_cast<int>(e9), {"Pink Line"});

delhiMetro.addEdge("Rajouri Garden", "Maharani Bagh", static_cast<int>(e10), {"Pink Line"});
delhiMetro.addEdge("Maharani Bagh", "Rajouri Garden", static_cast<int>(e10), {"Pink Line"});

delhiMetro.addEdge("Maharani Bagh", "Hazrat Nizamuddin", static_cast<int>(e11), {"Pink Line"});
delhiMetro.addEdge("Hazrat Nizamuddin", "Maharani Bagh", static_cast<int>(e11), {"Pink Line"});

delhiMetro.addEdge("Hazrat Nizamuddin", "Jangpura", static_cast<int>(e12), {"Pink Line"});
delhiMetro.addEdge("Jangpura", "Hazrat Nizamuddin", static_cast<int>(e12), {"Pink Line"});

delhiMetro.addEdge("Jangpura", "Sarai Kale Khan", static_cast<int>(e13), {"Pink Line"});
delhiMetro.addEdge("Sarai Kale Khan", "Jangpura", static_cast<int>(e13), {"Pink Line"});

delhiMetro.addEdge("Sarai Kale Khan", "Sarai Kale Khan Metro Station", static_cast<int>(e14), {"Pink Line"});
delhiMetro.addEdge("Sarai Kale Khan Metro Station", "Sarai Kale Khan", static_cast<int>(e14), {"Pink Line"});

delhiMetro.addEdge("Sarai Kale Khan Metro Station", "IIT Delhi", static_cast<int>(e15), {"Pink Line"});
delhiMetro.addEdge("IIT Delhi", "Sarai Kale Khan Metro Station", static_cast<int>(e15), {"Pink Line"});

delhiMetro.addEdge("IIT Delhi", "Hauz Khas", static_cast<int>(e16), {"Pink Line"});
delhiMetro.addEdge("Hauz Khas", "IIT Delhi", static_cast<int>(e16), {"Pink Line"});

delhiMetro.addEdge("Hauz Khas", "Green Park", static_cast<int>(e17), {"Pink Line"});
delhiMetro.addEdge("Green Park", "Hauz Khas", static_cast<int>(e17), {"Pink Line"});

delhiMetro.addEdge("Green Park", "Safdarjung", static_cast<int>(e18), {"Pink Line"});
delhiMetro.addEdge("Safdarjung", "Green Park", static_cast<int>(e18), {"Pink Line"});

delhiMetro.addEdge("Safdarjung", "Lajpat Nagar", static_cast<int>(e19), {"Pink Line"});
delhiMetro.addEdge("Lajpat Nagar", "Safdarjung", static_cast<int>(e19), {"Pink Line"});

delhiMetro.addEdge("Lajpat Nagar", "Moolchand", static_cast<int>(e20), {"Pink Line"});
delhiMetro.addEdge("Moolchand", "Lajpat Nagar", static_cast<int>(e20), {"Pink Line"});

delhiMetro.addEdge("Moolchand", "Hazarat Nizamuddin Metro Station", static_cast<int>(e21), {"Pink Line"});
delhiMetro.addEdge("Hazarat Nizamuddin Metro Station", "Moolchand", static_cast<int>(e21), {"Pink Line"});

delhiMetro.addEdge("Hazarat Nizamuddin Metro Station", "Sarai Kale Khan", static_cast<int>(e22), {"Pink Line"});
delhiMetro.addEdge("Sarai Kale Khan", "Hazarat Nizamuddin Metro Station", static_cast<int>(e22), {"Pink Line"});

delhiMetro.addEdge("Sarai Kale Khan", "Sarai Kale Khan Metro Station", static_cast<int>(e23), {"Pink Line"});
delhiMetro.addEdge("Sarai Kale Khan Metro Station", "Sarai Kale Khan", static_cast<int>(e23), {"Pink Line"});

delhiMetro.addEdge("Sarai Kale Khan Metro Station", "Jangpura", static_cast<int>(e24), {"Pink Line"});
delhiMetro.addEdge("Jangpura", "Sarai Kale Khan Metro Station", static_cast<int>(e24), {"Pink Line"});

delhiMetro.addEdge("Jangpura", "Hazrat Nizamuddin", static_cast<int>(e25), {"Pink Line"});
delhiMetro.addEdge("Hazrat Nizamuddin", "Jangpura", static_cast<int>(e25), {"Pink Line"});

delhiMetro.addEdge("Hazrat Nizamuddin", "Maharani Bagh", static_cast<int>(e26), {"Pink Line"});
delhiMetro.addEdge("Maharani Bagh", "Hazrat Nizamuddin", static_cast<int>(e26), {"Pink Line"});

delhiMetro.addEdge("Maharani Bagh", "Rajouri Garden", static_cast<int>(e27), {"Pink Line"});
delhiMetro.addEdge("Rajouri Garden", "Maharani Bagh", static_cast<int>(e27), {"Pink Line"});

delhiMetro.addEdge("Rajouri Garden", "Kirti Nagar", static_cast<int>(e28), {"Pink Line"});
delhiMetro.addEdge("Kirti Nagar", "Rajouri Garden", static_cast<int>(e28), {"Pink Line"});

delhiMetro.addEdge("Kirti Nagar", "Patel Nagar", static_cast<int>(e29), {"Pink Line"});
delhiMetro.addEdge("Patel Nagar", "Kirti Nagar", static_cast<int>(e29), {"Pink Line"});

delhiMetro.addEdge("Patel Nagar", "Maya Puri", static_cast<int>(e30), {"Pink Line"});
delhiMetro.addEdge("Maya Puri", "Patel Nagar", static_cast<int>(e30), {"Pink Line"});

delhiMetro.addEdge("Maya Puri", "Karam Pura", static_cast<int>(e31), {"Pink Line"});
delhiMetro.addEdge("Karam Pura", "Maya Puri", static_cast<int>(e31), {"Pink Line"});

delhiMetro.addEdge("Karam Pura", "Tis Hazari", static_cast<int>(e32), {"Pink Line"});
delhiMetro.addEdge("Tis Hazari", "Karam Pura", static_cast<int>(e32), {"Pink Line"});

delhiMetro.addEdge("Tis Hazari", "Shastri Nagar", static_cast<int>(e33), {"Pink Line"});
delhiMetro.addEdge("Shastri Nagar", "Tis Hazari", static_cast<int>(e33), {"Pink Line"});

delhiMetro.addEdge("Shastri Nagar", "Kanhaiya Nagar", static_cast<int>(e34), {"Pink Line"});
delhiMetro.addEdge("Kanhaiya Nagar", "Shastri Nagar", static_cast<int>(e34), {"Pink Line"});

delhiMetro.addEdge("Kanhaiya Nagar", "Keshav Puram", static_cast<int>(e35), {"Pink Line"});
delhiMetro.addEdge("Keshav Puram", "Kanhaiya Nagar", static_cast<int>(e35), {"Pink Line"});

delhiMetro.addEdge("Keshav Puram", "Majlis Park", static_cast<int>(e36), {"Pink Line"});
delhiMetro.addEdge("Majlis Park", "Keshav Puram", static_cast<int>(e36), {"Pink Line"});

delhiMetro.addEdge("Majlis Park", "Keshav Puram", static_cast<int>(e37), {"Pink Line"});
delhiMetro.addEdge("Keshav Puram", "Majlis Park", static_cast<int>(e37), {"Pink Line"});


delhiMetro.addEdge("Janakpuri West", "Dabri Mor", static_cast<int>(f1), {"Magenta Line"});
delhiMetro.addEdge("Dabri Mor", "Janakpuri West", static_cast<int>(f1), {"Magenta Line"});

delhiMetro.addEdge("Dabri Mor", "Dashrathpuri", static_cast<int>(f2), {"Magenta Line"});
delhiMetro.addEdge("Dashrathpuri", "Dabri Mor", static_cast<int>(f2), {"Magenta Line"});

delhiMetro.addEdge("Dashrathpuri", "Palam", static_cast<int>(f3), {"Magenta Line"});
delhiMetro.addEdge("Palam", "Dashrathpuri", static_cast<int>(f3), {"Magenta Line"});

delhiMetro.addEdge("Palam", "Sadar Bazar Cantonment", static_cast<int>(f4), {"Magenta Line"});
delhiMetro.addEdge("Sadar Bazar Cantonment", "Palam", static_cast<int>(f4), {"Magenta Line"});

delhiMetro.addEdge("Sadar Bazar Cantonment", "Terminal 1 IGI Airport", static_cast<int>(f5), {"Magenta Line"});
delhiMetro.addEdge("Terminal 1 IGI Airport", "Sadar Bazar Cantonment", static_cast<int>(f5), {"Magenta Line"});

delhiMetro.addEdge("Terminal 1 IGI Airport", "Shankar Vihar", static_cast<int>(f6), {"Magenta Line"});
delhiMetro.addEdge("Shankar Vihar", "Terminal 1 IGI Airport", static_cast<int>(f6), {"Magenta Line"});

delhiMetro.addEdge("Shankar Vihar", "Vasant Vihar", static_cast<int>(f7), {"Magenta Line"});
delhiMetro.addEdge("Vasant Vihar", "Shankar Vihar", static_cast<int>(f7), {"Magenta Line"});

delhiMetro.addEdge("Vasant Vihar", "Munirka", static_cast<int>(f8), {"Magenta Line"});
delhiMetro.addEdge("Munirka", "Vasant Vihar", static_cast<int>(f8), {"Magenta Line"});

delhiMetro.addEdge("Munirka", "RK Puram", static_cast<int>(f9), {"Magenta Line"});
delhiMetro.addEdge("RK Puram", "Munirka", static_cast<int>(f9), {"Magenta Line"});

delhiMetro.addEdge("RK Puram", "IIT", static_cast<int>(f10), {"Magenta Line"});
delhiMetro.addEdge("IIT", "RK Puram", static_cast<int>(f10), {"Magenta Line"});

delhiMetro.addEdge("IIT", "Hauz Khas", static_cast<int>(f11), {"Magenta Line"});
delhiMetro.addEdge("Hauz Khas", "IIT", static_cast<int>(f11), {"Magenta Line"});

delhiMetro.addEdge("Hauz Khas", "Panchsheel Park", static_cast<int>(f12), {"Magenta Line"});
delhiMetro.addEdge("Panchsheel Park", "Hauz Khas", static_cast<int>(f12), {"Magenta Line"});

delhiMetro.addEdge("Panchsheel Park", "Chirag Delhi", static_cast<int>(f13), {"Magenta Line"});
delhiMetro.addEdge("Chirag Delhi", "Panchsheel Park", static_cast<int>(f13), {"Magenta Line"});

delhiMetro.addEdge("Chirag Delhi", "Greater Kailash", static_cast<int>(f14), {"Magenta Line"});
delhiMetro.addEdge("Greater Kailash", "Chirag Delhi", static_cast<int>(f14), {"Magenta Line"});

delhiMetro.addEdge("Greater Kailash", "Nehru Enclave", static_cast<int>(f15), {"Magenta Line"});
delhiMetro.addEdge("Nehru Enclave", "Greater Kailash", static_cast<int>(f15), {"Magenta Line"});

delhiMetro.addEdge("Nehru Enclave", "Kalkaji Mandir", static_cast<int>(f16), {"Magenta Line"});
delhiMetro.addEdge("Kalkaji Mandir", "Nehru Enclave", static_cast<int>(f16), {"Magenta Line"});

delhiMetro.addEdge("Kalkaji Mandir", "Okhla NSIC", static_cast<int>(f17), {"Magenta Line"});
delhiMetro.addEdge("Okhla NSIC", "Kalkaji Mandir", static_cast<int>(f17), {"Magenta Line"});

delhiMetro.addEdge("Okhla NSIC", "Sukhdev Vihar", static_cast<int>(f18), {"Magenta Line"});
delhiMetro.addEdge("Sukhdev Vihar", "Okhla NSIC", static_cast<int>(f18), {"Magenta Line"});

delhiMetro.addEdge("Sukhdev Vihar", "Jamia Milia Islamiya", static_cast<int>(f19), {"Magenta Line"});
delhiMetro.addEdge("Jamia Milia Islamiya", "Sukhdev Vihar", static_cast<int>(f19), {"Magenta Line"});

delhiMetro.addEdge("Jamia Milia Islamiya", "Okhla Vihar", static_cast<int>(f20), {"Magenta Line"});
delhiMetro.addEdge("Okhla Vihar", "Jamia Milia Islamiya", static_cast<int>(f20), {"Magenta Line"});

delhiMetro.addEdge("Okhla Vihar", "Jasola Vihar Shaheen Bagh", static_cast<int>(f21), {"Magenta Line"});
delhiMetro.addEdge("Jasola Vihar Shaheen Bagh", "Okhla Vihar", static_cast<int>(f21), {"Magenta Line"});

delhiMetro.addEdge("Jasola Vihar Shaheen Bagh", "Kalindi Kunj", static_cast<int>(f22), {"Magenta Line"});
delhiMetro.addEdge("Kalindi Kunj", "Jasola Vihar Shaheen Bagh", static_cast<int>(f22), {"Magenta Line"});

delhiMetro.addEdge("Kalindi Kunj", "Okhla Bird Sanctuary", static_cast<int>(f23), {"Magenta Line"});
delhiMetro.addEdge("Okhla Bird Sanctuary", "Kalindi Kunj", static_cast<int>(f23), {"Magenta Line"});

delhiMetro.addEdge("Okhla Bird Sanctuary", "Botanical Garden", static_cast<int>(f24), {"Magenta Line"});
delhiMetro.addEdge("Botanical Garden", "Okhla Bird Sanctuary", static_cast<int>(f24), {"Magenta Line"});


delhiMetro.addEdge("New Delhi", "Shivaji Stadium", static_cast<int>(g1), {"Airport Express Line"});
delhiMetro.addEdge("Shivaji Stadium", "New Delhi", static_cast<int>(g1), {"Airport Express Line"});

delhiMetro.addEdge("Shivaji Stadium", "Dhaula Kuan", static_cast<int>(g2), {"Airport Express Line"});
delhiMetro.addEdge("Dhaula Kuan", "Shivaji Stadium", static_cast<int>(g2), {"Airport Express Line"});

delhiMetro.addEdge("Dhaula Kuan", "Delhi Aerocity", static_cast<int>(g3), {"Airport Express Line"});
delhiMetro.addEdge("Delhi Aerocity", "Dhaula Kuan", static_cast<int>(g3), {"Airport Express Line"});

delhiMetro.addEdge("Delhi Aerocity", "Airport T3", static_cast<int>(g4), {"Airport Express Line"});
delhiMetro.addEdge("Airport T3", "Delhi Aerocity", static_cast<int>(g4), {"Airport Express Line"});

delhiMetro.addEdge("Airport T3", "Dwarka Sector 21", static_cast<int>(g5), {"Airport Express Line"});
delhiMetro.addEdge("Dwarka Sector 21", "Airport T3", static_cast<int>(g5), {"Airport Express Line"});

delhiMetro.addEdge("Dwarka Sector 21", "Yashobhoomi Dwarka Sector 25", static_cast<int>(g6), {"Airport Express Line"});
delhiMetro.addEdge("Yashobhoomi Dwarka Sector 25", "Dwarka Sector 21", static_cast<int>(g6), {"Airport Express Line"});


// Adding edges between the stations on the Blue Line
delhiMetro.addEdge("Noida Electronic City", "Noida Sector 62", static_cast<int>(i1), {"Blue Line"});
delhiMetro.addEdge("Noida Sector 62", "Noida Electronic City", static_cast<int>(i1), {"Blue Line"});

delhiMetro.addEdge("Noida Sector 62", "Noida Sector 59", static_cast<int>(i2), {"Blue Line"});
delhiMetro.addEdge("Noida Sector 59", "Noida Sector 62", static_cast<int>(i2), {"Blue Line"});

delhiMetro.addEdge("Noida Sector 59", "Noida Sector 61", static_cast<int>(i3), {"Blue Line"});
delhiMetro.addEdge("Noida Sector 61", "Noida Sector 59", static_cast<int>(i3), {"Blue Line"});

delhiMetro.addEdge("Noida Sector 61", "Noida Sector 52", static_cast<int>(i4), {"Blue Line"});
delhiMetro.addEdge("Noida Sector 52", "Noida Sector 61", static_cast<int>(i4), {"Blue Line"});

delhiMetro.addEdge("Noida Sector 52", "Noida Sector 34", static_cast<int>(i5), {"Blue Line"});
delhiMetro.addEdge("Noida Sector 34", "Noida Sector 52", static_cast<int>(i5), {"Blue Line"});

delhiMetro.addEdge("Noida Sector 34", "Noida City Centre", static_cast<int>(i6), {"Blue Line"});
delhiMetro.addEdge("Noida City Centre", "Noida Sector 34", static_cast<int>(i6), {"Blue Line"});

delhiMetro.addEdge("Noida City Centre", "Golf Course", static_cast<int>(i7), {"Blue Line"});
delhiMetro.addEdge("Golf Course", "Noida City Centre", static_cast<int>(i7), {"Blue Line"});

delhiMetro.addEdge("Golf Course", "Botanical Garden", static_cast<int>(i8), {"Blue Line"});
delhiMetro.addEdge("Botanical Garden", "Golf Course", static_cast<int>(i8), {"Blue Line"});

delhiMetro.addEdge("Botanical Garden", "Noida Sector 18", static_cast<int>(i9), {"Blue Line"});
delhiMetro.addEdge("Noida Sector 18", "Botanical Garden", static_cast<int>(i9), {"Blue Line"});

delhiMetro.addEdge("Noida Sector 18", "Noida Sector 16", static_cast<int>(i10), {"Blue Line"});
delhiMetro.addEdge("Noida Sector 16", "Noida Sector 18", static_cast<int>(i10), {"Blue Line"});

delhiMetro.addEdge("Noida Sector 16", "Noida Sector 15", static_cast<int>(i11), {"Blue Line"});
delhiMetro.addEdge("Noida Sector 15", "Noida Sector 16", static_cast<int>(i11), {"Blue Line"});

delhiMetro.addEdge("Noida Sector 15", "New Ashok Nagar", static_cast<int>(i12), {"Blue Line"});
delhiMetro.addEdge("New Ashok Nagar", "Noida Sector 15", static_cast<int>(i12), {"Blue Line"});

delhiMetro.addEdge("New Ashok Nagar", "Mayur Vihar Extension", static_cast<int>(i13), {"Blue Line"});
delhiMetro.addEdge("Mayur Vihar Extension", "New Ashok Nagar", static_cast<int>(i13), {"Blue Line"});

delhiMetro.addEdge("Mayur Vihar Extension", "Mayur Vihar I", static_cast<int>(i14), {"Blue Line"});
delhiMetro.addEdge("Mayur Vihar I", "Mayur Vihar Extension", static_cast<int>(i14), {"Blue Line"});

delhiMetro.addEdge("Mayur Vihar I", "Akshardham", static_cast<int>(i15), {"Blue Line"});
delhiMetro.addEdge("Akshardham", "Mayur Vihar I", static_cast<int>(i15), {"Blue Line"});

delhiMetro.addEdge("Akshardham", "Yamuna Bank", static_cast<int>(i16), {"Blue Line"});
delhiMetro.addEdge("Yamuna Bank", "Akshardham", static_cast<int>(i16), {"Blue Line"});

delhiMetro.addEdge("Yamuna Bank", "Indraprastha", static_cast<int>(i17), {"Blue Line"});
delhiMetro.addEdge("Indraprastha", "Yamuna Bank", static_cast<int>(i17), {"Blue Line"});

delhiMetro.addEdge("Indraprastha", "Supreme Court (Pragati Maidan)", static_cast<int>(i18), {"Blue Line"});
delhiMetro.addEdge("Supreme Court (Pragati Maidan)", "Indraprastha", static_cast<int>(i18), {"Blue Line"});

delhiMetro.addEdge("Supreme Court (Pragati Maidan)", "Mandi House", static_cast<int>(i19), {"Blue Line"});
delhiMetro.addEdge("Mandi House", "Supreme Court (Pragati Maidan)", static_cast<int>(i19), {"Blue Line"});

delhiMetro.addEdge("Mandi House", "Barakhambha Road", static_cast<int>(i20), {"Blue Line"});
delhiMetro.addEdge("Barakhambha Road", "Mandi House", static_cast<int>(i20), {"Blue Line"});

delhiMetro.addEdge("Barakhambha Road", "Rajiv Chowk", static_cast<int>(i21), {"Blue Line"});
delhiMetro.addEdge("Rajiv Chowk", "Barakhambha Road", static_cast<int>(i21), {"Blue Line"});

delhiMetro.addEdge("Rajiv Chowk", "RK Ashram Marg", static_cast<int>(i22), {"Blue Line"});
delhiMetro.addEdge("RK Ashram Marg", "Rajiv Chowk", static_cast<int>(i22), {"Blue Line"});

delhiMetro.addEdge("RK Ashram Marg", "Jhandewalan", static_cast<int>(i23), {"Blue Line"});
delhiMetro.addEdge("Jhandewalan", "RK Ashram Marg", static_cast<int>(i23), {"Blue Line"});

delhiMetro.addEdge("Jhandewalan", "Karol Bagh", static_cast<int>(i24), {"Blue Line"});
delhiMetro.addEdge("Karol Bagh", "Jhandewalan", static_cast<int>(i24), {"Blue Line"});

delhiMetro.addEdge("Karol Bagh", "Rajendra Place", static_cast<int>(i25), {"Blue Line"});
delhiMetro.addEdge("Rajendra Place", "Karol Bagh", static_cast<int>(i25), {"Blue Line"});

delhiMetro.addEdge("Rajendra Place", "Patel Nagar", static_cast<int>(i26), {"Blue Line"});
delhiMetro.addEdge("Patel Nagar", "Rajendra Place", static_cast<int>(i26), {"Blue Line"});

delhiMetro.addEdge("Patel Nagar", "Shadipur", static_cast<int>(i27), {"Blue Line"});
delhiMetro.addEdge("Shadipur", "Patel Nagar", static_cast<int>(i27), {"Blue Line"});

delhiMetro.addEdge("Shadipur", "Kirti Nagar", static_cast<int>(i28), {"Blue Line"});
delhiMetro.addEdge("Kirti Nagar", "Shadipur", static_cast<int>(i28), {"Blue Line"});

delhiMetro.addEdge("Kirti Nagar", "Moti Nagar", static_cast<int>(i29), {"Blue Line"});
delhiMetro.addEdge("Moti Nagar", "Kirti Nagar", static_cast<int>(i29), {"Blue Line"});

delhiMetro.addEdge("Moti Nagar", "Ramesh Nagar", static_cast<int>(i30), {"Blue Line"});
delhiMetro.addEdge("Ramesh Nagar", "Moti Nagar", static_cast<int>(i30), {"Blue Line"});

delhiMetro.addEdge("Ramesh Nagar", "Rajouri Garden", static_cast<int>(i31), {"Blue Line"});
delhiMetro.addEdge("Rajouri Garden", "Ramesh Nagar", static_cast<int>(i31), {"Blue Line"});

delhiMetro.addEdge("Rajouri Garden", "Tagore Garden", static_cast<int>(i32), {"Blue Line"});
delhiMetro.addEdge("Tagore Garden", "Rajouri Garden", static_cast<int>(i32), {"Blue Line"});

delhiMetro.addEdge("Tagore Garden", "Subhash Nagar", static_cast<int>(i33), {"Blue Line"});
delhiMetro.addEdge("Subhash Nagar", "Tagore Garden", static_cast<int>(i33), {"Blue Line"});

delhiMetro.addEdge("Subhash Nagar", "Tilak Nagar", static_cast<int>(i34), {"Blue Line"});
delhiMetro.addEdge("Tilak Nagar", "Subhash Nagar", static_cast<int>(i34), {"Blue Line"});

delhiMetro.addEdge("Tilak Nagar", "Janakpuri East", static_cast<int>(i35), {"Blue Line"});
delhiMetro.addEdge("Janakpuri East", "Tilak Nagar", static_cast<int>(i35), {"Blue Line"});

delhiMetro.addEdge("Janakpuri East", "Janakpuri West", static_cast<int>(i36), {"Blue Line"});
delhiMetro.addEdge("Janakpuri West", "Janakpuri East", static_cast<int>(i36), {"Blue Line"});

delhiMetro.addEdge("Janakpuri West", "Uttam Nagar East", static_cast<int>(i37), {"Blue Line"});
delhiMetro.addEdge("Uttam Nagar East", "Janakpuri West", static_cast<int>(i37), {"Blue Line"});

delhiMetro.addEdge("Uttam Nagar East", "Uttam Nagar West", static_cast<int>(i38), {"Blue Line"});
delhiMetro.addEdge("Uttam Nagar West", "Uttam Nagar East", static_cast<int>(i38), {"Blue Line"});

delhiMetro.addEdge("Uttam Nagar West", "Nawada", static_cast<int>(i39), {"Blue Line"});
delhiMetro.addEdge("Nawada", "Uttam Nagar West", static_cast<int>(i39), {"Blue Line"});

delhiMetro.addEdge("Nawada", "Dwarka Mor", static_cast<int>(i40), {"Blue Line"});
delhiMetro.addEdge("Dwarka Mor", "Nawada", static_cast<int>(i40), {"Blue Line"});

delhiMetro.addEdge("Dwarka Mor", "Dwarka", static_cast<int>(i41), {"Blue Line"});
delhiMetro.addEdge("Dwarka", "Dwarka Mor", static_cast<int>(i41), {"Blue Line"});

delhiMetro.addEdge("Dwarka", "Dwarka Sector 14", static_cast<int>(i42), {"Blue Line"});
delhiMetro.addEdge("Dwarka Sector 14", "Dwarka", static_cast<int>(i42), {"Blue Line"});

delhiMetro.addEdge("Dwarka Sector 14", "Dwarka Sector 13", static_cast<int>(i43), {"Blue Line"});
delhiMetro.addEdge("Dwarka Sector 13", "Dwarka Sector 14", static_cast<int>(i43), {"Blue Line"});

delhiMetro.addEdge("Dwarka Sector 13", "Dwarka Sector 12", static_cast<int>(i44), {"Blue Line"});
delhiMetro.addEdge("Dwarka Sector 12", "Dwarka Sector 13", static_cast<int>(i44), {"Blue Line"});

delhiMetro.addEdge("Dwarka Sector 12", "Dwarka Sector 11", static_cast<int>(i45), {"Blue Line"});
delhiMetro.addEdge("Dwarka Sector 11", "Dwarka Sector 12", static_cast<int>(i45), {"Blue Line"});

delhiMetro.addEdge("Dwarka Sector 11", "Dwarka Sector 10", static_cast<int>(i46), {"Blue Line"});
delhiMetro.addEdge("Dwarka Sector 10", "Dwarka Sector 11", static_cast<int>(i46), {"Blue Line"});

delhiMetro.addEdge("Dwarka Sector 10", "Dwarka Sector 9", static_cast<int>(i47), {"Blue Line"});
delhiMetro.addEdge("Dwarka Sector 9", "Dwarka Sector 10", static_cast<int>(i47), {"Blue Line"});

delhiMetro.addEdge("Dwarka Sector 9", "Dwarka Sector 8", static_cast<int>(i48), {"Blue Line"});
delhiMetro.addEdge("Dwarka Sector 8", "Dwarka Sector 9", static_cast<int>(i48), {"Blue Line"});

delhiMetro.addEdge("Dwarka Sector 8", "Dwarka Sector 21", static_cast<int>(i49), {"Blue Line"});
delhiMetro.addEdge("Dwarka Sector 21", "Dwarka Sector 8", static_cast<int>(i49), {"Blue Line"});

    // Example usage: find shortest path and calculate fare
   string source, destination;
    cout << "Enter the source station: ";
    getline(cin, source);
    cout << "Enter the destination station: ";
    getline(cin, destination);

    auto [path, totalDistance] = delhiMetro.dijkstra(source, destination);

    // Output shortest path with line changes
    cout << "Shortest path from " << source << " to " << destination << ":\n";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        // Print metro line information and where to change lines (if applicable)
        if (i < path.size() - 1) {
            auto currentLines = delhiMetro.getMetroLines(path[i]);
            auto nextLines = delhiMetro.getMetroLines(path[i + 1]);
            auto commonLines = delhiMetro.getCommonLines(path[i], path[i + 1]);

            if (!commonLines.empty()) {
                cout << " (" << commonLines[0] << ")";
            } else {
                // Identify the line change and to which line
                for (const auto& line : currentLines) {
                    if (find(nextLines.begin(), nextLines.end(), line) == nextLines.end()) {
                        cout << " [Change to " << nextLines[0] << " Line]";
                        break;
                    }
                }
            }
            cout << " -> ";
        } else {
            // Print the line of the last station
            cout << " (" << delhiMetro.getMetroLines(path[i])[0] << ")";
        }
    }
    cout << "\nTotal distance: " << totalDistance << " km\n";

    // Calculate fare
    double fare = delhiMetro.calculateFare(totalDistance);
    cout << "Fare: Rs. " << fare << "\n";

    return 0;
}