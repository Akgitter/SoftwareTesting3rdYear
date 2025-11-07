//
// Created by HP on 03-11-2025.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <random>

// JSON-like string builder helper
std::string escapeJSON(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (c == '"') result += "\\\"";
        else if (c == '\\') result += "\\\\";
        else if (c == '\n') result += "\\n";
        else if (c == '\r') result += "\\r";
        else if (c == '\t') result += "\\t";
        else result += c;
    }
    return result;
}

// Generate UUID-like string (simple random hex)
std::string generateUUID() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);

    const char* hex = "0123456789abcdef";
    std::string uuid;
    for (int i = 0; i < 32; i++) {
        if (i == 8 || i == 12 || i == 16 || i == 20) uuid += '-';
        uuid += hex[dis(gen)];
    }
    return uuid;
}

// Get current UTC timestamp in ISO 8601 format
std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* utc = std::gmtime(&now);
    std::ostringstream oss;
    oss << std::put_time(utc, "%Y-%m-%dT%H:%M:%S") << "Z";
    return oss.str();
}

// TestCase structure
struct TestCase {
    std::string id;
    std::string title;
    std::string description;
    std::string technique;
    std::vector<std::string> steps;
    std::string expectedResult;
    std::string priority;
    std::vector<std::string> tags;
    std::vector<std::string> platforms;
    std::string createdAt;

    // Constructor
    TestCase(const std::string& t, const std::string& desc, const std::string& tech,
             const std::vector<std::string>& st, const std::string& exp,
             const std::string& prio = "Medium",
             const std::vector<std::string>& tgs = {},
             const std::vector<std::string>& plats = {})
        : title(t), description(desc), technique(tech), steps(st),
          expectedResult(exp), priority(prio), tags(tgs), platforms(plats) {
        id = generateUUID();
        createdAt = getCurrentTimestamp();
    }

    // Convert to JSON string
    std::string toJSON() const {
        std::ostringstream json;
        json << "  {\n";
        json << "    \"id\": \"" << id << "\",\n";
        json << "    \"title\": \"" << escapeJSON(title) << "\",\n";
        json << "    \"description\": \"" << escapeJSON(description) << "\",\n";
        json << "    \"technique\": \"" << technique << "\",\n";
        json << "    \"steps\": [\n";
        for (size_t i = 0; i < steps.size(); i++) {
            json << "      \"" << escapeJSON(steps[i]) << "\"";
            if (i < steps.size() - 1) json << ",";
            json << "\n";
        }
        json << "    ],\n";
        json << "    \"expected_result\": \"" << escapeJSON(expectedResult) << "\",\n";
        json << "    \"priority\": \"" << priority << "\",\n";
        json << "    \"tags\": [";
        for (size_t i = 0; i < tags.size(); i++) {
            json << "\"" << tags[i] << "\"";
            if (i < tags.size() - 1) json << ", ";
        }
        json << "],\n";
        json << "    \"platforms\": [";
        for (size_t i = 0; i < platforms.size(); i++) {
            json << "\"" << platforms[i] << "\"";
            if (i < platforms.size() - 1) json << ", ";
        }
        json << "],\n";
        json << "    \"created_at\": \"" << createdAt << "\"\n";
        json << "  }";
        return json.str();
    }

    // Convert to CSV row
    std::string toCSV() const {
        std::ostringstream csv;
        csv << "\"" << id << "\",";
        csv << "\"" << escapeJSON(title) << "\",";
        csv << "\"" << escapeJSON(description) << "\",";
        csv << "\"" << technique << "\",";
        // Steps as semicolon-separated
        csv << "\"";
        for (size_t i = 0; i < steps.size(); i++) {
            csv << escapeJSON(steps[i]);
            if (i < steps.size() - 1) csv << "; ";
        }
        csv << "\",";
        csv << "\"" << escapeJSON(expectedResult) << "\",";
        csv << "\"" << priority << "\",";
        // Tags as semicolon-separated
        csv << "\"";
        for (size_t i = 0; i < tags.size(); i++) {
            csv << tags[i];
            if (i < tags.size() - 1) csv << "; ";
        }
        csv << "\",";
        // Platforms as semicolon-separated
        csv << "\"";
        for (size_t i = 0; i < platforms.size(); i++) {
            csv << platforms[i];
            if (i < platforms.size() - 1) csv << "; ";
        }
        csv << "\",";
        csv << "\"" << createdAt << "\"";
        return csv.str();
    }
};

