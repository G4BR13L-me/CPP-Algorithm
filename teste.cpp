#include <bits/stdc++.h>
using namespace std;  

class Grafo 
{ 
    int V;    
    list<pair<int, int>> *adj;     
    public:  
        Grafo(int V) { this->V = V;  adj = new list<pair<int,int>>[V]; } 
        ~Grafo() { delete [] adj; } 

        void addAresta(int u, int v, int peso = 0) 
        { 
            adj[u].push_back(make_pair(v, peso)); 
            adj[v].push_back(make_pair(u, peso)); 
        } 
        void rmvAresta(int u, int v, int p)
        { 
            list<pair<int,int>>::iterator iv = find(adj[u].begin(), adj[u].end(), make_pair(v, p)); 
            iv->first = -1; 
            
            list<pair<int,int>>::iterator iu = find(adj[v].begin(), adj[v].end(), make_pair(u, p)); 
            iu->first = -1;
        }

        void PCC(int start) 
        { 
            if(hasCicloEuleriano())
                printCicloEuleriano(start);
            else {
                cout << "Não Euleriano" << endl;
                emparelhamentoMinimo();
                printCicloEuleriano(start);
            }
            cout << endl;
        }

        bool hasCicloEuleriano() 
        {
            if(V == 0)
                return false;

            for (int v = 0; v < V; v++)
            {
                if(adj[v].size() % 2 != 0)
                    return false;
            }
            
            return true;
        }

        void printCicloEuleriano(int u) 
        {
            list<pair<int,int>>::iterator i;
            int aux; 
            for (i = adj[u].begin(); i != adj[u].end(); ++i) 
            {   
                aux = i->first;
                if ((i->first) != -1 && isArestaValida(u, i->first, i->second)) 
                {
                    cout << u << "-" << aux << " -> ";
                    if(aux == i->first) 
                        rmvAresta(u, i->first, i->second); 
                    printCicloEuleriano(aux); 
                }
            } 
        }

        void emparelhamentoMinimo()
        {
            int dist[V];
            int antecessor[V];

            dijkstra(0, V-1, dist, antecessor);

            vector<int> impar = verticesImpar();
            //percorrer os V de grau impar com heuristica do vizinho mais prox
            vector<pair<int,int>> emp = emparelhar(impar, dist);

            adicionarArestas(emp, dist, antecessor);
        }

        void adicionarArestas(vector<pair<int,int>> e, int dist[], int ant[])
        {
            int aux;
            for (int i = 0; i < e.size(); i++)
            {
                if(e[i].first > e[i].second){
                    aux = e[i].first;
                    while (aux != e[i].second)
                    {
                        addAresta(aux, ant[aux]);
                        aux = ant[aux];
                    }
                }else{
                    aux = e[i].second;
                    while (aux != e[i].first)
                    {
                        addAresta(aux, ant[aux]);
                        aux = ant[aux];
                    }
                }
            }
        }

        vector<int> verticesImpar()
        {
            vector<int> impar;
            for (int v = 0; v < V; v++)
            {
                if(adj[v].size() % 2 != 0)
                    impar.push_back(v);
            }
            return impar;
        }

        vector<pair<int,int>> emparelhar(vector<int>impar, int dist[])
        {
            vector<pair<int,int>> emparelhamento;
            vector<bool> visitado(impar.size(), false);
            int min = INT32_MAX;
            int aux;

            for (int i = 0; i < impar.size(); i++)
            {
                min = INT32_MAX;
                if(!visitado[i]){
                    for (int j = 0; j < impar.size(); j++)
                    {
                        if(j != i && dist[j]<min && !visitado[j]){
                            min = dist[j];
                            aux = j;
                        }
                    }
                    visitado[i] = true;
                    visitado[aux] = true;
                    emparelhamento.push_back(make_pair(impar[i],impar[aux]));
                }
            }
            
            return emparelhamento;
        }

