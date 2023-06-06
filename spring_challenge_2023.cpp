#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

bool startGame = false, middleGame = true, endGame = false;

float current_nb_Eggs = 0, current_nb_Crystal = 0;
float initial_nb_Crystal = 0, initial_nb_Eggs = 0;
float current_nb_my_ants = 0, current_nb_opp_ants = 0;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct dir {
    int source;
    int target;
    int distance;
};

class Cell {
public:
    int index;
    int cell_type;
    int resources;
    vector<int> neighbors;
    int my_ants;
    int opp_ants;
    int distance;

    Cell(int index, int cell_type, int resources, std::vector<int> neighbors, int my_ants, int opp_ants) {
        this->index = index;
        this->cell_type = cell_type;
        this->resources = resources;
        this->neighbors = neighbors;
        this->my_ants = my_ants;
        this->opp_ants = opp_ants;
    }
    bool operator < (const Cell& elem) const
    {
        return (distance < elem.distance);
    }
};


int calculateDistance(const std::vector<Cell>& cells, int sourceIndex, int targetIndex) {
    std::queue<int> queue;
    std::vector<bool> visited(cells.size(), false);
    std::vector<int> distance(cells.size(), 0);

    queue.push(sourceIndex);
    visited[sourceIndex] = true;

    while (!queue.empty()) {
        int currentIndex = queue.front();
        queue.pop();

        if (currentIndex == targetIndex) {
            return distance[currentIndex];
        }

        for (int neighborIndex : cells[currentIndex].neighbors) {
            if (!visited[neighborIndex]) {
                queue.push(neighborIndex);
                visited[neighborIndex] = true;
                distance[neighborIndex] = distance[currentIndex] + 1;
            }
        }
    }

    return -1;  // No path found
}

int main()
{

    std::vector<Cell> cells;
    int number_of_cells; // amount of hexagonal cells in this map
    cin >> number_of_cells; cin.ignore();
    for (int i = 0; i < number_of_cells; i++) {
        int type; // 0 for empty, 1 for eggs, 2 for crystal
        int initial_resources; // the initial amount of eggs/crystals on this cell
        int neigh_0; // the index of the neighbouring cell for each direction
        int neigh_1;
        int neigh_2;
        int neigh_3;
        int neigh_4;
        int neigh_5;
        cin >> type >> initial_resources >> neigh_0 >> neigh_1 >> neigh_2 >> neigh_3 >> neigh_4 >> neigh_5; cin.ignore();
    	
		std::vector<int> neighbors = {neigh_0, neigh_1, neigh_2, neigh_3, neigh_4, neigh_5};
        neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [](int id) { return id == -1; }), neighbors.end());
		Cell cell(i, type, initial_resources, neighbors, 0, 0);
        cells.push_back(cell);
	}
    int number_of_bases;
    cin >> number_of_bases; cin.ignore();

	std::vector<int> my_bases;
    for (int i = 0; i < number_of_bases; i++) {
        int my_base_index;
        cin >> my_base_index; cin.ignore();
		my_bases.push_back(my_base_index);
    }
	std::vector<int> opp_bases;
    for (int i = 0; i < number_of_bases; i++) {
        int opp_base_index;
        cin >> opp_base_index; cin.ignore();
		opp_bases.push_back(opp_base_index);
    }

    // game loop
    bool flag = true;
    while (1) {
        int myScore, oppScore;
        current_nb_Eggs = 0, current_nb_Crystal = 0, current_nb_my_ants = 0, current_nb_opp_ants = 0;
        
        cin >> myScore >> oppScore; cin.ignore();
        vector<int> rcs_index;
        vector<struct dir> bounty;
        for (int i = 0; i < number_of_cells; i++) {
            int resources; // the current amount of eggs/crystals on this cell
            int my_ants; // the amount of your ants on this cell
            int opp_ants; // the amount of opponent ants on this cell
            cin >> resources >> my_ants >> opp_ants; cin.ignore();

			cells[i].resources = resources;
            cells[i].my_ants = my_ants;
            cells[i].opp_ants = opp_ants;
            current_nb_my_ants += my_ants;
            current_nb_opp_ants += opp_ants;
            if (cells[i].resources > 0){
                if (cells[i].cell_type == 1)
                {
                    current_nb_Eggs += cells[i].resources;
                    if (!endGame)
                        rcs_index.push_back(cells[i].index);
                }
                if (cells[i].cell_type == 2)
                {
                    current_nb_Crystal += cells[i].resources;
                    if (!startGame)
                        rcs_index.push_back(cells[i].index);
                }
            }
        }

        if (flag)   // only once
        {
            initial_nb_Crystal = current_nb_Crystal;
            initial_nb_Eggs = current_nb_Eggs;
        }
        flag = false;
        cerr << "Important Data:\n";
        cerr << ">>> current_nb_my_ants: " << current_nb_my_ants << endl;
        cerr << ">>> current_nb_opp_ants: " << current_nb_opp_ants << endl;
        cerr << "\n\nSTATISTICS:\n";
        cerr << "myScore: " << myScore << " | oppScore: " << oppScore << endl;
        cerr << "Eggs Storage: " << (current_nb_Eggs / initial_nb_Eggs) * 100 << " %" << endl;
        cerr << "Crystals Storage: " << (current_nb_Crystal / initial_nb_Crystal) * 100 << " %" << endl;
        
        if (current_nb_Eggs / initial_nb_Eggs >= 0.7)
        {
            cerr << ">>> START GAME\n";
            startGame = true; middleGame = false; endGame = false;
        }
        else if (current_nb_Crystal / initial_nb_Crystal <= 0.2)
        {
            cerr << ">>> END GAME\n";
            startGame = false; middleGame = false; endGame = true;
        }
        else
        {
            cerr << ">>> MIDDLE GAME\n";
            startGame = false; middleGame = true; endGame = false;
        }


        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        struct dir elem;
        for (int j = 0; j < my_bases.size(); j++)
        {
            elem.source = my_bases[j];
            for (int i = 0; i < rcs_index.size(); i++) {
                elem.target = rcs_index[i];
                elem.distance = calculateDistance(cells, elem.source, elem.target);
                bounty.push_back(elem);
            }
        }
        std::sort(bounty.begin(), bounty.end(), [&](struct dir elem1, struct dir elem2) {
            int distance1 = elem1.distance;
            int distance2 = elem2.distance;
            return distance1 < distance2;
        });

        cerr << "---------------\n";
        int strength = 1;
        int source, target, distance, tmp_distance;
        cout << "MESSAGE RAJOL;";
        for (int i = 0; i < bounty.size(); i++) {
            source = bounty[i].source;
            target = bounty[i].target;
            distance = bounty[i].distance;
            if (cells[target].cell_type == 1 && endGame)
                continue;
            else if (cells[target].cell_type == 2 && startGame)
                continue;
            for (int j = 0; j < i; j++) {
                tmp_distance = calculateDistance(cells, bounty[j].target, bounty[i].target);
                if (distance > tmp_distance)
                {
                    source = bounty[j].target;
                    distance = tmp_distance;
                }
            }
            if ((distance + 1) * strength <= current_nb_my_ants) {
                cout << "LINE " << source << " " << target << " "  << strength << ";";
                current_nb_my_ants -= (distance + 1);
            }

        cout << "MESSAGE RAJOL;";
        }
        cout << endl;
    }
}