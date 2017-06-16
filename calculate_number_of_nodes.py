#coding=utf-8
"""计算每个子图的节点数目，并按照节点数来随机生成其忙、闲时的概率"""
if __name__ == "__main__":
    name = 'j60_1_sub_topos'
    with open(name+'.txt', 'r') as f:
        content = f.readlines()
    sub_topos = []
    sums = []
    busy_prob = []
    leisure_prob = []
    sum_of_all = 0.0
    leisure_sum_of_all = 0.0
    for c in content:
        sub_topos.append([int(i) for i in c.split()])
        sums.append(sum(sub_topos[-1]))
        sum_of_all += sums[-1]
    print "sum of all sub graph：{}".format(sum_of_all)
    for i in range(len(sums)):
        busy_prob.append(sums[i] / float(sum_of_all))
        leisure_sum_of_all += (1.0 / sums[i])
        # print busy_prob[i]
    for i in range(len(sums)):
        leisure_prob.append((1.0 / sums[i]) / leisure_sum_of_all)
        # print "sum:{}, busy prob:{}, leisure prob:{}".format(sums[i], busy_prob[i], leisure_prob[i])
        # print leisure_prob[i]
    with open(name+'_prob.txt', 'w+') as f:
        f.write('busy:\n')
        for i in busy_prob:
            f.write(str(i) + '\n')
        f.write('leisure:\n')
        for i in leisure_prob:
            f.write(str(i) + '\n')