// Function prototypes
std::vector<TestCase> generateCompatibilityCases(const std::string& siteName);
std::vector<TestCase> generateNavigationalCases(const std::string& siteName);
std::vector<TestCase> generateFormBasedCases(const std::string& siteName);
std::vector<TestCase> generateUsabilityCases(const std::string& siteName);
std::vector<TestCase> generateLoadCases(const std::string& siteName);
void writeJSON(const std::vector<TestCase>& cases, const std::string& filename);
void writeCSV(const std::vector<TestCase>& cases, const std::string& filename);

// Main function
int main(int argc, char* argv[]) {
    std::string siteName = "ShoppingSite";
    std::string outputFormat = "json";
    std::string outputFile = "test_cases.json";

    // Simple argument parsing
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--site-name" && i + 1 < argc) {
            siteName = argv[++i];
        } else if (arg == "--output-format" && i + 1 < argc) {
            outputFormat = argv[++i];
        } else if (arg == "--out" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [options]\n";
            std::cout << "Options:\n";
            std::cout << "  --site-name <name>       Name of the shopping site (default: ShoppingSite)\n";
            std::cout << "  --output-format <format> Output format: json or csv (default: json)\n";
            std::cout << "  --out <filename>         Output filename (default: test_cases.json)\n";
            std::cout << "  --help                   Show this help message\n";
            return 0;
        }
    }

    std::cout << "Generating test cases for: " << siteName << "\n";

    // Collect all test cases
    std::vector<TestCase> allCases;

    auto compatCases = generateCompatibilityCases(siteName);
    auto navCases = generateNavigationalCases(siteName);
    auto formCases = generateFormBasedCases(siteName);
    auto usabilityCases = generateUsabilityCases(siteName);
    auto loadCases = generateLoadCases(siteName);

    allCases.insert(allCases.end(), compatCases.begin(), compatCases.end());
    allCases.insert(allCases.end(), navCases.begin(), navCases.end());
    allCases.insert(allCases.end(), formCases.begin(), formCases.end());
    allCases.insert(allCases.end(), usabilityCases.begin(), usabilityCases.end());
    allCases.insert(allCases.end(), loadCases.begin(), loadCases.end());

    // Write output
    if (outputFormat == "json") {
        if (outputFile.find(".json") == std::string::npos) {
            outputFile += ".json";
        }
        writeJSON(allCases, outputFile);
    } else if (outputFormat == "csv") {
        if (outputFile.find(".csv") == std::string::npos) {
            outputFile += ".csv";
        }
        writeCSV(allCases, outputFile);
    }

    std::cout << "Successfully wrote " << allCases.size() << " test cases to " << outputFile << "\n";

    return 0;
}

