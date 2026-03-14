/*
 * ============================================================
 *  ITM Skills University
 *  B.Tech CSE 2025-29 | C++ Semester II
 *  Case Study 86: Fuel Consumption Monitoring System
 *  Student Project – Problem Statement Implementation
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

// -------------------------------------------------------
//  Helper: get today's date as a formatted string
// -------------------------------------------------------
string getCurrentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buf[40];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d",
             ltm->tm_mday, ltm->tm_mon + 1, 1900 + ltm->tm_year);
    return string(buf);
}

// -------------------------------------------------------
//  Data structure for one fuel entry
// -------------------------------------------------------
struct FuelEntry
{
    int    entryId;
    string vehicleNo;
    string date;
    double litresFilled;   // litres added to tank
    double kmReading;      // odometer reading at fill-up
    double pricePerLitre;
    double totalCost;
    string driverName;
    string fuelType;       // Petrol / Diesel / CNG
};

// -------------------------------------------------------
//  Global storage (in-memory)
// -------------------------------------------------------
vector<FuelEntry> fuelLog;
int nextId = 1;

// -------------------------------------------------------
//  Utility: clear input buffer
// -------------------------------------------------------
void flushInput()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// -------------------------------------------------------
//  Utility: draw a simple horizontal line
// -------------------------------------------------------
void drawLine(char c = '-', int len = 60)
{
    for (int i = 0; i < len; i++) cout << c;
    cout << "\n";
}

// -------------------------------------------------------
//  1. Add a new fuel entry (log)
// -------------------------------------------------------
void addFuelEntry()
{
    FuelEntry e;
    e.entryId = nextId++;

    cout << "\n--- Add Fuel Entry (ID: " << e.entryId << ") ---\n";

    cout << "Vehicle Number    : ";
    flushInput();
    getline(cin, e.vehicleNo);

    cout << "Driver Name       : ";
    getline(cin, e.driverName);

    cout << "Date (DD/MM/YYYY) [leave blank for today]: ";
    getline(cin, e.date);
    if (e.date.empty()) e.date = getCurrentDate();

    cout << "Fuel Type (Petrol/Diesel/CNG): ";
    getline(cin, e.fuelType);

    cout << "Odometer Reading (km): ";
    while (!(cin >> e.kmReading) || e.kmReading < 0)
    {
        cout << "  Invalid. Enter a positive number: ";
        cin.clear(); flushInput();
    }

    cout << "Litres Filled        : ";
    while (!(cin >> e.litresFilled) || e.litresFilled <= 0)
    {
        cout << "  Invalid. Enter a positive number: ";
        cin.clear(); flushInput();
    }

    cout << "Price per Litre (Rs) : ";
    while (!(cin >> e.pricePerLitre) || e.pricePerLitre <= 0)
    {
        cout << "  Invalid. Enter a positive number: ";
        cin.clear(); flushInput();
    }

    e.totalCost = e.litresFilled * e.pricePerLitre;

    fuelLog.push_back(e);

    cout << "\n  Entry saved successfully.\n";
    cout << "  Total Cost = Rs " << fixed << setprecision(2) << e.totalCost << "\n";
}

// -------------------------------------------------------
//  2. Display all fuel entry logs
// -------------------------------------------------------
void displayAllEntries()
{
    if (fuelLog.empty())
    {
        cout << "\n  No fuel entries found.\n";
        return;
    }

    cout << "\n";
    drawLine('=');
    cout << left
         << setw(4)  << "ID"
         << setw(12) << "Vehicle"
         << setw(12) << "Date"
         << setw(10) << "Driver"
         << setw(8)  << "Type"
         << setw(8)  << "Litres"
         << setw(10) << "Km Read"
         << setw(12) << "Cost(Rs)"
         << "\n";
    drawLine('=');

    for (const FuelEntry &e : fuelLog)
    {
        cout << left
             << setw(4)  << e.entryId
             << setw(12) << e.vehicleNo.substr(0, 11)
             << setw(12) << e.date
             << setw(10) << e.driverName.substr(0, 9)
             << setw(8)  << e.fuelType.substr(0, 7)
             << setw(8)  << fixed << setprecision(1) << e.litresFilled
             << setw(10) << fixed << setprecision(0) << e.kmReading
             << setw(12) << fixed << setprecision(2) << e.totalCost
             << "\n";
    }
    drawLine('=');
    cout << "  Total Entries: " << fuelLog.size() << "\n";
}

// -------------------------------------------------------
//  3. Consumption Analysis for a specific vehicle
// -------------------------------------------------------
void consumptionAnalysis()
{
    if (fuelLog.size() < 2)
    {
        cout << "\n  Need at least 2 entries to compute consumption.\n";
        return;
    }

    cout << "\n  Enter Vehicle Number for analysis: ";
    flushInput();
    string vno;
    getline(cin, vno);

    // collect entries for this vehicle, sorted by km reading
    vector<FuelEntry> vEntries;
    for (const FuelEntry &e : fuelLog)
        if (e.vehicleNo == vno)
            vEntries.push_back(e);

    if (vEntries.size() < 2)
    {
        cout << "\n  Not enough data for vehicle: " << vno << "\n";
        return;
    }

    // sort by odometer
    sort(vEntries.begin(), vEntries.end(),
         [](const FuelEntry &a, const FuelEntry &b)
         { return a.kmReading < b.kmReading; });

    cout << "\n--- Consumption Analysis: " << vno << " ---\n";
    drawLine();

    double totalLitres = 0, totalCost = 0;
    double totalKm = vEntries.back().kmReading - vEntries.front().kmReading;

    for (size_t i = 1; i < vEntries.size(); i++)
    {
        double kmDiff     = vEntries[i].kmReading - vEntries[i-1].kmReading;
        double litres     = vEntries[i].litresFilled;
        double mileage    = (litres > 0) ? (kmDiff / litres) : 0;

        totalLitres += litres;
        totalCost   += vEntries[i].totalCost;

        cout << "  Fill-up #" << i
             << " | Date: " << vEntries[i].date
             << " | km covered: " << fixed << setprecision(0) << kmDiff
             << " | Litres: " << fixed << setprecision(2) << litres
             << " | Mileage: " << fixed << setprecision(2) << mileage << " km/L\n";
    }

    double avgMileage = (totalLitres > 0) ? (totalKm / totalLitres) : 0;

    drawLine();
    cout << "  Total Distance   : " << fixed << setprecision(0) << totalKm    << " km\n";
    cout << "  Total Fuel Used  : " << fixed << setprecision(2) << totalLitres << " L\n";
    cout << "  Total Fuel Cost  : Rs " << fixed << setprecision(2) << totalCost << "\n";
    cout << "  Average Mileage  : " << fixed << setprecision(2) << avgMileage  << " km/L\n";
    cout << "  Cost per km      : Rs "
         << fixed << setprecision(2) << (totalKm > 0 ? totalCost / totalKm : 0) << "\n";
}

// -------------------------------------------------------
//  4. Efficiency Report (all vehicles summary)
// -------------------------------------------------------
void efficiencyReport()
{
    if (fuelLog.empty())
    {
        cout << "\n  No data available.\n";
        return;
    }

    // collect unique vehicle numbers
    vector<string> vehicles;
    for (const FuelEntry &e : fuelLog)
    {
        if (find(vehicles.begin(), vehicles.end(), e.vehicleNo) == vehicles.end())
            vehicles.push_back(e.vehicleNo);
    }

    cout << "\n";
    drawLine('=');
    cout << "           FUEL EFFICIENCY REPORT\n";
    cout << "           Generated: " << getCurrentDate() << "\n";
    drawLine('=');

    double grandTotalCost = 0, grandTotalLitres = 0, grandTotalKm = 0;

    for (const string &vno : vehicles)
    {
        vector<FuelEntry> vEntries;
        for (const FuelEntry &e : fuelLog)
            if (e.vehicleNo == vno)
                vEntries.push_back(e);

        sort(vEntries.begin(), vEntries.end(),
             [](const FuelEntry &a, const FuelEntry &b)
             { return a.kmReading < b.kmReading; });

        double totalLitres = 0, totalCost = 0;
        double totalKm = 0;

        if (vEntries.size() >= 2)
            totalKm = vEntries.back().kmReading - vEntries.front().kmReading;

        for (size_t i = 1; i < vEntries.size(); i++)
        {
            totalLitres += vEntries[i].litresFilled;
            totalCost   += vEntries[i].totalCost;
        }

        double avgMileage = (totalLitres > 0) ? (totalKm / totalLitres) : 0;
        string status = (avgMileage >= 12) ? "GOOD" : (avgMileage >= 8) ? "AVERAGE" : "POOR";

        grandTotalCost   += totalCost;
        grandTotalLitres += totalLitres;
        grandTotalKm     += totalKm;

        cout << "\n  Vehicle   : " << vno
             << " | Entries: " << vEntries.size() << "\n";
        cout << "  Distance  : " << fixed << setprecision(0) << totalKm << " km"
             << " | Fuel: " << fixed << setprecision(2) << totalLitres << " L"
             << " | Cost: Rs " << fixed << setprecision(2) << totalCost << "\n";
        cout << "  Avg Mileage: " << fixed << setprecision(2) << avgMileage
             << " km/L  [" << status << "]\n";
        drawLine();
    }

    cout << "\n  GRAND TOTAL\n";
    cout << "  Total Vehicles   : " << vehicles.size() << "\n";
    cout << "  Total Distance   : " << fixed << setprecision(0) << grandTotalKm    << " km\n";
    cout << "  Total Fuel Used  : " << fixed << setprecision(2) << grandTotalLitres << " L\n";
    cout << "  Total Cost       : Rs " << fixed << setprecision(2) << grandTotalCost << "\n";
    drawLine('=');
}

// -------------------------------------------------------
//  5. Search entry by vehicle number
// -------------------------------------------------------
void searchByVehicle()
{
    cout << "\n  Enter Vehicle Number to search: ";
    flushInput();
    string vno;
    getline(cin, vno);

    bool found = false;
    for (const FuelEntry &e : fuelLog)
    {
        if (e.vehicleNo == vno)
        {
            if (!found)
            {
                cout << "\n  Records for vehicle: " << vno << "\n";
                drawLine();
                found = true;
            }
            cout << "  [" << e.entryId << "] " << e.date
                 << " | Driver: " << e.driverName
                 << " | " << e.fuelType
                 << " | " << e.litresFilled << " L"
                 << " | Rs " << fixed << setprecision(2) << e.totalCost
                 << " | KM: " << e.kmReading << "\n";
        }
    }
    if (!found)
        cout << "\n  No records found for vehicle: " << vno << "\n";
}

// -------------------------------------------------------
//  6. Save data to file
// -------------------------------------------------------
void saveToFile()
{
    ofstream fout("fuel_data.txt");
    if (!fout)
    {
        cout << "\n  Error: Could not open file for saving.\n";
        return;
    }

    fout << nextId << "\n";
    for (const FuelEntry &e : fuelLog)
    {
        fout << e.entryId      << "|"
             << e.vehicleNo    << "|"
             << e.date         << "|"
             << e.litresFilled << "|"
             << e.kmReading    << "|"
             << e.pricePerLitre<< "|"
             << e.totalCost    << "|"
             << e.driverName   << "|"
             << e.fuelType     << "\n";
    }
    fout.close();
    cout << "\n  Data saved to fuel_data.txt (" << fuelLog.size() << " records).\n";
}

// -------------------------------------------------------
//  7. Load data from file
// -------------------------------------------------------
void loadFromFile()
{
    ifstream fin("fuel_data.txt");
    if (!fin)
    {
        cout << "\n  No saved data found. Starting fresh.\n";
        return;
    }

    fuelLog.clear();
    fin >> nextId;
    fin.ignore();

    string line;
    while (getline(fin, line))
    {
        if (line.empty()) continue;
        FuelEntry e;
        stringstream ss(line);
        string token;

        getline(ss, token, '|'); e.entryId       = stoi(token);
        getline(ss, e.vehicleNo,    '|');
        getline(ss, e.date,         '|');
        getline(ss, token, '|');  e.litresFilled  = stod(token);
        getline(ss, token, '|');  e.kmReading     = stod(token);
        getline(ss, token, '|');  e.pricePerLitre = stod(token);
        getline(ss, token, '|');  e.totalCost     = stod(token);
        getline(ss, e.driverName,   '|');
        getline(ss, e.fuelType);

        fuelLog.push_back(e);
    }
    fin.close();
    cout << "\n  " << fuelLog.size() << " record(s) loaded from file.\n";
}

// -------------------------------------------------------
//  Main menu
// -------------------------------------------------------
void showMenu()
{
    cout << "\n";
    drawLine('=');
    cout << "   ITM Skills University – Fuel Monitoring System\n";
    cout << "   B.Tech CSE 2025-29 | Semester II | C++\n";
    drawLine('=');
    cout << "   1. Add Fuel Entry\n";
    cout << "   2. View All Fuel Logs\n";
    cout << "   3. Consumption Analysis (per vehicle)\n";
    cout << "   4. Efficiency Report (all vehicles)\n";
    cout << "   5. Search by Vehicle Number\n";
    cout << "   6. Save Data to File\n";
    cout << "   7. Load Data from File\n";
    cout << "   0. Exit\n";
    drawLine('=');
    cout << "   Enter your choice: ";
}

// -------------------------------------------------------
//  Program entry point
// -------------------------------------------------------
int main()
{
    cout << "\n  Welcome to the Fuel Consumption Monitoring System\n";
    loadFromFile();

    int choice;
    do
    {
        showMenu();
        while (!(cin >> choice))
        {
            cout << "  Invalid input. Enter a number: ";
            cin.clear(); flushInput();
        }

        switch (choice)
        {
            case 1: addFuelEntry();        break;
            case 2: displayAllEntries();   break;
            case 3: consumptionAnalysis(); break;
            case 4: efficiencyReport();    break;
            case 5: searchByVehicle();     break;
            case 6: saveToFile();          break;
            case 7: loadFromFile();        break;
            case 0:
                saveToFile();
                cout << "\n  Thank you. Exiting system.\n\n";
                break;
            default:
                cout << "\n  Invalid option. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
