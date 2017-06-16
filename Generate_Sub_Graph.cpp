#include "Generate_Sub_Graph.h"


int main() {
	struct node *nodes = NULL;
	int nodes_length;
	//printf("Everything is ok! %lf\n", u01());
	struct node *start = NULL, *end = NULL;
	start = (struct node *)malloc(sizeof(struct node));
	memset(start, 0, sizeof(struct node));
	end = (struct node *)malloc(sizeof(struct node));
	memset(end, 0, sizeof(struct node));
	read_data("j30_1.txt", &nodes, &nodes_length, start, end);
	//printf("start node succ: %d, end node pred: %d \n", start->num_succ, end->num_pred);
	// 初始化一个number of nodes + 2 的vector（包括另外添加的初始节点和结束节点的int数组： 1|0）
	std::vector<int> sub_array(nodes_length + 2, 0);
	std::vector<int> status(nodes_length + 2, 0); // 是否已经加入到队列中

	std::vector<std::vector<int>> sub_topos;
	std::queue<struct node *> q;
	q.push(start);
	status[start->id] = 1;
	generate_sub_graphy(sub_array, &sub_topos, q, status);
	//std::cout << sub_array[0] << std::endl;
	std::cout <<"本次生成"<< sub_topos.size() << "个子图,如下所示:" << std::endl;
	for (uint16_t i = 0; i < sub_topos.size(); i++) {
		//std::cout << "子图" << i << "：";
		for (uint16_t j = 0; j < nodes_length; j++) {
			printf("%d ", sub_topos[i][j]);
		}
		std::cout << std::endl;
	}
	return 0;
}

void generate_sub_graphy(std::vector<int> sub_array, std::vector<std::vector<int>> *sub_topos, std::queue<struct node *> q, std::vector<int> status) {
	//(*sub_array)[0] = 1;
	if (q.empty()) {
		(*sub_topos).push_back(sub_array);
		return;
	}
	struct node *cur = q.front();
	sub_array[cur->id] = 1;
	q.pop();
	if (cur->num_succ > 1) {
		double prob = u01();
		// 大于P则将该分支结构转为OR结构
		if (prob > P) {
			std::cout << "id：" << cur->id << "转换为OR结构" << std::endl;
			for (int i = 0; i < cur->num_succ; i++) {
				if (1 == status[cur->succ[i]->id])
					continue;
				q.push(cur->succ[i]);
				status[cur->succ[i]->id] = 1;
				generate_sub_graphy(sub_array, sub_topos, q, status);
				q.pop();
			}
		}
		//否则为AND结构，即所有都加入队列
		else {
			for (int i = 0; i < cur->num_succ; i++) {
				if (1 == status[cur->succ[i]->id])
					continue;
				q.push(cur->succ[i]);
				status[cur->succ[i]->id] = 1;
			}
			generate_sub_graphy(sub_array, sub_topos, q, status);
		}
	}
	else if (cur->num_succ == 1 && 1 != status[cur->succ[0]->id]) {
		q.push(cur->succ[0]);
		status[cur->succ[0]->id] = 1;
		generate_sub_graphy(sub_array, sub_topos, q, status);
	}
	else {
		generate_sub_graphy(sub_array, sub_topos, q, status);
	}
}

void read_data(char *file_name, struct node **nodes_output, int *num_nodes, struct node *start, struct node *end)
{
	FILE *fp;
	int num_node, num_edges, i, temp, current, next;
	void *new_ptr = NULL;
	struct node *nodes;

	fp = fopen(file_name, "r");
	if (fp == NULL) {
		fprintf(stderr, "make sure the file exist");
		exit(1);
	}
	fscanf(fp, "%d", &num_node);
	fscanf(fp, "%d", &num_edges);
	*num_nodes = num_node;
	nodes = (struct node *)malloc(sizeof(struct node) * num_node);
	memset(nodes, 0, sizeof(struct node) * num_node);
	for (i = 0; i < num_node; i++) {
		nodes[i].id = i;
		nodes[i].succ = (struct node**)malloc(1 * sizeof(struct node*));
		nodes[i].pred = (struct node**)malloc(1 * sizeof(struct node*));
		nodes[i].num_pred = 0;
		nodes[i].num_succ = 0;
		nodes[i].pred_len = 1;
		nodes[i].succ_len = 1;
	}
	for (i = 0; i < num_edges; i++) {
		fscanf(fp, "%d %d %d", &temp, &current, &next);
		// 检查后继是否越界，如果是则realloc内存,为了方便每次扩大两倍 应该能够满足需求
		while (nodes[current - 1].succ_len <= nodes[current - 1].num_succ) {
			nodes[current - 1].succ_len *= 2;
			new_ptr = realloc(nodes[current - 1].succ, sizeof(struct node *) * (nodes[current - 1].succ_len));
			if (!new_ptr) {
				fprintf(stderr, "succ realloc error!");
				exit(-1);
			}
			nodes[current - 1].succ = (struct node **)new_ptr;
		}
		nodes[current - 1].succ[nodes[current - 1].num_succ] = &nodes[next - 1];
		nodes[current - 1].num_succ++;

		// 检查前驱是否越界，如果是则realloc内存,为了方便每次扩大两倍 应该能够满足需求
		while (nodes[next - 1].pred_len <= nodes[next - 1].num_pred) {
			nodes[next - 1].pred_len *= 2;
			new_ptr = realloc(nodes[next - 1].pred, sizeof(struct node *) * nodes[next - 1].pred_len);
			if (!new_ptr) {
				fprintf(stderr, "pred realloc error!");
				exit(-1);
			}
			nodes[next - 1].pred = (struct node **)new_ptr;
		}
		nodes[next - 1].pred[nodes[next - 1].num_pred] = &nodes[current - 1];
		nodes[next - 1].num_pred++;
	}

	*nodes_output = nodes;
	start->id = *num_nodes;
	end->id = *num_nodes + 1;
	start->succ_len = 1;
	start->succ = (struct node**)malloc(sizeof(struct node*) * start->succ_len);
	end->pred_len = 1;
	end->pred = (struct node**)malloc(sizeof(struct node*) * end->pred_len);
	void *tmp = NULL;
	for (int i = 0; i < *num_nodes; i++) {
		if (!nodes[i].num_pred) {
			nodes[i].num_pred = 1;
			nodes[i].pred_len = 1;
			nodes[i].pred = (struct node**)malloc(sizeof(struct node*));
			nodes[i].pred[0] = start;
			start->succ[(start->num_succ)++] = nodes + i;
			while (start->num_succ >= start->succ_len) {
				(start->succ_len) *= 2;
				tmp = realloc(start->succ, sizeof(struct node *) * start->succ_len);
				if (!tmp) {
					fprintf(stderr, "succ realloc error!");
					exit(-1);
				}
				start->succ = (struct node **)tmp;
				tmp = NULL;
			}
		}
		if (!nodes[i].num_succ) {
			nodes[i].num_succ = 1;
			nodes[i].succ_len = 1;
			nodes[i].succ = (struct node**)malloc(sizeof(struct node*));
			nodes[i].succ[0] = end;
			end->pred[(end->num_pred)++] = nodes + i;
			while (end->num_pred >= end->pred_len) {
				(end->pred_len) *= 2;
				tmp = realloc(end->pred, sizeof(struct node *) * end->pred_len);
				if (!tmp) {
					fprintf(stderr, "succ realloc error!");
					exit(-1);
				}
				end->pred = (struct node **)tmp;
				tmp = NULL;
			}
		}
	}
	fclose(fp);
}
