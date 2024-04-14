#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

using namespace std;

int calculateTotalSatisfaction(const vector<vector<int>>& clients, const vector<vector<int>>& satisfaction) {
    int total_satisfaction = 0;
    for (size_t i = 0; i < clients.size(); ++i) {
        for (int event_num : clients[i]) {
            total_satisfaction += satisfaction[i][event_num];
        }
    }
    return total_satisfaction;
}

bool hasOverlap(const vector<int>& event1, const vector<int>& event2) {
    return (event1[0] < event2[1] && event1[1] > event2[0]);
}

vector<vector<int>> generateNeighbor(const vector<vector<int>>& current_schedule, const vector<vector<int>>& events, mt19937& gen) {
    vector<vector<int>> neighbor_schedule(current_schedule);

    // Create a matrix to store overlap information
    vector<vector<int>> overlap(events.size(), vector<int>(events.size(), 0));

    // Determine overlap between events
    for (size_t i = 0; i < events.size(); ++i) {
        for (size_t j = i; j < events.size(); ++j) {
            int a = events[i][0], b = events[i][1];
            int c = events[j][0], d = events[j][1];
            // Check if events [a, b] and [c, d] overlap
            bool not_overlap = (b < c) || (d < a);
            if (!not_overlap) {
                overlap[i][j] = 1;
                overlap[j][i] = 1;
            }
        }
    }

    // Iterate through clients and assign events without overlap
    for (size_t i = 0; i < neighbor_schedule.size(); ++i) {
        shuffle(neighbor_schedule[i].begin(), neighbor_schedule[i].end(), gen); // Shuffle event order for each client
        vector<int> valid_events;
        for (int event_index : neighbor_schedule[i]) {
            // Check if the event overlaps with any event already attended by the client
            bool can_assign = true;
            for (int assigned_event_num : valid_events) {
                if (overlap[event_index][assigned_event_num]) {
                    can_assign = false;
                    break;
                }
            }
            if (can_assign) {
                valid_events.push_back(event_index); // Add the event to valid events for this client
            }
        }
        // Update the client's schedule with valid events
        neighbor_schedule[i] = valid_events;
    }

    return neighbor_schedule;
}


vector<vector<int>> simulatedAnnealing(const vector<vector<int>>& satisfaction, const vector<vector<int>>& events, int initial_temperature, double cooling_rate, int max_iterations) {
    random_device rd;
    mt19937 gen(rd());

    vector<vector<int>> current_schedule(satisfaction.size(), vector<int>());
    vector<vector<int>> best_schedule;
    int best_satisfaction = 0;

    // Initialize current schedule randomly
    for (auto& client : current_schedule) {
        vector<int> event_order(events.size());
        iota(event_order.begin(), event_order.end(), 0);
        random_shuffle(event_order.begin(), event_order.end(), [&gen](int i) { return gen() % i; });
        for (int event_index : event_order) {
            if (events[event_index][2] > client.size()) {
                bool can_assign = true;
                for (int assigned_event_num : client) {
                    if (hasOverlap(events[assigned_event_num], events[event_index])) {
                        can_assign = false;
                        break;
                    }
                }
                if (can_assign) {
                    client.push_back(event_index);
                }
            }
        }
    }

    int current_satisfaction = calculateTotalSatisfaction(current_schedule, satisfaction);
    best_satisfaction = current_satisfaction;
    best_schedule = current_schedule;

    int current_temperature = initial_temperature;
    int iteration = 0;

    while (current_temperature > 1 && iteration < max_iterations) {
        vector<vector<int>> neighbor_schedule = generateNeighbor(current_schedule, events, gen);
        int neighbor_satisfaction = calculateTotalSatisfaction(neighbor_schedule, satisfaction);

        int delta_satisfaction = neighbor_satisfaction - current_satisfaction;

        if (delta_satisfaction > 0 || exp(delta_satisfaction / current_temperature) > (double)rand() / RAND_MAX) {
            current_schedule = neighbor_schedule;
            current_satisfaction = neighbor_satisfaction;
            if (current_satisfaction > best_satisfaction) {
                best_satisfaction = current_satisfaction;
                best_schedule = current_schedule;
            }
        }

        current_temperature *= cooling_rate;
        ++iteration;
    }

    return best_schedule;
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<vector<int>> satisfaction(N, vector<int>(M));
    vector<vector<int>> events(M, vector<int>(3)); // Each event has start_time, end_time, and capacity

    // Input satisfaction levels of clients
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> satisfaction[i][j];
        }
    }

    // Input event details
    for (int i = 0; i < M; ++i) {
        cin >> events[i][0] >> events[i][1] >> events[i][2]; // Start time, end time, capacity
    }

    // Sort events based on start time
    sort(events.begin(), events.end(), [](const vector<int>& e1, const vector<int>& e2) {
        return e1[0] < e2[0];
    });

    int initial_temperature = 10000;
    double cooling_rate = 0.999;
    int max_iterations = 10000;

    vector<vector<int>> best_schedule = simulatedAnnealing(satisfaction, events, initial_temperature, cooling_rate, max_iterations);

    for (size_t i = 0; i < best_schedule.size(); ++i) {
        cout << best_schedule[i].size() << " ";
        for (int event_num : best_schedule[i]) {
            cout << event_num + 1 << " ";
        }
        cout << endl;
    }

    return 0;
}
