#include "common.hh"

constexpr size_t INVALID = UINT64_MAX;

struct Graph
{
    std::vector<std::string> nodes;
    std::vector<std::vector<size_t>> edges;

    size_t start_node = INVALID;
    size_t end_node = INVALID;

    void add_edge(size_t a, size_t b)
    {
        edges[a].push_back(b);
        edges[b].push_back(a);
    }

    size_t add_node(const std::string& s)
    {
        size_t index = index_of(s);
        if (index != INVALID)
        {
            return index;
        }
        index = nodes.size();
        nodes.push_back(s);
        edges.resize(nodes.size());
        if (s == "start")
        {
            start_node = index;
        }
        else if (s == "end")
        {
            end_node = index;
        }
        return index;
    }

    size_t index_of(const std::string& s) const
    {
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i] == s)
            {
                return i;
            }
        }
        return INVALID;
    }

    bool is_small(size_t i) const
    {
        return std::islower(nodes[i][0]);
    }
};

static Graph parse_input(std::ifstream& input)
{
    Graph graph;
    auto lines = input_to_lines(input);
    for (const auto& line : lines)
    {
        auto s = split(line, "-");
        size_t a = graph.add_node(s[0]);
        size_t b = graph.add_node(s[1]);
        graph.add_edge(a, b);
    }
    return graph;
}

template <typename F>
static uint32_t explore(const Graph& graph, size_t node, std::vector<size_t> visited, const F& predicate)
{
    if (node == graph.end_node)
    {
        return 1;
    }

    if (graph.is_small(node))
    {
        visited[node]++;
    }

    uint32_t total = 0;
    for (size_t next : graph.edges[node])
    {
        if (predicate(next, visited))
        {
            total += explore(graph, next, visited, predicate);
        }
    }
    return total;
}

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    Graph graph = parse_input(input);
    std::vector<size_t> visited;
    visited.resize(graph.nodes.size());
    return explore(graph, graph.start_node, visited,
                   [&](size_t next, const std::vector<size_t>& visited) { return visited[next] == 0; });
}

static bool has_2(const std::vector<size_t>& v)
{
    for (size_t i : v)
    {
        if (i == 2)
        {
            return true;
        }
    }
    return false;
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    Graph graph = parse_input(input);
    std::vector<size_t> visited;
    visited.resize(graph.nodes.size());
    return explore(graph, graph.start_node, visited, [&](size_t next, const std::vector<size_t>& visited) {
        return visited[next] == 0 || (visited[next] == 1 && next != graph.start_node && !has_2(visited));
    });
}

void Day12::run()
{
    std::cout << "Day12:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day12");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day12");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day12::sanity_check()
{
    uint32_t step1_expected = 10;
    uint32_t step2_expected = 36;
    bool ret = true;

    uint32_t step1_got = step1("../input/day12_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day12_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
