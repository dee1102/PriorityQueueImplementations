#include <iostream>
#include <math.h>
#include <bits/stdc++.h>
#include<thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>
using namespace std;
using namespace std::chrono;


class ThreadPool // cited in the report
{
    public:

    ThreadPool (int threads) : shutdown_ (false)
    {
        // Create the specified number of threads
        threads_.reserve (threads);
        for (int i = 0; i < threads; ++i)
            threads_.emplace_back (std::bind (&ThreadPool::threadEntry, this, i));
    }

    ~ThreadPool ()
    {
        {
            // Unblock any threads and tell them to stop
            std::unique_lock <std::mutex> l (lock_);

            shutdown_ = true;
            condVar_.notify_all();
        }

        // Wait for all threads to stop
        std::cerr << "Joining threads" << std::endl;
        for (auto& thread : threads_)
            thread.join();
    }

    void doJob (std::function <void (void)> func)
    {
        // Place a job on the queu and unblock a thread
        std::unique_lock <std::mutex> l (lock_);

        jobs_.emplace (std::move (func));
        condVar_.notify_one();
    }

    protected:

    void threadEntry (int i)
    {
        std::function <void (void)> job;

        while (1)
        {
            {
                std::unique_lock <std::mutex> l (lock_);

                while (! shutdown_ && jobs_.empty())
                    condVar_.wait (l);

                if (jobs_.empty ())
                {
                    // No jobs to do and we are shutting down
                    std::cerr << "Thread " << i << " terminates" << std::endl;
                    return;
                 }

                std::cerr << "Thread " << i << " does a job" << std::endl;
                job = std::move (jobs_.front ());
                jobs_.pop();
            }

            // Do the job without holding any locks
            job ();
        }

    }

    std::mutex lock_;
    std::condition_variable condVar_;
    bool shutdown_;
    std::queue <std::function <void (void)>> jobs_;
    std::vector <std::thread> threads_;
};

struct PvEB {
    int universeSize;
    int min;
    int max;
    int elementsInEach;
    PvEB *summary;
    PvEB **clusters;

    PvEB(int uSize) {
        
        min = NULL;
        max = NULL;
        elementsInEach = 0;
        universeSize = uSize;
        if (universeSize == 1 || universeSize == 0 ) {
            cout << "universe size must be greater than 1" << std::endl;
            return;
        } else if (universeSize == 2) {
            clusters = NULL;
            summary = new PvEB(int(sqrt(universeSize)));
        } else { // u > 2
            summary = new PvEB(int(sqrt(universeSize)));
            clusters = new PvEB * [int(sqrt(universeSize))];
            for (int i = 0; i < int(sqrt(universeSize)); i++) {
                clusters[i] = new PvEB(int(sqrt(universeSize)));
            }
        }
    }
};


PvEB *newClusterToWorkOn = NULL;
int valueToBeInserted = 0;

int high(int uSize, int value) {
    return int(floor(value/sqrt(uSize)));
};
    
int low(int uSize, int value) {
    return value % int(sqrt(uSize));
};

int min(PvEB *tree) {
    return tree->min;
};

int max(PvEB *tree) {
    return tree->max;
};

bool localInsert(PvEB *tree, int value) {
    tree->elementsInEach++;
    cout << "elements number" << tree->elementsInEach << std::endl;
    if (tree->elementsInEach == 1) {
        tree->min = value;
        tree->max = value;
        return true;
    }
    if (value < tree->min) {
        cout << "value = " << value << " tree min " << tree->min << "  <- preswap"<< std::endl;
        swap(value, tree->min);
        cout << "value = " << value << " tree min " << tree->min << "  <- postswap"<< std::endl;
    } 
    cout << "tree -> usize " << (tree->universeSize > 2) << std::endl;
    
    if(value > tree->max) {
        swap(value, tree->max);
    }
    if (tree->universeSize > 2) {
        
        // cout << "high " << tree->(high(tree->universeSize, value)) << std::endl;
        // cout << printPvEB(tree->(high(tree->universeSize, value))) << "-----------";
        // cout << "min " << min((tree->clusters)[(high(tree->universeSize, value))]) << std::endl;
        // cout << "low " << low(tree->universeSize, value) << std::endl;
        cout << value  << "--->>>>" << std::endl;
        if (!min((tree->clusters)[(high(tree->universeSize, value))])) {
            cout <<  "in it" << std::endl;
            (tree->clusters)[(high(tree->universeSize, value))]->min = low(tree->universeSize, value);
            cout << "min " << min((tree->clusters)[(high(tree->universeSize, value))]) << std::endl;
            newClusterToWorkOn = tree->summary;
            valueToBeInserted = high(tree->universeSize, value);
            cout << "value inserted " << valueToBeInserted << std::endl;
        } else {
            newClusterToWorkOn = (tree->clusters)[(high(tree->universeSize, value))];
            valueToBeInserted = low(tree->universeSize, value);
        }
    }
    return false;
};

bool insert(PvEB *tree, int value) {
    auto start = high_resolution_clock::now();
    bool result = false;
    newClusterToWorkOn = tree;
    valueToBeInserted = value;
    while (!result)
    {   
        result = localInsert(newClusterToWorkOn, value);
    }
    cout << "------------ completed insertion - " << value << std::endl;
    // printPvEB(tree);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    cout << "--------------->Time taken by function: "
         << duration.count() << " microseconds" << endl;
    return true;
};

// int localMin() {

// };

// int extractMin(PvEB *tree) {

// };


void printPvEB(PvEB *tree) {
    if (!tree) {
        cout << "Empty Tree";
    } else {
        cout << "tree: " << tree << std::endl;
        cout << "min: " << tree->min << ", max: " << tree->max << std::endl;
        cout << "uni: " << tree->universeSize << ", uniSqrt: " << sqrt(tree->universeSize) << std::endl;
        cout << "summary: " << tree->summary << std::endl;
        if (tree->universeSize > 2) {
            for (int i = 0; i < int(sqrt(tree->universeSize)); i++) {
                cout << "cluster " << i << tree->clusters[i] <<std::endl;
            }
        } else {
            cout << "cluster " << tree->clusters <<std::endl;
        }
    }
    cout << "------------------------" <<std::endl;
};

int main(int argc, char const *argv[])
{   
    
    int totalRunningThreads = 0;
    int uSize = 16;
    PvEB *tree = new PvEB(uSize);

    ThreadPool t (log2(log2(uSize)));
    for(int i = 0; i < 16; i++) {
    int insertVal = ( std::rand() % ( 15 - 0 + 1 ) );
    cout << insertVal << std::endl;
    t.doJob(std::bind(insert, tree, insertVal));
        
     }
    printPvEB(tree);

    return 0;
}