// Generate compatibility test cases
std::vector<TestCase> generateCompatibilityCases(const std::string& siteName) {
    std::vector<TestCase> cases;

    std::vector<std::string> browsers = {"Chrome", "Firefox", "Safari", "Edge"};
    std::vector<std::string> devices = {"Desktop", "Mobile (iOS)", "Mobile (Android)", "Tablet"};
    std::vector<std::string> pages = {"home", "product-list", "product-page", "cart", "checkout", "account"};
    std::vector<std::string> locales = {"en-US", "es-ES", "fr-FR", "zh-CN"};
    std::vector<std::string> networks = {"WiFi", "4G", "3G", "Slow 2G"};

    int count = 0;
    int maxCombos = 60; // Limit combinations to avoid explosion

    // Browser x Device x Page x Locale combinations
    for (const auto& browser : browsers) {
        for (const auto& device : devices) {
            for (const auto& page : pages) {
                for (const auto& locale : locales) {
                    if (count++ >= maxCombos) break;

                    std::string title = "[Compatibility] " + page + " renders on " + browser + " / " + device + " (" + locale + ")";
                    std::string desc = "Verify that the " + page + " of " + siteName + " renders and basic interactions work on " + browser + " (" + device + ") with locale " + locale + ".";

                    std::vector<std::string> steps = {
                        "Open " + page + " on " + siteName + " using " + browser + " on " + device + " with locale " + locale,
                        "Verify layout and that key UI elements (nav, search, product cards, add-to-cart) are visible",
                        "Perform a basic interaction relevant to the page (e.g., add product to cart on product-page)"
                    };

                    std::string expected = "Page loads without layout break; interactions complete successfully and UI is usable.";

                    std::vector<std::string> tags = {"compatibility", page, browser, device};
                    std::vector<std::string> platforms = {browser, device, locale};

                    cases.push_back(TestCase(title, desc, "Compatibility", steps, expected, "Medium", tags, platforms));
                }
                if (count >= maxCombos) break;
            }
            if (count >= maxCombos) break;
        }
        if (count >= maxCombos) break;
    }

    // Network variation tests
    for (const auto& network : networks) {
        std::string title = "[Compatibility] Checkout completes under " + network;
        std::string desc = "Verify checkout flow completes when network speed is " + network;

        std::vector<std::string> steps = {
            "Open product page and add item to cart",
            "Proceed to checkout and fill minimal required details",
            "Simulate network speed: " + network,
            "Submit order"
        };

        std::string expected = "Checkout completes or displays graceful retry/error messages for the user.";
        std::vector<std::string> tags = {"compatibility", "checkout", network};
        std::vector<std::string> platforms = {"Any", network};

        cases.push_back(TestCase(title, desc, "Compatibility", steps, expected, "High", tags, platforms));
    }

    return cases;
}

// Generate navigational test cases
std::vector<TestCase> generateNavigationalCases(const std::string& siteName) {
    std::vector<TestCase> cases;

    // Main menu navigation
    {
        std::string title = "[Navigational] Main menu navigation";
        std::string desc = "Verify top/main navigation links lead to correct pages and preserve state (cart count, user logged-in state).";
        std::vector<std::string> steps = {
            "Open home page",
            "Click each top navigation link (Shop, Deals, Account, Cart, Help)",
            "Verify the URL and page header match the link clicked",
            "Verify persistent elements (cart count, search) remain correct"
        };
        std::string expected = "Each link navigates to the correct destination; no broken links; persistent UI remains accurate.";
        std::vector<std::string> tags = {"navigation", "smoke"};
        std::vector<std::string> platforms = {"All"};

        cases.push_back(TestCase(title, desc, "Navigational", steps, expected, "High", tags, platforms));
    }

    // Breadcrumbs and back button
    {
        std::string title = "[Navigational] Breadcrumbs & Back button";
        std::string desc = "Verify breadcrumbs reflect current path and back button returns to previous context.";
        std::vector<std::string> steps = {
            "From home, navigate to a category then a product page",
            "Use breadcrumbs to go back one level",
            "Press browser back button"
        };
        std::string expected = "Breadcrumbs show correct path; both methods return to correct previous pages and state.";
        std::vector<std::string> tags = {"navigation", "breadcrumbs"};
        std::vector<std::string> platforms = {"All"};

        cases.push_back(TestCase(title, desc, "Navigational", steps, expected, "Medium", tags, platforms));
    }

    // Search navigation
    {
        std::string title = "[Navigational] Search results navigation and filters";
        std::string desc = "Validate search results and that applying/clearing filters updates results and URL query params.";
        std::vector<std::string> steps = {
            "Open home page and search for 'headphones'",
            "Apply a filter (brand or price range)",
            "Click a product, then use back to return to search results",
            "Clear filters and confirm results update"
        };
        std::string expected = "Search and filters behave consistently, URL reflects filter state, back returns to prior results.";
        std::vector<std::string> tags = {"search", "navigation", "filters"};
        std::vector<std::string> platforms = {"All"};

        cases.push_back(TestCase(title, desc, "Navigational", steps, expected, "High", tags, platforms));
    }

    return cases;
}

