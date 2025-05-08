#include <iostream>
#include <string>
#include <vector>        // Added to maintain conversation history
#include <tuple>         // Added for return type of sendMessageWithRetries
#include <curl/curl.h>
#include "json.hpp"
#include <chrono>         // Added to measure API call times
#include <thread>         // Added for retry delays

using json = nlohmann::json;
using namespace std;
using Clock = std::chrono::high_resolution_clock;  // Alias for timing

// cURL write callback (unchanged from original)
typedef size_t write_callback_t(void*, size_t, size_t, string*);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* out) {
    size_t totalSize = size * nmemb;
    out->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Function to fetch current time in Italy (unchanged logic)
string getTimeInItaly() {
    string response;
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://worldtimeapi.org/api/timezone/Europe/Rome");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem"); // Path to certificate bundle
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "cURL error: " << curl_easy_strerror(res) << endl;
        }
        curl_easy_cleanup(curl);
    }
    if (response.empty()) {
        return "Error: Could not fetch the time. Please try again later.";
    }
    try {
        auto j = json::parse(response);
        return j.value("datetime", "Error: Unexpected response format.");
    } catch (...) {
        return "Error: Failed to parse response from WorldTimeAPI.";
    }
}

// Recursive API call with retry logic and performance measurement
// NEW: wraps original sendMessageToChatbot and measures time
tuple<string, double> sendMessageWithRetries(
    const string& userMsg,
    const string& apiKey,
    int retries = 3,
    int delayMs = 500
) {
    string response;
    double elapsedMs = 0;
    CURL* curl = curl_easy_init();
    if (curl) {
        const string url = "https://api.openai.com/v1/chat/completions";
        const string payload = R"({"model":"gpt-3.5-turbo","messages":[{"role":"user","content":)")"
                              + userMsg
                              + R"("}]})";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

        // NEW: start timing
        auto start = Clock::now();
        CURLcode res = curl_easy_perform(curl);
        auto end = Clock::now();
        elapsedMs = chrono::duration<double, std::milli>(end - start).count();

        // Handle errors with recursion
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            if (retries-- > 0) {
                this_thread::sleep_for(chrono::milliseconds(delayMs));
                return sendMessageWithRetries(userMsg, apiKey, retries, delayMs);
            }
            cerr << "Failed after retries: " << curl_easy_strerror(res) << endl;
            return make_tuple(string(""), elapsedMs);
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return make_tuple(response, elapsedMs);  // Return raw JSON and elapsed time
}

int main() {
    // NEW: prompt for API key rather than hardcoding it
    cout << "Enter your OpenAI API key: ";
    string apiKey;
    getline(cin, apiKey);

    string userName = "User", botName = "Assistant";
    vector<pair<string, string>> history;  // NEW: store conversation history
    int iterationCount = 0;                // NEW: count exchanges
    double totalResponseTime = 0;          // NEW: accumulate times for average

    cout << "Chatbot (type 'exit' to quit):\n";
    while (true) {
        // NEW: input validation loop
        string userInput;
        do {
            cout << userName << ": ";
            getline(cin, userInput);
            if (userInput == "exit") break;
            if (userInput.empty()) {
                cout << "Error: input cannot be empty. Please try again." << endl;
            } else if (userInput.size() > 1000) {
                cout << "Error: input too long (max 1000 chars)." << endl;
                userInput.clear();
            }
        } while (userInput.empty());
        if (userInput == "exit") break;

        // Preserve original name-change commands with new history logic
        if (userInput.rfind("my name is ", 0) == 0) {
            userName = userInput.substr(11);
            cout << botName << ": Nice to meet you, " << userName << "!" << endl;
            continue;
        }
        if (userInput.rfind("Your name is now ", 0) == 0) {
            botName = userInput.substr(18);
            cout << botName << ": Got it—I'll call myself " << botName << "." << endl;
            continue;
        }
        if (userInput.find("time in Italy") != string::npos) {
            string t = getTimeInItaly();
            cout << botName << ": The current time in Italy is " << t << endl;
            history.emplace_back(userInput, t);  // Save to history
            continue;
        }

        // NEW: call recursive API function and measure time
        auto [rawResponse, respMs] = sendMessageWithRetries(userInput, apiKey);
        totalResponseTime += respMs;
        iterationCount++;
        cout << "[Response time: " << respMs
             << " ms | Avg: " << (totalResponseTime / iterationCount)
             << " ms]" << endl;

        // Parse JSON and extract content (unchanged from original)
        string botReply;
        try {
            auto j = json::parse(rawResponse);
            botReply = j["choices"][0]["message"]["content"].get<string>();
        } catch (...) {
            botReply = "Sorry, I couldn't parse the response.";
        }
        cout << botName << ": " << botReply << endl;

        // NEW: record and display conversation history
        history.emplace_back(userInput, botReply);
        cout << "\n--- Conversation (#" << iterationCount << ") ---\n";
        for (size_t i = 0; i < history.size(); ++i) {
            cout << "[" << (i+1) << "] " << userName << ": " << history[i].first << endl;
            cout << "     " << botName << ": " << history[i].second << endl;
        }
        cout << "-----------------------------\n";
    }

    cout << "Goodbye!" << endl;
    return 0;
}

