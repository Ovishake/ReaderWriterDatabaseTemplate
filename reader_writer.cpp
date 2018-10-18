using namespace std;
#include <iostream>
#include <map>
#include <shared_mutex>
#include <string>
#include <thread>

map<string, int> pairing{{"A",1},{"B",2},{"C",3}};

shared_timed_mutex pair_mutex;

void add_to_map(const string& na, int t)
{
    lock_guard<shared_timed_mutex> writerLock(pair_mutex);
    cout<<"beginning to add your map"<<endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    pairing[na] = t;
    cout<<"Update halted"<<endl;
    cout<<"Releasing locks"<<endl;
    
}

void printkey(const string& na)
{
    shared_lock<shared_timed_mutex> readerLock(pair_mutex);
    cout<<na<<":"<<pairing[na]<<endl;
    //return NULL;
}

int main()
{
    thread reader1([]{printkey("A");});
    thread reader2([]{printkey("B");});
    thread writer1([]{add_to_map("D",4);});
    thread writer2([]{add_to_map("E",5);});
    thread reader3([]{printkey("D");});
    thread reader4([]{printkey("E");});
    reader1.join();
    reader2.join();
    reader3.join();
    reader4.join();
    writer1.join();
    writer2.join();
    cout<<endl;
    //printing the whole map
    cout<<"printing the whole map"<<endl;
    for (map<string, int>::iterator it = pairing.begin();
        it != pairing.end();
        ++it)
        {
            cout<< it->first <<"=>" << it->second <<endl;
        }
    return 0;
}