// Generate form-based test cases
std::vector<TestCase> generateFormBasedCases(const std::string& siteName) {
    std::vector<TestCase> cases;

    // Signup form validations
    {
        std::string title = "[Form] Signup form validations";
        std::string desc = "Validate client and server-side validation for signup form fields.";
        std::vector<std::string> steps = {
            "Open signup page",
            "Submit with empty required fields",
            "Submit with invalid email, short password, mismatched password confirmation",
            "Submit with already-registered email"
        };
        std::string expected = "Appropriate validation messages shown client-side; server returns proper errors; no security leak of stack traces.";
        std::vector<std::string> tags = {"form", "signup", "validation"};
        std::vector<std::string> platforms = {"All"};

        cases.push_back(TestCase(title, desc, "Form-based", steps, expected, "High", tags, platforms));
    }

    // Checkout address & payment
    {
        std::string title = "[Form] Checkout address & payment validation";
        std::string desc = "Verify address and payment forms validate inputs and recover from errors.";
        std::vector<std::string> steps = {
            "Add item to cart and go to checkout",
            "Enter invalid postal code and incomplete card number",
            "Attempt to submit",
            "Correct inputs and resubmit"
        };
        std::string expected = "Invalid inputs are rejected with helpful messages; correcting allows successful submission.";
        std::vector<std::string> tags = {"form", "checkout", "payment"};
        std::vector<std::string> platforms = {"All"};

        cases.push_back(TestCase(title, desc, "Form-based", steps, expected, "Critical", tags, platforms));
    }

    // Promo code edge cases
    {
        std::string title = "[Form] Promo code application edge cases";
        std::string desc = "Test applying promo codes: expired, not applicable, stacking, uppercase/lowercase handling.";
        std::vector<std::string> steps = {
            "Add item and go to cart",
            "Apply expired promo code, then inapplicable code, then valid code",
            "Try multiple sequential applications"
        };
        std::string expected = "Correct errors for invalid codes; discounts apply only when allowed; UI reflects totals correctly.";
        std::vector<std::string> tags = {"form", "promo"};
        std::vector<std::string> platforms = {"All"};

        cases.push_back(TestCase(title, desc, "Form-based", steps, expected, "Medium", tags, platforms));
    }

    return cases;
}

// Generate usability test cases
std::vector<TestCase> generateUsabilityCases(const std::string& siteName) {
    std::vector<TestCase> cases;

    // First-time user flow
    {
        std::string title = "[Usability] First-time buyer checkout flow";
        std::string desc = "Observe and measure friction for a first-time user buying one item (no account).";
        std::vector<std::string> steps = {
            "Start from home page",
            "Find an item using search or categories",
            "Add to cart and complete checkout as guest",
            "Note any confusing labels, errors, or unexpected steps"
        };
        std::string expected = "User completes flow without confusion; any pain points are documented for improvement.";
        std::vector<std::string> tags = {"usability", "onboarding"};
        std::vector<std::string> platforms = {"All"};

        cases.push_back(TestCase(title, desc, "Usability", steps, expected, "Medium", tags, platforms));
    }

    // Accessibility basics
    {
        std::string title = "[Usability] Basic accessibility checks";
        std::string desc = "Verify keyboard navigation, focus order, and presence of alt text and ARIA on critical pages.";
        std::vector<std::string> steps = {
            "Tab through header, search, product cards, and add-to-cart to ensure logical focus order",
            "Scan for missing alt attributes on product images",
            "Verify color contrast for important CTAs"
        };
        std::string expected = "Keyboard navigation works; important images have alt text; color contrast meets accessibility thresholds.";
        std::vector<std::string> tags = {"usability", "accessibility", "a11y"};
        std::vector<std::string> platforms = {"All"};

        cases.push_back(TestCase(title, desc, "Usability", steps, expected, "High", tags, platforms));
    }

    // Checkout clarity
    {
        std::string title = "[Usability] Checkout clarity & error recovery";
        std::string desc = "Ensure errors in checkout are presented clearly and user can recover without losing data.";
        std::vector<std::string> steps = {
            "Fill checkout with an invalid card and submit",
            "Observe error messages and whether previously entered fields remain",
            "Correct error and complete checkout"
        };
        std::string expected = "Errors are clear and contextual; form remembers previously entered valid data.";
        std::vector<std::string> tags = {"usability", "checkout"};
        std::vector<std::string> platforms = {"All"};

        cases.push_back(TestCase(title, desc, "Usability", steps, expected, "High", tags, platforms));
    }

    return cases;
}

