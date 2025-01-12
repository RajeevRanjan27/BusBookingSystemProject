// main.cpp

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>      // for std::setw
#include <algorithm>    // for std::find_if
#include <limits>       // for clearing std::cin

/**
 * @file main.cpp
 * @brief Console-based Bus Booking System
 *
 * This application allows users to manage bus reservations, including installing new buses,
 * reserving seats, canceling reservations, viewing bus details, and searching buses by route.
 */

/**
 * @brief ANSI escape codes to clear the terminal screen.
 *
 * This function clears the terminal screen using ANSI escape codes, which are supported by
 * many modern terminals.
 */
void clearScreen() {
    std::cout << "\033[2J\033[H";
}

/**
 * @brief Utility function to draw a horizontal line of repeated characters.
 *
 * @param ch The character to repeat.
 * @param length The number of times to repeat the character. Default is 80.
 */
void printLine(char ch, int length = 80) {
    for(int i = 0; i < length; ++i) std::cout << ch;
    std::cout << std::endl;
}

/**
 * @struct Seat
 * @brief Represents a single seat in the bus.
 *
 * Each seat has a passenger name (or "Empty" if unreserved) and a fare price.
 */
struct Seat {
    std::string passengerName; /**< Name of the passenger. "Empty" if seat is vacant. */
    double fare;               /**< Fare price for the seat. */

    /**
     * @brief Default constructor initializes seat as empty with zero fare.
     */
    Seat() : passengerName("Empty"), fare(0.0) {}
};

/**
 * @class Bus
 * @brief Represents a bus with its details and seat information.
 */
class Bus {
private:
    std::string busNumber;          /**< Unique identifier for the bus. */
    std::string driverName;         /**< Name of the bus driver. */
    std::string arrivalTime;        /**< Arrival time of the bus. */
    std::string departureTime;      /**< Departure time of the bus. */
    std::string from;               /**< Origin location. */
    std::string to;                 /**< Destination location. */

    /**
     * @brief 2D vector representing seats arranged in rows and columns.
     *
     * 8 rows with 4 seats each (total 32 seats).
     */
    std::vector<std::vector<Seat>> seats;

public:
    /**
     * @brief Default constructor initializes seats with default fares.
     */
    Bus();

    /**
     * @brief Install a new bus by reading user input.
     *
     * Allows the user to input bus details. If the user enters "0" or leaves a prompt empty,
     * the installation is canceled.
     */
    void install();

    /**
     * @brief Reserve a seat on the bus.
     *
     * Allows the user to reserve a seat by specifying the bus number and seat number.
     * Ensures that the seat is available before booking.
     */
    void allotment();

    /**
     * @brief Cancel a seat reservation.
     *
     * Allows the user to cancel a previously reserved seat by specifying the bus number
     * and seat number.
     */
    void cancelSeat();

    /**
     * @brief Display detailed information about the bus, including seat map.
     */
    void show() const;

    /**
     * @brief Display a concise summary of the bus information.
     */
    void printBasicInfo() const;

    /**
     * @brief Check if the bus matches the given route.
     *
     * @param origin Origin location to match.
     * @param dest Destination location to match.
     * @return true If the bus matches the route.
     * @return false Otherwise.
     */
    bool matchesRoute(const std::string& origin, const std::string& dest) const;

    /**
     * @brief Get the bus number.
     *
     * @return std::string The bus number.
     */
    std::string getBusNumber() const;

    /**
     * @brief Friend function to allow access to private members for seat reservation and cancellation.
     *
     * @param seatNumber The seat number to access.
     * @return Seat& Reference to the specified seat.
     */
    Seat& getSeat(int seatNumber);
};

/**
 * @brief Global container storing all buses.
 */
std::vector<Bus> busList;

/****************************************
 *      Bus Class Method Definitions    *
 ****************************************/

Bus::Bus()
    : seats(8, std::vector<Seat>(4))  // 8 rows, each row has 4 Seat objects
{
    // Initialize default seat fares (e.g., 300.0 for all seats)
    for(auto &row : seats) {
        for(auto &seat : row) {
            seat.fare = 300.0;
        }
    }
}

