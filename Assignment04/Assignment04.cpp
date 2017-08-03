/*
* Osman Pamuk, Assignment 4
* Microsoft (R) C/C++ Optimizing Compiler Version 19.10.25019 for x86
* Windows 10
* Tested by GNU C++ compiler
* From C++11, I have used:
* - Brace Initialization
* - static_cast
* - for each
*/

/*******************************************************************
* FILE NAME: Assignment04.cpp
********************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <cstring>

#define DEFAULT_SIZE 20

using namespace std;

// enum types
enum class AirshipType { AIRPLANE, BALLOON };

enum class EngineType { JET, PROPELLER };

enum class GasType { HELIUM, HYDROGEN };

/********************************************************************
* CLASS NAME: CAirship *
* PURPOSE: To store basic airship information and 
* to declare the pure virtual functions
**********************************************************************/
class CAirship {
public:
	virtual ~CAirship() = default;

	CAirship(int maxPassengerCount, int maxCargoWeight, char* pName) :
		m_maxPassengerCount{maxPassengerCount}, m_maxCargoWeight{maxCargoWeight} {
		strncpy(m_strName, pName, DEFAULT_SIZE);
	}

	virtual AirshipType getAirshipType() = 0;
	virtual void showData() = 0;
protected:
	int m_maxPassengerCount;
	int m_maxCargoWeight;
	char m_strName[DEFAULT_SIZE];
};


/********************************************************************
* CLASS NAME: CAirplane *
* PURPOSE: To store airplane specific information and
* to define  airplane specific virtual functions
**********************************************************************/
class CAirplane : public CAirship {
public:
	CAirplane(int maxPassengerCount, int maxCargoWeight, char* strName, EngineType engineType, int maxRange) :
		CAirship{maxPassengerCount, maxCargoWeight, strName}, m_engineType(engineType), m_maxRange(maxRange) {}

	AirshipType getAirshipType() override { return AirshipType::AIRPLANE; }
	void showData() override;
private:
	EngineType m_engineType;
	int m_maxRange;
};

/********************************************************************
* CLASS NAME: CBalloon *
* PURPOSE: To store balloon specific information and
* to define balloon specific virtual functions
**********************************************************************/
class CBalloon : public CAirship {
public:
	CBalloon(int maxPassengerCount, int maxCargoWeight, char* strName, GasType gasType, int maxAltitude) :
		CAirship{maxPassengerCount, maxCargoWeight, strName}, m_gasType(gasType), m_maxAltitude(maxAltitude) {}

	AirshipType getAirshipType() override { return AirshipType::BALLOON; };
	void showData() override;
private:
	GasType m_gasType;
	int m_maxAltitude;
};

/********************************************************************
* FUNCTION: CAirplane::showData *
* PURPOSE: prints airplane specific information
*
* PARAMETERS: N/A
* LOCAL VARIABLES: N/A
********************************************************************/
void CAirplane::showData() {
	cout << setw(30) << m_strName << setw(20);
	cout << (m_engineType == EngineType::JET ? "Jet" : "Propeller");
	cout << setw(16) << m_maxRange << endl;
}

/********************************************************************
* FUNCTION: CAirplane::showData *
* PURPOSE: prints balloon specific information
*
* PARAMETERS: N/A
* LOCAL VARIABLES: N/A
********************************************************************/
void CBalloon::showData() {
	cout << setw(30) << m_strName << setw(20);
	cout << (m_gasType == GasType::HELIUM ? "helium" : "hydrogen");
	cout << setw(16) << m_maxAltitude << endl;
}


int main(int argc, char* argv[]) {
	// look if there is an command line argument 
	if (argc < 2) {
		cout << "Usage: " << argv[0] << " <filename>" << endl;
		return 0;
	}
	// look if file is present
	ifstream Infile(argv[1]);
	if (!Infile) {
		cout << "Cannot open file\n";
		return 1;
	}
	// be ready to read the file
	char strLineBuf[100]; // to store a file line
	char d[] = ","; // seperator for words in a line
	CAirship* pAirships[10]; // CAirship array
	auto i = 0;
	// read the file line by line
	while (Infile.getline(strLineBuf, 100)) {
		// parse the line string
		auto airshipType = static_cast<AirshipType>(atoi(strtok(strLineBuf, d)));
		auto strName = strtok(nullptr, d);
		auto maxPassengerCount = atoi(strtok(nullptr, d));
		auto maxCargoWeight = atoi(strtok(nullptr, d));
		auto gas_engineType = atoi(strtok(nullptr, d));
		auto maxRange_Altitude = atoi(strtok(nullptr, d));
		// check if it is an airplane or a balloon
		switch (airshipType) {
			case AirshipType::AIRPLANE:
				// Create Airplane Object
				pAirships[i++] = new CAirplane{maxPassengerCount, maxCargoWeight, strName,
					static_cast<EngineType>(gas_engineType), maxRange_Altitude};
				break;
			case AirshipType::BALLOON:
				// Create Balloon Object
				pAirships[i++] = new CBalloon{maxPassengerCount, maxCargoWeight, strName,
					static_cast<GasType>(gas_engineType), maxRange_Altitude};
				break;
		} // end switch
		// clear the input buffer
		memset(strLineBuf, '\0', 100);
	}

	// print all airplanes
	cout << "Listing of all Airplanes\n";
	cout << left << setw(30) << "Name" << setw(20) << "Engine Type" << setw(16) << "Maximum Range" << endl;
	cout << setw(66) << setfill('-') << "" << endl << setfill(' ');
	// loop over all the airships and print its information if it is an airplane 
	for (const auto& airship : pAirships) {
		if (airship->getAirshipType() == AirshipType::AIRPLANE)
			airship->showData();
	}

	// print all balloons
	cout << "\n\nListing of all Balloons\n";
	cout << left << setw(30) << "Name" << setw(20) << "Gas Type" << setw(16) << "Maximum Altitude" << endl;
	cout << setw(66) << setfill('-') << "" << endl << setfill(' ');
	// loop over all the airships and print its information if it is an balloon 
	for (const auto& airship : pAirships) {
		if (airship->getAirshipType() == AirshipType::BALLOON)
			airship->showData();
	}
	// clean up
	for (i = 0; i < 10; i++) {
		if (pAirships[i])
			delete pAirships[i]; // Delete appropriate object
	} // end for loop
	return 0;
}
