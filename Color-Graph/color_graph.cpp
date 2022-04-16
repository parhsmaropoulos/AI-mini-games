#include <iostream>
#include <list>

// η κλάση του γράφου
class Graph
{
    // αριθμός κορυφών
    int V;
    // λίστα γειτονικών αριθμών
    std::list<int> *adj;
    public:
        // Const/Dest
        Graph(int V) {this->V; adj = new std::list<int>[V];}
        ~Graph()    {delete [] adj;}


        //Προσθήκη κορφής
        void AddEdge(int v, int w);

        //Χρωματισμός
        void Coloring();
};

void Graph::AddEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v);
}

// Χρωματισμός γράφου
void Graph::Coloring()
{
    // Τελικό αποτέλεσμα
    int final[V];
    std::cout << final;

    // Πρώτο χρώμα της πρώτης κορυφής
    final[0] = 0;
    // Αρχικοποίηση των υπόλοιπων κορυφών (-1 σημάινει δεν έχει χρώμα ακόμα)
    for (int v = 1; v < V; v++)
    {
        final[v] = -1 ;
    }
    // Λίστα για να αποθηκεύει τα διαθέσιμα χρώματα.Αν είναι αλήθεια τότε σημάινει ότι το χρώμα είναι σε μία απο τα γειτονικές κορυφές
    bool available[V];

    for (int cl = 0; cl < V ; cl++)
    {
        available[cl] = false;
    }
    // Χρωματισμός των υπόλοιπων κορθφών.
    for(int v = 1; v<V; v++)
    {   
        // Έλεγχος γειτονικών χρωμάτων
        std::list<int>::iterator i;
        for(i=adj[v].begin(); i != adj[v].end(); ++i)
        {
            if(final[*i] != -1)
                available[final[*i]] = true;
        }
        // Εύρεση πρώτου διαθέσιμου χρώματος
        int cl;
        for (cl = 0; cl<V; cl++)
        {
            if(available[cl] == false)
                break;
        }
        
        final[v] = cl;
        // Ανανέωση της λίστα των διαθέσιμων χρωμάτων
        // για την επόμενη επανάληψη.
        for( i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            available[final[*i]] = false;
        }
    }
    std::cout<<"here";
        // print final colours
        for (int k = 0; k <V; k++)
            std::cout <<"Vertex "<< k << " ---> Color " << final[k] << std::endl;
}


int main()
{   
    Graph graph(7);
    graph.AddEdge(0,1);
    graph.AddEdge(0,2);
    graph.AddEdge(0,3);
    graph.AddEdge(0,4);
    graph.AddEdge(0,5);
    graph.AddEdge(0,6);
    std::cout << "Coloring of graph 1 \n"; 
    graph.Coloring();    

    return 0;
}