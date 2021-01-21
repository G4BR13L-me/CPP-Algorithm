#include <bits/stdc++.h>
using namespace std;

void addAresta(vector <pair<int, int>> g[], int u, 
									int v, int wt = 0) 
{ 
	g[u].push_back(make_pair(v, wt)); 
	g[v].push_back(make_pair(u, wt)); 
}

void printGrafo(vector<pair<int,int>> g[]) 
{ 
	int v, w; 
	for (int u = 0; u < g->size(); u++) 
	{ 
		cout << "Arestas vertice " << u << ": \n"; 
		for (auto it = g[u].begin(); it!=g[u].end(); it++) 
		{ 
			v = it->first; 
			w = it->second; 
			cout << "\tAresta para " << v << " com peso = "
				<< w << "\n"; 
		} 
		cout << "\n"; 
	} 
} 

void PCC(vector<pair<int,int>> g[], int start) 
{ 
	if(hasCicloEuleriano(g))
        printCicloEuleriano(g,start);
    else {
        emparelhamentoMinimo(g);
        printCicloEuleriano(g,start);
    } 
}

bool hasCicloEuleriano(vector<pair<int,int>> g[]) 
{
    if(g->size() == 0)
        return false;

    for (int v = 0; v < g->size(); v++)
    {
        if(g[v].size() % 2 != 0)
            return false;
    }
    
    return true;
}


void printCicloEuleriano(vector<pair<int,int>> g[], int u) 
{  
    for (auto i = g[u].begin(); i != g[u].end(); ++i) 
    {  
        // If edge u-v is not removed and it's a a valid next edge 
        if (i->second != -1 && isArestaValida(g, u, i->first)) 
        { 
            cout << u << "-" << i->first << " "; 
            rmvAresta(g, u, i->first); 
            printCicloEuleriano(g,i->first); 
        } 
    } 
}

void emparelhamentoMinimo(vector<pair<int,int>> g[]) 
{  
    
}

bool isArestaValida(vector<pair<int,int>> g[], int u, int v) 
{ 
    // The edge u-v is valid in one of the following two cases: 

    // 1) If v is the only adjacent vertex of u 
    int count = 0; // To store count of adjacent vertices 

    for (auto i = g[u].begin(); i != g[u].end(); ++i) 
        if (i->second != -1) 
            count++; 
    if (count == 1) 
        return true; 


    // 2) If there are multiple adjacents, then u-v is not a bridge 
    // Do following steps to check if u-v is a bridge 

    // 2.a) count of vertices reachable from u 
    bool visitado[g->size()]; 
    memset(visitado, false, g->size()); 
    int count1 = DFSSomar(g, u, visitado); 

    // 2.b) Remove edge (u, v) and after removing the edge, count 
    // vertices reachable from u 
    rmvAresta(g, u, v); 
    memset(visitado, false, g->size()); 
    int count2 = DFSSomar(g, u, visitado); 

    // 2.c) Add the edge back to the graph 
    addAresta(g, u, v); 

    // 2.d) If count1 is greater, then edge (u, v) is a bridge 
    return (count1 > count2)? false: true; 
} 

// This function removes edge u-v from graph. It removes the edge by 
// replacing adjcent vertex value with -1. 
void rmvAresta(vector<pair<int,int>> g[], int u, int v) 
{
    g[u][v].second = -1;
    g[v][u].second = -1;
} 

// A DFS based function to count reachable vertices from v 
int DFSSomar(vector<pair<int,int>> g[], int v, bool visitado[]) 
{ 
    // Mark the current node as visitado 
    visitado[v] = true; 
    int count = 1; 

    // Recur for all vertices adjacent to this vertex  
    for (auto i = g[v].begin(); i != g[v].end(); ++i) 
        if (i->second != -1 && !visitado[i->first]) 
            count += DFSSomar(g, i->first, visitado); 

    return count; 
}

int main() {
    
    vector<pair<int,int>> g1[4]; 
    addAresta(g1,0, 1); 
    addAresta(g1,0, 2); 
    addAresta(g1,1, 2); 
    addAresta(g1,2, 3);
    PCC(g1,0); 

    vector<pair<int,int>> g2[3];  
    addAresta(g2,0, 1); 
    addAresta(g2,1, 2); 
    addAresta(g2,2, 0);
    PCC(g2,0); 

    vector<pair<int,int>> g3[5];  
    addAresta(g3,1, 0); 
    addAresta(g3,0, 2); 
    addAresta(g3,2, 1); 
    addAresta(g3,0, 3); 
    addAresta(g3,3, 4); 
    addAresta(g3,3, 2); 
    addAresta(g3,3, 1); 
    addAresta(g3,2, 4);
    PCC(g3,0); 
    
    return 0;
} 