#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#define P 0.7

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <queue>
#include <algorithm>

#include <limits.h>

boost::mt19937 rng((unsigned)time(0));
boost::uniform_01<boost::mt19937&> u01(rng);

struct node {
	struct node **succ;
	struct node **pred;
	int num_succ = 0;
	int num_pred = 0;
	int pred_len, succ_len;  //数组长度
	int id;
};

void read_data(char *file_name, struct node **nodes_output, int *num_nodes, struct node *start, struct node *end);
void generate_sub_graphy(std::vector<int> sub_array, std::vector<std::vector<int>> *sub_topos, std::queue<struct node *> q, std::vector<int> status);