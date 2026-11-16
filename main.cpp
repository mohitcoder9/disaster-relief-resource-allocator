#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <fstream>
using namespace std;

// ==========================================
// DISASTER AREA CLASS
// ==========================================
class DisasterArea
{
public:
    string areaName, disasterType;
    int severity, population;

    int calculatePriority() const
    {
        return severity * 10 + population / 100;
    }

    void displayArea() const
    {
        cout << "\nArea Name  : " << areaName << endl;
        cout << "Disaster   : " << disasterType << endl;
        cout << "Severity   : " << severity << "/10" << endl;
        cout << "Population : " << population << endl;
        cout << "Priority   : " << calculatePriority() << endl;
    }
};

// ==========================================
// LINKED LIST FOR DISASTER AREAS
// ==========================================
struct AreaNode
{
    DisasterArea area;
    AreaNode* next;

    AreaNode(DisasterArea a)
    {
        area = a;
        next = NULL;
    }
};

class DisasterList
{
private:
    AreaNode* head;

public:
    DisasterList()
    {
        head = NULL;
    }

    void addArea(DisasterArea area)
    {
        AreaNode* node = new AreaNode(area);

        if (head == NULL)
            head = node;
        else
        {
            AreaNode* temp = head;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = node;
        }
    }

    void displayAll()
    {
        if (head == NULL)
        {
            cout << "No disaster areas registered.\n";
            return;
        }

        AreaNode* temp = head;

        while (temp != NULL)
        {
            temp->area.displayArea();
            temp = temp->next;
        }
    }

    // Build a linked-list priority queue
    void buildPriorityQueue(class PriorityQueue& pq);

    void saveAreas()
    {
        ofstream file("disaster_areas.txt");
        AreaNode* temp = head;

        while (temp != NULL)
        {
            file << temp->area.areaName << endl;
            file << temp->area.disasterType << endl;
            file << temp->area.severity << endl;
            file << temp->area.population << endl;

            temp = temp->next;
        }
    }

    void loadAreas()
    {
        ifstream file("disaster_areas.txt");
        DisasterArea area;

        while (getline(file, area.areaName))
        {
            if (!getline(file, area.disasterType))
                break;

            if (!(file >> area.severity >> area.population))
                break;

            file.ignore(10000, '\n');

            addArea(area);
        }
    }

