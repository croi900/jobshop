#ifndef SCHEDULER_HGUARD
#define SCHEDULER_HGUARD

#include "job.hpp"

#include <vector>
#include <unordered_map>
#include <sstream>

typedef std::vector<std::vector<size_t>> Adjacency;
typedef std::vector<std::vector<float>> Matrix;
typedef std::vector<Adjacency> JobDataframe;
typedef std::unordered_map<size_t,size_t> IDMap;
typedef struct{float time; float dur; size_t pid;} Operation;
typedef std::vector<std::deque<Operation>> QueueVec; 
typedef std::vector<std::deque<size_t>> InsertionQueue; 
typedef std::vector<size_t> Visited;

class Scheduler
{
private:
    void create_dataframe();
    void build_id_map();
    void build_time_matrix();
    void build_machine_queue();
    void matrix_intersection_sort();
    void dfs_assign(size_t id, Visited& vis, Adjacency& adj,float t,size_t pid);
    void dfs_intersection();
    void dfs_intersection_sort();
    void optimize_queue();
    void build_insertion_queue();

    IDMap sidm; // Machine Real ID to Scheduler ID
    IDMap ridm; // Machine Scheduler ID to Real ID

    IDMap sidp; // Part Real ID to Scheduler ID
    IDMap ridp; // Part Scheduler ID to Real ID

    JobDataframe dataframe;
    Matrix T; // (1) 
    Matrix T2; // (3) experimental, no desc yet 

    float time; // global schedule time

    QueueVec Q; // (2)
    InsertionQueue Qs; // (4)

public:
    Job& job;

    std::string as_string();
    std::string schedule_queue_as_parsable();

    Scheduler(Job& job);
    ~Scheduler();
};

/*
(1) -> T is a matrix containing the total times of each
part subprocess, for exaple for Input_Two we would have

        m_1      m_2      m_3     ...     m_n
p_1   p_11+c_1 p_12+c_2 p_13+c_3  ...   p_1n+c_n
p_2   p_21+c_1          ...             p_2n+c_n
...      ...                              ...
p_m   p_m1+c_1          ...             p_mn+c_n

where 
    - p_ij is the time it takes for subprocess j
of process for part i to finish
    - c_i is the cooldown of machine i

(2) -> Q is a vector of queues that represent 
the the time a machine will process a part, how
long it will take and what part it's processing.

(4) -> Qs is a vector of sorted queues representing
the order of insertion for each "first machine" of
a part based on shortest time on that machine.

*/

#endif