void Bus::install() {
    std::cout << "Enter bus number (or 0 to cancel): ";
    std::getline(std::cin, busNumber);
    if(busNumber == "0" || busNumber.empty()) {
        std::cout << "Installation cancelled.\n";
        busNumber.clear();
        return;
    }

    std::cout << "Enter driver's name (or 0 to cancel): ";
    std::getline(std::cin, driverName);
    if(driverName == "0" || driverName.empty()) {
        std::cout << "Installation cancelled.\n";
        busNumber.clear();
        return;
    }

    std::cout << "Enter arrival time (or 0 to cancel): ";
    std::getline(std::cin, arrivalTime);
    if(arrivalTime == "0" || arrivalTime.empty()) {
        std::cout << "Installation cancelled.\n";
        busNumber.clear();
        driverName.clear();
        return;
    }

    std::cout << "Enter departure time (or 0 to cancel): ";
    std::getline(std::cin, departureTime);
    if(departureTime == "0" || departureTime.empty()) {
        std::cout << "Installation cancelled.\n";
        busNumber.clear();
        driverName.clear();
        arrivalTime.clear();
        return;
    }

    std::cout << "Enter origin (From) (or 0 to cancel): ";
    std::getline(std::cin, from);
    if(from == "0" || from.empty()) {
        std::cout << "Installation cancelled.\n";
        busNumber.clear();
        driverName.clear();
        arrivalTime.clear();
        departureTime.clear();
        return;
    }

    std::cout << "Enter destination (To) (or 0 to cancel): ";
    std::getline(std::cin, to);
    if(to == "0" || to.empty()) {
        std::cout << "Installation cancelled.\n";
        busNumber.clear();
        driverName.clear();
        arrivalTime.clear();
        departureTime.clear();
        from.clear();
        return;
    }

    std::cout << "\nBus installed successfully!\n";
}

void Bus::allotment() {
    std::cout << "Enter bus number to reserve seat (or 0 to cancel): ";
    std::string number;
    std::getline(std::cin, number);

    if(number == "0" || number.empty()) {
        std::cout << "Operation cancelled.\n";
        return;
    }

    // Find the bus in the global busList
    auto it = std::find_if(busList.begin(), busList.end(),
        [&](const Bus &b){ return b.getBusNumber() == number; });

    if(it == busList.end()) {
        std::cout << "Bus not found. Please try again.\n";
        return;
    }

    // Ask for seat number
    std::cout << "Enter seat number (1-32) (or 0 to cancel): ";
    int seatNumber;
    if(!(std::cin >> seatNumber)) {
        std::cout << "Invalid input. Operation cancelled.\n";
        // Clear the error flag and ignore invalid input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    // Clear buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(seatNumber == 0) {
        std::cout << "Operation cancelled.\n";
        return;
    }

    if(seatNumber < 1 || seatNumber > 32) {
        std::cout << "Invalid seat number. Please enter a number between 1 and 32.\n";
        return;
    }

    // Convert seatNumber to row and column
    int row = (seatNumber - 1) / 4;
    int col = (seatNumber - 1) % 4;

    // Check if seat is already booked
    if(it->seats[row][col].passengerName != "Empty") {
        std::cout << "That seat is already reserved by " << it->seats[row][col].passengerName << "!\n";
        return;
    }

    // If empty, ask for passenger name
    std::cout << "Enter passenger's name (or 0 to cancel): ";
    std::string passenger;
    std::getline(std::cin, passenger);

    if(passenger == "0" || passenger.empty()) {
        std::cout << "Operation cancelled.\n";
        return;
    }

    // Book seat
    it->seats[row][col].passengerName = passenger;
    double cost = it->seats[row][col].fare;

    std::cout << "Seat " << seatNumber << " reserved successfully for "
              << passenger << ".\n"
              << "Fare: Rs. " << std::fixed << std::setprecision(2) << cost << "\n";
}

