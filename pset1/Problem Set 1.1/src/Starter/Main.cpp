#include "SetInternal.h"
#include "ObjectParser.h"
#include "TestDriver.h"
#include "FileParser.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include "simpio.h"
#include "strlib.h"
#include "filelib.h"
#include "console.h"
using namespace std;

/* Everything in here is private to this file. */
namespace {   
    /* Padding sizes for various outputs. */
    const int kObjectWidth = 12;

    /* The specific test cases we're going to run. */
    const vector<NamedFunction> kTestFunctions = {
        { isElementOf,              "S ∈ T      " },
        { isSubsetOf,               "S ⊆ T      " },
        { areDisjointSets,          "S ∩ T = Ø  " },
        { isSingletonOf,            "S = {T}    " },
        { isElementOfPowerSet,      "S ∈ ℘(T)   " },
        { isSubsetOfPowerSet,       "S ⊆ ℘(T)   " },
        { isSubsetOfDoublePowerSet, "S ⊆ ℘(℘(T))" },
    };

    /* Given a Result, returns a string representation of that result. */
    string toString(Result r) {
        if (r == Result::TRUE)  return "true";
        if (r == Result::FALSE) return "false";
        if (r == Result::ERROR) return "error";
        Utilities::error("Unknown result type?");
    }

    /* Names of the files we'll look at. */
    vector<string> objectFileNames() {
      return { "MuchAdoAboutNothing.sets", "Sample.sets" };
    }

    /* Lists all objects available in the res/ directory. */
    void viewObjects() {
        for (string filename: objectFileNames()) {
            cout << "  " << filename << ": " << endl;

            /* List all entries. */
            try {
                for (const auto& entry: parseFile(filename)) {
                    cout << "    " << left << setw(kObjectWidth) << entry.first << ": " << flush;
                    try {
                        auto obj = loadObject(*entry.second);
                        cout << obj << endl;
                    } catch (const std::exception& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                }
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
            cout << endl;
        }
    }

    /* Reads a string from the user, reprompting until we can understand it as an object. */
    string getObject(const string& prompt) {
        while (true) {
            string result = getLine(prompt);
            try {
                /* See if we can parse this. */
                istringstream converter(result);
                (void) loadObject(converter);

                return result;
            } catch (const exception& e) {
                cerr << "Sorry, I couldn't understand that." << endl;
            }
        }
    }

    /* Runs all tests on a pair of sets provided by the user. */
    void manualTests() {
        string sName = getObject("Enter object S: ");
        string tName = getObject("Enter object T: ");

        auto results = runAllTests({ { sName, tName, make_shared<istringstream>(sName), make_shared<istringstream>(tName) } }, kTestFunctions);
        for (TestResults result: results) {
            for (NamedResult oneTest: result.results) {
                cout << "  " << oneTest.name << ": " << flush;
                if (oneTest.result != Result::ERROR) {
                    cout << toString(oneTest.result) << endl;
                } else {
                    cerr << "Error: " << oneTest.message << endl;
                }
            }
        }
    }

    /* Returns a list of all the sample objects from Sample.sets. */
    vector<string> objectsFrom(const string& filename) {
        vector<string> result;
        for (const auto& entry: parseFile(filename)) {
            ostringstream builder;
            builder << entry.second->rdbuf();
            result.push_back(trim(builder.str()));
        }
        return result;
    }

    /* Given a list of strings representing objects, returns the all-pairs
     * tests of running each object against each other.
     */
    vector<NamedSource> allPairsTests(const vector<string>& objects) {
        vector<NamedSource> result;
        for (const auto& one: objects) {
            for (const auto& two: objects) {
                NamedSource source;
                source.sName = one;
                source.sSource = make_shared<istringstream>(one);
                source.tName = two;
                source.tSource = make_shared<istringstream>(two);

                result.push_back(source);
            }
        }
        return result;
    }

    /* Runs all tests on a pair of sets provided by the user. */
    void automatedTests() {
        try {
            /* Build the list of tests; all pairs against one another. */
            vector<NamedSource> tests = allPairsTests(objectsFrom("Sample.sets"));

            auto results = runAllTests(tests, kTestFunctions);

            for (TestResults result: results) {
                cout << "  S = " << result.sName << ", T = " << result.tName << '\n';
                for (NamedResult oneTest: result.results) {
                    cout << "    " << oneTest.name << ": ";
                    if (oneTest.result != Result::ERROR) {
                        cout << toString(oneTest.result) << '\n';
                    } else {
                        cerr << "Error: " << oneTest.message << '\n';
                    }
                }
                cout << endl;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    /* Type representing a menu option. */
    enum MenuOption {
        VIEW_OBJECTS = 1,
        MANUAL_TESTS,
        AUTOMATED_TESTS,
        QUIT
    };

    /* Displays all the menu options. */
    void displayMenu() {
        cout << "CS103: Programming Assignment for Problem Set 1" << endl;
        cout << "  " << VIEW_OBJECTS    << ": View all objects contained in the bundled files." << endl;
        cout << "  " << MANUAL_TESTS    << ": Run your functions on a pair of objects." << endl;
        cout << "  " << AUTOMATED_TESTS << ": Run your functions on all our sample objects." << endl;
        cout << "  " << QUIT            << ": Quit this program." << endl;
    }

    /* Handles a response from the menu. */
    MenuOption handleMenuResponse() {
        while (true) {
            MenuOption option = MenuOption(getInteger("Enter a choice: "));
            switch (option) {
            case VIEW_OBJECTS:
                viewObjects();
                return option;
            case MANUAL_TESTS:
                manualTests();
                return option;
            case AUTOMATED_TESTS:
                automatedTests();
                return option;
            case QUIT:
                return option; // Unreachable; silences warnings
            default:
                cerr << "Please enter a valid selection." << endl;
            }
        }
    }
}

int main() {
    while (true) {
        displayMenu();
        if (handleMenuResponse() == QUIT) return 0;
        cout << endl;
        cout << endl;
    }
}
