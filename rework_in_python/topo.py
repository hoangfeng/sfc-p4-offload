import time

# denote 2 as VMP4S
# denote 1 as PP4S
VMP4S = 2
PP4S = 1


class Color:
    PURPLE = '\033[95m'
    CYAN = '\033[96m'
    DARKCYAN = '\033[36m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    END = '\033[0m'


def read_node_from_file(filename):
    f = open(filename, "r")
    nodelist = []
    for line in f:
        element = line.split()
        index = element[0]
        element = element[1:]
        function = set()
        for e in element:
            function.add(int(e))
        nodelist.append(Node(index, function))
    return nodelist


def read_link_from_file(filename):
    f = open(filename, "r")
    linklist = []
    for line in f:
        ele = line.split()
        ele = ele[1:]
        linklist.append(Link(ele[0], ele[1], ele[2]))
    return linklist


def link_to_dict(links):
    link_dict = {}
    for i in links:
        if i.node_start in link_dict:
            link_dict[i.node_start].append([i. node_end, float(i.weight)])
        else:
            link_dict[i.node_start] = [(i.node_end, float(i.weight))]
    return link_dict


def node_to_dict(node):
    dic = {}
    for i in node:
        dic[i.uid] = i.function
    return dic


# print a list to screen
def print_list(lst):
    temp = "["
    for i in lst:
        temp += str(i)
        temp += ', '
    temp = temp[:-2]
    temp += ']'
    print(Color.PURPLE + temp + Color.END)


class MeasureDuration:
    def __init__(self):
        self.start = None
        self.end = None

    def __enter__(self):
        self.start = time.time()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.end = time.time()
        print("Total time taken: %s" % self.duration())

    def duration(self):
        return str((self.end - self.start) * 1000) + ' milliseconds'


class Node:
    uid = 0
    function = {}
    capacity = 0
    cost = 0
    remain = 0
    # if not p4 isp4 = 0
    # if vmp4 == 2
    # if ppp4 == 1
    isP4 = 0
    # reuse for dijkstra
    previous = 0
    # shortest from start
    sfs = 0

    def __init__(self, uid, function):
        self.uid = uid
        self.function = function

    def view(self):
        temp = 'Id = ' + str(self.uid) + "\t Function = "
        for i in self.function:
            temp += str(i) + ', '
        temp = temp[:-2]
        return temp


class Link:
    node_start = ''
    node_end = ''
    weight = 0
    capacity = 0
    remain = 0
    cost = 0

    def __init__(self, node_start, node_end, weight):
        self.node_start = node_start
        self.node_end = node_end
        self.weight = weight

    def view(self):
        temp = 'Start = ' + str(self.node_start) + "\t"
        temp += 'End = ' + str(self.node_end) + "\t"
        temp += 'Weight = ' + str(self.weight) + "\t"
        return temp


class Topo:
    link = []
    node = []
    link_dic = {}
    node_dic = {}

    def __init__(self, node, link):
        self.link = link
        self.node = node
        self.link_dic = link_to_dict(link)
        self.node_dic = node_to_dict(node)

    def view(self):
        print(len(self.link))
        print(len(self.node))

    @staticmethod
    def shortest_path(topo, start, end):
        # create dict present dijkstra table
        dijkstra_t = {}
        unvisited = []
        visited = []
        links = link_to_dict(topo.link)
        current_node = str(start)
        for n in topo.node:
            unvisited.append(str(n.uid))
            if n.uid == str(start):
                dijkstra_t[str(n.uid)] = [0, '', False]
            else:
                dijkstra_t[str(n.uid)] = [float("inf"), '', False]
        while len(unvisited) != 0:
            visited.append(current_node)
            unvisited.remove(current_node)
            dijkstra_t[current_node][2] = True
            if current_node in links:
                connection = links[current_node]
                for i in connection:
                    if i[0] not in visited:
                        weight = i[1] + dijkstra_t[current_node][0]
                        if weight < dijkstra_t[i[0]][0]:
                            dijkstra_t[i[0]][0] = weight
                            dijkstra_t[i[0]][1] = current_node
            temp = 0
            min_node = 0
            for i in dijkstra_t:
                if dijkstra_t[i][2] is False:
                    if temp == 0:
                        temp = dijkstra_t[i][0]
                        min_node = i
                    elif dijkstra_t[i][0] != -1 and dijkstra_t[i][0] < temp:
                        temp = dijkstra_t[i][0]
                        min_node = i
            current_node = min_node
        getpath = str(end)
        result = []
        while getpath != str(start):
            result.append(getpath)
            getpath = dijkstra_t[str(getpath)][1]
        result.append(str(start))
        return reversed(result), dijkstra_t[str(end)][0]

    @staticmethod
    def get_shortest_path_table(topo, start):
        # create dict present dijkstra table
        dijkstra_t = {}
        unvisited = []
        visited = []
        links = link_to_dict(topo.link)
        current_node = str(start)
        for n in topo.node:
            unvisited.append(str(n.uid))
            if n.uid == str(start):
                dijkstra_t[str(n.uid)] = [0, '', False]
            else:
                dijkstra_t[str(n.uid)] = [float("inf"), '', False]
        while len(unvisited) != 0:
            visited.append(current_node)
            unvisited.remove(current_node)
            dijkstra_t[current_node][2] = True
            if current_node in links:
                connection = links[current_node]
                for i in connection:
                    if i[0] not in visited:
                        weight = i[1] + dijkstra_t[current_node][0]
                        if weight < dijkstra_t[i[0]][0]:
                            dijkstra_t[i[0]][0] = weight
                            dijkstra_t[i[0]][1] = current_node
            temp = 0
            min_node = 0
            for i in dijkstra_t:
                if dijkstra_t[i][2] is False:
                    if temp == 0:
                        temp = dijkstra_t[i][0]
                        min_node = i
                    elif dijkstra_t[i][0] != -1 and dijkstra_t[i][0] < temp:
                        temp = dijkstra_t[i][0]
                        min_node = i
            current_node = min_node
        a = dijkstra_t['469']
        return dijkstra_t


class Algo:
    start = 0
    end = 0
    topo = Topo
    sfc = []
    topo_clone = Topo
    topo_propose = Topo

    def __init__(self, start, end, topo, sfc):
        self.start = start
        self.end = end
        self.topo = topo
        self.sfc = sfc
        self.topo_propose = topo

    # method base on Shortest Path and Maximum Flow Problems Under Service Function Chaining Constraints
    # https://arxiv.org/abs/1801.05795
    @staticmethod
    def merge_graph(lst):
        new_lst = []
        for i in lst:
            new_lst.append(str(i[:-2]))
        return new_lst

    def compare_method(self):
        c_start, c_end = self.construct_cm_graph()
        result, weight = self.topo_clone.shortest_path(self.topo_clone, c_start, c_end)
        lst = self.merge_graph(result)
        print(Color.GREEN + "***  Compared method  ***" +Color.END)
        print_list(lst)
        print("Weight = " + str(weight))
        return lst, weight

    def construct_cm_graph(self):
        # init
        for i in self.topo.node:
            for j in range(0, len(self.sfc) + 1):
                self.topo_clone.node.append(Node(str(i.uid) + "_" + str(j), i.function))
        node_temp = node_to_dict(self.topo_clone.node)
        # create new graph
        for i in self.topo.link:
            weight = i.weight
            for j in range(0, len(self.sfc)+1):
                start = i.node_start+"_" + str(j)
                k_index={}
                for k in range(0, len(self.sfc)+1):
                    end = i.node_end + "_" + str(k)
                    if k >= j:
                        k_set = set(self.sfc[j:k])
                        if k_set.issubset(set(node_temp[end])):
                            index=len(k_set)
                            k_index[end] = index
                end = max(k_index, key = k_index.get)
                self.topo_clone.link.append(Link(start, end, weight))
        # pruning
        prune_dict = {}
        for i in self.topo_clone.link:
            if i.node_start not in prune_dict:
                prune_dict[i.node_start] = [1, 0]
            else:
                prune_dict[i.node_start][0] = prune_dict[i.node_start][0] + 1
            if i.node_end not in prune_dict:
                prune_dict[i.node_end] = [0, 1]
            else:
                prune_dict[i.node_end][1] = prune_dict[i.node_end][1] + 1
        node_start = str(self.start) + '_0'
        node_end = str(self.end) + '_' + str(len(self.sfc))
        for i in prune_dict:
            if i.strip() != node_start and i.strip() != node_end:
                if prune_dict[i][1] == 0 or prune_dict[i][0] == 0:
                    self.topo_clone.link = [x for x in self.topo_clone.link if not x.node_end == i or x.node_start == i]
                    self.topo_clone.node = [x for x in self.topo_clone.node if not x.uid == i]
        return node_start, node_end

    def sfc_workable_node_dic(self):
        sfc_dic = {}
        for i in self.sfc:
            for j in self.topo.node:
                # check if node have link
                if j.uid in self.topo.link_dic:
                    for k in j.function:
                        if k == i:
                            if i not in sfc_dic:
                                sfc_dic[i] = [j]
                            else:
                                sfc_dic[i].append(j)
        return sfc_dic

    def sfc_workable_node_p4_dic(self):
        # Get all the node function base on SFC
        # Treat P4 switch as single node in SFC
        sfc_dic = {}
        for i in self.sfc:
            for j in self.topo.node:
                # check if node have link
                if j.uid in self.topo.link_dic:
                    for k in j.function:
                        if k == i or k == 1 or k == 2:
                            if k == 1:
                                j.isP4 = 1
                            elif k == 2:
                                j.isP4 = 2
                            if i not in sfc_dic:
                                sfc_dic[i] = [j]
                            else:
                                sfc_dic[i].append(j)
        return sfc_dic

    @staticmethod
    def isP4(self, node_uid, current_func):
        te = self.topo.node_dic[node_uid]
        type_node = 0
        have_current_func = False
        for i in te:
            if type_node == 0:
                if i == 1:
                    type_node = 1
                elif i == 2:
                    type_node = 2
            else:
                if i == 2:
                    type_node = 2
            if i == current_func:
                have_current_func = True
        return type_node, have_current_func

    def layered_graph(self):
        print(Color.GREEN + "***** Layered Graph *****" + Color.END)
        # Get all the node function base on SFC
        sfc_dic = self.sfc_workable_node_dic()
        path = []
        weight = 0
        # find shortest path in each layer graph
        d_table = self.topo.get_shortest_path_table(self.topo, self.start)
        start = self.start
        nearest_node = 0
        nearest_distance = 0
        for i in self.sfc:
            for j in sfc_dic[i]:
                if nearest_distance == 0:
                    nearest_distance = d_table[j.uid][0]
                    nearest_node = j.uid
                elif nearest_distance > d_table[j.uid][0]:
                    nearest_distance = d_table[j.uid][0]
                    nearest_node = j.uid
            weight += d_table[nearest_node][0]
            getpath = str(nearest_node)
            result = []
            while getpath != str(start):
                result.append(getpath)
                getpath = d_table[str(getpath)][1]
            if not path: result.append(str(start))
            for k in reversed(result):
                path.append(k)
            start = nearest_node
            d_table = self.topo.get_shortest_path_table(self.topo, nearest_node)
            if i != self.sfc[len(self.sfc) - 1]:
                nearest_distance = float('inf')
        # find shortest path in last sfc to end
        d_table = self.topo.get_shortest_path_table(self.topo, path[len(path)-1])
        weight += d_table[str(self.end)][0]
        getpath = str(self.end)
        result = []
        while getpath != str(nearest_node):
            result.append(getpath)
            getpath = d_table[str(getpath)][1]
        for i in reversed(result):
            path.append(i)
        print_list(path)
        print("Weight = " + str(weight))
        return path, weight

    # my proposed method
    def proposed_method(self):
        print(Color.GREEN + "####  My proposed method ####"+ Color.END)
        # Get all the node function base on SFC
        # Treat P4 switch as single node in SFC
        sfc_dic = self.sfc_workable_node_p4_dic()
        path = []
        weight = 0
        # find shortest path in each layer graph
        d_table = self.topo.get_shortest_path_table(self.topo, self.start)
        start = self.start
        nearest_node = 0
        nearest_distance = 0
        # same weight but have more desire than others
        skip = 0
        for i in self.sfc:
            if skip != 0:
                skip = skip - 1
            else:
                for j in sfc_dic[i]:
                    if j.uid != start:
                        if nearest_distance == 0:
                            nearest_distance = d_table[j.uid][0]
                            nearest_node = j.uid
                        elif nearest_distance > d_table[j.uid][0]:
                            nearest_distance = d_table[j.uid][0]
                            nearest_node = j.uid
                node_type, current_func = self.isP4(self, nearest_node, i)
                # Check dead-end
                de_check = self.topo.get_shortest_path_table(self.topo, nearest_node)
                if de_check[str(self.end)][0] != float("inf"):
                    if current_func:
                        skip = node_type
                    else:
                        skip = node_type - 1
                    weight += d_table[nearest_node][0]
                    getpath = str(nearest_node)
                    result = []
                    while getpath != str(start):
                        result.append(getpath)
                        getpath = d_table[str(getpath)][1]
                    if not path:
                        result.append(str(start))
                    for k in reversed(result):
                        path.append(k)
                    start = nearest_node
                    # able to choose dead end node
                    d_table = de_check
                else:
                    print('Dead-End detected')
            if i != self.sfc[len(self.sfc) - 1]:
                nearest_distance = float('inf')
        # find shortest path in last sfc to end
        d_table = self.topo.get_shortest_path_table(self.topo, path[len(path) - 1])
        weight += d_table[str(self.end)][0]
        getpath = str(self.end)
        result = []
        while getpath != str(nearest_node):
            result.append(getpath)
            getpath = d_table[str(getpath)][1]
        # result.append(str(nearest_node))
        for i in reversed(result):
            path.append(i)
        print_list(path)
        print("Weight = " + str(weight))

        return path, weight


class Test:
    @staticmethod
    def test_path(topo, test, sfc):
        sfc_index = 0
        result = ''
        a = topo.node_dic['301']
        colorP4 = 0
        for i in test:
            for j in topo.node_dic[i]:
                if j in sfc and j == sfc[sfc_index]:
                    result += '('
                    result += Color.RED + str(sfc[sfc_index]) + Color.END
                    if sfc_index < len(sfc)-1:
                        sfc_index = sfc_index + 1
                    result += ')'
                elif j == 1 or j == 2:
                    colorP4 = 1
            if colorP4 == 1:
                result += Color.YELLOW + str(i) + Color.END + '; '
                colorP4 = 0
            else:
                result += str(i) + '; '
        print(result)
        pass

    @staticmethod
    def test_path_all(topo, test, sfc):
        sfc_index = 0
        result = ''
        for i in test:
            # if node_dic[i]:
            result += '('
            for j in topo.node_dic[i]:
                if j in sfc and j == sfc[sfc_index]:
                    result += Color.RED + str(sfc[sfc_index]) + Color.END + ', '
                    if sfc_index < len(sfc)-1:
                        sfc_index = sfc_index + 1
                elif j == 1 or j == 2:
                    result += Color.YELLOW + str(j) + Color.END + ', '
                else:
                    result += str(j) + ', '
            result = result[:-2]
            if topo.node_dic[i]:
                result += ')'
            result += ' ' + str(i) + '; '
        print(result)

    @staticmethod
    def stress_test(topo):
        pass


def main():
    # DeadEnd Case
    # start = 575
    # end = 467
    # sfc = [11, 5, 29, 8]
    #
    # start = 246
    # end = 1
    # sfc = [11, 6, 5, 8, 13]
    #
    start = 301
    end = 561
    sfc = [3, 27, 20, 19, 4]

    node = read_node_from_file("../function.txt")
    link = read_link_from_file("../output.txt")
    topo = Topo(node, link)
    t = Test
    # Run dijkstra
    # with MeasureDuration() as m:
    #     r, w = topo.shortest_path(topo, start, end)
    #     print("***  Dijkstra run  ***")
    #     print_list(r)
    #     print("Weight = " + str(w))
    print_list(sfc)
    print('Start = ' + str(start) + '  End = ' + str(end))
    print('')
    al = Algo(start, end, topo, sfc)
    with MeasureDuration() as m:
        p, v = al.compare_method()
    print('')
    with MeasureDuration() as m:
        p1, v1 = al.layered_graph()
    print('')
    with MeasureDuration() as m:
        p2, w2 = al.proposed_method()

    print("")
    t.test_path(topo, p, sfc)
    t.test_path(topo, p1, sfc)
    t.test_path(topo, p2, sfc)
    # t.test_path_all(topo, p2, sfc)


if __name__ == "__main__":
    main()


