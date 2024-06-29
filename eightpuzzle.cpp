#include<bits/stdc++.h>
using namespace std;
class State {
public:
    int puz[3][3];
    int g;
    int f;
    State *par;
    
    State() {
        g = 0;
        f = 0;
        par = nullptr;
    }

    void print_state() {
        for(int i = 0; i < 9; i++) {
            cout << this->puz[i/3][i%3] << " ";
            if(i % 3 == 2) {
                cout << endl;
            }
        }
        cout << endl;
    }

    bool check_state(State k) {
        for(int i = 0; i < 9; i++) {
            if(this->puz[i/3][i%3] != k.puz[i/3][i%3]) {
                return false;
            }
        }
        return true;
    }

   
    int heuristic(State k) {
        int h = 0;
        for(int i = 0; i < 9; i++) {
            if(this->puz[i/3][i%3] != k.puz[i/3][i%3]) {
                h++;
            }
        }
        return h;
    }
    
    void add_child(int oldi, int oldj, int newi, int newj, priority_queue<State, vector<State>, greater<State>> &openlist, vector<State> &closedlist, State b) {
        State *newstate = new State;
        for(int i = 0; i < 9; i++) {
            newstate->puz[i/3][i%3] = this->puz[i/3][i%3];
        }
        swap(newstate->puz[oldi][oldj], newstate->puz[newi][newj]);
        if(!newstate->is_in_list(closedlist)) {
            newstate->g = this->g + 1;
            newstate->f = newstate->g + newstate->heuristic(b);
            newstate->par = this;
            openlist.push(*newstate);
        } else {
            delete newstate;
        }
    }

    void children(priority_queue<State, vector<State>, greater<State>> &open, vector<State> &closed, State b) {
        int p = this->find_pos(0);
        if(p/3 - 1 >= 0) {
            add_child(p/3, p%3, p/3 - 1, p%3, open, closed, b);
        }
        if(p/3 + 1 < 3) {
            add_child(p/3, p%3, p/3 + 1, p%3, open, closed, b);
        }
        if(p%3 - 1 >= 0) {
            add_child(p/3, p%3, p/3, p%3 - 1, open, closed, b);
        }
        if(p%3 + 1 < 3) {
            add_child(p/3, p%3, p/3, p%3 + 1, open, closed, b);
        }
    }

    int find_pos(int x) {
        for(int i = 0; i < 9; i++) {
            if(this->puz[i/3][i%3] == x) {
                return i;
            }
        }
    }

    bool is_in_list(vector<State> list) {
        for(int i = 0; i < list.size(); i++) {
            if(this->check_state(list[i])) {
                return true;
            }
        }
        return false;
    }
};

bool solvable(vector<int> s, vector<int> g) {
    int sinv = 0, ginv = 0;
    for(int i = 0; i < 9; i++) {
        if(s[i] != 0) {
            for(int j = i + 1; j < 9; j++) {
                if(s[j] != 0 && s[i] > s[j]) {
                    sinv++;
                }
            }
        }
    } 
    for(int i = 0; i < 9; i++) {
        if(g[i] != 0) {
            for(int j = i + 1; j < 9; j++) {
                if(g[j] != 0 && g[i] > g[j]) {
                    ginv++;
                }
            }
        }
    }
    return sinv % 2 == ginv % 2;
}

bool operator>(const State& a, const State& b) {
    return a.f > b.f;
}

void solve(State a, State b) {
    priority_queue<State, vector<State>, greater<State>> openlist;
    vector<State> closedlist;
    openlist.push(a);
    State curr;
    while(!openlist.empty()) {
        curr = openlist.top();
        openlist.pop();
        if(curr.check_state(b)) {
            b.g = curr.g;
            b.f = b.g+b.heuristic(b);
            b.par = curr.par;
            cout<<"Number of steps to reach goal state are:"<<b.g<<endl;
            cout<<"Total number of states visited to find goal state are:"<<b.g + closedlist.size()<<endl;
            return;
        } else {
            closedlist.push_back(curr);
            curr.children(openlist, closedlist, b);
        }
    }
}

int main() {
    
    State start, goal;

    vector<int> s_v;
    vector<int> g_v;
    cout<<"Enter Start state 3*3 matrix(0-8):"<<endl;
    for(int i=0;i<9;i++){
        int num;
        cin>>num;
        s_v.push_back(num);
    }
    cout<<"Enter Goal state 3*3 matrix(0-8):"<<endl;
    for(int i=0;i<9;i++){
        int num;
        cin>>num;
        g_v.push_back(num);
    }
    for(int i = 0; i < 9; i++) {
        start.puz[i/3][i%3] = s_v[i];
        goal.puz[i/3][i%3] = g_v[i];
    }
    
    if(solvable(s_v, g_v)) {
        start.f = start.g + start.heuristic(goal);
        solve(start,goal);
    } else {
        cout << "There is no path from start to goal\n";
    }
    return 0;
}
