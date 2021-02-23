#include <fstream>
#include <deque>
#include <vector>
#include <algorithm>

struct Berth {
    Berth(): empty(true) {}
    bool empty;
    std::string shipName;
};

bool is_empty (std::ifstream &file) {
    return file.peek() == std::ifstream::traits_type::eof();
}

void trim (std::string &str) {
    str.erase(0, str.find_first_not_of(" \n\r\t"));
    str.erase(str.find_last_not_of(" \n\r\t") + 1);
}

int main() {
    std::ifstream fin("SHIPS.IN");
    std::ofstream fout("SHIPS.OUT");

    if (!fin.is_open()) {
        fout << "Error: cannot open the input file";
        return 0;
    }
    if (is_empty(fin)) {
        fout << "Error: the input file is empty";
        return 0;
    }

    int nBerths = 0;
    fin >> nBerths;
    std::vector<Berth> berths(nBerths);
    std::deque<std::string> raid;

    int state = 0;
    while (fin >> state) {
        if (state == 1) {
            std::string shipName;
            std::getline(fin, shipName);
            trim(shipName);
            auto freeBerth = std::find_if(berths.begin(), berths.end(), [](const Berth &berth) {
                return berth.empty;
            });
            if (freeBerth != berths.end()) {
                freeBerth->empty = false;
                freeBerth->shipName = shipName;
            }
            else {
                raid.push_back(shipName);
            }
        } else if (state == 2) {
            int vacantBerth = 0;
            fin >> vacantBerth;
            if (!raid.empty()) {
                berths[vacantBerth - 1].shipName = raid[0];
                raid.pop_front();
            }
            else {
                berths[vacantBerth - 1].empty = true;
                berths[vacantBerth - 1].shipName.clear();
            }
        } else if (state == 3) {
            fout << "Ships in the raid: " << raid.size() << '\n';
            if (!raid.empty()) {
                fout << "Current queue:\n";
                for (int i = 0; i < raid.size(); ++i) {
                    fout << i + 1 << ") " << raid[i] << '\n';
                }
            }
            fout << '\n';
        } else {
            for (int i = 0; i < berths.size(); ++i) {
                fout << "Berth #" << i + 1 << ":\n";
                fout << "Status: ";
                if (berths[i].empty) {
                    fout << "Empty\n\n";
                }
                else {
                    fout << "Busy\n" << "Ship name: " << berths[i].shipName << "\n\n";
                }
            }
        }
    }

    fin.close();
    fout.close();

    return 0;
}
