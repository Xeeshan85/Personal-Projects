#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *next;
};

class Queue
{
    Node *front;
    Node *rear;
    int length;

public:
    Queue() : front(NULL), rear(NULL), length(0) {}
    bool isEmpty()
    {
        return front == NULL;
    }

    void push(int val)
    {
        length++;
        Node *newNode = new Node;
        newNode->data = val;
        newNode->next = NULL;
        if (isEmpty())
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }

    void pop()
    {
        length--;
        if (isEmpty())
        {
            cout << "Queue is empty. Cannot pop." << endl;
            return;
        }
        Node *temp = front;
        front = front->next;
        delete temp;
        temp = NULL;
        if (front == NULL)
        {
            rear = NULL;
        }
    }

    bool empty()
    {
        return front == NULL;
    }

    int peek()
    {
        if (isEmpty())
        {
            cout << "Queue is empty." << endl;
            return -1;
        }
        return front->data;
    }

    int getLength() { return length; }
};

struct node
{
    int data;
    float days;
    node *next;
};

class graph
{
    node *arr;
    int size;

public:
    graph(int n)
    {
        if (n <= 0)
        { // Input Handling
            cout << "No Cities to traverse.\n";
            exit(0);
        }
        
        size = n;
        arr = new node[n];
        arr[0].days = 0;
        // Adding Nodes and their respective weights for creation of Graph
        for (int i = 0; i < n - 1; i++)
        {
            if (i != 0)
            {
                arr[i].days = arr[i - 1].days + 0.1;
                int temp = static_cast<int>((arr[i].days + 0.000001) * 10);
                if (temp % 10 == 6)
                {
                    arr[i].days = ceil(arr[i - 1].days);
                }
            }
            // Adding Nodes
            arr[i].data = i;
            node *newRoute = new node;
            newRoute->data = i + 1;
            newRoute->next = NULL;
            newRoute->days = (size * 10);
            arr[i].next = newRoute;
        }
        // Last Node
        arr[n - 1].data = n - 1;
        arr[n - 1].next = NULL;
        arr[n - 1].days = arr[n - 2].days + 0.1;
        int temp = static_cast<int>((arr[n - 1].days + 0.000001) * 10);
        if (temp % 10 == 6)
        {
            arr[n - 1].days = ceil(arr[n - 1].days);
        }
    }

    int BFSTraversal()
    {
        bool *visited = new bool[size];
        int *a = new int[size], max = -10000, index = 0;
        for (int i = 0; i < size; i++)
        { // For Keeping Track of Visited Nodes
            visited[i] = false;
            a[i] = 0;
        }

        Queue q;
        if (!visited[0])
        {
            q.push(0);
            visited[0] = true;
            bool destination = false;
            while (!q.empty())
            { // Traverse Until Queue is Empty
                int current = q.peek();
                // Uncomment to see BFS Traversal.....
                // cout << current << " ";
                q.pop();

                if (current == size - 1)
                {
                    destination = true;
                }

                // Pushing New Branches to Queue
                node *temp = arr[current].next;
                while (temp != NULL)
                {
                    if (!visited[temp->data])
                    {
                        if (temp->data > max)
                        {
                            max = temp->data;
                        }
                        q.push(temp->data);
                        visited[temp->data] = true;
                    }
                    temp = temp->next;
                }
                if (a[index - 1] != max)
                    a[index++] = max; // Keeping Track of Solution
                // if (destination) break;
            }
        }
        delete[] visited;
        visited = NULL;
        // cout << endl << endl;
        // BFS TRAVERSAL Solution Activate it to see solution
        // for (int i = 0; i < index; i++) {
        //     if (a[i] != 0) {
        //         cout << a[i] << " ";
        //     }
        // }
        return ceil(arr[size - 1].days);
    }
    void addRoutes(int from, int to)
    {
        if (from < 0 || to < 0)
        { // Input Handling
            cout << "Invalid Routes e.g(-ev).\n";
            return;
        }

        if (from >= size || to >= size)
        {
            // Input Handling
            cout << "Invalid Routes - Enter again (Route should be less than " << size << ").\n";

            do
            {
                cin >> from >> to;

                if (from >= size || to >= size)
                {
                    cout << "Invalid Routes - Enter again (Route should be less than " << size << ").\n";
                }
            } while (from >= size || to >= size);
        }

        node *newRoute = new node;
        newRoute->data = to;
        newRoute->next = NULL;
        node *temp;
        int x;
        if (from == size - 1)
        { // For Last Node connections
            temp = &arr[from];
        }

        else
        {
            temp = arr[from].next;
        }

        while (temp->next != NULL)
        { // Insertion At end
            temp = temp->next;
            // If Aerial Route Already exists Don't create a new one
            if (to == temp->data)
            {
                cout << "Route Already Exists\n";
                return;
            }
        }

        temp->next = newRoute;

        if ((ceil(arr[from].days)) + 1 < arr[to].days)
        { // Update if Minimum weigt is found
            int Temp = static_cast<int>((arr[from].days) * 10);

            if (arr[from].data != 0)
                arr[to].days = ceil(arr[from].days);
            else
                arr[to].days = arr[from].days + 1;

            for (int i = to + 1; i < size; i++)
            {
                if (arr[i - 1].days < arr[i].days)
                {
                    arr[i].days = arr[i - 1].days + 0.1;
                    Temp = static_cast<int>((arr[i].days + 0.00001) * 10);

                    if (Temp % 10 == 6)
                    {
                        arr[i].days = ceil(arr[i - 1].days);
                    }
                }
            }
        }
    }

    void display()
    { // Visualizing Graph
        for (int i = 0; i < size; i++)
        {
            cout << arr[i].data << "|" << arr[i].days << " ::";
            node *temp = arr[i].next;

            while (temp != NULL)
            {
                cout << " - " << temp->data;
                temp = temp->next;
            }
            cout << endl;
            delete temp;
            temp = NULL;
        }
    }

    int floor(float num)
    {
        return static_cast<int>(num);
    }

    int ceil(float num)
    {
        int integerPart = static_cast<int>(num);
        return (num > integerPart) ? (integerPart + 1) : integerPart;
    }

};

int main()
{
    int tests, cities, air, to, from;
    cout << "Tests: ";
    cin >> tests;
    if (tests <= 0)
    { // Input Handling
        cout << "No test Cases to go through.\n";
        return -1;
    }

    while (tests > 0)
    {
        cout << "Cities: ";
        cin >> cities;
        if (air < 0)
        {
            cout << "Invalid Air Route.\n";
            return -1;
        }
        graph G(cities);
        cout << "Air: ";
        cin >> air;
        for (int i = 0; i < air; i++)
        {
            G.display();
            cout << "Routes: ";
            cin >> from >> to;
            // -----------------WHEN the input is like 2 3 4(More than 2) than uncomment this to handle this error
            // if (!(cin >> from >> to) || cin.peek() != '\n') {
            //     cout << "Kindly follow Routes Format(from - to).\n";
            //     // Clear the input buffer
            //     cin.clear();
            //     // Discard the remaining input until a newline
            //     while (cin.get() != '\n') {}
            // }
            G.addRoutes(from, to);
        }
        cout << "Days: ";
        cout << G.BFSTraversal() << endl;
        G.display();
        tests--;
    }

    return 0;
}