        void dijkstra(int orig, int dest,int dist[], int ant[])
        {
            bool visitados[V];

            priority_queue < pair<int, int>,
                        vector<pair<int, int> >, greater<pair<int, int> > > pq;

            for(int i = 0; i < V; i++)
            {
                dist[i] = 10000000;
                visitados[i] = false;
            }

            dist[orig] = 0;

            pq.push(make_pair(dist[orig], orig));

            while(!pq.empty())
            {
                pair<int, int> p = pq.top();
                int u = p.second;
                pq.pop();

                if(visitados[u] == false)
                {
                    visitados[u] = true;

                    list<pair<int, int> >::iterator it;
                    for(it = adj[u].begin(); it != adj[u].end(); it++)
                    {
                        int v = it->first;
                        int custo_aresta = it->second;

                        if(dist[v] > (dist[u] + custo_aresta))
                        {
                            ant[v] = u;
                            dist[v] = dist[u] + custo_aresta;
                            pq.push(make_pair(dist[v], v));
                        }
                    }
                }
            }
        }
        
        void printGrafo();
    private:
        bool isArestaValida(int u, int v, int peso)
        {  
            // 1) If v is the only adjacent vertex of u 
            int count = 0;  // To store count of adjacent vertices 
            list<pair<int,int>>::iterator i; 
            for (i = adj[u].begin(); i != adj[u].end(); ++i) 
                if ((i->first) != -1) 
                    count++; 
            if (count == 1) 
                return true; 
            
            
            return isPonte(u,v,peso);
        }

        // Checa a soma de vertices antes e depois de remover a aresta
        // Se a soma antes for maior que a depois, retorna false
        bool isPonte(int u, int v, int peso)
        {   
            bool visitado[V]; 
            memset(visitado, false, V); 
            int soma1 = SomaDFS(u, visitado); 
            
            int soma2 = somaRetirandoAresta(u,v,peso,visitado);
            
            return (soma1 > soma2)? false: true; 
        }

        int SomaDFS(int v, bool visitado[])
        { 
            visitado[v] = true; 
            int count = 1; 

            list<pair<int,int>>::iterator i; 
            for (i = adj[v].begin(); i != adj[v].end(); ++i) 
                if ((i->first) != -1 && !visitado[i->first]) 
                    count += SomaDFS(i->first, visitado); 

            return count; 
        }

        int somaRetirandoAresta(int u, int v, int peso, bool visitado[])
        {
            list<pair<int,int>>::iterator iv = find(adj[u].begin(), adj[u].end(), make_pair(v, peso)); 
            iv->first = -1; 
            list<pair<int,int>>::iterator iu = find(adj[v].begin(), adj[v].end(), make_pair(u, peso)); 
            iu->first = -1;

            memset(visitado, false, V); 
            int soma = SomaDFS(u, visitado); 
            
            iv->first = v;
            iu->first = u;
            
            return soma;
        }
}; 
   

void Grafo::printGrafo() 
{
    list<pair<int,int>>::iterator i;
    int v;
    for(int u = 0; u < V; u++){
        cout << "Vertice " << u << ":\n";
        for (i = adj[u].begin(); i != adj[u].end(); i++) 
        { 
            v = i->first; 
            cout << v << " ";   
        }
        cout << "\n";
    }
}

int main() 
{ 
    Grafo g1(4); 
    g1.addAresta(0, 1, 10); 
    g1.addAresta(0, 2, 5); 
    g1.addAresta(1, 2, 6); 
    g1.addAresta(2, 3, 7); 
    g1.PCC(0); 

    Grafo g2(3); 
    g2.addAresta(0, 1, 1); 
    g2.addAresta(1, 2, 2); 
    g2.addAresta(2, 0, 3); 
    g2.PCC(0); 

    Grafo g3(5); 
    g3.addAresta(1, 0, 1); 
    g3.addAresta(0, 2, 2); 
    g3.addAresta(2, 1, 3); 
    g3.addAresta(0, 3, 4); 
    g3.addAresta(3, 4, 5); 
    g3.addAresta(3, 2, 6); 
    g3.addAresta(3, 1, 7); 
    g3.addAresta(2, 4, 8); 
    g3.PCC(0); 

    return 0; 
}