#include <iostream> // Allows us to use cout and cin to print and read from the console.
#include <string> // Allows usage of the string type to store text.
#include <vector> // Allows use of vector (dynamic list).
#include <iomanip> // Allows formatting numbers with decimal places.
#include <limits> // Allows cleaning invalid imput from user.
using namespace std; // Lets us use cout, cin, string etc, without std::

// Structure that represents a product in the vending machine.
struct Item {
    int code; // Product code that the user types to select an item. 
    string name; // Product name that will be show in the menu.
    string category; // Type of product.
    int pricePence; // Product price in cents (ex.: 350 = £3.50)
    int stock; // Number of product units available.
};

// Class that represents the vending machine behavior.
class VendingMachine {
private:
    vector<Item> items; // Vector holding all products available in the vending machine.
    int creditPence; // User money currently inserted.

public:
// Constructor: initializes the machine whit products and zero credit.
VendingMachine() : creditPence(0) {
    items.push_back({101, "Water 500ml",           "Drink",       100,  5});
    items.push_back({102, "Coca Cola Can",         "Drink",       300,  4});
    items.push_back({201, "Potato Crisps",         "Snack",       350,  6});
    items.push_back({202, "Salted Peanuts",        "Snack",       300,  5});
    items.push_back({301, "Milk Chocolate Bar",    "Chocolate",   400,  5});
    items.push_back({302, "White Chocolate Bar",   "Chocolate",   450,  4});
}

// Function that displays the full product list on the screen.
void showItems() const { // Const means it does not change data.
    cout << "----------------- AVAILABLE ITEMS ------------------------\n";
    cout << left << setw(8) << "Code" // Prints table headers whith spacing.
        << setw(22) << "Product"
        << setw(12) << "Category"
        << setw(10) << "Price"
        << "Stock\n";

        cout << "----------------------------------------------------------\n";

    
    for (const auto& item : items) { // Loop through all products in the vector and show each one.
        cout << left << setw(8) << item.code // Prints product code with spacing.
             << setw(22) << item.name // Prints product name.
             << setw(12) << item.category // Prints category.
             << "GBP " << fixed << setprecision(2) // Formats price in 2 decimals.
             << (item.pricePence / 100.0) // Converts pence to pounds.
             << "  (" << item.stock << ")\n"; // Shows stock count.
    }

    cout << "==========================================================\n"; // The user will type the code after this line appears.
}

// Show the main menu and current balance.
void showMenu() const {
    cout << "\n==== VENDING MACHINE ====\n";
    cout << "Current credit: GBP " << fixed << setprecision(2)
         << (creditPence / 100.0) << "\n\n"; // Converts pence to pounds.
    cout << "1 - Insert money\n"; // Menu option 1.
    cout << "2 - Buy item\n"; // Option 2.
    cout << "3 - Return change\n"; // Option 3.
    cout << "4 - Exit\n"; // Option 4.
    cout << "Choose an option: "; // Prompts user.
}

// User inserts money in pence into the machine.
void insertMoney() {
    cout << "\nEnter the value in pence (example 200 = GBP 2.00): ";
    int value; // Stores user input.

    if (!(cin >> value)) { // Checks if input is invalid.
        clearInput(); // Clear input buffer.
        cout << "Invalid input. No money added.\n"; // Shows error message.
        return; // Exit the function early.
    }
    if (value <= 0) { // Prevents zero or negative amounts.
        cout << "Value must be greater than zero.\n";
        return;
    }
    
    creditPence += value; // Adds the value entered by the user (in pence)to the current credit stored in the machine.
    cout << "Money inserted successfully.\n"; // Displays a confirmation message informing the user that the money was added.
    cout << "Current credit: GBP " << fixed << setprecision(2) // Prints a label and formats the number to always show two decimal places.
         << (creditPence / 100.0) << "\n";  // Converts the credit from pence to pounds and displays the update balance.
}

// Searches for product by code
Item* findItemByCode(int code) { // Returns pointe to item. 
    for (auto& item : items) { //Loops through vector.
        if (item.code == code) { // Checks if code matches.
            return &item; // Returns address of found item.
        }
    }
    return nullptr; // Returns null if not found.
}

// Suggests a complementary item.
void suggestExtra(const Item& purchased) {
    cout << "\nSuggested combo:\n";

    if (purchased.category == "Drink") {
        cout << "How about a snack or chocolate as well?\n"; // Suggests snack or chocolate.
    } else if (purchased.category == "Snack") {
        cout << "Snacks go well with a drink!\n"; // Sugessts drink
    } else if (purchased.category == "Chocolate") {
        cout << "Chocolate and drink would make a great combo!\n"; // Suggests drink.
    }
    cout << "You can keep buying while you have credit available.\n"; // It informs the user that they can make purchases as long as they have credit.
}

// Purchase process.
void buyItem() {
    if (creditPence <= 0) {
        cout << "\nYou must insert money before buying.\n"; // Checks if user has no credit.
        return;
    }

    showItems(); // Shows all products.
    cout << "Enter product code: ";

    int code; // Stores the user's choice.
    if (!(cin >> code)) { // If the user types letters, the message invalid input will be displayed.
        clearInput();
        cout << "Invalid input. Try again.\n";
        return;
    }
    Item* item = findItemByCode(code); // If product not found, shows the message invalid input.
    if (item == nullptr) {
        cout << "Invalid code. Product not found.\n";
        return;
    }
    if (item->stock <= 0) { // Item has no stock.
        cout << "Sorry,  \"" << item->name << "\"is out of stock.\n";
        return;
    }
    if (creditPence < item->pricePence) { // If user has not enough money.
        int missing = item->pricePence - creditPence; // Calculates missing amount.
        cout << "Not enough credit to buy \"" << item->name << "\".\n";
        cout << "Missing GBP " << fixed << setprecision(2)
             << (missing / 100.0) << "\n";
        return;
    }

    // Purchase successful.
    creditPence -= item->pricePence; // Deducts price from credit.
    item->stock -= 1; // Reduces stock with each item sold.

    cout << "\nDispensing: " << item->name << "...\n"; // Simulates vending.
    cout << "Purchase successful.\n";
    cout << "Remainig credit: GBP " << fixed << setprecision(2)
         << (creditPence / 100.0) << "\n";

         suggestExtra(*item); // Suggests complimentary item.
}

// Return any remaining credit user.
void returnChange() {
    if (creditPence <=0) { // Checks if there is no change.
        cout << "\nNo change available.\n";
        return;
    }
    cout << "\nReturning change: GBP " << fixed << setprecision(2)
         << (creditPence / 100.0) << "\n";
    creditPence = 0; // Resets credit to zero.
}

// Main program Loop. Keeps program running until user exits.
void run() {
    bool running = true; // Controls loop.

    while (running) { // Loop continues until false.
        showMenu(); // Shows menu again.

        int option; // Stores user selection.
        if (!(cin >> option)) { // Handles invalid input.
            clearInput();
            cout << "Invalid input. Try again.\n";
            continue;
        }

        switch (option) { // Handles user selection.
            case 1:
                insertMoney(); // Calls money insertion function.
                break;
            case 2:
                buyItem(); // Calls buying function
                break;
            case 3:
                returnChange(); // Returns remaining money.
                break;
            case 4:
                if (creditPence > 0) { // If user still has credit.
                    cout << "\nYou still have credit.\n";
                    returnChange();
                }
                cout << "\nThank you for using the Vending Machine.\n";
                running = false; // End loop.
                break;
            default:
                cout << "Invalid option. Choose 1 to 4.\n"; // Error for invalid menu choice.
                break;
        }
    }
}
private:
// Cleans incorrect input from cin to avoid infinite Loops.
void clearInput() {
    cin.clear(); // Clear error flags.
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Removes lefover input.
}
};

// Main function. Program starts here.
int main() {
    VendingMachine machine; // Creates vending machine object.
    machine.run(); // Starts the program loop.

    return 0; // Ends program.
}