// Generate load test cases
std::vector<TestCase> generateLoadCases(const std::string& siteName) {
    std::vector<TestCase> cases;

    // Baseline load
    {
        std::string title = "[Load] Baseline traffic - 100 concurrent users";
        std::string desc = "Simulate 100 concurrent typical users performing browse -> add-to-cart -> checkout actions to measure latency and error rates.";
        std::vector<std::string> steps = {
            "Create load test script modeling: 70% browse/search, 20% add-to-cart, 10% checkout",
            "Run for 10 minutes with 100 concurrent users",
            "Collect response times, error rates, and CPU/memory usage"
        };
        std::string expected = "Average response time < 2s for browse, error rate < 1%, system remains stable.";
        std::vector<std::string> tags = {"load", "performance"};
        std::vector<std::string> platforms = {"Load test environment"};

        cases.push_back(TestCase(title, desc, "Load", steps, expected, "High", tags, platforms));
    }

    // Spike test
    {
        std::string title = "[Load] Spike test - sudden 5x traffic";
        std::string desc = "Verify system handles sudden spike and shows graceful degradation (queueing, rate-limiting, 503s) rather than crashing.";
        std::vector<std::string> steps = {
            "Ramp from 100 to 500 concurrent users within 1 minute",
            "Sustain for 5 minutes and then drop back to baseline",
            "Monitor system metrics and error behaviour"
        };
        std::string expected = "System maintains partial functionality, errors return meaningful messages; no data loss.";
        std::vector<std::string> tags = {"load", "spike"};
        std::vector<std::string> platforms = {"Load test environment"};

        cases.push_back(TestCase(title, desc, "Load", steps, expected, "Critical", tags, platforms));
    }

    // Endurance test
    {
        std::string title = "[Load] Endurance test - 12 hours steady load";
        std::string desc = "Run steady medium load for 12 hours to catch memory leaks, resource saturation, and degradation.";
        std::vector<std::string> steps = {
            "Run scenario with 200 concurrent users for 12 hours",
            "Monitor memory, DB connections, error rates over time"
        };
        std::string expected = "No resource leaks; performance metrics remain within SLA bounds.";
        std::vector<std::string> tags = {"load", "endurance"};
        std::vector<std::string> platforms = {"Load test environment"};

        cases.push_back(TestCase(title, desc, "Load", steps, expected, "High", tags, platforms));
    }

    return cases;
}

// Write test cases to JSON file
void writeJSON(const std::vector<TestCase>& cases, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    file << "{\n";
    file << "  \"generated_at\": \"" << getCurrentTimestamp() << "\",\n";
    file << "  \"total_cases\": " << cases.size() << ",\n";
    file << "  \"cases\": [\n";

    for (size_t i = 0; i < cases.size(); i++) {
        file << cases[i].toJSON();
        if (i < cases.size() - 1) {
            file << ",";
        }
        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    file.close();
}

// Write test cases to CSV file
void writeCSV(const std::vector<TestCase>& cases, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Write header
    file << "\"id\",\"title\",\"description\",\"technique\",\"steps\",\"expected_result\",\"priority\",\"tags\",\"platforms\",\"created_at\"\n";

    // Write test cases
    for (const auto& testCase : cases) {
        file << testCase.toCSV() << "\n";
    }

    file.close();
}