    ~DisasterList()
    {
        while (head != NULL)
        {
            AreaNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// ==========================================
// CUSTOM PRIORITY QUEUE USING LINKED LIST
// No vector is used
// ==========================================
struct PriorityNode
{
    DisasterArea area;
    PriorityNode* next;

    PriorityNode(DisasterArea a)
    {
        area = a;
        next = NULL;
    }
};

class PriorityQueue
{
private:
    PriorityNode* front;

public:
    PriorityQueue()
    {
        front = NULL;
    }

    bool empty() const
    {
        return front == NULL;
    }

    void push(DisasterArea area)
    {
        PriorityNode* node = new PriorityNode(area);

        // Insert higher priority before lower priority
        if (front == NULL ||
            area.calculatePriority() >
            front->area.calculatePriority())
        {
            node->next = front;
            front = node;
            return;
        }

        PriorityNode* temp = front;

        while (temp->next != NULL &&
               temp->next->area.calculatePriority() >=
               area.calculatePriority())
        {
            temp = temp->next;
        }

        node->next = temp->next;
        temp->next = node;
    }

    DisasterArea top() const
    {
        return front->area;
    }

    void pop()
    {
        if (front == NULL)
            return;

        PriorityNode* temp = front;
        front = front->next;
        delete temp;
    }

    ~PriorityQueue()
    {
        while (front != NULL)
            pop();
    }
};

void DisasterList::buildPriorityQueue(PriorityQueue& pq)
{
    AreaNode* temp = head;

    while (temp != NULL)
    {
        pq.push(temp->area);
        temp = temp->next;
    }
}

// ==========================================
// INVENTORY FILE HANDLING
// ==========================================
void saveInventory(int food, int medicine, int water,
                   int blankets, int tents, int clothes,
                   int firstAid)
{
    ofstream file("inventory.txt");

    file << food << endl << medicine << endl << water << endl;
    file << blankets << endl << tents << endl;
    file << clothes << endl << firstAid << endl;
}

void loadInventory(int& food, int& medicine, int& water,
                   int& blankets, int& tents, int& clothes,
                   int& firstAid)
{
    ifstream file("inventory.txt");

    if (!(file >> food >> medicine >> water))
    {
        food = 300;
        medicine = 150;
        water = 1500;
        blankets = 200;
        tents = 50;
        clothes = 200;
        firstAid = 100;
    }
    else
    {
        if (!(file >> blankets)) blankets = 200;
        if (!(file >> tents)) tents = 50;
        if (!(file >> clothes)) clothes = 200;
        if (!(file >> firstAid)) firstAid = 100;
    }

    file.close();

    saveInventory(food, medicine, water,
                  blankets, tents, clothes, firstAid);
}

// ==========================================
// DISPLAY INVENTORY
// ==========================================
void displayInventory(int food, int medicine, int water,
                      int blankets, int tents, int clothes,
                      int firstAid)
{
    cout << "\n======= RESOURCE INVENTORY =======\n";
    cout << "Food Kits      : " << food << endl;
    cout << "Medical Kits   : " << medicine << endl;
    cout << "Water          : " << water << " liters\n";
    cout << "Blankets       : " << blankets << endl;
    cout << "Tents          : " << tents << endl;
    cout << "Clothes        : " << clothes << endl;
    cout << "First-Aid Kits : " << firstAid << endl;
}

// ==========================================
// UPDATE INVENTORY
// ==========================================
void updateInventory(int& food, int& medicine, int& water,
                     int& blankets, int& tents, int& clothes,
                     int& firstAid)
{
    int resource, operation, quantity;

    int* stock[] = {
        &food, &medicine, &water, &blankets,
        &tents, &clothes, &firstAid
    };

    cout << "\n1. Food Kits\n2. Medical Kits\n3. Water\n";
    cout << "4. Blankets\n5. Tents\n6. Clothes\n";
    cout << "7. First-Aid Kits\n";
    cout << "Select resource: ";
    cin >> resource;

    if (resource < 1 || resource > 7)
    {
        cout << "Invalid resource.\n";
        return;
    }

    cout << "1. Add stock\n2. Set total quantity\n";
    cout << "Choose operation: ";
    cin >> operation;

    if (operation != 1 && operation != 2)
    {
        cout << "Invalid operation.\n";
        return;
    }

    cout << "Current quantity: "
         << *stock[resource - 1] << endl;
    cout << "Enter quantity: ";
    cin >> quantity;

    if (quantity < 0 ||
        (operation == 1 &&
         *stock[resource - 1] > 2147483647 - quantity))
    {
        cout << "Invalid quantity.\n";
        return;
    }

    if (operation == 1)
        *stock[resource - 1] += quantity;
    else
        *stock[resource - 1] = quantity;

    saveInventory(food, medicine, water,
                  blankets, tents, clothes, firstAid);

    cout << "Inventory updated successfully.\n";
}

// ==========================================
// STACK: ALLOCATION HISTORY
// ==========================================
void saveHistory(stack<string> history)
{
    ofstream file("history.txt");

    // Save newest to oldest
    while (!history.empty())
    {
        file << history.top() << endl;
        history.pop();
    }
}

void loadHistory(stack<string>& history)
{
    ifstream file("history.txt");
    stack<string> temporary;
    string record;

    // Reverse saved order to restore the newest on top
    while (getline(file, record))
        temporary.push(record);

    while (!temporary.empty())
    {
        history.push(temporary.top());
        temporary.pop();
    }
}

void displayHistory(stack<string> history)
{
    if (history.empty())
    {
        cout << "No allocation history available.\n";
        return;
    }

    cout << "\n======= ALLOCATION HISTORY =======\n";

    while (!history.empty())
    {
        cout << history.top() << endl;
        history.pop();
    }
}

// ==========================================
// ALLOCATE RESOURCES BASED ON SEVERITY
// ==========================================
void allocateResources(
    int& food, int& medicine, int& water,
    int& blankets, int& tents, int& clothes,
    int& firstAid, stack<string>& history,
    DisasterArea area)
{
    int f, m, w, b, t, c, a;

    // Severity-based relief packages
    if (area.severity <= 3)
    {
        f = 50;  m = 25;  w = 250;
        b = 25;  t = 5;   c = 25;  a = 10;
    }
    else if (area.severity <= 6)
    {
        f = 100; m = 50;  w = 500;
        b = 50;  t = 10;  c = 50;  a = 25;
    }
    else if (area.severity <= 8)
    {
        f = 150; m = 75;  w = 750;
        b = 75;  t = 15;  c = 75;  a = 35;
    }
    else
    {
        f = 200; m = 100; w = 1000;
        b = 100; t = 20;  c = 100; a = 50;
    }

    cout << "\n======= PROPOSED RELIEF PACKAGE =======\n";
    cout << "Area           : " << area.areaName << endl;
    cout << "Severity       : " << area.severity << "/10\n";
    cout << "Food Kits      : " << f << endl;
    cout << "Medical Kits   : " << m << endl;
    cout << "Water          : " << w << " liters\n";
    cout << "Blankets       : " << b << endl;
    cout << "Tents          : " << t << endl;
    cout << "Clothes        : " << c << endl;
    cout << "First-Aid Kits : " << a << endl;

    if (food < f || medicine < m || water < w ||
        blankets < b || tents < t ||
        clothes < c || firstAid < a)
    {
        cout << "\nInsufficient inventory for this package.\n";
        cout << "No resources were deducted.\n";
        return;
    }

    food -= f;
    medicine -= m;
    water -= w;
    blankets -= b;
    tents -= t;
    clothes -= c;
    firstAid -= a;

    saveInventory(food, medicine, water,
                  blankets, tents, clothes, firstAid);

    // Save area and ALL allocated resource quantities
    string record =
        "Area: " + area.areaName +
        " | Severity: " + to_string(area.severity) +
        " | Food: " + to_string(f) +
        " | Medical: " + to_string(m) +
        " | Water: " + to_string(w) + " liters" +
        " | Blankets: " + to_string(b) +
        " | Tents: " + to_string(t) +
        " | Clothes: " + to_string(c) +
        " | First-Aid: " + to_string(a);

    history.push(record);
    saveHistory(history);

    cout << "\nResources allocated successfully!\n";
    cout << "Full allocation details saved in history.\n";
}

// ==========================================
// QUEUE: EMERGENCY CALLS
// ==========================================
void saveEmergencyCalls(queue<string> calls)
{
    ofstream file("emergency_calls.txt");

    while (!calls.empty())
    {
        file << calls.front() << endl;
        calls.pop();
    }
}

void loadEmergencyCalls(queue<string>& calls)
{
    ifstream file("emergency_calls.txt");
    string call;

    while (getline(file, call))
        calls.push(call);
}

// ==========================================
// MAIN PROGRAM
// ==========================================
int main()
{
    int food, medicine, water;
    int blankets, tents, clothes, firstAid;
    int choice;

    loadInventory(food, medicine, water,
                  blankets, tents, clothes, firstAid);

    DisasterList areas;
    areas.loadAreas();

    queue<string> emergencyCalls;
    loadEmergencyCalls(emergencyCalls);

    stack<string> history;
    loadHistory(history);

    do
    {
        cout << "\n===== DISASTER RELIEF RESOURCE ALLOCATOR =====\n";
        cout << "1. Register Disaster Area\n";
        cout << "2. Display All Disaster Areas\n";
        cout << "3. Show Priority Order\n";
        cout << "4. Add Emergency Call\n";
        cout << "5. Process Emergency Call\n";
        cout << "6. Allocate Resources by Severity\n";
        cout << "7. Check Inventory\n";
        cout << "8. Update Inventory\n";
        cout << "9. View Allocation History\n";
        cout << "10. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            DisasterArea area;
            cin.ignore(10000, '\n');

            cout << "Enter Area Name: ";
            getline(cin, area.areaName);

            cout << "Enter Disaster Type: ";
            getline(cin, area.disasterType);

            cout << "Enter Severity (1-10): ";
            cin >> area.severity;

            cout << "Enter Affected Population: ";
            cin >> area.population;

            if (area.areaName.empty() ||
                area.disasterType.empty() ||
                area.severity < 1 || area.severity > 10 ||
                area.population < 0)
            {
                cout << "Invalid input. Area not registered.\n";
                break;
            }

            areas.addArea(area);
            areas.saveAreas();

            cout << "Disaster area registered successfully!\n";
            cout << "Priority Score: "
                 << area.calculatePriority() << endl;
            break;
        }

        case 2:
            areas.displayAll();
            break;

        case 3:
        {
            PriorityQueue pq;
            areas.buildPriorityQueue(pq);

            if (pq.empty())
            {
                cout << "No disaster areas registered.\n";
                break;
            }

            cout << "\n======= PRIORITY ORDER =======\n";

            while (!pq.empty())
            {
                pq.top().displayArea();
                pq.pop();
            }
            break;
        }

        case 4:
        {
            cin.ignore(10000, '\n');
            string call;

            cout << "Enter Emergency Call Details: ";
            getline(cin, call);

            if (call.empty())
            {
                cout << "Call details cannot be empty.\n";
                break;
            }

            emergencyCalls.push(call);
            saveEmergencyCalls(emergencyCalls);

            cout << "Emergency call added successfully.\n";
            break;
        }

        case 5:
            if (emergencyCalls.empty())
            {
                cout << "No emergency calls waiting.\n";
            }
            else
            {
                cout << "Processing: "
                     << emergencyCalls.front() << endl;

                emergencyCalls.pop();
                saveEmergencyCalls(emergencyCalls);

                cout << "Emergency call processed.\n";
            }
            break;

        case 6:
        {
            PriorityQueue pq;
            areas.buildPriorityQueue(pq);

            if (pq.empty())
            {
                cout << "Register a disaster area first.\n";
                break;
            }

            cout << "\nSelect allocation method:\n";
            cout << "1. Highest-priority area\n";
            cout << "2. Enter area name\n";
            cout << "Choice: ";

            int method;
            cin >> method;

            DisasterArea selected;
            bool found = false;

            if (method == 1)
            {
                selected = pq.top();
                found = true;
            }
            else if (method == 2)
            {
                string name;
                cin.ignore(10000, '\n');

                cout << "Enter registered area name: ";
                getline(cin, name);

                // Search the priority queue in priority order
                while (!pq.empty())
                {
                    DisasterArea current = pq.top();
                    pq.pop();

                    if (current.areaName == name)
                    {
                        selected = current;
                        found = true;
                        break;
                    }
                }
            }
            else
            {
                cout << "Invalid allocation method.\n";
                break;
            }

            if (!found)
            {
                cout << "Area not found.\n";
                break;
            }

            allocateResources(
                food, medicine, water, blankets,
                tents, clothes, firstAid, history,
                selected);

            break;
        }

        case 7:
            displayInventory(food, medicine, water,
                             blankets, tents, clothes, firstAid);
            break;

        case 8:
            updateInventory(food, medicine, water,
                            blankets, tents, clothes, firstAid);
            break;

        case 9:
            displayHistory(history);
            break;

        case 10:
            cout << "Exiting system. Thank you!\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 10);

    return 0;
}
