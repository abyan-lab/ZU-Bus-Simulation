#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
using namespace std;

class TicketQueue{ 
    queue<string> q;
    public:
    void bookTicket(string Name){  //booking ticket 
        q.push(Name);
        cout<<"Ticket booked for: "<<Name<<endl;
    }
    void displayQueue(){  //display booked ticket
        if (q.empty()){cout<<"Queue empty. \n"; return;}
        queue<string> temp = q;
        int pos = 1;
        while(!temp.empty()){
            cout<<pos++<<". "<<temp.front()<<endl;
            temp.pop();
        }
    }
};

struct Passenger {  
    string Name, TicketType, Destination;
    int Age;
    Passenger* next;
    Passenger(string n, int a, string t, string d): Name(n), Age(a), TicketType(t), Destination(d), next(nullptr){}
};
class PassengerList {  
    Passenger* head;
public:
    PassengerList(): head(nullptr) {}
    void addPassenger(string Name, int Age, string TicketType, string Destination) {  
        Passenger* newPassenger = new Passenger(Name, Age, TicketType, Destination);
        if(!head){
            head = newPassenger;
        } else {
            Passenger* temp = head;
            while(temp->next){
                temp = temp->next;
            }
            temp->next = newPassenger;
        }
        cout<<"Passenger Added!\n";
    }
    void displayAll(){
        if(head == nullptr){
            cout<<"No passenger List is Available"<<endl;
            return;
        }
        Passenger* temp = head;
        int count = 1;
        cout<<"\n === PASSENGER LIST ===\n";
        while(temp != nullptr){
            cout << count << ". Name: " << temp->Name 
                 << ", Age: " << temp->Age 
                 << ", Ticket: " << temp->TicketType 
                 << ", To: " << temp->Destination << endl;
            temp = temp->next;
            count++;
        }
    }
    Passenger* getHead(){
        return head;
    }
};

class MetroGraph{
    map<string, vector<string> > stations;
    public:
    void addRoute(string s1, string s2){
        stations[s1].push_back(s2);
        stations[s2].push_back(s1);
    }
    vector<string> findShortestRoute(string start, string end){
        map<string, bool> visited;
        map<string, string> parent;
        queue<string>q;
        q.push(start);
        visited[start] = true;
        parent[start] = "";

        while (!q.empty()){
            string current = q.front();
            q.pop();
            if(current == end){
                vector<string> path;
                for(string at = end; !at.empty(); at = parent[at])
                    path.push_back(at);
                reverse(path.begin(), path.end());
                return path;
            }
            for(string neighbor : stations[current]){
                if(!visited[neighbor]){
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
        return{};
    }
};

class DisplayManager{
    public:
    static void showAllPassengers(PassengerList& list){
        cout<<"\n=== ALL PASSENGER ===\n";
        list.displayAll();
    }
};

class FileHandler {
public:
    static void saveToFile(PassengerList& list) {
        ofstream file("records.txt");
        Passenger* current = list.getHead();
        while (current) {
            file << current->Name << "," << current->Age << "," 
                 << current->TicketType << "," << current->Destination << "\n";
            current = current->next;
        }
        cout << "All records saved to file!\n";
    }
};

int main(){
    TicketQueue ticketQueue;  
    PassengerList passengers;                   
    MetroGraph graph;                  
    DisplayManager display;           
    // Setup routes
    graph.addRoute("Lahore Adda", "Gulbahar Chowk");
    graph.addRoute("Gulbahar Chowk", "Hastnagri");
    graph.addRoute("Hastnagri", "Malik Saad Shaheed");
    graph.addRoute("Malik Saad Shaheed", "Khyber Bazar");
    graph.addRoute("Khyber Bazar", "Shuba Bazar");
    graph.addRoute("Shuba Bazar", "Dabgari Garden");
    
    int choice;
    do {
        cout << "\n=== ZU METRO SIMULATION ===\n";
        cout << "1. Book Ticket \n";
        cout << "2. Add Passenger\n";
        cout << "3. Display Passengers \n";
        cout << "4. Find Route \n";
        cout << "5. Save to File \n";
        cout << "6. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1: {
                string name;
                cout << "Enter name: ";
                getline(cin, name);
                ticketQueue.bookTicket(name);
                break;
            }
            case 2: {
                string name, ticketType, destination;
                int age;
                cout << "Name: "; getline(cin, name);
                cout << "Age: "; cin >> age;
                cin.ignore();
                cout << "Ticket type: "; getline(cin, ticketType);
                cout << "Destination: "; getline(cin, destination);
                passengers.addPassenger(name, age, ticketType, destination);
                break;
            }
            case 3:
                display.showAllPassengers(passengers);
                break;
            case 4: {
                string start, end;
                cout << "Start: "; getline(cin, start);
                cout << "End: "; getline(cin, end);
                vector<string> route = graph.findShortestRoute(start, end);
                if (!route.empty()) {
                    cout << "Shortest Route: ";
                    for (size_t i = 0; i < route.size(); i++) {
                        cout << route[i];
                        if (i != route.size()-1) cout << " -> ";
                    }
                    cout << endl;
                } else {
                    cout << "No route found!\n";
                }
                break;
            }
            case 5:
                FileHandler::saveToFile(passengers);
                break;
        }
    } while (choice != 6);   
    return 0;
}