void Bus::cancelSeat() {
    std::cout << "Enter bus number to cancel a seat (or 0 to cancel): ";
    std::string number;
    std::getline(std::cin, number);

    if(number == "0" || number.empty()) {
        std::cout << "Operation cancelled.\n";
        return;
    }

    // Find the bus in the global busList
    auto it = std::find_if(busList.begin(), busList.end(),
        [&](const Bus &b){ return b.getBusNumber() == number; });

    if(it == busList.end()) {
        std::cout << "Bus not found.\n";
        return;
    }

    std::cout << "Enter seat number to cancel (1-32) (or 0 to cancel): ";
    int seatNumber;
    if(!(std::cin >> seatNumber)) {
        std::cout << "Invalid input. Operation cancelled.\n";
        // Clear the error flag and ignore invalid input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(seatNumber == 0) {
        std::cout << "Operation cancelled.\n";
        return;
    }

    if(seatNumber < 1 || seatNumber > 32) {
        std::cout << "Invalid seat number. Please enter a number between 1 and 32.\n";
        return;
    }

    int row = (seatNumber - 1) / 4;
    int col = (seatNumber - 1) % 4;

    if(it->seats[row][col].passengerName == "Empty") {
        std::cout << "This seat is already empty.\n";
        return;
    }

    // Confirm cancellation
    std::cout << "Are you sure you want to cancel the reservation for seat "
              << seatNumber << " (Passenger: " << it->seats[row][col].passengerName << ")? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(tolower(confirm) != 'y') {
        std::cout << "Cancellation aborted.\n";
        return;
    }

    // Cancel the booking
    it->seats[row][col].passengerName = "Empty";
    std::cout << "Reservation for seat " << seatNumber << " has been cancelled.\n";
}

void Bus::show() const {
    printLine('*');
    std::cout << "Bus Number   : " << busNumber << "\n"
              << "Driver       : " << driverName << "\n"
              << "Arrival Time : " << arrivalTime << "\n"
              << "Departure Time: " << departureTime << "\n"
              << "From         : " << from << "\n"
              << "To           : " << to << "\n";
    printLine('*');

    int seatIndex = 1;
    int emptyCount = 0;

    // Print seats in a grid
    for(int i = 0; i < 8; ++i) {
        std::cout << "\nRow " << (i+1) << ":\n";
        for(int j = 0; j < 4; ++j) {
            std::cout << "  Seat " << std::setw(2) << seatIndex << ": ";
            if(seats[i][j].passengerName == "Empty") {
                std::cout << "Empty (Rs. " << std::fixed << std::setprecision(2) << seats[i][j].fare << ")";
                emptyCount++;
            } else {
                std::cout << seats[i][j].passengerName
                          << " (Rs. " << std::fixed << std::setprecision(2) << seats[i][j].fare << ")";
            }
            std::cout << "\n";
            seatIndex++;
        }
    }
    std::cout << "\nTotal empty seats: " << emptyCount << "\n\n";
}

void Bus::printBasicInfo() const {
    std::cout << "Bus Number    : " << busNumber << "\n"
              << "Driver        : " << driverName << "\n"
              << "Arrival Time  : " << arrivalTime << "\n"
              << "Departure Time: " << departureTime << "\n"
              << "Route         : " << from << " -> " << to << "\n";
}

bool Bus::matchesRoute(const std::string& origin, const std::string& dest) const {
    return (from == origin && to == dest);
}

std::string Bus::getBusNumber() const {
    return busNumber;
}

Seat& Bus::getSeat(int seatNumber) {
    int row = (seatNumber - 1) / 4;
    int col = (seatNumber - 1) % 4;
    return seats[row][col];
}

/****************************************
 *        Other Global Functions        *
 ****************************************/

/**
 * @brief Display all buses available in the system.
 *
 * Iterates through the global bus list and prints basic information for each bus.
 */
