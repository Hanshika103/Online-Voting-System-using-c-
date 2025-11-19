#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

class Candidate {
public:
    string name;
    int votes;
    Candidate(const string& n = "") : name(n), votes(0) {}
};

class Voter {
public:
    string id;
    bool hasVoted;
    Voter(const string& vid = "") : id(vid), hasVoted(false) {}
};

class VotingSystem {
private:
    vector<Candidate> candidates;
    vector<Voter> voters;
    const string adminPassword = "admin123";

    // Helper to read an integer safely
    int readInt() {
        int x;
        while (!(cin >> x)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // consume leftover
        return x;
    }

    // Helper to read a non-empty line
    string readLineNonEmpty(const string &prompt) {
        string s;
        do {
            cout << prompt;
            getline(cin, s);
            if (s.empty()) cout << "Input cannot be empty. Try again.\n";
        } while (s.empty());
        return s;
    }

    Voter* findVoter(const string& id) {
        for (auto &v : voters) {
            if (v.id == id) return &v;
        }
        return nullptr;
    }

public:
    void adminPanel() {
        cout << "\n--- Admin Panel ---\n";
        cout << "1. Add Candidate\n";
        cout << "2. Remove Candidate\n";
        cout << "3. List Candidates\n";
        cout << "4. View Results\n";
        cout << "5. Back to Main Menu\n";
        cout << "Choose option: ";
        int opt = readInt();

        switch (opt) {
            case 1: {
                string name = readLineNonEmpty("Enter candidate name: ");
                candidates.emplace_back(name);
                cout << "Candidate \"" << name << "\" added.\n";
                break;
            }
            case 2: {
                if (candidates.empty()) { cout << "No candidates to remove.\n"; break; }
                cout << "Candidates:\n";
                for (size_t i = 0; i < candidates.size(); ++i)
                    cout << i+1 << ". " << candidates[i].name << "\n";
                cout << "Enter candidate number to remove: ";
                int idx = readInt();
                if (idx >= 1 && idx <= (int)candidates.size()) {
                    cout << "Removed \"" << candidates[idx-1].name << "\".\n";
                    candidates.erase(candidates.begin() + (idx-1));
                } else cout << "Invalid index.\n";
                break;
            }
            case 3: {
                if (candidates.empty()) cout << "No candidates added yet.\n";
                else {
                    cout << "Candidates:\n";
                    for (size_t i = 0; i < candidates.size(); ++i)
                        cout << i+1 << ". " << candidates[i].name << "\n";
                }
                break;
            }
            case 4:
                showResults();
                break;
            case 5:
                return;
            default:
                cout << "Invalid option.\n";
        }
    }

    void showResults() {
        cout << "\n===== Current Results =====\n";
        if (candidates.empty()) {
            cout << "No candidates.\n";
            return;
        }
        for (const auto &c : candidates)
            cout << c.name << " : " << c.votes << " vote(s)\n";

        // Optionally display winner(s)
        int maxVotes = -1;
        for (const auto &c : candidates) if (c.votes > maxVotes) maxVotes = c.votes;
        if (maxVotes <= 0) {
            cout << "No votes cast yet.\n";
            return;
        }
        cout << "Leading candidate(s): ";
        bool first = true;
        for (const auto &c : candidates) {
            if (c.votes == maxVotes) {
                if (!first) cout << ", ";
                cout << c.name;
                first = false;
            }
        }
        cout << "\n";
    }

    void adminLoginAndPanel() {
        cout << "Enter admin password: ";
        string pass;
        // read password line safely
        getline(cin, pass);
        if (pass == adminPassword) {
            cout << "Admin login successful.\n";
            // keep admin panel until they choose to go back
            while (true) {
                adminPanel();
                cout << "Return to admin panel? (y/n): ";
                string ans;
                getline(cin, ans);
                if (ans.empty() || ans[0]=='n' || ans[0]=='N') break;
            }
        } else {
            cout << "Incorrect password.\n";
        }
    }

    void registerVoter() {
        string id = readLineNonEmpty("Enter new Voter ID: ");
        if (findVoter(id)) {
            cout << "Voter ID already registered.\n";
            return;
        }
        voters.emplace_back(id);
        cout << "Voter \"" << id << "\" registered successfully.\n";
    }

    void castVote() {
        if (candidates.empty()) {
            cout << "No candidates available. Ask admin to add candidates first.\n";
            return;
        }
        string id = readLineNonEmpty("Enter your Voter ID: ");
        Voter* v = findVoter(id);
        if (!v) { cout << "Voter not registered. Please register first.\n"; return; }
        if (v->hasVoted) { cout << "You have already voted. Duplicate voting is not allowed.\n"; return; }

        cout << "Candidates:\n";
        for (size_t i = 0; i < candidates.size(); ++i)
            cout << i+1 << ". " << candidates[i].name << "\n";

        cout << "Enter candidate number to vote for: ";
        int choice = readInt();
        if (choice < 1 || choice > (int)candidates.size()) {
            cout << "Invalid candidate selection. Vote cancelled.\n";
            return;
        }
        candidates[choice-1].votes++;
        v->hasVoted = true;
        cout << "Vote cast successfully for \"" << candidates[choice-1].name << "\".\n";
    }

    void mainMenu() {
        while (true) {
            cout << "\n==== ONLINE VOTING SYSTEM ====\n";
            cout << "1. Admin Login\n";
            cout << "2. Register Voter\n";
            cout << "3. Cast Vote\n";
            cout << "4. View Results\n";
            cout << "5. Exit\n";
            cout << "Enter choice: ";
            int ch = readInt();

            switch (ch) {
                case 1: adminLoginAndPanel(); break;
                case 2: registerVoter(); break;
                case 3: castVote(); break;
                case 4: showResults(); break;
                case 5:
                    cout << "Exiting. Goodbye.\n";
                    return;
                default:
                    cout << "Invalid option. Try again.\n";
            }
        }
    }
};

int main() {
    VotingSystem vs;
    vs.mainMenu();
    return 0;
}
