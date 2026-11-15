
#include <iostream>
#include <string>
#include <limits>
using namespace std;

const int CAPACITY = 5;

class Node
{
public:
    string key;
    string value;
    int frequency;  
    int lastUsed;    
    Node *prev;
    Node *next;

    Node(string k, string v, int time)
    {
        key = k;
        value = v;
        frequency = 1;
        lastUsed = time;
        prev = NULL;
        next = NULL;
    }
};

class Cache
{
private:
    Node *head;
    Node *tail;
    int count;
    int hits;
    int misses;
    int clock;   

    Node* find(const string &key)
    {
        for (Node *cur = head; cur != NULL; cur = cur->next)
        {
            if (cur->key == key)
                return cur;
        }
        return NULL;
    }

    void addFront(Node *n)
    {
        n->next = head;
        if (head != NULL)
            head->prev = n;
        else
            tail = n;
        head = n;
        count++;
    }

    void removeNode(Node *n)
    {
        if (n->prev != NULL)
            n->prev->next = n->next;
        else
            head = n->next;

        if (n->next != NULL)
            n->next->prev = n->prev;
        else
            tail = n->prev;

        delete n;
        count--;
    }

    Node* pickVictim()
    {
        Node *victim = head;
        for (Node *cur = head; cur != NULL; cur = cur->next)
        {
            if (cur->frequency < victim->frequency ||
               (cur->frequency == victim->frequency && cur->lastUsed < victim->lastUsed))
            {
                victim = cur;
            }
        }
        return victim;
    }

    void evict()
    {
        Node *victim = pickVictim();
        cout << "Cache full\n";
        cout << "Evicted key: " << victim->key << "\n";
        removeNode(victim);
    }

public:
    Cache()
    {
        head = NULL;
        tail = NULL;
        count = 0;
        hits = 0;
        misses = 0;
        clock = 0;
    }

    ~Cache()
    {
        while (head != NULL)
            removeNode(head);
    }

    void set(const string &key, const string &value)
    {
        clock++;
        Node *found = find(key);

        if (found != NULL)
        {
            found->value = value;
            found->frequency++;
            found->lastUsed = clock;
            cout << "Value updated successfully\n";
            return;
        }

        if (count >= CAPACITY)
            evict();

        addFront(new Node(key, value, clock));
        cout << "Key inserted successfully\n";
    }

    void get(const string &key)
    {
        clock++;
        Node *found = find(key);

        if (found == NULL)
        {
            misses++;
            cout << "Key not found\n";
            return;
        }

        hits++;
        found->frequency++;
        found->lastUsed = clock;
        cout << "Value: " << found->value << "\n";
    }

    void del(const string &key)
    {
        Node *found = find(key);

        if (found == NULL)
        {
            cout << "Key not found\n";
            return;
        }

        removeNode(found);
        cout << "Key deleted successfully\n";
    }

    void display()
    {
        if (head == NULL)
        {
            cout << "Cache is empty\n";
            return;
        }

        cout << "\nCache Contents:\n";
        for (Node *cur = head; cur != NULL; cur = cur->next)
        {
            cout << "Key: " << cur->key
                 << " Value: " << cur->value
                 << " Frequency: " << cur->frequency
                 << " Last Used: " << cur->lastUsed << "\n";
        }
        cout << "Total entries: " << count << "\n";
    }

    void stats()
    {
        int total = hits + misses;

        cout << "\nCache Statistics:\n";
        cout << "Hits: " << hits << "\n";
        cout << "Misses: " << misses << "\n";
        cout << "Total entries: " << count << "\n";

        if (total > 0)
        {
            cout << "Hit Ratio: " << hits * 100.0 / total << "%\n";
            cout << "Miss Ratio: " << misses * 100.0 / total << "%\n";
        }
        else
        {
            cout << "No GET requests yet\n";
        }
    }
};

void showMenu()
{
    cout << "\nKeyFlux Menu:\n";
    cout << "1. SET\n";
    cout << "2. GET\n";
    cout << "3. DELETE\n";
    cout << "4. DISPLAY\n";
    cout << "5. STATISTICS\n";
    cout << "6. EXIT\n";
    cout << "Enter choice: ";
}
string askKey()
{
    string key;
    cout << "Enter key: ";
    cin >> key;
    return key;
}

int main()
{
    Cache cache;
    int choice = 0;

    do
    {
        showMenu();

        if (!(cin >> choice))
        {
            // user typed a letter, clear the error and throw the line away
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice\n";
            choice = 0;
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                string key = askKey();
                string value;
                cout << "Enter value: ";
                cin.ignore();           
                getline(cin, value);  
                cache.set(key, value);
                break;
            }
            case 2:
                cache.get(askKey());
                break;
            case 3:
                cache.del(askKey());
                break;
            case 4:
                cache.display();
                break;
            case 5:
                cache.stats();
                break;
            case 6:
                cout << "Exiting KeyFlux\n";
                break;
            default:
                cout << "Invalid choice\n";
        }
    } while (choice != 6);

    return 0;
}