void showAllBuses() {
    if(busList.empty()) {
        std::cout << "No buses available.\n";
        return;
    }
    for(const auto &b : busList) {
        // If busNumber is empty, skip printing (shouldn't be in the list)
        if(b.getBusNumber().empty()) continue;

        printLine('*');
        b.printBasicInfo();
        printLine('*');
    }
}

/**
 * @brief Search for buses based on origin and destination.
 *
 * Prompts the user to enter origin and destination, then displays matching buses.
 */
void searchBusesByRoute() {
    if(busList.empty()) {
        std::cout << "No buses available.\n";
        return;
    }

    std::cout << "Enter origin (From): ";
    std::string origin;
    std::getline(std::cin, origin);

    // Check cancel
    if(origin == "0" || origin.empty()) {
        std::cout << "Search cancelled.\n";
        return;
    }

    std::cout << "Enter destination (To): ";
    std::string destination;
    std::getline(std::cin, destination);

    if(destination == "0" || destination.empty()) {
        std::cout << "Search cancelled.\n";
        return;
    }

    bool foundAny = false;
    for(const auto &b : busList) {
        if(b.matchesRoute(origin, destination)) {
            foundAny = true;
            printLine('=');
            b.printBasicInfo();
            printLine('=');
        }
    }
    if(!foundAny) {
        std::cout << "No matching buses found for route "
                  << origin << " -> " << destination << ".\n";
    }
}

/****************************************
 *              Main Function           *
 ****************************************/

/**
 * @brief Entry point of the Bus Booking System.
 *
 * Presents a menu-driven interface for users to interact with the system.
 * Continuously loops until the user chooses to exit.
 *
 * @return int Exit status.
 */
int main() {
    clearScreen();
    while(true) {
        std::cout << "\n\t\t===== Bus Booking System =====\n"
                  << "\t\t1. Install New Bus\n"
                  << "\t\t2. Reserve a Seat\n"
                  << "\t\t3. Show Bus Details\n"
                  << "\t\t4. Show All Buses Available\n"
                  << "\t\t5. Cancel (Remove) a Seat\n"
                  << "\t\t6. Search Buses by Route\n"
                  << "\t\t7. Exit\n\n"
                  << "\t\tEnter your choice:-> ";

        int choice;
        if(!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number between 1 and 7.\n";
            // Clear the error flag and ignore invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        // Clear newline
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice) {
            case 1: {
                Bus newBus;
                newBus.install();
                // Only add if busNumber is valid
                if(!newBus.getBusNumber().empty()) {
                    busList.push_back(newBus);
                }
                break;
            }
            case 2: {
                if(busList.empty()) {
                    std::cout << "No buses installed. Please install a bus first.\n";
                } else {
                    // Handle seat allotment using the Bus class
                    Bus dummy; // Temporary object to call the method
                    dummy.allotment();
                }
                break;
            }
            case 3: {
                if(busList.empty()) {
                    std::cout << "No buses installed yet.\n";
                    break;
                }
                std::cout << "Enter bus number to show details (or 0 to cancel): ";
                std::string number;
                std::getline(std::cin, number);
                if(number == "0" || number.empty()) {
                    std::cout << "Operation cancelled.\n";
                    break;
                }
                auto it = std::find_if(busList.begin(), busList.end(),
                    [&](const Bus &b){ return b.getBusNumber() == number; });
                if(it == busList.end()) {
                    std::cout << "Bus not found.\n";
                } else {
                    it->show();
                }
                break;
            }
            case 4: {
                showAllBuses();
                break;
            }
            case 5: {
                if(busList.empty()) {
                    std::cout << "No buses installed yet.\n";
                } else {
                    // Handle seat cancellation using the Bus class
                    Bus dummy; // Temporary object to call the method
                    dummy.cancelSeat();
                }
                break;
            }
            case 6: {
                searchBusesByRoute();
                break;
            }
            case 7: {
                std::cout << "Exiting... Have a nice day!\n";
                return 0;
            }
            default: {
                std::cout << "Invalid choice. Please enter a number between 1 and 7.\n";
                break;
            }
        }
    }
    return 0;
}
