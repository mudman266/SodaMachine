// Josh Williams
// 12/1/2019
// CSC-134-81A
// Final Project - Drink Machine Simulator

// Create a class that simulates and manages a soft drink machine.

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class DrinkMachine {
	// The structure Product will be used for items in the machine
	struct Product
	{
		string name;
		int qty = 0;
		float price = 0.0;
	};

	// An array of buttons for the machine, initalized at their starting values
	Product buttons[5] = {
		{ "Cola", 20, 1.0 },
		{ "Root Beer", 20, 1.0 },
		{ "Orange Soda", 20, 1.0 },
		{ "Grape Soda", 20, 1.0 },
		{ "Bottled Water", 20, 1.5 }
	};

private:
	// Variables
	float totalSales = 0.0;
	int drinksSold = 0;
	float currentAmount = 0.0;

	float inputMoney(int sel) {
		// Called by buyDrink to accept, validate, and return the amount of money input
		float money = 0.0; // Amount tendered
		float add = 0.0; // Add to tendered
		// Print price of drink
		printf("Input: $%.2f\n", buttons[sel - 1].price);
		// Gather money input
		printf("Amount to input (-1 to quit): ");
		cin >> money;
		// Verify a float was entered
		while (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			printf("Please enter a valid amount (-1 to quit): ");
			cin >> money;
		}
		// Check for quitting
		if (money < 0.0) {
			if (money == -1) {
				if (currentAmount > 0.0) {
					refund();
				}
				return 0;
			}
			else {
				// Negative amount, not -1
				while (money < 0 && money != -1) {
					printf("Enter a valid amount (-1 to quit): ");
					cin >> money;
				}
				while (cin.fail()) {
					cin.clear();
					cin.ignore(1000, '\n');
					printf("Enter a valid amount (-1 to quit): ");
					cin >> money;
				}
			}
		}
		currentAmount = money;
		// Ask for remaining amount of currentAmount if there is not enough for the selection
		while (currentAmount < buttons[sel - 1].price && add != -1.0) {
			printf("Input: $%.2f\nAmount to input (-1 to exit): ", buttons[sel - 1].price - currentAmount);
			cin >> add;
			// Check for float
			while (cin.fail()) {
				cin.clear();
				cin.ignore(1000, '\n');
				printf("Enter a valid amount (-1 to quit): ");
				cin >> add;
			}
			// Check for under 0 and not -1
			while (add < 0.0 && add != -1) {
				cin.clear();
				cin.ignore(1000, '\n');
				printf("Enter a valid amount (-1 to quit): ");
				cin >> add;
				// Check for float
				while (cin.fail()) {
					cin.clear();
					cin.ignore(1000, '\n');
					printf("Enter a valid amount (-1 to quit): ");
					cin >> add;
				}
			}
			// Add the new money to amount tendered
			if (add > 0) {
				currentAmount += add;
			}
		}
		if (add == -1) {
			if (currentAmount > 0.0) {
				refund();
			}
		}
	return currentAmount;
	}

	void dailyReport() {
		// Called by the destructor. Reports how many of each drink remain and how much money was collected
		printf("Drinks Sold: %d\nTotal Money: $%.2f\n", drinksSold, totalSales);
		printf("Drink Inventory Report\n");
		printf("%15s |%3s\n", "Drink Name", "Qty");
		for (int i = 0; i < 5; i++) {
			printf("%15s | %d\n", buttons[i].name.c_str(), buttons[i].qty);
		}
	}

	void refund() {
		// Return any money in currentAmount
		printf("Change Given: $%.2f\n", currentAmount);
		currentAmount = 0.0;
	}

	void sale(int item) {
		// Handles the sale after all checks have passed
		// Move drink money from tendered to totalSales
		totalSales += buttons[item - 1].price;
		// Decrement the tendered amount by the drink amount taken
		currentAmount -= buttons[item - 1].price;
		// Increment the drinks sold counter
		drinksSold += 1;
		// Deliver the drink
		printf("Drink Delivered: %s\n", buttons[item - 1].name.c_str());
		// Adjust inventory
		buttons[item - 1].qty -= 1;
		// Refund any money left
		if (currentAmount > 0.0) {
			refund();
		}
	}

public:
	float selection = 0; // User's menu selection
	
	// Constructor
	DrinkMachine() {

	}

	// Destructor
	~DrinkMachine() {
		dailyReport();
	}

	void displayChoices() {
		// Print the drink selection menu including Name, Price, In Stock status
		printf("Make a selection\n");
		printf("       Drink        | Price| In Stock \n");
		printf("1 - %15s | %.2f |  %s\n", buttons[0].name.c_str(), buttons[0].price, buttons[0].qty ? "True" : "False");
		printf("2 - %15s | %.2f |  %s\n", buttons[1].name.c_str(), buttons[1].price, buttons[1].qty ? "True" : "False");
		printf("3 - %15s | %.2f |  %s\n", buttons[2].name.c_str(), buttons[2].price, buttons[2].qty ? "True" : "False");
		printf("4 - %15s | %.2f |  %s\n", buttons[3].name.c_str(), buttons[3].price, buttons[3].qty ? "True" : "False");
		printf("5 - %15s | %.2f |  %s\n", buttons[4].name.c_str(), buttons[4].price, buttons[4].qty ? "True" : "False");
		printf("6 - Quit\n");
	}

	void buyDrink(int sel) {
		// Validate the selection
		while (sel < 1 || sel > 6) {
			// Not valid. Get a new selection and set on the machine
			printf("Enter a valid selection 1-6: ");
			cin >> sel;
			while (cin.fail()) {
				cin.clear();
				cin.ignore(1000, '\n');
				printf("Enter a valid selection 1-6: ");
				cin >> sel;
			}
		}
		selection = sel;
		// Check if the user wants to quit
		if (selection != 6) {
			// Not quitting and input was valid.
			// Make sure item is in stock before accepting money
			if (!buttons[sel - 1].qty) {
				// Not in stock
				printf("You selected an item that was out of stock.\n");
			}
			else {
				// At least 1 drink in machine. Continuing...
				// Collect the money
				currentAmount = inputMoney(sel);
				if (currentAmount >= buttons[sel - 1].price) {
					// Enough money to purchase
					// Process the sale
					sale(sel);
				}
				else {
					// Net enough money entered
					printf("Aborting Sale.\n");
					if (currentAmount > 0.0) {
						refund();
					}
				}
			}
		}
		else {
			// User selected quit
			if (currentAmount > 0.0) {
				refund();
			}
			printf("Shutting down drink machine.\n");
		}
	}
};

int main()
{
	// Variables
	string gather = "";
	bool valid = false;
	// Welcome message
    cout << "Drink Machine Simulator!\n";

	// Create a drink machine
	DrinkMachine machine1;
	do {
		// Print the drink selection menu
		machine1.displayChoices();
		printf("Selection: ");
		// Set the selection on the machine
		cin >> machine1.selection;
		// Check for proper input type
		while (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			printf("Enter a valid selection 1-6: ");
			cin >> machine1.selection;
		}
		// Does the users selection line up with a menu option (also checking for accidental decimals)
		while (machine1.selection != 1.0 && machine1.selection != 2.0 && machine1.selection != 3.0
			&& machine1.selection != 4.0 && machine1.selection != 5.0 && machine1.selection != 6.0) {
			cin.clear();
			cin.ignore(1000, '\n');
			printf("Enter a valid selection 1-6: ");
			cin >> machine1.selection;
			// Validate the new input after a bad menu selection
			while (cin.fail()) {
				cin.clear();
				cin.ignore(1000, '\n');
				printf("Enter a valid selection 1-6: ");
				cin >> machine1.selection;
			}
		}
		// Buy the drink
		machine1.buyDrink(machine1.selection);
	} while (machine1.selection != 6);
}